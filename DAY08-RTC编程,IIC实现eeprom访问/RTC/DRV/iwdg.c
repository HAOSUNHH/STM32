#include <stm32f4xx.h>
#include <iwdg.h>

void iwdg_init(void)
{
	//1.关闭寄存器写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//2.设置看门狗的分频系数和初始计数值
	IWDG_SetPrescaler(IWDG_Prescaler_32);//1KHz
	IWDG_SetReload(1000);//1s
	
	//3.喂狗
	IWDG_ReloadCounter();
	
	//4.使能看门狗
	IWDG_Enable();
}
