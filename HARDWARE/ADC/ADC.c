#include "ADC.h"
#include "delay.h"
#define N 50 //ÿͨ����50��
#define M 4 //Ϊ4��ͨ��
/*�����������AD_Value[N][M],�ֱ��AD1 �ĸ�ͨ��ת��������*/
vu16 AD_Value[N][M]; 
vu16 After_filter[M]; //���������ƽ��ֵ֮��Ľ��
extern u16 send[8],value[8],array[8],i,j,k;
extern u8 send_date[32];
 void ADC1_GPIO_Config(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	/* ʹ�� DMA ʱ�� */
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
 /*ʹ�� ADC1 and GPIOC ʱ�� */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);

//Ƭѡ��	
//PB.5 �˿����ã���������ģ�⿪�ص�����
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//PB.6 �˿����ã���������ģ�⿪�ص�����
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
//PB.7 �˿����ã���������ģ�⿪�ص�����
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_ResetBits(GPIOB,GPIO_Pin_7);

//AD�����ڣ���Ӧ�ĸ�ģ�⿪��
//PC0/1/2/3 ��Ϊģ��ͨ����������
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


void ADC1_Mode_Config(void)
{
 DMA_InitTypeDef  DMA_InitStructure;
 ADC_InitTypeDef  ADC_InitStructure;
 /* ����DMA�йصļĴ�����Ϊ��ʼֵ */
 DMA_DeInit(DMA1_Channel1);
 /*����DMA�������ַ, ���þ��Ǹ���DMAȡ���͵� ADC1->DR ������ȡ��*/
 DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&ADC1->DR;
 /*�����ڴ����ַ��������DMAҪ����AD��ȡ�������ŵ�AD_Value�� */
 DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&AD_Value;
 /*����AD������Ϊ���ݴ������Դ��������DMA�ǽ�AD�е�����ȡ���ŵ��ڴ��У����ܷ�����*/
 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/*�趨�Ĵ�����ַ�̶���������DMA��ֻ�ӹ̶���һ���ط�ȡ��*/
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 /*�趨�ڴ��ַ�̶�����ÿ��DMA����ֻ�����ᵽ�̶����ڴ���*/
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 /*ָ��DMAͨ����DMA����Ĵ�С,������DMA���ټ����ڴ�ռ䣬��������ȡ�ĸ�ͨ����ÿ��ͨ��50��AD���ݣ������迪��N*M�ڴ�ռ�*/
 DMA_InitStructure.DMA_BufferSize = N*M;
/*�趨�������ݿ�ȣ�������DMAҪȡ�����Ĵ�С*/	
DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	/*�趨�ڴ�ĵĿ��*/
 DMA_InitStructure.DMA_MemoryDataSize= DMA_MemoryDataSize_HalfWord;
	/*�趨DMA������ѭ������ģʽ��������DMAҪ��ͣ�İ���*/
 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
 /*�趨DMAѡ����ͨ��������ȼ�*/
 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
 DMA_Init(DMA1_Channel1, &DMA_InitStructure);
 /* Enable DMA channel1������ֻ�� DMA1_Channel1*/
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
/*����ADC�����ڶ���ģʽ*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	/*�涨ADת��������ɨ��ģʽ�����Զ��ͨ������*/
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ;
	/*�趨ADת��������ģʽ*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	/*��ʹ���ⲿ�ٷ�ת��*/
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	/*�ɼ��������ڼĴ��������Ҷ���ķ�ʽ���*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	/*�趨Ҫת����ADͨ����Ŀ*/
	ADC_InitStructure.ADC_NbrOfChannel = M;
	ADC_Init(ADC1, &ADC_InitStructure);
	
/*����ADCʱ�ӣ�ΪPCLK2 6��Ƶ����12MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
//ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_239Cycles5);
	
/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/*��λУ׼�Ĵ��� */
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	/* ��ʼת������ͨ�� */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

u16 GetVolt(u16 advalue)//��ADת����Ľ��
{
//��Ľ��������100���������������С��
return (u16)(advalue * 330 / 4096); 
}
void filter(void)//��ÿ��ͨ����ƽ��ֵ
{
int sum = 0;
u8 count,i;
for(i=0;i<4;i++)
{
for ( count=0;count<N;count++)
{
sum += AD_Value[count][i];
}
After_filter[i]=sum/N;
sum=0;
}
}
void distance(void)
{
	//**************************************************************  0
		//ģ�⿪��ȡ����·����
		PBout(5)=0;
		PBout(6)=0;
		PBout(7)=0;
		delay_ms(5);		
		//�ȴ�������ɷ����һλ�������׶�ʧ
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		filter();
		for(i=0;i<4;i++)
			{
			send[i]=After_filter[i];
			value[i]= GetVolt(After_filter[i]);
			array[i]=(int)(24.08/((value[i]/100.0)-0.13));
			}	

//		printf("%d %d %d %d ",a,b,c,d);//26,20,31,19
		
//**************************************************************  1
		//ģ�⿪��ȡ��һ·����
		PBout(5)=1;
		PBout(6)=0;
		PBout(7)=0;	
		delay_ms(5);
		//�ȴ�������ɷ����һλ�������׶�ʧ
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		filter();
		for(i=0;i<4;i++)
			{
			send[i+4]=After_filter[i];	//send����ֻ��8��	
			value[i+4]= GetVolt(After_filter[i]);
			array[i+4]=(int)(24.08/((value[i+4]/100.0)-0.13));
			}	

}
void minimum_value(void)
{
		for(i=j=k=0;i<8;i++) //�ҳ����������С���������ж�Ӧ���±�
		if(array[i]>array[j])j=i;
			else if(array[i]<array[k])k=i;
}

void date_change(void)
{
	u8 shi[8],ge[8],i;	
	for(i=0;i<8;i++)
	{
		if((*(array+i))/10>9)
		{
		 (*(array+i))=90+(*(array+i))%10;
		}
	}	
	for(i=0;i<8;i++)
	{
	ge[i]=0x30+(*(array+i))%10;
	shi[i]=0x30+(*(array+i))/10;	
	}
	send_date[0]=0x1a;//��һλ��ʾҪ���͵����ݸ���
	for(i=0;i<8;i++)
	{
//	if(qian[i]==0x30)
//	{
//	qian[i]=0x20;
//	}
	send_date[1+i*3]=shi[i];
	send_date[2+i*3]=ge[i];	
	send_date[3+i*3]=0x20;	
	}
	send_date[24]=0x0d;
	send_date[25]=0x0a;
}

