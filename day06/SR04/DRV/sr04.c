#include <stm32f4xx.h>
#include <sr04.h>
#include <delay.h>
#include <sys.h>

void sr04_init(void)
{
	//GPIO��ʼ���ṹ
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.����GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);
	
	//2.��ʼ��PE6Ϊ������� PA8Ϊ��������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;//PE6
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	//Ĭ������͵�ƽ
	TRIG = 0;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//PA8
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}

//��������
int get_distance(void)
{
	u32 retry = 0,count = 0;
	
	//TRIG����>10us������
	TRIG = 1;
	delay_us(15);
	TRIG = 0;
	
	//�ȴ�ECHO���,�ȴ�60ms��û�б�ߣ�ֱ�ӷ���-1
	while(ECHO==0){
		retry++;
		delay_us(1);
		if(retry>60000)
			return -1;
	}
	
	//����������ʱ��
	TIM_SetCounter(TIM2,100000);
	TIM_Cmd(TIM2,ENABLE);
	
	retry = 0;
	//�ȴ�ECHO��ͣ�����ߵ�ƽʱ��
	while(ECHO==1){
		retry++;
		delay_us(1);
		if(retry>60000){
			TIM_Cmd(TIM2,DISABLE);
			return -1;
		}
	}
	
	//��ȡ��ʱ���ļ���ֵ���رն�ʱ��
	count = TIM_GetCounter(TIM2);
	TIM_Cmd(TIM2,DISABLE);
	
	//return retry*10/58;
	return (100000-count)/58;
}
