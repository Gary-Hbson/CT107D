#ifndef PWM_H_
#define PWM_H_

#include "drive.h"

void PWMInit(uint16_t freq);
void SetDuty(uint8_t duty);

#endif
