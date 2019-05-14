#include "main.h"
#include "modules.h"
#include "drv_tim.h"
#include "dbus.h"
extern DroneState drone_state;
void modules_init(void)
{
	delayMs_Block(300);
	gimbal_init();
	delayMs_Block(300);
	shoot_init();
}
void modules_task(void)
{
	if(getRCData().rc.s2 == 1)
	{
		drone_state.drone_state = remote_control;
	}
	else if(getRCData().rc.s2 == 3)
	{
		drone_state.drone_state = key_board_control;
	}
	else if(getRCData().rc.s2 == 2)
	{
		drone_state.drone_state = emergency_stop;
	}
	gimbal_tast(1);
	shoot_task(1);
}
