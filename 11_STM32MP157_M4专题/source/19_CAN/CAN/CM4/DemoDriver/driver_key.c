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

#include "driver_key.h"


extern uint8_t test_flag;
/*
 *  函数名：void HAL_GPIO_EXTI_Callback(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：外部中断处理函数的回调函数，用以处理不同引脚触发的中断服务最终函数
*/
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case KEY_USER1_Pin:
		{
			break;
		}

		case KEY_USER2_Pin:
		{
			test_flag = 1;
			break;
		}
		default:break;
	}
}
