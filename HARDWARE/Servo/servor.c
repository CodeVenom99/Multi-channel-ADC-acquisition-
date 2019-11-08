#include "servor.h"
#include "delay.h"
#include "usart.h"
extern u16 k,angle,servor_pwm;

void TIM2_PWM_Init(u16 arr,u16 psc)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);  
	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2,ENABLE);  
}

void servor_control(void)
{
			switch(k)
			{
				case 0:angle=0;break;
				case 1:angle=90;break;
				case 2:angle=180;break;
				case 3:angle=270;break;
				case 4:angle=45;break;
				case 5:angle=135;break;
				case 6:angle=225;break;
				case 7:angle=315;break;
	
			}
			switch(angle)
			{
				case 0:servor_pwm=5;break;
				case 45:servor_pwm=10;break;				
				case 90:servor_pwm=15;break;
				case 135:servor_pwm=20;break;				
				case 180:servor_pwm=25;break;	
				default:break;				
			}		
			TIM_SetCompare3(TIM2,servor_pwm);
}
