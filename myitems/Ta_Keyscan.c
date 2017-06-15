#include "Ta_Keyscan.h"

int keyCode=-1;		//��Ű����ı��룬�����Լ��� ,-1��ʾû�а���
unsigned char key[4][4];  //���ÿ��������״̬��1��ʾ���£�0��ʾû����

//���̳�ʼ������
void keyboard_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; //�˿ڽṹ�����
	unsigned char i;

	//�мǣ���Ҫ����ʵ����Ҫ�޸ģ�����
	//���ö˿ڣ��õ���PA PG���˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOG, ENABLE);


	/* ������ɨ������� ����ߵ�ƽ���������������� */
	/* PA12 PA10 PA8 PC8 ���*/ //�мǣ���Ҫ����ʵ����Ҫ�޸ģ�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_10 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ������ɨ�������� ������ʱ����ߵ�ƽ �ſ�����͵�ƽ */
	/* PC6 PG7 PG5 PG3 ����*/ //�мǣ���Ҫ����ʵ����Ҫ�޸ģ�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU; 
    GPIO_Init(GPIOG, &GPIO_InitStructure);

	//��ʼ���ĸ�������ߵ�ƽ
	for(i = 0; i < 4; i++)	{
       GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
	}
}

//����ɨ�����
int keyscan(void){
	unsigned char i, j;

	for(i = 0; i < 4; i++)             //i������ڣ������õ͵�ƽ
	{
  		GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
  		for(j = 0; j < 4; j++)   //j���з��򣬵�������ʱ��ͨ����Ϊ�͵�ƽ  
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
	for(i = 0; i < 4; i++)             //i������ڣ����θߵ�ƽ
	{
	 	GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
	}
	return keyCode;
}




/*=======TIMER4===========*/

//��ʱ����ʼ������,secondΪ��������ms>1<32767���룬32������
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
    //���ö�ʱ��4�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

//��ʱ���жϷ�����

void TIM4_IRQHandler(void){ 
  TIM_ClearFlag(TIM4,TIM_FLAG_Update);  
  //�õ綯��תһȦ
  keyscan();// 

  //printf("�ж�ִ��%d��  \r\n",i++); 
  
}

/*=====TIMER4=============*/
