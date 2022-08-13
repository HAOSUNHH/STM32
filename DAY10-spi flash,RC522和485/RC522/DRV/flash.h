#ifndef _FLASH_H_
#define _FLASH_H_

#include <stm32f4xx.h>

#define SECTOR_5_ADDRESS 0x08020000

void flash_test(void);
void flash_write(u8 temp,u8 humi);

#endif
