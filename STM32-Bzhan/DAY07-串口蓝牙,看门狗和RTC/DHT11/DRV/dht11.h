#ifndef _DHT11_H_
#define _DHT11_H_

#include <stm32f4xx.h>

#define DQ_IN PGin(9)
#define DQ_OUT PGout(9)

int dht11_check(void);
int dht11_read_data(u8 *temp,u8 *humi);

#endif
