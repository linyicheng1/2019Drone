#include "dbus.h"

#include "string.h"
#include "board.h"


struct RC remoteControl;
void Sbus_Decode(unsigned char *data)
{
	memcpy(remoteControl.dbus.raw,data,18);
//	if((remoteControl.dbus.msg.chan1-1024<15)&&(remoteControl.dbus.msg.chan1-1024>-15))remoteControl.dbus.msg.chan1=1024;
//	if((remoteControl.dbus.msg.chan2-1024<15)&&(remoteControl.dbus.msg.chan2-1024>-15)) remoteControl.dbus.msg.chan2=1024;
//	if((remoteControl.dbus.msg.chan3-1024<10)&&(remoteControl.dbus.msg.chan3-1024>-10)) remoteControl.dbus.msg.chan3=1024;
//	if((remoteControl.dbus.msg.chan4-1024<10)&&(remoteControl.dbus.msg.chan4-1024>-10)) remoteControl.dbus.msg.chan4=1024;
	remoteControl.updateTime = GetSysTimeMs();
}


