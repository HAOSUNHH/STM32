#ifndef _DAC_H_
#define _DAC_H_

#include <stm32f4xx.h>

void dac_init(void);
void set_vol(u16 vol);

#endif
