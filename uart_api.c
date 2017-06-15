#include "stm32f10x.h"
#include <stdio.h>
#include "uart_api.h"

#define RS485_TX_Disable()	GPIO_ResetBits(GPIOF,  GPIO_Pin_10 | GPIO_Pin_11) /* 释放总线 */
#define RS485_TX_Enable()	GPIO_SetBits(GPIOF,  GPIO_Pin_10 | GPIO_Pin_11) 	/* 占用总线，准备发送 */ 


/* 定义3个串口结构体变量 */
static UART_T g_tUart1;
static UART_T g_tUart2;
static UART_T g_tUart3;

/* 定义各自的串口FIFO缓冲区 */
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
* 函数名: ConfigUartNVIC
* 功  能: 配置硬件中断.
* 输  入: 无
* 输  出: 无
* 返  回: 无
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
* 函数名: comInit
* 功  能: 初始化串口硬件，并对全局变量赋初值.
* 输  入: 无
* 输  出: 无
* 返  回: 无
*/
void comInit(void)
{
	UartVarInit();		/* 必须先初始化全局变量,再配置硬件 */
	
//	InitHardUart1();	/* 配置串口1的硬件参数 */
	InitHardUart2();	/* 配置串口2的硬件参数 */
//	InitHardUart3();	/* 配置串口3的硬件参数 */

	ConfigUartNVIC();
}

/****************************************************************************
* 函数名: comSendBuf
* 功  能: 向串口发送一组数据，非阻塞。
* 输  入: 	_ucPort: 端口号(COM1 COM2 COM3)
*			_ucaBuf: 待发送的数据缓冲区
*			_usLen : 数据长度
* 输  出: 无
* 返  回: 无
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

		RS485_TX_Enable();	/* 控制RS485芯片，准备发送数据 */
	}
	else
	{
		/* 不做任何处理 */
		return;
	}

	UartSend(pUart, _ucaBuf, _usLen);	
}

/****************************************************************************
* 函数名: comSendChar
* 功  能: 向串口发送1个字节，非阻塞。
* 输  入: 	_ucPort: 端口号(COM1 COM2 COM3)
*			_ucByte: 待发送的数据
* 输  出: 无
* 返  回: 无
*/
void comSendChar(uint8_t _ucPort, uint8_t _ucByte)
{
	/****************************************************************************
	* 函数名: comSendBuf
	* 功  能: 向串口发送一组数据，非阻塞。
	* 输  入: 	_ucPort: 端口号(COM1 COM2 COM3)
	*			_ucaBuf: 待发送的数据缓冲区
	*			_usLen : 数据长度
	* 输  出: 无
	* 返  回: 无
	*/

	comSendBuf(_ucPort, &_ucByte, 1);
}

/****************************************************************************
* 函数名: comGetChar
* 功  能: 从串口缓冲区读取1字节，非阻塞。
* 输  入: _pByte 结果存放地址
* 输  出: 无
* 返  回: 0 表示无数据, 1 表示读取到有效字节
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
* 函数名: UartVarInit
* 功  能: 初始化串口相关的变量
* 输  入: 无
* 输  出: 无
* 返  回: 无
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
	g_tUart2.SendOver = RS485SendOver;	/* RS485是半双工,因此数据发送完毕后,需要释放RS485总线 */
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
* 函数名: InitHardUart1
* 功  能: 配置UART1硬件参数
* 输  入: 无
* 返  回: 无
*/

static void InitHardUart1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_TypeDef *uart;

	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 第4步：设置串口硬件参数 */
	uart = USART1;	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(uart, &USART_InitStructure);
	USART_ITConfig(uart, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/* 
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(uart, ENABLE);		/* 使能串口 */ 

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送外城标志，Transmission Complete flag */
}

/****************************************************************************
* 函数名: InitHardUart2
* 功  能: 配置USART2硬件参数
* 输  入: 无
* 返  回: 无
*/
static void InitHardUart2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_TypeDef *uart;

	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 第4步：设置串口硬件参数 */
	uart = USART2;	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(uart, &USART_InitStructure);
	USART_ITConfig(uart, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/* 
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(uart, ENABLE);		/* 使能串口 */ 
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* 清发送外城标志，Transmission Complete flag */
}

/****************************************************************************
* 函数名: InitHardUart3
* 功  能: 配置USART3硬件参数
* 输  入: 无
* 返  回: 无
*/
static void InitHardUart3(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_TypeDef *uart;

	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 第4步：设置串口硬件参数 */
	uart = USART3;	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(uart, &USART_InitStructure);
	USART_ITConfig(uart, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/* 
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(uart, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* 清发送外城标志，Transmission Complete flag */	 
}

/****************************************************************************
* 函数名: RS485SendOver
* 功  能: 向RS485总线发送一帧数据结束后，立即调用本函数.
* 输  入: 无
* 输  出: 无
* 返  回: 无
*/
static void RS485SendOver(void)
{
	RS485_TX_Disable();		/* 释放总线 */
}

/****************************************************************************
* 函数名: UartSend
* 功  能: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动
*			关闭发送中断。
* 输  入: _pUart: 串口结构体指针
* 输  出: 无
* 返  回: 无
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
* 函数名: UartIRQ
* 功  能: 供中断服务程序调用，通用串口中断处理函数
* 输  入: _pUart: 串口结构体指针
* 输  出: 无
* 返  回: 无
*/
void UartIRQ(UART_T *_pUart)
{
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{ 
		/* 回调函数,通知应用程序发送完毕,一般是发送1个消息或者设置一个标记 */
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

	/* 发送缓冲区空中断 */
	if (USART_GetITStatus(_pUart->uart, USART_IT_TXE) != RESET)
	{   
		if (_pUart->usTxRead == _pUart->usTxWrite)
		{
			/* 禁止发送缓冲区空中断，使能发送完毕中断 */
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
	/* 1个字节发送完毕的中断 */
	else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
	{
		if (_pUart->usTxRead == _pUart->usTxWrite)
		{
			/* 禁止发送缓冲区空中断，使能发送完毕中断 */
			USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);
			
			/* 回调函数,通知应用程序发送完毕,一般是发送1个消息或者设置一个标记 */
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
* 函数名: USART1_IRQHandler
* 功  能: USART1中断服务程序.
* 输  入: 无
* 输  出: 无
* 返  回: 无
*/
void USART1_IRQHandler(void)
{
	UartIRQ(&g_tUart1);
}

/****************************************************************************
* 函数名: USART2_IRQHandler
* 功  能: USART2中断服务程序.
* 输  入: 无
* 输  出: 无
* 返  回: 无
*/
void USART2_IRQHandler(void)
{
	UartIRQ(&g_tUart2);
}

/****************************************************************************
* 函数名: USART3_IRQHandler
* 功  能: UART3中断服务程序.
* 输  入: 无
* 输  出: 无
* 返  回: 无
*/
void USART3_IRQHandler(void)
{
	UartIRQ(&g_tUart3);
}

/****************************************************************************
* 函数名: UartGetChar
* 功  能: 从串口接收缓冲区读取n字节数据
* 输  入: _pUart: 串口结构体指针
* 输  出: 无
* 返  回: 1表示正确，0表示无数据
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
//开启一次DMA传输
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len)
{
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭 指示的通道        
	DMA_SetCurrDataCounter(DMA_CHx,len);//DMA通道的DMA缓存的大小	
	DMA_Cmd(DMA_CHx, ENABLE);           //开启DMA传输
}





