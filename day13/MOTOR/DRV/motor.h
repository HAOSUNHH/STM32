#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <sys.h>

#define IN1 PGout(15)
#define IN2 PCout(7)
#define IN3 PCout(9)
#define IN4 PBout(6)

void motor_init(void);

#endif
