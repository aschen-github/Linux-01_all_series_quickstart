/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 *
 * 文件名称：driver_key.h
 * 摘要：
 *
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_KEY_H
#define __DRIVER_KEY_H

#include "main.h"

/*********************
 * 按键引脚状态定义
**********************/
#define PUSH_DOWN                       GPIO_PIN_RESET
#define SPRING_UP                       GPIO_PIN_SET

/*********************
 * 引脚宏定义
**********************/

/*********************
 * 按键状态读取函数宏定义
**********************/
#define READ_KEY1                  		HAL_GPIO_ReadPin(KEY_USER1_GPIO_Port, KEY_USER1_Pin)
#define READ_KEY2                		HAL_GPIO_ReadPin(KEY_USER2_GPIO_Port, KEY_USER2_Pin)


/*
 *  函数名：void Key1Polling(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：使用轮询方式查询键是否按下，通过按下控制灯亮灭
*/
extern void Key1Polling(void);

/*
 *  函数名：void Key2Polling(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：使用轮询方式查询键是否按下，通过按下控制灯亮灭
*/
extern void Key2Polling(void);

#endif
