#include "gimbal.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "motor.h"
#include "dbus.h"
#include "selfcheck.h"
#include "pc.h"
#include "imu.h"
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

extern PC_communication PC;
struct GM6020 yaw;
struct MX64 pitch;

static void limit_max_min(float *value,float max,float min);
static void gimbal_config(void);
static void gimbal_imu_config(void);

/*
 *                     s1                       S2
 *              1 --> 遥控器      1 --> 停止 2 --> 开启摩擦轮 3 --> 转拨盘   
 *              3 --> 键盘鼠标    
 *              2 --> 停止
 *
 *            不同模式下云台的控制权
 *              1 --> 遥控器 
 *              3 --> 键盘 
 *              2 --> 掉电
 * */
 float yaw_offset = 0;

void gimbal_imu(void const *argument)
{
//	static 
	static float yaw_scale = -23.0f;
	
	static float set_pitch = 0;
	static float set_yaw = 0;

	gimbal_imu_config();
	for(;;)
	{
		if(getRCData()->dbus.msg.s1 != 2)
		{
			if(getRCData()->dbus.msg.s1 == 3)
			{//key mouse mode 
				set_pitch -= (float)getRCData()->dbus.msg.Y*0.01f;
				set_yaw -= (float)getRCData()->dbus.msg.X*0.01f;
				// 微调，一直有效
				set_pitch = set_pitch + (float)getRCData()->dbus.msg.press_w*0.01f 
					          - (float)getRCData()->dbus.msg.press_s*0.01f;
				set_yaw = set_yaw - (float)getRCData()->dbus.msg.press_d*0.01f
					          + (float)getRCData()->dbus.msg.press_a*0.01f;
			}
			else if(getRCData()->dbus.msg.s1 == 1)
			{//RC mode
				set_yaw -= (getRCData()->dbus.msg.chan3 - 1024)*0.0002f;
				set_pitch +=  (getRCData()->dbus.msg.chan4 - 1024)*0.0002f;		
			}
			/********** 限幅 **********/
			yaw_offset = (yaw.Angle_zoroCheck - motorCenter)/yaw_scale - getYawAngle();
			limit_max_min(&set_pitch,pitch_Max,pitch_Min);
			limit_max_min(&set_yaw,-yaw_offset+yaw_Max,-yaw_offset+yaw_Min);
	    yaw.setPoint = set_yaw;
			pitch.MX64_sendData.setPoint = (short)(set_pitch*pitch_scale + pitch_offset);
			/********** send data **********/
//			taskENTER_CRITICAL();//进入临界段
			CAN_SendData(&hcan1,0x1FF,GM6020_control(&yaw));
			send_MX64_position(pitch.MX64_sendData.setPoint);
			send_MX64_action();	
//			taskEXIT_CRITICAL();//退出临界段
		}
		else 
		{//掉电
			
		}
		osDelay(1);
	}
}
float set_pitch = 0;
float set_yaw = 0;
void gimbal(void const * argument)
{

	gimbal_config();
	for(;;)
	{
		if(getRCData()->dbus.msg.s1 != 2)
		{// 遥控器s1 == 2 则不控制处于掉电状态
			if(getRCData()->dbus.msg.s1 == 3)
			{//key mouse mode 
				set_yaw += (short)getRCData()->dbus.msg.X*0.25;
				set_pitch -= (short)getRCData()->dbus.msg.Y*0.15;
				// 微调，一直有效
				set_pitch = set_pitch + (float)getRCData()->dbus.msg.press_w*0.1f 
					          - (float)getRCData()->dbus.msg.press_s*0.1f;
				set_yaw = yaw.setPoint + (float)getRCData()->dbus.msg.press_d*0.1f
					          - (float)getRCData()->dbus.msg.press_a*0.1f;
			}
			else if(getRCData()->dbus.msg.s1 == 1)
			{//RC mode
					set_yaw += (getRCData()->dbus.msg.chan3 - 1024)*0.02f;
					set_pitch +=  (getRCData()->dbus.msg.chan4 - 1024)*0.005f;					
			}
			// 限制幅度与pid控制
			limit_max_min(&set_pitch,800.0f,220.0f);
			limit_max_min(&set_yaw,4700.0f,1250.0f);
			pitch.MX64_sendData.setPoint = (short)set_pitch;
			yaw.setPoint = set_yaw;
			/********** send data **********/
//			taskENTER_CRITICAL();//进入临界段
//			CAN_SendData(&hcan1,0x1FF,GM6020_control(&yaw));
//			send_MX64_position(pitch.MX64_sendData.setPoint);
//			send_MX64_action();	
//			taskEXIT_CRITICAL();//退出临界段
		}
		osDelay(1);
	}
}
static void limit_max_min(float *value,float max,float min)
{
	if(*value >max)
	{
		*value = max;
	}
	else if(*value<min)
	{
		*value = min;
	}
}
static void gimbal_imu_config(void)
{
	/**********yaw***********/
	yaw.id = 0;
	yaw.receiveId = 0x207;
	ZeroCheck_Init_Interface(&yaw.zero,8191);
	yaw.mode = imuYaw;
	
	yaw.setPoint = getYawAngle();
	pitch.MX64_sendData.setPoint = 0;
	
	PID_struct_init(&yaw.positionPID,1,30000.0f,100.0f,0.6f,0.000f,0.6f);
	PID_struct_init(&yaw.speedPID,1,30000.0f,100.0f,-5000.0f,-0.01f,-4000.0f);
	
}
static void gimbal_config(void)
{
	/**********yaw***********/
	yaw.id = 0;
	yaw.receiveId = 0x207;
	ZeroCheck_Init_Interface(&yaw.zero,8191);
	yaw.mode = doubleLoop;
	yaw.setPoint = 3000;
	PID_struct_init(&yaw.positionPID,1,30000.0f,80.0f,0.06f,0.001f,0.05f);
	PID_struct_init(&yaw.speedPID,1,30000.0f,80.0f,200.0f,10.0f,10.0f);
	pitch.MX64_sendData.setPoint = 500;
}
