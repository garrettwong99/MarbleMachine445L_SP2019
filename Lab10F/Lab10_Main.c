#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "files/tm4c123gh6pm.h"
#include "files/PLL.h"
#include "files/LaunchPad.h"
#include "files/CortexM.h"
#include "files/ST7735.h"
#include "files/Timer2A.h"
#include "files/Timer3A.h"
#include "files/esp8266.h"
#include "files/UART.h"
#include "files/UART1.h"
#include "files/PWM.h"
#include "files/Timer5A.h"
#include "files/InputCapture.h"
#include "MotorControl.h"


int New_Speed;
char serial_buf[64];
char Pin_Number[2]   = "99";       // Initialize to invalid pin number
char Pin_Integer[8]  = "0000";     //
char Pin_Float[8]    = "0.0000";   //
uint32_t pin_num; 
uint32_t pin_int;

// ----------------------------------- TM4C_to_Blynk ------------------------------
// Send data to the Blynk App
// It uses Virtual Pin numbers between 70 and 99
// so that the ESP8266 knows to forward the data to the Blynk App
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

void SendInformation(){
	//calls TM4C_to_Blynk
}

void Blynk_to_TM4C(void){
	int j; char data;
	// Check to see if a there is data in the RXD buffer
  if(ESP8266_GetMessage(serial_buf)){  // returns false if no message   
		// Rip the 3 fields out of the CSV data. The sequence of data from the 8266 is:
		// Pin #, Integer Value, Float Value.
    strcpy(Pin_Number, strtok(serial_buf, ","));
    strcpy(Pin_Integer, strtok(NULL, ","));       // Integer value that is determined by the Blynk App
    strcpy(Pin_Float, strtok(NULL, ","));         // Not used
    pin_num = atoi(Pin_Number);     // Need to convert ASCII to integer
    pin_int = atoi(Pin_Integer);  
  }  
}

int main(void){
		PLL_Init(Bus80MHz);                 	// Setup PLL for 80 MHz
  	DisableInterrupts();		// Disable Interrupts during INIT
  	//LED_Init();                 	// Initialize the LEDs
		UART_Init();               	// Enable Debug Serial Port
  	ESP8266_Reset();			// Reset the WiFi module
  	ST7735_InitR(INITR_REDTAB); 	// Initialize the ST7735
  	ESP8266_Init();                	// Setup communications to Blynk Server
  	MC_init();                  	// Initialize Motor Control function
  	Timer5A_Init(&SendInformation, 80000000, 2);    // Timer for Blynk Server Comm
  	EnableInterrupts();		// Enable Interrupts 
  
    while (1) { 
      Blynk_to_TM4C();             // Get serial data from ESP8266
      PI_Motor_Control(New_Speed); // Run the PI MC Loop with desired speed
    }
}
