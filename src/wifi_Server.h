#ifndef _H_WIFISERVER_
#define _H_WIFISERVER_

#include <Arduino.h>
#include <WebSocketsServer.h> // arduinoWebSocketsƒ‰ƒCƒuƒ‰ƒŠ
#include <SPIFFS.h>
#include "index_html.h" // web server root index

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include  "defhtml.h"




void wifi_setup( int wifi_mode );
void wifi_loop();

void handleRoot();
void handleRC();
void handleNotFound();
void handlePlot();

void data_loop();

#endif




