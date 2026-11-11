/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_rs485.h
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#ifndef __DRIVER_RS485_H
#define __DRIVER_RS485_H

#include "main.h"
#include "stm32mp1xx_hal.h"

/********  硬件相关宏定义 ********/
#define DE_TX()	HAL_GPIO_WritePin(RS485_CTL_GPIO_Port, RS485_CTL_Pin, GPIO_PIN_SET)
#define DE_RX()	HAL_GPIO_WritePin(RS485_CTL_GPIO_Port, RS485_CTL_Pin, GPIO_PIN_RESET)

/********  函数声明 ********/
extern void RS485_TX(uint8_t *pdata, uint8_t sz);
extern void RS485_RX(uint8_t *pdata, uint8_t sz);

extern uint8_t RS485_GetTxFlag(void);
extern uint8_t RS485_GetRxFlag(void);

#endif
