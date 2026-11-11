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
 *  函数名：void HAL_GPIO_EXTI_Callback(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：外部中断处理函数的回调函数，用以处理不同引脚触发的中断服务最终函数
*/
static volatile bool key1_flag = false;           // 定义一个全局静态标志，用以判断按键按下的次数
static volatile bool key2_flag = false;
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case KEY_USER1_Pin:
		{
			key1_flag = !key1_flag;
			if(key1_flag)
			{
				LED_GREEN_ON();
			}
			else
			{
				LED_GREEN_OFF();
			}
			break;
		}

		case KEY_USER2_Pin:
		{
			key2_flag = !key2_flag;
			if(key2_flag)
			{
				LED_YELLOW_ON();
			}
			else
			{
				LED_YELLOW_OFF();
			}
			break;
		}
		default:break;
	}
}
