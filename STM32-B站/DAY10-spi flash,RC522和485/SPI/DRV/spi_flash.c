#include <spi_flash.h>
#include <delay.h>

void spi1_init(void)
{
	//GPIO初始化结构
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	//1.使能GPIOB和SPI1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//84M
	
	//2.初始化CS(PB14)为输出功能，SCLK,MOSI,MISO(PB3 PB4 PB5)为GPIO复用功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;//PB14
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//关闭片选
	W25Q128_CS = 1;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	//3.初始化SPI1
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//主设备
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//1字节
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//0 0 
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//低电平,第一个边沿
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//软件控制片选
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//16分频 84/16 = 5.25MHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//高位在前
	SPI_Init(SPI1,&SPI_InitStruct);
	
	//4.使能SPI1
	SPI_Cmd(SPI1,ENABLE);
}

//发送接收数据
//data是要发送的，返回接收的数据
u8 spi1_send_recv_data(u8 data)
{
	//等待上一个数据发送完成
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);
	//发送数据
	SPI_I2S_SendData(SPI1,data);
	//由于是同步通信，发送和接收同时进行
	//等待接收缓冲区非空
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);
	//接收数据
	return SPI_I2S_ReceiveData(SPI1);
}

//读取W25Q128的设备ID
u16 w25q128_read_id(void)
{
	u16 id = 0;
	
	//片选选中
	W25Q128_CS = 0;
	
	//发送90H
	spi1_send_recv_data(0x90);
	
	//发送24位0地址
	spi1_send_recv_data(0x00);
	spi1_send_recv_data(0x00);
	spi1_send_recv_data(0x00);
	
	//接收厂家和设备ID
	id |= spi1_send_recv_data(0xff)<<8;
	id |= spi1_send_recv_data(0xff);
	
	//关闭片选
	W25Q128_CS = 1;
	
	return id;
}

//读W25Q128
void w25q128_read_data(u32 addr,u8 *data,u8 len)
{
	//片选选中
	W25Q128_CS = 0;
	
	//发送03H
	spi1_send_recv_data(0x03);
	
	//发送24位地址，先发高位
	spi1_send_recv_data((addr>>16)&0xff);//16~23位
	spi1_send_recv_data((addr>>8)&0xff);//8~15位
	spi1_send_recv_data(addr&0xff);//0~7位
	
	//读取数据
	while(len--){
		*data++ = spi1_send_recv_data(0xff);
	}
	
	//关闭片选
	W25Q128_CS = 1;
}

//写使能开启/关闭 0 --- 开启写使能 1 --- 关闭写使能
void w25q128_write_enable_disable(u8 enable)
{
	//片选选中
	W25Q128_CS = 0;
	
	if(enable){
		spi1_send_recv_data(0x04);
	}
	else{
		spi1_send_recv_data(0x06);
	}
	
	//关闭片选
	W25Q128_CS = 1;
}

//读取状态寄存器1
u8 w25q128_read_status_reg1(void)
{
	u8 status = 0;
	
	//片选选中
	W25Q128_CS = 0;
	
	//发送05H
	spi1_send_recv_data(0x05);
	//读取寄存器的值
	status = spi1_send_recv_data(0xff);
	
	//关闭片选
	W25Q128_CS = 1;
	
	return status;
}

//扇区擦除
void w25q128_erase_sector(u32 addr)
{
	//开启写使能
	w25q128_write_enable_disable(0);
	
	//延时，让片选信号保持一段时间
	delay_us(50);
	
	//片选选中
	W25Q128_CS = 0;
	
	//发送20H
	spi1_send_recv_data(0x20);
	
	//发送24位擦除地址，先发高位
	spi1_send_recv_data((addr>>16)&0xff);//16~23位
	spi1_send_recv_data((addr>>8)&0xff);//8~15位
	spi1_send_recv_data(addr&0xff);//0~7位
	
	//关闭片选
	W25Q128_CS = 1;
	
	//延时，让片选信号保持一段时间
	delay_us(50);
	
	//等待擦除完成
	while(w25q128_read_status_reg1()&0x1){
		delay_ms(1);//1ms查询一次
	}
	
	//延时，让片选信号保持一段时间
	delay_us(50);
	
	//关闭写使能
	w25q128_write_enable_disable(1);
}


//按页写
void w25q128_write_page(u32 addr,u8 *data,u8 len)
{
	
	//开启写使能
	w25q128_write_enable_disable(0);
	
	//延时，让片选信号保持一段时间
	delay_us(50);
	
	//片选选中
	W25Q128_CS = 0;
	
	
	//发送02H
	spi1_send_recv_data(0x02);
	
	//发送24位写地址，先发高位
	spi1_send_recv_data((addr>>16)&0xff);//16~23位
	spi1_send_recv_data((addr>>8)&0xff);//8~15位
	spi1_send_recv_data(addr&0xff);//0~7位
	
	//发送写的数据
	while(len--){
		spi1_send_recv_data(*data++);
	}
	
	//关闭片选
	W25Q128_CS = 1;
	
	//延时，让片选信号保持一段时间
	delay_us(50);
	
	//等待写完成
	while(w25q128_read_status_reg1()&0x1){
		delay_ms(1);//1ms查询一次
	}
	
	//延时，让片选信号保持一段时间
	delay_us(50);
	
	//关闭写使能
	w25q128_write_enable_disable(1);
}
