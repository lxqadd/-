#ifndef __MyDELAY_H
#define __MyDELAY_H
#include "stm32f10x.h"

void mydelay_Init(u8 SYSCLK) ;
/*΢����ʱ����*/  
void mydelay_us(u32 nus);
/*������ʱ����*/  
void mydelay_ms(u16 nms);

#endif
