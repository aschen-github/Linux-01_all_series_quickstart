#include "driver_dac.h"
#include "dac.h"

/*
 *  函数名：void DAC_Start(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：启动DAC_OUT2
 *
*/
void DAC_Start(void)
{
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
}

/*
 *  函数名：void DAC_Stop(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：停止DAC_OUT2
 *
*/
void DAC_Stop(void)
{
	HAL_DAC_Stop(&hdac1, DAC_CHANNEL_2);
}

/*
 *  函数名：int8_t DAC_SetValue(uint16_t data)
 *  输入参数：无
 *  输出参数：无
 *  返回值：0:正确 -1:超过设置范围 -2:设置错误
 *  函数作用：设置DAC输出值
 *
 */
int8_t DAC_SetValue(uint16_t data)
{
	if(data>=4096)
	{
		return -1;
	}
	if(HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, data) != HAL_OK)
	{
		return -2;
	}
	DAC_Start(); // 软件触发,每次修改值都需要启动DAC

	return 0;
}

/*
 *  函数名：void DAC_SetVoltage(float vol)
 *  输入参数：需要设置的输出电压值
 *  输出参数：无
 *  返回值：无
 *  函数作用：设置DAC输出值
 *
 *注意:参考电压为VREF_VALUE 3.3V
 */
void DAC_SetVoltage(float vol)
{
	if (vol > VREF_VALUE)
	{
		printf("\r Set voltage out of range \n");
		return;
	}

	DAC_SetValue(vol*4095/VREF_VALUE);
}
