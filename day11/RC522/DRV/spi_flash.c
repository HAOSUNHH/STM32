#include <spi_flash.h>
#include <delay.h>

void spi1_init(void)
{
	//GPIO��ʼ���ṹ
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	//1.ʹ��GPIOB��SPI1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//84M
	
	//2.��ʼ��CS(PB14)Ϊ������ܣ�SCLK,MOSI,MISO(PB3 PB4 PB5)ΪGPIO���ù���
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;//PB14
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//�ر�Ƭѡ
	W25Q128_CS = 1;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	//3.��ʼ��SPI1
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��ȫ˫��
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//���豸
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//1�ֽ�
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//0 0 
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//�͵�ƽ,��һ������
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//�������Ƭѡ
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//16��Ƶ 84/16 = 5.25MHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//��λ��ǰ
	SPI_Init(SPI1,&SPI_InitStruct);
	
	//4.ʹ��SPI1
	SPI_Cmd(SPI1,ENABLE);
}

//���ͽ�������
//data��Ҫ���͵ģ����ؽ��յ�����
u8 spi1_send_recv_data(u8 data)
{
	//�ȴ���һ�����ݷ������
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);
	//��������
	SPI_I2S_SendData(SPI1,data);
	//������ͬ��ͨ�ţ����ͺͽ���ͬʱ����
	//�ȴ����ջ������ǿ�
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);
	//��������
	return SPI_I2S_ReceiveData(SPI1);
}

//��ȡW25Q128���豸ID
u16 w25q128_read_id(void)
{
	u16 id = 0;
	
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	//����90H
	spi1_send_recv_data(0x90);
	
	//����24λ0��ַ
	spi1_send_recv_data(0x00);
	spi1_send_recv_data(0x00);
	spi1_send_recv_data(0x00);
	
	//���ճ��Һ��豸ID
	id |= spi1_send_recv_data(0xff)<<8;
	id |= spi1_send_recv_data(0xff);
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
	
	return id;
}

//��W25Q128
void w25q128_read_data(u32 addr,u8 *data,u8 len)
{
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	//����03H
	spi1_send_recv_data(0x03);
	
	//����24λ��ַ���ȷ���λ
	spi1_send_recv_data((addr>>16)&0xff);//16~23λ
	spi1_send_recv_data((addr>>8)&0xff);//8~15λ
	spi1_send_recv_data(addr&0xff);//0~7λ
	
	//��ȡ����
	while(len--){
		*data++ = spi1_send_recv_data(0xff);
	}
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
}

//дʹ�ܿ���/�ر� 0 --- ����дʹ�� 1 --- �ر�дʹ��
void w25q128_write_enable_disable(u8 enable)
{
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	if(enable){
		spi1_send_recv_data(0x04);
	}
	else{
		spi1_send_recv_data(0x06);
	}
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
}

//��ȡ״̬�Ĵ���1
u8 w25q128_read_status_reg1(void)
{
	u8 status = 0;
	
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	//����05H
	spi1_send_recv_data(0x05);
	//��ȡ�Ĵ�����ֵ
	status = spi1_send_recv_data(0xff);
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
	
	return status;
}

//��������
void w25q128_erase_sector(u32 addr)
{
	//����дʹ��
	w25q128_write_enable_disable(0);
	
	//��ʱ����Ƭѡ�źű���һ��ʱ��
	delay_us(50);
	
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	//����20H
	spi1_send_recv_data(0x20);
	
	//����24λ������ַ���ȷ���λ
	spi1_send_recv_data((addr>>16)&0xff);//16~23λ
	spi1_send_recv_data((addr>>8)&0xff);//8~15λ
	spi1_send_recv_data(addr&0xff);//0~7λ
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
	
	//��ʱ����Ƭѡ�źű���һ��ʱ��
	delay_us(50);
	
	//�ȴ��������
	while(w25q128_read_status_reg1()&0x1){
		delay_ms(1);//1ms��ѯһ��
	}
	
	//��ʱ����Ƭѡ�źű���һ��ʱ��
	delay_us(50);
	
	//�ر�дʹ��
	w25q128_write_enable_disable(1);
}


//��ҳд
void w25q128_write_page(u32 addr,u8 *data,u8 len)
{
	
	//����дʹ��
	w25q128_write_enable_disable(0);
	
	//��ʱ����Ƭѡ�źű���һ��ʱ��
	delay_us(50);
	
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	
	//����02H
	spi1_send_recv_data(0x02);
	
	//����24λд��ַ���ȷ���λ
	spi1_send_recv_data((addr>>16)&0xff);//16~23λ
	spi1_send_recv_data((addr>>8)&0xff);//8~15λ
	spi1_send_recv_data(addr&0xff);//0~7λ
	
	//����д������
	while(len--){
		spi1_send_recv_data(*data++);
	}
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
	
	//��ʱ����Ƭѡ�źű���һ��ʱ��
	delay_us(50);
	
	//�ȴ�д���
	while(w25q128_read_status_reg1()&0x1){
		delay_ms(1);//1ms��ѯһ��
	}
	
	//��ʱ����Ƭѡ�źű���һ��ʱ��
	delay_us(50);
	
	//�ر�дʹ��
	w25q128_write_enable_disable(1);
}
