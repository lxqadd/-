#ifndef TA__KEYSCAN_H 
#define TA__KEYSCAN_H 
#include "stm32f10x.h" 


//第1行
#define  K11 11	//启动电机
#define  K12 12	//停止电机
#define  K13 13	//测试报时
#define  K14 14	//测试照明

//第2行
#define  K21 21	 //校准时间
#define  K22 22	 //显示GPS时间
#define  K23 23	 //
#define  K24 24

//第3行
#define  K31 31
#define  K32 32
#define  K33 33
#define  K34 34

//第4行
#define  K41 41
#define  K42 42
#define  K43 43
#define  K44 44









//声明一个端口参数的结构体
struct io_port {                                            
	GPIO_TypeDef *GPIO_x;  		//端口的指针                
	unsigned short GPIO_pin;	//管脚序号
};  

//创建一个输出结构体数组变量       
static struct io_port key_output[4] = {
    //切记！！要根据实际需要修改！！！
	{GPIOA, GPIO_Pin_12}, 
	{GPIOA, GPIO_Pin_10},
	{GPIOA, GPIO_Pin_8}, 
	{GPIOC, GPIO_Pin_8}
};

//定义一个输入结构体数组变量
static struct io_port key_input[4] = {
    //切记！！要根据实际需要修改！！！
	{GPIOC, GPIO_Pin_6},
	{GPIOG, GPIO_Pin_7},
	{GPIOG, GPIO_Pin_5}, 
	{GPIOG, GPIO_Pin_3}
};

//存放16个键的一个数组，为1表示该键被按下，比如key[1][1]==1,就表示第二行的第二列被摁下



/* 4*4矩阵键盘 */ 
void keyboard_init(void); 
int keyscan(void);


#endif  
