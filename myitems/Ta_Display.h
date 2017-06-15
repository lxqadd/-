#ifndef TA_DESPLAY_H
#define TA_DESPLAY_H
#include "stm32f10x.h"
#include "bsp_tft_lcd.h"	/* TFTÒº¾§ÏÔÊ¾Æ÷Çý¶¯Ä£¿é */

void displayTextToScreen(int x,int y,char* str,int lineHeight);

#endif