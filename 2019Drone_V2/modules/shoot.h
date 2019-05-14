#ifndef __SHOOT_H
#define __SHOOT_H
#include "stm32f10x.h"
enum ShootMode
{
	single_shoot,//one click one shoot 
	low_squence_shoot,//low shoot squence 
	medium_squence_shoot,//medium shoot squence 
	high_squence_shoot//high shoot squence 
};
typedef struct ShootState
{
	uint8_t state;
}ShootState;
void shoot_task(unsigned char model);
void shoot_init(void);
#endif
