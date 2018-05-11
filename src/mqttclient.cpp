#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <PubSubClient.h>
#include "mqttclient.h"

WiFiClient espClient;
PubSubClient client(espClient);

MQCONS mqons;

bool MQCONS::lookup (char * topic, char * data) {
  for (int i = 0; i < count; i++) {
    if (!strcmp(topic, &(item[i].topic)[0])) {
      item[i].func(data);
      return true;
    }
  }
  return false;
}

void MQCONS::add(String topic, mqttcb func) {
  for (int i = 0; i < count; i++) {
    if (item[i].topic == topic) {
      item[i].func = func;
      return;
    }
  }
  if (count >= MAXONS) return;
  item[count].topic = topic;
  item[count].func = func;
  count++;
}

mqttnotfoundcb MQTTCLIENT::notfound = [](char * topic, char * data) {};

void MQTTCLIENT::mqttCallback(char* topic, byte* payload, unsigned int length) {
  char s1[length + 1];
  s1[length] = 0;
  memcpy(s1, payload, length);
  s1[length] = 0;
  if (!mqons.lookup(topic, &s1[0])) notfound(topic, s1);
}

MQTTCLIENT::MQTTCLIENT() {
  mqons.add("connect", [](char * data) {});
  mqons.add("failed", [](char * data) {});
};

void MQTTCLIENT::begin(char * _clientid, char * _host, int _port, char * _user, char * _pass) {
  host = _host;
  port = _port;
  clientid = _clientid;
  user = _user;
  pass = _pass;
  if (client.connected()) {
    client.disconnect();
  }
  client.setServer(host, port);
  client.setCallback(mqttCallback);
};

void MQTTCLIENT::on(String topic, mqttcb func) {
  mqons.add(topic, func);
};

void MQTTCLIENT::onNotFound(mqttnotfoundcb cb) {
  notfound = cb;
}

void MQTTCLIENT::loop() {
  while ((!client.connected()) && (millis() > mqttConnectTimer)) {
    client.setServer(host, port);
    if (client.connect(clientid, user, pass)) {
      mqons.lookup("connect", "");
    } else {
      mqttConnectTimer = millis() + 5000;
      mqons.lookup("disconnect", "");
    }
  }
  if (client.connected()) client.loop();
};

void MQTTCLIENT::publish(char* topic, String value) {
  if (client.connected()) client.publish(topic, value.c_str());
}

void MQTTCLIENT::publish(char* topic, String value, bool retain) {
  if (client.connected()) client.publish(topic, value.c_str()), retain;
}
void MQTTCLIENT::subscribe(char* topic) {
  client.subscribe(topic);
}

void MQTTCLIENT::unsubscribe(char* topic) {
  client.unsubscribe(topic);
}

