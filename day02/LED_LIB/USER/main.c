#include <stm32f4xx.h>
#include <led.h>


int main()
{
	led_init();
	
	while(1){
		//¡¡
		GPIO_ResetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
		GPIO_ResetBits(GPIOE,GPIO_Pin_13|GPIO_Pin_14);
		//—” ±
		delay(500);
		
		//√
		GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
		GPIO_SetBits(GPIOE,GPIO_Pin_13|GPIO_Pin_14);
		//—” ±
		delay(500);
	}
}
