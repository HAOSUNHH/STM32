#include <stm32f4xx.h>
#include <usart.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <includes.h>

//ARM开发板运行程序，如果主机运行了调试器，程序就会使用主机的输入输出设备
//这种方式叫半主机模式，printf要通过串口1输出，就要关闭半主机模式
#pragma import(__use_no_semihosting)

struct __FILE{
    int handle;    
};

FILE __stdout;

//定义__sys_exit函数，避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

//重定义fputc
int fputc(int ch,FILE *f)
{
	//等待数据发送完成
	while(SET!=USART_GetFlagStatus(USART1,USART_FLAG_TC));
	
	//发送
	USART_SendData(USART1,ch);
 
    return ch;
}

void usart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.开启GPIOA和USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//2.初始化PA9 PA10为串口功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//PA9 PA10
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//3.初始化串口 115200 8N1
	USART_InitStruct.USART_BaudRate = 9600;//波特率
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//发送和接收模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//无校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控
	USART_Init(USART1,&USART_InitStruct);
	
	//NVIC初始化
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//串口1中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
	
	//使能串口接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//4.使能串口
	USART_Cmd(USART1,ENABLE);
}

void usart1_putc(char ch)
{
	//等待上一个数据发送完成
	while(SET!=USART_GetFlagStatus(USART1,USART_FLAG_TC));
	//发送
	USART_SendData(USART1,ch);
}

void usart1_puts(const char *s)
{
	while(*s){
		usart1_putc(*s++);
	}
}

void usart3_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.开启GPIOA和USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//2.初始化PB10 PB11为串口功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//PB10 PB11
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	//3.初始化串口 115200 8N1
	USART_InitStruct.USART_BaudRate = 9600;//波特率
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//发送和接收模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//无校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控
	USART_Init(USART3,&USART_InitStruct);
	
	//NVIC初始化
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;//串口1中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
	
	//使能串口接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	//4.使能串口
	USART_Cmd(USART3,ENABLE);
}

void usart3_putc(char ch)
{
	//等待上一个数据发送完成
	while(SET!=USART_GetFlagStatus(USART3,USART_FLAG_TC));
	//发送
	USART_SendData(USART3,ch);
}

void usart3_puts(const char *s)
{
	while(*s){
		usart3_putc(*s++);
	}
}

//串口接收缓冲区
volatile u32 usart1_cnt = 0;//记录串口1收到的数据长度
volatile u8 usart1_buf[64] = {0};//记录串口1收的数据内容
volatile u32 usart1_flag = 0;//记录串口1接收到了完整数据 1---完整

void eeprom_write(void)
{
	char *buf = NULL;
	u8 addr,data;
	
	buf = strtok((char *)usart1_buf,":");
	if(buf==NULL){
		usart3_puts("eeprom commad error!");
	}
	else{
		//取写的地址
		buf = strtok(NULL,":");
		if(buf==NULL){
			usart3_puts("eeprom addr error!");
		}
		else{
			addr = atoi(buf);
			
			//取写的数据
			buf = strtok(NULL,":");
			if(buf==NULL){
				usart3_puts("eeprom data error!");
			}
			else{
				data = atoi(buf);
				at24c02_write_byte(addr,data);
				usart3_puts("eeprom write success!");
			}
		}
	}
}

void eeprom_read(void)
{
	u8 temp_buf[100] = {0};
	char *buf = NULL;
	u8 addr,data;
	
	buf = strtok((char *)usart1_buf,":");
	if(buf==NULL){
		usart3_puts("eeprom commad error!");
	}
	else{
		//取读的地址
		buf = strtok(NULL,":");
		if(buf==NULL){
			usart3_puts("eeprom addr error!");
		}
		else{
			addr = atoi(buf);
			data = at24c02_read_byte(addr);
			sprintf((char *)temp_buf,"data = %d!",data);
			usart3_puts((char *)temp_buf);
		}
	}
}

//处理串口收到的命令
void parse_cmd(void)
{
	int distance = 0;
	u8 temp = 0,humi = 0;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	while(1){
		//数据接收完成才处理
		if(usart1_flag){
			//控制BEEP命令
			if(strstr((char *)usart1_buf,"beep")){
				if(strstr((char *)usart1_buf,"on")){
					BEEP = 1;
					usart3_puts("BEEP ON!\r\n");
				}
				if(strstr((char *)usart1_buf,"off")){
					BEEP = 0;
					usart3_puts("BEEP OFF!\r\n");
				}
			}
			
			//控制LED命令
			if(strstr((char *)usart1_buf,"led1")){
				if(strstr((char *)usart1_buf,"on")){
					D1 = 0;
					usart3_puts("LED1 ON!\r\n");
				}
				if(strstr((char *)usart1_buf,"off")){
					D1 = 1;
					usart3_puts("LED1 OFF!\r\n");
				}
			}
			
			//读取可燃气体传感器状态
			if(strstr((char *)usart1_buf,"MQ2")){
				if(MQ2){
					printf("正常!\r\n");
				}
				else{
					printf("报警!\r\n");
				}
			}
			
			//读取超声波距离
			if(strstr((char *)usart1_buf,"sr04")){
				distance = get_distance();
				if(distance==-1){
					printf("get distance error!\r\n");
				}
				else{
					printf("distance = %d!\r\n",distance);
				}
			}
			
			//读取温湿度
			if(strstr((char *)usart1_buf,"dht11")){
				if(dht11_read_data(&temp,&humi)==0){
					printf("temp = %d,humi = %d\r\n",temp,humi);
				}
				else{
					printf("get dht11 error!\r\n");
				}
			}
			
			//获取时间
			if(strstr((char *)usart1_buf,"gettime")){
				RTC_GetDate(RTC_Format_BCD,&RTC_DateStruct);
				RTC_GetTime(RTC_Format_BCD,&RTC_TimeStruct);
				
				sprintf((char *)usart1_buf,"20%hhx-%hhx-%hhx %hhx %hhx:%hhx:%hhx",
						RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay,
						RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
				usart3_puts((char *)usart1_buf);
			}
			
			//读写eeprom
			if(strstr((char *)usart1_buf,"eeprom")){
				if(strstr((char *)usart1_buf,"write")){
					eeprom_write();
				}
				if(strstr((char *)usart1_buf,"read")){
					eeprom_read();
				}
			}
			
			//处理完成后清除标志
			usart1_flag = 0;
			//清空接收区
			memset((void *)usart1_buf,0,sizeof(usart1_buf));
			usart1_cnt = 0;
		}
	}
}

//串口1中断处理函数
void USART1_IRQHandler(void)
{
	//u8 data;
	
	//是不是接收中断
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET){
		//接收数据
		//data = USART_ReceiveData(USART1);
		//原路发回
		//usart1_putc(data);
		//放入缓冲区
		usart1_buf[usart1_cnt++] = USART_ReceiveData(USART1);
		
		//判断数据是否接收完成(数据以*结束)
		if(usart1_buf[usart1_cnt-1]=='*'||usart1_cnt>=sizeof(usart1_buf)){
			usart1_flag = 1;
		}
		
		//清除中断标志
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

//串口1中断处理函数
void USART3_IRQHandler(void)
{
	//u8 data;
	
	//是不是接收中断
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET){
		//接收数据
		//data = USART_ReceiveData(USART3);
		//通过串口1打印
		//usart1_putc(data);
		//放入缓冲区
		usart1_buf[usart1_cnt++] = USART_ReceiveData(USART3);
		
		//判断数据是否接收完成(数据以*结束)
		if(usart1_buf[usart1_cnt-1]=='*'||usart1_cnt>=sizeof(usart1_buf)){
			usart1_flag = 1;
		}
		
		//清除中断标志
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}

