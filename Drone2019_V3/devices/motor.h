#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "drv_can.h"
#include "M2006.h"
#include "GM6020.h"
#include "MX64.h"
enum motorMode
{
	openLoop,
	speedLoop,
	positionLoop,
	doubleLoop,
	imuPitch,
	imuYaw
};
void send_MX64_position(unsigned short Control_Angle);
void send_MX64_action(void);
void GM6020_discode(struct GM6020 *motor,uint8_t *receiveData);
uint8_t *GM6020_control(struct GM6020 *motor);
void M2006_discode(struct M2006 *motor,uint8_t *receiveData);
uint8_t *M2006_control(struct M2006 *motor);
#endif // __MOTOR_H__
