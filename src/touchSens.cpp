#include <touchSens.h>


void touchSens::setup( int pin){
    pinNum = pin;
    delay(50);
    defaultVal = touchRead( pinNum);

}


int touchSens::getTouch(){

    if( defaultVal == 0 ){
        defaultVal = touchRead( pinNum);
    }else{
        ratio = (double)touchRead( pinNum ) / (double)defaultVal;
    }

    // Serial.print( "Touch: " );
    // Serial.print( touchRead(15) );
    // Serial.print( ", " );
    // Serial.print( defaultVal );
    // Serial.print( ", " );
    // Serial.println( ratio );


    if( ratio < 0.7 ){
        return 1;
    }else{
        return 0;
    }

}











