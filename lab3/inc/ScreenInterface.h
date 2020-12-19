/* ScreenInterface.h
 * Authors: Takuma Fujiwara and Rish Bhatnagar
 * Initial Creation Date: 2/11/2020
 * Description: Contains functions for drawing alarm clock on LCD.
 * Lab 3
 * TA: Bhagawat Vinay
 * Last Revision: 2/21/2020
 * 
 */

#ifndef _ScreenInterface_
#define _ScreenInterface_
#include <stdint.h>

//------------Screen_Init------------
// Initializes LCD and draws a clock with given initial time.
// Input: initialHour - integer from 0 to 23.
//        initialMinute - integer from 0 to 59.
// Output: none
void Screen_Init(uint8_t initialHour, uint8_t initialMinute);

//------------Draw_Clockface------------
// Draws a circle with the numbers 1 through 12 placed evenly around circle.
// Input: none
// Output: none
void Draw_Clockface(void);

//------------Show_DigitalTime------------
// Displays current time in digital format.
// Input: hour - integer from 0 to 23.
//        minute - integer from 0 to 59.
//        second - integer from 0 to 59.
// Output: none
void Show_DigitalTime(uint8_t hour, uint8_t minute, uint8_t second);

//------------Show_AlarmTime------------
// Displays alarm time in digital format.
// Input: hour - integer from 0 to 23.
//        minute - integer from 0 to 59.
//        second - integer from 0 to 59.
// Output: none
void Show_AlarmTime(uint8_t hour, uint8_t minute, uint8_t second);

//------------Draw_Hands------------
// Draws hour and minute hands onto clock face.
// Input: hour - integer from 0 to 23.
//        minute - integer from 0 to 59.
// Output: none
void Draw_Hands(uint8_t hour, uint8_t minute);

#endif
