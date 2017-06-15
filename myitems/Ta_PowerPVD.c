 #include "Ta_PowerPVD.h"
 #include "stm32f10x_pwr.h"

 extern totalSeconds;
 

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж����ȼ����� 
  
  /* Enable the PVD Interrupt */ //����PVD�ж�
  NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and
     falling edges */
  EXTI_ClearITPendingBit(EXTI_Line16); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line16;// PVD���ӵ��ж���16�� 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ʹ���ж�ģʽ 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//��ѹ�������½�Խ���趨��ֵʱ�������жϡ�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;// ʹ���ж���
  EXTI_Init(&EXTI_InitStructure);// ��ʼ
}


//���ڵ��籣�浱ǰ���������жϷ�������

void PVD_Config(void){
    /*
	  000��2.2V
	  001��2.3V
	  010��2.4V
      011��2.5V
      100��2.6V
      101��2.7V
      110��2.8V
      111��2.9V
	*/
	/* Enable PWR and BKP clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//��Դ������ʱ�ӿ��� 

	/* Configure EXTI Line to generate an interrupt on falling edge */
    EXTI_Configuration();

    /* NVIC configuration */
    NVIC_Configuration();

	/* Configure the PVD Level to 2.9V */
    PWR_PVDLevelConfig(PWR_PVDLevel_2V9);// �趨��ط�ֵ 

	/* Enable the PVD Output */
    PWR_PVDCmd(ENABLE);// ʹ��PVD 

	}

//�жϷ���
void PVD_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line16) != RESET)
	{
	   //����totalSecond ��EEPROM
	   ee_Write(totalSeconds);//totalSecondsΪ�����߹�������
	   /* Clear the Key Button EXTI line pending bit */
	   EXTI_ClearITPendingBit(EXTI_Line16);
	}
}