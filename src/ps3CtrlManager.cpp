#include "ps3CtrlManager.h"

// ps3 pad
int ps3_battery = 0;
int inputRX;
int inputRY;
int inputLX;
int inputLY;

void ps3_onConnect(){
    Serial.println("Connected.");
}

void ps3_setup()
{
    ps3_setup_bt_mac_addres();

    Ps3.attach(ps3_notify);
    Ps3.attachOnConnect(ps3_onConnect);
    Ps3.begin("F0:08:D1:D8:29:FE");
    Serial.println("Ready.");

}


void ps3_setup_bt_mac_addres() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  uint8_t btmac[6];
  esp_read_mac(btmac, ESP_MAC_BT);
  Serial.println("");
  Serial.printf("[Bluetooth] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", btmac[0], btmac[1], btmac[2], btmac[3], btmac[4], btmac[5]);

}


int ps3_analog2val( int analogval ){
  int ret;

  ret = (analogval*100/127);
  if( ret >= 100 ){ ret = 100; }
  else if( ret <= -100 ){ ret = -100; }

  return ret;
}

void ps3_notify()
{

    // //--- Digital cross/square/triangle/circle button events ---
    // if( Ps3.event.button_down.cross ) Serial.println("Started pressing the cross button");
    // if( Ps3.event.button_up.cross ) Serial.println("Released the cross button");
    // if( Ps3.event.button_down.square ) Serial.println("Started pressing the square button");
    // if( Ps3.event.button_up.square ) Serial.println("Released the square button");
    // if( Ps3.event.button_down.triangle )  Serial.println("Started pressing the triangle button");
    // if( Ps3.event.button_up.triangle )  Serial.println("Released the triangle button");
    // if( Ps3.event.button_down.circle )  Serial.println("Started pressing the circle button");
    // if( Ps3.event.button_up.circle )  Serial.println("Released the circle button");

    // //--------------- Digital D-pad button events --------------
    // if( Ps3.event.button_down.up ) Serial.println("Started pressing the up button");
    // if( Ps3.event.button_up.up )  Serial.println("Released the up button");
    // if( Ps3.event.button_down.right )  Serial.println("Started pressing the right button");
    // if( Ps3.event.button_up.right )  Serial.println("Released the right button");
    // if( Ps3.event.button_down.down )  Serial.println("Started pressing the down button");
    // if( Ps3.event.button_up.down )  Serial.println("Released the down button");
    // if( Ps3.event.button_down.left )  Serial.println("Started pressing the left button");
    // if( Ps3.event.button_up.left )  Serial.println("Released the left button");

    // //------------- Digital shoulder button events -------------
    // if( Ps3.event.button_down.l1 )  Serial.println("Started pressing the left shoulder button");
    // if( Ps3.event.button_up.l1 )  Serial.println("Released the left shoulder button");
    // if( Ps3.event.button_down.r1 )  Serial.println("Started pressing the right shoulder button");
    // if( Ps3.event.button_up.r1 )  Serial.println("Released the right shoulder button");

    // //-------------- Digital trigger button events -------------
    // if( Ps3.event.button_down.l2 )  Serial.println("Started pressing the left trigger button");
    // if( Ps3.event.button_up.l2 )  Serial.println("Released the left trigger button");
    // if( Ps3.event.button_down.r2 )  Serial.println("Started pressing the right trigger button");
    // if( Ps3.event.button_up.r2 )  Serial.println("Released the right trigger button");

    // //--------------- Digital stick button events --------------
    // if( Ps3.event.button_down.l3 )  Serial.println("Started pressing the left stick button");
    // if( Ps3.event.button_up.l3 )  Serial.println("Released the left stick button");

    // if( Ps3.event.button_down.r3 )  Serial.println("Started pressing the right stick button");
    // if( Ps3.event.button_up.r3 )  Serial.println("Released the right stick button");

    // //---------- Digital select/start/ps button events ---------
    // if( Ps3.event.button_down.select )  Serial.println("Started pressing the select button");
    // if( Ps3.event.button_up.select )  Serial.println("Released the select button");

    // if( Ps3.event.button_down.start )  Serial.println("Started pressing the start button");
    // if( Ps3.event.button_up.start )  Serial.println("Released the start button");

    // if( Ps3.event.button_down.ps )  Serial.println("Started pressing the Playstation button");
    // if( Ps3.event.button_up.ps )  Serial.println("Released the Playstation button");

    int ps3_analogLX=0;
    int ps3_analogLY=0;
    int ps3_analogRX=0;
    int ps3_analogRY=0;


    //---------------- Analog stick value events ---------------
   if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){
       Serial.print("Moved the left stick:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       ps3_analogLY  = Ps3.data.analog.stick.ly;
       ps3_analogLX = Ps3.data.analog.stick.lx;
       inputLX  = ps3_analog2val( ps3_analogLX );
       inputLY = ps3_analog2val( ps3_analogLY )*-1;

    }

   if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
       Serial.print("Moved the right stick:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
       Serial.println();
       ps3_analogRY  = Ps3.data.analog.stick.ry;
       ps3_analogRX = Ps3.data.analog.stick.rx;
       inputRX  = ps3_analog2val( ps3_analogRX );
       inputRY = ps3_analog2val( ps3_analogRY )*-1;

   }



   //---------------------- Battery events ---------------------
    if( ps3_battery != Ps3.data.status.battery ){
        ps3_battery = Ps3.data.status.battery;
        Serial.print("The controller battery is ");
        if( ps3_battery == ps3_status_battery_charging )      Serial.println("charging");
        else if( ps3_battery == ps3_status_battery_full )     Serial.println("FULL");
        else if( ps3_battery == ps3_status_battery_high )     Serial.println("HIGH");
        else if( ps3_battery == ps3_status_battery_low)       Serial.println("LOW");
        else if( ps3_battery == ps3_status_battery_dying )    Serial.println("DYING");
        else if( ps3_battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
        else Serial.println("UNDEFINED");
    }

}



// void ps3_notify()
// {
//     //--- Digital cross/square/triangle/circle button events ---
//     if( Ps3.event.button_down.cross ) Serial.println("Started pressing the cross button");
//     if( Ps3.event.button_up.cross ) Serial.println("Released the cross button");
//     if( Ps3.event.button_down.square ) Serial.println("Started pressing the square button");
//     if( Ps3.event.button_up.square ) Serial.println("Released the square button");
//     if( Ps3.event.button_down.triangle )  Serial.println("Started pressing the triangle button");
//     if( Ps3.event.button_up.triangle )  Serial.println("Released the triangle button");
//     if( Ps3.event.button_down.circle )  Serial.println("Started pressing the circle button");
//     if( Ps3.event.button_up.circle )  Serial.println("Released the circle button");

//     //--------------- Digital D-pad button events --------------
//     if( Ps3.event.button_down.up ) Serial.println("Started pressing the up button");
//     if( Ps3.event.button_up.up )  Serial.println("Released the up button");
//     if( Ps3.event.button_down.right )  Serial.println("Started pressing the right button");
//     if( Ps3.event.button_up.right )  Serial.println("Released the right button");
//     if( Ps3.event.button_down.down )  Serial.println("Started pressing the down button");
//     if( Ps3.event.button_up.down )  Serial.println("Released the down button");
//     if( Ps3.event.button_down.left )  Serial.println("Started pressing the left button");
//     if( Ps3.event.button_up.left )  Serial.println("Released the left button");

//     //------------- Digital shoulder button events -------------
//     if( Ps3.event.button_down.l1 )  Serial.println("Started pressing the left shoulder button");
//     if( Ps3.event.button_up.l1 )  Serial.println("Released the left shoulder button");
//     if( Ps3.event.button_down.r1 )  Serial.println("Started pressing the right shoulder button");
//     if( Ps3.event.button_up.r1 )  Serial.println("Released the right shoulder button");

//     //-------------- Digital trigger button events -------------
//     if( Ps3.event.button_down.l2 )  Serial.println("Started pressing the left trigger button");
//     if( Ps3.event.button_up.l2 )  Serial.println("Released the left trigger button");
//     if( Ps3.event.button_down.r2 )  Serial.println("Started pressing the right trigger button");
//     if( Ps3.event.button_up.r2 )  Serial.println("Released the right trigger button");

//     //--------------- Digital stick button events --------------
//     if( Ps3.event.button_down.l3 )  Serial.println("Started pressing the left stick button");
//     if( Ps3.event.button_up.l3 )  Serial.println("Released the left stick button");

//     if( Ps3.event.button_down.r3 )  Serial.println("Started pressing the right stick button");
//     if( Ps3.event.button_up.r3 )  Serial.println("Released the right stick button");

//     //---------- Digital select/start/ps button events ---------
//     if( Ps3.event.button_down.select )  Serial.println("Started pressing the select button");
//     if( Ps3.event.button_up.select )  Serial.println("Released the select button");

//     if( Ps3.event.button_down.start )  Serial.println("Started pressing the start button");
//     if( Ps3.event.button_up.start )  Serial.println("Released the start button");

//     if( Ps3.event.button_down.ps )  Serial.println("Started pressing the Playstation button");
//     if( Ps3.event.button_up.ps )  Serial.println("Released the Playstation button");


//     //---------------- Analog stick value events ---------------
//    if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){
//        Serial.print("Moved the left stick:");
//        Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
//        Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
//        Serial.println();
//     }

//    if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
//        Serial.print("Moved the right stick:");
//        Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
//        Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
//        Serial.println();
//    }

//    //--------------- Analog D-pad button events ----------------
//    if( abs(Ps3.event.analog_changed.button.up) ){
//        Serial.print("Pressing the up button: ");
//        Serial.println(Ps3.data.analog.button.up, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.right) ){
//        Serial.print("Pressing the right button: ");
//        Serial.println(Ps3.data.analog.button.right, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.down) ){
//        Serial.print("Pressing the down button: ");
//        Serial.println(Ps3.data.analog.button.down, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.left) ){
//        Serial.print("Pressing the left button: ");
//        Serial.println(Ps3.data.analog.button.left, DEC);
//    }

//    //---------- Analog shoulder/trigger button events ----------
//    if( abs(Ps3.event.analog_changed.button.l1)){
//        Serial.print("Pressing the left shoulder button: ");
//        Serial.println(Ps3.data.analog.button.l1, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.r1) ){
//        Serial.print("Pressing the right shoulder button: ");
//        Serial.println(Ps3.data.analog.button.r1, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.l2) ){
//        Serial.print("Pressing the left trigger button: ");
//        Serial.println(Ps3.data.analog.button.l2, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.r2) ){
//        Serial.print("Pressing the right trigger button: ");
//        Serial.println(Ps3.data.analog.button.r2, DEC);
//    }

//    //---- Analog cross/square/triangle/circle button events ----
//    if( abs(Ps3.event.analog_changed.button.triangle)){
//        Serial.print("Pressing the triangle button: ");
//        Serial.println(Ps3.data.analog.button.triangle, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.circle) ){
//        Serial.print("Pressing the circle button: ");
//        Serial.println(Ps3.data.analog.button.circle, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.cross) ){
//        Serial.print("Pressing the cross button: ");
//        Serial.println(Ps3.data.analog.button.cross, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.square) ){
//        Serial.print("Pressing the square button: ");
//        Serial.println(Ps3.data.analog.button.square, DEC);
//    }

//    //---------------------- Battery events ---------------------
//     if( battery != Ps3.data.status.battery ){
//         battery = Ps3.data.status.battery;
//         Serial.print("The controller battery is ");
//         if( battery == ps3_status_battery_charging )      Serial.println("charging");
//         else if( battery == ps3_status_battery_full )     Serial.println("FULL");
//         else if( battery == ps3_status_battery_high )     Serial.println("HIGH");
//         else if( battery == ps3_status_battery_low)       Serial.println("LOW");
//         else if( battery == ps3_status_battery_dying )    Serial.println("DYING");
//         else if( battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
//         else Serial.println("UNDEFINED");
//     }

// }

















