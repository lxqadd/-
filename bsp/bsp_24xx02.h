/*
*********************************************************************************************************
*	                                  
*	模块名称 : 串行EEPROM 24xx02驱动模块
*	文件名称 : bsp_24xx02.h
*	版    本 : V1.0
*	说    明 : 头文件
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-08-21 armfly  ST固件库升级到V3.5.0版本。
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#ifndef _BSP_24C02_H
#define _BSP_24C02_H

#include <inttypes.h>

#define EE_DEV_ADDR			0xA0		/* 24xx02的设备地址 */
#define EE_PAGE_SIZE		8			/* 24xx02的页面大小 */
#define EE_SIZE				256			//用到16字节，/* 24xx02总容量 */
#define DATA_LEN			8			//存放秒数的字节数

uint8_t ee_CheckOk(void);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);

#endif
