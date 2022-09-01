#include "dataBuffer.h"


void dataBuffer::init( int _sampleNum ){
    size = BUF_SIZE;
    index = 0;
    setSampleNum( _sampleNum );

}

void  dataBuffer::setSampleNum( int _sampleNum ){
    sampleNum = _sampleNum;
    if( size < sampleNum ){
        sampleNum = -1;
    }
}

void dataBuffer::setBuf( double val ){
    buf[ index ] = val;
    index++;
    if( index >= size ){
        index = 0;
    }

    if( sampleNum > 0 ){
        sum += getBuf( 0 );
        sum -= getBuf( sampleNum );
        average = sum / sampleNum;

    }
    else{
        sum = 0;
        average = 0;
    }
}

double dataBuffer::getBuf( int num ){ // num��0���ƍŐV�l�A1���ƍŐV��1�O���A2����2�O��Ԃ�

    int getIndex = index - 1 - num;
    if( getIndex < 0 ){
        getIndex += size;
    }

    if( getIndex < 0 ) return -1000;
    if( getIndex >= size ) return -1000;

    return buf[ getIndex ];

}

double dataBuffer::getSampleSum(){ return sum; }
double dataBuffer::getSampleAverage(){ return average; }

double dataBuffer::getSampleMin(){ return min; }
double dataBuffer::getSampleMax(){ return max; }
double dataBuffer::getSampleMinMaxDif(){ return minmaxDif; }

//�@1���[�v��1�Ăяo���܂ł���
double dataBuffer::calcSampleMinMaxDif(){
    min = getBuf(0);
    max = getBuf(0);
    for( int i=1; i<sampleNum; i++ ){
        double bufval = getBuf(i);
        if( bufval > max ){
            max = bufval;
        }
        if( bufval < min ){
            min = bufval;
        }

    }
    minmaxDif = max - min;
    return 0;
}


