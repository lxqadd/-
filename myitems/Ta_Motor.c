 #include "stm32f10x.h"
#include <stdio.h>	
#include "bsp_usart.h"
#include "Ta_Motor.h"
#include "Ta_gps.h"		   //GPS模块
#include "bsp_timer.h"		/* systick定时器模块 */
#include "bsp_tft_lcd.h"	/* TFT液晶显示器驱动模块 */
#include "Ta_calibration.h"	/*获得当前时间以及校准时间*/
#include "Ta_Mp3_playe.h"		/* MP3播放器模块*/


extern gpsSeconds;
extern totalSeconds;
extern char *fileName[];			//整点报时文件名数组
extern DianjiConfig dianjiOthers;		//存放电机其他配置，如报时和照明




/*步进电机 基本情况：
 * 电压： 32V
 * 电流： 1安
 *细分：8细分，每个脉冲0.225度
*/

extern int keyCode;	

int isStop=0;							//是否停止电机运转

//==============================以下为用到的=============================
/*
  *电动机走一圈,3s左右,正常运转，电机正常运转时调用
*/
void goOneCircle(int djnum){ 
   int i=0;
   if(isStop)return;//如果isStop=1则电机停走,0正常走动
	 //默认情况，四个电机同时转
	 if(djnum==0)
	 {
		 for(i=0;i<ONE_CIRCLE_STEPS;i++){
				//对应管脚电平切换一下电机走一步
				outPut(1,1);			//输出1	
				outPut(2,1);			//输出1 
				outPut(3,1);
				outPut(4,1);
				delay_us(NORMAL_STEP_INTERVAL);		//延时 1毫秒 （1000微秒）		
				outPut(1,0);
				outPut(2,0);			//输出0
				outPut(3,0);
				outPut(4,0);
				delay_us(NORMAL_STEP_INTERVAL);	  	//延时 1毫秒 （1000微秒）
			}
		}
		
	 if(djnum==1)
	 {
		 for(i=0;i<ONE_CIRCLE_STEPS;i++){
				//对应管脚电平切换一下电机走一步
				outPut(1,1);			//输出1	
				delay_us(NORMAL_STEP_INTERVAL);		//延时 1毫秒 （1000微秒）		
				outPut(1,0);
				delay_us(NORMAL_STEP_INTERVAL);	  	//延时 1毫秒 （1000微秒）
			}
	 }
   //更新总秒数
   record_NowTiem();

   //晚上3点半，自动校准GPS的时间
   if(totalSeconds%12600==0){
      calibration_Time(0);//校准时间
   }   

   //整点自动报时
   if(totalSeconds%3600==0){//整点      
     i=totalSeconds/3600;
	  //夜间停止报时 (23点-6期间)
		//如果配置文件当中的报时值为1，就报时
		if(dianjiOthers.isBaoshi==1)
		{
			//读取配置文件当中的报时区间
			if(i>=dianjiOthers.baoshi_Start&&i<=dianjiOthers.baoshi_End)
			{
				//打开继电器，然后播放报时
				setOpen(1);
				Mp3PlayerAFile(fileName[i-1]);//报时i-1点整
			 //播放完毕后关闭继电器
				setClosed(1);
			 }
			 
		}
		//照明区间
		if(dianjiOthers.isZhaoMing==1)
		{
		if(i>=dianjiOthers.zhaomiang_Start&&i<=dianjiOthers.zhaoming_End)
			{
				//打开继电器，照明
				setOpen(2);
			}
		}
	  //可能要考虑报时引起的延时问题......................
   }

}
/*
  电机走N步的函数,
	两个参数，n表示走多少不，m表示哪个电机走
	当m=0，默认情况，四个电机同时走，
	m=1，电机1走，，，，，
*/
void goNStep(int n,int m){
   int  i;
    for(i=0;i<n;i++){
	    //对应管脚电平切换一下电机走一步
		if(m==0)
		{
			outPut(1,1);								//输出1
			outPut(2,1);
			outPut(3,1);
			outPut(4,1);		
			delay_us(JIAOZHUN_STEP_INTERVAL);		//延时 0.15毫秒 （150微秒）		
			outPut(1,0);
			outPut(2,0);
			outPut(3,0);
			outPut(4,0);

			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//延时 1毫秒 （1000微秒）		
		}
		if(m==1)//电机1，
		{
			outPut(1,1);								//输出1
			delay_us(JIAOZHUN_STEP_INTERVAL);		//延时 1毫秒 （1000微秒）		
			outPut(1,0);
			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//延时 1毫秒 （1000微秒）			
		}
		if(m==2)//电机2
		{
			outPut(2,1);								//输出1
			delay_us(JIAOZHUN_STEP_INTERVAL);		//延时 1毫秒 （1000微秒）		
			outPut(2,0);
			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//延时 1毫秒 （1000微秒）			
		}
		if(m==3)//电机3
		{
			outPut(3,1);								//输出1
			delay_us(JIAOZHUN_STEP_INTERVAL);		//延时 1毫秒 （1000微秒）		
			outPut(3,0);
			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//延时 1毫秒 （1000微秒）			
		}
		if(m==4)//电机4
		{
			outPut(4,1);								//输出1
			delay_us(JIAOZHUN_STEP_INTERVAL);		//延时 1毫秒 （1000微秒）		
			outPut(4,0);
			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//延时 1毫秒 （1000微秒）			
		}
	}
}

/*转一圈花费480ms左右，n>0，赶时间用
 *速度过快，可能会引起丢步，需要根据实际负载调整，并重新耗时
*/
void goNCircle(int n){
   int i;
   for(i=0;i<n;i++){
     goNStep(ONE_CIRCLE_STEPS,0);
  }
}


/*
  注意：下面的秒是表盘刻度秒，不是电机转动花费的时间
  让电机追赶一定时间,一圈1600步，15秒，1秒走1600/15步，一步花费15/1600=0.009375秒
	参数说明：second是追赶的秒数
	num是哪个电机

*/
void goNSecond(int second,int num){ 
  int steps; 
  float stepTime=JIAOZHUN_STEP_INTERVAL*2/1000000.0;//每步花费时间 s 0.0003
  float v1=15/(ONE_CIRCLE_STEPS*stepTime); //赶时间每秒间隔走过的度数
  float v2=360/60.0; 		//每秒间隔走过的度数
  float t=second/(v1-v2);  //赶时间需要消耗的时间
  steps=t/stepTime;			//电机应该走过的步数
 // isStop=1;				    //电机停止运转
  if(num==0)
	{
				isStop=1;				    //电机停止运转
				goNStep(steps,0);		    //开始追赶 默认为0，全部追赶
				isStop=0;					//电机正常运转
	}
	if(num==1)
	goNStep(steps,1);			//电机1追赶
	if(num==2)
	goNStep(steps,2);			//电机1追赶
	if(num==3)
	goNStep(steps,3);			//电机1追赶
	if(num==4)
	goNStep(steps,4);			//电机1追赶
  
//	isStop=0;					//电机正常运转
}

//控制PUL脉冲  管脚为PE0
void outPut(int moter,int value){
   	  switch (moter){
	     case DJ1://电机1，控制脉冲
			  if(value==0) 
			  		GPIO_ResetBits(GPIOE, GPIO_Pin_0);
		   	  else 	
			  		GPIO_SetBits(GPIOE, GPIO_Pin_0); 
			  break;
		  case DJ2://电机2，控制脉冲
			  if(value==0) 
			  		GPIO_ResetBits(GPIOC, GPIO_Pin_0);
		   	  else 	
			  		GPIO_SetBits(GPIOC, GPIO_Pin_0); 
			  break; 
				
		 case DJ3://电机2，控制脉冲
			  if(value==0) 
			  		GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		   	  else 	
			  		GPIO_SetBits(GPIOC, GPIO_Pin_2); 
			  break; 
				
		 case DJ4://电机2，控制脉冲
			  if(value==0) 
			  		GPIO_ResetBits(GPIOG, GPIO_Pin_10);
		   	  else 	
			  		GPIO_SetBits(GPIOG, GPIO_Pin_10); 
			  break; 
	  }  
}

//控制DIR方向  管脚为PB9
void SetMonorDirection(int monitor,int value){

	 
	  switch (monitor){
	    case DJ1:
			//逆时针输出0
   	  		if(value==NI_SHI_ZHEN){
	      		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	   		}
	  		else{
	  			//顺时针输出1
	  			GPIO_SetBits(GPIOB, GPIO_Pin_9);	     
	  		}   
		 	break;
		case DJ2:
			//逆时针输出0
   	  		if(value==NI_SHI_ZHEN){
	      		GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	   		}
	  		else{
	  			//顺时针输出1
	  			GPIO_SetBits(GPIOC, GPIO_Pin_1);	     
	  		} 
			break;
			
		case DJ3:
			//逆时针输出0
   	  		if(value==NI_SHI_ZHEN){
	      		GPIO_ResetBits(GPIOC, GPIO_Pin_3);
	   		}
	  		else{
	  			//顺时针输出1
	  			GPIO_SetBits(GPIOC, GPIO_Pin_3);	     
	  		} 
			break;
			
		case DJ4:
			//逆时针输出0
   	  if(value==NI_SHI_ZHEN){
	      		GPIO_ResetBits(GPIOG, GPIO_Pin_9);
	   		}
	  		else{
	  			//顺时针输出1
	  			GPIO_SetBits(GPIOG, GPIO_Pin_9);	     
	  		} 
			break;
		default:
			break;
	  }
}
//微秒延时
void delay_us(u32 nus)			   
{
 	u32 temp;
 	SysTick->LOAD = 9*nus;
 	SysTick->VAL=0X00;                       		//清空计数器
 	SysTick->CTRL=0X01;                      		//使能，减到零是无动作，采用外部时钟源
 	do
 	{
  		temp=SysTick->CTRL;                  		//读取当前倒计数值
 	}
	while((temp&0x01)&&(!(temp&(1<<16))));   		//等待时间到达
    SysTick->CTRL=0x00;                     	//关闭计数器
    SysTick->VAL =0X00;                     	//清空计数器 
}




//电机初始化,PE0=脉冲,PB9=电机方向，
void initMotor()
{  
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB,ENABLE);

	init.GPIO_Pin = GPIO_Pin_0;	//PE0，电机1脉冲
	init.GPIO_Mode = GPIO_Mode_Out_PP;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &init);	
	
	init.GPIO_Pin = GPIO_Pin_9;	 //PB9，电机1方向
	GPIO_Init(GPIOB, &init);

	
	
	//第二个电机
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	init.GPIO_Pin = GPIO_Pin_0;	 //PC0，电机2脉冲
	GPIO_Init(GPIOC, &init);

	init.GPIO_Pin = GPIO_Pin_1;	 //PC1，电机2方向
	GPIO_Init(GPIOC, &init);
	
	
	//第三个电机
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	init.GPIO_Pin = GPIO_Pin_2;	 //PC2，电机3脉冲
	GPIO_Init(GPIOC, &init);

	init.GPIO_Pin = GPIO_Pin_3;	 //PC3，电机3方向
	GPIO_Init(GPIOC, &init);
	
	
	//第四个电机
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	init.GPIO_Pin = GPIO_Pin_10;	 //PC2，电机3脉冲
	GPIO_Init(GPIOG, &init);

	init.GPIO_Pin = GPIO_Pin_9;	 //PC3，电机3方向
	GPIO_Init(GPIOG, &init);
	
}




/*=======TIMER2,用于电机自动运转，15秒转一圈===========*/



//定时器初始化函数,second为秒数，最大为32
void Time2Init(u8 second){
   TIM_TimeBaseInitTypeDef tim2Stu;
   TIM_DeInit(TIM2);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
   tim2Stu.TIM_Period=35999;
   tim2Stu.TIM_Prescaler=2000*second-1;
   tim2Stu.TIM_ClockDivision=0x0;
   tim2Stu.TIM_CounterMode=TIM_CounterMode_Up;

   TIM_TimeBaseInit(TIM2,&tim2Stu);
   
   TIM_ClearFlag(TIM2,TIM_FLAG_Update);

   TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
   TIM_Cmd(TIM2,ENABLE);

}

void TIM2_NVIC_Configuration(){

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    //配置定时器2中断通道
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
/*=====TIMER2=============*/


//定时器中断服务函数

void TIM2_IRQHandler(void){
  
  TIM_ClearFlag(TIM2,TIM_FLAG_Update);
  //让电动机转一圈,电机正常运转
  goOneCircle(0);
}




