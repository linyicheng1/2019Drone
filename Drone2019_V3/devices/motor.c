#include "motor.h"
#include "IMU.h"

extern IMU imu;
extern UART_HandleTypeDef huart2;

static float motor_PID_cal(pid_t *speedPID,pid_t *positionPID,
	    float setPoint,float speedFeedback,
		float positionFeedback,uint8_t mode);
void send_MX64_action(void)
{
	uint8_t MX64_buffer[6];
	MX64_buffer[0]=0xFF;
	MX64_buffer[1]=0xFF;
	MX64_buffer[2]=0xFE;
	MX64_buffer[3]=0x02;
	MX64_buffer[4]=0x05;
	MX64_buffer[5]=0xFA;
	HAL_UART_Transmit(&huart2,MX64_buffer,6,100);
}	
void send_MX64_position(unsigned short Control_Angle)
{
	uint8_t MX64_buffer[15];
	MX64_buffer[0]=0xFF;
	MX64_buffer[1]=0xFF;
	MX64_buffer[2]=0xFE;
	MX64_buffer[3]=0x05;
	MX64_buffer[4]=0x04;
	MX64_buffer[5]=0X1E;
	MX64_buffer[6]=(Control_Angle<<8)>>8;
	MX64_buffer[7]=Control_Angle>>8;
	MX64_buffer[8]=~(MX64_buffer[2]+MX64_buffer[3]+MX64_buffer[4]+MX64_buffer[5]+MX64_buffer[6]+MX64_buffer[7]);

	HAL_UART_Transmit(&huart2,MX64_buffer,9,100);
	
}
void GM6020_discode(struct GM6020 *motor,uint8_t *receiveData)
{
	//update
	motor->receiveAngle = receiveData[0]<<8|receiveData[1];
	motor->receiveSpeed = receiveData[2]<<8|receiveData[3];
	motor->qorque = receiveData[4]<<8|receiveData[5];
	//zero check
	motor->Angle_zoroCheck = Get_ZeroCheck_Value_Interface(&motor->zero,motor->receiveAngle);

    
}
uint8_t *GM6020_control(struct GM6020 *motor)
{
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
void M2006_discode(struct M2006 *motor,uint8_t *receiveData)
{
	//update
	motor->receiveAngle = receiveData[0]<<8|receiveData[1];
	motor->receiveSpeed = receiveData[2]<<8|receiveData[3];
	motor->qorque = receiveData[4]<<8|receiveData[5];
	//zero check
	motor->Angle_zoroCheck = Get_ZeroCheck_Value_Interface(&motor->zero,motor->receiveAngle);

	
}
uint8_t *M2006_control(struct M2006 *motor)
{
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
static float motor_PID_cal(pid_t *speedPID,pid_t *positionPID,
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
