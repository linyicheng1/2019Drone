#ifndef __IMU_H__
#define __IMU_H__
#include "stm32f1xx_hal.h"
typedef  struct IMU
{
	float yawAngle;
	float pitchAngle;
	float yawSpeed;
	float pitchSpeed;
	uint32_t sendId;
	uint32_t receiveId;
    uint32_t updateTime;
}IMU;
void imuDiscode(uint8_t *pitch_buffer,uint8_t *yaw_buffer,int update);
float getYawAngle(void);
float getPitchAngle(void);
float getYawSpeed(void);
float getPitchSpeed(void);
#endif // __DBUS_H__
