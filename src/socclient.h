#ifndef WSCLIENT_H
#define WSCLIENT_H
#include <WebSocketsClient.h>

typedef void (*wscb)(char * data);

#define MAXONS  15

class WSCONS {
  private:
    struct {
      String key;
      wscb func;
    } item[MAXONS];
    int count = 0;
  public:
    void add(String key, wscb func);
    void lookup (String key, char * data);
};

class SOCCLIENT {
  private:
    bool isConnected = false;
  public:
    SOCCLIENT();
    void connect(char * host, int port);
    void send(char * key, char * data); //  to do
    void on(String topic, wscb func);
    void loop();
};


#endif

