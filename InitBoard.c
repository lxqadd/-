#include "stm32f10x.h"		/* ���Ҫ��ST�Ĺ̼��⣬�����������ļ� */
#include <stdio.h>			/* ��Ϊ�õ���printf���������Ա����������ļ� */
#include "bsp_usart.h"		/* printf����������������ڣ����Ա����������ļ� */
#include "bsp_led.h"		/* LEDָʾ������ģ�� */
#include "bsp_button.h"		/* ��������ģ�� */
#include "bsp_timer.h"		/* systick��ʱ��ģ�� */
#include "bsp_tft_lcd.h"	/* TFTҺ����ʾ������ģ�� */
#include "keyscan.h"
#include "motor.h"
#include "uart_api.h"		

extern int keyCode;																//��������

void displayTextToScreen(int x,int y,char* str);	//������ֵ���Ļ

static void InitBoard(void); 											//��ʼ��������

void initSysClock(){
  
}

/*
*���������
*/
int main(void)
{
	InitBoard();							//��ʼ�������壬���ø��ֹ���	
	fangXiang(ZHENG);					//���Ʒ���		

	//�������
	TIM2_NVIC_Configuration();
	Time2Init(5);//�������4С��32

	 printf("=================\n");

	while(1)
	{		
		//startBiaopan(1);				//��������1
		//goMotor();	
	    
		//goOneCircle();
	}
}


void GPS_Init(){
  
}


static void InitBoard(void)
{	
	//1. ����ϵͳʱ��  72MHZ
	//TODO,ĿǰΪ72MHz

	//2. GPSģ���ʼ��  TTL��ƽ  ����1  9600bps
	

	//3. EEPROM��ʼ��  256�ֽ� �洢�ռ�

	//4. ��ʾ��ʼ�� ����������ܣ�	֧�ֺ���

	//5. 4*4������̳�ʼ��  8������  ����ģʽ

	//6. �������ģ���ʼ�� 	4*2   GND��VCC����?
	
	//7. MP3��ʱģ���ʼ��  ���̵���ģ���ʼ��  MP3ģ���ʼ��
	
	//8. ����ģ���ʼ��		�̵�����ʼ��
	
	//9. ��ʱ����ʼ��       
	//��ʱ��ʱ����ʵ��us ms s   GPS��ʱ��   ���������ʱ��  

	//10. �����жϳ�ʼ��


	
	LCD_SetBackLight(255);	 			//������Ļ
 	LCD_ClrScr(CL_BLUE);  			/* ������������ɫ */
	/* ���ð���GPIO, ������bsp_InitTimer֮ǰ���� */
	bsp_InitButton();
	/* ��ʼ��systick��ʱ������������ʱ�ж� */
   	bsp_InitTimer(); 	
	LCD_InitHard();							/* ��ʼ����ʾ��Ӳ��(����GPIO��FSMC,��LCD���ͳ�ʼ��ָ��) */
	initMotor();			   			//��ʼ��������
	//��ʼ������,ͨ�����ں��
	keyboard_init();
	initjdq();								//��ʼ���̵���

	
}