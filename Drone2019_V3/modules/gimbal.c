#include "gimbal.h"
#include "motor.h"
struct M2006 pitch;
struct DM50 yaw;

void gimbal_init()
{
	/**********pitch***********/
	pitch.id = 1;
	pitch.mode = positionLoop;
	M2006_control(&pitch);
	/**********yaw***********/
}