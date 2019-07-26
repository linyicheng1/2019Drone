/**
  * @file    drv_tim.c
  * @author  lyc
  * @brief   tim operations
  * @pinAssignment
  *          TIM3
  *            PC7---CH2---frictionWheel
  *            PC8---CH3---frictionWheel
  *             
  *            PC6---CH1
  *            ...wait to add
  */
#include "tim.h"
#include "drv_tim.h"
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

void pwm_setvalue(uint16_t value,TIM_HandleTypeDef *tim,uint32_t TIM_CHANNEL)
{
    TIM_OC_InitTypeDef sConfigOC;
 
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(tim, &sConfigOC, TIM_CHANNEL);
    HAL_TIM_PWM_Start(tim, TIM_CHANNEL);   
}

void pwm_setvalue_DMA(uint32_t *value,TIM_HandleTypeDef *tim,uint32_t TIM_CHANNEL,uint8_t length)
{
    TIM_OC_InitTypeDef sConfigOC;
 
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value[0];
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(tim, &sConfigOC, TIM_CHANNEL);
    HAL_TIM_PWM_Start_DMA(tim,TIM_CHANNEL_1,value,length);
}


