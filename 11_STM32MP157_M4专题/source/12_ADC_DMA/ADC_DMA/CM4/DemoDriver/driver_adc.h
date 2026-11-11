#ifndef __DRIVER_ADC_H
#define __DRIVER_ADC_H

#include "main.h"

extern void AdcStart(void);

/*
void GetAdcValue(uint16_t *pdatain, float *pdataout, uint16_t sz)
 *  输入参数：sz-采样点数
 *  输出参数：pdatain-采样的数据存储的首地址
 *  		pdataout-电压转换至存储的首地址
 *  返回值：无
 *  函数作用：获取ADC的采样到的模拟电压之
 *
 *  备注：此处参考电压为3.3V
*/
extern void GetAdcValue(uint16_t *pdatain, float *pdataout, uint16_t sz);

#endif
