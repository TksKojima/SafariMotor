#include "wifi_Server.h"

// input 
int dat0_input = 0;
int dat1_input = 0;
int dat2_input = 0;
int dat3_input = 0;

WebServer server(80);
// Websocketサーバー IPアドレス:81
WebSocketsServer webSocket = WebSocketsServer(81); // 81番ポート

const char* ssid_ap = "wifi";
const char* password_ap = "12345678";


const char* ssid_sta = "MYASUS";
const char* password_sta = "12345678";

// const IPAddress ip(192, 168, 4, 1);
// const IPAddress gateway(192, 168, 4, 1); //gatewayのIPアドレス
const IPAddress ip(192, 168, 201, 157);
const IPAddress gateway(192, 168, 201, 157); //gatewayのIPアドレス
const IPAddress subnet(255,255,255,0);


void wifi_setup( int wifi_mode ){
  delay(500);

  if(  wifi_mode == WIFI_STA ){
    int cnt = 0;
    do{
      if( cnt == 0){
        if (!WiFi.config(ip,gateway,subnet)){
            Serial.println("Failed to configure!");
        }  
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid_sta, password_sta);
        Serial.print("Connecting to ");
        Serial.println(ssid_sta);
      }
      delay(500);
      Serial.print(".");
      cnt++;
      if( cnt >= 10 ){ cnt = 0; }

    }while (WiFi.status() != WL_CONNECTED);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());    
  }
  else if( wifi_mode == WIFI_AP ){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_ap, password_ap, 3, 0, 4);
    delay(200);
    //WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
    WiFi.softAPConfig( ip,gateway,subnet );
    Serial.println("");
    Serial.print("AP IP address: ");
    Serial.print(WiFi.softAPIP());    

  }
    
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


void wifi_loop(){
  server.handleClient();
}


// Webコンテンツのイベントハンドラ
void handlePlot() {
  String s = INDEX_HTML; // index_html.hより読み込み
  webServer.send(200, "text/html", s);
}

void handleRoot() { //ブラウザのUI
  server.send(200, "text/html", index_html); 
}

void handleRC() { //ブラウザのUIを操作した結果のJSからアクセスされ??
  for (int i = 0; i < server.args(); i++) {
    int Val_i = server.arg(i).toInt();
    Serial.print(server.argName(i) + "=" + server.arg(i) + ", ");
    
    switch (server.argName(i)[0]) {
      case 'A': dat0_input = Val_i; break;
      case 'B': dat1_input = Val_i; break;
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


