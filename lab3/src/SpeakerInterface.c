#include "../inc/SpeakerInterface.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#define PWM_0_GENA_ACTCMPAD_ONE 0x000000C0  // Set the output signal to 1
#define PWM_0_GENA_ACTLOAD_ZERO 0x00000008  // Set the output signal to 0
#define PWM_0_GENB_ACTCMPBD_ONE 0x00000C00  // Set the output signal to 1
#define PWM_0_GENB_ACTLOAD_ZERO 0x00000008  // Set the output signal to 0

#define SYSCTL_RCC_USEPWMDIV    0x00100000  // Enable PWM Clock Divisor
#define SYSCTL_RCC_PWMDIV_M     0x000E0000  // PWM Unit Clock Divisor
#define SYSCTL_RCC_PWMDIV_2     0x00000000  // /2


// period is 16-bit number of PWM clock cycles in one period (3<=period)
// period for PD1 and PD0 must be the same
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
// PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
//                = BusClock/2
//                = 80 MHz/2 = 40 MHz (in this example)
// Output on PD0/M0PWM6
void PWM0G_Init(uint16_t period, uint16_t duty){
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x08;            // 2) activate port D
  while((SYSCTL_PRGPIO_R&0x08) == 0){};
  GPIO_PORTD_AFSEL_R |= 0x01;           // enable alt funct on PD0
  GPIO_PORTD_PCTL_R &= ~0x0000000F;     // configure PD0 as PWM6
  GPIO_PORTD_PCTL_R |= 0x00000004;
  GPIO_PORTD_AMSEL_R &= ~0x01;          // disable analog functionality on PD0
  GPIO_PORTD_DEN_R |= 0x01;             // enable digital I/O on PD0
  SYSCTL_RCC_R = 0x00100000 |           // 3) use PWM divider
      (SYSCTL_RCC_R & (~0x000E0000));   //    configure for /2 divider
  PWM0_3_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_3_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // PB6 goes low on LOAD
  // PB6 goes high on CMPA down
  PWM0_3_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_3_CMPA_R = duty - 1;             // 6) count value when output rises
  PWM0_3_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000040;          // enable PD0/M0PWM6
}
// change duty cycle of PB6
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM0G_Duty(uint16_t duty){
  PWM0_3_CMPA_R = duty - 1;             // 6) count value when output rises
}

void Start_Alarm(void){
	PWM0G_Init(20000,15000);
}

void Stop_Alarm(void){
	PWM0G_Init(1,1);
}
