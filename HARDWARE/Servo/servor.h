#ifndef __SERVOR_H
#define	__SERVOR_H

#include "stm32f10x.h"

void TIM2_PWM_Init(u16 arr,u16 psc);
void servor_control(void);
#endif 
