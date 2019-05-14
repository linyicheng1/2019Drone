/**
  * @file    drv_tim.c
  * @author  lyc
  * @brief   tim operations
  * @pinAssignment
  *          TIM3
  *            PC6---CH1---frictionWheel
  *            PC7---CH2---frictionWheel
  */
#include "drv_tim.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
static HAL_StatusTypeDef pwm_set_value(TIM_HandleTypeDef *tim,int channel,int value);

/**
  * @brief  设置led pwm占空比
  * @param  value:百分比
  * @retval HAL status
  */
HAL_StatusTypeDef led_set_value(int value,uint8_t channel)
{
	int pwm = (int)value*20;//最大为2000
	if(pwm_set_value(&htim2,channel,pwm)==HAL_OK)
	{
		return HAL_OK;
	}
}
/**
  * @brief  设置摩擦轮速度
  * @param  value:百分比
  * @retval HAL status
  */
HAL_StatusTypeDef friction_set_value(int value)
{
	int pwm = (int)value;//最大为2000
	if(pwm_set_value(&htim3,1,pwm)==HAL_OK&&pwm_set_value(&htim3,2,pwm)==HAL_OK)
	{
		return HAL_OK;
	}
}
/**
  * @brief  设置pwm波的占空比
  * @param  
  * @retval HAL status
  */
static HAL_StatusTypeDef pwm_set_value(TIM_HandleTypeDef *tim,int channel,int value)
{
    TIM_OC_InitTypeDef sConfigOC;
 
	
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if(channel == 1)
	{
		HAL_TIM_PWM_ConfigChannel(tim, &sConfigOC, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(tim, TIM_CHANNEL_1);   
	}
	else if(channel == 2)
	{
		HAL_TIM_PWM_ConfigChannel(tim, &sConfigOC, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(tim, TIM_CHANNEL_2);   	
	}
}