#ifndef _H_IMUDATAPROC_
#define _H_IMUDATAPROC_

#include <Arduino.h>
#include <dataBuffer.h>

// 属性を保持するための構造体の定義
typedef struct imuDataProc
{

    dataBuffer bufSpeed;
    dataBuffer bufGyroX;
    dataBuffer bufGyroY;
    dataBuffer bufGyroZ;
    dataBuffer bufGyroOffsetedX;
    dataBuffer bufGyroOffsetedY;
    dataBuffer bufGyroOffsetedZ;

    double offsetGyroX;
    double offsetGyroY;
    double offsetGyroZ;

    int sampleNum;
    int sampleMilliSec;
    int ctrlCycle;
    void setup( int _CtrlCycle, int _sampleMilliSec );
    void loop( double speed, double gyroX, double gyroY, double gyroZ );
    void setBuf( double speed, double gyroX, double gyroY, double gyroZ );
    void getGyroOffset();

    /// モータコマンド用
    double gyroVal;
    double gyroValY;
    double gyroValZ;
    double gyroAve;
    double gyroAveY;
    double gyroAveZ;
    double gyroAveGain;

    double gyroValThresh50;  // モータ50％の閾値
    double gyroValThresh100; // モータ100％の閾値
    double gyroAveThresh; 
    double motorCmd;

    double gyro2motorCmd();



}imuDataProc;




#endif
