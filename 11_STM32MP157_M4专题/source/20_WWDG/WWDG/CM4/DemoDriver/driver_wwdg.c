#include "driver_wwdg.h"
#include "wwdg.h"

/*
 *  函数名：void ClearIWDG(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：刷新独立看门狗的计数器，俗称"喂狗"
*/
void ClearWWDG(void)
{
    if (HAL_WWDG_Refresh(&hwwdg1) != HAL_OK)
    {
        Error_Handler();
    }
}

uint32_t TimeoutCalculation(uint32_t timevalue)
{
  uint32_t timeoutvalue = 0;
  uint32_t pclk1 = 0;
  uint32_t wdgtb = 0;

  /* considering APB divider is still 1, use HCLK value */
  pclk1 = HAL_RCC_GetPCLK1Freq();

  /* get prescaler */
  wdgtb = (1 << ((hwwdg1.Init.Prescaler) >> WWDG_CFR_WDGTB_Pos)); /* 2^WDGTB[1:0] */

  /* calculate timeout */
  timeoutvalue = ((4096 * wdgtb * timevalue) / (pclk1 / 1000));

  return timeoutvalue;
}
