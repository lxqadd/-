#include "bsp_tft_lcd.h"	/* TFT液晶显示器驱动模块 */
/*
*输出文字到屏幕
*/
void displayTextToScreen(int x,int y,char* str)
{
	FONT_T tFont;		/* 定义一个字体结构体变量，用于设置字体参数 */
	uint16_t usX;		/* X坐标 */
	uint16_t usY;		/* Y坐标 */
	uint16_t usHeight;	/* 行高 */ 

	/* 设置字体属性 */
	tFont.usFontCode = FC_ST_16X16;		/* 字体选择宋体16点阵，高16x宽15) */
	tFont.usTextColor = CL_WHITE;		/* 字体颜色设置为白色 */
	tFont.usBackColor = CL_MASK;	 	/* 文字背景颜色，透明 */  
	tFont.usSpace = 4;					/* 首行文字，字符水平间距设置为4个像素 */
	usHeight = 20;		/* 行高 */	
	LCD_DispStr(x, y, str, &tFont);
}