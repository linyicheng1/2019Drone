#include "judgment.h"
#include <string.h>
#include "algorithmOfCRC.h"
#include "algorithmOfCRC.h"

InteractiveData st_interactiveData;
extern UART_HandleTypeDef huart1;

void sendMesg(uint16_t u16_senderID,uint16_t u16_receiverID,uint8_t u8_data)
{
	st_interactiveData.data.dat.SOF = 0xA5;
	st_interactiveData.data.dat.dataLength = 7;
	st_interactiveData.data.dat.seq = 0x00;
	Append_CRC8_Check_Sum(st_interactiveData.data.raw,5);
	st_interactiveData.data.dat.cmd_id = 0x301;
	
	st_interactiveData.data.dat.data_cmd_id = 0x200;
	st_interactiveData.data.dat.sender_ID  = u16_senderID;
	st_interactiveData.data.dat.receiver_ID = u16_receiverID;
	st_interactiveData.data.dat.data = u8_data;
	Append_CRC16_Check_Sum(st_interactiveData.data.raw,16);
	HAL_UART_Transmit(&huart1,st_interactiveData.data.raw,16,100);
}























