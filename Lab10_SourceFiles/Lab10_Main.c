// -------------------------------------------------------------------
// File name: Lab10_Main
// Description: Code for PI Motor Controller interface with Blynk App
// Author: Garrett Wong, Jason Siegal, Takuma Fujiwara, Rish
// Converted to EE445L style Jonathan Valvano
// Orig gen date: April 24, 2020
// Last update: April 26, 2020
// -------------------------------------------------------------------

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "EE445L/ValvanoWareTM4C123v5/inc/tm4c123gh6pm.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/PLL.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/LaunchPad.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/CortexM.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/ST7735.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/UART.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/UART1.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/PWM.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/Timer5A.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/Timer2A.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/Timer3A.h"
#include "PWM.h"
#include "InputCapture.h"
#include "MotorControl.h"
#include "esp8266.h"
#include "PortF.h"

extern	uint16_t KP1;
extern	uint16_t KI1;
extern	uint16_t KP2;
extern	uint16_t KI2;
extern	uint16_t desiredSpeed;

extern	uint16_t RevSec;
extern	uint16_t U;
extern	int E;
extern	uint16_t PropTerm;
extern	uint16_t IntTerm;

extern 	uint16_t c;
extern  uint32_t speedActual;
	
char serial_buf[64];
char Pin_Number[2]   = "99";       // Initialize to invalid pin number
char Pin_Integer[8]  = "0000";     //
char Pin_Float[8]    = "0.0000";   //
uint32_t pin_num; 
uint32_t pin_int;
void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

uint32_t LED;      // VP1
 
// ----------------------------------- TM4C_to_Blynk *HELPER -------------------
// Send data to the Blynk App
// It uses Virtual Pin numbers between 70 and 99
void TM4C_to_Blynk(uint32_t pin,uint32_t value){
  if((pin < 70)||(pin > 99)){
    return; // ignore illegal requests
  }
// your account will be temporarily halted 
  // if you send too much data
  ESP8266_OutUDec(pin);       // Send the Virtual Pin #
  ESP8266_OutChar(',');
  ESP8266_OutUDec(value);      // Send the current value
  ESP8266_OutChar(',');
  ESP8266_OutString("0.0\n");  // Null value not used in this example
}
 
 
// -------------------------   Blynk_to_TM4C  -----------------------------------
// This routine receives the Blynk Virtual Pin data via the ESP8266 and parses the
// data and feeds the commands to the TM4C.

void Blynk_to_TM4C(void){int j; char data; 	// Check to see if a there is data in the RXD buffer
  if(ESP8266_GetMessage(serial_buf)){  // returns false if no message
    // Read the data from the UART5           
		// Rip the 3 fields out of the CSV data. The sequence of data from the 8266 is:
		// Pin #, Integer Value, Float Value.
    strcpy(Pin_Number, strtok(serial_buf, ","));
    strcpy(Pin_Integer, strtok(NULL, ","));       // Integer value that is determined by the Blynk App
    strcpy(Pin_Float, strtok(NULL, ","));         // Not used
    pin_num = atoi(Pin_Number);     // Need to convert ASCII to integer
    pin_int = atoi(Pin_Integer);  
		// Bhagawat's pinouts
		// VP1 -> KP1 
    if(pin_num == 1){  
			KP1 = pin_int;
    }
		// VP2 -> KP2
    if(pin_num == 2){
			KP2 = pin_int;			
    }
		// VP2 -> KI1
		if(pin_num == 3){
			KI1 = pin_int;
    }
		// VP2 -> KI2
		if(pin_num == 4){
			KI2 = pin_int;
    }
		// Desired Motor Speed
		if(pin_num == 7){
				PI_Motor_Control(desiredSpeed); 
    }
  }
}


// ----------------------------------- TM4C_to_Blynk ---------------------------
// Only send if data is different
uint16_t lastRevSec;
uint16_t lastU;
uint16_t lastE;
uint16_t lastPropTerm;
uint16_t lastIntTerm;

// TODO Think about rate sending information
void SendInformation(void){	
	uint16_t thisRevSec = RevSec;
	uint16_t thisU = U;
	uint16_t thisE = E;
	uint16_t thisPropTerm = PropTerm;
	uint16_t thisIntTerm = IntTerm;
	
	// Revolutions/sec -> V75
	if(thisRevSec != lastRevSec){
		TM4C_to_Blynk(75,thisRevSec);
		lastRevSec = thisRevSec;
	}
	// PWM Control (U) -> V80	
	if(thisU != lastU){
		TM4C_to_Blynk(80,thisU);
		lastU = thisU;
	}
	// PI ERROR (E)    -> V78
	if(thisE != lastE){
		TM4C_to_Blynk(78,thisE);
		lastE = thisE;
	}
	// Proportional Term -> V77
	if(thisPropTerm != lastPropTerm){
		TM4C_to_Blynk(77,thisPropTerm);
		lastPropTerm = thisPropTerm;
	}
	// Integral Term -> V79
	if(thisIntTerm != lastIntTerm){
		TM4C_to_Blynk(79,thisIntTerm);
		lastIntTerm = thisIntTerm;
	}		
}

// TODO: Double check inputs
void wait10ms (void){
	c++;
	if (c>=3){
		speed = 0;
	}
}
int main(void){
		PLL_Init(Bus80MHz);   // Setup PLL for 80 MHz
  	DisableInterrupts();	// Disable Interrupts during INIT
		PortF_Init();	
		ESP8266_Init();       // Enable ESP8266 Serial Port
		ESP8266_Reset();      // Reset the WiFi module
		ESP8266_SetupWiFi();  // Setup communications to Blynk Server
		//LED_Init();         // Initialize the LEDs
		UART_Init();               	// Enable Debug Serial Port
  	ST7735_InitR(INITR_REDTAB); 	// Initialize the ST7735
  	MC_init();                  	// Initialize Motor Control function
		Timer5A_Init(&wait10ms,800000,2);
		Timer2A_Init(&Blynk_to_TM4C,800000,2); 
		// check for receive data from Blynk App every 10ms
		Timer3A_Init(&SendInformation,40000000,2); 
		// Send data back to Blynk App every 1/2 seconds
  	EnableInterrupts();		// Enable Interrupts
  
    while (1) { 
      WaitForInterrupt(); // low power mode
    }
}
