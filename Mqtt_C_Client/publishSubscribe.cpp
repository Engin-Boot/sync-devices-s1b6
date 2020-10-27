#include "ClientDetails.h"
#include "ClientInitialization.h"
#include <cstring>
#include "publishSubscribe.h"
#include "PatientDetails.h"

extern volatile MQTTClient_deliveryToken deliveredToken;

void publishMessage(MQTTClient& client,int& ch)
{
	if (ch == 'p')
	{
		Patient patient;
		auto stringMsg = fillPatientDetails(patient);
		auto resultantPayload = stringMsg.c_str();
		publisher_msg.payload = const_cast<char*>(resultantPayload);
		publisher_msg.payloadlen = static_cast<int>(strlen(resultantPayload));
		publisher_msg.qos = QOS;
		publisher_msg.retained = 0;

		deliveredToken = 0;
		MQTTClient_publishMessage(client, TOPIC, &publisher_msg, &token);
		while (deliveredToken != token);
	}
}

void publishAndSubscribe(MQTTClient& client, int& ch)
{
	do
	{
		MQTTClient_subscribe(client, TOPIC, QOS);
		publishMessage(client, ch);
		ch = getchar();
	} while (ch != 'q');
}

