#ifndef __DRIVER_ADC_H
#define __DRIVER_ADC_H

#include "main.h"

/*
 *  函数名：int GetAdcAnlogValue(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：返回采样到的模拟电压
 *  函数作用：获取ADC的采样到的内部温度
 *
 *  备注：此处参考电压为3.3V
*/
extern int GetTemptureValue(void);

#endif
