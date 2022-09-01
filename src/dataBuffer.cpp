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

double dataBuffer::getBuf( int num ){ // numが0だと最新値、1だと最新の1つ前を、2だと2つ前を返す

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

//　1ループで1呼び出しまでかな
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


