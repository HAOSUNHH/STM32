#include <stm32f4xx.h>
#include <beep.h>

void beep_reg_init(void)
{
	//开启GPIOF 
	RCC->AHB1ENR |= (0x1<<5);
	//配置PF8 推挽输出，无上下拉，高速
	GPIOF->MODER &= ~(0x3<<16);
	GPIOF->MODER |= (0x1<<16);//01
	GPIOF->OTYPER &= ~(0x1<<8);//0
	GPIOF->OSPEEDR &= ~(0x3<<16);
	GPIOF->OSPEEDR |= (0x2<<16);//10
	GPIOF->PUPDR &= ~(0x3<<16);//00
	//默认不响
	GPIOF->BSRRH |= (0x1<<8);
}	

void beep_lib_init(void)
{
	//GPIO初始化结构
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.开启GPIOE F时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//2.初始化PF8
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//PF8
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//默认输出低 ----- 不响
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
}	

//flag:0 - 不响 1 - 响
void beep_onoff(int flag)
{
	if(flag){
		//GPIOF->BSRRL |= (0x1<<8);
		//GPIO_SetBits(GPIOF,GPIO_Pin_8);
		BEEP = 1;
	}
	else{
		//GPIOF->BSRRH |= (0x1<<8);
		//GPIO_ResetBits(GPIOF,GPIO_Pin_8);
		BEEP = 0;
	}
	
}
