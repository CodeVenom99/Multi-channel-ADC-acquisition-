#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "sys.h"
#define PWMA1   TIM5->CCR1  
#define PWMA2   TIM5->CCR2 

#define PWMB1   TIM5->CCR3  
#define PWMB2   TIM5->CCR4
void PWM2_Init(u16 arr,u16 psc);
void Motor_Gpio_init(void);


void Set_Pwm_Motor1(int motor_a);
void Set_Pwm_Motor2(int motor_b);
int Incremental_PI_A (int Encoder,int Target);
int Incremental_PI_B (int Encoder,int Target);
void Xianfu_Pwm(void);
#endif
