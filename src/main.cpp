#include <Arduino.h>

#include <Wire.h>
#include "mpu6050.h"

#include <imuDataProc.h>
#include <dataBuffer.h>


#include "ps3CtrlManager.h"

#include "AsyncUDP.h"
#include "udpTxRx.h"
#include <WiFiUdp.h>

#include "wifi_Server.h"

 

//general
const int CTRL_CYCLE=50; //制御周期

//DAC
const int DAC_PIN1 = 25; //電圧出力ピン
const int DAC_PIN2 = 26;

//UDP Rx
const int udpRx_port   = 10001; //UDP受信ポート
const int udpRx_dataNum = 4; //UDPで受信する変数数(全部同じ型(バイト数)の変数前提)


mpu6050 imu;
imuDataProc imuProc;


void setup(void) {

  Serial.begin(115200);

  imu.setup();
  wifi_setup( WIFI_AP );
//  wifi_setup( WIFI_STA );
  
  udpRx_setup( udpRx_port, udpRx_dataNum );

  ps3_setup();

  imuProc.setup( CTRL_CYCLE, 1000 );

}


void loop_dac( double motorCmd ){
  dacWrite( DAC_PIN1, (int)(motorCmd * 255.0 / 100.0 ));
  dacWrite( DAC_PIN2, (int)(motorCmd * 255.0 / 100.0 ));

}

void loop(void) {
  static unsigned long curr_prev=0;
  unsigned long curr = millis(); // 現在時刻を更新

  if( curr - curr_prev < CTRL_CYCLE ){
    return;
  }
  curr_prev = curr;

  imu.loop();
  imuProc.loop( 0, imu.GyroX_dps, imu.GyroY_dps, imu.GyroZ_dps);
  //imuProc.loop( udpRx_getData(0), udpRx_getData(1), udpRx_getData(2), udpRx_getData(3) );

  loop_dac( imuProc.gyro2motorCmd() );

  wifi_loop();

}






