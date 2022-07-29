#ifndef _RTC_H_
#define _RTC_H_

#include <stm32f4xx.h>

#define TOBCD(n) (n%10+(n/10)*16)

void rtc_init(void);
void rtc_alarm_init(void);
void rtc_set_time(u8 hour,u8 min,u8 sec);

#endif
