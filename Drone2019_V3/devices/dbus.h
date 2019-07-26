#ifndef __DBUS_H__
#define __DBUS_H__
#include "stm32f1xx_hal.h"
struct dbus_dat {
    uint32_t chan1 : 11;
    uint32_t chan2 : 11;
    uint32_t chan3 : 11;
    uint32_t chan4 : 11;
	//44
    uint32_t s1 : 2;
    uint32_t s2 : 2;
	//48
    int32_t X : 16;
    int32_t Y : 16;
    int32_t Z : 16;
	//96
    uint32_t left : 8;
    uint32_t right : 8;
	//112
		uint32_t press_w :1;
		uint32_t press_s :1;
		uint32_t press_a :1;
		uint32_t press_d :1;
		uint32_t press_shift :1;
		uint32_t press_ctrl :1;
		uint32_t press_q :1;
		uint32_t press_e :1;
		uint32_t press_r :1;
		uint32_t press_f :1;
		uint32_t press_g :1;
		uint32_t press_z :1;
		uint32_t press_x :1;
		uint32_t press_v :1;
		uint32_t press_b :1;
		
    uint32_t reverse : 16;
	//144 --- 144/8=18

} __attribute__ ((__packed__));
typedef struct RC
{
	union
	{	
		uint8_t  raw[18];
		struct dbus_dat msg;
	}dbus;
	uint32_t updateTime;
}RC;
struct RC *getRCData(void);
void Sbus_Decode(unsigned char *data);
uint8_t getRisingEdge(float value,float *recordValue);
uint8_t getFallingEdge(float value,float *recordValue);
#endif // __DBUS_H__
