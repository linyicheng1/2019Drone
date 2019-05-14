#ifndef __DRV_UART_H__
#define __DRV_UART_H__
#include "stm32f1xx_hal.h"
void UART3_receiveData_hook(void);
void UART2_receiveData_hook(void);
HAL_StatusTypeDef debuger_Send_Wave(float * data,int length);
void USART1_Cmd(void);
void USART2_Cmd(void);
void USART3_Cmd(void);
void Debug_discode(void);
#endif // __DRV_UART_H__
