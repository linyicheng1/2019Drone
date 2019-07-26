#include "debug.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "drv_uart.h"
#include "motor.h"
extern struct GM6020 yaw;
extern struct MX64 pitch;
float debug_SendData[4];
int cnt = 0;
void debug(void const * argument)
{

	for(;;)
  {		
		//500ms send now state 
		if(cnt%1000 == 0)
		{
			printf("frictionWheel speed is \n");
			printf("yaw angle is %d\n",yaw.receiveAngle);
			printf("pitch set angle is %d/n",pitch.MX64_sendData.setPoint);
		}
		cnt ++;
		if(cnt>100000)
			cnt = 0;
		osDelay(1);
  }
}
static void debugSend(float data,int id)
{
	debug_SendData[id] = data;
}
