#include "timer.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "encoder.h"
#include "motor.h"
#include "servor.h"
#include "adc.h"
#include "24l01.h"
#include "spi.h"
u16 send[8],value[8],array[8],i,j,k;
int Encoder_Left,Encoder_Right;
u8 t,servor_pwm,send_date[32];
u16 vout,adc_date;
int Motor_A,Motor_B,juli,target,angle;
extern int tiaojieA,Encoder_A_EXTI, Encoder_B_EXTI;
extern u16 date[2];		 
extern u8 tmp_buf[5];
void TIM1_Int_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	
	//定时器TIM1初始化
	TIM_TimeBaseStructure.TIM_Period = 4999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =143; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM1, ENABLE);  //使能TIMx					 
}


void TIM1_UP_IRQHandler(void)   //TIM1中断10ms
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{									
		t++;	
		if(t==10)
		{
			t=0;
			LED=!LED;
			distance();
			date_change();
			if(NRF24L01_TxPacket(send_date)==TX_OK)
			{				
				LED=0;
			}
			else
			{
				LED=1;				
//				printf("Send Failed\r\n");			   
			};  
//			printf("%d %d \r\n",array[4],value[4]);			
	
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );			
		TIM3 -> CNT=0;Encoder_A_EXTI=0;				
		}
}
//		minimum_value();//计算最小值





