#include <stm32f4xx.h>
#include <motor.h>

void motor_init(void)
{
	//GPIO��ʼ���ṹ
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.����GPIOG C Bʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB,ENABLE);
	
	//2.��ʼ��Ϊ�������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;//PB6
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9;//PC7 PC9
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;//PG15
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	IN1 = 0;
	IN2 = 0;
	IN3 = 0;
	IN4 = 0;
}
