#ifndef __MP3_PLAYER_H
#define __MP3_PLAYER_H

#include "stm32f10x.h"
#include "string.h"

#define VOLUME_MAX		180
#define VOLUME_STEP		5

/* 定义一个用于MP3播放器的结构体 
便于全局变量操作
*/
typedef struct
{
	uint8_t ucMuteOn;			/* 0 : 静音， 1: 放音 */
	uint8_t ucVolume;			/* 当前音量 */
	uint32_t uiProgress;		/* 当前进度(已读取的字节数) */
	uint8_t ucPauseEn;			/* 暂停使能 */
}MP3_T;

//=======================1======================================
//配置文件
typedef struct
{
  int hour;	 					/* 读取配置文件中的：小时 */	
  int fen;						/* 读取配置文件中的：分钟 */
  int miao;						/* 读取配置文件中的：秒 */
  int baoshi_Start;					/* 整点报时 */
	int baoshi_End;
	int isBaoshi;				/*是否报时*/
	int zhaomiang_Start; /*整点照明*/
	int zhaoming_End;
	int isZhaoMing;     /*是否照明*/

}DianjiConfig;					//

typedef struct{
	int funNum;//功能号
	char values[6];
}Fun;

//=======================1======================================


//Mp3播放文件
int Mp3PlayerAFile(char* fileName);
//播放文件
void Mp3Pro(void);

int readConfigFile(char* fileName);	//读取配置文件参数
//配置文件处理函数
void chuli_conf(char* str);

#endif
