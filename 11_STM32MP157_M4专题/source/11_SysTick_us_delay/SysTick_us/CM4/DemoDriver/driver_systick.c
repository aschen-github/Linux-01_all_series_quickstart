/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_systick.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include "driver_systick.h"

/*
 * 全局变量定义
*/

static volatile uint32_t systick_t = 0;

/*
 *  函数名：void SysTickInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化系统滴答时钟的频率和中断优先级
*/
void SysTickInit(void)
{
    uint32_t init_t = 0;

    init_t = SystemCoreClock/1000000;

	/* 时间(单位:s)=1/频率(单位:HZ)
	 * SystemCoreClock频率: 209MHz = 209,000,000
	 * 即MCU 1秒会计数209,000,000次
	 *       1us则计数 209MHz/1000000 = 209次
	 * 209就是滴答时钟的初始值,它向下计数209次,计数将变为0,就会产生一次中断
	 *
     * SystemCoreClock/1000:    1ms中断一次
     * SystemCoreClock/100000:  10us中断一次
     * SystemCoreClock/1000000: 1us中断一次
    */
    if(HAL_SYSTICK_Config(init_t) != HAL_OK)
    {
        Error_Handler();
    }

    // 设置滴答定时器中断优先级：最高
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    // 使能滴答定时器中断
    HAL_NVIC_EnableIRQ(SysTick_IRQn);
}

/*
 *  函数名：void SysTickDelay(uint16_t u)
 *  输入参数：u-延时时间
 *  输出参数：无
 *  返回值：无
 *  函数作用：滴答定时器实现的延时函数
*/
void SysTickDelay(uint32_t u)
{
    systick_t = u;
    while(systick_t != 0);
}

/*
 *  函数名：void OwnSysTick_Handler(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：滴答定时器的中断服务函数
*/
void OwnSysTick_Handler(void)
{
    if(systick_t != 0)
    {
        systick_t--;
    }
}
