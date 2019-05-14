#ifndef __GM6020_H__
#define __GM6020_H__
#include "pid.h"
#include "ZeroCheck.h"
/**
  * @note  
  * ID 设置的范围为1-8
  * 发送的标识符 0x200 ---对应id为 1-4
  * 输出值范围为 -30000~30000
  * DATA[0] id-1 高8位  
  * DATA[1] id-1 低8位
  * DATA[2] id-2 高8位
  * DATA[3] id-2 低8位
  * DATA[4] id-3 高8位
  * DATA[5] id-3 低8位
  * DATA[6] id-4 高8位
  * DATA[7] id-4 低8位
  * 发送的标识符 0x1ff ---对应id为 5-8
  * DATA[0] id-5 高8位  
  * DATA[1] id-5 低8位
  * DATA[2] id-6 高8位
  * DATA[3] id-6 低8位
  * DATA[4] id-7 高8位
  * DATA[5] id-7 低8位
  * DATA[6] id-8 高8位
  * DATA[7] id-8 低8位
  * 反馈id 0x20+id(如id为1，标识符为 0x201)
  * 反馈内容
  * DATA[0] 转子机械角度高8位
  * DATA[1] 转子机械角度低8位
  * DATA[2] 转子转速高8位
  * DATA[3] 转子转速低8位
  * DATA[4] 实际输出转矩高8位
  * DATA[5] 实际输出转矩低8位
  * 发送频率 1kHZ
  * 转子机械角度范围 0~8191
  * 减速比 36:1
  */
  
struct GM6020
{
	uint8_t id;
	uint32_t sendId;
	uint32_t receiveId;
	uint8_t mode;
	uint32_t sendMax;
	int16_t sendData;
	uint8_t sendBuffer[8];
	uint16_t receiveAngle;
	float Angle_zoroCheck;
	short receiveSpeed;
	uint16_t qorque;//力矩
	float setPoint;
	pid_t speedPID;
	pid_t positionPID;
	ZeroCheck_Typedef zero;
	float imuSpeed;
	float imuAngle;
	uint32_t updateTime;
};

#endif // _GM6020_H__
