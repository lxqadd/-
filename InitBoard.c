#include "stm32f10x.h"		/* 如果要用ST的固件库，必须包含这个文件 */
#include <stdio.h>			/* 因为用到了printf函数，所以必须包含这个文件 */
#include "bsp_usart.h"		/* printf函数定向输出到串口，所以必须包含这个文件 */
#include "bsp_led.h"		/* LED指示灯驱动模块 */
#include "bsp_button.h"		/* 按键驱动模块 */
#include "bsp_timer.h"		/* systick定时器模块 */
#include "bsp_tft_lcd.h"	/* TFT液晶显示器驱动模块 */
#include "keyscan.h"
#include "motor.h"
#include "uart_api.h"		

extern int keyCode;																//按键编码

void displayTextToScreen(int x,int y,char* str);	//输出文字到屏幕

static void InitBoard(void); 											//初始化开发板

void initSysClock(){
  
}

/*
*主程序入口
*/
int main(void)
{
	InitBoard();							//初始化开发板，启用各种功能	
	fangXiang(ZHENG);					//控制方向		

	//启动电机
	TIM2_NVIC_Configuration();
	Time2Init(5);//必须大于4小于32

	 printf("=================\n");

	while(1)
	{		
		//startBiaopan(1);				//启动表盘1
		//goMotor();	
	    
		//goOneCircle();
	}
}


void GPS_Init(){
  
}


static void InitBoard(void)
{	
	//1. 设置系统时钟  72MHZ
	//TODO,目前为72MHz

	//2. GPS模块初始化  TTL电平  串口1  9600bps
	

	//3. EEPROM初始化  256字节 存储空间

	//4. 显示初始化 （可用数码管）	支持汉字

	//5. 4*4矩阵键盘初始化  8个引脚  输入模式

	//6. 电机驱动模块初始化 	4*2   GND、VCC共用?
	
	//7. MP3报时模块初始化  含继电器模块初始化  MP3模块初始化
	
	//8. 照明模块初始化		继电器初始化
	
	//9. 定时器初始化       
	//延时定时器，实现us ms s   GPS定时器   步进电机定时器  

	//10. 键盘中断初始化


	
	LCD_SetBackLight(255);	 			//点亮屏幕
 	LCD_ClrScr(CL_BLUE);  			/* 清屏，背景蓝色 */
	/* 配置按键GPIO, 必须在bsp_InitTimer之前调用 */
	bsp_InitButton();
	/* 初始化systick定时器，并启动定时中断 */
   	bsp_InitTimer(); 	
	LCD_InitHard();							/* 初始化显示器硬件(配置GPIO和FSMC,给LCD发送初始化指令) */
	initMotor();			   			//初始化开发板
	//初始化键盘,通常放在后边
	keyboard_init();
	initjdq();								//初始化继电器

	
}