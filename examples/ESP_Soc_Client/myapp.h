#ifndef MYAPP_H
#define MYAPP_H

#include "webserver.h"
#include "socclient.h"

#define WSHOST  ".."; //  insert your own host uri
#define WSPORT  81;   //  insert your own host port
#include <init.h>     //  and remove this line

#define WSC_AUTH  WSHOST,WSPORT
WEBSERVER webserver(80);
SOCCLIENT socclient;

class MYAPP {
  private:
  public:
    void begin() {
      socclient.on("url",[](char * data) {
        Serial.println(data);
      });
      socclient.on("connect",[](char * data) {
        Serial.println("Connect ok");
      });
      socclient.connect(WSHOST, WSPORT);
      webserver.begin();
    }
    void loop() {
      socclient.loop();
      webserver.loop();
    }
} myApp;

#endif
