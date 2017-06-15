#ifndef TA__KEYSCAN_H 
#define TA__KEYSCAN_H 
#include "stm32f10x.h" 


//��1��
#define  K11 11	//�������
#define  K12 12	//ֹͣ���
#define  K13 13	//���Ա�ʱ
#define  K14 14	//��������

//��2��
#define  K21 21	 //У׼ʱ��
#define  K22 22	 //��ʾGPSʱ��
#define  K23 23	 //
#define  K24 24

//��3��
#define  K31 31
#define  K32 32
#define  K33 33
#define  K34 34

//��4��
#define  K41 41
#define  K42 42
#define  K43 43
#define  K44 44









//����һ���˿ڲ����Ľṹ��
struct io_port {                                            
	GPIO_TypeDef *GPIO_x;  		//�˿ڵ�ָ��                
	unsigned short GPIO_pin;	//�ܽ����
};  

//����һ������ṹ���������       
static struct io_port key_output[4] = {
    //�мǣ���Ҫ����ʵ����Ҫ�޸ģ�����
	{GPIOA, GPIO_Pin_12}, 
	{GPIOA, GPIO_Pin_10},
	{GPIOA, GPIO_Pin_8}, 
	{GPIOC, GPIO_Pin_8}
};

//����һ������ṹ���������
static struct io_port key_input[4] = {
    //�мǣ���Ҫ����ʵ����Ҫ�޸ģ�����
	{GPIOC, GPIO_Pin_6},
	{GPIOG, GPIO_Pin_7},
	{GPIOG, GPIO_Pin_5}, 
	{GPIOG, GPIO_Pin_3}
};

//���16������һ�����飬Ϊ1��ʾ�ü������£�����key[1][1]==1,�ͱ�ʾ�ڶ��еĵڶ��б�����



/* 4*4������� */ 
void keyboard_init(void); 
int keyscan(void);


#endif  
