#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "drv_can.h"
#include "M2006.h"
#include "DM50.h"
enum motorMode
{
	openLoop,
	speedLoop,
	positionLoop,
	doubleLoop
};
float motor_PID_cal(pid_t *speedPID,pid_t *positionPID,
	    float setPoint,float speedFeedback,
		float positionFeedback,uint8_t mode);
void M2006_control(struct M2006 *pitch);
void DM50_control(struct DM50 *motor);
#endif // __MOTOR_H__
