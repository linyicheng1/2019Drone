#ifndef __LED_H__
#define __LED_H__
#include "stm32f1xx_hal.h"
void led_selfCheck(void);
void led_IMU_Lost(void);
void led_RC_Lost(void);
void led_YAW_Lost(void);
void led_toggleBullet_Lost(void);
void led_normal(void);
void left_friction_Lost(void);
void right_friction_Lost(void);
void led_error(void);
void led_shoot_powerOff(void);
#endif // __LED_H__
