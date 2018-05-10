#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

typedef void (*mqttcb)(char * data);
typedef void (*mqttnotfoundcb)(char * topic, char * data);

class MQTTCLIENT {
  private:
    bool isConnected = false;
    static mqttnotfoundcb notfound;
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    long mqttConnectTimer;
    int constatus = 0;
    char * host;
    int port;
    char * clientid;
    char * user;
    char * pass;
  public:
    MQTTCLIENT();
    void begin(char * clientid, char * host, int port, char * user, char * pass);
    void on(String topic, mqttcb func);
    void onNotFound(mqttnotfoundcb func);
    void publish(char* topic, String value);
    void publish(char* topic, String value, bool retain);
    void subscribe(char* topic);
    void unsubscribe(char* topic);
    void loop();
};


#endif

