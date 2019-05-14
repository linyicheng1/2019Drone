#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "M2006.h"
#include "DM50.h"
#include "GM3510.h"
#include "RM6623.h"
#include "GM6020.h"
#include "drv_can.h"
enum motorMode
{
	openLoop,
	speedLoop,
	positionLoop,
	doubleLoop,
	imuPitch,
	imuYaw
};
float motor_PID_cal(pid_t *speedPID,pid_t *positionPID,
	    float setPoint,float speedFeedback,
		float positionFeedback,uint8_t mode);
uint8_t * M2006_control(struct M2006 *motor,uint8_t *receiveData);
void DM50_control(struct DM50 *motor);
void RM6623_control(struct RM6623 *motor,uint8_t model);
		uint8_t * GM6020_control(struct GM6020 *motor,uint8_t *receiveData);
#endif // __MOTOR_H__
