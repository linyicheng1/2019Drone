#ifndef __DRV_CAN_H__
#define __DRV_CAN_H__
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_can.h"
#include "board.h"
struct can_manage_obj
{
    uint8_t tx_buffer[8];
	uint8_t rx_buffer_20x[8][8];//id=20x  x=1,2,3,,,8
};
void CAN_receiveData_hook(uint8_t can );
void CAN_sendData(uint8_t *tx_bufferPrt,uint8_t can,uint32_t sendId);
unsigned char CAN1_Send_Msg(unsigned char* msg,unsigned char len);
uint8_t *CAN_getData(uint8_t can);
void CanTransmit_1234(CAN_HandleTypeDef *hcanx,int16_t cml_iq);
HAL_StatusTypeDef CanFilterInit(CAN_HandleTypeDef* hcan);
#endif // __DRV_CAN_H__
