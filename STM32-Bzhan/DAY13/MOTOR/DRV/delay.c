#include <stm32f4xx.h>
#include <delay.h>

void systick_init(void)
{
	//ѡ��ο�ʱ�� ------ 21M
	//SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

//������ʱ 1ms = 21000  (2^24-1)/21000 ----------- nms<=798
void delay_ms(unsigned int nms)
{
	//�����ʼֵ�����ʼֵ�Ĵ���
	SysTick->LOAD = 21000*nms-1;
	SysTick->VAL = 0;//��ǰ����ֵΪ0
	
	//����systick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	//�ȴ���ʱʱ�䵽,���ƼĴ�����16λΪ1
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	
	//�ر�systick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//΢����ʱ  1us = 21  (2^24-1)/21 ----------- nus<=798915
void delay_us(unsigned int nus)
{
	//�����ʼֵ�����ʼֵ�Ĵ���
	SysTick->LOAD = 21*nus-1;
	SysTick->VAL = 0;//��ǰ����ֵΪ0
	
	//����systick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	//�ȴ���ʱʱ�䵽,���ƼĴ�����16λΪ1
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	
	//�ر�systick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//����ʱ
void delay_s(unsigned int ns)
{
	while(ns--){
		delay_ms(500);
		delay_ms(500);
	}
}

