#ifndef WSCLIENT_H
#define WSCLIENT_H
#include <WebSocketsClient.h>

typedef void (*wscb)(char * data);

class SOCCLIENT {
  private:
    bool isConnected = false;
    static void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);  
  public:
    SOCCLIENT();
    void connect(char * host, int port);
    void send(char * key, char * data); //  to do
    void on(String topic, wscb func);
    void loop();
};


#endif

