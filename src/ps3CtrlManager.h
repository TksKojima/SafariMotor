#ifndef _H_PS3CTRLMANAGER_
#define _H_PS3CTRLMANAGER_

#include <Arduino.h>
#include <Ps3Controller.h>


void ps3_setup();
void ps3_setup_bt_mac_addres();
int  ps3_analog2val( int analogval );
void ps3_notify();


#endif



