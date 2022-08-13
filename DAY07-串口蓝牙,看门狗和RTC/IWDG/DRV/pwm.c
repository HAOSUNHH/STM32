#include <stm32f4xx.h>
#include <pwm.h>

void timer14_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//1.ʹ��GPIOF��TIM14ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	//2.��ʼ��PF9Ϊ���ù���
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//PF9
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//3.��TIM14 channel 1ӳ�䵽PF9
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	//4.��ʼ����ʱ��14�����ڶ�
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;//Ԥ��Ƶϵ��
	//1MHz 1000 = 1ms
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//������ʼֵ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//����ģʽ - ����
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ������
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	//5.��ʼ��PWM ----- ͨ��1
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWMģʽ1
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//���ԣ��͵�ƽ��Ч
	TIM_OCInitStruct.TIM_Pulse = 500;//�Ƚϼ���ֵ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//ʹ��
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);
	
	//6.ʹ��TIM14��Ԥװ�غ���װ�ع���
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	
	//7.������ʱ��
	TIM_Cmd(TIM14,ENABLE);
}	

void timer1_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//1.ʹ��GPIOF��TIM14ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	//2.��ʼ��PE14Ϊ���ù���
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;//PE14
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//3.��TIM1 channe4 1ӳ�䵽PE14
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	
	//4.��ʼ����ʱ��1�����ڶ�
	TIM_TimeBaseInitStruct.TIM_Prescaler = 168-1;//Ԥ��Ƶϵ��
	//1MHz 1000 = 1ms
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//������ʼֵ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//����ģʽ - ����
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ������
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//5.��ʼ��PWM ----- ͨ��1
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWMģʽ1
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//���ԣ��͵�ƽ��Ч
	TIM_OCInitStruct.TIM_Pulse = 500;//�Ƚϼ���ֵ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//ʹ��
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	
	//6.ʹ��TIM14��Ԥװ�غ���װ�ع���
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	//7.������ʱ��
	TIM_Cmd(TIM1,ENABLE);
	//TIM1�Ǹ߼���ʱ���������
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}	
