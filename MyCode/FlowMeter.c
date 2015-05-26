/*Code to interface with a Hall effect flow meter using
  the Raspberry Pi. ISR is used to pull information from 
  the selected pins. WiringPi library is used specifically
  the wiringPiISR() function. Using wiringPi library with
  Raspberry Pi.

  Author:Jose Estrada
  Date:4/18/15
*/
#include <stdio.h>
#include <wiringPi.h>

#define flowMeter1 4
#define flowMeter2 5 
#define flowMeter3 6
#define flowMeter4 7

static volatile int globalCounter[4];	//global counter for all four meters

//interput functions to increment flow meter counts
//functions must be "free" for wiringPiISR function to work
void myInterrupt0(){++globalCounter[0];}
void myInterrupt1(){++globalCounter[1];}
void myInterrupt2(){++globalCounter[2];}
void myInterrupt3(){++globalCounter[3];}

int main(void){
	int pin, gotOne;
	int myCounter[4];
	wiringPiSetup();

	//reset all counters to 0
	for(pin=0; pin < 4; pin++){
		globalCounter[pin] = myCounter [pin] = 0;
	}

	//setup flowMeter pins to be ISR
	wiringPiISR(flowMeter1,INT_EDGE_FALLING, &myInterrupt0);
	wiringPiISR(flowMeter2,INT_EDGE_FALLING, &myInterrupt1);
	wiringPiISR(flowMeter3,INT_EDGE_FALLING, &myInterrupt2);
	wiringPiISR(flowMeter4,INT_EDGE_FALLING, &myInterrupt3);

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
		}
	}
	return 0;
}
