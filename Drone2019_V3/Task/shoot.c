#include "shoot.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "drv_tim.h"
#include "motor.h"
#include "dbus.h"
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern struct RC remoteControl;

struct M2006 toggleBullet;
int pwm_value = 0;
int pwm_set_value = 245;
static void shoot_config(void);
void shoot(void const * argument)
{
	shoot_config();
	osDelay(15);
	for(;;)
	{	
		if(remoteControl.dbus.msg.s1 != 2)
		{
			if(remoteControl.dbus.msg.s1 == 3)
			{//key mouse mode 
				if(remoteControl.dbus.msg.left == 1)
				{
					pwm_value = pwm_set_value;
					toggleBullet.setPoint = 7500;
				}
				else 
				{
					toggleBullet.setPoint = 0;
				}
				pwm_value = pwm_set_value;
			}
			else if(remoteControl.dbus.msg.s1 == 1)
			{//RC mode
				if(remoteControl.dbus.msg.s2 == 1)
				{
					pwm_value = pwm_set_value;
					toggleBullet.setPoint = 5000;
				}
				else if(remoteControl.dbus.msg.s2 == 3)
				{
					pwm_value = pwm_set_value;
					toggleBullet.setPoint = 0;
				}
				else if(remoteControl.dbus.msg.s2 == 2)
				{
					pwm_value = 200;
					toggleBullet.setPoint = 0;
				}
			}			
			friction_set_value( pwm_value);
			CAN_SendData(&hcan1,0x200,M2006_control(&toggleBullet));
			osDelay(1);
		}

	}
}
static void shoot_config(void)
{
	toggleBullet.zero.CountCycle = 8191; 
	toggleBullet.mode = speedLoop;
	toggleBullet.receiveId = 0x201;
	PID_struct_init(&toggleBullet.speedPID,1,8000,500,4.0f,0.2f,1.5f);
	PID_struct_init(&toggleBullet.positionPID,1,8000,500,0.22f,0.05f,0.0f);
	pwm_value = 280;
}
