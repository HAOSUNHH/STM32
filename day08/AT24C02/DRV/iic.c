#include <iic.h>
#include <delay.h>
#include <stdio.h>

//��ʼ��IO��
void iic_init(void)
{
	//GPIO��ʼ���ṹ
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.����GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//2.��ʼ��PB8 PB9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;//
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

//����SDA�ķ���
void iic_sda_mode(GPIOMode_TypeDef IO)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//2.��ʼ��PB8 PB9
	GPIO_InitStruct.GPIO_Mode = IO;//
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

//��ʼ�ź�
void iic_start(void)
{
	//SDA���ģʽ
	iic_sda_mode(GPIO_Mode_OUT);
	
	//���߿���
	SCL = 1;
	SDA_OUT = 1;
	delay_us(5);
	
	//SDA����
	SDA_OUT = 0;
	delay_us(5);
	
	//SDA����֮��������SCL��ǯס����
	SCL = 0;
}

//ֹͣ�ź�
void iic_stop(void)
{
	//SDA���ģʽ
	iic_sda_mode(GPIO_Mode_OUT);

	SCL = 0;
	SDA_OUT = 0;
	delay_us(5);
	
	//SCL����
	SCL = 1;
	delay_us(5);
	
	//����SDA,��ʱSDA��SCL��Ϊ�ߣ����߿���
	SDA_OUT = 1;
}

//�ȴ����豸Ӧ�𣬷���0��ʾ��ЧӦ�𣬷���1��ʾ��ЧӦ��
u8 iic_wait_ack(void)
{
	u8 ack = 0;
	
	//SDA����ģʽ
	iic_sda_mode(GPIO_Mode_IN);
	
	//����SCL��������豸����SDA
	SCL = 1;
	delay_us(5);
	
	//��ȡSDA�ĵ�ƽ
	if(SDA_IN==1){
		ack = 1;//��ЧӦ��
		iic_stop();//�յ���ЧӦ������ֹͣͨ��
	}
	else{
		ack = 0;
	}
	
	SCL = 0;
	delay_us(5);
	
	return ack;
}

//����Ӧ���źţ�����ack=0��ЧӦ��ack=1��ЧӦ��
void iic_ack(u8 ack)
{
	//SDA���ģʽ
	iic_sda_mode(GPIO_Mode_OUT);
	
	//����ʱ���ߣ�׼���޸������ߵ�ƽ
	SCL = 0;
	delay_us(5);
	
	//����SDA��ƽ
	if(ack){
		SDA_OUT = 1;//��ЧӦ��
	}
	else{
		SDA_OUT = 0;//��ЧӦ��
	}
	
	delay_us(5);
	//�����ȶ�������ʱ�����ô��豸����ack
	SCL = 1;
	delay_us(5);
	
	SCL = 0;
}

//����1�ֽ����� ------ �ȸߺ��
void iic_send_byte(u8 txd)
{
	u8 i;
	
	//SDA���ģʽ
	iic_sda_mode(GPIO_Mode_OUT);
	
	SCL = 0;
	delay_us(5);
	
	//�Ӹߵ������η���ÿһλ
	for(i=0;i<8;i++){
		if(txd&1<<(7-i)){
			SDA_OUT = 1;
		}
		else{
			SDA_OUT = 0;
		}
		
		delay_us(5);
		
		//����ʱ���ߣ��Է�������һλ
		SCL = 1;
		delay_us(5);
		//����ʱ����,׼��������һλ
		SCL = 0;
	}
}

//����1�ֽ�����
u8 iic_recv_byte(void)
{
	u8 rxd = 0,i;
	
	//SDA����ģʽ
	iic_sda_mode(GPIO_Mode_IN);
	
	SCL = 0;
	
	//�Ӹߵ������ν���ÿһλ
	for(i=0;i<8;i++){
		//�ȴ��Է����úõ�ƽ
		delay_us(5);
		//�ߵ�ƽ�ڼ��ȡһλ����
		SCL = 1;
		if(SDA_IN)
			rxd |= 1<<(7-i);
		
		//׼��������һλ
		delay_us(5);
		SCL = 0;
	}
	
	return rxd;
}

//at24c02д1�ֽ�����
void at24c02_write_byte(u8 addr,u8 data)
{
	u8 ack;
	
	//��ʼ�ź�
	iic_start();
	
	//���ʹ��豸��ַ��д���� 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 1\r\n");
		return;
	}
	
	//����д��Ƭ�ڵ�ַ
	iic_send_byte(addr);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 2\r\n");
		return;
	}
	
	//����д������
	iic_send_byte(data);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 3\r\n");
		return;
	}
	
	//ֹͣ�ź�
	iic_stop();
}

//at24c02��1�ֽ�����
u8 at24c02_read_byte(u8 addr)
{
	u8 ack,data;

	//��ʼ�ź�
	iic_start();
	
	//���ʹ��豸��ַ��д���� 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 1\r\n");
		return 0;
	}
	
	//���Ͷ���Ƭ�ڵ�ַ
	iic_send_byte(addr);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 2\r\n");
		return 0;
	}
	
	//��ʼ�ź�
	iic_start();
	
	//���ʹ��豸��ַ�Ͷ����� 0x50<<1|1 = 0xa1
	iic_send_byte(0xa1);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 3\r\n");
		return 0;
	}
	
	//���ն�������
	data = iic_recv_byte();
	//��ЧӦ��
	iic_ack(1);
	
	//ֹͣ�ź�
	iic_stop();
	
	return data;
}
