 #include "stm32f10x.h"
#include <stdio.h>	
#include "bsp_usart.h"
#include "Ta_Motor.h"
#include "Ta_gps.h"		   //GPSģ��
#include "bsp_timer.h"		/* systick��ʱ��ģ�� */
#include "bsp_tft_lcd.h"	/* TFTҺ����ʾ������ģ�� */
#include "Ta_calibration.h"	/*��õ�ǰʱ���Լ�У׼ʱ��*/
#include "Ta_Mp3_playe.h"		/* MP3������ģ��*/


extern gpsSeconds;
extern totalSeconds;
extern char *fileName[];			//���㱨ʱ�ļ�������
extern DianjiConfig dianjiOthers;		//��ŵ���������ã��籨ʱ������




/*������� ���������
 * ��ѹ�� 32V
 * ������ 1��
 *ϸ�֣�8ϸ�֣�ÿ������0.225��
*/

extern int keyCode;	

int isStop=0;							//�Ƿ�ֹͣ�����ת

//==============================����Ϊ�õ���=============================
/*
  *�綯����һȦ,3s����,������ת�����������תʱ����
*/
void goOneCircle(int djnum){ 
   int i=0;
   if(isStop)return;//���isStop=1����ͣ��,0�����߶�
	 //Ĭ��������ĸ����ͬʱת
	 if(djnum==0)
	 {
		 for(i=0;i<ONE_CIRCLE_STEPS;i++){
				//��Ӧ�ܽŵ�ƽ�л�һ�µ����һ��
				outPut(1,1);			//���1	
				outPut(2,1);			//���1 
				outPut(3,1);
				outPut(4,1);
				delay_us(NORMAL_STEP_INTERVAL);		//��ʱ 1���� ��1000΢�룩		
				outPut(1,0);
				outPut(2,0);			//���0
				outPut(3,0);
				outPut(4,0);
				delay_us(NORMAL_STEP_INTERVAL);	  	//��ʱ 1���� ��1000΢�룩
			}
		}
		
	 if(djnum==1)
	 {
		 for(i=0;i<ONE_CIRCLE_STEPS;i++){
				//��Ӧ�ܽŵ�ƽ�л�һ�µ����һ��
				outPut(1,1);			//���1	
				delay_us(NORMAL_STEP_INTERVAL);		//��ʱ 1���� ��1000΢�룩		
				outPut(1,0);
				delay_us(NORMAL_STEP_INTERVAL);	  	//��ʱ 1���� ��1000΢�룩
			}
	 }
   //����������
   record_NowTiem();

   //����3��룬�Զ�У׼GPS��ʱ��
   if(totalSeconds%12600==0){
      calibration_Time(0);//У׼ʱ��
   }   

   //�����Զ���ʱ
   if(totalSeconds%3600==0){//����      
     i=totalSeconds/3600;
	  //ҹ��ֹͣ��ʱ (23��-6�ڼ�)
		//��������ļ����еı�ʱֵΪ1���ͱ�ʱ
		if(dianjiOthers.isBaoshi==1)
		{
			//��ȡ�����ļ����еı�ʱ����
			if(i>=dianjiOthers.baoshi_Start&&i<=dianjiOthers.baoshi_End)
			{
				//�򿪼̵�����Ȼ�󲥷ű�ʱ
				setOpen(1);
				Mp3PlayerAFile(fileName[i-1]);//��ʱi-1����
			 //������Ϻ�رռ̵���
				setClosed(1);
			 }
			 
		}
		//��������
		if(dianjiOthers.isZhaoMing==1)
		{
		if(i>=dianjiOthers.zhaomiang_Start&&i<=dianjiOthers.zhaoming_End)
			{
				//�򿪼̵���������
				setOpen(2);
			}
		}
	  //����Ҫ���Ǳ�ʱ�������ʱ����......................
   }

}
/*
  �����N���ĺ���,
	����������n��ʾ�߶��ٲ���m��ʾ�ĸ������
	��m=0��Ĭ��������ĸ����ͬʱ�ߣ�
	m=1�����1�ߣ���������
*/
void goNStep(int n,int m){
   int  i;
    for(i=0;i<n;i++){
	    //��Ӧ�ܽŵ�ƽ�л�һ�µ����һ��
		if(m==0)
		{
			outPut(1,1);								//���1
			outPut(2,1);
			outPut(3,1);
			outPut(4,1);		
			delay_us(JIAOZHUN_STEP_INTERVAL);		//��ʱ 0.15���� ��150΢�룩		
			outPut(1,0);
			outPut(2,0);
			outPut(3,0);
			outPut(4,0);

			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//��ʱ 1���� ��1000΢�룩		
		}
		if(m==1)//���1��
		{
			outPut(1,1);								//���1
			delay_us(JIAOZHUN_STEP_INTERVAL);		//��ʱ 1���� ��1000΢�룩		
			outPut(1,0);
			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//��ʱ 1���� ��1000΢�룩			
		}
		if(m==2)//���2
		{
			outPut(2,1);								//���1
			delay_us(JIAOZHUN_STEP_INTERVAL);		//��ʱ 1���� ��1000΢�룩		
			outPut(2,0);
			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//��ʱ 1���� ��1000΢�룩			
		}
		if(m==3)//���3
		{
			outPut(3,1);								//���1
			delay_us(JIAOZHUN_STEP_INTERVAL);		//��ʱ 1���� ��1000΢�룩		
			outPut(3,0);
			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//��ʱ 1���� ��1000΢�룩			
		}
		if(m==4)//���4
		{
			outPut(4,1);								//���1
			delay_us(JIAOZHUN_STEP_INTERVAL);		//��ʱ 1���� ��1000΢�룩		
			outPut(4,0);
			delay_us(JIAOZHUN_STEP_INTERVAL);	  	//��ʱ 1���� ��1000΢�룩			
		}
	}
}

/*תһȦ����480ms���ң�n>0����ʱ����
 *�ٶȹ��죬���ܻ����𶪲�����Ҫ����ʵ�ʸ��ص����������º�ʱ
*/
void goNCircle(int n){
   int i;
   for(i=0;i<n;i++){
     goNStep(ONE_CIRCLE_STEPS,0);
  }
}


/*
  ע�⣺��������Ǳ��̶̿��룬���ǵ��ת�����ѵ�ʱ��
  �õ��׷��һ��ʱ��,һȦ1600����15�룬1����1600/15����һ������15/1600=0.009375��
	����˵����second��׷�ϵ�����
	num���ĸ����

*/
void goNSecond(int second,int num){ 
  int steps; 
  float stepTime=JIAOZHUN_STEP_INTERVAL*2/1000000.0;//ÿ������ʱ�� s 0.0003
  float v1=15/(ONE_CIRCLE_STEPS*stepTime); //��ʱ��ÿ�����߹��Ķ���
  float v2=360/60.0; 		//ÿ�����߹��Ķ���
  float t=second/(v1-v2);  //��ʱ����Ҫ���ĵ�ʱ��
  steps=t/stepTime;			//���Ӧ���߹��Ĳ���
 // isStop=1;				    //���ֹͣ��ת
  if(num==0)
	{
				isStop=1;				    //���ֹͣ��ת
				goNStep(steps,0);		    //��ʼ׷�� Ĭ��Ϊ0��ȫ��׷��
				isStop=0;					//���������ת
	}
	if(num==1)
	goNStep(steps,1);			//���1׷��
	if(num==2)
	goNStep(steps,2);			//���1׷��
	if(num==3)
	goNStep(steps,3);			//���1׷��
	if(num==4)
	goNStep(steps,4);			//���1׷��
  
//	isStop=0;					//���������ת
}

//����PUL����  �ܽ�ΪPE0
void outPut(int moter,int value){
   	  switch (moter){
	     case DJ1://���1����������
			  if(value==0) 
			  		GPIO_ResetBits(GPIOE, GPIO_Pin_0);
		   	  else 	
			  		GPIO_SetBits(GPIOE, GPIO_Pin_0); 
			  break;
		  case DJ2://���2����������
			  if(value==0) 
			  		GPIO_ResetBits(GPIOC, GPIO_Pin_0);
		   	  else 	
			  		GPIO_SetBits(GPIOC, GPIO_Pin_0); 
			  break; 
				
		 case DJ3://���2����������
			  if(value==0) 
			  		GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		   	  else 	
			  		GPIO_SetBits(GPIOC, GPIO_Pin_2); 
			  break; 
				
		 case DJ4://���2����������
			  if(value==0) 
			  		GPIO_ResetBits(GPIOG, GPIO_Pin_10);
		   	  else 	
			  		GPIO_SetBits(GPIOG, GPIO_Pin_10); 
			  break; 
	  }  
}

//����DIR����  �ܽ�ΪPB9
void SetMonorDirection(int monitor,int value){

	 
	  switch (monitor){
	    case DJ1:
			//��ʱ�����0
   	  		if(value==NI_SHI_ZHEN){
	      		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	   		}
	  		else{
	  			//˳ʱ�����1
	  			GPIO_SetBits(GPIOB, GPIO_Pin_9);	     
	  		}   
		 	break;
		case DJ2:
			//��ʱ�����0
   	  		if(value==NI_SHI_ZHEN){
	      		GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	   		}
	  		else{
	  			//˳ʱ�����1
	  			GPIO_SetBits(GPIOC, GPIO_Pin_1);	     
	  		} 
			break;
			
		case DJ3:
			//��ʱ�����0
   	  		if(value==NI_SHI_ZHEN){
	      		GPIO_ResetBits(GPIOC, GPIO_Pin_3);
	   		}
	  		else{
	  			//˳ʱ�����1
	  			GPIO_SetBits(GPIOC, GPIO_Pin_3);	     
	  		} 
			break;
			
		case DJ4:
			//��ʱ�����0
   	  if(value==NI_SHI_ZHEN){
	      		GPIO_ResetBits(GPIOG, GPIO_Pin_9);
	   		}
	  		else{
	  			//˳ʱ�����1
	  			GPIO_SetBits(GPIOG, GPIO_Pin_9);	     
	  		} 
			break;
		default:
			break;
	  }
}
//΢����ʱ
void delay_us(u32 nus)			   
{
 	u32 temp;
 	SysTick->LOAD = 9*nus;
 	SysTick->VAL=0X00;                       		//��ռ�����
 	SysTick->CTRL=0X01;                      		//ʹ�ܣ����������޶����������ⲿʱ��Դ
 	do
 	{
  		temp=SysTick->CTRL;                  		//��ȡ��ǰ������ֵ
 	}
	while((temp&0x01)&&(!(temp&(1<<16))));   		//�ȴ�ʱ�䵽��
    SysTick->CTRL=0x00;                     	//�رռ�����
    SysTick->VAL =0X00;                     	//��ռ����� 
}




//�����ʼ��,PE0=����,PB9=�������
void initMotor()
{  
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB,ENABLE);

	init.GPIO_Pin = GPIO_Pin_0;	//PE0�����1����
	init.GPIO_Mode = GPIO_Mode_Out_PP;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &init);	
	
	init.GPIO_Pin = GPIO_Pin_9;	 //PB9�����1����
	GPIO_Init(GPIOB, &init);

	
	
	//�ڶ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	init.GPIO_Pin = GPIO_Pin_0;	 //PC0�����2����
	GPIO_Init(GPIOC, &init);

	init.GPIO_Pin = GPIO_Pin_1;	 //PC1�����2����
	GPIO_Init(GPIOC, &init);
	
	
	//���������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	init.GPIO_Pin = GPIO_Pin_2;	 //PC2�����3����
	GPIO_Init(GPIOC, &init);

	init.GPIO_Pin = GPIO_Pin_3;	 //PC3�����3����
	GPIO_Init(GPIOC, &init);
	
	
	//���ĸ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	init.GPIO_Pin = GPIO_Pin_10;	 //PC2�����3����
	GPIO_Init(GPIOG, &init);

	init.GPIO_Pin = GPIO_Pin_9;	 //PC3�����3����
	GPIO_Init(GPIOG, &init);
	
}




/*=======TIMER2,���ڵ���Զ���ת��15��תһȦ===========*/



//��ʱ����ʼ������,secondΪ���������Ϊ32
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
    //���ö�ʱ��2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
/*=====TIMER2=============*/


//��ʱ���жϷ�����

void TIM2_IRQHandler(void){
  
  TIM_ClearFlag(TIM2,TIM_FLAG_Update);
  //�õ綯��תһȦ,���������ת
  goOneCircle(0);
}




