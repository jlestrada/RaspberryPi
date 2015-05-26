/*This is a simple code to blink an LED
  using the Raspberry Pi and wiringPi Library
*/
#include <wiringPi.h>

int main(){
	wiringPiSetup();	//function to setup up library
	pinMode(0,OUTPUT);	//set pin 0 to output (GPIO #17)
	for(;;){
		digitalWrite(0,HIGH);	//set pin 0 to on
		delay(500);		//wait 500ms
		digitalWrite(0,LOW);	//set pin 0 to off
		delay(500);		//wait 500ms
	}
	return 0;
}
