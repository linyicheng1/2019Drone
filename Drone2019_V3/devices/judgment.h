#ifndef __JUDGMENT_H
#define __JUDGMENT_H	
#include "stm32f1xx_hal.h"

struct interactiveData_dat
{
	uint8_t SOF;//起始0xA5
	uint16_t dataLength;//数据帧的长度
	uint8_t seq;//包序号
	uint8_t CRC8;
	uint16_t cmd_id;
	/************************/
	uint16_t data_cmd_id;
	uint16_t sender_ID;
	uint16_t receiver_ID;
	uint8_t data;
	/************************/
	uint16_t CRC16;
} __attribute__ ((__packed__));
typedef struct InteractiveData
{
	union
	{	
		uint8_t  raw[16];
		struct interactiveData_dat dat;
	}data;
}InteractiveData;
void sendMesg(uint16_t u16_senderID,uint16_t u16_receiverID,uint8_t u8_data);
#endif 
