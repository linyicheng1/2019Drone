#include "drv_can.h"
#include "main.h"
extern DroneState drone_state;
CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;
extern struct M2006 toggleBullet;
extern struct GM6020 yaw;
extern struct M2006 pitch;
uint8_t CanReceiveBuffer[5][8];//according can num
void CAN_SendData(CAN_HandleTypeDef *hcan,uint32_t id,uint8_t *sendBuff)
{
	CanTxMsg tx_message;
    tx_message.StdId = id;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    for(int i=0;i<8;i++)
	{
		hcan->SendBuffer[i] = sendBuff[i];
	}
    tx_message.Data[0] = hcan->SendBuffer[0];
    tx_message.Data[1] = hcan->SendBuffer[1];
    tx_message.Data[2] = hcan->SendBuffer[2];
    tx_message.Data[3] = hcan->SendBuffer[3];
    tx_message.Data[4] = hcan->SendBuffer[4];
    tx_message.Data[5] = hcan->SendBuffer[5];
    tx_message.Data[6] = hcan->SendBuffer[6];
    tx_message.Data[7] = hcan->SendBuffer[7];
	if(drone_state.drone_state != emergency_stop)
	{
		if(hcan->UniqueID == 1)
			CAN_Transmit(CAN1,&tx_message);
		else if(hcan->UniqueID == 2)
			CAN_Transmit(CAN2,&tx_message);
	}
}
uint8_t* getCan1Msg(uint8_t num)
{
	return CanReceiveBuffer[num];
}
void Can1Receive_Hook(CanRxMsg *rxMsg)
{
	switch(rxMsg->StdId)
	{
		case 0x201://²¦µ¯µç»ú M2006
			 for(int i=0;i<8;i++)
		        CanReceiveBuffer[0][i] = rxMsg->Data[i];
				CAN_SendData(&hcan1,0x200,M2006_control(&toggleBullet,CanReceiveBuffer[0]));
				
			break;
		case 0x207://yaw 6020
			for(int i=0;i<8;i++)
				CanReceiveBuffer[4][i] = rxMsg->Data[i];
				CAN_SendData(&hcan1,0x1FF,GM6020_control(&yaw,CanReceiveBuffer[4]));
			break;
		default :
			break;
	}
}
void Can2Receive_Hook(CanRxMsg *rxMsg,uint8_t fifox)
{
	if(fifox == 0)
	{//imu
		switch(rxMsg->StdId)
		{
			case 0x100:
				 for(int i=0;i<8;i++)
					CanReceiveBuffer[2][i] = rxMsg->Data[i];			
			case 0x101:
				for(int i=0;i<8;i++)
					CanReceiveBuffer[3][i] = rxMsg->Data[i];
					imuDiscode(CanReceiveBuffer[2],CanReceiveBuffer[3]);
				break;
			default :
				break;
	    }
	}
	else if(fifox == 1)
	{
		switch(rxMsg->StdId)
		{
			
			case 0x203://Pitch M2006
				for(int i=0;i<8;i++)
					CanReceiveBuffer[1][i] = rxMsg->Data[i];
					CAN_SendData(&hcan2,0x200,M2006_control(&pitch,CanReceiveBuffer[1]));
				break;	
			default :
				break;
	    }
	}
}
void CAN_Config(void)
{
	{//clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	}
	{//GPIO
		hcan1.CAN_RX.GPIO_Pin = CAN1_Rx_Pin;
		hcan1.CAN_TX.GPIO_Pin = CAN1_Tx_Pin;
		hcan2.CAN_RX.GPIO_Pin = CAN2_Rx_Pin;
		hcan2.CAN_TX.GPIO_Pin = CAN2_Tx_Pin;
		
		hcan1.CAN_RX.GPIO_Mode = GPIO_Mode_IPU;
		hcan1.CAN_TX.GPIO_Mode = GPIO_Mode_AF_PP;
		hcan2.CAN_RX.GPIO_Mode = GPIO_Mode_IPU;
		hcan2.CAN_TX.GPIO_Mode = GPIO_Mode_AF_PP;
		
		hcan1.CAN_RX.GPIO_Speed = GPIO_Speed_50MHz;
		hcan1.CAN_TX.GPIO_Speed = GPIO_Speed_50MHz;
		hcan2.CAN_RX.GPIO_Speed = GPIO_Speed_50MHz;
		hcan2.CAN_TX.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(CAN1_Rx_GPIO_Port, &hcan1.CAN_RX);
		GPIO_Init(CAN1_Tx_GPIO_Port, &hcan1.CAN_TX);
		GPIO_Init(CAN2_Rx_GPIO_Port, &hcan2.CAN_RX);
		GPIO_Init(CAN2_Tx_GPIO_Port, &hcan2.CAN_TX);
		GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);  // CAN1 remap
	}
	{//NVIC
		hcan1.nvic.NVIC_IRQChannel=CAN1_RX0_IRQn;
		hcan1.nvic.NVIC_IRQChannelPreemptionPriority = 0;
		hcan1.nvic.NVIC_IRQChannelSubPriority = 0;
		hcan1.nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&hcan1.nvic);	
		hcan2.nvic.NVIC_IRQChannel=CAN2_RX1_IRQn;
		hcan2.nvic.NVIC_IRQChannelPreemptionPriority = 0;
		hcan2.nvic.NVIC_IRQChannelSubPriority = 0;
		hcan2.nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&hcan2.nvic);	
		hcan2.nvic.NVIC_IRQChannel=CAN2_RX0_IRQn;
		hcan2.nvic.NVIC_IRQChannelPreemptionPriority = 0;
		hcan2.nvic.NVIC_IRQChannelSubPriority = 0;
		hcan2.nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&hcan2.nvic);	
	}
	{//CAN
		CAN_DeInit(CAN1);
		CAN_DeInit(CAN2);
		CAN_StructInit(&hcan1.Init);   
        CAN_StructInit(&hcan2.Init);
		hcan1.Init.CAN_TTCM=DISABLE;
		hcan1.Init.CAN_ABOM=ENABLE;
		hcan1.Init.CAN_AWUM=ENABLE;
		hcan1.Init.CAN_NART=ENABLE;
		hcan1.Init.CAN_RFLM=DISABLE;
		hcan1.Init.CAN_TXFP=ENABLE;
		hcan1.Init.CAN_Mode=CAN_Mode_Normal;   
		hcan1.Init.CAN_SJW=SET_CAN_SJW;
		hcan1.Init.CAN_BS1=SET_CAN_BS1;  
		hcan1.Init.CAN_BS2=SET_CAN_BS2;	
		hcan1.Init.CAN_Prescaler=SET_CAN_PRES;
        CAN_Init(CAN1,&hcan1.Init);	// CAN1	
        
		hcan2.Init.CAN_TTCM=DISABLE;
		hcan2.Init.CAN_ABOM=ENABLE;
		hcan2.Init.CAN_AWUM=ENABLE;
		hcan2.Init.CAN_NART=ENABLE;
		hcan2.Init.CAN_RFLM=DISABLE;
		hcan2.Init.CAN_TXFP=ENABLE;
		hcan2.Init.CAN_Mode=CAN_Mode_Normal;   
		hcan2.Init.CAN_SJW=SET_CAN_SJW;
		hcan2.Init.CAN_BS1=SET_CAN_BS1;  
		hcan2.Init.CAN_BS2=SET_CAN_BS2;	
		hcan2.Init.CAN_Prescaler=SET_CAN_PRES;
        CAN_Init(CAN2,&hcan2.Init);	// CAN2		
	}
	{//filter
		hcan1.filter.CAN_FilterNumber=0;	 
		hcan1.filter.CAN_FilterMode=CAN_FilterMode_IdMask;	
		hcan1.filter.CAN_FilterScale=CAN_FilterScale_32bit;   
		hcan1.filter.CAN_FilterIdHigh=0x0000;		
		hcan1.filter.CAN_FilterIdLow=0x0000;				
		hcan1.filter.CAN_FilterMaskIdHigh=0x0000;		
		hcan1.filter.CAN_FilterMaskIdLow=0x0000;
		hcan1.filter.CAN_FilterFIFOAssignment=CAN_FIFO0;	 
		hcan1.filter.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&hcan1.filter);

		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  // CAN1
		
		hcan2.filter.CAN_FilterNumber=16;// 
		hcan2.filter.CAN_FilterMode=CAN_FilterMode_IdMask;	 
		hcan2.filter.CAN_FilterScale=CAN_FilterScale_32bit;   
		hcan2.filter.CAN_FilterIdHigh=0x0000;			
		hcan2.filter.CAN_FilterIdLow=0x0000;				
		hcan2.filter.CAN_FilterMaskIdHigh=0x0000;		
		hcan2.filter.CAN_FilterMaskIdLow=0x0000;
		hcan2.filter.CAN_FilterFIFOAssignment=CAN_FIFO1;	
		hcan2.filter.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&hcan2.filter);

		hcan2.filter.CAN_FilterNumber=15;//imu
		hcan2.filter.CAN_FilterMode=CAN_FilterMode_IdList;	 
		hcan2.filter.CAN_FilterScale=CAN_FilterScale_32bit;  
		hcan2.filter.CAN_FilterIdHigh=0x100<<5;			
		hcan2.filter.CAN_FilterIdLow=0 | CAN_ID_STD;				
		hcan2.filter.CAN_FilterMaskIdHigh=0x101<<5;			
		hcan2.filter.CAN_FilterMaskIdLow=0 | CAN_ID_STD;
		hcan2.filter.CAN_FilterFIFOAssignment=CAN_FIFO0;	 
		hcan2.filter.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&hcan2.filter);
		CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);  // CAN2
		CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);  // CAN2
	}
	hcan1.UniqueID = 1;
    hcan2.UniqueID = 2;
}
