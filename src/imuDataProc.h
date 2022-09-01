#ifndef _H_IMUDATAPROC_
#define _H_IMUDATAPROC_

#include <Arduino.h>
#include <dataBuffer.h>

// ������ێ����邽�߂̍\���̂̒�`
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

    /// ���[�^�R�}���h�p
    double gyroVal;
    double gyroValY;
    double gyroValZ;
    double gyroAve;
    double gyroAveY;
    double gyroAveZ;
    double gyroAveGain;

    double gyroValThresh50;  // ���[�^50����臒l
    double gyroValThresh100; // ���[�^100����臒l
    double gyroAveThresh; 
    double motorCmd;

    double gyro2motorCmd();



}imuDataProc;




#endif
