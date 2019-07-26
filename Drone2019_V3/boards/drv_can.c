#include "drv_can.h"
#include "imu.h"
#include "motor.h"
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern struct M2006 toggleBullet;
extern struct M2006 left_friction;
extern struct M2006 right_friction;
extern struct GM6020 yaw;
//can2 采用两个邮箱接收数据
void FilterConfig(void)
{
	CAN_FilterTypeDef  sFilterConfig;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterFIFOAssignment = 0;
	
	sFilterConfig.FilterIdHigh = 0x0000;	
	sFilterConfig.FilterIdLow = 0x0000;		
	sFilterConfig.FilterMaskIdHigh = 0x0000;		
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//	sFilterConfig.FilterIdHigh = 0x100<<5;	
//	sFilterConfig.FilterIdLow = 0 | CAN_ID_STD;		
//	sFilterConfig.FilterMaskIdHigh = 0x202<<5;		
//	sFilterConfig.FilterMaskIdLow = 0 | CAN_ID_STD;
//	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
//	
//	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

	HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);
	/*******************************************/
//	sFilterConfig.FilterActivation = ENABLE;
//	sFilterConfig.FilterBank = 15;
//	sFilterConfig.FilterFIFOAssignment = 0;
//	
//	sFilterConfig.FilterIdHigh = 0x201 << 5;
//	sFilterConfig.FilterIdLow = 0x202 << 5;
//	sFilterConfig.FilterMaskIdHigh = 0x203 << 5;		
//	sFilterConfig.FilterMaskIdLow = 0x207 << 5;
//	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
//	
//	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;

//	HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);
	/*******************************************/
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.FilterBank = 1;
	sFilterConfig.FilterFIFOAssignment = 0;
	
	sFilterConfig.FilterIdHigh = 0x0000;	
	sFilterConfig.FilterIdLow = 0x0000;		
	sFilterConfig.FilterMaskIdHigh = 0x0000;		
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);

		
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO0_MSG_PENDING);
	
	HAL_CAN_Start(&hcan1);
	HAL_CAN_Start(&hcan2);

//	HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO1_MSG_PENDING);
}

void CAN_SendData(CAN_HandleTypeDef *hcan,uint32_t id,uint8_t sendBuff[])
{
	CAN_TxHeaderTypeDef CAN_TxMessage;
	CAN_TxMessage.DLC = 0x08;
	CAN_TxMessage.IDE = CAN_ID_STD;
	CAN_TxMessage.RTR = CAN_RTR_DATA;
	CAN_TxMessage.StdId = id;
	HAL_CAN_AddTxMessage(hcan,&CAN_TxMessage,sendBuff,(uint32_t*)CAN_TX_MAILBOX1);
}

void CAN_SendData2(CAN_HandleTypeDef *hcan,uint32_t id,uint8_t sendBuff1[],uint8_t sendBuff2[])
{
	CAN_TxHeaderTypeDef CAN_TxMessage;
	CAN_TxMessage.DLC = 0x08;
	CAN_TxMessage.IDE = CAN_ID_STD;
	CAN_TxMessage.RTR = CAN_RTR_DATA;
	CAN_TxMessage.StdId = id;
	uint8_t sendBuff[8];
	for(int i=0;i<8;i++)
	{
		sendBuff[i] = sendBuff1[i]|sendBuff2[i];
	}
	HAL_CAN_AddTxMessage(hcan,&CAN_TxMessage,sendBuff,(uint32_t*)CAN_TX_MAILBOX1);
}
/**
  * @brief  fifo0 接受中断的回调函数
  * @param  none
  * @retval none
  */
uint8_t data0[8];
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RxMessage1;
	if(hcan->Instance == CAN2)
	{
		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxMessage1,data0);
		if(RxMessage1.StdId == 0x100||RxMessage1.StdId == 0x101)
		{
			imuDiscode(data0,data0,RxMessage1.StdId - 0x100);
		}
		else if(RxMessage1.StdId == 0x202)
		{
			left_friction.updateTime = GetSysTimeMs();
			M2006_discode(&left_friction,data0);
		}
	}
	else if(hcan->Instance == CAN1)
	{
		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxMessage1,data0);
		if(RxMessage1.StdId == 0x207)//yaw 6020
		{
			GM6020_discode(&yaw,data0);
			yaw.updateTime = GetSysTimeMs();
		}
		else if(RxMessage1.StdId == 0x203)//拨弹电机 M2006
		{
			M2006_discode(&toggleBullet,data0);
			toggleBullet.updateTime = GetSysTimeMs();
		}
		else if(RxMessage1.StdId == 0x202)
		{
			M2006_discode(&left_friction,data0);
			left_friction.updateTime = GetSysTimeMs();
		}
		else if(RxMessage1.StdId == 0x201)
		{
			M2006_discode(&right_friction,data0);
			right_friction.updateTime = GetSysTimeMs();
		}
	}
}
/**
  * @brief  fifo0 接受中断的回调函数
  * @param  none
  * @retval none
  */
//uint8_t data1[8];
//void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	CAN_RxHeaderTypeDef RxMessage1;
//	if(hcan->Instance == CAN2)
//	{
//		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxMessage1,data1);
//		if(RxMessage1.StdId == 0x100||RxMessage1.StdId == 0x101)
//		{
//			imuDiscode(data1,data1,RxMessage1.StdId - 0x100);
//		}
//		else if(RxMessage1.StdId == 0x202)
//		{
//			M2006_discode(&left_friction,data0);
//			left_friction.updateTime = GetSysTimeMs();
//		}
//	}
//	else if(hcan->Instance == CAN1)
//	{
//		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxMessage1,data1);
//		if(RxMessage1.StdId == 0x207)//yaw 6020
//		{
//			GM6020_discode(&yaw,data1);
//			yaw.updateTime = GetSysTimeMs();
//		}
//		else if(RxMessage1.StdId == 0x201)//拨弹电机 M2006
//		{
//			M2006_discode(&toggleBullet,data1);
//			toggleBullet.updateTime = GetSysTimeMs();
//		}
//	}
//}
