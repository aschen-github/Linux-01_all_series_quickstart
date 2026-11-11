#ifndef __DRIVER_TIM_H
#define __DRIVER_TIM_H

#include "main.h"

/*
 *  函数名：void us_timer_delay(uint32_t t)
 *  输入参数：t-延时时间us(范围:0~21,474,836us)
 *  输出参数：无
 *  返回值：无
 *  函数作用：定时器实现的延时函数，延时时间为t us，为了缩短时间，函数体使用寄存器操作，用户可对照手册查看每个寄存器每位的含义
*/
extern void us_timer_delay(uint32_t t);

#endif
