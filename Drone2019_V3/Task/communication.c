#include "string.h"
#include "FreeRTOS.h"
#include "communication.h"
#include "cmsis_os.h"
#include "usbd_custom_hid_if.h"
#include "pc.h"
#include "imu.h"
#include "judgment.h"
#include "dbus.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
PC_communication PC;
uint8_t u8_send = 0;
/*
 * 0x00 无飞机
 * 0x01 有飞机
 * 
 * */
void communication(void const * argument)
{
	for(;;)
  {
//		short send_angle_yaw = (short)(getYawAngle()*100.0f);
//		short send_angle_pitch = (short)(getPitchAngle()*100.0f);
//		PC.tx_buffer[0] = '!';
//		memcpy(&PC.tx_buffer[2],&send_angle_pitch,2);
//		memcpy(&PC.tx_buffer[4],&send_angle_yaw,2);
//		PC.tx_buffer[7] = '#';
//		USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,PC.tx_buffer,8);
//		PC_discode(&PC);
		if(getRCData()->dbus.msg.press_f == 1)
		{
			u8_send = 1;
		}
		if(getRCData()->dbus.msg.press_g == 1)
		{
			u8_send = 0;
		}
		sendMesg(0x06,0x07,u8_send);
		osDelay(1000);
  }
}
