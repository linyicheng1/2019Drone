#ifndef __PC_H__
#define __PC_H__
#include "stm32f1xx_hal.h"

typedef struct PC_communication
{
	uint8_t rx_buffer[8];
	uint8_t tx_buffer[8];
	float set_pitch_angle;
	float set_yaw_angle;
	short shoot_flag;
	uint32_t updateTime;
}PC_communication;
void PC_discode(PC_communication * PC);
#endif // __PC_H__
