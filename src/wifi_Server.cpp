#include "wifi_Server.h"

//SPIFFS�A�b�v���[�h
//C:\Users\user\.platformio\penv\Scripts\platformio.exe run --target uploadfs 

// input 
int dat0_input = 0;
int dat1_input = 0;
int dat2_input = 0;
int dat3_input = 0;

int plotMode = 0;

WebServer server(80);
// Websocket�T�[�o�[ IP�A�h���X:81
WebSocketsServer webSocket = WebSocketsServer(81); // 81�ԃ|�[�g

const char* ssid_ap = "wifi";
const char* password_ap = "12345678";

// const char* ssid_sta = "MYASUS";
// const char* password_sta = "12345678";

const char* ssid_sta = "safari_wifi";
const char* password_sta = "87654321";


// AP mode
// const IPAddress ip(192, 168, 0, 55);
// const IPAddress gateway(192, 168, 0, 55); //gateway��IP�A�h���X

// MYASUS
// const IPAddress ip(192, 168, 43, 149);
// const IPAddress gateway(192, 168, 43, 149); //gateway��IP�A�h���X
// const IPAddress subnet(255,255,255,0);

// Kojima Rooter
const IPAddress ip(192, 168, 10, 108);
const IPAddress gateway(192, 168, 10, 108); //gateway��IP�A�h���X
const IPAddress subnet(255,255,255,0);


// �Z���T�̃f�[�^(JSON�`��)
//const char SENSOR_JSON[] PROGMEM = R"=====({"val1":%.1f})=====";
//const char SENSOR_JSON[] PROGMEM = R"=====({"val0":%.1f, "val1":%.1f, "val2":%.1f, "str0":"%s", "str1":"%s", "str2":"%s"})=====";
const char SENSOR_JSON[] PROGMEM = R"=====({"val0":%.1f, "val1":%.1f, "val2":%.1f, "mode":%d})=====";


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
    WiFi.softAP(ssid_ap, password_ap, 3, 0, 6);
    delay(200);
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


  // Web�T�[�o�[�̃R���e���c�ݒ�
  // favicon.ico, Chart.min.js�� data�t�H���_���ɔz�u
  SPIFFS.begin();
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  server.serveStatic("/Chart.min.js", SPIFFS, "/Chart.min.js");
  server.serveStatic("/hoge", SPIFFS, "/Chart.min.jshogehoge");
  server.on("/", handleRoot);
  server.on("/rc", handleRC); 
  server.onNotFound(handleNotFound);
  server.on("/inputPad", handleInputPad);
  server.on("/plot", handlePlot);
  server.on("/plot0", handlePlot0);
  server.on("/plot1", handlePlot1);
  server.on("/plot2", handlePlot2);
  server.on("/plot3", handlePlot3);
  server.on("/plot4", handlePlot4);

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  // WebSocket�T�[�o�[�J�n
  webSocket.begin();

}

// �f�[�^�̍X�V
void wifi_data_loop( double data0, double data1, double data2 ) {
  char payload[80];
//=============================================
  std::string label0;
  std::string label1;
  std::string label2;

  snprintf_P(payload, sizeof(payload), SENSOR_JSON, data0, data1, data2, plotMode);
//============================================= 

  // WebSocket�Ńf�[�^���M(�S�[���փu���[�h�L���X�g)
  webSocket.broadcastTXT(payload, strlen(payload));
//  Serial.println(payload);
}


void wifi_loop(){
  webSocket.loop();
  server.handleClient();
  //data_loop();

  // Serial.println("SPIFF �t�@�C���\��");
  // auto root = SPIFFS.open("/");
  // auto file = root.openNextFile();
  // while (file) {
  //   Serial.println(file.name());
  //   file = root.openNextFile();
  // }


}


// Web�R���e���c�̃C�x���g�n���h��
void handlePlot() {
  String s = INDEX_HTML; // index_html.h���ǂݍ���
  server.send(200, "text/html", s);
}

void handlePlot0() {
  plotMode = 0;
  handlePlot();
}
void handlePlot1() {
  plotMode = 1;
  handlePlot();
  
}
void handlePlot2() {
  plotMode = 2;
  handlePlot();
  
}
void handlePlot3() {
  plotMode = 3;
  handlePlot();
  
}
void handlePlot4() {
  plotMode = 4;
  handlePlot();
  
}


void handleRoot() { //�u���E�U��UI
  handlePlot4();

}

void handleInputPad() { //�u���E�U��UI
  server.send(200, "text/html", inputPad_html); 
}

void handleRC() { //�u���E�U��UI�𑀍삵�����ʂ�JS����A�N�Z�X�����
  for (int i = 0; i < server.args(); i++) {
    int Val_i = server.arg(i).toInt();
    Serial.print(server.argName(i) + "=" + server.arg(i) + ", ");
    
    if(      server.argName(i) == "D0" ) { dat0_input = Val_i; }
    else if( server.argName(i) == "D1" ) { dat1_input = Val_i; }

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


