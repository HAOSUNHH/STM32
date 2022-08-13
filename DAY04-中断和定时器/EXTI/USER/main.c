#include <stm32f4xx.h>
#include <led.h>
#include <beep.h>
#include <key.h>

int main()
{
	//1.配置中断优先级分组 2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	//beep_reg_init();
	beep_lib_init();
	//key_init();
	exti_init();
	
	
	while(1){
		//beep_onoff(1);
		delay(500);
		
		beep_onoff(0);
		delay(500);
	}
}

