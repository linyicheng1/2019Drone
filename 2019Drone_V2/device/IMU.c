#include "IMU.h"
#include "string.h"
#include "drv_can.h"
IMU imu;
void imuDiscode(uint8_t *pitch_buffer,uint8_t *yaw_buffer)
{
	memcpy(&imu.pitchAngle,pitch_buffer,4);	
	memcpy(&imu.pitchSpeed,&pitch_buffer[4],4);
	memcpy(&imu.yawAngle,yaw_buffer,4);	
	memcpy(&imu.yawSpeed,&yaw_buffer[4],4);
}
float getYawAngle(void)
{
	return imu.yawAngle;
}
float getPitchAngle(void)
{
	return imu.pitchAngle;
}
float getYawSpeed(void)
{
	return imu.yawSpeed;
}
float getPitchSpeed(void)
{
	return imu.pitchSpeed;
}
