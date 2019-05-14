#include "led.h"
#include "main.h"
/*
 * 蓝灯 慢闪 --- 正常
 * 蓝灯 双闪 --- 辅瞄模式
 * 红蓝双闪 ---  遥控器掉线
 * 红蓝交替 ---  自检
 * 红灯快闪 ---  can1 不通
 * 红灯双闪 ---  can2 不通
 * 红灯慢闪，蓝灯中间闪n次 --- can1总线上id为n的can设备掉线
 * 红灯双双，蓝灯中间闪n次 --- can2总线上id为n的can设备掉线
 * 
 */
static void blueOn(void);
static void blueOff(void);
static void blueToggle(void);
static void redOn(void);
static void redOff(void);
static void redToggle(void);
/**
  * @brief  正常模式 蓝灯 慢闪
  *         1 ms 进入一次
  * @param  none
  * @retval none
  */
void led_normal(void)
{
	static int cnt;
	redOff();
	if(cnt%20 == 0||cnt%20 == 2)
	{
		blueToggle();
	}
	cnt ++;
	if(cnt>100000)
		cnt = 0;
}
/**
  * @brief  辅瞄模式 蓝灯 慢双闪
  *         1 ms 进入一次
  * @param  none
  * @retval none
  */
void led_aidAim(void)
{
	static int cnt;
	redOff();
	if(cnt%200 == 0||cnt%200 == 10||cnt%200 == 20||cnt%200 == 30)
	{
		blueToggle();
	}
	cnt ++;
	if(cnt>100000)
		cnt = 0;
}
/**
  * @brief  遥控器掉线 红蓝交替
  *         1 ms 进入一次
  * @param  none
  * @retval none
  */
void led_RC_Lost(void)
{
	static int cnt;
	if(cnt%20 == 0)
	{
		blueOn();
	}
	else if(cnt%20 == 2)
	{
		blueOff();
	}
	if(cnt%20 == 10)
	{
		redOn();
	}
	else if(cnt%20 == 12)
	{
		redOff();
	}
	cnt ++;
	if(cnt>100000)
		cnt = 0;
}
/**
  * @brief  自检
  *         1 ms 进入一次
  * @param  none
  * @retval none
  */
void led_selfCheck(void)
{
	redOn();
	blueOn();

}
/**
  * @brief  IMU掉线
  *         1 ms 进入一次
  * @param  none
  * @retval none
  */
void led_IMU_Lost(void)
{
	static int cnt;
	redOff();
	if(cnt%10 == 0)
	{
		blueOn();
	}
	else if(cnt%10 == 1)
	{
		blueOff();
	}
	cnt ++;
	if(cnt>100000)
		cnt = 0;
}
/**
  * @brief  yaw 轴电机掉线
  *         1 ms 进入一次
  * @param  none
  * @retval none
  */
void led_YAW_Lost(void)
{
	static int cnt;
	blueOff();
	if(cnt%20 == 0)
	{
		redOn();
	}
	else if(cnt%20 == 2)
	{
		redOff();
	}
	cnt ++;
	if(cnt>100000)
		cnt = 0;
}
/**
  * @brief  can2掉线
  *         1 ms 进入一次
  * @param  none
  * @retval none
  */
void led_toggleBullet_Lost(void)
{
	static int cnt;
	blueOff();
	if(cnt%5 == 0)
	{
		redOn();
	}
	else if(cnt%5 == 1)
	{
		redOff();
	}
	cnt ++;
	if(cnt>100000)
		cnt = 0;
}
/**
  * @brief  can1上设备掉线
  *         1 ms 进入一次
  * @param  none
  * @retval none
  */
void led_device1_Lost(id)
{
	
}
/**
  * @brief  由于不同led驱动不一样，在这做一层封装
  *         打开蓝色led
  * @param  none
  * @retval none
  */
static void blueOn(void)
{
	HAL_GPIO_WritePin(GPIOC, LED1_Pin,GPIO_PIN_RESET);
}
static void blueOff(void)
{
	HAL_GPIO_WritePin(GPIOC, LED1_Pin,GPIO_PIN_SET);
}
static void blueToggle(void)
{
	HAL_GPIO_TogglePin(GPIOC, LED1_Pin);
}
static void redOn(void)
{
	HAL_GPIO_WritePin(GPIOC, LED2_Pin,GPIO_PIN_RESET);
}
static void redOff(void)
{
	HAL_GPIO_WritePin(GPIOC, LED2_Pin,GPIO_PIN_SET);
}
static void redToggle(void)
{
	HAL_GPIO_TogglePin(GPIOC, LED2_Pin);
}