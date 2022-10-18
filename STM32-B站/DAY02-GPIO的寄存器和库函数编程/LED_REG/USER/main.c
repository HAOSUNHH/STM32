#include <stm32f4xx.h>

void delay(unsigned int ms)
{
	int i,j;
	
	for(i=0;i<ms;i++)
		for(j=0;j<2500;j++);
}

int main()
{
	//1.��ʼ��PF9 PF10 PE13 PE14
	//����GPIOF GPIOEʱ��
	RCC->AHB1ENR |= (0x3<<4);
	//����PF9 �����������������������
	GPIOF->MODER &= ~(0xf<<18);
	GPIOF->MODER |= (0x5<<18);//0101
	GPIOF->OTYPER &= ~(0x3<<9);//00
	GPIOF->OSPEEDR &= ~(0xf<<18);
	GPIOF->OSPEEDR |= (0xa<<18);//1010
	GPIOF->PUPDR &= ~(0xf<<18);//0000
	
	GPIOE->MODER &= ~(0xf<<26);
	GPIOE->MODER |= (0x5<<26);//0101
	GPIOE->OTYPER &= ~(0x3<<13);//00
	GPIOE->OSPEEDR &= ~(0xf<<26);
	GPIOE->OSPEEDR |= (0xa<<26);//1010
	GPIOE->PUPDR &= ~(0xf<<26);//0000
	
	
	//2.��˸
	while(1){
		//��
		GPIOF->BSRRH |= (0x3<<9);//�͵�ƽ
		GPIOE->BSRRH |= (0x3<<13);//�͵�ƽ
		//��ʱ
		delay(500);
		
		//��
		GPIOF->BSRRL |= (0x3<<9);//�ߵ�ƽ
		GPIOE->BSRRL |= (0x3<<13);//�ߵ�ƽ
		//��ʱ
		delay(500);
	}
}
