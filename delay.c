#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
//SYSCLKΪϵͳʱ��Ƶ�ʳ���1000000���ֵ��
//�������ϵͳʱ��Ƶ��Ϊ72MHz������øú���ʱ�˴���ʵ��Ϊ72 
int fac_us;
int fac_ms;
//SYSCLKΪϵͳʱ��Ƶ�ʳ���1000000���ֵ��
//�������ϵͳʱ��Ƶ��Ϊ72MHz������øú���ʱ�˴���ʵ��Ϊ72  

void mydelay_Init(u8 SYSCLK)  
{  
    SysTick->CTRL&=0xFFFFFFFB;  
    fac_us=SYSCLK/8;  
    fac_ms=(u16)(fac_us*1000);       
}  
 
//΢����ʱ
void mydelay_us(u32 nus)			   
{
 	u32 temp;
 	SysTick->LOAD = 9*nus;
 	SysTick->VAL=0X00;                       		//��ռ�����
 	SysTick->CTRL=0X01;                      		//ʹ�ܣ����������޶����������ⲿʱ��Դ
 	do
 	{
  		temp=SysTick->CTRL;                  		//��ȡ��ǰ������ֵ
 	}
	while((temp&0x01)&&(!(temp&(1<<16))));   		//�ȴ�ʱ�䵽��
    SysTick->CTRL=0x00;                     	//�رռ�����
    SysTick->VAL =0X00;                     	//��ռ����� 
}


/*������ʱ����*/  
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