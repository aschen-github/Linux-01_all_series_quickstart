#include "driver_fdcan.h"
#include "fdcan.h"

CAN_RX_DATA can_rx_data;
FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader;

/*
 * 此函数设置接收的过滤器
*/
void HAL_FDCAN_SetFilter(void)
{
	FDCAN_FilterTypeDef  sFilterConfig;
	/* 配置CAN的过滤器，此处全部接收，不做过滤 */
	sFilterConfig.IdType 		= FDCAN_STANDARD_ID;			// 标准帧
	sFilterConfig.FilterIndex 	= 0;
	sFilterConfig.FilterType 	= FDCAN_FILTER_DUAL;
	sFilterConfig.FilterConfig 	= FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = 0;	// ID1和ID2都等于0表示都进行接收不做过滤
	sFilterConfig.FilterID2 = 0;
	if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
	{
		Error_Handler();
	}

	// 激活接收FIFO
	if(HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, FDCAN_IT_TX_COMPLETE) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
 *  函数名：void CAN_Transmit(uint16_t ID, uint8_t *pdata, uint8_t length)
 *  输入参数：ID    -> CAN发送报文的ID
 *            pdata -> 发送报文的首地址
 *            length-> 发送报文的个数，最多8个字节
 *  输出参数：无
 *  返回值：无
 *  函数作用：CAN发送函数
*/
void CAN_Transmit(uint16_t ID, uint8_t *pdata, uint8_t length)
{
	TxHeader.Identifier 	= ID;					// 发送ID
	TxHeader.IdType 		= FDCAN_STANDARD_ID;	// 发送帧为标准ID
	TxHeader.TxFrameType 	= FDCAN_DATA_FRAME;		// 是数据帧
	TxHeader.DataLength 	= FDCAN_DLC_BYTES_8;	// 长度是8个字节
	TxHeader.ErrorStateIndicator 	= FDCAN_ESI_PASSIVE;
	TxHeader.BitRateSwitch 			= FDCAN_BRS_OFF;
	TxHeader.FDFormat 			= FDCAN_CLASSIC_CAN;// 传统CAN
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, pdata) != HAL_OK)
	{
		Error_Handler();
	}
}

volatile uint8_t rx_flag = 0;

void CAN_SetReceiveFlag(uint8_t flag)
{
	rx_flag = 0;
}

int8_t CAN_Receive(void)
{
	if(rx_flag==1)
	{
		rx_flag = 0;
		return 1;
	}

	return 0;
}

/*
 *  函数名：void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
 *  输入参数：CanHandle -> CAN句柄
 *  输出参数：无
 *  返回值：无
 *  函数作用：CAN接收回调函数
*/
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	/* Get RX message */
	if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, (uint8_t*)&can_rx_data.RX_Data[0]) != HAL_OK)
	{
		Error_Handler();
	}
	can_rx_data.ID = RxHeader.Identifier;
	can_rx_data.length = RxHeader.DataLength>>16;
	rx_flag = 1;
}
