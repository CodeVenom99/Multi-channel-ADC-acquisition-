#include "led.h"
#include "delay.h"


//��ʼ��PA0��PA1Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTBʱ��
	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PB0 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //����Ϊ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOB0,1	
	GPIO_SetBits(GPIOE,GPIO_Pin_5);				 //PB0 �����


}

