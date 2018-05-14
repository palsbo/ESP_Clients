# ESP Clients

General client libraries for WebSocket and MQTT.

##	WebSocket library

This library expects messages to be received as JSON. Each key in the JSON array will generate an evenet handled by the 'on' method.
'on' methods include on("connect",...) and on("disconnect",...);
The 'on' method on('notJSON',...) will handle messages with no or invalid JSON encoding.
The 'on' method on("data",...) will overwrite JSON decoding and handle the full message, exept for the message "__ping__" that will return a message "__pong__" (for keep alive);

Uses the followng libraries:

	WebSocketsClient.h     https://github.com/brandenhall/Arduino-Websocket/
	ArduinoJson.h          https://github.com/bblanchon/ArduinoJson/blob/master/ArduinoJson.h


###	Include

	#include <socclient.h>
	
###	Define object

	SOCCLIENT	wsc;
	
###	Methods

    void connect(char * host, int port);
    void send(char * key, char * data);
    void on(String topic, wscb func);            //	func = [] (char * data) {};
    void loop();

##	MQTT library

This library connects to a MQTT-broker as a client.

Uses the followng libraries:

	PubSubClient.h         https://github.com/knolleary/pubsubclient

###	Include

	#include <mqttclient.h>
	
###	Define object

	MQTTCLIENT mqtt;
	
###	Methods

    void begin(char * clientid, char * host, int port, char * user, char * pass);
    void on(String topic, mqttcb func);          //	func = [] (char * data) {};
    void onNotFound(mqttnotfoundcb func);        //	func = [] (char * topic, char * data) {};
    void publish(char* topic, String value);
    void publish(char* topic, String value, bool retain);
    void subscribe(char* topic);
    void unsubscribe(char* topic);
    void loop();

