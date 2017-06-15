#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_timer.h"
#include "bsp_button.h"
#include "bsp_tft_lcd.h"
#include "bsp_VS1003B.h"
#include "bsp_sdio_sd.h"
#include "ff.h"
#include "Ta_Mp3_playe.h"

#define LINE_CAP	18		/* 定义行间距 */
#define MAX_FILE_COUNT 64   //盘里面最多方的文件个数



MP3_T g_tMP3;

/* 访问Fatfs用到的全局变量 */
FRESULT g_Result;
FATFS   g_fs;
DIR     g_DirInf;
FILINFO g_FileInf;
FIL 	g_File;





char* fileName[]={"00语音.MP3","01语音.MP3","02语音.MP3","03语音.MP3",
"04语音.MP3","05语音.MP3","06语音.MP3","07语音.MP3","08语音.MP3","09语音.MP3",
"10语音.MP3","11语音.MP3","12语音.MP3","13语音.MP3","14语音.MP3","15语音.MP3",
"16语音.MP3","17语音.MP3","18语音.MP3","19语音.MP3","20语音.MP3","21语音.MP3",
"22语音.MP3","23语音.MP3"};

char config_txt[100];	//存放SD卡config.txt文件内容
Fun funs[10];			//十个功能结构体
DianjiConfig dianjiTime[4];		//存放四个电机的时间
DianjiConfig dianjiOthers;		//存放电机其他配置，如报时和照明
u8  buf;

/*
*********************************************************************************************************
*	函 数 名: Mp3Player
*	功能说明: MP3播放器主程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
int Mp3PlayerAFile(char* fileName)
{
	FRESULT result;
	DIR DirInf;
	uint8_t fFound = 0;
	int maxFileCount=100;
	int i;

	vs1003_Init();
	vs1003_SoftReset();

	g_tMP3.ucVolume = 10; 			/* 缺省音量,越大声音越小 */
	vs1003_SetVolume(g_tMP3.ucVolume);

 	/* 挂载文件系统 */
	g_Result = f_mount(0, &g_fs);	/* Mount a logical drive */
	
	if (g_Result != FR_OK)
	{
		return	-1;//表示读卡失败
	}
	else
	{
		/* 打开根文件夹 */
		result = f_opendir(&DirInf, "/"); /* 如果不带参数，则从当前目录开始 */
		if (result != FR_OK)
		{
		  return -1;//表示读取文件夹失败
		}
		fFound = 0;//是否找到文件
		for(i=0;i<MAX_FILE_COUNT;i++)
		{
			result = f_readdir(&DirInf,&g_FileInf); 		/* 读取目录项，索引会自动下移 */
			if (result != FR_OK || g_FileInf.fname[0] == 0)
			{
				break;
			}
			if (g_FileInf.fname[0] == '.')	/* 表示目录 */
			{
				continue;
			}
			if (g_FileInf.fattrib != AM_DIR)
			{
				uint8_t Len; 
				Len = strlen(g_FileInf.fname); 			
				//比较传进来的文件名和扫描的文件名是否相同，相同则break，否则继续
				if (memcmp(&g_FileInf.fname, fileName, 3) == 0)
				{
					fFound = 1;
					break;
				}
			
			}
		}//end of for


		if (fFound == 1)
		{
			/* 打开根目录下的 test.mp3 文件 */
			g_Result = f_open(&g_File, g_FileInf.fname, FA_OPEN_EXISTING | FA_READ);

			if (g_Result !=  FR_OK)
			{
				return -1;//打开文件失败，直接跳出
			}
		}
		else
		{
		  return -1;//表示没找到mp3报时文件
		}
	}

	//打开文件成功，开始播放MP3
	g_tMP3.ucPauseEn = 0;	/* 缺省开始播放 */
	g_tMP3.uiProgress = 0;	/* 进度 */
	//bsp_StartTimer(1, 100);	//用定时器1播放MP3
	while (1)
	{
		CPU_IDLE();

		if (g_tMP3.ucPauseEn == 0)
		{
		   Mp3Pro();  //开始播放MP3		
		}
		else 
		  break;
	}

	/* 关闭文件*/
	f_close(&g_File);

	/* 卸载文件系统 */
	f_mount(0, NULL);

}


/*
*********************************************************************************************************
*	函 数 名: Mp3Pro
*	功能说明: MP3文件播放，在主程序while循环中调用. 每次向VS100B发送32字节。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Mp3Pro(void)
{
	uint32_t bw,i;
	char buf[32];

	/* 如果VS1003空闲，则写入新的数据 */
	if (vs1003_ReqNewData())
	{
		f_read(&g_File, &buf, 32, &bw);
		if (bw <= 0)
		{
			/* 文件读取完毕 */
			g_tMP3.ucPauseEn = 1;
			return;
		}

		/* 计算进度 */
		g_tMP3.uiProgress += bw;

		vs1003_PreWriteData();	/* 写数据准备，设置好片选 */
		for (i = 0; i < bw; i++)
		{
			vs1003_WriteData(buf[i]);
		}
	}	
}


/*
  读取配置文件信息，每行一个记录
  记录的格式为
  序号:数据
  样本数据
  00://待定
  01:231645  表示1号电机需要校准为23点16分45秒
  02:091216  表示2号电机需要校准为09点12分16秒
  03:类似
  04:类似
  05:整点报时区间 07-23
  06:是否报时      0不报时  1报时
  07:照明时间区间07-23
  08:是否打开照明  0不照明  1照明
  09://待定

  配置文件必须是config.txt
*/
int readConfigFile(char* fileName)
{
	FRESULT result;
	DIR DirInf;
	uint8_t fFound = 0;
	uint32_t bw;


 	/* 挂载文件系统 */
	g_Result = f_mount(0, &g_fs);	/* Mount a logical drive */
	if (g_Result != FR_OK)
	{
		return	-1;//表示读卡失败
	}
	else
	{

		/* 打开文件 */
		result = f_open(&g_File, fileName, FA_OPEN_EXISTING | FA_READ);
		if (result !=  FR_OK)
		{
			return;		
		}

		/* 读取文件 */
		//result = f_read(&g_File, &config_txt, sizeof(config_txt) - 1, &bw);

		//===================
		  result=f_read(&g_File, &config_txt, 100, &bw);
		  if(result!=0)return -1;//表示读取错误，返回即可
		  if(bw==0)return 0;//文件已经读完
		  //displayTextToScreen(100,120,config_txt,20);
			//chuliConfigFile(config_txt);
			chuli_conf(config_txt);
		//===================

	   /*
		if (bw > 0)
		{
			config_txt[bw] = 0;
			displayTextToScreen(100,120,config_txt,20); //屏幕显示sd卡中的配置文件信息
			//分析字符串
			
			return 1;
		}
		 */
	}
   	/* 关闭文件*/
	f_close(&g_File);

	/* 卸载文件系统 */
	f_mount(0, NULL);

}

void chuli_conf(char* str)
{
	
	uint32_t bw;
	int num1=0;		//一个功能号及字符串序号
	int num=0;		//字符序号
	char  temp[10];
	int fnum=0;
	int j;
	char buf[10];

	while(1){	

			 if(config_txt[num]==':'){			 
			 	funs[fnum].funNum=((config_txt[num-2])-'0')*10+(config_txt[num-1]-'0');//放置功能号
				
				num++;
				continue;
			 }
			 if(config_txt[num]==';'){
					if(fnum>0 && fnum<5)
					{
						//四个电机时间，
						dianjiTime[fnum].hour=((config_txt[num-6])-'0')*10+(config_txt[num-5]-'0');
						dianjiTime[fnum].fen=((config_txt[num-4])-'0')*10+(config_txt[num-3]-'0');
						dianjiTime[fnum].miao=((config_txt[num-2])-'0')*10+(config_txt[num-1]-'0');
					
					}
					if(fnum==5){
					//电机报时开始与结束时间
						dianjiOthers.baoshi_Start=((config_txt[num-5])-'0')*10+(config_txt[num-4]-'0');
						dianjiOthers.baoshi_End=((config_txt[num-2])-'0')*10+(config_txt[num-1]-'0');
					}
					if(fnum==6)
					//电机是否报时
					dianjiOthers.isBaoshi=config_txt[num-1]-'0';
					if(fnum==7){
					//电机报时开始与结束时间
						dianjiOthers.zhaomiang_Start=((config_txt[num-5])-'0')*10+(config_txt[num-4]-'0');
						dianjiOthers.zhaoming_End=((config_txt[num-2])-'0')*10+(config_txt[num-1]-'0');
					}
					if(fnum==6)
					//电机是否报时
					dianjiOthers.isZhaoMing=config_txt[num-1]-'0';
					

				 num++;	
				 fnum++;
	
			 }

				 num++;	
				 num1++;
				 if(num==bw)break;	
				 if(config_txt[num]=='\0')break;
				 if(config_txt[num]==' '||config_txt[num]=='\r'||config_txt[num]=='\n'){
							num++;
							continue;
					}			 
	  }


}




