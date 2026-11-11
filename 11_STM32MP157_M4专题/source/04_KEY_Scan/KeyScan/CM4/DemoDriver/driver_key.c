/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 *
 * 文件名称：driver_key.c
 * 摘要：
 *
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include <stdbool.h>
#include "driver_led.h"
#include "driver_key.h"

/*
 *  函数名：void Key1Polling(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：使用轮询方式查询键是否按下，通过按下控制灯亮灭
*/
static bool key1_flag = false;
void Key1Polling(void)
{
    if(READ_KEY1 == PUSH_DOWN)     // 如果检测到键被按下
    {
    	HAL_Delay(5);               //  延时5ms防按键抖动
        if(READ_KEY1 == PUSH_DOWN) // 如果防抖动后按键依然是处于被按下的状态，就认为按键被按下过
        {
        	key1_flag = !key1_flag; // 用一个标志位来判断按键被按下次数
        	if(key1_flag)
        	{
        		LED_GREEN_ON();
        	}
        	else
        	{
        		LED_GREEN_OFF();
        	}
        }
    }
}

/*
 *  函数名：void Key2Polling(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：使用轮询方式查询键是否按下，通过按下控制灯亮灭
*/
static bool key2_flag = false;
void Key2Polling(void)
{
    if(READ_KEY2 == PUSH_DOWN)     // 如果检测到键被按下
    {
        HAL_Delay(5);              // 延时5ms防按键抖动
        if(READ_KEY2 == PUSH_DOWN) // 如果防抖动后按键依然是处于被按下的状态，就认为按键被按下过
        {
        	key2_flag = !key2_flag; // 用一个标志位来判断按键被按下次数
        	if(key2_flag)
        	{
        		LED_YELLOW_ON();
        	}
        	else
        	{
        		LED_YELLOW_OFF();
        	}
        }
    }
}
