/* Lab3.c
 * Authors: Takuma Fujiwara and Rish Bhatnagar
 * Initial Creation Date: 2/11/2020
 * Description: Main file for running alarm clock clock
 * Lab 3
 * TA: Bhagawat Vinay
 * Last Revision: 2/21/2020
 * 
 */

#include <stdint.h>
#include "../../inc/tm4c123gh6pm.h"
#include "../../inc/PLL.h"
#include "../../inc/LaunchPad.h"
#include "../../inc/CortexM.h"
#include "../../inc/ST7735.h"
#include "../../inc/Switch.h"
#include "../../inc/SwitchExternal.h"
#include "../inc/TimerInterface.h"
#include "../inc/ButtonInterface.h"
#include "../inc/SpeakerInterface.h"
#include "../inc/ScreenInterface.h"



//global variables shared between interrupts
volatile uint8_t is_Changed_Time = 0;
volatile uint8_t hour = 5;
volatile uint8_t minute = 59;
volatile uint8_t second = 40;

//unshared global variable
volatile uint8_t is_Enabled = 1;
volatile uint8_t alarm_Hour = 6;
volatile uint8_t alarm_Minute = 0;
volatile uint8_t alarm_Second = 0;

//------------Rise------------
// Handler for rising edge of edge trigger interrupt. Toggles enabling of alarm and PF2.
// Input: none
// Output: none
void Rise(void) {
  Stop_Alarm();
  
  if(is_Enabled == 1) {
    is_Enabled = 0;
    PF2&=~0x04;
  } else {
    is_Enabled = 1;
    PF2|=0x04;
  }
  
}


//------------Fall------------
// Handler for falling edge of edge trigger interrupt. Does nothing.
// Input: none
// Output: none
void Fall(void) {
  //do nothing....
}


//------------Timer1_Init------------
// Initializes Timer 1 for button debouncing.
// Input: none
// Output: none
void Timer1_Init(void){ volatile uint32_t delay;
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, down-count 
  TIMER1_TAILR_R = 0xFFFFFFFF;  // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}


//Alarm clock's main function
int main(void){
  PLL_Init(Bus80MHz);                   // 80 MHz
  LaunchPad_Init();                     // activate port F
  SwitchPB1_Init();
  SwitchPC6_Init();
  Screen_Init(hour,minute);
  Show_AlarmTime(alarm_Hour, alarm_Minute, alarm_Second);
  Show_DigitalTime(hour, minute, second);
  Timer1_Init();                        // for measuring time
  Timer5A_Init();                       // for keeping track of alarm 
  Switch_Init(&Rise,&Fall);
  
  if(is_Enabled){
    PF2|=0x04;
  } else {
    PF2&=~0x04;
  }
  
  uint8_t myHour;
  uint8_t myMinute;
  uint8_t mySecond;
  uint8_t counter = 0;
  uint32_t status1;
  uint32_t status2;
  
  while(1){
    status1 = SwitchPB1_Debounce();
    status2 = SwitchPC6_Debounce();
    
    if(status1) {
      counter++;
      counter%=6;
    }
    TIMER5_CTL_R = 0x00000000;
    if(status2) {
      if(counter == 0) {
        hour++;
        if(hour >= 24) {
          hour = 0;
        }
      }
      if(counter == 1) {
        minute++;
        if(minute >= 60) {
          minute = 0;
        }
      }
      if(counter == 2) {
        second++;
        if(second >= 60) {
          second = 0;
        }
      }
      if(counter == 3) {
        alarm_Hour++;
        if(alarm_Hour >= 24) {
          alarm_Hour = 0;
        }
      }
      if(counter == 4) {
        alarm_Minute++;
        if(alarm_Minute >= 60) {
          alarm_Minute = 0;
        }
      }
      if(counter == 5) {
        alarm_Second++;
        if(alarm_Second >= 60) {
          alarm_Second = 0;
        }
      }
      is_Changed_Time = 1;
    }
    TIMER5_CTL_R = 0x00000001;
    
    if(is_Changed_Time==1){
      
      //Critical Section
      DisableInterrupts();
      is_Changed_Time = 0;
      myHour = hour;
      myMinute = minute;
      mySecond = second;
      EnableInterrupts();
      //End Critical Section
      
      uint32_t startTime = TIMER1_TAR_R;   //starting measurement of time to update LCD
      Show_DigitalTime(myHour, myMinute, mySecond);
      Show_AlarmTime(alarm_Hour, alarm_Minute, alarm_Second);
      Draw_Hands(myHour, myMinute);  
      uint32_t endTime = TIMER1_TAR_R;     //ending measurement of time to update LCD
      uint32_t elapsedTime = startTime - endTime;
      if((is_Enabled == 1)&&(second==alarm_Second)&&(minute==alarm_Minute)&&(hour==alarm_Hour)){
        Start_Alarm();
      }
    }

  }
}


