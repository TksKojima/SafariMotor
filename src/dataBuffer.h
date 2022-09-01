#ifndef _H_DATABUFFER_
#define _H_DATABUFFER_

#include <Arduino.h>

#define BUF_SIZE 100

typedef struct dataBuffer
{
    int size;
    int index;
    double buf[ BUF_SIZE ];
    void init( int _sampleNum );
    void setBuf( double val );
    double getBuf( int num );

    void setSampleNum( int num);
    int sampleNum;
    double sum;
    double average;
    double min;    
    double max;
    double minmaxDif;
    double getSampleSum();
    double getSampleAverage();

    double calcSampleMinMaxDif();
    double getSampleMinMaxDif();
    double getSampleMin();
    double getSampleMax();
    

}dataBuffer;



#endif
