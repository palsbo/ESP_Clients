#ifndef MYAPP_H
#define MYAPP_H

#include "webserver.h"
#include "socclient.h"

#define WSC_AUTH  "192.168.1.3",81
WEBSERVER webserver(80);
SOCCLIENT socclient;

class MYAPP {
  private:
  public:
    void begin() {
      socclient.connect(WSC_AUTH);
      webserver.begin();
    }
    void loop() {
      socclient.loop();
      webserver.loop();
    }
} myApp;

#endif
