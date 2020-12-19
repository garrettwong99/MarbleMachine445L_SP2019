/* TimerInterface.c
 * Authors: Takuma Fujiwara and Rish Bhatnagar
 * Initial Creation Date: 2/11/2020
 * Description: Contains functions for keeping track of time for alarm clock.
 * Lab 3
 * TA: Bhagawat Vinay
 * Last Revision: 2/21/2020
 * 
 */

#include "../../inc/LaunchPad.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "../inc/TimerInterface.h"
#include "../inc/ScreenInterface.h"

//------------Timer5A_Init------------
// Initializing Timer5 to trigger every 1 second.
// Input: none
// Output: none
void Timer5A_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x20;   // 0) activate TIMER5
  TIMER5_CTL_R = 0x00000000;    // 1) disable TIMER5A during setup
  TIMER5_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER5_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER5_TAILR_R = 79999999;    // 4) reload value for 1 second
  TIMER5_TAPR_R = 0;            // 5) bus clock resolution
  TIMER5_ICR_R = 0x00000001;    // 6) clear TIMER5A timeout flag
  TIMER5_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI23_R = (NVIC_PRI23_R&0xFFFFFF00)|(2<<5); // priority 2 
// interrupts enabled in the main program after all devices initialized
// vector number 108, interrupt number 92
  NVIC_EN2_R = 1<<28;           // 9) enable IRQ 92 in NVIC
  TIMER5_CTL_R = 0x00000001;    // 10) enable TIMER5A
}



uint8_t test_HourHand = 0; //debugging variable for drawing hand
uint8_t test_MinuteHand = 0;

//------------Debug_Hand------------
// Debugging code for drawing hands. Increments and draws hour and minute hand every call.
// Input: none
// Output: none
void Debug_Hand(void){	
	test_HourHand++;
	if(test_HourHand == 24){
		test_HourHand = 0;
	}
	test_MinuteHand++;
	if(test_MinuteHand == 60){
		test_MinuteHand = 0;
	}
	Draw_Hands(test_HourHand, test_MinuteHand);
}

//------------Timer5A_Handler------------
// Handler for Timer5A. Increments time by a second.
// Input: none
// Output: none
void Timer5A_Handler(void){
  TIMER5_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER5A timeout
	PF1 ^= 0x02; // LED heartbeat	
	//Debug_Hand();
  //updating time code
	second++;
	if(second==60){
		second=0;
		minute++;
		if(minute==60){
			minute=0;
			hour++;
			if(hour==24){
				hour=0;
			}
		}
	}
	is_Changed_Time = 1;
}
