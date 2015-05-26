/*
 * motor_control.c:
 *      This will allow the Raspberry Pi to control two 
 *	indivdual DC motors with the SN754410 driver chip
 *	and the wiringpi library. Download library
 *	@ http://wiringpi.com/download-and-install/
 *	Reference:      
 *	Gordon Henderson, projects@drogon.net
 *	Author:
 *	Jose Estrada, jlestrada26@att.net
 *	Date: 4-24-14
 */

#include <stdio.h>
#include <wiringPi.h>

void setup(void){
	wiringPiSetup();
	pinMode(0,OUTPUT);		// BCM_GPIO pin 17 (for motor_1)
					// Forward control for motor_1
	pinMode(2,OUTPUT);		// BCM_GPIO pin 27 (for motor_1)
					// Backward control for motor_1
	pinMode(3,OUTPUT);		// BCM_GPIO pin 22 (for motor_2)
					// Forward control for motor_2
	pinMode(4,OUTPUT);		// BCM_GPIO pin 23 (for motor_2)
					// Backward control for motor_2

	pinMode(1,PWM_OUTPUT);		// BCM_GPIO pin 18...PWM (motor_speed)
	/* Note:This is the only pin that can be set for hardware PWM.
	Softwware PWM can be set if more needed, but has limitations.*/  
}
void clear(void){
	digitalWrite(0,0);
	digitalWrite(1,0);
	digitalWrite(2,0);
	digitalWrite(3,0);
	digitalWrite(4,0);
}
int pwmSet(int n){
	pwmWrite(1,n);	// Turn PWM pin On (range:0-1023)
				// value 1023 = 100% duty cycle
	printf ("pwm=%d\n",n);
	return 0;
}
int forward(int n){
	// Turn both wheels forward for 10 seconds
	printf ("Turn wheels forward for %d seconds\n", n/1000);
	// Motor_1 control	
	digitalWrite(0,1);	// On
	digitalWrite(2,0);	// Off
	// Motor_2 control
	digitalWrite(3,1);	// On
	digitalWrite(4,0);	// Off
	delay(n);		// 1000=1 second
	return 0;
}
int backward(int n){
	// Turn both wheels backward for 10 seconds!
	printf ("Turn wheels backward for %d seconds\n", n/1000);
	// Motor_1 control
	digitalWrite(0,0);	// On
	digitalWrite(2,1);   	// Off
	// Motor_2 control
	digitalWrite(3,0);	// On
	digitalWrite(4,1);	// Off
	delay(n);		// 1000=1 second
	return 0;
}
int opp_1(int n){
	// Stop motor_1. Move motor_2 forward for 10 seconds
	printf ("Move motor_1 backward. Move motor_2 forward\n") ;
	// Motor_1 control	
	digitalWrite (0, 0) ;	// On
	digitalWrite (2, 1) ;	// Off
	// Motor_2 control
	digitalWrite(3,1);	// On
	digitalWrite(4,0);	// Off
	delay(n) ;		// 1000=1 second
	return 0;
}
int main (void){
	int t=5000;		//Set time for motors (milliseconds)
	int speed=1023;		//Set pwm (speed) of motors (0-1023)
	setup();
	printf("Raspberry Pi: Run motors\n") ;
	pwmSet(speed);
	forward(t);
	backward(t);	
	opp_1(2500);
	clear();
	printf("motor_control.c now complete.\n") ;
	return 0;
}
