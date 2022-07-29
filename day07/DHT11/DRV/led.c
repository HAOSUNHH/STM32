#include <stm32f4xx.h>
#include <led.h>

void delay(unsigned int ms)
{
	int i,j;
	
	for(i=0;i<ms;i++)
		for(j=0;j<5000;j++);
}

//LED初始化
void led_init(void)
{
	//GPIO初始化结构
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.开启GPIOE F时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF,ENABLE);
	
	//2.初始化PF9 PF10 PE13 PE14	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//PF9 PF10
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//PE13 PE14
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//默认输出高 ----- 灭
	GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_13|GPIO_Pin_14);
}
