#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "GPIOLIKE51.h"


void GPIO_Configuration(void);
int main(void)
{   int TRUE=1;
    GPIO_Configuration();
    uart_init(9600);
    delay_init();
    while(TRUE){
        PCout(13)=1;
        USART_SEND(USART1,"Hello World!\n");
        delay_ms(1000);
    }
    return 0;
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE);
//=============================================================================
//LED -> PC13
//=============================================================================
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}