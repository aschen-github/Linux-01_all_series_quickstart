#ifndef __DRIVER_DAC_H
#define __DRIVER_DAC_H

#include "main.h"

// 定义参考电压
#define VREF_VALUE  3.3
/*
 *  函数名：void DAC_Start(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：启动DAC
 *
*/
extern void DAC_Start(void);

/*
 *  函数名：void DAC_Stop(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：停止DAC_OUT2
 *
*/
void DAC_Stop(void);

/*
 *  函数名：int8_t DAC_SetValue(uint16_t data)
 *  输入参数：无
 *  输出参数：无
 *  返回值：0:正确 -1:超过设置范围 -2:设置错误
 *  函数作用：设置DAC输出值
 *
 */
extern int8_t DAC_SetValue(uint16_t data);


/*
 *  函数名：void DAC_SetVoltage(float vol)
 *  输入参数：需要设置的输出电压值
 *  输出参数：无
 *  返回值：无
 *  函数作用：设置DAC输出值
 *
 *注意:参考电压为VREF_VALUE 3.3V
 */
void DAC_SetVoltage(float vol);

#endif
