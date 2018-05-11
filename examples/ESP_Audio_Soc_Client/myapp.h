#ifndef MYAPP_H
#define MYAPP_H

#define DAC   //  uncomment if I2S DAC module is  used
//#define SPIRAM  //  uncomment if 23LC1024 SPI RAM is used

#include "socclient.h"
#include "audio.h"

#define WSHOST  ".."; //  insert your own host uri
#define WSPORT  81;   //  insert your own host port
#include <init.h>     //  and remove this line

SOCCLIENT socclient;
AUDIO radio;

class MYAPP {
  private:
  public:
    void begin() {
      socclient.on("url", [](char * data) {
        Serial.println(data);
        radio.start(data);
      });
      socclient.on("gain", [](char* data) {
        Serial.printf("[WSc] Volume! '%s'\n", data);
        radio.setGain(atoi(data));
      });
      socclient.on("connect", [](char * data) {
        Serial.println("Connect ok");
      });
      socclient.connect(WSHOST, WSPORT);
      radio.begin();
    }
    void loop() {
      socclient.loop();
      radio.loop();
    }
} myApp;

#endif