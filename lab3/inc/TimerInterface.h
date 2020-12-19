/* TimerInterface.h
 * Authors: Takuma Fujiwara and Rish Bhatnagar
 * Initial Creation Date: 2/11/2020
 * Description: Contains functions for keeping track of time for alarm clock.
 * Lab 3
 * TA: Bhagawat Vinay
 * Last Revision: 2/21/2020
 * 
 */
#ifndef _TimerInterface_
#define _TimerInterface_

#include <stdint.h>

extern volatile uint8_t hour;
extern volatile uint8_t minute;
extern volatile uint8_t second;
extern volatile uint8_t is_Changed_Time;

//------------Timer5A_Init------------
// Initializing Timer5 to trigger every 1 second.
// Input: none
// Output: none
void Timer5A_Init(void);

//------------Timer5A_Handler------------
// Handler for Timer5A. Increments time by a second.
// Input: none
// Output: none
void Timer5A_Handler(void);

#endif
