/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 *
 * 文件名称：driver_systick.h
 * 摘要：
 *
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_SYSTICK_H
#define __DRIVER_SYSTICK_H

#include "main.h"
#include "stm32mp1xx_hal.h"


/*
 *  函数名：void SysTickInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化系统滴答时钟的频率和中断优先级
*/
extern void SysTickInit(void);

/*
 *  函数名：void SysTickDelay(uint16_t u)
 *  输入参数：u-延时时间
 *  输出参数：无
 *  返回值：无
 *  函数作用：滴答定时器实现的延时函数
*/
extern void SysTickDelay(uint32_t u);

/*
 *  函数名：void OwnSysTick_Handler(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：滴答定时器的中断服务函数
*/
void OwnSysTick_Handler(void);

#endif
