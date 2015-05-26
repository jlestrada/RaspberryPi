/*Code to test bluetooth serial connection
  using android phone with app inventor app
  as device  to connect to....
  Module: HC-05
  baud=9600
  
  Author: Jose Estrada
*/
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

//GPIO pin setup
#define motor1 0
#define motor2 1
#define motor3 2
#define motor4 3

int main(void){
  //Necessary variables to initailize
  int fd,x,i=0;
  char data[5];
  
  wiringPiSetup();			//Needed to setup wiringPi library
 
  //GPIO pin setup
  pinMode(motor1,OUTPUT);		//set pin 0 to control motor1
  pinMode(motor2,OUTPUT);		//set pin 1 to control motor2
  pinMode(motor3,OUTPUT);		//set pin 2 to control motor3
  pinMode(motor4,OUTPUT);		//set pin 3 to control motor4

  printf("Start Bluetooth Communication\n");
  fd=serialOpen("/dev/ttyAMA0",9600);	//Open serial port and set baud=9600

  for(;;){
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
  return 0;
}
