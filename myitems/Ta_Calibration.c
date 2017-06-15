#include "stm32f10x.h"
#include "Ta_gps.h"		   //GPSģ��
#include "Ta_calibration.h"	//��õ�ǰʱ���Լ�У׼ʱ��
#include "ta_eeprom.h"
#include "Ta_Mp3_playe.h"		/* MP3������ģ��*/
#include "Ta_KeyScan.h"
#include "bsp_tft_lcd.h"	/* TFTҺ����ʾ������ģ�� */

extern nmea_msg gpsx; 	//GPS��Ϣ
extern isStop;			//����Ƿ���
extern	DianjiConfig dianjiTime[4];		//����ĸ������ʱ��
extern int MODE_1; //��ֵΪ0ʱ�ǹ���ģʽ������Ĭ��ģʽ
extern int MODE_2;	//��ֵΪ1ʱ������У׼ģʽ
extern int keyCode;					//��������
extern int keyMode; 		//����ģʽ 


int totalSeconds=0;		//���̴�0���߹�������
int gpsSeconds=0;
int time[8];	//����һ���������飬�����洢ʱ�䡣�磺01083000�ǵ��1��ʱ��

//=============================================================================
//��¼GPSʱ��
void record_GPSTime(void)
{
    gpsSeconds=gpsx.utc.sec+gpsx.utc.min*60+(gpsx.utc.hour+8)*3600;
}

//��¼��ǰʱ��ĺ���,��ʼֵΪ 12:00
void record_NowTiem(void)
{
  	totalSeconds+=15;		//ʱ�侭����15�룬��Ϊ���ת���ж�15��һ��
		totalSeconds%=86400;	//ÿ��86400(=24*3600)�룬ʱ�侭��24Сʱ�����¼���  
 }



//У׼ʱ�䣬����GPSʱ�����У׼���೤ʱ��У׼һ�ο����Լ���
void calibration_Time(int cal)
{
	int timeCha;	//ʱ���ڵ������ʱ��У׼�ã��ر�������
	int timeChaDJ1,timeChaDJ2,timeChaDJ3,timeChaDJ4;//�ĸ�����������ļ��е�ʱ����Ҫ���ڳ�ʼ�����߻ָ�����ʱ
	
	char buff[10];
	int j,k;	  
  record_GPSTime();//����GPSʱ��  
	if(cal==0)
	{
		//Ĭ������µ�ʱ��	
		timeCha=totalSeconds-gpsSeconds;//��ȡʱ���
		
		//ʱ����ˣ�ֱ����ʱ�ȴ�����������
		if(timeCha>0){
	  //ֹͣ�綯��
	  isStop=1;
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
	  //�����綯��
	  isStop=0;
		}
		else{  //ʱ����
			//��ʱ�䣬�����ǰת
			goNSecond(-timeCha,0);
			//������¼��ʱ��
	    record_GPSTime();//����GPSʱ��
	    totalSeconds=gpsSeconds;
		}
	}

	if(cal==1)
	{

		//����ʱ��Ͱ���GPSУ׼
		timeCha=dianjiTime[1].hour*3600+dianjiTime[1].fen*60+dianjiTime[1].miao-gpsSeconds;
		sprintf(buff,"ʱ���:%d",timeCha);
		displayTextToScreen(100,100,buff,20);
		if(timeCha>0){
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
		}
		else{  //ʱ����
			//��ʱ�䣬�����ǰת
			//ÿ��15�룬�����Ҫ�ܵ�Ӱ�죬����0.48��תһȦ������3.2��תһȦ������2.72��
			timeCha=(-timeCha)+(-timeCha)/15*2.72;//Ҫ��ʱ���ӳ����ֲ���ʱ
			goNSecond(timeCha,1);
		}	
	}
	if(cal==2)
	{
		//���2��ʱ��
		timeCha=dianjiTime[2].hour*3600+dianjiTime[2].fen*60+dianjiTime[2].miao-gpsSeconds;
		sprintf(buff,"ʱ���:%d",timeCha);
		displayTextToScreen(100,100,buff,20);
		if(timeCha>0){
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
		}
		else{  //ʱ����
			//��ʱ�䣬�����ǰת
			timeCha=(-timeCha)+(-timeCha)/15*2.72;//Ҫ��ʱ���ӳ����ֲ���ʱ
			goNSecond(timeCha,2);
		}
	}
	if(cal==3)
	{
		//���3��ʱ���	

		timeCha=dianjiTime[3].hour*3600+dianjiTime[3].fen+dianjiTime[3].miao-gpsSeconds;
		sprintf(buff,"ʱ���:%d",timeCha);
		displayTextToScreen(100,100,buff,20);
		if(timeCha>0){
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
		}
		else{  //ʱ����
			//��ʱ�䣬�����ǰת
			timeCha=(-timeCha)+(-timeCha)/15*2.72;//Ҫ��ʱ���ӳ����ֲ���ʱ
			goNSecond(timeCha,3);
		}
	}
	if(cal==4)
	{
		//���4��ʱ���

		timeCha=dianjiTime[4].hour*3600+dianjiTime[4].fen+dianjiTime[4].miao-gpsSeconds;
		sprintf(buff,"ʱ���:%d",timeCha);
		displayTextToScreen(100,100,buff,20);
		if(timeCha>0){
	  for(j=0;j<timeCha;j++){
	    for(k=0;k<1000;k++)	mydelay_us(1000);
	  }
		}
		else{  //ʱ����
			//��ʱ�䣬�����ǰת
			timeCha=(-timeCha)+(-timeCha)/15*2.72;//Ҫ��ʱ���ӳ����ֲ���ʱ
			goNSecond(timeCha,4);
		}
	}
	//���ĸ�����ֱ�ϵ�0�㣬����ͬʱ
	if(cal==5)
	{
		if(totalSeconds==0)
		{
			//��ʼ�����߻ָ�����ʱ���ã�һ���԰��ĸ����ͬʱ�ϵ�0��
			//ֹͣ�綯��
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
		//�����GPSʱ�䵽��0�㣬���������ϵͳ����
		if(gpsSeconds==0)
		{
			//�����綯��
			isStop=0;		
		}
	}
	
}


//���ݰ���ֵУ׼���
void keyCalDianJi(int keyCde)
{
	int calseconds;	//��ҪУ׼������
	static int i=-1;
	char buf[10];
	char bugg[10];
	int num=0;

	//K41�����밴�����������밴��֮�󣬽����鸳ֵ	
	if(keyCode==K41)
	{
		
		displayTextToScreen_nad(50,120,"����:",20);			
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




















	//����43��ȷ�ϰ��������ַ�����������ҪУ׼������
	if(keyCode==K43)
	{
		//���㵱ǰ�����GPS��������
		calseconds=(time[2]*10+time[3])*3600+(time[4]*10+time[5])*60+time[6]*10+time[7]-gpsSeconds;

		
		if(time[0]*10+time[1]==1)
		{
			displayTextToScreen_nad(240,120,"ȷ��",20);	//ȷ�ϲ��˳�
			goNSecond(calseconds,1);
			keyMode=0;//ģʽ�л�ΪĬ��
			initAD();//�������
			i=0;
			isStop=0;//��ʾ�������
			return;
		}
		if(time[0]*10+time[1]==2)
		{
			displayTextToScreen_nad(240,120,"ȷ��",20);
			goNSecond(calseconds,2);
			keyMode=0;//ģʽ�л�ΪĬ��
			initAD();//�������
			i=-1;
			isStop=0;//��ʾ�������
			return;
		}
		if(time[0]*10+time[1]==3)
		{
			displayTextToScreen_nad(240,120,"ȷ��",20);
			goNSecond(calseconds,3);
			keyMode=0;//ģʽ�л�ΪĬ��
			initAD();//�������
			i=-1;
			isStop=0;//��ʾ�������
			return;
		}
		if(time[0]*10+time[1]==4)
		{
			displayTextToScreen_nad(240,120,"ȷ��",20);
			goNSecond(calseconds,4);
			keyMode=0;//ģʽ�л�ΪĬ��
			initAD();//�������
			i=-1;
			isStop=0;//��ʾ�������
			return;
		}
		else
		{
			displayTextToScreen_nad(50,140,"����������л�ģʽ��������",20);		
			keyMode=0;//ģʽ�л�ΪĬ��
			i=-1;
			return;
		}
	}
	if(keyCode==K43)
	{
		keyMode=0;//ģʽ�л�ΪĬ��
		i=-1;
		return;
	}
	
}

