#include "../Inc/stm32f10x.h"


 void Delay(u32 count)
 {
   u32 i=0;
   for(;i<count;i++);
 }
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

 int main(void)
 {
     GPIO_Configuration();

  while(1)
	{
	    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//	    GPIO_SetBits(GPIOE,GPIO_Pin_5);
//		Delay(1000000);
//		GPIO_SetBits(GPIOC,GPIO_Pin_13);
//		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//		Delay(1000000);
	}
 }
