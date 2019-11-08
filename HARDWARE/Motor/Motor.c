#include "motor.h"
#include "delay.h"
float Velocity_KPA=30,Velocity_KIA=0,Velocity_KDA=600;		   //左30，0，600
float Velocity_KPB=30,Velocity_KIB=0,Velocity_KDB=600;	       //右，80，60
extern int Motor_A,Motor_B,Servo,Target_A,Target_B;  //电机舵机控制相关  

void Motor_Gpio_init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);// 


	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}




void PWM2_Init(u16 arr,u16 psc)//TIM5
{		 		
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;							//设置在下一个更新事件装入活动的自动重装载寄存器周期的值 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc;						//设置用来作为 TIMx 时钟频率除数的预分频值 不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 				//②初始化 TIMx

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //脉宽调制模式 通道1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
	
	TIM_OC1Init(TIM5, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable); 	//CH1 预装载使能
	
	TIM_OC2Init(TIM5, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); 	//CH1 预装载使能
	
	TIM_OC3Init(TIM5, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable); 	//CH1 预装载使能
	
	TIM_OC4Init(TIM5, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable); 	//CH1 预装载使能
	
	TIM_Cmd(TIM5, ENABLE); //④使能 TIM2
	
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
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int BiasA,PwmA,Last_biasA,Integral_biasA;
	 BiasA=Target-Encoder;                //计算偏差
	 Integral_biasA+=BiasA;
	 PwmA+=Velocity_KPA*BiasA+Velocity_KIA*Integral_biasA+Velocity_KDA*(BiasA-Last_biasA);   //增量式PI控制器
	 Last_biasA=BiasA;	                   //保存上一次偏差 
	 return PwmA;                         //增量输出
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int BiasB,PwmB,Last_biasB,Integral_biasB;
	 BiasB=Target-Encoder;  
	//计算偏差
	 PwmB+=Velocity_KPB*BiasB+Velocity_KIB*Integral_biasB+Velocity_KDB*(BiasB-Last_biasB);   //增量式PI控制器
	 Last_biasB=BiasB;	                   //保存上一次偏差 
	 return PwmB;                         //增量输出
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	
	 
	  int Amplitude=7100;    //===PWM满幅是7199 限制在7100
    if(Motor_A<-Amplitude) Motor_A=-Amplitude;	
		if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	
		if(Motor_B>Amplitude)  Motor_B=Amplitude;		
}
