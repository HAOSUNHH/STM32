#include <stm32f4xx.h>
#include <led.h>
#include <beep.h>


int main()
{
	led_init();
	//beep_reg_init();
	beep_lib_init();
	
	while(1){
		//beep_onoff(1);
		//��ʱ
		delay(500);
		
		beep_onoff(0);
		//��ʱ
		delay(500);
	}
}
