#include "led.h"
#include "delay.h"


//初始化PA0和PA1为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTB时钟
	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PB0 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOB0,1	
	GPIO_SetBits(GPIOE,GPIO_Pin_5);				 //PB0 输出高


}

