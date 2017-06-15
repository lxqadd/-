#include "stm32f10x.h"
#include "Ta_gps.h"		   //GPS模块
#include "Ta_calibration.h"	//获得当前时间以及校准时间
#include "ta_eeprom.h"
#include "Ta_Mp3_playe.h"		/* MP3播放器模块*/
#include "Ta_KeyScan.h"
#include "bsp_tft_lcd.h"	/* TFT液晶显示器驱动模块 */

extern nmea_msg gpsx; 	//GPS信息
extern isStop;			//电机是否工作
extern	DianjiConfig dianjiTime[4];		//存放四个电机的时间
extern int MODE_1; //键值为0时是功能模式，这是默认模式
extern int MODE_2;	//键值为1时是输入校准模式
extern int keyCode;					//按键编码
extern int keyMode; 		//按键模式 


int totalSeconds=0;		//表盘从0点走过的秒数
int gpsSeconds=0;
int time[8];	//定义一个整形数组，用来存储时间。如：01083000是电机1的时间

//=============================================================================
//记录GPS时间
void record_GPSTime(void)
{
    gpsSeconds=gpsx.utc.sec+gpsx.utc.min*60+(gpsx.utc.hour+8)*3600;
}

//记录当前时间的函数,初始值为 12:00
void record_NowTiem(void)
{
  	totalSeconds+=15;		//时间经过了15秒，因为电机转动中断15秒一次
		totalSeconds%=86400;	//每隔86400(=24*3600)秒，时间经过24小时，重新计数  
 }



//校准时间，根据GPS时间进行校准，多长时间校准一次可以自己定
void calibration_Time(int cal)
{
	int timeCha;	//时间差，在电机运行时候校准用，特别是晚上
	int timeChaDJ1,timeChaDJ2,timeChaDJ3,timeChaDJ4;//四个电机在配置文件中的时间差，主要用在初始化或者恢复出厂时
	
	char buff[10];
	int j,k;	  
  record_GPSTime();//计算GPS时间  
	if(cal==0)
	{
		//默认情况下的时间差，	
		timeCha=totalSeconds-gpsSeconds;//获取时间差
		
		//时间快了，直接延时等待，钟像死了
		if(timeCha>0){
	  //停止电动机
	  isStop=1;
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
	  //开启电动机
	  isStop=0;
		}
		else{  //时间慢
			//赶时间，电机向前转
			goNSecond(-timeCha,0);
			//修正记录的时间
	    record_GPSTime();//计算GPS时间
	    totalSeconds=gpsSeconds;
		}
	}

	if(cal==1)
	{

		//否则时间就按照GPS校准
		timeCha=dianjiTime[1].hour*3600+dianjiTime[1].fen*60+dianjiTime[1].miao-gpsSeconds;
		sprintf(buff,"时间差:%d",timeCha);
		displayTextToScreen(100,100,buff,20);
		if(timeCha>0){
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
		}
		else{  //时间慢
			//赶时间，电机向前转
			//每隔15秒，电机都要受到影响，本来0.48秒转一圈，现在3.2秒转一圈，差了2.72秒
			timeCha=(-timeCha)+(-timeCha)/15*2.72;//要将时间延长，弥补耗时
			goNSecond(timeCha,1);
		}	
	}
	if(cal==2)
	{
		//电机2的时间
		timeCha=dianjiTime[2].hour*3600+dianjiTime[2].fen*60+dianjiTime[2].miao-gpsSeconds;
		sprintf(buff,"时间差:%d",timeCha);
		displayTextToScreen(100,100,buff,20);
		if(timeCha>0){
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
		}
		else{  //时间慢
			//赶时间，电机向前转
			timeCha=(-timeCha)+(-timeCha)/15*2.72;//要将时间延长，弥补耗时
			goNSecond(timeCha,2);
		}
	}
	if(cal==3)
	{
		//电机3的时间差	

		timeCha=dianjiTime[3].hour*3600+dianjiTime[3].fen+dianjiTime[3].miao-gpsSeconds;
		sprintf(buff,"时间差:%d",timeCha);
		displayTextToScreen(100,100,buff,20);
		if(timeCha>0){
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
		}
		else{  //时间慢
			//赶时间，电机向前转
			timeCha=(-timeCha)+(-timeCha)/15*2.72;//要将时间延长，弥补耗时
			goNSecond(timeCha,3);
		}
	}
	if(cal==4)
	{
		//电机4的时间差

		timeCha=dianjiTime[4].hour*3600+dianjiTime[4].fen+dianjiTime[4].miao-gpsSeconds;
		sprintf(buff,"时间差:%d",timeCha);
		displayTextToScreen(100,100,buff,20);
		if(timeCha>0){
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
		}
		else{  //时间慢
			//赶时间，电机向前转
			timeCha=(-timeCha)+(-timeCha)/15*2.72;//要将时间延长，弥补耗时
			goNSecond(timeCha,4);
		}
	}
	//将四个电机分别赶到0点，不是同时
	if(cal==5)
	{
		if(totalSeconds==0)
		{
			//初始化或者恢复出厂时候用，一次性把四个电机同时赶到0点
			//停止电动机
			isStop=1;
			
			timeChaDJ1=86400-dianjiTime[1].hour*3600+dianjiTime[1].fen*60+dianjiTime[1].miao;
			timeChaDJ2=86400-dianjiTime[2].hour*3600+dianjiTime[2].fen*60+dianjiTime[2].miao;
			timeChaDJ3=86400-dianjiTime[3].hour*3600+dianjiTime[3].fen*60+dianjiTime[3].miao;
			timeChaDJ4=86400-dianjiTime[4].hour*3600+dianjiTime[4].fen*60+dianjiTime[4].miao;
			goNSecond(timeChaDJ1,1);
			goNSecond(timeChaDJ2,2);
			goNSecond(timeChaDJ3,3);
			goNSecond(timeChaDJ4,4);	

		}
		//如果等GPS时间到了0点，电机启动，系统运行
		if(gpsSeconds==0)
		{
			//启动电动机
			isStop=0;		
		}
	}
	
}


//根据按键值校准电机
void keyCalDianJi(int keyCde)
{
	int calseconds;	//需要校准的秒数
	static int i=-1;
	char buf[10];
	char bugg[10];
	int num=0;

	//K41是输入按键，按下输入按键之后，将数组赋值	
	if(keyCode==K41)
	{
		
		displayTextToScreen_nad(50,120,"输入:",20);			
		for(;i<7;){
			switch(keyCode)
			{
				case K11:
					i++;
					time[i]=1;
					displayTextToScreen_nad(100+i*10,120,"1",20);	
				break;
				case K12:
					i++;
					time[i]=2;
					sprintf(buf,"%d",time[i]);
					displayTextToScreen_nad(100+i*10,120,"2",20);	
				break;
				case K13: 
					i++;
					time[i]=3;
					sprintf(buf,"%d",time[i]);
					displayTextToScreen_nad(100+i*10,120,"3",20);		
				break;
				case K21:
					i++;				
					time[i]=4;
					sprintf(buf,"%d",time[i]);
					displayTextToScreen_nad(100+i*10,120,buf,20);	
					break;			
				case K22:
					i++;
					time[i]=5;
					sprintf(buf,"%d",time[i]);
					displayTextToScreen_nad(100+i*10,120,buf,20);	
				break;
				case K23: 
					i++;
					time[i]=6;
					sprintf(buf,"%d",time[i]);
					displayTextToScreen_nad(100+i*10,120,buf,20);		
				break;
				case K31: 
					i++;
					time[i]=7;
					sprintf(buf,"%d",time[i]);
					displayTextToScreen_nad(100+i*10,120,buf,20);	
				break;
				case K32: 
					i++;
					time[i]=8;
					sprintf(buf,"%d",time[i]);
					displayTextToScreen_nad(100+i*10,120,buf,20);	
				break;
				case K33: 
					i++;
					time[i]=9;
					sprintf(buf,"%d",time[i]);
					displayTextToScreen_nad(100+i*10,120,buf,20);	
				break;
				case K42: 
					i++;
					time[i]=0;
					sprintf(buf,"%d",time[i]);
					displayTextToScreen_nad(100+i*10,120,"0",20);	
				break;		
				default:	    
				break;
			}
			break;
		}
	}//end of for




















	//按键43是确认按键，将字符串分析成需要校准的秒数
	if(keyCode==K43)
	{
		//计算当前电机和GPS相差的秒数
		calseconds=(time[2]*10+time[3])*3600+(time[4]*10+time[5])*60+time[6]*10+time[7]-gpsSeconds;

		
		if(time[0]*10+time[1]==1)
		{
			displayTextToScreen_nad(240,120,"确认",20);	//确认并退出
			goNSecond(calseconds,1);
			keyMode=0;//模式切换为默认
			initAD();//启动广告
			i=0;
			isStop=0;//表示启动电机
			return;
		}
		if(time[0]*10+time[1]==2)
		{
			displayTextToScreen_nad(240,120,"确认",20);
			goNSecond(calseconds,2);
			keyMode=0;//模式切换为默认
			initAD();//启动广告
			i=-1;
			isStop=0;//表示启动电机
			return;
		}
		if(time[0]*10+time[1]==3)
		{
			displayTextToScreen_nad(240,120,"确认",20);
			goNSecond(calseconds,3);
			keyMode=0;//模式切换为默认
			initAD();//启动广告
			i=-1;
			isStop=0;//表示启动电机
			return;
		}
		if(time[0]*10+time[1]==4)
		{
			displayTextToScreen_nad(240,120,"确认",20);
			goNSecond(calseconds,4);
			keyMode=0;//模式切换为默认
			initAD();//启动广告
			i=-1;
			isStop=0;//表示启动电机
			return;
		}
		else
		{
			displayTextToScreen_nad(50,140,"输入错误，请切换模式重新输入",20);		
			keyMode=0;//模式切换为默认
			i=-1;
			return;
		}
	}
	if(keyCode==K43)
	{
		keyMode=0;//模式切换为默认
		i=-1;
		return;
	}
	
}

