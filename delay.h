#ifndef __MyDELAY_H
#define __MyDELAY_H
#include "stm32f10x.h"

void mydelay_Init(u8 SYSCLK) ;
/*微秒延时函数*/  
void mydelay_us(u32 nus);
/*毫秒延时函数*/  
void mydelay_ms(u16 nms);

#endif
