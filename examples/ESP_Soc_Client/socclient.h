#ifndef WSCLIENT_H
#define WSCLIENT_H

typedef void (*wscb)(char * data);

class SOCCLIENT {
  private:
  public:
    SOCCLIENT();
    void connect(char * host, int port);
    void send(char * key, char * data); //  to do
    void on(String topic, wscb func);
    void loop();
};


#endif

