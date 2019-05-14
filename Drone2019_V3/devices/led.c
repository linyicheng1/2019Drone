#include "led.h"
#include "main.h"
/*
 * ���� ���� --- ����
 * ���� ˫�� --- ����ģʽ
 * ����˫�� ---  ң��������
 * �������� ---  �Լ�
 * ��ƿ��� ---  can1 ��ͨ
 * ���˫�� ---  can2 ��ͨ
 * ��������������м���n�� --- can1������idΪn��can�豸����
 * ���˫˫�������м���n�� --- can2������idΪn��can�豸����
 * 
 */
static void blueOn(void);
static void blueOff(void);
static void blueToggle(void);
static void redOn(void);
static void redOff(void);
static void redToggle(void);
/**
  * @brief  ����ģʽ ���� ����
  *         1 ms ����һ��
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
  * @brief  ����ģʽ ���� ��˫��
  *         1 ms ����һ��
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
  * @brief  ң�������� ��������
  *         1 ms ����һ��
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
  * @brief  �Լ�
  *         1 ms ����һ��
  * @param  none
  * @retval none
  */
void led_selfCheck(void)
{
	redOn();
	blueOn();

}
/**
  * @brief  IMU����
  *         1 ms ����һ��
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
  * @brief  yaw ��������
  *         1 ms ����һ��
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
  * @brief  can2����
  *         1 ms ����һ��
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
  * @brief  can1���豸����
  *         1 ms ����һ��
  * @param  none
  * @retval none
  */
void led_device1_Lost(id)
{
	
}
/**
  * @brief  ���ڲ�ͬled������һ����������һ���װ
  *         ����ɫled
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