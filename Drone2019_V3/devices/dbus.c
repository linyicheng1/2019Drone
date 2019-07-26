#include "dbus.h"

#include "string.h"
#include "board.h"


RC remoteControl;
uint32_t delta_T;
void Sbus_Decode(unsigned char *data)
{
	memcpy(remoteControl.dbus.raw,data,18);
//	if((remoteControl.dbus.msg.chan1-1024<15)&&(remoteControl.dbus.msg.chan1-1024>-15))remoteControl.dbus.msg.chan1=1024;
//	if((remoteControl.dbus.msg.chan2-1024<15)&&(remoteControl.dbus.msg.chan2-1024>-15)) remoteControl.dbus.msg.chan2=1024;
//	if((remoteControl.dbus.msg.chan3-1024<10)&&(remoteControl.dbus.msg.chan3-1024>-10)) remoteControl.dbus.msg.chan3=1024;
//	if((remoteControl.dbus.msg.chan4-1024<10)&&(remoteControl.dbus.msg.chan4-1024>-10)) remoteControl.dbus.msg.chan4=1024;
	delta_T = GetSysTimeMs()- remoteControl.updateTime;
	remoteControl.updateTime = GetSysTimeMs();
}

RC *getRCData(void)
{
	return &remoteControl;
}

uint8_t getRisingEdge(float value,float *recordValue)
{
	if(value == 1 && *recordValue == 0)
	{
		*recordValue = value;
		return 1;
	}
	else 
	{
		*recordValue = value;
		return 0;
	}
}
uint8_t getFallingEdge(float value,float *recordValue)
{
	if(value == 0 && *recordValue == 1)
	{
		*recordValue = value;
		return 1;
	}
	else 
	{
		*recordValue = value;
		return 0;
	}
}
