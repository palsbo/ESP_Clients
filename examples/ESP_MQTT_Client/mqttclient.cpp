#include <PubSubClient.h>
#include "mqttclient.h"

#define MAXONS  15

PubSubClient client;

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

struct {
  const char * item[10];
  int count = 0;
  void add(const char * sub) {
    for (int i = 0; i < count; i++) if (!strcmp(sub, item[i])) return;
    item[count++] = sub;
  };
  void sub(const char * sub) {
    for (int i = 0; i < count; i++) if (!strcmp(sub, item[i])) {
        if (i < count) {
          item[i] = item[count - 1];
          count--;
        }
      }
  };
} sublist;

bool isConnected = false;
mqttnotfoundcb notfound = [](char * topic, char * data){};

void MQTTCLIENT::onNotFound(mqttnotfoundcb cb) {
  notfound = cb;
}

void MQTTCLIENT::mqttcallback(char* topic, byte* payload, unsigned int length) {
  char s1[length + 1];
  memcpy(s1, payload, length);
  s1[length] = 0;
  if (!mq.lookup(topic, s1)) notfound(topic, s1);
}

void MQTTCLIENT::reconnect() {
  if (constatus == 0) return;
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient")) {
      isConnected = true;
      mq.lookup("connected", "");
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
MQTTCLIENT::MQTTCLIENT() {
  mq.add("connect", [](char * data) {});
  mq.add("failed", [](char * data) {});
};

void MQTTCLIENT::connect(char * host, int port, char * clientid, char * user, char * pass) {
  auth.host = host;
  auth.port = port;
  auth.clientid = clientid;
  auth.user = user;
  auth.pass = pass;
  if (client.connected()) client.disconnect();
  //client.setServer(auth.host, auth.port);
  client.setCallback(mqttcallback);
};

void MQTTCLIENT::loop() {
  while ((!client.connected()) && (millis() > mqttConnectTimer)) {
    client.setServer(auth.host, auth.port);
    if (client.connect(auth.clientid, auth.user, auth.pass)) {
      mq.lookup("connect", "");
    } else {
      //  here if failed
      mqttConnectTimer = millis() + 5000;
      mq.lookup("failed", "");
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

