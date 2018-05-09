#ifndef MYAPP_H
#define MYAPP_H

#include "webserver.h"
#include "mqttclient.h"

#define MQTT_AUTH  "192.168.1.3",1883,"test","",""

WEBSERVER webserver(80);
MQTTCLIENT mqttclient;

class MYAPP {
  private:
  public:
    void begin() {
      mqttclient.on("connect", [](char * data) {
        Serial.println("Connected");
        mqttclient.subscribe("/#");
      });
      mqttclient.on("failed", [](char * data) {
        Serial.println("Failed");
      });
      mqttclient.onNotFound([](char * topic, char * data) {
        Serial.printf("Topic: '%s' Value: '%s'\n", topic, data);
      });
      mqttclient.connect(MQTT_AUTH);
      webserver.begin();
    }
    void loop() {
      mqttclient.loop();
      webserver.loop();
    }
} myApp;

#endif
