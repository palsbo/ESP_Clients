#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <PubSubClient.h>
#include "mqttclient.h"

#define MAXONS  15

WiFiClient espClient;
PubSubClient client(espClient);

struct {
  private:
    struct {
      String topic;
      mqttcb func;
    } ons[MAXONS];
    int oncount = 0;
  public:
    bool lookup (String topic, char * data) {
      for (int i = 0; i < oncount; i++) {
        if (topic == ons[i].topic) {
          ons[i].func(data);
          return true;
        }
      }
      return false;
    }
    void add(String topic, mqttcb func) {
      for (int i = 0; i < oncount; i++) {
        if (ons[i].topic == topic) {
          ons[i].func = func;
          return;
        }
      }
      if (oncount >= MAXONS) return;
      ons[oncount].topic = topic;
      ons[oncount].func = func;
      oncount++;
    }
} mq;

mqttnotfoundcb MQTTCLIENT::notfound = [](char * topic, char * data) {};

void MQTTCLIENT::onNotFound(mqttnotfoundcb cb) {
  notfound = cb;
}

void MQTTCLIENT::mqttCallback(char* topic, byte* payload, unsigned int length) {
  char s1[length + 1];
  s1[length] = 0;
  memcpy(s1, payload, length);
  s1[length] = 0;
  if (!mq.lookup(topic, s1)) notfound(topic, s1);
}

MQTTCLIENT::MQTTCLIENT() {
  mq.add("connect", [](char * data) {});
  mq.add("failed", [](char * data) {});
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

void MQTTCLIENT::loop() {
  while ((!client.connected()) && (millis() > mqttConnectTimer)) {
    client.setServer(host, port);
    if (client.connect(clientid, user, pass)) {
      mq.lookup("connect", "");
    } else {
      mqttConnectTimer = millis() + 5000;
      mq.lookup("disconnect", "");
    }
  }
  if (client.connected()) client.loop();
};

void MQTTCLIENT::on(String topic, mqttcb func) {
  mq.add(topic, func);
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

