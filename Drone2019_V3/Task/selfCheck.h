#ifndef __SELFCHECK__
#define __SELFCHECK__
#define selfChck 1

struct compose {
    unsigned int IMU_disconneced : 2;
    unsigned int RC_disconneced : 2;
    unsigned int yaw_disconneced : 2;
    unsigned int pitch_disconneced : 2;
    unsigned int toggleBullet_disconneced : 2;
    unsigned int left_friction_disconneced : 2;
    unsigned int right_friction_disconneced : 2;
} __attribute__ ((__packed__));

typedef struct state
{
	union
	{	
		unsigned short droneState;
		struct compose composeState;
	}drone;
}state;
void selfCheck(void const * argument);
void rePowerShoot(void);
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))
#endif
