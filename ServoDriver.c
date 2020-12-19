// -------------------------------------------------------------------
// File name: ServiDriver.c
// Description: This code communicates with 2 servos on our final project using PDM =
// Author: Garrett Wong, Jason Siegal, Takuma, Rish
// Orig gen date: March 25, 2020
// Last update: April 9, 2020
//
// PIN D0,D1

//Position "0" (1.5 ms pulse) is middle, "90" (~2ms pulse) is all the way to the left.
//"-90" (~1 ms pulse) is middle, "90" (~2 ms pulse) is all the way to the right, 
//the left.

//------------------------------------------------------------------------------

#include "../ValvanoWareTM4C123v5/inc/PWM.h"

//TODO rename these better
#define D0 0;
#define D1 1;

#define PB7       (*((volatile uint32_t *)0x40005200))
#define PB6       (*((volatile uint32_t *)0x40005100))
#define PB5       (*((volatile uint32_t *)0x40005080))
#define PB4       (*((volatile uint32_t *)0x40005040))

volatile int servoxdeg;
volatile int servoydeg;

// This function takes raw data from blynk and and translates into degrees for servo x and servo y
// changes the global variables servoxdeg and servydeg
void accToServo(int accRawData){
	
}


//ServoInit -> Initializes PWM
//period 20ms 50Hz "PWM period"
//duty init to 1.5ms "position duty"
//PWM output on PB6 and PB4
void ServoInit(void){
	PWM0G_Init(25000,1725); // initializes the PWM
}

//SetDegrees -> Changes the degrees by changing the Duty Cycles for appropriate pin number
void SetDegrees(int degrees, int pin){
	//Degrees Range: [-9,9]
	//PWM Duty Cycle Range[525, 3225]
	//Resting Servo Position: 1725
	//Increments: 138
	int duty;
	if(degrees > 9) {
		duty = 3200;
	} else if(degrees < -9) {
		duty = 600;
	} else {
		int tempDegree = degrees + 9;
		duty = 600 + (tempDegree * 138);
	}
	//TDO set to change other duty cycle
	if(pin){
		PWM0G_Duty(duty);
	} else {
		PWM0G_Duty(duty) ;
	}
	
	//IF DEGREE RANGE IS [-90,90]
	//int tempDegree = degree + 90;
	//PWMOG_Init(1600000, 80000 + (tempDegree * 444));
	//if(pin){
	//	Right_Duty(duty, 1);
	//} else {
	//	Left_Duty(duty, 1);
	//}
}

void setX(void){
	//SetDegrees(servoxdeg, 0);
}

void setY(void){
	//SetDegrees(servoydeg, 1);
}
