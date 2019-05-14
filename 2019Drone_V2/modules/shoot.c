#include "main.h"
#include "shoot.h"
#include "motor.h"
#include "dshot.h"
#include "drv_tim.h"
#include "dbus.h"
extern DroneState drone_state;
struct M2006 toggleBullet;

void shoot_init(void)
{
	/*******friction wheel**********/
	setDshotValue(0);
	/*******toggleBullet**********/
	toggleBullet.zero.CountCycle = 8191; 
	toggleBullet.mode = doubleLoop;
	toggleBullet.receiveId = 0x201;
	PID_struct_init(&toggleBullet.speedPID,1,8000,500,4.0f,0.2f,1.5f);
	PID_struct_init(&toggleBullet.positionPID,1,8000,500,0.22f,0.05f,0.0f);

}
short shoot_delay = 0;
short shoot_speed = 500;
short toggleBullet_delay = 0;
void shoot_task(unsigned char model)
{
	switch (drone_state.drone_state)
	{
		case remote_control:
			if(getRCData().rc.s1 == 1)
			{
				toggleBullet_delay = 0;
				toggleBullet.mode = speedLoop;
				toggleBullet.setPoint = 0;
				setDshotValue(0);
//				toggleBullet.mode = doubleLoop;
//				if(toggleBullet_delay>400)
//				{
//					toggleBullet.setPoint = toggleBullet.Angle_zoroCheck +42362;
//					toggleBullet_delay = 0;
//					setDshotValue(shoot_speed);
//				}
//				toggleBullet_delay ++;
			}
			else if(getRCData().rc.s1 == 3)
			{
				toggleBullet_delay = 0;
				toggleBullet.mode = speedLoop;
				toggleBullet.setPoint = 3000;
				setDshotValue(shoot_speed);
			}
			else 
			{
				toggleBullet_delay = 0;
				toggleBullet.mode = speedLoop;
				toggleBullet.setPoint = 0;
				setDshotValue(0);
			}
			break;
		case key_board_control:
			if(getRCData().key.a_ == 1)
			{
				toggleBullet.mode = speedLoop;
				toggleBullet.setPoint = 3000;
			}
			else if(getRCData().key.q_ == 1)
			{
				toggleBullet.mode = speedLoop;
				toggleBullet.setPoint = 0;
			}
			if(getRCData().key.s_  == 1)
			{
				setDshotValue(shoot_speed);
			}
			else if(getRCData().key.w_ == 1)
			{
				setDshotValue(0);
			}
			break;
		case emergency_stop:
			toggleBullet.setPoint = 0;
			break;
		default :
			break;
	}
}
