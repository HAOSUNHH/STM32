#include <stm32f4xx.h>
#include <led.h>
#include <beep.h>
#include <key.h>
#include <mq2.h>

int main()
{
	//1.�����ж����ȼ����� 2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	//beep_reg_init();
	beep_lib_init();
	//key_init();
	exti_init();
	mq2_init();
	
	
	while(1){
		if(MQ2){//δ����
			BEEP = 0;
		}
		else{
			BEEP = 1;
		}
	}
}
