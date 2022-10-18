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
			//延时消抖10ms
			delay(100);
			if(S1==0){//真实按键事件
				if(key_flag==0){//按下没有松开
					D1 = ~D1;//取反
					
					key_flag = 1;
				}
			}
		}
		else{
			//延时消抖10ms
			delay(100);
			if(S1){
				key_flag = 0;
			}
		}
		
		
		/*
		if(GPIOA->IDR & 0x1){//松开
			D1 = 1;
		}
		else{//按下
			D1 = 0;
		}
		
		//位段操作
		if(S2){//松开
			D2 = 1;
		}
		else{//按下
			D2 = 0;
		}
		
		//库函数
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)){//松开
			D3 = 1;
		}
		else{//按下
			D3 = 0;
		}
		
		//寄存器
		if(GPIOE->IDR & (0x1<<4)){//松开
			D4 = 1;
			BEEP = 0;
		}
		else{//按下
			D4 = 0;
			BEEP = 1;
		}
		*/
	}
}
