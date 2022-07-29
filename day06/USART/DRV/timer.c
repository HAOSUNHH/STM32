#include <stm32f4xx.h>
#include <timer.h>
#include <beep.h>
#include <led.h>

void timer2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.������ʱ��2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//1s
	//2.��ʼ����ʱ�� 84MHz  8400��Ƶ = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//Ԥ��Ƶϵ��
	//10KHz 10000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = 10000-1;//������ʼֵ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//����ģʽ - ����
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ������
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//3.NVIC��ʼ��
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//TIM2
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	//4.������ʱ����ʱ�ж�
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	//6.������ʱ��2
	TIM_Cmd(TIM2,ENABLE);
}

void timer10_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.������ʱ��10ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	
	//200ms
	//2.��ʼ����ʱ�� 168MHz  16800��Ƶ = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800-1;//Ԥ��Ƶϵ��
	//10KHz 2000 = 200ms
	TIM_TimeBaseInitStruct.TIM_Period = 2000-1;//������ʼֵ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//����ģʽ - ����
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ������
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStruct);
	
	//3.NVIC��ʼ��
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;//TIM10
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	//4.������ʱ����ʱ�ж�
	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
	
	//6.������ʱ��10
	TIM_Cmd(TIM10,ENABLE);
}

//5.��ʱ��2�жϴ�����
void TIM2_IRQHandler(void)
{
	//�ж��Ƿ��Ǹ����ж�
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		//LED��˸
		D1 = ~D1;
		
		//����жϱ�־
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

//��ʱ��10�жϴ�����
void TIM1_UP_TIM10_IRQHandler(void)
{
	//�ж��Ƿ��Ǹ����ж�
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET){
		//LED��˸
		D4 = ~D4;
		
		//����жϱ�־
		TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
	}
}
