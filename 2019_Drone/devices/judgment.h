#ifndef __JUDGMENT_H
#define __JUDGMENT_H	
#include "stm32f1xx_hal.h"
typedef union
{
	float fdata;
	unsigned long idata;
}
FloatlongType;
typedef struct
{
	uint8_t RobotLevel;
	uint8_t bulletFreq;
	uint16_t remainHP;
	uint16_t shooterHeat17;
	uint16_t shooterHeat42;
	float bulletSpeed;
	float realChassisOutV;
	float realChassisOutA;
	float realChassispower;
	float remainEnergy;       // £”‡ƒ‹¡ø
}
tGameInfo;
void judgement_config(void);
void JudgmentInit(int level);
void BloodCalc(void);
void HeatCalc(float speed);
void HeatCold(void);
short GetHeat(void);
#endif 
