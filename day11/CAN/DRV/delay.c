#include <stm32f4xx.h>
#include <delay.h>

void systick_init(void)
{
	//选择参考时钟 ------ 21M
	//SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

//毫秒延时 1ms = 21000  (2^24-1)/21000 ----------- nms<=798
void delay_ms(unsigned int nms)
{
	//计算初始值放入初始值寄存器
	SysTick->LOAD = 21000*nms-1;
	SysTick->VAL = 0;//当前计数值为0
	
	//启动systick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	//等待定时时间到,控制寄存器的16位为1
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	
	//关闭systick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//微秒延时  1us = 21  (2^24-1)/21 ----------- nus<=798915
void delay_us(unsigned int nus)
{
	//计算初始值放入初始值寄存器
	SysTick->LOAD = 21*nus-1;
	SysTick->VAL = 0;//当前计数值为0
	
	//启动systick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	//等待定时时间到,控制寄存器的16位为1
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	
	//关闭systick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//秒延时
void delay_s(unsigned int ns)
{
	while(ns--){
		delay_ms(500);
		delay_ms(500);
	}
}

