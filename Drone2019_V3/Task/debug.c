#include "debug.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "drv_uart.h"
float debug_SendData[4];
int cnt = 0;
void debug(void const * argument)
{

	for(;;)
    {		
//		USART2_Cmd();
		Debug_discode();
		if(cnt%40 == 0)
		{
//			debuger_Send_Wave(debug_SendData,4);
		}
		cnt ++;
		if(cnt>100000)
			cnt = 0;
		osDelay(1);
    }
}
void debugSend(float data,int id)
{
	debug_SendData[id] = data;
}
