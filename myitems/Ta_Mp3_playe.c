#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_timer.h"
#include "bsp_button.h"
#include "bsp_tft_lcd.h"
#include "bsp_VS1003B.h"
#include "bsp_sdio_sd.h"
#include "ff.h"
#include "Ta_Mp3_playe.h"

#define LINE_CAP	18		/* �����м�� */
#define MAX_FILE_COUNT 64   //��������෽���ļ�����



MP3_T g_tMP3;

/* ����Fatfs�õ���ȫ�ֱ��� */
FRESULT g_Result;
FATFS   g_fs;
DIR     g_DirInf;
FILINFO g_FileInf;
FIL 	g_File;





char* fileName[]={"00����.MP3","01����.MP3","02����.MP3","03����.MP3",
"04����.MP3","05����.MP3","06����.MP3","07����.MP3","08����.MP3","09����.MP3",
"10����.MP3","11����.MP3","12����.MP3","13����.MP3","14����.MP3","15����.MP3",
"16����.MP3","17����.MP3","18����.MP3","19����.MP3","20����.MP3","21����.MP3",
"22����.MP3","23����.MP3"};

char config_txt[100];	//���SD��config.txt�ļ�����
Fun funs[10];			//ʮ�����ܽṹ��
DianjiConfig dianjiTime[4];		//����ĸ������ʱ��
DianjiConfig dianjiOthers;		//��ŵ���������ã��籨ʱ������
u8  buf;

/*
*********************************************************************************************************
*	�� �� ��: Mp3Player
*	����˵��: MP3������������
*	��    �Σ���
*	�� �� ֵ: ��
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

	g_tMP3.ucVolume = 10; 			/* ȱʡ����,Խ������ԽС */
	vs1003_SetVolume(g_tMP3.ucVolume);

 	/* �����ļ�ϵͳ */
	g_Result = f_mount(0, &g_fs);	/* Mount a logical drive */
	
	if (g_Result != FR_OK)
	{
		return	-1;//��ʾ����ʧ��
	}
	else
	{
		/* �򿪸��ļ��� */
		result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
		if (result != FR_OK)
		{
		  return -1;//��ʾ��ȡ�ļ���ʧ��
		}
		fFound = 0;//�Ƿ��ҵ��ļ�
		for(i=0;i<MAX_FILE_COUNT;i++)
		{
			result = f_readdir(&DirInf,&g_FileInf); 		/* ��ȡĿ¼��������Զ����� */
			if (result != FR_OK || g_FileInf.fname[0] == 0)
			{
				break;
			}
			if (g_FileInf.fname[0] == '.')	/* ��ʾĿ¼ */
			{
				continue;
			}
			if (g_FileInf.fattrib != AM_DIR)
			{
				uint8_t Len; 
				Len = strlen(g_FileInf.fname); 			
				//�Ƚϴ��������ļ�����ɨ����ļ����Ƿ���ͬ����ͬ��break���������
				if (memcmp(&g_FileInf.fname, fileName, 3) == 0)
				{
					fFound = 1;
					break;
				}
			
			}
		}//end of for


		if (fFound == 1)
		{
			/* �򿪸�Ŀ¼�µ� test.mp3 �ļ� */
			g_Result = f_open(&g_File, g_FileInf.fname, FA_OPEN_EXISTING | FA_READ);

			if (g_Result !=  FR_OK)
			{
				return -1;//���ļ�ʧ�ܣ�ֱ������
			}
		}
		else
		{
		  return -1;//��ʾû�ҵ�mp3��ʱ�ļ�
		}
	}

	//���ļ��ɹ�����ʼ����MP3
	g_tMP3.ucPauseEn = 0;	/* ȱʡ��ʼ���� */
	g_tMP3.uiProgress = 0;	/* ���� */
	//bsp_StartTimer(1, 100);	//�ö�ʱ��1����MP3
	while (1)
	{
		CPU_IDLE();

		if (g_tMP3.ucPauseEn == 0)
		{
		   Mp3Pro();  //��ʼ����MP3		
		}
		else 
		  break;
	}

	/* �ر��ļ�*/
	f_close(&g_File);

	/* ж���ļ�ϵͳ */
	f_mount(0, NULL);

}


/*
*********************************************************************************************************
*	�� �� ��: Mp3Pro
*	����˵��: MP3�ļ����ţ���������whileѭ���е���. ÿ����VS100B����32�ֽڡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Mp3Pro(void)
{
	uint32_t bw,i;
	char buf[32];

	/* ���VS1003���У���д���µ����� */
	if (vs1003_ReqNewData())
	{
		f_read(&g_File, &buf, 32, &bw);
		if (bw <= 0)
		{
			/* �ļ���ȡ��� */
			g_tMP3.ucPauseEn = 1;
			return;
		}

		/* ������� */
		g_tMP3.uiProgress += bw;

		vs1003_PreWriteData();	/* д����׼�������ú�Ƭѡ */
		for (i = 0; i < bw; i++)
		{
			vs1003_WriteData(buf[i]);
		}
	}	
}


/*
  ��ȡ�����ļ���Ϣ��ÿ��һ����¼
  ��¼�ĸ�ʽΪ
  ���:����
  ��������
  00://����
  01:231645  ��ʾ1�ŵ����ҪУ׼Ϊ23��16��45��
  02:091216  ��ʾ2�ŵ����ҪУ׼Ϊ09��12��16��
  03:����
  04:����
  05:���㱨ʱ���� 07-23
  06:�Ƿ�ʱ      0����ʱ  1��ʱ
  07:����ʱ������07-23
  08:�Ƿ������  0������  1����
  09://����

  �����ļ�������config.txt
*/
int readConfigFile(char* fileName)
{
	FRESULT result;
	DIR DirInf;
	uint8_t fFound = 0;
	uint32_t bw;


 	/* �����ļ�ϵͳ */
	g_Result = f_mount(0, &g_fs);	/* Mount a logical drive */
	if (g_Result != FR_OK)
	{
		return	-1;//��ʾ����ʧ��
	}
	else
	{

		/* ���ļ� */
		result = f_open(&g_File, fileName, FA_OPEN_EXISTING | FA_READ);
		if (result !=  FR_OK)
		{
			return;		
		}

		/* ��ȡ�ļ� */
		//result = f_read(&g_File, &config_txt, sizeof(config_txt) - 1, &bw);

		//===================
		  result=f_read(&g_File, &config_txt, 100, &bw);
		  if(result!=0)return -1;//��ʾ��ȡ���󣬷��ؼ���
		  if(bw==0)return 0;//�ļ��Ѿ�����
		  //displayTextToScreen(100,120,config_txt,20);
			//chuliConfigFile(config_txt);
			chuli_conf(config_txt);
		//===================

	   /*
		if (bw > 0)
		{
			config_txt[bw] = 0;
			displayTextToScreen(100,120,config_txt,20); //��Ļ��ʾsd���е������ļ���Ϣ
			//�����ַ���
			
			return 1;
		}
		 */
	}
   	/* �ر��ļ�*/
	f_close(&g_File);

	/* ж���ļ�ϵͳ */
	f_mount(0, NULL);

}

void chuli_conf(char* str)
{
	
	uint32_t bw;
	int num1=0;		//һ�����ܺż��ַ������
	int num=0;		//�ַ����
	char  temp[10];
	int fnum=0;
	int j;
	char buf[10];

	while(1){	

			 if(config_txt[num]==':'){			 
			 	funs[fnum].funNum=((config_txt[num-2])-'0')*10+(config_txt[num-1]-'0');//���ù��ܺ�
				
				num++;
				continue;
			 }
			 if(config_txt[num]==';'){
					if(fnum>0 && fnum<5)
					{
						//�ĸ����ʱ�䣬
						dianjiTime[fnum].hour=((config_txt[num-6])-'0')*10+(config_txt[num-5]-'0');
						dianjiTime[fnum].fen=((config_txt[num-4])-'0')*10+(config_txt[num-3]-'0');
						dianjiTime[fnum].miao=((config_txt[num-2])-'0')*10+(config_txt[num-1]-'0');
					
					}
					if(fnum==5){
					//�����ʱ��ʼ�����ʱ��
						dianjiOthers.baoshi_Start=((config_txt[num-5])-'0')*10+(config_txt[num-4]-'0');
						dianjiOthers.baoshi_End=((config_txt[num-2])-'0')*10+(config_txt[num-1]-'0');
					}
					if(fnum==6)
					//����Ƿ�ʱ
					dianjiOthers.isBaoshi=config_txt[num-1]-'0';
					if(fnum==7){
					//�����ʱ��ʼ�����ʱ��
						dianjiOthers.zhaomiang_Start=((config_txt[num-5])-'0')*10+(config_txt[num-4]-'0');
						dianjiOthers.zhaoming_End=((config_txt[num-2])-'0')*10+(config_txt[num-1]-'0');
					}
					if(fnum==6)
					//����Ƿ�ʱ
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




