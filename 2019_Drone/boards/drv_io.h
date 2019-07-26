#ifndef __DRV_IO_H__
#define __DRV_IO_H__

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "board.h"
void IO_setPin(int pin,uint8_t GPIO_PinState);
uint8_t IO_readPin(uint8_t pin);

#endif // __DRV_IO_H__
