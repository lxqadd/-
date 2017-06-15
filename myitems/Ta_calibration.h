#ifndef __CALIBRATION_H
#define __CALIBRATION_H
//===========================�ṹ��===================================

//����ṹ�壬��¼GPSʱ��
typedef struct gpstime
{
	u8 gpshours;			//GPSʱ�䣺ʱ
	u8 gpsmins;				//GPSʱ�䣺��
	u8 gpsseconds;		//GPSʱ�䣺��
	
}GPSTime ;




//======================У׼ʱ��======================================
void record_GPSTime(void);	//��¼GPSʱ��
void record_NowTiem(void);	//��¼��ǰʱ��ĺ���,��ʼֵΪ0
void calibration_Time(int cal); //����GPSУ׼ʱ��
void nowToGPS(void);				//����ǰʱ���ΪGPSʱ��
void keyCalDianJi(int keyCde);				//���ݰ���ֵУ׼ʱ��

#endif
