/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_led.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include "driver_led.h"

void DemoLedInit(void)
{
	/* LED对应的引脚的初始化,已经由STM32CubeMX在main.c自动生成 */

	/* 两个灯初始化状态为亮 */
	LED_GREEN_ON();
	LED_YELLOW_ON();
}

void LedBlinking(void)
{
	/* LED2(GREEN)和LED3(YELLOW)间隔1S闪烁  */
	LED_GREEN_ON();
	LED_YELLOW_ON();
	HAL_Delay(1000);

	LED_GREEN_OFF();
	LED_YELLOW_OFF();
	HAL_Delay(1000);
}
