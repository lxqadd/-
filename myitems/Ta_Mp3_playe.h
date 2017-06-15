#ifndef __MP3_PLAYER_H
#define __MP3_PLAYER_H

#include "stm32f10x.h"
#include "string.h"

#define VOLUME_MAX		180
#define VOLUME_STEP		5

/* ����һ������MP3�������Ľṹ�� 
����ȫ�ֱ�������
*/
typedef struct
{
	uint8_t ucMuteOn;			/* 0 : ������ 1: ���� */
	uint8_t ucVolume;			/* ��ǰ���� */
	uint32_t uiProgress;		/* ��ǰ����(�Ѷ�ȡ���ֽ���) */
	uint8_t ucPauseEn;			/* ��ͣʹ�� */
}MP3_T;

//=======================1======================================
//�����ļ�
typedef struct
{
  int hour;	 					/* ��ȡ�����ļ��еģ�Сʱ */	
  int fen;						/* ��ȡ�����ļ��еģ����� */
  int miao;						/* ��ȡ�����ļ��еģ��� */
  int baoshi_Start;					/* ���㱨ʱ */
	int baoshi_End;
	int isBaoshi;				/*�Ƿ�ʱ*/
	int zhaomiang_Start; /*��������*/
	int zhaoming_End;
	int isZhaoMing;     /*�Ƿ�����*/

}DianjiConfig;					//

typedef struct{
	int funNum;//���ܺ�
	char values[6];
}Fun;

//=======================1======================================


//Mp3�����ļ�
int Mp3PlayerAFile(char* fileName);
//�����ļ�
void Mp3Pro(void);

int readConfigFile(char* fileName);	//��ȡ�����ļ�����
//�����ļ�������
void chuli_conf(char* str);

#endif
