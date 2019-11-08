#ifndef __ADC_H
#define __ADC_H	 

#include "sys.h"

#define Input_A  PBout(5)
#define Input_B  PBout(6)
#define Input_C  PBout(7)

void ADC1_GPIO_Config(void);
void ADC1_Mode_Config(void);
u16 GetVolt(u16 advalue);
void filter(void);
void distance(void);
void minimum_value(void);
void date_change(void);		 				    
#endif
