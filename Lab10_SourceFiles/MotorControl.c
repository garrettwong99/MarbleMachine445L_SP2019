#include "MotorControl.h"
#include "PWM.h"
#include <stdint.h>

	uint16_t RevSec;
	uint16_t U;
	int E;
	uint16_t PropTerm;
	uint16_t IntTerm;
	
	uint16_t KP1;
	uint16_t KI1;
	uint16_t KP2;
	uint16_t KI2;
	uint16_t desiredSpeed;
	extern uint32_t speedActual;
		
void PI_Motor_Control(int speed){
	E = (speed/40) - speedActual;
	PropTerm = (KP1 * E) / KP2;
	if(PropTerm < 300) PropTerm = 300;
	if(PropTerm > 39900) PropTerm = 39900;
	
	IntTerm = IntTerm + (KI1 * E) / KI2;
	if(IntTerm < 300) IntTerm = 300;
	if(IntTerm > 39900) IntTerm = 39900;
	
	U = PropTerm + IntTerm;
	if(U < 300) U = 300;
	if(U > 39900) U = 39900;	
	PWM0B_Duty(U);
	
};

void MC_init(void){
	PWM0B_Init(40000,0); // TODO : what rate do I initialize this to
};
