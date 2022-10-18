#include <adc.h>

void adc1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	//1.使能GPIO和ADC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	//2.初始化GPIOA5为模拟功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;//
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//3.复位ADC
	ADC_DeInit();
	
	//4.ADC通用初始化
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//4分频
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//关闭DMA
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两次采样时间间隔
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//5.初始化ADC1
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//分辨率12位
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//关闭多通道扫描
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//不使用外部触发
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	ADC_InitStruct.ADC_NbrOfConversion = 1;//转换序列中的规则数
	ADC_Init(ADC1,&ADC_InitStruct);
	
	//6.使能ADC
	ADC_Cmd(ADC1,ENABLE);
	
	//7.配置规则通道参数  ADC1 通道5 单规则 采样周期=3
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
}

void adc3_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	//1.使能GPIO和ADC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	
	//2.初始化GPIOA5为模拟功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;//
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//3.复位ADC
	ADC_DeInit();
	
	//4.ADC通用初始化
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//4分频
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//关闭DMA
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两次采样时间间隔
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//5.初始化ADC3
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//分辨率12位
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//关闭多通道扫描
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//不使用外部触发
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	ADC_InitStruct.ADC_NbrOfConversion = 1;//转换序列中的规则数
	ADC_Init(ADC3,&ADC_InitStruct);
	
	//6.使能ADC
	ADC_Cmd(ADC3,ENABLE);
	
	//7.配置规则通道参数  ADC1 通道5 单规则 采样周期=3
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
}

//开启转换，得到结果
u16 get_adc1_vol(void)
{
	//1.开启转换
	ADC_SoftwareStartConv(ADC1);
	//2.等待转换完成
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)!=SET);
	//3.获取转换结果
	return ADC_GetConversionValue(ADC1);
}

u16 get_adc3_vol(void)
{
	//1.开启转换
	ADC_SoftwareStartConv(ADC3);
	//2.等待转换完成
	while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC)!=SET);
	//3.获取转换结果
	return ADC_GetConversionValue(ADC3);
}
