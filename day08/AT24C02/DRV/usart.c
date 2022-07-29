#include <stm32f4xx.h>
#include <usart.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <includes.h>

//ARM���������г���������������˵�����������ͻ�ʹ����������������豸
//���ַ�ʽ�а�����ģʽ��printfҪͨ������1�������Ҫ�رհ�����ģʽ
#pragma import(__use_no_semihosting)

struct __FILE{
    int handle;    
};

FILE __stdout;

//����__sys_exit����������ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}

//�ض���fputc
int fputc(int ch,FILE *f)
{
	//�ȴ����ݷ������
	while(SET!=USART_GetFlagStatus(USART1,USART_FLAG_TC));
	
	//����
	USART_SendData(USART1,ch);
 
    return ch;
}

void usart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.����GPIOA��USART1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//2.��ʼ��PA9 PA10Ϊ���ڹ���
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//PA9 PA10
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//3.��ʼ������ 115200 8N1
	USART_InitStruct.USART_BaudRate = 9600;//������
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//���ͺͽ���ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;//��У��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������
	USART_Init(USART1,&USART_InitStruct);
	
	//NVIC��ʼ��
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//����1�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//4.ʹ�ܴ���
	USART_Cmd(USART1,ENABLE);
}

void usart1_putc(char ch)
{
	//�ȴ���һ�����ݷ������
	while(SET!=USART_GetFlagStatus(USART1,USART_FLAG_TC));
	//����
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
	
	//1.����GPIOA��USART1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//2.��ʼ��PB10 PB11Ϊ���ڹ���
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//PB10 PB11
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	//3.��ʼ������ 115200 8N1
	USART_InitStruct.USART_BaudRate = 9600;//������
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//���ͺͽ���ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;//��У��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������
	USART_Init(USART3,&USART_InitStruct);
	
	//NVIC��ʼ��
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;//����1�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	//4.ʹ�ܴ���
	USART_Cmd(USART3,ENABLE);
}

void usart3_putc(char ch)
{
	//�ȴ���һ�����ݷ������
	while(SET!=USART_GetFlagStatus(USART3,USART_FLAG_TC));
	//����
	USART_SendData(USART3,ch);
}

void usart3_puts(const char *s)
{
	while(*s){
		usart3_putc(*s++);
	}
}

//���ڽ��ջ�����
volatile u32 usart1_cnt = 0;//��¼����1�յ������ݳ���
volatile u8 usart1_buf[64] = {0};//��¼����1�յ���������
volatile u32 usart1_flag = 0;//��¼����1���յ����������� 1---����

void eeprom_write(void)
{
	char *buf = NULL;
	u8 addr,data;
	
	buf = strtok((char *)usart1_buf,":");
	if(buf==NULL){
		usart3_puts("eeprom commad error!");
	}
	else{
		//ȡд�ĵ�ַ
		buf = strtok(NULL,":");
		if(buf==NULL){
			usart3_puts("eeprom addr error!");
		}
		else{
			addr = atoi(buf);
			
			//ȡд������
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
		//ȡ���ĵ�ַ
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

//�������յ�������
void parse_cmd(void)
{
	int distance = 0;
	u8 temp = 0,humi = 0;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	while(1){
		//���ݽ�����ɲŴ���
		if(usart1_flag){
			//����BEEP����
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
			
			//����LED����
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
			
			//��ȡ��ȼ���崫����״̬
			if(strstr((char *)usart1_buf,"MQ2")){
				if(MQ2){
					printf("����!\r\n");
				}
				else{
					printf("����!\r\n");
				}
			}
			
			//��ȡ����������
			if(strstr((char *)usart1_buf,"sr04")){
				distance = get_distance();
				if(distance==-1){
					printf("get distance error!\r\n");
				}
				else{
					printf("distance = %d!\r\n",distance);
				}
			}
			
			//��ȡ��ʪ��
			if(strstr((char *)usart1_buf,"dht11")){
				if(dht11_read_data(&temp,&humi)==0){
					printf("temp = %d,humi = %d\r\n",temp,humi);
				}
				else{
					printf("get dht11 error!\r\n");
				}
			}
			
			//��ȡʱ��
			if(strstr((char *)usart1_buf,"gettime")){
				RTC_GetDate(RTC_Format_BCD,&RTC_DateStruct);
				RTC_GetTime(RTC_Format_BCD,&RTC_TimeStruct);
				
				sprintf((char *)usart1_buf,"20%hhx-%hhx-%hhx %hhx %hhx:%hhx:%hhx",
						RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay,
						RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
				usart3_puts((char *)usart1_buf);
			}
			
			//��дeeprom
			if(strstr((char *)usart1_buf,"eeprom")){
				if(strstr((char *)usart1_buf,"write")){
					eeprom_write();
				}
				if(strstr((char *)usart1_buf,"read")){
					eeprom_read();
				}
			}
			
			//������ɺ������־
			usart1_flag = 0;
			//��ս�����
			memset((void *)usart1_buf,0,sizeof(usart1_buf));
			usart1_cnt = 0;
		}
	}
}

//����1�жϴ�����
void USART1_IRQHandler(void)
{
	//u8 data;
	
	//�ǲ��ǽ����ж�
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET){
		//��������
		//data = USART_ReceiveData(USART1);
		//ԭ·����
		//usart1_putc(data);
		//���뻺����
		usart1_buf[usart1_cnt++] = USART_ReceiveData(USART1);
		
		//�ж������Ƿ�������(������*����)
		if(usart1_buf[usart1_cnt-1]=='*'||usart1_cnt>=sizeof(usart1_buf)){
			usart1_flag = 1;
		}
		
		//����жϱ�־
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

//����1�жϴ�����
void USART3_IRQHandler(void)
{
	//u8 data;
	
	//�ǲ��ǽ����ж�
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET){
		//��������
		//data = USART_ReceiveData(USART3);
		//ͨ������1��ӡ
		//usart1_putc(data);
		//���뻺����
		usart1_buf[usart1_cnt++] = USART_ReceiveData(USART3);
		
		//�ж������Ƿ�������(������*����)
		if(usart1_buf[usart1_cnt-1]=='*'||usart1_cnt>=sizeof(usart1_buf)){
			usart1_flag = 1;
		}
		
		//����жϱ�־
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}

