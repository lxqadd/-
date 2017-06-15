/****************************************************************************
* Copyright (C), 2010 ���������� www.armfly.com
*
* �ļ���: usart_api.h
* ���ݼ���: ����ģ��ͷ�ļ���
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2010-08-21 armfly  �������ļ�
*
*/

#ifndef _USART_API_H_
#define _USART_API_H_

#define  CPU_IntDis()           { __set_PRIMASK(1); }	/* ���ж� */
#define  CPU_IntEn()            { __set_PRIMASK(0); }	/* ���ж� */
	  


#define USART2_MAX_RECV_LEN		800					//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		200					//����ͻ����ֽ���
#define USART2_RX_EN 			1					//0,������;1,����. 

/* ȫ�ֲ��� */
typedef struct
{
	USART_TypeDef *uart;	/*  */	
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	uint16_t usTxWrite;		/* ���ͻ�����дָ�� */
	uint16_t usTxRead;		/* ���ͻ�������ָ�� */
	uint16_t usRxWrite;		/* ���ջ�����дָ�� */
	uint16_t usRxRead;		/* ���ջ�������ָ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�� */
	void (*ReciveNew)(void);	/* �����յ����ݵĻص�����ָ�� */
}UART_T;

/* ����˿ں� */
typedef enum
{
	COM1 = 0,	/* COM1��, RS232 */
	COM2 = 1,	/* COM2��, RS232 */
	COM3 = 2	/* COM3��, RS485 */
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
