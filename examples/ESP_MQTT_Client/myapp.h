#ifndef MYAPP_H
#define MYAPP_H

#include "webserver.h"
#include "mqttclient.h"

#define MQTT_CLIENT "MQTTCLIENT"   // fill in your own value
#define MQTT_SERVER "192.168.1.3"  // fill in your own value
#define MQTT_PORT 1883             // fill in your own value
#define MQTT_USER ""               // fill in your own value
#define MQTT_PASS ""               // fill in your own value
#include <init.h>                  // - and remove this line!

char* mqtt_client = "ESP123"; //String(ESP.getChipId());
char* mqtt_host = MQTT_SERVER;
uint16_t mqtt_port = MQTT_PORT;
char* mqtt_user = MQTT_USER;
char* mqtt_pass = MQTT_PASS;

WEBSERVER webserver(80);
MQTTCLIENT mqttclient;

class MYAPP {
  private:
  public:
    void begin() {
      mqttclient.on("connect", [](char * data) {
        Serial.println("MQTT Connected");
        mqttclient.subscribe("#");
      });
      mqttclient.on("failed", [](char * data) {
        Serial.println("MQTT Failed");
      });
      mqttclient.on("esp8266/Vaerelset/hum/state", [] (char * data) {
        Serial.printf("Topic: '%s' Value: '%s'\n", "esp8266/Vaerelset/hum/state", data);
      });
      /*
        mqttclient.onNotFound([](char * topic, char * data) {
        Serial.printf("Topic: '%s' Value: '%s'\n", topic, data);
        });
      */
      mqttclient.begin(mqtt_client, mqtt_host, mqtt_port, mqtt_user, mqtt_pass);
      webserver.begin();
    }
    void loop() {
      mqttclient.loop();
      webserver.loop();
    }
} myApp;

#endif
