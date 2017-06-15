/****************************************************************************
* Copyright (C), 2010 安富莱电子 www.armfly.com
*
* 文件名: usart_api.h
* 内容简述: 串口模块头文件。
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1    2010-08-21 armfly  创建该文件
*
*/

#ifndef _USART_API_H_
#define _USART_API_H_

#define  CPU_IntDis()           { __set_PRIMASK(1); }	/* 关中断 */
#define  CPU_IntEn()            { __set_PRIMASK(0); }	/* 开中断 */
	  


#define USART2_MAX_RECV_LEN		800					//最大接收缓存字节数
#define USART2_MAX_SEND_LEN		200					//最大发送缓存字节数
#define USART2_RX_EN 			1					//0,不接收;1,接收. 

/* 全局参数 */
typedef struct
{
	USART_TypeDef *uart;	/*  */	
	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	uint16_t usTxWrite;		/* 发送缓冲区写指针 */
	uint16_t usTxRead;		/* 发送缓冲区读指针 */
	uint16_t usRxWrite;		/* 接收缓冲区写指针 */
	uint16_t usRxRead;		/* 接收缓冲区读指针 */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针 */
	void (*ReciveNew)(void);	/* 串口收到数据的回调函数指针 */
}UART_T;

/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* COM1口, RS232 */
	COM2 = 1,	/* COM2口, RS232 */
	COM3 = 2	/* COM3口, RS485 */
}PORT_NO_E;

#define UART1_TX_BUF_SIZE	2*1024
#define UART1_RX_BUF_SIZE	2*1024

#define UART2_TX_BUF_SIZE	2*1024
#define UART2_RX_BUF_SIZE	2*1024

#define UART3_TX_BUF_SIZE	1024
#define UART3_RX_BUF_SIZE	1024

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

void comInit(void);
void comSendBuf(uint8_t _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(uint8_t _ucPort, uint8_t _ucByte);
uint8_t comGetChar(uint8_t _ucPort, uint8_t *_pByte);


#endif
