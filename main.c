#include "stm32f10x.h"			/* ���Ҫ��ST�Ĺ̼��⣬�����������ļ� */
#include <stdio.h>					/* ��Ϊ�õ���printf���������Ա����������ļ� */
#include "bsp_usart.h"			/* printf����������������ڣ����Ա����������ļ� */
#include "bsp_tft_lcd.h"		/* TFTҺ����ʾ������ģ�� */
#include "uart_api.h"	   		//���ڸ��ֹ���ʵ��
#include "Ta_Motor.h"
#include "Ta_KeyScan.h"
#include "Ta_Display.h"
#include "Ta_gps.h"		   		//GPSģ��

#include "bsp_sdio_sd.h"  	/* SD������ģ�� */
#include "ff.h"							/* FatFS�ļ�ϵͳģ��*/	
#include "Ta_Mp3_playe.h"		/* MP3������ģ��*/
#include "bsp_24xx02.h"			/*EEPROMоƬ����*/

//����ģʽ�л�
int MODE_1=0; //��ֵΪ0ʱ�ǹ���ģʽ������Ĭ��ģʽ
int MODE_2=1;	//��ֵΪ1ʱ������У׼ģʽ


extern nmea_msg gpsx; 			//GPS��Ϣ
extern __align(4) u8 dtbuf[50]; 		//������
extern g_TxBuf2[UART2_TX_BUF_SIZE];	//����2�ķ��ͻ�����
extern g_RxBuf2[UART2_RX_BUF_SIZE];	//����2�Ľ��ջ����� 
extern char *config_txt;  	//��ȡSD�������ı���Ϣ

extern int isStop;					//1��ʾֹͣ���ת����0��ʾ�������
extern int keyCode;					//��������
extern char *fileName[];		//���㱨ʱ�ļ�������
extern uint8_t buff_ReadFromEEPROM[DATA_LEN];
extern int sucess;					//��ȡEEPROM�Ƿ�ɹ�
extern totalSeconds;
extern Fun funs[10];				//ʮ�����ܽṹ��
extern	DianjiConfig dianjiTime[4];		//����ĸ������ʱ��
extern DianjiConfig dianjiOthers;		//��ŵ���������ã��籨ʱ������



static void InitBoard(void); 	//Ӳ����ʼ��
void process(void);						//��ѭ������
void doKeyEvents(int keyCde);	//������Ӧ�¼�
void initAD(void);			 			//������Ĺ��
int keyMode; 		//����ģʽ 


/*
*���������    
*/
int main(void)
{
		int i=0;
		int time=0;
   //Ӳ����ʼ��
   	InitBoard(); 	//��ʼ�������壬���ø��ֹ���
   
   //������ĳ�ֵ
   //initValues();   

    initAD();//�������
		
		//��ȡEEPROM��ֵ
		ee_Read();
		
		//����������
		process();
		
		

}















//��ѭ������
void process(void){   
	//��������Ĭ��ģʽ
	keyMode=MODE_1;
	
	SetMonorDirection(DJ1,SHUN_SHI_ZHEN);//���õ��1˳ʱ�뷽��
	SetMonorDirection(DJ2,SHUN_SHI_ZHEN);//���õ��2˳ʱ�뷽��
	SetMonorDirection(DJ3,SHUN_SHI_ZHEN);//���õ��1˳ʱ�뷽��
	SetMonorDirection(DJ4,SHUN_SHI_ZHEN);//���õ��2˳ʱ�뷽��

	isStop=1; 		//1��ʾֹͣ���ת������ʱ��ʱ��������	
								 	
	Time2Init(15);//���������ʱ�������õ��15����һȦ���������4С��32

	Time4Init(20);////����������̼�ⶨʱ������λΪms	

	//��ѭ��
	while(1){
			setClosed(1);			//�رռ̵���	
			setClosed(2);			//�رռ̵���
			if(keyMode==0)
			{
				doKeyEvents(keyCode);//���ü��̴�����
			}			
			if(keyMode==1)
			{
			
				keyCalDianJi(keyCode);	//��������У׼���
			}
	}
}

//���̴�����
void doKeyEvents(int keyCde){
  static int oldKeyCode=-1;
  static int resetTimes=0;//��¼��λ����������10��ִ�и�λ
  int j=0;
  char buf[10];
	char buff[10];
  if(keyCode==oldKeyCode)return ; 	
 
 switch(keyCode){
    case K11: //�������
			isStop=0;//��ʾ�������
			LCD_ClrScr(CL_BLUE);  	
	  	displayTextToScreen(100,120,"�������[ K11 ]...",20);	
	  break;
	 case K12: //ֹͣ���
	  	isStop=1;//��ʾֹͣ���
			LCD_ClrScr(CL_BLUE);  	
			if(keyCode!=oldKeyCode)displayTextToScreen(100,120,"ֹͣ���[ K12 ]...",20);
		break;		
	 case K13: //������ʱ����	
			//�򿪼̵���
			setOpen();
			LCD_ClrScr(CL_BLUE); 
			if(keyCode!=oldKeyCode)displayTextToScreen(100,120,"������ʱ[ K13 ]...",20);
			Mp3PlayerAFile(fileName[1]);//��ʱ8����	
	 	break;

	case K14: //��ȡSD��������Ϣ	
		LCD_ClrScr(CL_BLUE); 
		readConfigFile("config.txt");
		sprintf(buf,"���ܺ�λ:%d",funs[8].funNum);
		sprintf(buff,"���ʱ��:%d",dianjiTime[1].hour*3600+dianjiTime[1].fen*60+dianjiTime[1].miao);
		//sprintf(buff,"�Ƿ�����:%d",dianjiOthers.isZhaoMing);
		displayTextToScreen(100,120,buf,20); 
		displayTextToScreen(100,140,buff,20); 		
		break; 
		
	case K21: //У׼ʱ��	
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"У׼ʱ��[ K21 ]...",20);		
		calibration_Time(0);		    
		break;
 
	case K22: //��ʾGPS״̬	  	
		GPS_Analysis(&gpsx,(u8*)g_RxBuf2);//�����ַ���
		LCD_ClrScr(CL_BLUE);  	
		sprintf((char *)dtbuf,"%ld",(u8*)g_RxBuf2); //��Ҫ������ַ���������g_RxBuf2
		Gps_Msg_Show(); //GPS��Ϣ��ʾ				 
	  break;
	case K23: //���Գ�ʼ��ʱ�����ת ����ʼУ׼��
		LCD_ClrScr(CL_BLUE);  	
		displayTextToScreen(100,120,"��ʼУ׼",20);	
		calibration_Time(5);
	  break;
  
	case K24://TODO�ָ�����ģʽ����EEPROM�б���0  
	   	 LCD_ClrScr(CL_BLUE);
			if(resetTimes<9){		 
		   displayTextToScreen(100,120,"�ָ��������������ٰ���λ����",20);
		   resetTimes++;//��¼����
		   break;
		  }
		
		 ee_Erase();  //����EEPROM
		 delay_us(100);
	   ee_Write(0); //��ʼֵ0�룬д��EEPROM
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
		 displayTextToScreen(100,120,"��λ�ɹ�!",20);
		 resetTimes=0; 
	     break;
	case K31: //У׼���1��ʱ��
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"У׼���1ʱ��[ K31 ]...",20);	
		readConfigFile("config.txt");
		calibration_Time(1);		    
		break;
	case K32: //У׼���2��ʱ��
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"У׼���2ʱ��[ K32 ]...",20);	
		readConfigFile("config.txt");		
		calibration_Time(2);		    
		break;	
	case K33: //У׼��3��ʱ��
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"У׼���3ʱ��[ K33 ]...",20);		
		readConfigFile("config.txt");
		calibration_Time(3);		    
		break;	
	case K34: //У׼���4��ʱ��
		LCD_ClrScr(CL_BLUE); 
		displayTextToScreen(100,120,"У׼���4ʱ��[ K34 ]...",20);		
		readConfigFile("config.txt");
		calibration_Time(4);		    
		break;
	case K44:
		LCD_ClrScr(CL_BLUE);
		keyMode=1;
		displayTextToScreen(100,100,"ģʽ�л�...",20);
	break;

	
	default:	    
	    break;
  }
    
	oldKeyCode=keyCode;//��¼�ϴΰ���
  
}




/*��ʼ��������
*��ʼ�� ��Ļ
*��ʼ�� LED��
*��ʼ�� ����
*��ʼ�� ����
*��ʼ�� ��ʱ��
*��ʼ�� �綯��
*��ʼ�� 4*4����
*/
static void InitBoard(void)
{	
	/* ��ʼ��systick��ʱ������������ʱ�ж� */

	bsp_InitButton();	//û������������ܿ����������ˡ���ʱ��Ϊɶ���أ�������
  
 	bsp_InitTimer(); 	//�����ڰ�����ʼ��֮��
	

	LCD_InitHard();	/* ��ʼ����ʾ��Ӳ��(����GPIO��FSMC,��LCD���ͳ�ʼ��ָ��) */	
	LCD_SetBackLight(BRIGHT_DEFAULT);   
  	LCD_ClrScr(CL_BLUE);  	/* ������������ɫ */


	keyboard_init(); 		//������ʼ��

  	initjdq();				//��ʼ���̵���

 	initMotor();			//��ʼ���������
	
	comInit();			    //GPS����COM3��ʼ��

	//�������ת����ʱ��
	TIM2_NVIC_Configuration();

	//����������ⶨʱ��
	TIM4_NVIC_Configuration();
	


}

//���
void initAD(void){
   int x=100;
   int y=120;
   	FONT_T tFont;		/* ����һ������ṹ���������������������� */

	/* ������������ */
	tFont.usFontCode = FC_ST_16X16;	/* ����ѡ������16���󣬸�16x��15) */
	tFont.usTextColor = CL_WHITE;		/* ������ɫ����Ϊ��ɫ */
	tFont.usBackColor = CL_MASK;	 	/* ���ֱ�����ɫ��͸�� */
	tFont.usSpace = 2;					/* �ַ�ˮƽ���, ��λ = ���� */	
    //����
	LCD_ClrScr(CL_BLUE);
	LCD_DispStr(x-40, y-62, "======��ְ̨ҵ����ѧԺ=====", &tFont);
	
	LCD_DispStr(x, y, "�밴������ϵͳ��", &tFont);
	
	//����
	LCD_DispStr(x-60, y+62, "======��Ȩ����2016-2020=====",&tFont);
}
