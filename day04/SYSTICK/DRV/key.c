#include <stm32f4xx.h>
#include <key.h>
#include <led.h>

void key_init(void)
{
	//GPIO��ʼ���ṹ
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.����GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);
	
	//2.��ʼ��PA0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//PA0
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_2;//PE2 PE3 PE4
	GPIO_Init(GPIOE,&GPIO_InitStruct);
}


void exti_init(void)
{
	//GPIO��ʼ���ṹ
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//2.����GPIOA SYSCFGʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//3.��ʼ��Ϊ����ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//PA0
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_2;//PE2 PE3 PE4
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//4.ѡ��IO�ں��ⲿ�ж��ߵ�ӳ���ϵ
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
	
	//5.��ʼ���ⲿ�ж�
	EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line2|EXTI_Line3|EXTI_Line4;//�ⲿ�ж�0 2 3 4
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//ʹ��
	EXTI_Init(&EXTI_InitStruct);
	
	//6.��ʼ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�3
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;//�ⲿ�ж�4
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
}

//7.��д�жϴ�����
void EXTI0_IRQHandler(void)
{
	D1 = 0;
	delay(10000);
	D1 = 1;
	
	//����жϱ�־
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI2_IRQHandler(void)
{
	D2 = 0;
	delay(10000);
	D2 = 1;
	
	//����жϱ�־
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{
	D3 = 0;
	delay(10000);
	D3 = 1;
	
	//����жϱ�־
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
	D4 = 0;
	delay(10000);
	D4 = 1;
	
	//����жϱ�־
	EXTI_ClearITPendingBit(EXTI_Line4);
}

