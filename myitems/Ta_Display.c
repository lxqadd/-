
#include "Ta_Display.h"

//��Ļ��ʾ����
 void displayTextToScreen(int x,int y,char* str,int lineHeight)
{
	FONT_T tFont;		/* ����һ������ṹ���������������������� */

	/* ������������ */
	tFont.usFontCode = FC_ST_16X16;	/* ����ѡ������16���󣬸�16x��15) */
	tFont.usTextColor = CL_WHITE;		/* ������ɫ����Ϊ��ɫ */
	tFont.usBackColor = CL_MASK;	 	/* ���ֱ�����ɫ��͸�� */
	tFont.usSpace = 2;					/* �ַ�ˮƽ���, ��λ = ���� */	
    
	//����
	LCD_DispStr(x-40, y-62, "======��ְ̨ҵ����ѧԺ=====", &tFont);
	
	LCD_DispStr(x, y, str, &tFont);
	
	//����
	LCD_DispStr(x-60, y+62, "======��Ȩ����2016-2020=====", &tFont);

	
}
//û�й�����ʾ
void displayTextToScreen_nad(int x,int y,char* str,int lineHeight)
{
	FONT_T tFont;		/* ����һ������ṹ���������������������� */

	/* ������������ */
	tFont.usFontCode = FC_ST_16X16;	/* ����ѡ������16���󣬸�16x��15) */
	tFont.usTextColor = CL_WHITE;		/* ������ɫ����Ϊ��ɫ */
	tFont.usBackColor = CL_MASK;	 	/* ���ֱ�����ɫ��͸�� */
	tFont.usSpace = 2;					/* �ַ�ˮƽ���, ��λ = ���� */	
	
	LCD_DispStr(x, y, str, &tFont);


	
}