#include <iic.h>
#include <delay.h>
#include <stdio.h>

//初始化IO口
void iic_init(void)
{
	//GPIO初始化结构
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.开启GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//2.初始化PB8 PB9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;//
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

//设置SDA的方向
void iic_sda_mode(GPIOMode_TypeDef IO)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//2.初始化PB8 PB9
	GPIO_InitStruct.GPIO_Mode = IO;//
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

//起始信号
void iic_start(void)
{
	//SDA输出模式
	iic_sda_mode(GPIO_Mode_OUT);
	
	//总线空闲
	SCL = 1;
	SDA_OUT = 1;
	delay_us(5);
	
	//SDA拉低
	SDA_OUT = 0;
	delay_us(5);
	
	//SDA拉低之后，在拉低SCL，钳住总线
	SCL = 0;
}

//停止信号
void iic_stop(void)
{
	//SDA输出模式
	iic_sda_mode(GPIO_Mode_OUT);

	SCL = 0;
	SDA_OUT = 0;
	delay_us(5);
	
	//SCL拉高
	SCL = 1;
	delay_us(5);
	
	//拉高SDA,此时SDA和SCL均为高，总线空闲
	SDA_OUT = 1;
}

//等待从设备应答，返回0表示有效应答，返回1表示无效应答
u8 iic_wait_ack(void)
{
	u8 ack = 0;
	
	//SDA输入模式
	iic_sda_mode(GPIO_Mode_IN);
	
	//拉高SCL，允许从设备操作SDA
	SCL = 1;
	delay_us(5);
	
	//读取SDA的电平
	if(SDA_IN==1){
		ack = 1;//无效应答
		iic_stop();//收到无效应答，立即停止通信
	}
	else{
		ack = 0;
	}
	
	SCL = 0;
	delay_us(5);
	
	return ack;
}

//产生应答信号，传参ack=0有效应答，ack=1无效应答
void iic_ack(u8 ack)
{
	//SDA输出模式
	iic_sda_mode(GPIO_Mode_OUT);
	
	//拉低时钟线，准备修改数据线电平
	SCL = 0;
	delay_us(5);
	
	//设置SDA电平
	if(ack){
		SDA_OUT = 1;//无效应答
	}
	else{
		SDA_OUT = 0;//有效应答
	}
	
	delay_us(5);
	//数据稳定后拉高时钟线让从设备接收ack
	SCL = 1;
	delay_us(5);
	
	SCL = 0;
}

//发送1字节数据 ------ 先高后低
void iic_send_byte(u8 txd)
{
	u8 i;
	
	//SDA输出模式
	iic_sda_mode(GPIO_Mode_OUT);
	
	SCL = 0;
	delay_us(5);
	
	//从高到低依次发送每一位
	for(i=0;i<8;i++){
		if(txd&1<<(7-i)){
			SDA_OUT = 1;
		}
		else{
			SDA_OUT = 0;
		}
		
		delay_us(5);
		
		//拉高时钟线，对方读走这一位
		SCL = 1;
		delay_us(5);
		//拉低时钟线,准备发送下一位
		SCL = 0;
	}
}

//接收1字节数据
u8 iic_recv_byte(void)
{
	u8 rxd = 0,i;
	
	//SDA输入模式
	iic_sda_mode(GPIO_Mode_IN);
	
	SCL = 0;
	
	//从高到低依次接收每一位
	for(i=0;i<8;i++){
		//等待对方设置好电平
		delay_us(5);
		//高电平期间读取一位数据
		SCL = 1;
		if(SDA_IN)
			rxd |= 1<<(7-i);
		
		//准备接收下一位
		delay_us(5);
		SCL = 0;
	}
	
	return rxd;
}

//at24c02写1字节数据
void at24c02_write_byte(u8 addr,u8 data)
{
	u8 ack;
	
	//起始信号
	iic_start();
	
	//发送从设备地址和写操作 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 1\r\n");
		return;
	}
	
	//发送写的片内地址
	iic_send_byte(addr);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 2\r\n");
		return;
	}
	
	//发送写的数据
	iic_send_byte(data);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 3\r\n");
		return;
	}
	
	//停止信号
	iic_stop();
}

//at24c02读1字节数据
u8 at24c02_read_byte(u8 addr)
{
	u8 ack,data;

	//起始信号
	iic_start();
	
	//发送从设备地址和写操作 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 1\r\n");
		return 0;
	}
	
	//发送读的片内地址
	iic_send_byte(addr);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 2\r\n");
		return 0;
	}
	
	//起始信号
	iic_start();
	
	//发送从设备地址和读操作 0x50<<1|1 = 0xa1
	iic_send_byte(0xa1);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("ack failed 3\r\n");
		return 0;
	}
	
	//接收读的数据
	data = iic_recv_byte();
	//无效应答
	iic_ack(1);
	
	//停止信号
	iic_stop();
	
	return data;
}
