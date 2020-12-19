// -------------------------------------------------------------------
// File name: ServiDriver.h
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

// This function takes raw data from blynk and and translates into degrees for servo 1 and servo 2
void accToServo(int accRawData);


//-----------This initilizess port D0 and Port D1 for 
void ServoInit(void);

//SetDegrees -> Changes the degrees by changing the Duty Cycles
void SetDegrees(int degrees, int pin);

void setX(void);
void setY(void);