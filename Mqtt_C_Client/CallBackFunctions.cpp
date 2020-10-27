#include "CallBackFunctions.h"
#include <iostream>
#include "PatientDetails.h"

extern volatile MQTTClient_deliveryToken deliveredToken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
	deliveredToken = dt;
}

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	std::cout << "Message Arrived with Topic-> " << topicName << "\nMessage-> " << (char*)message->payload << "\n\n\n";	
	displayPatientDetails(splitPublishedData(static_cast<char*>(message->payload)));
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}