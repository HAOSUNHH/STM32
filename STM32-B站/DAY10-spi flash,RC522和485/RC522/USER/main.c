#include <stm32f4xx.h>
#include <includes.h>
#include <stdio.h>

int main()
{
	u32 i;
	u8 status,card_size;
	//MFRC522数据区
	u8  mfrc552pidbuf[18];
	u8  card_pydebuf[2];
	u8  card_numberbuf[5];
	u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	u8  card_readbuf[18] = {0};

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
	spi1_init();

	printf("app start!\r\n");
	
	
	//等待dht11 >1s
	delay_s(2);
	
	if(dht11_check()==0){
		printf("dht11 check!\r\n");
	}
	
	//parse_cmd();
	
	
	while(1){
		MF522_Init();
		//请求卡
		status=MFRC522_Request(0x52, card_pydebuf);
		if(status!=MI_OK){
			printf("MFRC522_Request failed!\r\n");
			delay_s(2);
			continue;
		}
		
		//防碰撞
		status=MFRC522_Anticoll(card_numberbuf);
		if(status!=MI_OK){
			printf("MFRC522_Anticoll failed!\r\n");
			delay_s(2);
			continue;
		}
		
		printf("cardid = 0x%hhx %hhx %hhx %hhx %hhx\r\n",card_numberbuf[0],card_numberbuf[1],
				card_numberbuf[2],card_numberbuf[3],card_numberbuf[4]);
		//0x5e1ae7a9a 0x8918a12b1b
		if(card_numberbuf[0]==0x5e&&card_numberbuf[1]==0x1a&&card_numberbuf[2]==0xe7&&card_numberbuf[3]==0xa9&&card_numberbuf[4]==0xa){
			BEEP = 1;
			delay_ms(200);
			BEEP = 0;
		}
		else{
			BEEP = 1;
			delay_s(1);
			BEEP = 0;
		}
		
		/*
		//选卡
		card_size=MFRC522_SelectTag(card_numberbuf);
		printf("cardsize = %d\r\n",card_size);
		
		//密钥验证
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);
		if(status!=MI_OK){
			printf("MFRC522_Auth failed!\r\n");
			delay_s(2);
			continue;
		}
		
		//写卡
		status=MFRC522_Write(4, card_writebuf);	
		if(status!=MI_OK){
			printf("MFRC522_Write failed!\r\n");
			delay_s(2);
			continue;
		}
		
		//读卡
		status=MFRC522_Read(4, card_readbuf);
		if(status!=MI_OK){
			printf("MFRC522_Read failed!\r\n");
			delay_s(2);
			continue;
		}
		
		for(i=0;i<16;i++){
			printf("%d ",card_readbuf[i]);
		}
		printf("\r\n");
		*/
		
		delay_s(2);
	}
	
}
