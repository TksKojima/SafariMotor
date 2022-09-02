#include <Arduino.h>

#include <Wire.h>
#include "mpu6050.h"

#include <imuDataProc.h>
#include <dataBuffer.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "ps3CtrlManager.h"

#include "AsyncUDP.h"
#include "udpTxRx.h"
#include <WiFiUdp.h>

#include  "defhtml.h"
 
const char* ssid = "wifi";
const char* password = "12345678";

//general
const int CTRL_CYCLE=50; //制御周期

//DAC
const int DAC_PIN1 = 25; //電圧出力ピン
const int DAC_PIN2 = 26;

//UDP Rx
const int udpRx_port   = 10001; //UDP受信ポート
const int udpRx_dataNum = 4; //UDPで受信する変数数(全部同じ型(バイト数)の変数前提)


// input 
int fow_input = 0;
int st_input = 0;
int fow_input2 = 0;
int st_input2 = 0;


WebServer server(80);

mpu6050 imu;
imuDataProc imuProc;


void handleRoot() { //ブラウザのUI
  server.send(200, "text/html", index_html); 
}

void handleRC() { //ブラウザのUIを操作した結果のJSからアクセスされ??
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


void setup(void) {

  Serial.begin(115200);

  imu.setup();
  setup_wifi();
  udpRx_setup( udpRx_port, udpRx_dataNum );

  ps3_setup();

  imuProc.setup( CTRL_CYCLE, 1000 );

}


void loop_dac( double motorCmd ){
  dacWrite( DAC_PIN1, (int)(motorCmd * 255.0 / 100.0 ));
  dacWrite( DAC_PIN2, (int)(motorCmd * 255.0 / 100.0 ));

}

void loop(void) {
  static unsigned long curr_prev=0;
  unsigned long curr = millis(); // 現在時刻を更新

  if( curr - curr_prev < CTRL_CYCLE ){
    return;
  }
  curr_prev = curr;
  server.handleClient();

  imu.loop();
  imuProc.loop( 0, imu.GyroX_dps, imu.GyroY_dps, imu.GyroZ_dps);
  //imuProc.loop( udpRx_getData(0), udpRx_getData(1), udpRx_getData(2), udpRx_getData(3) );


  loop_dac( imuProc.gyro2motorCmd() );

}






