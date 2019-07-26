#include "FrictionWheel.h"
#include "drv_tim.h"
#define  begin_speed 200
#define  initTime 800
/**
  * @brief  摩擦轮初始化，可卡弹启动
  *         1 ms 进入一次
  * @param  none
  * @retval none
  */
int8_t state = -1;
float times = 0;
uint8_t ramp_speed(int setSpeed)
{

//	static //cnt times
	if(state == -1)
	{//initial
		friction_set_value(begin_speed);
		times++;
		if(times>=initTime)
		{
			times = 0;
			state = 0;
		}
	}
	if(state == 0)
	{
		friction_set_value1(times + begin_speed);
		if(times>=setSpeed)
		{
			times = 0;
			state = 1;
			return 0;
		}
		else if(times<setSpeed)
		{
			times = times + 0.01;
			return 0;
		}
  }
	else if(state == 1)
	{
		friction_set_value2(times + begin_speed);
		if(times>=setSpeed)
		{
			times = 0;
//			state = -1;
			return 1;
		}
		else if(times<setSpeed)
		{
			times = times + 0.01;
			return 0;
		}
	}
}
/**
  * @brief  电调油门行程校准，pwm为400hz 行程为10%占空比到80%占空比
  *         pwm给到 80%，然后电调上电听到三声后给到pwm给到10%，听到一声确认结束校准。
  * @param  none
  * @retval none
  */
uint8_t elec_control_calibration(short begin_PWM,short end_PWM)
{
	static int cnt = 0;
	friction_set_value(end_PWM*20);
	if(cnt++>1500)
	{
		friction_set_value(begin_PWM*20);
		return 1;
	}
	else 
	{
		return 0;
	}
}

