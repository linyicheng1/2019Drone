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
extern struct M2006 left_friction;
extern struct M2006 right_friction;

static void selfCheck_update(void);
static void led_update(void);
static void control(void );
static void dis_control(void);
static uint16_t checkComponent(uint32_t now,uint32_t last,uint32_t delay);
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
	if(droneState.drone.droneState == 0||droneState.drone.droneState != 0x1400||selfChck == 0)
	{
		/*自检通过后开始正常控制*/
		osThreadTerminate(init_TASKHandle);
		
		control();
		dis_control_flag = 0;
	}
	osThreadDef(communication_TASK, communication,osPriorityNormal , 0, 128);
	communication_TASKHandle = osThreadCreate(osThread(communication_TASK), NULL);
		
	osThreadDef(debug_TASK, debug, osPriorityNormal, 0, 0);
	debug_TASKHandle = osThreadCreate(osThread(debug_TASK), NULL);
	
  for(;;)
  {//100ms一次自检
		selfCheck_update();
		led_update();
		if((droneState.drone.droneState == 0||droneState.drone.droneState == 0x1400)&&dis_control_flag == 1)
		{
			/*自检通过后开始正常控制*/		
			control();
			dis_control_flag = 0;
		}
		else if(droneState.drone.droneState != 0&&droneState.drone.droneState != 0x1400&&droneState.drone.droneState != 0x0001
			&&dis_control_flag == 0&&selfChck == 1)
		{//只有当selfCheck 的宏定义打开之后才会生效，关闭可以用来调试
			dis_control_flag = 1;
			dis_control();
		}
		osDelay(100);
  }
}
static void control(void )
{
	if(droneState.drone.composeState.IMU_disconneced == 1)
	{
		osThreadDef(gimbal_TASK, gimbal, osPriorityRealtime, 0, 128);
		gimbal_TASKHandle = osThreadCreate(osThread(gimbal_TASK), NULL);
	}
	else if(droneState.drone.composeState.IMU_disconneced == 0)
	{
		osThreadDef(gimbal_TASK, gimbal_imu, osPriorityRealtime, 0, 128);
		gimbal_TASKHandle = osThreadCreate(osThread(gimbal_TASK), NULL);
	}
	osThreadDef(shoot_TASK, shoot_2006, osPriorityRealtime, 0, 128);
	shoot_TASKHandle = osThreadCreate(osThread(shoot_TASK), NULL);
}

static void dis_control(void)
{
	osThreadTerminate(gimbal_TASKHandle);
	osThreadTerminate(shoot_TASKHandle);
}
static void led_update(void)
{
	switch (droneState.drone.droneState)
	{
		case 0x0000: led_normal();
			break;
		case 0x0004:led_RC_Lost(); //RC
			break;
		case 0x0001:led_IMU_Lost();//IMU
			break;
		case 0x0010:led_YAW_Lost();//yaw
			break;
		case 0x0100:led_toggleBullet_Lost();//toggle bullet
			break;
		case 0x0400:left_friction_Lost();//left friction
			break;
		case 0x1000:right_friction_Lost();//right friction
			break;
		case 0x1400:led_shoot_powerOff();//shoot power offline
			break;
		default :led_error();
			break;
	}
}

static void selfCheck_update(void)
{
	uint32_t nowTime = GetSysTimeMs();
	/*********IMU**************/
	droneState.drone.composeState.IMU_disconneced = checkComponent(nowTime,imu.updateTime,10);
	/*********YAW**************/
	droneState.drone.composeState.yaw_disconneced = checkComponent(nowTime,yaw.updateTime,10);
	/*********Pitch************/

	/*********RC***************/
	droneState.drone.composeState.RC_disconneced = checkComponent(nowTime,remoteControl.updateTime,100);
	/*********拨弹*************/
	droneState.drone.composeState.toggleBullet_disconneced = checkComponent(nowTime,toggleBullet.updateTime,10);
	/*********左摩擦轮*************/
	droneState.drone.composeState.left_friction_disconneced = checkComponent(nowTime,left_friction.updateTime,10);
	/*********右摩擦轮*************/	
	droneState.drone.composeState.right_friction_disconneced = checkComponent(nowTime,right_friction.updateTime,10);
	
}
static uint16_t checkComponent(uint32_t now,uint32_t last,uint32_t delay)
{
	if(now-last > delay)
	return 1;
	else 
		return 0;
}