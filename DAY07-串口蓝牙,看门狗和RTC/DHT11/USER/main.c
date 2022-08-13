#include <stm32f4xx.h>
#include <includes.h>
#include <stdio.h>

int main()
{
	//u32 comp = 0;
	u8 temp = 0,humi = 0;
	
	
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
	//timer2_init();
	sr04_init();
	
	printf("app start!\r\n");
	
	//等待dht11 >1s
	delay_s(2);
	
	if(dht11_check()==0){
		printf("dht11 check!\r\n");
	}
	
	parse_cmd();
	
	
	while(1){
		delay_s(1);
		if(dht11_read_data(&temp,&humi)==0){
			printf("temp = %d,humi = %d\r\n",temp,humi);
		}
	}
	
}
