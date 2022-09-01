#include "imuDataProc.h"



//　制御周期と、平均等のサンプリング時間(1秒=1000を想定)
void imuDataProc::setup( int _CtrlCycle, int _sampleMilliSec ){

    ctrlCycle = _CtrlCycle;
    sampleMilliSec = _sampleMilliSec;

    sampleNum = sampleMilliSec / ctrlCycle; // 時間と周期から点数を取得

    bufSpeed.init( sampleNum );

    bufGyroX.init( sampleNum );
    bufGyroY.init( sampleNum );
    bufGyroZ.init( sampleNum );
    bufGyroOffsetedX.init( sampleNum );
    bufGyroOffsetedY.init( sampleNum );
    bufGyroOffsetedZ.init( sampleNum );

    //モータ指令値計算用 角速度の閾値
    gyroValThresh50  = 50; // モータ指令値を50%にするときの角速度の閾値
    gyroValThresh100 = gyroValThresh50 * 1.5; //100%

    // 平均値が大きいときに指令値を小さくする閾値
    gyroAveThresh = 5;  // この値からゲインを小さくし始めて、この値+1でゲイン0


    
}

// バッファを更新する loopから呼び出す
void imuDataProc::setBuf( double speed, double gyroX, double gyroY, double gyroZ ){
    bufSpeed.setBuf( speed );

    bufGyroX.setBuf( gyroX );
    bufGyroY.setBuf( gyroY );
    bufGyroZ.setBuf( gyroZ );
    bufGyroOffsetedX.setBuf( gyroX - offsetGyroX );
    bufGyroOffsetedY.setBuf( gyroY - offsetGyroY );
    bufGyroOffsetedZ.setBuf( gyroZ - offsetGyroZ );
}

//指令速度(speed)が小さいときにジャイロオフセット値を取得
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

//角速度の値からモータ指令値の算出
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


    // 平均値が一定以上の場合、振動ではなく回転運動をしているとして、指令をしない処理
    if( gyroAve <= gyroAveThresh ){ //平均値が閾値以下の場合（＝0付近で振動を想定） ゲインは1
        gyroAveGain = 1;

    }else if( gyroAve > (gyroAveThresh + 1) ){ //閾値+1より大きい場合はゲイン0  振動ではなく坂道等の動作。モータを動作させない
        gyroAveGain = 0;

    }else{ // 値は 閾値以上、閾値+1以下 なので 漸減区間
        gyroAveGain = gyroAve - gyroAveThresh;
    }

    double motorCmd0=0;
    //閾値50％ 以下の場合は 指令0、 閾値100％ 以上の場合は指令値100、 そうでない場合は線形補間
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


// loopで呼び出す
void imuDataProc::loop( double speed, double gyroX, double gyroY, double gyroZ ){

    setBuf( speed, gyroX, gyroY, gyroZ ); //オフセットされてない生値をセット
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

