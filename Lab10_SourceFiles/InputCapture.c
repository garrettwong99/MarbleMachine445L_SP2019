

#include <stdint.h>
#include <stdlib.h>
#include "EE445L/ValvanoWareTM4C123v5/inc/tm4c123gh6pm.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/PLL.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/LaunchPad.h"
#include "EE445L/ValvanoWareTM4C123v5/inc/CortexM.h"

uint16_t first = 0;
uint16_t period = 0;
uint16_t c;
uint32_t speedActual;

void (*PeriodicTask)(void);   // user function

void TimerCapture_Init(void){   
	SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0   
	SYSCTL_RCGCGPIO_R |= 0x00000002; // activate port B   
	int count = 0;                   // allow time to finish activating
	count = 42;                      // extra time to finish activating lol   
	GPIO_PORTB_DEN_R |= 0x10;        // enable digital I/O on PB4   
	GPIO_PORTB_AFSEL_R |= 0x10;      // enable alt funct on PB4 
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFF0FFFF)+0x00070000;   
	TIMER0_CTL_R &= ~0x00000001;     // disable timer0A during setup   
	TIMER0_CFG_R = 0x00000004;       // configure for 16-bit timer mode   
	TIMER0_TAMR_R = 0x00000007;      // configure for input capture mode   
	TIMER0_CTL_R &= ~(0x000C);       // TAEVENT is rising edge   
	TIMER0_TAILR_R = 0x0000FFFF;     // start value   
	TIMER0_IMR_R |= 0x00000004;      // enable capture match interrupt   
	TIMER0_ICR_R = 0x00000004;       // clear timer0A capture flag   
	TIMER0_CTL_R |= 0x00000001;      // enable timer0A    
	NVIC_PRI4_R =(NVIC_PRI4_R&0x00FFFFFF)|0x40000000; //Timer0A=priority 2   
	NVIC_EN0_R = 0x00080000;         // enable interrupt 19 in NVIC   
	EnableInterrupts(); 
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_CAECINT;// acknowledge timer0A capture match
	period = 0x00FFFFFF&(first - TIMER0_TAR_R);
	if((c<0)&&(period != 0 )){
		speed = 200000000/period; // .1 rps
	}
	c = 0;
	first = TIMER0_TAR_R;
  //(*PeriodicTask)();               // execute user task
}