#ifndef __DRV_UART_H__
#define __DRV_UART_H__
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_usart.h"
#include "board.h"
enum ReceiveMODE
{
	unBusyInterruption,//闲时中断
	DMA_IndefiniteLength,//不定长DMA
	pingPongCache//乒乓缓存
};
#define uart1Mode unBusyInterruption
#define uart2Mode unBusyInterruption
#define uart3Mode unBusyInterruption
#define uart4Mode unBusyInterruption
#define uart5Mode unBusyInterruption

struct uart_manage_obj
{
    uint8_t tx_buffer[8];
	uint8_t rx_buffer[8];
};
void UART_sendData(uint8_t *tx_bufferPrt,uint8_t uart,uint16_t size);
void UART1_receiveData_hook(void);
void UART2_receiveData_hook(void);
void UART3_receiveData_hook(void);
void UART4_receiveData_hook(void);
void UART5_receiveData_hook(void);
#endif // __DRV_UART_H__
