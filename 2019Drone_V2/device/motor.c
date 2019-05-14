#include "motor.h"
extern IMU imu;
extern struct M2006 toggleBullet;
void RM6623_control(struct RM6623 *motor,uint8_t model)
{
	uint8_t *receiveBuffer;
	//update
	receiveBuffer = getCan1Msg(motor->id);
	motor->receiveAngle = receiveBuffer[0]<<8|receiveBuffer[1];
	motor->real_flow = receiveBuffer[2]<<8|receiveBuffer[3]; 
	motor->set_flow = receiveBuffer[4]<<8|receiveBuffer[5]; 
	//zero check
	motor->Angle_zoroCheck = Get_ZeroCheck_Value_Interface(&motor->zero,motor->receiveAngle);
	//calculate PID
	if(model==0)
	{
		motor->sendData = (short)motor_PID_cal(&motor->speedPID,&motor->positionPID,motor->setPoint,
				   motor->real_flow ,motor->Angle_zoroCheck,motor->mode);
    }
	else if(model ==1)
	{
		motor->sendData = (short)motor_PID_cal(&motor->speedPID,&motor->positionPID,motor->setPoint,
				   motor->imu_speed ,motor->imu_angle,motor->mode);
	}
}

uint8_t * GM6020_control(struct GM6020 *motor,uint8_t *receiveData)
{
	//update
	motor->receiveAngle = receiveData[0]<<8|receiveData[1];
	motor->receiveSpeed = receiveData[2]<<8|receiveData[3];
	motor->qorque = receiveData[4]<<8|receiveData[5];
	//zero check
	motor->Angle_zoroCheck = Get_ZeroCheck_Value_Interface(&motor->zero,motor->receiveAngle);
//	//calculate PID
    motor->sendData = (short)motor_PID_cal(&motor->speedPID,&motor->positionPID,motor->setPoint,
				   motor->receiveSpeed ,motor->Angle_zoroCheck,motor->mode);
	//send databuff
	switch(motor->receiveId)
	{
		case 0x205:
			motor->sendBuffer[0] = motor->sendData>>8&0xff;
			motor->sendBuffer[1] = motor->sendData&0xff;
			break;
		case 0x206:
			motor->sendBuffer[2] = motor->sendData>>8&0xff;
			motor->sendBuffer[3] = motor->sendData&0xff;
			break;
		case 0x207:
			motor->sendBuffer[4] = motor->sendData>>8&0xff;
			motor->sendBuffer[5] = motor->sendData&0xff;
			break;
		case 0x208:
			motor->sendBuffer[6] = motor->sendData>>8&0xff;
			motor->sendBuffer[7] = motor->sendData&0xff;
			break;
	}
	return motor->sendBuffer;
}
uint8_t * M2006_control(struct M2006 *motor,uint8_t *receiveData)
{
	//update
	motor->receiveAngle = receiveData[0]<<8|receiveData[1];
	motor->receiveSpeed = receiveData[2]<<8|receiveData[3];
	motor->qorque = receiveData[4]<<8|receiveData[5];
	//zero check
	motor->Angle_zoroCheck = Get_ZeroCheck_Value_Interface(&motor->zero,motor->receiveAngle);
//	//calculate PID
    motor->sendData = (short)motor_PID_cal(&motor->speedPID,&motor->positionPID,motor->setPoint,
				   motor->receiveSpeed ,motor->Angle_zoroCheck,motor->mode);
	//send databuff
	switch(motor->receiveId)
	{
		case 0x201:
			motor->sendBuffer[0] = motor->sendData>>8&0xff;
			motor->sendBuffer[1] = motor->sendData&0xff;
			break;
		case 0x202:
			motor->sendBuffer[2] = motor->sendData>>8&0xff;
			motor->sendBuffer[3] = motor->sendData&0xff;
			break;
		case 0x203:
			motor->sendBuffer[4] = motor->sendData>>8&0xff;
			motor->sendBuffer[5] = motor->sendData&0xff;
			break;
		case 0x204:
			motor->sendBuffer[6] = motor->sendData>>8&0xff;
			motor->sendBuffer[7] = motor->sendData&0xff;
			break;
	}
	return motor->sendBuffer;
	
}
void DM50_control(struct DM50 *motor)
{
	
}

float motor_PID_cal(pid_t *speedPID,pid_t *positionPID,
	    float setPoint,float speedFeedback,
		float positionFeedback,uint8_t mode)
{
	
	if(mode == openLoop)
	{
		return setPoint;
	}
	else if(mode == speedLoop)
	{
		return pid_calc(speedPID,speedFeedback,setPoint);
	}
	else if(mode == positionLoop)
	{
		return pid_calc(positionPID,positionFeedback,setPoint);
	}
	else if(mode ==doubleLoop)
	{
		float temp;
		temp = pid_calc(positionPID,positionFeedback,setPoint);
		return pid_calc(speedPID,speedFeedback,temp);
	}
	else if(mode == imuPitch)
	{
		float temp;
		temp = pid_calc(positionPID,positionFeedback,setPoint);
		return pid_calc(speedPID,speedFeedback,temp);
	}
	else if(mode == imuYaw)
	{
		float temp;
		temp = pid_calc(positionPID,imu.yawAngle,setPoint);
		return pid_calc(speedPID,imu.yawSpeed,temp);
	}
	else 
		return 0;
}

