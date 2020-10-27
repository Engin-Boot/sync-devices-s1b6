#include "CallBackFunctions.h"
#include "ClientDetails.h"
#include <iostream>
#include "ClientInitialization.h"
#include "publishSubscribe.h"

volatile MQTTClient_deliveryToken deliveredToken;

int main(int argc, char* argv[])
{
	MQTTClient client;
	int rc,ch = 7;

	//client id is specified in cmd line 
	MQTTClient_create(&client, ADDRESS, argv[1], MQTTCLIENT_PERSISTENCE_NONE, NULL);
	connect_options.keepAliveInterval = 10;

	MQTTClient_setCallbacks(client, NULL,NULL,messageArrived,delivered);

	if((rc = MQTTClient_connect(client,&connect_options)) != MQTTCLIENT_SUCCESS)
	{
		std::cout << "Failed to Connect Client with Broker and rc = " << rc;
		exit(EXIT_FAILURE);
	}
	publishAndSubscribe(client, ch);
	
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);

	return rc;
}



