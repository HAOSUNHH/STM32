#include <stm32f4xx.h>
#include <led.h>
#include <beep.h>
#include <key.h>
#include <mq2.h>
#include <delay.h>

int main()
{
	//1.配置中断优先级分组 2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	//beep_reg_init();
	beep_lib_init();
	//key_init();
	exti_init();
	mq2_init();
	
	
	while(1){
		if(flag){
			BEEP = 0;
			
			D1 = 0;
			delay_ms(500);
			D1 = 1;
			delay_ms(500);
		}
		else{
			D1 = 1;
			
			if(MQ2){
				BEEP = 0;
			}
			else{//报警
				BEEP = 1;
			}
		}
	}
}
