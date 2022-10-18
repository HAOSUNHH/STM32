#ifndef _USART_H_
#define _USART_H_

void usart1_init(void);
void usart1_putc(char ch);
void usart1_puts(const char *s);
void parse_cmd(void);

#endif
