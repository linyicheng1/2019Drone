#include "main.h"
#include "gimbal.h"
#include "motor.h"
#include "IMU.h"
#include "dbus.h"
extern DroneState drone_state;

struct M2006 pitch;
struct GM6020 yaw;
extern IMU imu;

void gimbal_init(void)
{
	/**********yaw***********/
	yaw.id = 0;
	yaw.receiveId = 0x207;
	ZeroCheck_Init_Interface(&yaw.zero,8191);
	yaw.mode = doubleLoop;
	
	PID_struct_init(&yaw.positionPID,1,30000.0f,80.0f,0.1f,0.001f,0.05f);
	PID_struct_init(&yaw.speedPID,1,30000.0f,80.0f,300.0f,10.0f,10.0f);

	/**********pitch***********/
    pitch.id = 1;
	pitch.receiveId = 0x203;
	ZeroCheck_Init_Interface(&pitch.zero,8191);
	pitch.mode = doubleLoop ;
	
	PID_struct_init(&pitch.speedPID,1,32767,500,4.0f,0.2f,1.5f);
	PID_struct_init(&pitch.positionPID,1,32767,500,0.22f,0.05f,0.0f);
	yaw.setPoint = yaw.Angle_zoroCheck;
	pitch.setPoint = pitch.Angle_zoroCheck;
}
short gimbal_delay;
float scale_yaw = 0.02;
float scale_pitch = 1;
void gimbal_tast(unsigned char model)
{
	//pitch  --> 60256 --> -60256
	
	switch (drone_state.drone_state)
	{
		case remote_control:
			if(getRCData().rc.s1 == 1||getRCData().rc.s1 == 2)
			{
				yaw.mode = doubleLoop;
				pitch.mode = doubleLoop ;
				yaw.setPoint += (getRCData().rc.ch2 - 1024)*scale_yaw ;
				pitch.setPoint += (getRCData().rc.ch3 - 1024)*scale_pitch ;
			}
			else 
			{
				yaw.setPoint = yaw.Angle_zoroCheck;
				pitch.setPoint = pitch.Angle_zoroCheck;
			}
			break;
		case key_board_control:
			if(getRCData().rc.s1 == 1||getRCData().rc.s1 == 2)
			{
				yaw.mode = doubleLoop;
				pitch.mode = doubleLoop ;
				yaw.setPoint += (getRCData().mouse.x)*0.1 ;
				pitch.setPoint += (getRCData().mouse.y)*5 ;
			}
			else 
			{
				yaw.setPoint = yaw.Angle_zoroCheck;
				pitch.setPoint = pitch.Angle_zoroCheck;
			}
			break;
		case emergency_stop:

			break;
		default :
			break;
	}

}
