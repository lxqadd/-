#ifndef motor_H
#define motor_H
#include "stm32f10x.h"

 #define SHUN_SHI_ZHEN 1	 							//电机顺时针方向转动
#define NI_SHI_ZHEN   0 						 	//电机逆时针方向转动
#define NORMAL_STEP_INTERVAL 1000			//正常转动时候的延时为1ms
#define JIAOZHUN_STEP_INTERVAL 150  	//校准赶时间时候的延时为0.15ms
#define ONE_CIRCLE_STEPS	1600				//电机转动一圈的脉冲数

//电机定义
#define DJ1		1  //电机1
#define DJ2		2  //电机2
#define DJ3		3  //电机3
#define DJ4		4  //电机4
//电机配置参数结构体
typedef struct{
	int djnum;	//记录电机编号
	int djstep;	//记录电机步数
	int djChaTime;	//记录电机时间差

}Moter;

//=====================初始化及管脚设置==============================
void outPut(int moter,int value);						//控制PUL脉冲  管脚为PE0
void SetMonorDirection(int monitor,int value);//控制DIR方向  管脚为PB9
void delay_us(u32 nus);						//微秒延时函数

//======================电机运转======================================
void goOneCircle(int djnum);					//让电动机转1圈
void goNCircle(int n);			//让电机转n圈，每圈花费480ms
void goNSecond(int second,int num);	//让电机追赶指定秒数
void goNStep(int n,int m);			//让电机走N步，m表示是哪个电机


#endif
