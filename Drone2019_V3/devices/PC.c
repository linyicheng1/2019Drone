#include "pc.h"
#include "string.h"

void PC_discode(PC_communication * PC)
{
	if(PC->rx_buffer[0] == '!')
	{
		short receive_pitch = 0;
		short receive_yaw = 0;
		memcpy(&receive_pitch,&PC->tx_buffer[2],2);
		memcpy(&receive_yaw,&PC->tx_buffer[4],2);
		PC->set_pitch_angle = (float)receive_pitch/100.0f;
		PC->set_yaw_angle = (float)receive_yaw/100.0f;
	}
}
/*
 * @brief  PC poweroff
 * */
void PC_poweroff(void)
{
	
}

void PC_code(PC_communication * PC)
{
	
}
 
 