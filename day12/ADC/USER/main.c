#include <stm32f4xx.h>
#include <includes.h>
#include <stdio.h>

int main()
{
	//u8 buf[4] = {0x11,0x22,0x33,0x44};

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
	usart3_init();
	//timer2_init();
	sr04_init();
	rtc_init();
	//rtc_alarm_init();
	iic_init();
	//I2C_Configuration();
	//OLED_Init();
	spi1_init();
	adc3_init();
	can1_init();

	printf("app start!\r\n");
	
	
	//等待dht11 >1s
	delay_s(2);
	
	if(dht11_check()==0){
		printf("dht11 check!\r\n");
	}
	
	//parse_cmd();
	
	while(1){
		//can1_send_message(buf,4,0x456);
		printf("vol = %dmV\r\n",get_adc3_vol()*3300/0xfff);
		
		delay_s(1);
	}
	
}
