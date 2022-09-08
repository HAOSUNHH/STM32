#include <stm32f4xx.h>
#include <sr04.h>
#include <delay.h>
#include <sys.h>

void sr04_init(void)
{
	//GPIO初始化结构
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.开启GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);
	
	//2.初始化PE6为推挽输出 PA8为浮空输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;//PE6
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	//默认输出低电平
	TRIG = 0;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//PA8
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}

//测量距离
int get_distance(void)
{
	u32 retry = 0,count = 0;
	
	//TRIG发送>10us高脉冲
	TRIG = 1;
	delay_us(15);
	TRIG = 0;
	
	//等待ECHO变高,等待60ms还没有变高，直接返回-1
	while(ECHO==0){
		retry++;
		delay_us(1);
		if(retry>60000)
			return -1;
	}
	
	//重置启动定时器
	TIM_SetCounter(TIM2,100000);
	TIM_Cmd(TIM2,ENABLE);
	
	retry = 0;
	//等待ECHO变低，计算高电平时间
	while(ECHO==1){
		retry++;
		delay_us(1);
		if(retry>60000){
			TIM_Cmd(TIM2,DISABLE);
			return -1;
		}
	}
	
	//读取定时器的计数值，关闭定时器
	count = TIM_GetCounter(TIM2);
	TIM_Cmd(TIM2,DISABLE);
	
	//return retry*10/58;
	return (100000-count)/58;
}
