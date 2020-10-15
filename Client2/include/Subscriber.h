#pragma once

#include <iostream>
#include <windows.h>
#include <cstdio>
#include <stdlib.h>
#include <string>

#include "include/MQTTAsync.h"
#include "include/MQTTClientPersistence.h"
#include "pubsub_opts.h"

using namespace std;

#define sleep Sleep

//extern void setPatientDetails(string);

volatile int finished = 0;
int subscribed = 0;
int disconnected = 0;

MQTTAsync_connectOptions sub_opts = MQTTAsync_connectOptions_initializer;

struct pubsub_opts s_opts =
{
	0, 0, 0, MQTTASYNC_TRACE_MAXIMUM, "\n", 100,
	MQTTVERSION_DEFAULT, "Patient/#", "PET-CTdata", 2, 0,NULL,NULL, "localhost", "1883", NULL, 10 /* MQTT options */
};

int sub_messageArrived(void* context, char* topic, int topicLen, MQTTAsync_message* message);
void sub_onDisconnect(void* context, MQTTAsync_successData* response);
void onSubscribe(void* context, MQTTAsync_successData* response);
void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
void sub_onConnectFailure(void* context, MQTTAsync_failureData* response);
void sub_onConnect(void* context, MQTTAsync_successData* response);
void sub_createClient(MQTTAsync* client, string url, MQTTAsync_createOptions* create_opts);
void sub_setCallbacks(MQTTAsync* client);
void sub_connectToBroker(MQTTAsync* client);
int subscribe();