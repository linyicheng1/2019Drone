#ifndef __GIMBAL_H__
#define __GIMBAL_H__
#include "stm32f10x.h"
enum GimbalMode
{
	IMU_single_yaw = 0,
	IMU_single_pitch,
	IMU_double_axis,
	motorFeedback
};
typedef struct GimbalState
{
	uint8_t state;
}GimbalState;
void gimbal_init(void);
void gimbal_tast(unsigned char model);
#endif // __GIMBAL_H__
