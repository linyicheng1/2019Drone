#ifndef __FRICTIONWHEEL_H__
#define __FRICTIONWHEEL_H__
#include "stm32f1xx_hal.h"
uint8_t elec_control_calibration(short begin_PWM,short end_PWM);
uint8_t ramp_speed(int setSpeed);
#endif
