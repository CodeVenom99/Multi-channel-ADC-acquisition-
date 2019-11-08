#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "motor.h"
#include "encoder.h"
#include "timer.h"
#include "servor.h"
#include "adc.h"
#include "24l01.h"
#include "spi.h"
//#include ".h"
	u16 date[2];		 
	u8 tmp_buf[5];
 int main(void)
 {	

	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	uart_init(115200);
	LED_Init(); 
	 
	NRF24L01_Init();    	//初始化NRF24L01   

 	while(NRF24L01_Check())	//检查NRF24L01是否在位.	
	{
	printf("NRF24L01 Error\r\n");
	}								   
	printf("NRF24L01 OK\r\n");
	
	NRF24L01_TX_Mode();
	
	Encoder_Init_TIM3();
	Encoder_Init_TIM_Exit0();
	Encoder_Init_TIM_Exit1();
	 
	Motor_Gpio_init();
	PWM2_Init(7199,0);
		
  TIM2_PWM_Init(199,7199);
	 
	ADC1_GPIO_Config();
	ADC1_Mode_Config();

	TIM1_Int_Init();
	while(1);
 }
