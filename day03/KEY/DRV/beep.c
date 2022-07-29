#include <stm32f4xx.h>
#include <beep.h>

void beep_reg_init(void)
{
	//����GPIOF 
	RCC->AHB1ENR |= (0x1<<5);
	//����PF8 �����������������������
	GPIOF->MODER &= ~(0x3<<16);
	GPIOF->MODER |= (0x1<<16);//01
	GPIOF->OTYPER &= ~(0x1<<8);//0
	GPIOF->OSPEEDR &= ~(0x3<<16);
	GPIOF->OSPEEDR |= (0x2<<16);//10
	GPIOF->PUPDR &= ~(0x3<<16);//00
	//Ĭ�ϲ���
	GPIOF->BSRRH |= (0x1<<8);
}	

void beep_lib_init(void)
{
	//GPIO��ʼ���ṹ
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.����GPIOE Fʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//2.��ʼ��PF8
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//PF8
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//Ĭ������� ----- ����
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
}	

//flag:0 - ���� 1 - ��
void beep_onoff(int flag)
{
	if(flag){
		//GPIOF->BSRRL |= (0x1<<8);
		//GPIO_SetBits(GPIOF,GPIO_Pin_8);
		BEEP = 1;
	}
	else{
		//GPIOF->BSRRH |= (0x1<<8);
		//GPIO_ResetBits(GPIOF,GPIO_Pin_8);
		BEEP = 0;
	}
	
}
