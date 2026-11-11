#include "driver_adc.h"
#include "adc.h"
#include "dma.h"

volatile uint8_t conv_flag = 0;

void AdcStart(void)
{
	volatile uint16_t temp[100] = {0};
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)temp, 100);
}

/*
void GetAdcValue(uint16_t *pdatain, float *pdataout, uint16_t sz)
 *  输入参数：sz-采样点数
 *  输出参数：pdatain-采样的数据存储的首地址
 *  		 pdataout-电压转换至存储的首地址
 *  返回值：无
 *  函数作用：获取ADC的采样到的模拟电压
 *
 *  备注：此处参考电压为3.3V
*/
void GetAdcValue(uint16_t *pdatain, float *pdataout, uint16_t sz)
{
	uint16_t i = 0;
	conv_flag = 0;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)pdatain, sz);
	while(conv_flag != 1);

	for(i=0; i<sz; i++)
	{
		pdataout[i] = pdatain[i] * 3.3 / 65536;
	}
}

/*
 *  函数名：void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
 *  输入参数：hadc->adc句柄
 *  输出参数：无
 *  函数作用：每当ADC转换完成会到此中断回调函数，在此函数内更新完成标志
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance==ADC1)
    {
        conv_flag = 1;
    }
}
