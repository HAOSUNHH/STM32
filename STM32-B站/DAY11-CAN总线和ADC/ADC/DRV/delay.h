#ifndef _DELAY_H_
#define _DELAY_H_

void systick_init(void);
void delay_ms(unsigned int nms);
void delay_us(unsigned int nus);
void delay_s(unsigned int ns);

#endif
