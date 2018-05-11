#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "socclient.h"

WebSocketsClient webSocket;

WSCONS ws;

void WSCONS::lookup (String key, char * data) {
  for (int i = 0; i < count; i++) {
    if (key == item[i].key) {
      item[i].func(data);
      return;
    }
  }
}
void WSCONS::add(String key, wscb func) {
  for (int i = 0; i < count; i++) {
    if (item[i].key == key) {
      item[i].func = func;
      return;
    }
  }
  if (count >= MAXONS) return;
  item[count].key = key;
  item[count].func = func;
  count++;
}

void webSocketClientEvent(WStype_t type, uint8_t * payload, size_t length) {
  char s1[length + 1];
  strncpy_P(s1, (char*)payload, length);
  s1[length] = 0;
  switch (type) {
    case WStype_DISCONNECTED:
      ws.lookup("disconnect", &s1[0]);
      break;
    case WStype_CONNECTED:
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
  webSocket.onEvent(webSocketClientEvent);
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

