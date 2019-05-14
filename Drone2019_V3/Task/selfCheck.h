#ifndef __SELFCHECK__
#define __SELFCHECK__
struct state
{
	char IMU_disconneced;
	char RC_disconneced;
	char yaw_disconneced;
	char pitch_disconneced;
	char toggleBullet_disconneced;
	char normal;
};
void selfCheck(void const * argument);
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))
#endif
