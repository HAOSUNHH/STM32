#ifndef _ADC_H_
#define _ADC_H_

#include <stm32f4xx.h>

void adc1_init(void);
u16 get_adc1_vol(void);
void adc3_init(void);
u16 get_adc3_vol(void);

#endif
