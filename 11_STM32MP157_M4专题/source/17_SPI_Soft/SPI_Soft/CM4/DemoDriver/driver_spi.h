/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 *
 * 文件名称：driver_spi.h
 * 摘要：
 *
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H

#include "main.h"

/************************* SPI 硬件相关定义 *************************/

#define SPI_CLK(level)                   HAL_GPIO_WritePin(SPI5_CLK_GPIO_Port, SPI5_CLK_Pin, level&GPIO_PIN_SET)
#define SPI_MISO()                       HAL_GPIO_ReadPin(SPI5_MISO_GPIO_Port, SPI5_MISO_Pin)
#define SPI_MOSI(bit)                    HAL_GPIO_WritePin(SPI5_MOSI_GPIO_Port, SPI5_MOSI_Pin, bit&GPIO_PIN_SET)
#define SPI_CS(level)                    HAL_GPIO_WritePin(SPI5_NSS_GPIO_Port, SPI5_NSS_Pin, level&GPIO_PIN_SET)
/************************* SPI 硬件相关定义结束 *************************/
/*
 *  函数名：void SPI_Init(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化SPI的四根引脚
*/
extern void SPI_Init(void);

/*
 *  函数名：uint8_t SPI_WriteReadByte(uint8_t pdata)
 *  输入参数：pdata -> 要写的一个字节数据
 *  输出参数：无
 *  返回值：读到的数据
 *  函数作用：模拟SPI读写一个字节
*/
extern uint16_t SPI_WriteReadByte(uint16_t pdata);

/*
 *  函数名：void SPI_WriteByte(uint8_t data)
 *  输入参数：data -> 要写的数据
 *  输出参数：无
 *  返回值：无
 *  函数作用：模拟SPI写一个字节
*/
extern void SPI_WriteByte(uint8_t data);

/*
 *  函数名：uint8_t SPI_ReadByte(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：读到的数据
 *  函数作用：模拟SPI读一个字节
*/
extern uint8_t SPI_ReadByte(void);

#endif  //__DRIVER_SPI_H
