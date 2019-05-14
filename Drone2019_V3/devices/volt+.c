#include "volt+.h"
#include "string.h"

uint16_t Volt_encode(uint8_t *send,float * get,uint16_t length)
{
	memcpy(send,get,4*length);
	send[4*length] = 0;
	send[4*length+1] = 0;
	send[4*length+2] = 0x80;
	send[4*length+3] = 0x7f;
	return (4*length+4);
}

uint16_t Volt_discode(uint8_t *src,float *data)
{
	uint16_t id;
	if(src[0] == 0x49)
	{
		id = src[1] - 0x30;
		data[id] = (float)((src[3]-0x30)*100 + (src[4]-0x30)*10 + (src[5]-0x30));
	}
	return id;
}
