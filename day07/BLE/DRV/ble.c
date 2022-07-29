#include <stm32f4xx.h>
#include <ble.h>
#include <delay.h>
#include <usart.h>

void ble_config(void)
{
	usart3_puts("AT+BAUD\r\n");
	delay_ms(100);
	usart3_puts("AT+LADDR\r\n");
	delay_ms(100);
	//ÉèÖÃÃû×Ö
	usart3_puts("AT+NAMETeacher.Zhang\r\n");
}
