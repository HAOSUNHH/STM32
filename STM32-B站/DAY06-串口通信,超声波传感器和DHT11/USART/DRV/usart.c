#include <stm32f4xx.h>
#include <usart.h>
#include <stdio.h>
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
	USART_InitStruct.USART_BaudRate = 115200;//������
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

//���ڽ��ջ�����
volatile u32 usart1_cnt = 0;//��¼����1�յ������ݳ���
volatile u8 usart1_buf[64] = {0};//��¼����1�յ���������
volatile u32 usart1_flag = 0;//��¼����1���յ����������� 1---����

//�������յ�������
void parse_cmd(void)
{
	while(1){
		//���ݽ�����ɲŴ���
		if(usart1_flag){
			//����BEEP����
			if(strstr((char *)usart1_buf,"beep")){
				if(strstr((char *)usart1_buf,"on")){
					BEEP = 1;
					printf("BEEP ON!\r\n");
				}
				if(strstr((char *)usart1_buf,"off")){
					BEEP = 0;
					printf("BEEP OFF!\r\n");
				}
			}
			
			//����LED����
			if(strstr((char *)usart1_buf,"led1")){
				if(strstr((char *)usart1_buf,"on")){
					D1 = 0;
					printf("LED1 ON!\r\n");
				}
				if(strstr((char *)usart1_buf,"off")){
					D1 = 1;
					printf("LED1 OFF!\r\n");
				}
			}
			
			//����LED����
			if(strstr((char *)usart1_buf,"MQ2")){
				if(MQ2){
					printf("����!\r\n");
				}
				else{
					printf("����!\r\n");
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

