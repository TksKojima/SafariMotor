#include "imuDataProc.h"



//�@��������ƁA���ϓ��̃T���v�����O����(1�b=1000��z��)
void imuDataProc::setup( int _CtrlCycle, int _sampleMilliSec ){

    ctrlCycle = _CtrlCycle;
    sampleMilliSec = _sampleMilliSec;

    sampleNum = sampleMilliSec / ctrlCycle; // ���ԂƎ�������_�����擾

    bufSpeed.init( sampleNum );

    bufGyroX.init( sampleNum );
    bufGyroY.init( sampleNum );
    bufGyroZ.init( sampleNum );
    bufGyroOffsetedX.init( sampleNum );
    bufGyroOffsetedY.init( sampleNum );
    bufGyroOffsetedZ.init( sampleNum );

    //���[�^�w�ߒl�v�Z�p �p���x��臒l
    gyroValThresh50  = 50; // ���[�^�w�ߒl��50%�ɂ���Ƃ��̊p���x��臒l
    gyroValThresh100 = gyroValThresh50 * 1.5; //100%

    // ���ϒl���傫���Ƃ��Ɏw�ߒl������������臒l
    gyroAveThresh = 5;  // ���̒l����Q�C�������������n�߂āA���̒l+1�ŃQ�C��0


    
}

// �o�b�t�@���X�V���� loop����Ăяo��
void imuDataProc::setBuf( double speed, double gyroX, double gyroY, double gyroZ ){
    bufSpeed.setBuf( speed );

    bufGyroX.setBuf( gyroX );
    bufGyroY.setBuf( gyroY );
    bufGyroZ.setBuf( gyroZ );
    bufGyroOffsetedX.setBuf( gyroX - offsetGyroX );
    bufGyroOffsetedY.setBuf( gyroY - offsetGyroY );
    bufGyroOffsetedZ.setBuf( gyroZ - offsetGyroZ );
}

//�w�ߑ��x(speed)���������Ƃ��ɃW���C���I�t�Z�b�g�l���擾
void imuDataProc::getGyroOffset(){
    double thresh = 5;

//    if( abs( bufSpeed.getSampleAverage() ) < 2 ){
    if( ( abs( bufGyroOffsetedX.minmaxDif ) < thresh ) &&
        ( abs( bufGyroOffsetedY.minmaxDif ) < thresh ) &&
        ( abs( bufGyroOffsetedZ.minmaxDif ) < thresh )  ){

        offsetGyroX = bufGyroX.getSampleAverage();
        offsetGyroY = bufGyroY.getSampleAverage();
        offsetGyroZ = bufGyroZ.getSampleAverage();        

    }
}

//�p���x�̒l���烂�[�^�w�ߒl�̎Z�o
double imuDataProc::gyro2motorCmd(){
    gyroValY = bufGyroOffsetedY.getSampleMinMaxDif();
    gyroValZ = bufGyroOffsetedZ.getSampleMinMaxDif(); 
    gyroVal = abs(gyroValY) + abs(gyroValZ) ;

    gyroAveY = bufGyroOffsetedY.getSampleAverage();
    gyroAveZ = bufGyroOffsetedZ.getSampleAverage();

    if( abs( gyroAveY ) >= abs( gyroValZ ) ){
        gyroAve = abs( gyroAveY );
    }else{
        gyroAve = abs( gyroAveZ );

    }


    // ���ϒl�����ȏ�̏ꍇ�A�U���ł͂Ȃ���]�^�������Ă���Ƃ��āA�w�߂����Ȃ�����
    if( gyroAve <= gyroAveThresh ){ //���ϒl��臒l�ȉ��̏ꍇ�i��0�t�߂ŐU����z��j �Q�C����1
        gyroAveGain = 1;

    }else if( gyroAve > (gyroAveThresh + 1) ){ //臒l+1���傫���ꍇ�̓Q�C��0  �U���ł͂Ȃ��⓹���̓���B���[�^�𓮍삳���Ȃ�
        gyroAveGain = 0;

    }else{ // �l�� 臒l�ȏ�A臒l+1�ȉ� �Ȃ̂� �Q�����
        gyroAveGain = gyroAve - gyroAveThresh;
    }

    double motorCmd0=0;
    //臒l50�� �ȉ��̏ꍇ�� �w��0�A 臒l100�� �ȏ�̏ꍇ�͎w�ߒl100�A �����łȂ��ꍇ�͐��`���
    if( gyroVal < gyroValThresh50  ){ 
        motorCmd0 = 0;

    }else if( gyroVal > gyroValThresh100 ){
        motorCmd0 = 100;

    }else{
        motorCmd0 = 50 * ( gyroVal -gyroValThresh50 ) / ( gyroValThresh100 -gyroValThresh50 ) + 50;
    }

    motorCmd = motorCmd0 * gyroAveGain;


    return motorCmd;

}


// loop�ŌĂяo��
void imuDataProc::loop( double speed, double gyroX, double gyroY, double gyroZ ){

    setBuf( speed, gyroX, gyroY, gyroZ ); //�I�t�Z�b�g����ĂȂ����l���Z�b�g
    getGyroOffset();
    bufGyroOffsetedX.calcSampleMinMaxDif();
    bufGyroOffsetedY.calcSampleMinMaxDif();
    bufGyroOffsetedZ.calcSampleMinMaxDif();


    if( 0 ){
        Serial.print("motor: "); 
        Serial.print( gyro2motorCmd() );
        Serial.print(", gyroAve: "); 
        Serial.print( gyroAve );
        Serial.print(", gyroAveGain: "); 
        Serial.print( gyroAveGain );

        Serial.print(", xoft: "); 
        Serial.print( bufGyroOffsetedX.getSampleAverage() );
        Serial.print(", yofst: "); 
        Serial.print( bufGyroOffsetedY.getSampleAverage() );
        Serial.print(", zofst: "); 
        Serial.print( bufGyroOffsetedZ.getSampleAverage() );

        Serial.print(", xmm: "); 
        Serial.print( bufGyroOffsetedX.getSampleMinMaxDif() );
        Serial.print(", ymm: "); 
        Serial.print( bufGyroOffsetedY.getSampleMinMaxDif() );
        Serial.print(", zmm: "); 
        Serial.print( bufGyroOffsetedZ.getSampleMinMaxDif() );

        Serial.println(""); 
    }

}

