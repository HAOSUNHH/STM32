#include <adc.h>

void adc1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	//1.ʹ��GPIO��ADCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	//2.��ʼ��GPIOA5Ϊģ�⹦��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;//
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//3.��λADC
	ADC_DeInit();
	
	//4.ADCͨ�ó�ʼ��
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//4��Ƶ
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//�ر�DMA
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���β���ʱ����
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//5.��ʼ��ADC1
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//�ֱ���12λ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//�رն�ͨ��ɨ��
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ʹ���ⲿ����
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���
	ADC_InitStruct.ADC_NbrOfConversion = 1;//ת�������еĹ�����
	ADC_Init(ADC1,&ADC_InitStruct);
	
	//6.ʹ��ADC
	ADC_Cmd(ADC1,ENABLE);
	
	//7.���ù���ͨ������  ADC1 ͨ��5 ������ ��������=3
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
}

void adc3_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	//1.ʹ��GPIO��ADCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	
	//2.��ʼ��GPIOA5Ϊģ�⹦��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;//
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//3.��λADC
	ADC_DeInit();
	
	//4.ADCͨ�ó�ʼ��
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//4��Ƶ
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//�ر�DMA
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���β���ʱ����
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//5.��ʼ��ADC3
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//�ֱ���12λ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//�رն�ͨ��ɨ��
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ʹ���ⲿ����
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���
	ADC_InitStruct.ADC_NbrOfConversion = 1;//ת�������еĹ�����
	ADC_Init(ADC3,&ADC_InitStruct);
	
	//6.ʹ��ADC
	ADC_Cmd(ADC3,ENABLE);
	
	//7.���ù���ͨ������  ADC1 ͨ��5 ������ ��������=3
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
}

//����ת�����õ����
u16 get_adc1_vol(void)
{
	//1.����ת��
	ADC_SoftwareStartConv(ADC1);
	//2.�ȴ�ת�����
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)!=SET);
	//3.��ȡת�����
	return ADC_GetConversionValue(ADC1);
}

u16 get_adc3_vol(void)
{
	//1.����ת��
	ADC_SoftwareStartConv(ADC3);
	//2.�ȴ�ת�����
	while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC)!=SET);
	//3.��ȡת�����
	return ADC_GetConversionValue(ADC3);
}
