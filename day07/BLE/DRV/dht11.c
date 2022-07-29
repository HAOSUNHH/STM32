#include <dht11.h>
#include <delay.h>
#include <sys.h>

//����DQΪ����/���ģʽ
void dht11_set_io(GPIOMode_TypeDef IO)
{
	//GPIO��ʼ���ṹ
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.����GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	//2.��ʼ��PE6Ϊ������� PA8Ϊ��������
	GPIO_InitStruct.GPIO_Mode = IO;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//PG9
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}

//����DHT11�Ƿ�����Ӧ,����0��ʾ���������ط�0��ʾ����Ӧ
int dht11_check(void)
{
	u32 retry = 0;
	
	//���ģʽ
	dht11_set_io(GPIO_Mode_OUT);
	//����>18ms��ʼ�ź�
	DQ_OUT = 0;
	delay_ms(20);//> 18ms �ߵ�ƽ
	DQ_IN = 1;
	
	//�ȴ�20~40us
	delay_us(30);
	
	//����ģʽ
	dht11_set_io(GPIO_Mode_IN);
	//��ȡDHT11����Ӧ
	//�ȴ�����
	while(DQ_IN&&retry<100){
		retry++;
		delay_us(1);
	}
	if(retry>=100){//���û�����ͣ�����Ӧ�˳�
		return -1;
	}
	
	retry = 0;
	//80us������
	while(!DQ_IN&&retry<100){
		retry++;
		delay_us(1);
	}
	if(retry>=100){//���û�����ߣ�����Ӧ�˳�
		return -1;
	}
	
	return 0;
}

//��1λ����
u8 dht11_read_bit(void)
{
	u32 retry = 0;
	
	//�ȴ���� ,���80us
	while(DQ_IN&&retry<100){
		retry++;
		delay_us(1);
	}
	if(retry>=100){//���û�����ͣ�����Ӧ�˳�
		return 100;
	}
	
	//�ȴ����
	retry = 0;
	//50us������
	while(!DQ_IN&&retry<100){
		retry++;
		delay_us(1);
	}
	if(retry>=100){//���û�����ߣ�����Ӧ�˳�
		return 100;
	}
	
	//��ʱ40us����ʱ�͵�ƽ --- 0  �ߵ�ƽ --- 1
	delay_us(40);
	
	return DQ_IN;
}

//��1���ֽ�
u8 dht11_read_byte(void)
{
	u8 i,dat = 0;
	
	for(i=0;i<8;i++){
		dat |= dht11_read_bit()<<(7-i);//��λ�ȳ�
	}
	
	return dat;
}

//��ȡ��ʪ������
//����0��ʾ���������ط�0��ʾʧ��
//temp,humiΪ��������������ʪ��
int dht11_read_data(u8 *temp,u8 *humi)
{
	u8 i,buf[5] = {0};
	
	//������ʼ�ź�
	if(dht11_check()==0){
		//��ȡ40bit����
		for(i=0;i<5;i++){
			buf[i] = dht11_read_byte();
		}
		
		//�ж�У���
		if(((buf[0]+buf[1]+buf[2]+buf[3])&0xff)==buf[4]){
			*temp = buf[2];
			*humi = buf[0];
			return 0;
		}
	}
	
	return -1;
}
