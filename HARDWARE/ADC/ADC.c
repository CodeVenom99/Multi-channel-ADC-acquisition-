#include "ADC.h"
#include "delay.h"
#define N 50 //每通道采50次
#define M 4 //为4个通道
/*定义数组变量AD_Value[N][M],分别放AD1 四个通道转换的数据*/
vu16 AD_Value[N][M]; 
vu16 After_filter[M]; //用来存放求平均值之后的结果
extern u16 send[8],value[8],array[8],i,j,k;
extern u8 send_date[32];
 void ADC1_GPIO_Config(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	/* 使能 DMA 时钟 */
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
 /*使能 ADC1 and GPIOC 时钟 */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);

//片选口	
//PB.5 端口配置，用来控制模拟开关的输入
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//PB.6 端口配置，用来控制模拟开关的输入
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
//PB.7 端口配置，用来控制模拟开关的输入
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_ResetBits(GPIOB,GPIO_Pin_7);

//AD采样口，对应四个模拟开关
//PC0/1/2/3 作为模拟通道输入引脚
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


void ADC1_Mode_Config(void)
{
 DMA_InitTypeDef  DMA_InitStructure;
 ADC_InitTypeDef  ADC_InitStructure;
 /* 将与DMA有关的寄存器设为初始值 */
 DMA_DeInit(DMA1_Channel1);
 /*定义DMA外设基地址, 作用就是告诉DMA取数就到 ADC1->DR 这里来取。*/
 DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&ADC1->DR;
 /*定义内存基地址，即告诉DMA要将从AD中取来的数放到AD_Value中 */
 DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&AD_Value;
 /*定义AD外设作为数据传输的来源，即告诉DMA是将AD中的数据取出放到内存中，不能反过来*/
 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/*设定寄存器地址固定，即告诉DMA，只从固定的一个地方取数*/
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 /*设定内存地址固定，即每次DMA，，只将数搬到固定的内存中*/
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 /*指定DMA通道的DMA缓存的大小,即告诉DMA开辟几个内存空间，由于我们取四个通道、每个通道50个AD数据，所以需开辟N*M内存空间*/
 DMA_InitStructure.DMA_BufferSize = N*M;
/*设定外设数据宽度，即告诉DMA要取的数的大小*/	
DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	/*设定内存的的宽度*/
 DMA_InitStructure.DMA_MemoryDataSize= DMA_MemoryDataSize_HalfWord;
	/*设定DMA工作再循环缓存模式，即告诉DMA要不停的搬运*/
 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
 /*设定DMA选定的通道软件优先级*/
 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
 DMA_Init(DMA1_Channel1, &DMA_InitStructure);
 /* Enable DMA channel1，现在只用 DMA1_Channel1*/
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
/*设置ADC工作在独立模式*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	/*规定AD转换工作在扫描模式，即对多个通道采样*/
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ;
	/*设定AD转化在连续模式*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	/*不使用外部促发转换*/
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	/*采集的数据在寄存器中以右对齐的方式存放*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	/*设定要转换的AD通道数目*/
	ADC_InitStructure.ADC_NbrOfChannel = M;
	ADC_Init(ADC1, &ADC_InitStructure);
	
/*配置ADC时钟，为PCLK2 6分频，即12MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
//ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_239Cycles5);
	
/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/*复位校准寄存器 */
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	/* 开始转换规则通道 */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

u16 GetVolt(u16 advalue)//求AD转换后的结果
{
//求的结果扩大了100倍，方便下面求出小数
return (u16)(advalue * 330 / 4096); 
}
void filter(void)//求每个通道的平均值
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
		//模拟开关取第零路输入
		PBout(5)=0;
		PBout(6)=0;
		PBout(7)=0;
		delay_ms(5);		
		//等待传输完成否则第一位数据容易丢失
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
		//模拟开关取第一路输入
		PBout(5)=1;
		PBout(6)=0;
		PBout(7)=0;	
		delay_ms(5);
		//等待传输完成否则第一位数据容易丢失
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		filter();
		for(i=0;i<4;i++)
			{
			send[i+4]=After_filter[i];	//send数组只有8个	
			value[i+4]= GetVolt(After_filter[i]);
			array[i+4]=(int)(24.08/((value[i+4]/100.0)-0.13));
			}	

}
void minimum_value(void)
{
		for(i=j=k=0;i<8;i++) //找出最大数、最小数在数组中对应的下标
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
	send_date[0]=0x1a;//第一位表示要发送的数据个数
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

