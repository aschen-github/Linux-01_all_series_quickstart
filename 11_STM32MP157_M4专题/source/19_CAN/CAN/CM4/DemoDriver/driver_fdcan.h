#ifndef __DRIVER_FDCAN_H
#define __DRIVER_FDCAN_H

#include "main.h"

typedef struct
{
	uint16_t ID;
	uint8_t length;
	uint8_t RX_Data[16];
}CAN_RX_DATA;

extern CAN_RX_DATA can_rx_data;
extern void HAL_FDCAN_SetFilter(void);
extern void CAN_Transmit(uint16_t ID, uint8_t *pdata, uint8_t length);
extern int8_t CAN_Receive(void);
extern void CAN_SetReceiveFlag(uint8_t flag);

#endif
