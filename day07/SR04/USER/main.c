#include <stm32f4xx.h>
#include <includes.h>
#include <stdio.h>

int main()
{
	u32 comp = 0;
	
	
	//1.配置中断优先级分组 2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	//beep_reg_init();
	beep_lib_init();
	//key_init();
	exti_init();
	mq2_init();
	
	//timer10_init();
	//timer14_pwm_init();
	//timer1_pwm_init();
	usart1_init();
	timer2_init();
	
	sr04_init();
	
	
	printf("app start!\r\n");
	
	parse_cmd();
	
	/*
	while(1){
		printf("distance = %d\r\n",get_distance());
		
		delay_s(1);
	}
	*/
}
