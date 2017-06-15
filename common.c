#include "bsp_tft_lcd.h"	/* TFTҺ����ʾ������ģ�� */
/*
*������ֵ���Ļ
*/
void displayTextToScreen(int x,int y,char* str)
{
	FONT_T tFont;		/* ����һ������ṹ���������������������� */
	uint16_t usX;		/* X���� */
	uint16_t usY;		/* Y���� */
	uint16_t usHeight;	/* �и� */ 

	/* ������������ */
	tFont.usFontCode = FC_ST_16X16;		/* ����ѡ������16���󣬸�16x��15) */
	tFont.usTextColor = CL_WHITE;		/* ������ɫ����Ϊ��ɫ */
	tFont.usBackColor = CL_MASK;	 	/* ���ֱ�����ɫ��͸�� */  
	tFont.usSpace = 4;					/* �������֣��ַ�ˮƽ�������Ϊ4������ */
	usHeight = 20;		/* �и� */	
	LCD_DispStr(x, y, str, &tFont);
}