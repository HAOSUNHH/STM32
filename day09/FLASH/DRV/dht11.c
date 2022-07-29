#include <dht11.h>
#include <delay.h>
#include <sys.h>

//配置DQ为输入/输出模式
void dht11_set_io(GPIOMode_TypeDef IO)
{
	//GPIO初始化结构
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.开启GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	//2.初始化PE6为推挽输出 PA8为浮空输入
	GPIO_InitStruct.GPIO_Mode = IO;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//PG9
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}

//测试DHT11是否有响应,返回0表示正常，返回非0表示无响应
int dht11_check(void)
{
	u32 retry = 0;
	
	//输出模式
	dht11_set_io(GPIO_Mode_OUT);
	//发送>18ms起始信号
	DQ_OUT = 0;
	delay_ms(20);//> 18ms 高电平
	DQ_IN = 1;
	
	//等待20~40us
	delay_us(30);
	
	//输入模式
	dht11_set_io(GPIO_Mode_IN);
	//读取DHT11的响应
	//等待拉低
	while(DQ_IN&&retry<100){
		retry++;
		delay_us(1);
	}
	if(retry>=100){//如果没有拉低，无响应退出
		return -1;
	}
	
	retry = 0;
	//80us后拉高
	while(!DQ_IN&&retry<100){
		retry++;
		delay_us(1);
	}
	if(retry>=100){//如果没有拉高，无响应退出
		return -1;
	}
	
	return 0;
}

//读1位数据
u8 dht11_read_bit(void)
{
	u32 retry = 0;
	
	//等待变低 ,最多80us
	while(DQ_IN&&retry<100){
		retry++;
		delay_us(1);
	}
	if(retry>=100){//如果没有拉低，无响应退出
		return 100;
	}
	
	//等待变高
	retry = 0;
	//50us后拉高
	while(!DQ_IN&&retry<100){
		retry++;
		delay_us(1);
	}
	if(retry>=100){//如果没有拉高，无响应退出
		return 100;
	}
	
	//延时40us，此时低电平 --- 0  高电平 --- 1
	delay_us(40);
	
	return DQ_IN;
}

//读1个字节
u8 dht11_read_byte(void)
{
	u8 i,dat = 0;
	
	for(i=0;i<8;i++){
		dat |= dht11_read_bit()<<(7-i);//高位先出
	}
	
	return dat;
}

//读取温湿度数据
//返回0表示正常，返回非0表示失败
//temp,humi为输出参数，输出温湿度
int dht11_read_data(u8 *temp,u8 *humi)
{
	u8 i,buf[5] = {0};
	
	//发送起始信号
	if(dht11_check()==0){
		//读取40bit数据
		for(i=0;i<5;i++){
			buf[i] = dht11_read_byte();
		}
		
		//判断校验和
		if(((buf[0]+buf[1]+buf[2]+buf[3])&0xff)==buf[4]){
			*temp = buf[2];
			*humi = buf[0];
			return 0;
		}
	}
	
	return -1;
}
