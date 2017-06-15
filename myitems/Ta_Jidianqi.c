#include "stm32f10x.h"
#include "delay.h"
#include "ta_jidianqi.h"

#define MP3BOFANG 1
#define ZHAOMING	2

//打开继电器，置高电平,参数m表示照明还是MP3
void setOpen(int m){
	if(m==MP3BOFANG)	
  GPIO_ResetBits(GPIOF, GPIO_Pin_4);
	if(m==ZHAOMING)
	GPIO_ResetBits(GPIOF, GPIO_Pin_5);
}
//关闭继电器，置低电平
void setClosed(int m)
{
	if(m==MP3BOFANG)
	GPIO_SetBits(GPIOF, GPIO_Pin_4);
	if(m==ZHAOMING)	
	GPIO_SetBits(GPIOF, GPIO_Pin_5);
}


//初始化继电器管脚	PF4，（可修改管脚）
void initjdq(){
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	//控制MP3播放的管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//控制灯亮的管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}


