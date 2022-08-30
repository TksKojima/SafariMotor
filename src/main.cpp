#include <Arduino.h>

#include <Wire.h>
#include "mpu6050.h"

#include  <WiFi.h>
#include  <WiFiClient.h>
#include  <WebServer.h>
#include  <ESPmDNS.h>
#include <Ps3Controller.h>

#include "AsyncUDP.h"
#include <WiFiUdp.h>

#include  "defhtml.h"
 
const char* ssid = "wifi";
const char* password = "12345678";

// servo setting
const int servoChHead = 0;
const int servoPinNum = 4;
const int servoPin[] = {A15, A14, A16, A13}; //D12,D13,D14,D15
int    servoFreq = 50; //Hz
double servoCycle = 1000.0/servoFreq; 
int    servoFactorBit = 16;
double  servoZeroOffset = 11;
double  pwm_m90deg = 65535.0 * 0.50 / servoCycle; //-90deg  servo sg90 
double  pwm_0deg   = 65535.0 * 1.45 / servoCycle; //  0deg  servo sg90 
double  pwm_90deg  = 65535.0 * 2.40 / servoCycle; //+90deg  servo sg90 

// driveMotor setting
const int motorChHead = servoPinNum;
const double motorDutyMax = 100.0;
const int motorPinNum = 6;
const int motorPin[] = {A12, A10, A4, A5, A19, A7}; //D2,4,D32,33,D26,27,
int    motorFreq = 50; //Hz
double motorCycle = 1000.0/servoFreq; 
int    motorFactorBit = 16;

// ps3 pad
int ps3_battery = 0;

int fow_input = 0;
int st_input = 0;
int fow_input2 = 0;
int st_input2 = 0;


WebServer server(80);
const int led = 4;

AsyncUDP udp;


mpu6050 imu;


void handleRoot() { //ブラウザのUI
  server.send(200, "text/html", index_html); 
}


void handleRC() { //ブラウザのUIを操作した結果のJSからアクセスされる
  for (int i = 0; i < server.args(); i++) {
    int Val_i = server.arg(i).toInt();
    Serial.print(server.argName(i) + "=" + server.arg(i) + ", ");
    
    switch (server.argName(i)[0]) {
      case 'F': fow_input = Val_i; break;
      case 'S': st_input  = Val_i; break;
    }
  }
  Serial.println();
  server.send(200, "text/plain", "\n\n\n");
}
 

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup_pin(){
//  pinMode(led, OUTPUT);
//  digitalWrite(led, LOW);

}

void setup_bt_mac_addres() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  uint8_t btmac[6];
  esp_read_mac(btmac, ESP_MAC_BT);
  Serial.printf("[Bluetooth] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", btmac[0], btmac[1], btmac[2], btmac[3], btmac[4], btmac[5]);

}

int analog2val( int analogval ){
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
       st_input  = analog2val( ps3_analogLX );
       fow_input = analog2val( ps3_analogLY )*-1;

    }

   if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
       Serial.print("Moved the right stick:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
       Serial.println();
       ps3_analogRY  = Ps3.data.analog.stick.ry;
       ps3_analogRX = Ps3.data.analog.stick.rx;
       st_input2  = analog2val( ps3_analogRX );
       fow_input2 = analog2val( ps3_analogRY )*-1;

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

void ps3_onConnect(){
    Serial.println("Connected.");
}

void setup_ps3pad()
{
    Serial.begin(115200);
    setup_bt_mac_addres();

    Ps3.attach(ps3_notify);
    Ps3.attachOnConnect(ps3_onConnect);
    Ps3.begin("F0:08:D1:D8:29:FE");

    Serial.println("Ready.");
}


void setup_wifi(){
 //WiFi.mode(WIFI_STA);
 WiFi.mode(WIFI_AP);
 //WiFi.begin(ssid, password);
 WiFi.softAP(ssid, password, 3, 0, 4);
 delay(200);
 WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
   
 Serial.println("");
 Serial.print("AP IP address: ");
 Serial.print(WiFi.softAPIP());
 Serial.println();
 delay(100);
   
 if (MDNS.begin("esp32led")) {
   Serial.println("MDNS responder started");
 }
 server.on("/", handleRoot);
 server.on("/rc", handleRC);
 
 server.on("/inline", []() {
  server.send(200, "text/plain", "hello from esp8266!");
 });
 server.onNotFound(handleNotFound);
 server.begin();
 Serial.println("HTTP server started");


}

void setup_pwm(){
}

void setup_servo_pwm(){
  for( int i=0; i<servoPinNum; i++ ){
    ledcSetup( i, servoFreq, servoFactorBit);
    ledcAttachPin(servoPin[i], i);
  }
}

void setup_motor_pwm(){
  for( int i=0; i<motorPinNum; i++ ){
    ledcSetup( i + motorChHead, motorFreq, motorFactorBit);
    ledcAttachPin(motorPin[i], i + motorChHead);
  }  
}

void setup_UdpRecv(){

    if(udp.listen(10000)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();

            int recvValNum  = 4; //全部同じバイト数のデータを想定
            int recvValByte; 
            recvValByte = packet.length() / recvValNum;

            uint8_t *byteval8 = packet.data();
            uint8_t  byteval8_fix[ recvValNum * recvValByte ];
            uint32_t temp[] = {0};

            for( int i=0; i<recvValNum;  i++ ){
                // byteval8_fix[i+ 0] = *(byteval8+3-i+0);
                // byteval8_fix[i+ 4] = *(byteval8+3-i+4);
                // byteval8_fix[i+ 8] = *(byteval8+3-i+8);
                for( int j=0; j<recvValByte;  j++ ){
                  byteval8_fix[j+i*recvValByte] = *(byteval8 + recvValByte - 1 - j + i*recvValByte);
                }

            }
            for( int i=0; i<packet.length(); i++ ){
                //Serial.print( byteval8_fix[i], HEX);

            }
            uint32_t *byteval32 = (uint32_t *)byteval8_fix;
            
            // Serial.println();
            // Serial.print( *(byteval32+0), HEX );
            // Serial.print(", ");
            // Serial.print( *(byteval32+1), HEX );
            // Serial.print(", ");
            // Serial.print( *(byteval32+2), HEX );
            // Serial.print(", ");
            // Serial.println( *(byteval32+3), HEX );
            
            Serial.print("ESP32 Recv: ");
            Serial.print( ((int)(*(byteval32+0))));
            Serial.print(", ");
            Serial.print( ((int)(*(byteval32+1))));
            Serial.print(", ");
            Serial.print( ((int)(*(byteval32+2))));
            Serial.print(", ");
            Serial.println( ((int)(*(byteval32+3))));

            //Serial.println();
            //reply to the client
            packet.printf("Got %u bytes of data", packet.length());

            st_input = (int)(*(byteval32+0));
        });
    }

}

int calcServoDeg2Pwm(  double inputDeg ){
  double inputDegComp = 0;
  // if( inputDeg < -90 ){
  //   inputDeg = -90;
  // }
  // else if( inputDeg > 90 ){
  //   inputDeg = 90;
  // }

  // オフセット=11で見た目 0deg
  //-97＋オフセットで見た目-90deg
  //+93＋オフセットで見た目+90deg
  if( inputDeg > 0 ){
    inputDegComp = inputDeg * 93 / 90;
  }
  if( inputDeg < 0 ){
    inputDegComp = inputDeg * 97 / 90;
  }

  return ( inputDegComp + servoZeroOffset ) * ( pwm_90deg - pwm_m90deg ) / 180.0 + pwm_0deg;

}

int calcMotor2Pwm(  double inputFow ){ //入力=±100

  return  (int)(inputFow * 65535.0 / 100.0);

}



void loop_pwm(){
  static uint8_t brightness = 0;
  brightness = ( fow_input + 100 )/2;
  //ledcWrite(0, brightness);
  
  dacWrite( 25, max( abs(fow_input), abs(st_input) ) * 255 /100 );
}

void loop_servo_pwm(){   
  double st_lpf_gain = 0.95; 
  static double st_lpfed = 0;;
  double st_lpfed0, st_lpfed1, st_lpfed2, st_lpfed3;

  //st_lpfed = st_lpfed * st_lpf_gain + st_input * (1 - st_lpf_gain);
  st_lpfed = st_lpfed * st_lpf_gain + (double)st_input * (1.0 - st_lpf_gain);

  st_lpfed0 = st_lpfed;
  st_lpfed1 = st_lpfed;
  st_lpfed2 = st_lpfed0*-1;
  st_lpfed3 = st_lpfed1*-1;

  int inputPwm0 = calcServoDeg2Pwm( st_lpfed0 );
  int inputPwm1 = calcServoDeg2Pwm( st_lpfed1 );
  int inputPwm2 = calcServoDeg2Pwm( st_lpfed2 );
  int inputPwm3 = calcServoDeg2Pwm( st_lpfed3 );


  //Serial.print("inputPwm: ");
  //Serial.println(inputPwm);
  ledcWrite(0, inputPwm0 );
  ledcWrite(1, inputPwm1 );
  ledcWrite(2, inputPwm2 );
  ledcWrite(3, inputPwm3 );

}

void loop_motor_pwm(){   
  int inputPwm = calcMotor2Pwm(  abs( (double)( fow_input ) ) );
  //Serial.print("inputPwm: ");
  //Serial.println(inputPwm);
  for( int i=0; i<motorPinNum; i=i+2 ){
    if( fow_input >= 0 ){
      ledcWrite( i + motorChHead     , inputPwm );
      ledcWrite( i + motorChHead + 1 , 0 );
      
    }else{
      ledcWrite( i + motorChHead     , 0 );
      ledcWrite( i + motorChHead + 1 , inputPwm );
    }
  }
  if( abs(inputPwm) > 1 ){

      Serial.print("PWM: ");
      Serial.println( inputPwm );
  }


}

void setup(void) {

 Serial.begin(115200);

 imu.setup();

 setup_pin();
 setup_wifi();
 setup_UdpRecv();
 setup_servo_pwm();
 setup_motor_pwm();
 setup_ps3pad();

}


void loop(void) {
 static unsigned long curr_prev=0;
 unsigned long curr = millis(); // 現在時刻を更新

 if( curr - curr_prev < 10 ){
    return;
 }
 curr_prev = curr;
 server.handleClient();
 
 imu.loop();

 loop_pwm();
 loop_servo_pwm();
 //loop_motor_pwm();
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



