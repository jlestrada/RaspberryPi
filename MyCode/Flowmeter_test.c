/*Code to interface with a Hall effect flow meter using
  the Raspberry Pi. Intended to figure out the number of 
  ticks per ounce

  Author:Jose Estrada
  Date:4/18/15
*/
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define flowMeter1 4
#define flowMeter2 5 
#define flowMeter3 6
#define flowMeter4 7

//GPIO pin setup
#define motor1 0
#define motor2 1
#define motor3 2
#define motor4 3

static volatile int globalCounter[4];	//global counter for all four meters

//interput functions to increment flow meter counts
//functions must be "free" for wiringPiISR function to work
void myInterrupt0(){++globalCounter[0];}
void myInterrupt1(){++globalCounter[1];}
void myInterrupt2(){++globalCounter[2];}
void myInterrupt3(){++globalCounter[3];}

int main(void){
	int pin,gotOne,fd,x,i=0;
	int myCounter[4];
	char data[5];

	wiringPiSetup();

	//GPIO pin setup
	pinMode(motor1,OUTPUT);		//set pin 0 to control motor1
	pinMode(motor2,OUTPUT);		//set pin 1 to control motor2
	pinMode(motor3,OUTPUT);		//set pin 2 to control motor3
	pinMode(motor4,OUTPUT);		//set pin 3 to control motor4

	//setup flowMeter pins to be ISR
	wiringPiISR(flowMeter1,INT_EDGE_FALLING, &myInterrupt0);
	wiringPiISR(flowMeter2,INT_EDGE_FALLING, &myInterrupt1);
	wiringPiISR(flowMeter3,INT_EDGE_FALLING, &myInterrupt2);
	wiringPiISR(flowMeter4,INT_EDGE_FALLING, &myInterrupt3);
	
	//reset all counters to 0
	for(pin=0; pin < 4; pin++){
		globalCounter[pin] = myCounter [pin] = 0;
	}

	printf("Start Bluetooth Communication\n");
	fd=serialOpen("/dev/ttyAMA0",9600);	//Open serial port and set baud=9600

	//Wait for Interrupts to be triggered on specified pins
      	//gotOne only allows "Waiting..." to be placed before 
	//each trigger of the ISR....So not really needed.
	//Counter also resets for each pin after 11 counts
	for(;;){
		gotOne=0;
		printf("Waiting...");fflush(stdout);
		for(;;){
	 		for(pin=0 ;pin < 4; pin++){
				if(globalCounter[pin] != myCounter[pin]){
					printf("Int on pin %d: Counter:%5d\n",pin,globalCounter[pin]);
					myCounter[pin] = globalCounter[pin];
					++gotOne;
					if(myCounter[pin]>10){
						myCounter[pin]=globalCounter[pin]=0;
					}
				}
			}
			if(gotOne != 0){
				break;
			}
		
			while(serialDataAvail(fd)){
				data[i++]=(char)serialGetchar(fd);
				if(strcmp(data,"on")==0){
					digitalWrite(motor1,HIGH);
					printf("-> %s\n",data);
				        fflush(stdout);
			        	for(x=0;x<i;x++){
						data[x]=0; 
					}
          				i=0;
      				}
				else if(strcmp(data,"off")==0){
					digitalWrite(motor1,LOW);
					printf("-> %s\n",data);
					fflush(stdout);
		        		for(x=0;x<i;x++){
						data[x]=0; 
					}
					i=0;
				}
			}
		}
	}

	return 0;
}
