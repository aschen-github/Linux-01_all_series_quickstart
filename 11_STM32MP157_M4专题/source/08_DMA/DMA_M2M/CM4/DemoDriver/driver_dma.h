#ifndef __DRIVER_DMA_H
#define __DRIVER_DMA_H

#include "main.h"

extern __IO uint32_t transferErrorDetected;
extern __IO uint32_t transferCompleteDetected;

/*
 *  函数名：void RegisterCallbackFunction(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：注册回调函数
*/
extern void RegisterCallbackFunction(void);

/*
 *  函数名：void DMA_M2M_Start(uint32_t *srcAddr, uint32_t *dstAddr, uint16_t bufsz
 *  输入参数：srcAddr-数据原地址
 *            dstAddr-DMA搬移的目的地址
 *            bufsz-DMA搬移的数据大小，单位是初始化的时候选择的：byte,half-word,word
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化DMA_Channel1，配置为内存-内存模式，每次搬移一个word即4bytes
*/
extern void DMA_M2M_Start(uint32_t *srcAddr, uint32_t *dstAddr, uint16_t bufsz);

#endif
