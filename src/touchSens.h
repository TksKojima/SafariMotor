#ifndef _H_TOUCHSENS_
#define _H_TOUCHSENS_

#include <Arduino.h>


//touch T0-9 
// = [ 4, 0, 2, 15, 13,   12, 14, 27, 33, 32 ]


typedef struct touchSens
{
    int pinNum;
    int defaultVal;
    double ratio;
    void setup( int pinNum );
    int  getTouch();

}touchSens;


#endif


