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
//to control power to flowmeters and pumps
#define group1 0
#define group2 1
#define group3 2
#define group4 3

//global counter for all four meters
static volatile int globalCounter[4];	

//Interput functions to increment flow meter counts
//functions must be "free" for wiringPiISR function to work
//For now till further testing is done-->10 counts=1oz
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

//Calibrate function to prime motors
//use data protocol to grab data
void Calibrate(char data[100], int i){
	printf("Initate Calibrate function");
	printf("Data-->%s\n",data);
	
	//for protocol that indicates On
	if(strncmp(data,"CalOn",5)==0){
		if(data[i-2]=='1'){
			digitalWrite(group1,HIGH);
			printf("Group1 ON\n");
		}
		else if(data[i-2]=='2'){
			digitalWrite(group2,HIGH);
			printf("Group2 ON\n");
		}
		else if(data[i-2]=='3'){
			digitalWrite(group3,HIGH);
			printf("Group3 ON\n");
		}
		else if(data[i-2]=='4'){
			digitalWrite(group4,HIGH);
			printf("Group4 ON\n");
		}
	}

	//for protocol that indicates OFF
	else if(strncmp(data,"CalOff",5)==0){
		if(data[i-2]=='1'){
			digitalWrite(group1,LOW);
			printf("Group1 OFF\n");
		}
		else if(data[i-2]=='2'){
			digitalWrite(group2,LOW);
			printf("Group2 OFF\n");
		}
		else if(data[i-2]=='3'){
			digitalWrite(group3,LOW);
			printf("Group3 OFF\n");
		}
		else if(data[i-2]=='4'){
			digitalWrite(group4,LOW);
			printf("Group4 OFF\n");
		}
	}
}

//Drink function to turn on specified groups to 
//create drinks (i.e. groups 1 & 2 create drink 1)
void Drink(char data[100], int i){
	printf("Initate Drink function");
	
	//Drink 1= bottle 1 & 2
	if(strncmp(data,"DriDrink1",9)==0){
		globalCounter[0]=0;	//reset counters
		globalCounter[1]=0;
		
		//start groups to begin making drink
		digitalWrite(group1,HIGH);
		digitalWrite(group1,HIGH);
	
		//stop groups when correct amount is given
		while(1){
			if(globalCounter[0]==20){	//2oz from group 1
				printf("Stop group 1 2oz has been dispensed");
				digitalWrite(group1,LOW);
			}
			else if(globalCounter[0]>=20 && globalCounter[1]==60){	//6oz from group 2
				printf("Stop group 2 6oz has been dispensed");
				digitalWrite(group2,LOW);
				break;
			}
		}
	}
}

int main(){
	int fd,x,i=0;;
	char data[100];

	//Setup pins to be used
	Setup();
	
	//Set data to Null
	data[0]='\0';

	//Open serial port on GPIO and set baud=9600
	fd=serialOpen("/dev/ttyAMA0",9600);
	
	printf("Initiate Project Thirsty\n");
	fflush(stdout);

	//Collect data from bluetooth module and 
	//use protocol structure to decrypt
	for(;;){

		//Collect data from Bluetooth module
		while(serialDataAvail(fd)){
			data[i++]=(char)serialGetchar(fd);
			data[i+1]='\0';
		}

		//to check if ending flag is dedected, 
		//in our case it is x
		if(data[i-1]=='X'){
			if(strncmp(data,"Cal",3)==0){	//find if data is to be routed to Calibrate()
				Calibrate(data, i);	
			}
			else if(strncmp(data,"Dri",3)==0){ //find if data is to be routed to Drink()
				Drink(data,i);
			}
			
			//Reset data string
			for(x=0;x<i;x++){	
				data[x]=0; 
			}
          		i=0;
		}
  	}
	return 0;
}
