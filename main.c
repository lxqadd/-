#include "stm32f10x.h"			/* 如果要用ST的固件库，必须包含这个文件 */
#include <stdio.h>					/* 因为用到了printf函数，所以必须包含这个文件 */
#include "bsp_usart.h"			/* printf函数定向输出到串口，所以必须包含这个文件 */
#include "bsp_tft_lcd.h"		/* TFT液晶显示器驱动模块 */
#include "uart_api.h"	   		//串口各种功能实现
#include "Ta_Motor.h"
#include "Ta_KeyScan.h"
#include "Ta_Display.h"
#include "Ta_gps.h"		   		//GPS模块

#include "bsp_sdio_sd.h"  	/* SD卡驱动模块 */
#include "ff.h"							/* FatFS文件系统模块*/	
#include "Ta_Mp3_playe.h"		/* MP3播放器模块*/
#include "bsp_24xx02.h"			/*EEPROM芯片驱动*/

//按键模式切换
int MODE_1=0; //键值为0时是功能模式，这是默认模式
int MODE_2=1;	//键值为1时是输入校准模式


extern nmea_msg gpsx; 			//GPS信息
extern __align(4) u8 dtbuf[50]; 		//缓冲区
extern g_TxBuf2[UART2_TX_BUF_SIZE];	//串口2的发送缓冲区
extern g_RxBuf2[UART2_RX_BUF_SIZE];	//串口2的接收缓冲区 
extern char *config_txt;  	//读取SD卡配置文本信息

extern int isStop;					//1表示停止电机转动，0表示启动电机
extern int keyCode;					//按键编码
extern char *fileName[];		//整点报时文件名数组
extern uint8_t buff_ReadFromEEPROM[DATA_LEN];
extern int sucess;					//读取EEPROM是否成功
extern totalSeconds;
extern Fun funs[10];				//十个功能结构体
extern	DianjiConfig dianjiTime[4];		//存放四个电机的时间
extern DianjiConfig dianjiOthers;		//存放电机其他配置，如报时和照明



static void InitBoard(void); 	//硬件初始化
void process(void);						//主循环部分
void doKeyEvents(int keyCde);	//键盘响应事件
void initAD(void);			 			//启动后的广告
int keyMode; 		//按键模式 


/*
*主程序入口    
*/
int main(void)
{
		int i=0;
		int time=0;
   //硬件初始化
   	InitBoard(); 	//初始化开发板，启用各种功能
   
   //启动后的初值
   //initValues();   

    initAD();//启动广告
		
		//读取EEPROM的值
		ee_Read();
		
		//进入主程序
		process();
		
		

}















//主循环部分
void process(void){   
	//启动键盘默认模式
	keyMode=MODE_1;
	
	SetMonorDirection(DJ1,SHUN_SHI_ZHEN);//设置电机1顺时针方向
	SetMonorDirection(DJ2,SHUN_SHI_ZHEN);//设置电机2顺时针方向
	SetMonorDirection(DJ3,SHUN_SHI_ZHEN);//设置电机1顺时针方向
	SetMonorDirection(DJ4,SHUN_SHI_ZHEN);//设置电机2顺时针方向

	isStop=1; 		//1表示停止电机转动，赶时间时不起作用	
								 	
	Time2Init(15);//启动电机定时器，设置电机15秒走一圈，必须大于4小于32

	Time4Init(20);////启动矩阵键盘检测定时器，单位为ms	

	//主循环
	while(1){
			setClosed(1);			//关闭继电器	
			setClosed(2);			//关闭继电器
			if(keyMode==0)
			{
				doKeyEvents(keyCode);//调用键盘处理函数
			}			
			if(keyMode==1)
			{
			
				keyCalDianJi(keyCode);	//输入数字校准电机
			}
	}
}

//键盘处理函数
void doKeyEvents(int keyCde){
  static int oldKeyCode=-1;
  static int resetTimes=0;//记录复位按键次数，10次执行复位
  int j=0;
  char buf[10];
	char buff[10];
  if(keyCode==oldKeyCode)return ; 	
 
 switch(keyCode){
    case K11: //启动电机
			isStop=0;//表示启动电机
			LCD_ClrScr(CL_BLUE);  	
	  	displayTextToScreen(100,120,"启动电机[ K11 ]...",20);	
	  break;
	 case K12: //停止电机
	  	isStop=1;//表示停止电机
			LCD_ClrScr(CL_BLUE);  	
			if(keyCode!=oldKeyCode)displayTextToScreen(100,120,"停止电机[ K12 ]...",20);
		break;		
	 case K13: //语音报时测试	
			//打开继电器
			setOpen();
			LCD_ClrScr(CL_BLUE); 
			if(keyCode!=oldKeyCode)displayTextToScreen(100,120,"语音报时[ K13 ]...",20);
			Mp3PlayerAFile(fileName[1]);//报时8点整	
	 	break;

	case K14: //读取SD卡配置信息	
		LCD_ClrScr(CL_BLUE); 
		readConfigFile("config.txt");
		sprintf(buf,"功能号位:%d",funs[8].funNum);
		sprintf(buff,"电机时间:%d",dianjiTime[1].hour*3600+dianjiTime[1].fen*60+dianjiTime[1].miao);
		//sprintf(buff,"是否照明:%d",dianjiOthers.isZhaoMing);
		displayTextToScreen(100,120,buf,20); 
		displayTextToScreen(100,140,buff,20); 		
		break; 
		
	case K21: //校准时间	
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"校准时间[ K21 ]...",20);		
		calibration_Time(0);		    
		break;
 
	case K22: //显示GPS状态	  	
		GPS_Analysis(&gpsx,(u8*)g_RxBuf2);//分析字符串
		LCD_ClrScr(CL_BLUE);  	
		sprintf((char *)dtbuf,"%ld",(u8*)g_RxBuf2); //需要处理的字符串来自于g_RxBuf2
		Gps_Msg_Show(); //GPS信息显示				 
	  break;
	case K23: //测试初始化时电机运转 ，初始校准，
		LCD_ClrScr(CL_BLUE);  	
		displayTextToScreen(100,120,"初始校准",20);	
		calibration_Time(5);
	  break;
  
	case K24://TODO恢复出厂模式，让EEPROM中保存0  
	   	 LCD_ClrScr(CL_BLUE);
			if(resetTimes<9){		 
		   displayTextToScreen(100,120,"恢复出厂设置吗？请再按复位键！",20);
		   resetTimes++;//记录次数
		   break;
		  }
		
		 ee_Erase();  //擦出EEPROM
		 delay_us(100);
	   ee_Write(0); //初始值0秒，写入EEPROM
		 delay_us(100);
		 ee_Read();
		 /*
		 for(j=0;j<DATA_LEN;j++){
		   buff_ReadFromEEPROM[j]=j;
		 } */
		 buff_ReadFromEEPROM[DATA_LEN]=0;
		 for(j=0;j<DATA_LEN;j++){
		   buff_ReadFromEEPROM[j]+=48;
		 }
		 displayTextToScreen(100,120,buff_ReadFromEEPROM,20);
		 displayTextToScreen(100,120,"复位成功!",20);
		 resetTimes=0; 
	     break;
	case K31: //校准电机1的时间
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"校准电机1时间[ K31 ]...",20);	
		readConfigFile("config.txt");
		calibration_Time(1);		    
		break;
	case K32: //校准电机2的时间
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"校准电机2时间[ K32 ]...",20);	
		readConfigFile("config.txt");		
		calibration_Time(2);		    
		break;	
	case K33: //校准电3的时间
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"校准电机3时间[ K33 ]...",20);		
		readConfigFile("config.txt");
		calibration_Time(3);		    
		break;	
	case K34: //校准电机4的时间
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"校准电机4时间[ K34 ]...",20);		
		readConfigFile("config.txt");
		calibration_Time(4);		    
		break;
	case K44:
		LCD_ClrScr(CL_BLUE);
		keyMode=1;
		displayTextToScreen(100,100,"模式切换...",20);
	break;

	
	default:	    
	    break;
  }
    
	oldKeyCode=keyCode;//记录上次按键
  
}




/*初始化开发板
*初始化 屏幕
*初始化 LED灯
*初始化 按键
*初始化 串口
*初始化 定时器
*初始化 电动机
*初始化 4*4键盘
*/
static void InitBoard(void)
{	
	/* 初始化systick定时器，并启动定时中断 */

	bsp_InitButton();	//没有这个，按键很可怜，就死了》这时候为啥米呢？！！！
  
 	bsp_InitTimer(); 	//必须在按键初始化之后
	

	LCD_InitHard();	/* 初始化显示器硬件(配置GPIO和FSMC,给LCD发送初始化指令) */	
	LCD_SetBackLight(BRIGHT_DEFAULT);   
  	LCD_ClrScr(CL_BLUE);  	/* 清屏，背景蓝色 */


	keyboard_init(); 		//按键初始化

  	initjdq();				//初始化继电器

 	initMotor();			//初始化步进电机
	
	comInit();			    //GPS串口COM3初始化

	//启动电机转动定时器
	TIM2_NVIC_Configuration();

	//启动按键检测定时器
	TIM4_NVIC_Configuration();
	


}

//广告
void initAD(void){
   int x=100;
   int y=120;
   	FONT_T tFont;		/* 定义一个字体结构体变量，用于设置字体参数 */

	/* 设置字体属性 */
	tFont.usFontCode = FC_ST_16X16;	/* 字体选择宋体16点阵，高16x宽15) */
	tFont.usTextColor = CL_WHITE;		/* 字体颜色设置为白色 */
	tFont.usBackColor = CL_MASK;	 	/* 文字背景颜色，透明 */
	tFont.usSpace = 2;					/* 字符水平间距, 单位 = 像素 */	
    //广告词
	LCD_ClrScr(CL_BLUE);
	LCD_DispStr(x-40, y-62, "======邢台职业技术学院=====", &tFont);
	
	LCD_DispStr(x, y, "请按键启动系统！", &tFont);
	
	//广告词
	LCD_DispStr(x-60, y+62, "======版权所有2016-2020=====",&tFont);
}
