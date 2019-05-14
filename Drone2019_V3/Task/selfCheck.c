#include "FreeRTOS.h"
#include "selfCheck.h"
#include "cmsis_os.h"
#include "shoot.h"
#include "gimbal.h"
#include "debug.h"
#include "communication.h"
#include "drv_tim.h"
#include "board.h"
#include "IMU.h"
#include "M2006.h"
#include "GM6020.h"
#include "dbus.h"
#include "led.h"
extern osThreadId selfCheck_TASKHandle;
extern osThreadId gimbal_TASKHandle;
extern osThreadId shoot_TASKHandle;
extern osThreadId communication_TASKHandle;
extern osThreadId debug_TASKHandle;
extern osThreadId init_TASKHandle;
osThreadId control_TASKHandle;
struct state droneState;

extern IMU imu;
extern struct GM6020 yaw;
extern struct MX64 pitch;
extern struct RC remoteControl;
extern struct M2006 toggleBullet;

static void selfCheck_update(void);
static void led_update(void);
static void control(void );
static void dis_control(void);
/**
  * @brief  初始化时首先自检，然后300ms检查一次信号更新
  * @param  none 
  * @retval none
  */
void selfCheck(void const * argument)
{
	static uint32_t initTime;
	static short dis_control_flag = 1;
	initTime = GetSysTimeMs();
    //自检300ms
	led_selfCheck();
	while(GetSysTimeMs()-initTime<300)
	{
		selfCheck_update();
		osDelay(1);
	}
	if(droneState.normal == 0)
	{
		/*自检通过后开始正常控制*/
		osThreadTerminate(init_TASKHandle);
		
		control();
		dis_control_flag = 0;
	}
	osThreadDef(communication_TASK, communication,osPriorityNormal , 0, 0);
	communication_TASKHandle = osThreadCreate(osThread(communication_TASK), NULL);
		
	osThreadDef(debug_TASK, debug, osPriorityNormal, 0, 0);
	debug_TASKHandle = osThreadCreate(osThread(debug_TASK), NULL);
	
    for(;;)
    {//100ms一次自检
		selfCheck_update();
		led_update();
//		if(droneState.normal == 0&&dis_control_flag == 1)
//		{
			/*自检通过后开始正常控制*/		
			control();
			dis_control_flag = 0;
//		}
//		else if(droneState.normal == 1&&dis_control_flag == 0)
//		{
//			dis_control_flag = 1;
//			dis_control();
//		}
		osDelay(100);
    }
}
static void control(void )
{
	osThreadDef(gimbal_TASK, gimbal, osPriorityRealtime, 0, 128);
	gimbal_TASKHandle = osThreadCreate(osThread(gimbal_TASK), NULL);
		
	osThreadDef(shoot_TASK, shoot, osPriorityRealtime, 0, 128);
	shoot_TASKHandle = osThreadCreate(osThread(shoot_TASK), NULL);
	
}
static void dis_control(void)
{
	osThreadTerminate(gimbal_TASKHandle);
	osThreadTerminate(shoot_TASKHandle);
}
static void led_update(void)
{
//	if(droneState.IMU_disconneced == 1)
//	{
//		led_IMU_Lost();
//	}
	 if(droneState.RC_disconneced == 1)
	{
		led_RC_Lost();
	}
	else if(droneState.yaw_disconneced == 1)
	{
		led_YAW_Lost();
	}
	else if(droneState.toggleBullet_disconneced == 1)
	{
		led_toggleBullet_Lost();
	}
	else if(droneState.normal == 0)
	{
		led_normal();
	}
}

static void selfCheck_update(void)
{
	uint32_t nowTime = GetSysTimeMs();
	/*********IMU**************/
	if(nowTime - imu.updateTime>10)
	{
		droneState.IMU_disconneced = 1;
	}
	else 
		droneState.IMU_disconneced = 0;
	/*********YAW**************/
	if(nowTime - yaw.updateTime>10)
	{
		droneState.yaw_disconneced = 1;
	}
	else 
		droneState.yaw_disconneced = 0;
	/*********Pitch************/

	/*********RC***************/
	if(nowTime - remoteControl.updateTime>100)
	{
		droneState.RC_disconneced = 1;
	}
	else 
		droneState.RC_disconneced = 0;
	/*********拨弹*************/
	if(nowTime - toggleBullet.updateTime>10)
	{
		droneState.toggleBullet_disconneced = 1;
	}
	else 
		droneState.toggleBullet_disconneced = 0;
	droneState.normal = droneState.pitch_disconneced|droneState.RC_disconneced
						|droneState.toggleBullet_disconneced|droneState.yaw_disconneced;
	
}
