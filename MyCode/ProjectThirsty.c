/*Project: Thirsty
  A project to create an automated drink mixer with
  a diaphragm pump and flow meter. 

  Current state: All four motors are now running...
  individually and all together. Now I need to figure out 
  how to interface with the flow meters using interrupt 
  service routines (ISR). 

  Author: Jose Estrada
  Date: 04/15/15
*/
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

//Defs for Flowmeters for data to be collected
#define flowMeter1 4
#define flowMeter2 5 
#define flowMeter3 6
#define flowMeter4 7

//GPIO pin setup
#define group1 0
#define group2 1
#define group3 2
#define group4 3

//Reset count
#define N 10

static volatile int globalCounter[4];	//global counter for all four meters

//Interput functions to increment flow meter counts
//functions must be "free" for wiringPiISR function to work
void myInterrupt0(){++globalCounter[0];}
void myInterrupt1(){++globalCounter[1];}
void myInterrupt2(){++globalCounter[2];}
void myInterrupt3(){++globalCounter[3];}

void Setup(){
	int pin;
	int myCounter[4];

	wiringPiSetup();	//setup wiringPi library

	//reset all counters to 0
	for(pin=0; pin < 4; pin++){
		globalCounter[pin] = myCounter [pin] = 0;
	}

	//Setup motor pins
	pinMode(group1,OUTPUT);	//set pin 0 to control group1
	pinMode(group2,OUTPUT); //set pin 1 to control group2
	pinMode(group3,OUTPUT);	//set pin 2 to control group3
	pinMode(group4,OUTPUT); //set pin 3 to control group4

	//Make sure all motor pins are turned off
	digitalWrite(group1,LOW);
	digitalWrite(group2,LOW);
	digitalWrite(group3,LOW);
	digitalWrite(group4,LOW);

	//setup flowMeter pins to be ISR
	wiringPiISR(flowMeter1,INT_EDGE_FALLING, &myInterrupt0);
	wiringPiISR(flowMeter2,INT_EDGE_FALLING, &myInterrupt1);
	wiringPiISR(flowMeter3,INT_EDGE_FALLING, &myInterrupt2);
	wiringPiISR(flowMeter4,INT_EDGE_FALLING, &myInterrupt3);
}

int main(){
	int pin,fd,x,i=0;;
	int myCounter[4];
	char data[10];
	
	//Setup pins to be used
	Setup();
	
	//Open serial port on GPIO and set baud=9600
	fd=serialOpen("/dev/ttyAMA0",9600);
	
	printf("Initiate Project Thirsty\n");
	fflush(stdout);

	//Accept input from serial port whenever it comes
	//and then re-route the input to the correct Output
	for(;;){
		while(serialDataAvail(fd)){
			data[i++]=(char)serialGetchar(fd);
			data[i+1]='\0';
			if(strcmp(data,"ONGroup1")==0){
				digitalWrite(group1,HIGH);
	  			printf("-> %s\n",data);
          			fflush(stdout);
			        for(x=0;x<i;x++){	//to reset data 
					data[x]=0; 
				}
          			i=0;
      			}
			else if(strcmp(data,"ONGroup2")==0){
				digitalWrite(group2,HIGH);
	  			printf("-> %s\n",data);
          			fflush(stdout);
			        for(x=0;x<i;x++){	//to reset data 
					data[x]=0; 
				}
          			i=0;
      			}
			else if(strcmp(data,"ONGroup3")==0){
				digitalWrite(group3,HIGH);
	  			printf("-> %s\n",data);
          			fflush(stdout);
			        for(x=0;x<i;x++){	//to reset data 
					data[x]=0; 
				}
          			i=0;
      			}
			else if(strcmp(data,"ONGroup4")==0){
				digitalWrite(group4,HIGH);
	  			printf("-> %s\n",data);
          			fflush(stdout);
			        for(x=0;x<i;x++){	//to reset data 
					data[x]=0; 
				}
          			i=0;
      			}
			else if(strcmp(data,"OFFGroup1")==0){
				digitalWrite(group1,LOW);
	  			printf("-> %s\n",data);
          			fflush(stdout);
			        for(x=0;x<i;x++){	//to reset data 
					data[x]=0; 
				}
          			i=0;
      			}
			else if(strcmp(data,"OFFGroup2")==0){
				digitalWrite(group2,LOW);
	  			printf("-> %s\n",data);
          			fflush(stdout);
			        for(x=0;x<i;x++){	//to reset data 
					data[x]=0; 
				}
          			i=0;
      			}
			else if(strcmp(data,"OFFGroup3")==0){
				digitalWrite(group3,LOW);
	  			printf("-> %s\n",data);
          			fflush(stdout);
			        for(x=0;x<i;x++){	//to reset data 
					data[x]=0; 
				}
          			i=0;
      			}
			else if(strcmp(data,"OFFGroup4")==0){
				digitalWrite(group4,LOW);
	  			printf("-> %s\n",data);
          			fflush(stdout);
			        for(x=0;x<i;x++){	//to reset data 
					data[x]=0; 
				}
          			i=0;
      			}
    		}  

	//code for the flowMeters
	//to increment counters and reset them after a certain number of counts
		for(pin=0 ;pin < 4; pin++){
			if(globalCounter[pin] != myCounter[pin]){
				printf("Int on flowmeter %d: Counter:%5d\n",pin,globalCounter[pin]);
				myCounter[pin] = globalCounter[pin];
				// To reset count after certain number of ticks
				if(myCounter[pin]==N){
					myCounter[pin]=globalCounter[pin]=0;
				}
			}
		}
  	}
	return 0;
}
