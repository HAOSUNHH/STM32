#ifndef _CAN_H_
#define _CAN_H_

#include <stm32f4xx.h>

void can1_init(void);
u8 can1_send_message(u8 *data,u8 len,u32 message_id);

#endif
