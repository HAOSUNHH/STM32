#include <stm32f4xx.h>
#include <iwdg.h>

void iwdg_init(void)
{
	//1.�رռĴ���д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//2.���ÿ��Ź��ķ�Ƶϵ���ͳ�ʼ����ֵ
	IWDG_SetPrescaler(IWDG_Prescaler_32);//1KHz
	IWDG_SetReload(1000);//1s
	
	//3.ι��
	IWDG_ReloadCounter();
	
	//4.ʹ�ܿ��Ź�
	IWDG_Enable();
}
