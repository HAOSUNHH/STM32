#include <stm32f4xx.h>
#include <mq2.h>

void mq2_init(void)
{
	//GPIO��ʼ���ṹ
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.����GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//2.��ʼ��PA0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;//PA2
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
