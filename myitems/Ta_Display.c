
#include "Ta_Display.h"

//屏幕显示函数
 void displayTextToScreen(int x,int y,char* str,int lineHeight)
{
	FONT_T tFont;		/* 定义一个字体结构体变量，用于设置字体参数 */

	/* 设置字体属性 */
	tFont.usFontCode = FC_ST_16X16;	/* 字体选择宋体16点阵，高16x宽15) */
	tFont.usTextColor = CL_WHITE;		/* 字体颜色设置为白色 */
	tFont.usBackColor = CL_MASK;	 	/* 文字背景颜色，透明 */
	tFont.usSpace = 2;					/* 字符水平间距, 单位 = 像素 */	
    
	//广告词
	LCD_DispStr(x-40, y-62, "======邢台职业技术学院=====", &tFont);
	
	LCD_DispStr(x, y, str, &tFont);
	
	//广告词
	LCD_DispStr(x-60, y+62, "======版权所有2016-2020=====", &tFont);

	
}
//没有广告的显示
void displayTextToScreen_nad(int x,int y,char* str,int lineHeight)
{
	FONT_T tFont;		/* 定义一个字体结构体变量，用于设置字体参数 */

	/* 设置字体属性 */
	tFont.usFontCode = FC_ST_16X16;	/* 字体选择宋体16点阵，高16x宽15) */
	tFont.usTextColor = CL_WHITE;		/* 字体颜色设置为白色 */
	tFont.usBackColor = CL_MASK;	 	/* 文字背景颜色，透明 */
	tFont.usSpace = 2;					/* 字符水平间距, 单位 = 像素 */	
	
	LCD_DispStr(x, y, str, &tFont);


	
}