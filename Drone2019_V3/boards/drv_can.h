#ifndef __DRV_CAN_H__
#define __DRV_CAN_H__
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_can.h"
#include "board.h"
void FilterConfig(void);
void CAN_SendData(CAN_HandleTypeDef *hcan,uint32_t id,uint8_t sendBuff[]);
void CAN_SendData2(CAN_HandleTypeDef *hcan,uint32_t id,uint8_t sendBuff1[],uint8_t sendBuff2[]);
#endif // __DRV_CAN_H__
