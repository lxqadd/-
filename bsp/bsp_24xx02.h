/*
*********************************************************************************************************
*	                                  
*	ģ������ : ����EEPROM 24xx02����ģ��
*	�ļ����� : bsp_24xx02.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-08-21 armfly  ST�̼���������V3.5.0�汾��
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#ifndef _BSP_24C02_H
#define _BSP_24C02_H

#include <inttypes.h>

#define EE_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */
#define EE_PAGE_SIZE		8			/* 24xx02��ҳ���С */
#define EE_SIZE				256			//�õ�16�ֽڣ�/* 24xx02������ */
#define DATA_LEN			8			//����������ֽ���

uint8_t ee_CheckOk(void);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);

#endif
