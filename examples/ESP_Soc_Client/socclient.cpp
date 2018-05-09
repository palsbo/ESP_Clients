#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "socclient.h"

#define MAXONS  15

WebSocketsClient webSocket;

struct {
  private:
    struct {
      String key;
      wscb func;
    } ons[MAXONS];
    int oncount = 0;
  public:
    void lookup (String key, char * data) {
      for (int i = 0; i < oncount; i++) {
        if (key == ons[i].key) {
          ons[i].func(data);
          return;
        }
      }
    }
    void add(String key, wscb func) {
      for (int i = 0; i < oncount; i++) {
        if (ons[i].key == key) {
          ons[i].func = func;
          return;
        }
      }
      if (oncount >= MAXONS) return;
      ons[oncount].key = key;
      ons[oncount].func = func;
      oncount++;
    }
} ws;

bool isConnected = false;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  char s1[length + 1];
  strncpy_P(s1, (char*)payload, length);
  s1[length] = 0;
  switch (type) {
    case WStype_DISCONNECTED:
      isConnected = false;
      ws.lookup("disconnect", &s1[0]);
      break;
    case WStype_CONNECTED:
      isConnected = true;
      ws.lookup("connect", &s1[0]);
      break;
    case WStype_TEXT:
      ws.lookup("data", &s1[0]);
      break;
  }
}

SOCCLIENT::SOCCLIENT() {
  on("connect", [](char * data) { });
  on("disconnect", [](char * data) { });
  on("data", [](char * data) {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(data);
    for (auto field : root) {
      String value = root[field.key];
      ws.lookup(field.key, (char*)value.c_str());
    }
  });
}

void SOCCLIENT::connect(char * host, int port) {
  webSocket.begin(host, port, "/");
  webSocket.onEvent(webSocketEvent);
};

void SOCCLIENT::loop() {
  webSocket.loop();
};

void SOCCLIENT::send(char * key, char * data) {
  char  buf[100];
  sprintf(buf, "{\"%s\":\"%s\"}\0", key, data);
  webSocket.sendTXT(buf);
};

void SOCCLIENT::on(String key, wscb func) {
  ws.add(key, func);
};

