#include <stm32f4xx.h>
#include <led.h>
#include <beep.h>
#include <key.h>

int main()
{
	int key_flag = 0;
	
	led_init();
	//beep_reg_init();
	beep_lib_init();
	key_init();
	
	while(1){
		if(S1==0){
			//��ʱ����10ms
			delay(100);
			if(S1==0){//��ʵ�����¼�
				if(key_flag==0){//����û���ɿ�
					D1 = ~D1;//ȡ��
					
					key_flag = 1;
				}
			}
		}
		else{
			//��ʱ����10ms
			delay(100);
			if(S1){
				key_flag = 0;
			}
		}
		
		
		/*
		if(GPIOA->IDR & 0x1){//�ɿ�
			D1 = 1;
		}
		else{//����
			D1 = 0;
		}
		
		//λ�β���
		if(S2){//�ɿ�
			D2 = 1;
		}
		else{//����
			D2 = 0;
		}
		
		//�⺯��
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)){//�ɿ�
			D3 = 1;
		}
		else{//����
			D3 = 0;
		}
		
		//�Ĵ���
		if(GPIOE->IDR & (0x1<<4)){//�ɿ�
			D4 = 1;
			BEEP = 0;
		}
		else{//����
			D4 = 0;
			BEEP = 1;
		}
		*/
	}
}
