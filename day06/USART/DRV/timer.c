#include <stm32f4xx.h>
#include <timer.h>
#include <beep.h>
#include <led.h>

void timer2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.开启定时器2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//1s
	//2.初始化定时器 84MHz  8400分频 = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//预分频系数
	//10KHz 10000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = 10000-1;//计数初始值
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//计数模式 - 向下
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//采样时钟因子
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//3.NVIC初始化
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//TIM2
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
	
	//4.开启定时器超时中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	//6.启动定时器2
	TIM_Cmd(TIM2,ENABLE);
}

void timer10_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.开启定时器10时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	
	//200ms
	//2.初始化定时器 168MHz  16800分频 = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800-1;//预分频系数
	//10KHz 2000 = 200ms
	TIM_TimeBaseInitStruct.TIM_Period = 2000-1;//计数初始值
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//计数模式 - 向下
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//采样时钟因子
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStruct);
	
	//3.NVIC初始化
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;//TIM10
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
	
	//4.开启定时器超时中断
	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
	
	//6.启动定时器10
	TIM_Cmd(TIM10,ENABLE);
}

//5.定时器2中断处理函数
void TIM2_IRQHandler(void)
{
	//判断是否是更新中断
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		//LED闪烁
		D1 = ~D1;
		
		//清楚中断标志
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

//定时器10中断处理函数
void TIM1_UP_TIM10_IRQHandler(void)
{
	//判断是否是更新中断
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET){
		//LED闪烁
		D4 = ~D4;
		
		//清楚中断标志
		TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
	}
}
