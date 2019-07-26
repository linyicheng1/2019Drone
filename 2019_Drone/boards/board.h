#ifndef __BOARD_H__
#define __BOARD_H__
#include "drv_tim.h"
#include "drv_uart.h"
#include "drv_can.h"
enum CAN
{
	can1,
	can2
};
enum IO
{
	led1,
    led2,
    key1,
    key2,
    laser,
    signal, 
    enable, 
    valve1,
    valve2,
    valve3,
    valve4
};
enum UART
{
	judgment = 1,
	other_usart2,
    remote,
	other_usart4, 
    debug 	
};
#endif // __BOARD__
