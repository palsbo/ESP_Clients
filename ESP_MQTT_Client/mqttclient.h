#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

typedef void (*mqttcb)(char * data);
typedef void (*mqttnotfoundcb)(char * topic, char * data);

class MQTTCLIENT {
  private:
    uint32_t mqttConnectTimer;
    int constatus = 0;
    void reconnect();
    static void mqttcallback(char* topic, byte* payload, unsigned int length);
    struct {
      char * host;
      int port;
      char * clientid;
      char * user;
      char * pass;
    } auth;
  public:
    MQTTCLIENT();
    void connect(char * host, int port, char * clientid, char * user, char * pass);
    void on(String topic, mqttcb func);
    void onNotFound(mqttnotfoundcb func);
    void publish(char* topic, String value);
    void publish(char* topic, String value, bool retain);
    void subscribe(char* topic);
    void unsubscribe(char* topic);
    void loop();
};


#endif

