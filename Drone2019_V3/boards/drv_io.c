/**
  * @file    drv_io.c
  * @author  lyc
  * @brief   gpio operations
  * @pinAssignment
  *          laser
  *            PC1
  *          led
  *            PB0
  */
#include "drv_io.h"
/**
  * @brief  打开激光
  * @retval None
  */
HAL_StatusTypeDef laser_ON(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1) == GPIO_PIN_RESET)
	{
		return HAL_OK;
	}
	else 
		return HAL_ERROR;
}
/**
  * @brief  关闭激光
  * @retval None
  */
HAL_StatusTypeDef laser_OFF(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1) == GPIO_PIN_SET)
	{
		return HAL_OK;
	}
	else 
		return HAL_ERROR;
}
////...led代码
  
  
  
  
  
  
  
  
  
