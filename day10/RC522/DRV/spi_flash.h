#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include <stm32f4xx.h>
#include <sys.h>

#define W25Q128_CS PBout(14)

void spi1_init(void);
u16 w25q128_read_id(void);
void w25q128_read_data(u32 addr,u8 *data,u8 len);
void w25q128_erase_sector(u32 addr);
void w25q128_write_page(u32 addr,u8 *data,u8 len);


#endif
