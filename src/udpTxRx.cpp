#include "udpTxRx.h"

// USAGE
// for( int i=0; i<udpRx_getDataNum; i++){
//  Serial.println( udpRx_getData(i) );
// }

AsyncUDP udp;
int listenPort;
int rxData[ RX_BUF_SIZE ];
int rxDataNum;

int udpRx_getDataNum(){ return rxDataNum; }
int udpRx_getData( int idx ){ return rxData[idx]; };


void udpRx_setup( int _listenPort, int _rxDataNum ){
    listenPort = _listenPort;
    rxDataNum = _rxDataNum;
    if(udp.listen( _listenPort )) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();

            int rxDataByte; 
            rxDataByte = packet.length() / rxDataNum;

            uint8_t *byteval8 = packet.data();
            uint8_t  byteval8_fix[ rxDataNum * rxDataByte ];

            for( int i=0; i<rxDataNum;  i++ ){
                // byteval8_fix[i+ 0] = *(byteval8+3-i+0);
                // byteval8_fix[i+ 4] = *(byteval8+3-i+4);
                // byteval8_fix[i+ 8] = *(byteval8+3-i+8);
                for( int j=0; j<rxDataByte;  j++ ){
                    byteval8_fix[j+i*rxDataByte] = *(byteval8 + rxDataByte - 1 - j + i*rxDataByte);
                }

            }
            for( int i=0; i<packet.length(); i++ ){
                //Serial.print( byteval8_fix[i], HEX);
            }
            uint32_t *byteval32 = (uint32_t *)byteval8_fix;
            
            // Serial.println();
            // Serial.print( *(byteval32+0), HEX );
            // Serial.print(", ");
            // Serial.print( *(byteval32+1), HEX );
            // Serial.print(", ");
            // Serial.print( *(byteval32+2), HEX );
            // Serial.print(", ");
            // Serial.println( *(byteval32+3), HEX );
            

            //int rxData[10];
            Serial.print("ESP32 Recv: ");
            for( int i=0; i<rxDataNum; i++){
                rxData[i] =  ((int)(*(byteval32+i)));
                Serial.print( rxData[i] );
                Serial.print(", ");
            }
            Serial.println("");

            // Serial.print("ESP32 Recv: ");
            // Serial.print( ((int)(*(byteval32+0))));
            // Serial.print(", ");
            // Serial.print( ((int)(*(byteval32+1))));
            // Serial.print(", ");
            // Serial.print( ((int)(*(byteval32+2))));
            // Serial.print(", ");
            // Serial.println( ((int)(*(byteval32+3))));

        });

    }

}







