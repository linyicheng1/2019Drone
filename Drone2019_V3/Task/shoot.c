#include "shoot.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "drv_tim.h"
#include "motor.h"
#include "dbus.h"
#include "frictionWheel.h"
#include <stdlib.h>
#include "gpio.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

struct M2006 toggleBullet;
struct M2006 left_friction;
struct M2006 right_friction;

static void shoot_config(void);
static void shoot_2006_config(void);

float toggleBullet_speed = 1530;//拨弹电机速度 
/*
1000 36s
1500 24.8s
1530 22.94
1550 23
1600 24s 23.12
1800 31s
*/
int friction_value = -12000;//摩擦轮设定速度

void shoot_2006(void const * argument)
{
	shoot_2006_config();
	for(;;)
	{
		if(getRCData()->dbus.msg.s1 != 2)
		{
			if(getRCData()->dbus.msg.s1 == 3)
			{//key mouse mode
				right_friction.setPoint = -friction_value;
				left_friction.setPoint = friction_value;
				if(getRCData()->dbus.msg.left == 1&&
					abs(right_friction.receiveSpeed)>abs(friction_value)*0.8&&
					abs(left_friction.receiveSpeed)>abs(friction_value)*0.8)
				{
					
					toggleBullet.setPoint += toggleBullet_speed;	
//					if(toggleBullet.setPoint -toggleBullet.Angle_zoroCheck>=4000)
//					{
//						toggleBullet.setPoint = toggleBullet.Angle_zoroCheck+4000;
//					}
				}	
			}
			else if(getRCData()->dbus.msg.s1 == 1)
			{//RC mode
				if(getRCData()->dbus.msg.s2 == 2)
				{
					right_friction.setPoint = 0;
					left_friction.setPoint = 0;
				}
				else if(getRCData()->dbus.msg.s2 == 3)
				{
						right_friction.setPoint = -friction_value;
						left_friction.setPoint = friction_value;
				}
				else if(getRCData()->dbus.msg.s2 == 1)
				{
//					if(abs(right_friction.receiveSpeed)>abs(friction_value)*0.8&&
//						abs(left_friction.receiveSpeed)>abs(friction_value)*0.8)
//					{//摩擦轮转起来之后才能转拨盘
						toggleBullet.setPoint += toggleBullet_speed;	
						right_friction.setPoint = -friction_value;
						left_friction.setPoint = friction_value;
//					  if(toggleBullet.setPoint -toggleBullet.Angle_zoroCheck>=4000)
//						{
//							toggleBullet.setPoint = toggleBullet.Angle_zoroCheck+4000;
//						}
//					}
				}
			}
			taskENTER_CRITICAL();//进入临界段
			CAN_SendData(&hcan2,0x200,M2006_control(&left_friction));			
			CAN_SendData2(&hcan1,0x200,M2006_control(&toggleBullet),M2006_control(&right_friction));		
			taskEXIT_CRITICAL();//退出临界段			
		}
		else 
		{//掉电
			osDelay(1);	
		}
		osDelay(1);	
	}
}
static void shoot_2006_config(void)
{
	toggleBullet.zero.CountCycle = 8191; 
	toggleBullet.mode = doubleLoop;
	toggleBullet.receiveId = 0x203;
	toggleBullet.setPoint = toggleBullet.receiveAngle;
	PID_struct_init(&toggleBullet.speedPID,1,16384,500,2.0f,0.2f,1.0f);
	PID_struct_init(&toggleBullet.positionPID,1,16384,500,0.1f,0.04f,0.0f);
	
	left_friction.zero.CountCycle = 8191; 
	left_friction.mode = speedLoop;
	left_friction.receiveId = 0x202;
	left_friction.setPoint = friction_value;
	PID_struct_init(&left_friction.speedPID,1,16384,500,28.0f,2.6f,4.0f);
	PID_struct_init(&left_friction.positionPID,1,16384,100,1.0f,0.05f,0.0f);
	
	right_friction.zero.CountCycle = 8191; 
	right_friction.mode = speedLoop;
	right_friction.receiveId = 0x201;
	right_friction.setPoint = -friction_value;
	PID_struct_init(&right_friction.speedPID,1,16384,500,28.0f,2.6f,4.0f);
	PID_struct_init(&right_friction.positionPID,1,16384,100,1.0f,0.05f,0.0f);
	
//	HAL_GPIO_WritePin(GPIOC, Laser_Pin, GPIO_PIN_SET);
}


