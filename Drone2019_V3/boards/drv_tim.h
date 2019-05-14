#ifndef __DRV_TIM_H__
#define __DRV_TIM_H__
#include "stm32f1xx_hal.h"

HAL_StatusTypeDef friction_set_value(int value);
HAL_StatusTypeDef led_set_value(int value,uint8_t channel);
#endif // __DRV_TIM_H__
