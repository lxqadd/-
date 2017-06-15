#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
//SYSCLK为系统时钟频率除以1000000后的值，
//例：如果系统时钟频率为72MHz，则调用该函数时此处的实参为72 
int fac_us;
int fac_ms;
//SYSCLK为系统时钟频率除以1000000后的值，
//例：如果系统时钟频率为72MHz，则调用该函数时此处的实参为72  

void mydelay_Init(u8 SYSCLK)  
{  
    SysTick->CTRL&=0xFFFFFFFB;  
    fac_us=SYSCLK/8;  
    fac_ms=(u16)(fac_us*1000);       
}  
 
//微秒延时
void mydelay_us(u32 nus)			   
{
 	u32 temp;
 	SysTick->LOAD = 9*nus;
 	SysTick->VAL=0X00;                       		//清空计数器
 	SysTick->CTRL=0X01;                      		//使能，减到零是无动作，采用外部时钟源
 	do
 	{
  		temp=SysTick->CTRL;                  		//读取当前倒计数值
 	}
	while((temp&0x01)&&(!(temp&(1<<16))));   		//等待时间到达
    SysTick->CTRL=0x00;                     	//关闭计数器
    SysTick->VAL =0X00;                     	//清空计数器 
}


/*毫秒延时函数*/  
void mydelay_ms(u16 nms)  
{  
    u32 temp;  
    SysTick->LOAD=(u32)(nms*fac_ms);  
    SysTick->VAL=0x00;  
    SysTick->CTRL=0x01;  
    do  
    {  
        temp=SysTick->CTRL;  
    }  
    while(temp&0x01&&!(temp&0x10000));  
    SysTick->CTRL=0x00;  
    SysTick->VAL=0x00;     
}