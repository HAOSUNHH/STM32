#include <stm32f4xx.h>
#include <pwm.h>

void timer14_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//1.使能GPIOF和TIM14时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	//2.初始化PF9为复用功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//PF9
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//3.将TIM14 channel 1映射到PF9
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	//4.初始化定时器14，周期短
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;//预分频系数
	//1MHz 1000 = 1ms
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//计数初始值
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//计数模式 - 向下
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//采样时钟因子
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	//5.初始化PWM ----- 通道1
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM模式1
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//极性，低电平有效
	TIM_OCInitStruct.TIM_Pulse = 500;//比较计数值
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//使能
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);
	
	//6.使能TIM14的预装载和重装载功能
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	
	//7.启动定时器
	TIM_Cmd(TIM14,ENABLE);
}	

void timer1_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//1.使能GPIOF和TIM14时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	//2.初始化PE14为复用功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;//PE14
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//3.将TIM1 channe4 1映射到PE14
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	
	//4.初始化定时器1，周期短
	TIM_TimeBaseInitStruct.TIM_Prescaler = 168-1;//预分频系数
	//1MHz 1000 = 1ms
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//计数初始值
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//计数模式 - 向下
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//采样时钟因子
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//5.初始化PWM ----- 通道1
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM模式1
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//极性，低电平有效
	TIM_OCInitStruct.TIM_Pulse = 500;//比较计数值
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//使能
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	
	//6.使能TIM14的预装载和重装载功能
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	//7.启动定时器
	TIM_Cmd(TIM1,ENABLE);
	//TIM1是高级定时器，另外打开
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}	
