/*Diaphragm Pumps
  This code interfaces with 4 different diaphragm pumps
  connected to a quad motor driver(H bridge IC). It will
  turn on each motor for a total of 1 second.  

  Author: Jose Estrada
  Date: 04/18/15
*/
#include <stdio.h>
#include <wiringPi.h>

#define motor1 0
#define motor2 1
#define motor3 2
#define motor4 3

int main(){
	wiringPiSetup();	//setup wiringPi library
	pinMode(motor1,OUTPUT);	//set pin 0 to control motor1
	pinMode(motor2,OUTPUT); //set pin 1 to control motor2
	pinMode(motor3,OUTPUT);	//set pin 2 to control motor3
	pinMode(motor4,OUTPUT); //set pin 3 to control motor4
	
	//Turn motor1:motor4 on for 3 seconds each then turn off
	printf("Motor1 On\n");
	digitalWrite(motor1,HIGH);
	delay(1000);
	digitalWrite(motor1,LOW);
	delay(1000);

	printf("Motor2 On\n");
	digitalWrite(motor2,HIGH);
	delay(1000);
	digitalWrite(motor2,LOW);
	delay(1000);

	printf("Motor3 On\n");
	digitalWrite(motor3,HIGH);
	delay(1000);
	digitalWrite(motor3,LOW);
	delay(1000);

	printf("Motor4 On\n");
	digitalWrite(motor4,HIGH);
	delay(1000);
	digitalWrite(motor4,LOW);

	printf("That's All Folks!\n");
	return 0;
}
