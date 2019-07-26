#ifndef __DRV_TIM_H__
#define __DRV_TIM_H__
#include "stm32f1xx_hal.h"
#include "board.h"
void pwm_setvalue(uint16_t value,TIM_HandleTypeDef *tim,uint32_t TIM_CHANNEL);
void pwm_setvalue_DMA(uint32_t *value,TIM_HandleTypeDef *tim,
	uint32_t TIM_CHANNEL,uint8_t length);
#endif // __DRV_TIM_H__
