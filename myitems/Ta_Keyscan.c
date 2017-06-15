#include "Ta_Keyscan.h"

int keyCode=-1;		//存放按键的编码，编码自己起 ,-1表示没有按键
unsigned char key[4][4];  //存放每个按键的状态，1表示按下，0表示没按下

//键盘初始化函数
void keyboard_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; //端口结构体变量
	unsigned char i;

	//切记！！要根据实际需要修改！！！
	//启用端口，用到的PA PG俩端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOG, ENABLE);


	/* 键盘行扫描输出线 输出高电平，从左至右依次是 */
	/* PA12 PA10 PA8 PC8 输出*/ //切记！！要根据实际需要修改！！！
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_10 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* 键盘列扫描输入线 键被按时输入高电平 放开输入低电平 */
	/* PC6 PG7 PG5 PG3 输入*/ //切记！！要根据实际需要修改！！！
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU; 
    GPIO_Init(GPIOG, &GPIO_InitStructure);

	//开始让四个行输出高电平
	for(i = 0; i < 4; i++)	{
       GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
	}
}

//键盘扫描程序
int keyscan(void){
	unsigned char i, j;

	for(i = 0; i < 4; i++)             //i是输出口，依次置低电平
	{
  		GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
  		for(j = 0; j < 4; j++)   //j是列方向，当键按下时导通被置为低电平  
  		{
    		if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)
     		{	
			 // bsp_DelayMS(20);
			  if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0){
			     //key[i][j] = 1;  
			     keyCode=(i+1)*10+(j+1); 
			     goto outer;
			  }
			}
			else{ 
			   //key[i][j] = 0;				   
			}
    	}
  	
	}
outer:
	for(i = 0; i < 4; i++)             //i是输出口，依次高电平
	{
	 	GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
	}
	return keyCode;
}




/*=======TIMER4===========*/

//定时器初始化函数,second为毫秒数，ms>1<32767毫秒，32秒以内
void Time4Init(u8 ms){
   TIM_TimeBaseInitTypeDef tim2Stu;
   TIM_DeInit(TIM4);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
   tim2Stu.TIM_Period=35999;
   tim2Stu.TIM_Prescaler=2*ms-1;
   tim2Stu.TIM_ClockDivision=0x0;
   tim2Stu.TIM_CounterMode=TIM_CounterMode_Up;

   TIM_TimeBaseInit(TIM4,&tim2Stu);
   
   TIM_ClearFlag(TIM4,TIM_FLAG_Update);

   TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
   TIM_Cmd(TIM4,ENABLE);

}

void TIM4_NVIC_Configuration(){
	
	NVIC_InitTypeDef NVIC_InitStructure;
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    //配置定时器4中断通道
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

//定时器中断服务函数

void TIM4_IRQHandler(void){ 
  TIM_ClearFlag(TIM4,TIM_FLAG_Update);  
  //让电动机转一圈
  keyscan();// 

  //printf("中断执行%d次  \r\n",i++); 
  
}

/*=====TIMER4=============*/
