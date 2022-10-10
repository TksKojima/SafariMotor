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

#include "touchSens.h"
 

//general
const int CTRL_CYCLE=50; //�������

//DAC
const int DAC_PIN1 = 25; //�d���o�̓s��
const int DAC_PIN2 = 26;

//Touch
const int TOUCH_PIN = 15; //


//UDP Rx
const int udpRx_port   = 10001; //UDP��M�|�[�g
const int udpRx_dataNum = 3; //UDP�Ŏ�M����ϐ���(�S�������^(�o�C�g��)�̕ϐ��O��)


mpu6050 imu;
imuDataProc imuProc;
touchSens touch;

void setup(void) {

  Serial.begin(115200);

  imu.setup();
  
  ps3_setup();
  imuProc.setup( CTRL_CYCLE, 1000 );

  touch.setup( TOUCH_PIN );

  //wifi_setup( WIFI_AP );  //������Wifi���M
  wifi_setup( WIFI_STA );  //���[�^����Wifi���M

  udpRx_setup( udpRx_port, udpRx_dataNum );

}


void loop_dac( double motorCmd, double motorCmdMax ){
  // dacWrite( DAC_PIN1, (int)(motorCmd * 255.0 / 100.0 ));
  // dacWrite( DAC_PIN2, (int)(motorCmd * 255.0 / 100.0 ));

  dacWrite( DAC_PIN1, (int)(motorCmd * 255.0 / 100.0 ));
  dacWrite( DAC_PIN2, (int)(motorCmd * 255.0 / motorCmdMax)); // LED�p

}

void loop(void) {
  static unsigned long curr_prev=0;
  unsigned long curr = millis(); // ���ݎ������X�V

  if( curr - curr_prev < CTRL_CYCLE ){
    return;
  }
  curr_prev = curr;

  imu.loop();

  //imuProc.loop( 0, imu.GyroX_dps, imu.GyroY_dps, imu.GyroZ_dps); //�����̃W���C��
  imuProc.loop( 0, udpRx_getData(0), udpRx_getData(1), udpRx_getData(2) );  //���W�R������̃W���C��


  double motorCmd = 0;

  if( touch.getTouch() == 1){
    motorCmd = imuProc.motorCmdMaxVal;
  }else{
    motorCmd = imuProc.gyro2motorCmd();
  }

  loop_dac( motorCmd, imuProc.motorCmdMaxVal );

  wifi_loop();

  //if(      plotMode == 0 ) wifi_data_loop( imu.GyroX_dps, imu.GyroY_dps, imu.GyroZ_dps );
  if(      plotMode == 0 ) wifi_data_loop( udpRx_getData(0), udpRx_getData(1), udpRx_getData(2) );
  else if( plotMode == 1 ) wifi_data_loop( imuProc.bufGyroOffsetedX.getSampleAverage(), imuProc.bufGyroOffsetedY.getSampleAverage(), imuProc.bufGyroOffsetedZ.getSampleAverage() );
  else if( plotMode == 2 ) wifi_data_loop( imuProc.bufGyroOffsetedX.getSampleMinMaxDif(), imuProc.bufGyroOffsetedY.getSampleMinMaxDif(), imuProc.bufGyroOffsetedZ.getSampleMinMaxDif() );
  else if( plotMode == 3 ) wifi_data_loop( imuProc.gyroVal, imuProc.motorCmd, imuProc.gyroAveGain*100 );
  else if( plotMode == 4 ) wifi_data_loop( imuProc.gyroVal, imuProc.motorCmd, imuProc.gyroValThresh50 );

  if( dat0_input != 0 ){
    imuProc.reloadThresh( imuProc.gyroValThresh50 + dat0_input );
    dat0_input = 0;
  }


}






