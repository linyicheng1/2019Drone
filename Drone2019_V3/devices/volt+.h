#ifndef __VOLT_H__
#define __VOLT_H__
#include "drv_uart.h"
uint16_t Volt_encode(uint8_t *send,float * get,uint16_t length);
uint16_t Volt_discode(uint8_t *src,float *data);
#endif // __VOLT_H__
