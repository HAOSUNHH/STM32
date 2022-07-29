#include <stm32f4xx.h>
#include <includes.h>
#include <stdio.h>

int main()
{
	//1.配置中断优先级分组 2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	beep_lib_init();
	exti_init();
	usart1_init();
	usart3_init();
	sr04_init();
	I2C_Configuration();
	OLED_Init();

	printf("app start!\r\n");
	
	motor_init();
	
	parse_cmd();
	
	while(1){
		
		delay_s(1);
	}
	
}
