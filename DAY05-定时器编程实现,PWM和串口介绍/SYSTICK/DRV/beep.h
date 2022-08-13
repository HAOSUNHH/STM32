#ifndef _BEEP_H_
#define _BEEP_H_

#include <sys.h>

//#define BEEP GPIO_Pin_8
#define BEEP PFout(8)

void beep_reg_init(void);
void beep_lib_init(void);
void beep_onoff(int flag);

#endif
