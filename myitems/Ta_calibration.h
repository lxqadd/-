#ifndef __CALIBRATION_H
#define __CALIBRATION_H
//===========================结构体===================================

//定义结构体，记录GPS时间
typedef struct gpstime
{
	u8 gpshours;			//GPS时间：时
	u8 gpsmins;				//GPS时间：分
	u8 gpsseconds;		//GPS时间：秒
	
}GPSTime ;




//======================校准时间======================================
void record_GPSTime(void);	//记录GPS时间
void record_NowTiem(void);	//记录当前时间的函数,初始值为0
void calibration_Time(int cal); //根据GPS校准时间
void nowToGPS(void);				//将当前时间记为GPS时间
void keyCalDianJi(int keyCde);				//根据按键值校准时间

#endif
