#include "driver_dma.h"
#include "dma.h"

__IO uint32_t transferErrorDetected;
__IO uint32_t transferCompleteDetected;

static void TransferComplete(DMA_HandleTypeDef *DmaHandle);
static void TransferError(DMA_HandleTypeDef *DmaHandle);

/*
 *  函数名：void RegisterCallbackFunction(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：注册回调函数
*/
void RegisterCallbackFunction(void)
{
	  // 注册传输完成和传输错误回调函数
	  HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
	  HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

	  // 配置DMA的中断优先级及其使能(在dma.c的MX_DMA_Init里自动生成)
	  // HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	  // HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/*
 *  函数名：void TransferComplete(DMA_HandleTypeDef *DmaHandle)
 *  输入参数：DmaHandle-DMA句柄
 *  输出参数：无
 *  返回值：无
 *  函数作用：如果DMA传输完成且不发生错误，则在此函数将传输完成标志置一
*/
static void TransferComplete(DMA_HandleTypeDef *DmaHandle)
{
  transferCompleteDetected = 1;
}

/*
 *  函数名：void TransferError(DMA_HandleTypeDef *DmaHandle)
 *  输入参数：DmaHandle-DMA句柄
 *  输出参数：无
 *  返回值：无
 *  函数作用：如果DMA传输过程中发生错误，则在此函数中将传输错误标志置一
*/
static void TransferError(DMA_HandleTypeDef *DmaHandle)
{
  transferErrorDetected = 1;
}

/*
 *  函数名：void DMA_M2M_Start(uint32_t *srcAddr, uint32_t *dstAddr, uint16_t bufsz
 *  输入参数：srcAddr-数据原地址
 *            dstAddr-DMA搬移的目的地址
 *            bufsz-DMA搬移的数据大小，单位是初始化的时候选择的：byte,half-word,word
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化DMA_Channel1，配置为内存-内存模式，每次搬移一个word即4bytes
*/
void DMA_M2M_Start(uint32_t *srcAddr, uint32_t *dstAddr, uint16_t bufsz)
{
    if (HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)srcAddr, (uint32_t)dstAddr, bufsz) != HAL_OK)
    {
        Error_Handler();
    }
}

