#include <stm32f4xx.h>
#include <includes.h>
#include <stdio.h>

int main()
{
	extern const unsigned char BMP1[];
	u32 i;
	int distance = 0;
	u8 buf[100] = {0};
	//u32 comp = 0;
	//u8 temp = 0,humi = 0;
	
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
	//iic_init();
	I2C_Configuration();
	OLED_Init();

	printf("app start!\r\n");
	
	//等待dht11 >1s
	delay_s(2);
	
	if(dht11_check()==0){
		printf("dht11 check!\r\n");
	}
	
	//parse_cmd();
	
	
	while(1){
		OLED_Fill(0xFF);//全屏点亮
		delay_s(1);
		OLED_Fill(0x00);//全屏灭
		delay_s(1);
		for(i=5;i<9;i++)
		{
			OLED_ShowCN(22+(i-5)*16,0,i);//测试显示中文
		}
//		delay_s(1);
//		OLED_ShowStr(0,3,"HelTec Automation",1);//测试6*8字符
//		OLED_ShowStr(0,4,"Hello Tech",2);				//测试8*16字符
		
//		distance = get_distance();
//		if(distance==-1){
//			OLED_ShowStr(0,4,"get distance error",2);
//		}
//		else{
//			sprintf((char *)buf,"distance=%d",distance);
//			OLED_ShowStr(0,4,buf,2);
//		}
		
		delay_s(1);
		
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
		
		delay_s(1);
	}
	
}
