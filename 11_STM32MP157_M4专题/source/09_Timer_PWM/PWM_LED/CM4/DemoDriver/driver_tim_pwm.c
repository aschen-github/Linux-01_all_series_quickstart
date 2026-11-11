#include "driver_tim_pwm.h"
#include "tim.h"

/*
 *	函数名：void TIM_LED3_PWM_Start(void)
 *	输入参数：无
 *	输出参数：无
 *	函数作用：开启定时器
*/
void TIM_LED3_PWM_Start(void)
{
	if(HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

/*
 *	函数名：void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 *	输入参数：htim->定时器句柄
 *	输出参数：无
 *	函数作用：每当发生一次周期溢出时间会到此中断回调函数，在此函数内更新占空比
*/
static uint16_t pulse = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
    	pulse = pulse + 1;
    	if(pulse>=1000)
    	{
    		pulse = 0;
    	}
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
    }
}
