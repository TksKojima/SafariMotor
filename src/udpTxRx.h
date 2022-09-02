#ifndef _H_UDPTXRX_
#define _H_UDPTXRX_


// USAGE
// for( int i=0; i<udpRx_getDataNum; i++){
//  Serial.println( udpRx_getData(i) );
// }


#include <WiFi.h>
#include <Arduino.h>
#include "AsyncUDP.h"

#define RX_BUF_SIZE 30

extern AsyncUDP udp;
extern int listenPort;
extern int rxData[ RX_BUF_SIZE ];
extern int rxDataNum;

void udpRx_setup( int _listenPort, int _rxDataNum  );
int  udpRx_getDataNum();
int  udpRx_getData( int idx );

#endif


