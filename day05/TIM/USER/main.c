#include <stm32f4xx.h>
#include <led.h>
#include <beep.h>
#include <key.h>
#include <mq2.h>
#include <delay.h>
#include <timer.h>
#include <pwm.h>

int main()
{
	u32 comp = 0;
	
	//1.配置中断优先级分组 2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	//beep_reg_init();
	beep_lib_init();
	//key_init();
	exti_init();
	mq2_init();
	//timer2_init();
	//timer10_init();
	timer14_pwm_init();
	timer1_pwm_init();
	
	while(1){
		//2s从最暗到最亮
		while(comp<999){
			TIM_SetCompare1(TIM14,comp);
			TIM_SetCompare4(TIM1,comp);
			comp++;
			delay_ms(2);
		}
		
		//2s从最亮到最暗
		while(comp>0){
			TIM_SetCompare1(TIM14,comp);
			TIM_SetCompare4(TIM1,comp);
			comp--;
			delay_ms(2);
		}
		
		delay_ms(200);                                                                                                                                                        
	}
}
