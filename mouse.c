/*
 * Mouse.c:
 *	This program will control two DC motors with 
 *	a single Ping Sensor(For now). The two motors will 
 *	run forward continously until the ping sensor picks
 *	up a signal that is "x" distantance away.It will 
 *	then reverse the motors for 2.5 seconds and then 
 *	turn each of the motors in opposing directions for
 * 	another 2.5 seconds. WiringPi library was used to 
 *	control the GPIO pins. Download library @
 *	http://wiringpi.com/download-and-install/
 *	Author: Jose Estrada, jlestrada26@att.net
 *	Date: 5-21-14
 */

// Necessary Libraries
#include <stdio.h>
#include <wiringPi.h>

// Gobal Variables
#define motor_1_fwd 0	// forward control for motor_1
#define motor_1_bwd 2	// backward control for motor_1
#define motor_2_fwd 3	// forward control for motor_2
#define motor_2_bwd 4	// backward control for motor_2
#define motor_speed 1	// PWM pin to control speed of motors
#define echo_1 6        // echo for Ping_1
#define trig_1 5        // trig for Ping_1

void setup(void){
        wiringPiSetup();

	// For motor control
        pinMode(motor_1_fwd,OUTPUT);            // BCM_GPIO pin 17 (for motor_1)
                                        	// Forward control for motor_1
        pinMode(motor_1_bwd,OUTPUT);            // BCM_GPIO pin 27 (for motor_1)
                                        	// Backward control for motor_1
        pinMode(motor_2_fwd,OUTPUT);		// BCM_GPIO pin 22 (for motor_2)
                                        	// Forward control for motor_2
        pinMode(motor_2_bwd,OUTPUT);            // BCM_GPIO pin 23 (for motor_2)
                                        	// Backward control for motor_2

        pinMode(motor_speed,PWM_OUTPUT);	// BCM_GPIO pin 18...PWM (motor_speed)
        /* Note:This is the only pin that can be set for hardware PWM.
        Softwware PWM can be set if more needed, but has limitations.*/

	// For Ping sensor
        pinMode(trig_1,OUTPUT);			// Set trig_1 to output
        pinMode(echo_1,INPUT);			// Set echo_1 to input
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
        return dist;
}
int forward(void){
        // Turn both wheels forward
        printf ("Turn wheels forward\n");
        // Motor_1 control
        digitalWrite(motor_1_fwd,1);	// ON
        digitalWrite(motor_1_bwd,0);	// OFF
        // Motor_2 control
        digitalWrite(motor_2_fwd,1);	// ON
        digitalWrite(motor_2_bwd,0);	// OFF
        return 0;
}
int backward(void){
        // Turn both wheels backward
        printf ("Turn wheels backward\n");
        // Motor_1 control
        digitalWrite(motor_1_fwd,0);    // OFF
        digitalWrite(motor_1_bwd,1);    // ON
        // Motor_2 control
        digitalWrite(motor_2_fwd,0);	// OFF
        digitalWrite(motor_2_bwd,1);	// ON
	delay(2500);		// 1000=1 second
        return 0;
}
int opp_1(void){
        // Stop motor_1. Move motor_2 forward for 10 seconds
        printf ("Move motor_1 backward. Move motor_2 forward\n") ;
        // Motor_1 control
        digitalWrite (0, 0) ;   // On
        digitalWrite (2, 1) ;   // Off
        // Motor_2 control
        digitalWrite(3,1);      // On
        digitalWrite(4,0);      // Off
        delay(2500) ;              // 1000=1 second
        return 0;
}
void main (void){
	printf("Project Mouse: An autonomous mobile vehicle");
	setup();
	while(1){
		int x=distance();
		if (x<15){
			backward();
			opp_1();
		}
		else{
			forward();
		}
	}
}
