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
	u8 write_buf[] = {0x0f,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xf1};	
	u8 read_buf[16] = {0};
	//1.ÅäÖÃÖÐ¶ÏÓÅÏÈ¼¶·Ö×é 2:2
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
	spi1_init();

	printf("app start!\r\n");
	
	//flash_test();
	//printf("data = 0x%x\r\n",*(u32 *)SECTOR_5_ADDRESS);
	
	//µÈ´ýdht11 >1s
	delay_s(2);
	
	if(dht11_check()==0){
		printf("dht11 check!\r\n");
	}
	
	//printf("id = 0x%hx\r\n",w25q128_read_id());
	//w25q128_erase_sector(0x0);
	//w25q128_write_page(0x0,write_buf,16);
	//w25q128_read_data(0x0,read_buf,16);
	
//	for(i=0;i<16;i++){
//		printf("%#x ",read_buf[i]);
//	}
//	printf("\r\n");
	
	
	parse_cmd();
	
	
	while(1){
		OLED_Fill(0xFF);//È«ÆÁµãÁÁ
		delay_s(1);
		OLED_Fill(0x00);//È«ÆÁÃð
		delay_s(1);
		for(i=5;i<9;i++)
		{
			OLED_ShowCN(22+(i-5)*16,0,i);//²âÊÔÏÔÊ¾ÖÐÎÄ
		}
//		delay_s(1);
//		OLED_ShowStr(0,3,"HelTec Automation",1);//²âÊÔ6*8×Ö·û
//		OLED_ShowStr(0,4,"Hello Tech",2);				//²âÊÔ8*16×Ö·û
		
//		distance = get_distance();
//		if(distance==-1){
//			OLED_ShowStr(0,4,"get distance error",2);
//		}
//		else{
//			sprintf((char *)buf,"distance=%d",distance);
//			OLED_ShowStr(0,4,buf,2);
//		}
		
		delay_s(1);
		
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//²âÊÔBMPÎ»Í¼ÏÔÊ¾
		
		delay_s(1);
	}
	
}
