#ifndef __GIMBAL__
#define __GIMBAL__
void gimbal_imu(void const *argument);
void gimbal(void const * argument);

#define yaw_Min  -145.0f
#define yaw_Max  55.0f

#define pitch_Min  -45.0f
#define pitch_Max  20.0f

#define pitch_offset  687.0f
#define pitch_scale  10.8f

#define motorCenter 2870 
#define motorLeft   1660  
#define motorRight  5470 



#endif //__GIMBAL__
