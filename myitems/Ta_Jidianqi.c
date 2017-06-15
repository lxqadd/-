#include "stm32f10x.h"
#include "delay.h"
#include "ta_jidianqi.h"

#define MP3BOFANG 1
#define ZHAOMING	2

//�򿪼̵������øߵ�ƽ,����m��ʾ��������MP3
void setOpen(int m){
	if(m==MP3BOFANG)	
  GPIO_ResetBits(GPIOF, GPIO_Pin_4);
	if(m==ZHAOMING)
	GPIO_ResetBits(GPIOF, GPIO_Pin_5);
}
//�رռ̵������õ͵�ƽ
void setClosed(int m)
{
	if(m==MP3BOFANG)
	GPIO_SetBits(GPIOF, GPIO_Pin_4);
	if(m==ZHAOMING)	
	GPIO_SetBits(GPIOF, GPIO_Pin_5);
}


//��ʼ���̵����ܽ�	PF4�������޸Ĺܽţ�
void initjdq(){
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	//����MP3���ŵĹܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//���Ƶ����Ĺܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}


