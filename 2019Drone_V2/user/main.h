#ifndef __MAIN_H
#define __MAIN_H
#include "driver.h"
#include "gimbal.h"
#include "shoot.h"
#include "stm32f10x.h"
enum DroneMode
{
	remote_control,//Ò£¿ØÆ÷¿ØÖÆ
	key_board_control,//¼üÊó¿ØÖÆ
	emergency_stop,//½ôÍ£Ä£Ê½
};
typedef struct systick_time
{
	uint32_t systick_ms;
	uint32_t systick_s;
	uint32_t systick_min;
}systick_time;
typedef struct DroneState
{
	uint8_t drone_state;
	systick_time time;
	GimbalState gimbal_state;
	ShootState shoot_state;
}DroneState;
void droneConfig(void);
void droneInit(void);
void RCC_Configuration(void);
uint32_t get_systick_ms(void);
uint32_t get_systick_s(void);
uint32_t get_systick_min(void);
#endif



