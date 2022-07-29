#include <dac.h>

void dac_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	DAC_InitTypeDef DAC_InitStruct;
	
	//1.ʹ��GPIOA��DACʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	//2.��ʼ��GPIOA4λģ�⹦��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;//PA4
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//3.��ʼ��DAC
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;//��ʹ���ⲿ����
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;//����������
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;//������������
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//��ʹ�û�����
	DAC_Init(DAC_Channel_1,&DAC_InitStruct);
	
	//4.ʹ��DACͨ��1
	DAC_Cmd(DAC_Channel_1,ENABLE);
}


//�������ֵ
void set_vol(u16 vol)
{
	DAC_SetChannel1Data(DAC_Channel_1,vol);
}
