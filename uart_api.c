#include "stm32f10x.h"
#include <stdio.h>
#include "uart_api.h"

#define RS485_TX_Disable()	GPIO_ResetBits(GPIOF,  GPIO_Pin_10 | GPIO_Pin_11) /* �ͷ����� */
#define RS485_TX_Enable()	GPIO_SetBits(GPIOF,  GPIO_Pin_10 | GPIO_Pin_11) 	/* ռ�����ߣ�׼������ */ 


/* ����3�����ڽṹ����� */
static UART_T g_tUart1;
static UART_T g_tUart2;
static UART_T g_tUart3;

/* ������ԵĴ���FIFO������ */
 uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];
 uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];

 uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];
 uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];

static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];
static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];

static void UartVarInit(void);
static void InitHardUart1(void);
static void InitHardUart2(void);
static void InitHardUart3(void);

static void RS485SendOver(void);
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);

/****************************************************************************
* ������: ConfigUartNVIC
* ��  ��: ����Ӳ���ж�.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
static void ConfigUartNVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USART3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* ������: comInit
* ��  ��: ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
void comInit(void)
{
	UartVarInit();		/* �����ȳ�ʼ��ȫ�ֱ���,������Ӳ�� */
	
//	InitHardUart1();	/* ���ô���1��Ӳ������ */
	InitHardUart2();	/* ���ô���2��Ӳ������ */
//	InitHardUart3();	/* ���ô���3��Ӳ������ */

	ConfigUartNVIC();
}

/****************************************************************************
* ������: comSendBuf
* ��  ��: �򴮿ڷ���һ�����ݣ���������
* ��  ��: 	_ucPort: �˿ں�(COM1 COM2 COM3)
*			_ucaBuf: �����͵����ݻ�����
*			_usLen : ���ݳ���
* ��  ��: ��
* ��  ��: ��
*/
void comSendBuf(uint8_t _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;
		
	if (_ucPort == COM1)
	{
		pUart = &g_tUart1;
		
	}
	else if (_ucPort == COM2)
	{
		pUart = &g_tUart2;
	}
	else if (_ucPort == COM3)
	{
		pUart = &g_tUart3;

		RS485_TX_Enable();	/* ����RS485оƬ��׼���������� */
	}
	else
	{
		/* �����κδ��� */
		return;
	}

	UartSend(pUart, _ucaBuf, _usLen);	
}

/****************************************************************************
* ������: comSendChar
* ��  ��: �򴮿ڷ���1���ֽڣ���������
* ��  ��: 	_ucPort: �˿ں�(COM1 COM2 COM3)
*			_ucByte: �����͵�����
* ��  ��: ��
* ��  ��: ��
*/
void comSendChar(uint8_t _ucPort, uint8_t _ucByte)
{
	/****************************************************************************
	* ������: comSendBuf
	* ��  ��: �򴮿ڷ���һ�����ݣ���������
	* ��  ��: 	_ucPort: �˿ں�(COM1 COM2 COM3)
	*			_ucaBuf: �����͵����ݻ�����
	*			_usLen : ���ݳ���
	* ��  ��: ��
	* ��  ��: ��
	*/

	comSendBuf(_ucPort, &_ucByte, 1);
}

/****************************************************************************
* ������: comGetChar
* ��  ��: �Ӵ��ڻ�������ȡ1�ֽڣ���������
* ��  ��: _pByte �����ŵ�ַ
* ��  ��: ��
* ��  ��: 0 ��ʾ������, 1 ��ʾ��ȡ����Ч�ֽ�
*/
uint8_t comGetChar(uint8_t _ucPort, uint8_t *_pByte)
{
	UART_T *pUart;
	
	if (_ucPort == COM1)
	{
		pUart = &g_tUart1;
	}
	else if (_ucPort == COM2)
	{
		pUart = &g_tUart2;
	}
	else if (_ucPort == COM3)
	{
		pUart = &g_tUart3;
	}
	else
	{
		return 0;
	}
	
	return UartGetChar(pUart, _pByte);
}

/****************************************************************************
* ������: UartVarInit
* ��  ��: ��ʼ��������صı���
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
static void UartVarInit(void)
{
	g_tUart1.uart = USART1;
	g_tUart1.pTxBuf = g_TxBuf1;
	g_tUart1.pRxBuf = g_RxBuf1;
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;
	g_tUart1.usTxWrite = 0;
	g_tUart1.usTxRead = 0;
	g_tUart1.usRxWrite = 0;
	g_tUart1.usRxRead = 0;
	g_tUart1.SendOver = 0;	/* */
	g_tUart1.ReciveNew = 0;

	g_tUart2.uart = USART2;
	g_tUart2.pTxBuf = g_TxBuf2;
	g_tUart2.pRxBuf = g_RxBuf2;
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;
	g_tUart2.usTxWrite = 0;
	g_tUart2.usTxRead = 0;
	g_tUart2.usRxWrite = 0;
	g_tUart2.usRxRead = 0;
	g_tUart2.SendOver = RS485SendOver;	/* RS485�ǰ�˫��,������ݷ�����Ϻ�,��Ҫ�ͷ�RS485���� */
	g_tUart2.ReciveNew = 0;

	g_tUart3.uart = USART3;
	g_tUart3.pTxBuf = g_TxBuf3;
	g_tUart3.pRxBuf = g_RxBuf3;
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;
	g_tUart3.usTxWrite = 0;
	g_tUart3.usTxRead = 0;
	g_tUart3.usRxWrite = 0;
	g_tUart3.usRxRead = 0;
	g_tUart3.SendOver = 0;
	g_tUart3.ReciveNew = 0;
}

/****************************************************************************
* ������: InitHardUart1
* ��  ��: ����UART1Ӳ������
* ��  ��: ��
* ��  ��: ��
*/

static void InitHardUart1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_TypeDef *uart;

	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  ��3���Ѿ����ˣ�����ⲽ���Բ���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ��4�������ô���Ӳ������ */
	uart = USART1;	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(uart, &USART_InitStructure);
	USART_ITConfig(uart, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/* 
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(uart, ENABLE);		/* ʹ�ܴ��� */ 

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */
}

/****************************************************************************
* ������: InitHardUart2
* ��  ��: ����USART2Ӳ������
* ��  ��: ��
* ��  ��: ��
*/
static void InitHardUart2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_TypeDef *uart;

	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  ��3���Ѿ����ˣ�����ⲽ���Բ���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ��4�������ô���Ӳ������ */
	uart = USART2;	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(uart, &USART_InitStructure);
	USART_ITConfig(uart, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/* 
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(uart, ENABLE);		/* ʹ�ܴ��� */ 
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */
}

/****************************************************************************
* ������: InitHardUart3
* ��  ��: ����USART3Ӳ������
* ��  ��: ��
* ��  ��: ��
*/
static void InitHardUart3(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_TypeDef *uart;

	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*  ��3���Ѿ����ˣ�����ⲽ���Բ���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ��4�������ô���Ӳ������ */
	uart = USART3;	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(uart, &USART_InitStructure);
	USART_ITConfig(uart, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/* 
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(uart, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */	 
}

/****************************************************************************
* ������: RS485SendOver
* ��  ��: ��RS485���߷���һ֡���ݽ������������ñ�����.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
static void RS485SendOver(void)
{
	RS485_TX_Disable();		/* �ͷ����� */
}

/****************************************************************************
* ������: UartSend
* ��  ��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ�
*			�رշ����жϡ�
* ��  ��: _pUart: ���ڽṹ��ָ��
* ��  ��: ��
* ��  ��: ��
*/
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t ucNew = 0;
    
	CPU_IntDis();
	if (_pUart->usTxWrite == _pUart->usTxRead)
	{
		ucNew = 1;
	}
	CPU_IntEn();
	
	for (i = 0; i < _usLen; i++)
	{	
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];
		
		CPU_IntDis();	
		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
		{
			_pUart->usTxWrite = 0;
		}
		CPU_IntEn();
	}

	if (ucNew == 1)
	{
		USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);
	}
}

/****************************************************************************
* ������: UartIRQ
* ��  ��: ���жϷ��������ã�ͨ�ô����жϴ�����
* ��  ��: _pUart: ���ڽṹ��ָ��
* ��  ��: ��
* ��  ��: ��
*/
void UartIRQ(UART_T *_pUart)
{
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{ 
		/* �ص�����,֪ͨӦ�ó��������,һ���Ƿ���1����Ϣ��������һ����� */
		if (_pUart->usRxWrite == _pUart->usRxRead)
		{
			if (_pUart->ReciveNew)
			{
				_pUart->ReciveNew();
			}
		}
			
		/* Read one byte from the receive data register */
		_pUart->pRxBuf[_pUart->usRxWrite] = USART_ReceiveData(_pUart->uart);		
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
		{
			_pUart->usRxWrite = 0;
		}		
	}

	/* ���ͻ��������ж� */
	if (USART_GetITStatus(_pUart->uart, USART_IT_TXE) != RESET)
	{   
		if (_pUart->usTxRead == _pUart->usTxWrite)
		{
			/* ��ֹ���ͻ��������жϣ�ʹ�ܷ�������ж� */
			USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);
			USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
		} 
		else
		{		
			/* Write one byte to the transmit data register */
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);                    
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
		}
   
	}
	/* 1���ֽڷ�����ϵ��ж� */
	else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
	{
		if (_pUart->usTxRead == _pUart->usTxWrite)
		{
			/* ��ֹ���ͻ��������жϣ�ʹ�ܷ�������ж� */
			USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);
			
			/* �ص�����,֪ͨӦ�ó��������,һ���Ƿ���1����Ϣ��������һ����� */
			if (_pUart->SendOver)
			{
				_pUart->SendOver();
			}
		} 
		else
		{		
			/* Write one byte to the transmit data register */
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);                    
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
		}
	}
}

/****************************************************************************
* ������: USART1_IRQHandler
* ��  ��: USART1�жϷ������.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
void USART1_IRQHandler(void)
{
	UartIRQ(&g_tUart1);
}

/****************************************************************************
* ������: USART2_IRQHandler
* ��  ��: USART2�жϷ������.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
void USART2_IRQHandler(void)
{
	UartIRQ(&g_tUart2);
}

/****************************************************************************
* ������: USART3_IRQHandler
* ��  ��: UART3�жϷ������.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
void USART3_IRQHandler(void)
{
	UartIRQ(&g_tUart3);
}

/****************************************************************************
* ������: UartGetChar
* ��  ��: �Ӵ��ڽ��ջ�������ȡn�ֽ�����
* ��  ��: _pUart: ���ڽṹ��ָ��
* ��  ��: ��
* ��  ��: 1��ʾ��ȷ��0��ʾ������
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usRxWrite;
	
	CPU_IntDis();	
	usRxWrite = _pUart->usRxWrite;
	CPU_IntEn();

	if (_pUart->usRxRead == usRxWrite)
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];
		
		CPU_IntDis();
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)
		{
			_pUart->usRxRead = 0;
		}		
		CPU_IntEn();
		return 1;
	}
}
//����һ��DMA����
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len)
{
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر� ָʾ��ͨ��        
	DMA_SetCurrDataCounter(DMA_CHx,len);//DMAͨ����DMA����Ĵ�С	
	DMA_Cmd(DMA_CHx, ENABLE);           //����DMA����
}





