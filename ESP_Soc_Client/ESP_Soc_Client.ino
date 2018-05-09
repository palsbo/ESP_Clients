#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#define SSID  ".."  //  insert your wlan ssid
#define PASS  ".."  //  .. and password
#include <init.h>   //  and remove this line

#include "myapp.h"

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  Serial.print("\nConnecting");
  while (WiFi.status() != WL_CONNECTED) delay(500),Serial.print("."); Serial.println("ok!");
  Serial.print("Connected as ");
  Serial.println(WiFi.localIP());
  myApp.begin();
}

void loop() {
  myApp.loop();
}
