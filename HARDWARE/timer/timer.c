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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM1��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 4999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =143; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx					 
}


void TIM1_UP_IRQHandler(void)   //TIM1�ж�10ms
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
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
//		minimum_value();//������Сֵ





