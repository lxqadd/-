#ifndef motor_H
#define motor_H
#include "stm32f10x.h"

 #define SHUN_SHI_ZHEN 1	 							//���˳ʱ�뷽��ת��
#define NI_SHI_ZHEN   0 						 	//�����ʱ�뷽��ת��
#define NORMAL_STEP_INTERVAL 1000			//����ת��ʱ�����ʱΪ1ms
#define JIAOZHUN_STEP_INTERVAL 150  	//У׼��ʱ��ʱ�����ʱΪ0.15ms
#define ONE_CIRCLE_STEPS	1600				//���ת��һȦ��������

//�������
#define DJ1		1  //���1
#define DJ2		2  //���2
#define DJ3		3  //���3
#define DJ4		4  //���4
//������ò����ṹ��
typedef struct{
	int djnum;	//��¼������
	int djstep;	//��¼�������
	int djChaTime;	//��¼���ʱ���

}Moter;

//=====================��ʼ�����ܽ�����==============================
void outPut(int moter,int value);						//����PUL����  �ܽ�ΪPE0
void SetMonorDirection(int monitor,int value);//����DIR����  �ܽ�ΪPB9
void delay_us(u32 nus);						//΢����ʱ����

//======================�����ת======================================
void goOneCircle(int djnum);					//�õ綯��ת1Ȧ
void goNCircle(int n);			//�õ��תnȦ��ÿȦ����480ms
void goNSecond(int second,int num);	//�õ��׷��ָ������
void goNStep(int n,int m);			//�õ����N����m��ʾ���ĸ����


#endif
