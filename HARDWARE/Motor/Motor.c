#include "motor.h"
#include "delay.h"
float Velocity_KPA=30,Velocity_KIA=0,Velocity_KDA=600;		   //��30��0��600
float Velocity_KPB=30,Velocity_KIB=0,Velocity_KDB=600;	       //�ң�80��60
extern int Motor_A,Motor_B,Servo,Target_A,Target_B;  //�������������  

void Motor_Gpio_init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);// 


	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}




void PWM2_Init(u16 arr,u16 psc)//TIM5
{		 		
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;							//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc;						//����������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 				//�ڳ�ʼ�� TIMx

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //�������ģʽ ͨ��1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
	
	TIM_OC1Init(TIM5, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
	
	TIM_OC2Init(TIM5, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
	
	TIM_OC3Init(TIM5, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
	
	TIM_OC4Init(TIM5, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
	
	TIM_Cmd(TIM5, ENABLE); //��ʹ�� TIM2
	
	TIM_SetCompare1(TIM5,0);
	TIM_SetCompare2(TIM5,0);
	TIM_SetCompare3(TIM5,0);
	TIM_SetCompare4(TIM5,0);
	
	return;

	
} 


void Set_Pwm_Motor1(int motor_a)
{
    	if(motor_a<0)			PWMA2=7200,PWMA1=7200+motor_a;
			else 	            PWMA1=7200,PWMA2=7200-motor_a;
}


void Set_Pwm_Motor2(int motor_b)
{
		  if(motor_b<0)			PWMB1=7200,PWMB2=7200+motor_b;
			else 	            PWMB2=7200,PWMB1=7200-motor_b;
}


/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int BiasA,PwmA,Last_biasA,Integral_biasA;
	 BiasA=Target-Encoder;                //����ƫ��
	 Integral_biasA+=BiasA;
	 PwmA+=Velocity_KPA*BiasA+Velocity_KIA*Integral_biasA+Velocity_KDA*(BiasA-Last_biasA);   //����ʽPI������
	 Last_biasA=BiasA;	                   //������һ��ƫ�� 
	 return PwmA;                         //�������
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int BiasB,PwmB,Last_biasB,Integral_biasB;
	 BiasB=Target-Encoder;  
	//����ƫ��
	 PwmB+=Velocity_KPB*BiasB+Velocity_KIB*Integral_biasB+Velocity_KDB*(BiasB-Last_biasB);   //����ʽPI������
	 Last_biasB=BiasB;	                   //������һ��ƫ�� 
	 return PwmB;                         //�������
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	 
	  int Amplitude=7100;    //===PWM������7199 ������7100
    if(Motor_A<-Amplitude) Motor_A=-Amplitude;	
		if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	
		if(Motor_B>Amplitude)  Motor_B=Amplitude;		
}
