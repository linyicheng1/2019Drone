#ifndef __BOARD_H__
#define __BOARD_H__
#include "drv_tim.h"
#include "drv_uart.h"
#include "drv_can.h"
#include "cmsis_os.h"
void boardInit(void);
uint64_t GetSysTimeUs(void);
uint32_t GetSysTimeMs(void);
#endif // __BOARD__
