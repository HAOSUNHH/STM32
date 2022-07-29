#include <dac.h>

void dac_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	DAC_InitTypeDef DAC_InitStruct;
	
	//1.使能GPIOA和DAC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	//2.初始化GPIOA4位模拟功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;//PA4
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//3.初始化DAC
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;//不使用外部触发
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;//不产生波形
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;//噪声波形配置
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//不使用缓冲区
	DAC_Init(DAC_Channel_1,&DAC_InitStruct);
	
	//4.使能DAC通道1
	DAC_Cmd(DAC_Channel_1,ENABLE);
}


//设置输出值
void set_vol(u16 vol)
{
	DAC_SetChannel1Data(DAC_Channel_1,vol);
}
