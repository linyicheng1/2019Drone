/**
  * @file    drv_io.c
  * @author  lyc
  * @brief   gpio operations
  * @pinAssignment
  *          led1---PC13
  *          led2---PC14
  *          key1---PC2
  *          key2---PC3
  *          laser---PC1
  *          Photoelectric switch
  *            signal---PC5
  *            enable---PC4
  *          electromagnetic valve
  *            valve1---PA4
  *            valve2---PA5
  *            valve3---PA6
  *            valve4---PA7
  */
#include "drv_io.h"
/**
  * @brief  change gpio state
  * @retval None
  */
void IO_setPin(int pin,uint8_t GPIO_PinState)
{
  switch (pin)
  {   
	  case led1:
		  HAL_GPIO_WritePin(GPIOC, LED1_Pin, GPIO_PinState);
	  break;
	  case led2:
		  HAL_GPIO_WritePin(GPIOC, LED2_Pin, GPIO_PinState);break;
	  case laser:
		  HAL_GPIO_WritePin(GPIOC, LASER_Pin, GPIO_PinState);break;
	  case enable:
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PinState);break;
	  case valve1:
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PinState);break;
	  case valve2:
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PinState);break;
	  case valve3:
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PinState);break;
	  case valve4:
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PinState);break;
	  default: break;
		  
  }
}
/**
  * @brief  read gpio state
  * @retval None
  */
uint8_t IO_readPin(uint8_t pin)
{
	int8_t state = -1;
	while(state==-1)
	{
		switch (pin)
		{   
			case key1:
				state = HAL_GPIO_ReadPin(GPIOC, KEY1_Pin);break;
			case key2:
				state = HAL_GPIO_ReadPin(GPIOC, KEY2_Pin);break;
			case signal:
				state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5);break;  
		}
    }
	return state;
}	
  
  
  
  
  
  
  
  
  
  
  
