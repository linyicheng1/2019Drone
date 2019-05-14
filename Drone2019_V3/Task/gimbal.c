#include "gimbal.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "motor.h"
#include "dbus.h"
#include "selfcheck.h"
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern struct RC remoteControl;

struct GM6020 yaw;
struct MX64 pitch;
static void gimbal_config(void);
float temp_k = 1;
void gimbal(void const * argument)
{
	gimbal_config();
	for(;;)
	{
		if(remoteControl.dbus.msg.s1 != 2)
		{
			if(remoteControl.dbus.msg.s1 == 3)
			{//key mouse mode 
				yaw.setPoint += (short)remoteControl.dbus.msg.X*temp_k;
				pitch.MX64_sendData.setPoint += (short)remoteControl.dbus.msg.Y*temp_k	;
			}
			else if(remoteControl.dbus.msg.s1 == 1)
			{//RC mode
				yaw.setPoint += (remoteControl.dbus.msg.chan3 - 1024)*0.02f;
				pitch.MX64_sendData.setPoint -=  (remoteControl.dbus.msg.chan4 - 1024)*0.005f;
			}			
			if(pitch.MX64_sendData.setPoint>750)
				pitch.MX64_sendData.setPoint = 750;
			else if(pitch.MX64_sendData.setPoint<220)
				pitch.MX64_sendData.setPoint = 220;
			if(yaw.setPoint>4000)
				yaw.setPoint = 4000;
			else if(yaw.setPoint<1500)
				yaw.setPoint = 1500;
			CAN_SendData(&hcan1,0x1FF,GM6020_control(&yaw));
			send_MX64_position(pitch.MX64_sendData.setPoint);
			send_MX64_action();	
		}
		osDelay(1);
	}
}

static void gimbal_config(void)
{
	/**********yaw***********/
	yaw.id = 0;
	yaw.receiveId = 0x207;
	ZeroCheck_Init_Interface(&yaw.zero,8191);
	yaw.mode = doubleLoop;
	yaw.setPoint = 2600;
	PID_struct_init(&yaw.positionPID,1,30000.0f,80.0f,0.1f,0.001f,0.05f);
	PID_struct_init(&yaw.speedPID,1,30000.0f,80.0f,300.0f,10.0f,10.0f);
	pitch.MX64_sendData.setPoint = 500;
}
