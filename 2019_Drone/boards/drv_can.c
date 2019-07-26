/**
  * @file    drv_can.c
  * @author  lyc
  * @brief   can operations
  */
#include "can.h"
#include "drv_can.h"
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
struct can_manage_obj can1_manage;
struct can_manage_obj can2_manage;
uint8_t sendbuffer[8];
CAN_TxHeaderTypeDef can1_tx;
/**
  * @brief  CAN总线发送函数
  * @retval None
  */
uint8_t sendSucces = 0;
unsigned char CAN1_Send_Msg(unsigned char* msg,unsigned char len)
{	
	can1_tx.DLC = 8;
	can1_tx.ExtId=0x00002018;
	can1_tx.IDE = CAN_ID_STD;
	can1_tx.RTR = CAN_RTR_DATA;
	can1_tx.StdId = 0x018;
	
	if(HAL_CAN_AddTxMessage(&hcan1,&can1_tx,sendbuffer, (uint32_t *)CAN_TX_MAILBOX0)==HAL_OK)
	{
		sendSucces = 1;
	}
}
void CanTransmit_1234(CAN_HandleTypeDef *hcanx,int16_t cml_iq)
{
	CAN_TxHeaderTypeDef  TxMessage;
	TxMessage.DLC=0x08;
	TxMessage.StdId=0x200;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.RTR=CAN_RTR_DATA;
	uint8_t TxData[8];

	TxData[0] = (uint8_t)(cml_iq >> 8);//電壓給定高八位
	TxData[1] = (uint8_t)cml_iq;//電壓給定低八位
	if(HAL_CAN_AddTxMessage(hcanx,&TxMessage,TxData,(uint32_t*)CAN_TX_MAILBOX0)!=HAL_OK)
	{
		 Error_Handler();       //如果CAN資訊傳送失敗則進入死迴圈
	}
}
HAL_StatusTypeDef CanFilterInit(CAN_HandleTypeDef* hcan)
{
	CAN_FilterTypeDef  sFilterConfig;

	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if(hcan == &hcan1)
	{
		sFilterConfig.FilterBank = 0;
	}
	if(hcan == &hcan2)
	{
		sFilterConfig.FilterBank = 14;
	}

	if(HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_CAN_Start(hcan) != HAL_OK)
	{
		Error_Handler();
	}

}
/**
  * @brief  CAN总线接受中断调用函数
  * @retval None
  */
void CAN_receiveData_hook(uint8_t can )
{
//	struct can_manage_obj *can_manage;
//	CAN_HandleTypeDef *canPrt;
//	if(can==can1)
//	{
//		canPrt = &hcan1;
//		can_manage = &can1_manage;
//	}
//	else 
//	{
//		canPrt = &hcan2;
//		can_manage = &can2_manage;
//	}
//	for(int i=0;i<8;i++)
//	  can_manage->rx_buffer_20x[(int)(canPrt->pRxMsg->StdId-0x200)][i] = canPrt->pRxMsg->Data[i];
}
/**
  * @brief  获取当前can的数据
  * @retval None
  */
uint8_t *CAN_getData(uint8_t can)
{
	struct can_manage_obj *can_manage;
	if(can==can1)
		can_manage = &can1_manage;
	else 
		can_manage = &can2_manage;
	return 0;//can_manage->rx_buffer;
}
