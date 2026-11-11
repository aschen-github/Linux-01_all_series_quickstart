#include "driver_adc.h"
#include "adc.h"

/*
 *  函数名：int GetAdcAnlogValue(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：返回采样到的模拟电压
 *  函数作用：获取ADC的采样到的内部温度
 *
 *  备注：此处参考电压为3.3V
*/
int GetTemptureValue(void)
{
	uint32_t nData = 0;
	uint16_t nValue = 0;

	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, 100);  // 等待转换完成，超时时间为100ms
	nData = HAL_ADC_GetValue(&hadc2);

	// 157的参考电压为3.3V;采样位数是16bit;
	nValue = __HAL_ADC_CALC_TEMPERATURE(3300UL, nData, ADC_RESOLUTION_16B);

	return nValue;
}
