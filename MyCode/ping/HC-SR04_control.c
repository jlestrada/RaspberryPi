/*
 * HC-SR04_control.c:
 *      This will all the Raspberry Pi to control three individual HC-SR04 ping 
 *	sensors.Wiringpi library used to setup the GPIO pins. Download library
 *	@ http://wiringpi.com/download-and-install/
 *	Reference:      
 *	Matt, http://www.raspberrypi-spy.co.uk/2012/12/ultrasonic-distance-measurement-using-python-part-1/
 *	Author:
 *	Jose Estrada, jlestrada26@att.net
 *	Date: 4--14
 */

// Necessary Libraries
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

// Gobal Variables
#define echo_1 6	// BCM 25...echo for Ping_1
#define trig_1 5	// BCM 24...trig for Ping_1

void setup(){
        wiringPiSetup ();
        //First need to set Pins to input and output
        pinMode(trig_1,OUTPUT);         // Set trig_1 to output
        pinMode(echo_1,INPUT);          // Set echo_1 to input
}
int distance(){
        // Set trigger to 0
        digitalWrite(trig_1,0);
        delay(500);

        // Send 10us signal to trig_1 then turn off
        digitalWrite(trig_1,1);
        delay(0.002);
        digitalWrite(trig_1,0);
	
	while (digitalRead(echo_1)==0);
	
	//Start time
	long start=micros();
        while (digitalRead(echo_1)==1);
	long stop=micros();
	int elapsed=stop-start;
        int dist=elapsed/58;
	printf("Distance: %d cm\n",dist);
	return 0;
}
int main (void)
{
	printf("Raspberry Pi Ping sensor\n");
	setup();
	while(1){	
	distance();
	}
	return 0;
}
