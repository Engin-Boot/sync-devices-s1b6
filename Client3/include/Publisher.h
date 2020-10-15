
#pragma once

#include "include/MQTTAsync.h"
#include "pubsub_opts.h"

#include <stdio.h>
#include <signal.h>
#include <string>
#include <stdlib.h>
#include<Windows.h>

using namespace std;

MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;
static int pub_disconnected = 0;
static int pub_connected = 0;
static int pub_published = 0;
MQTTAsync pub_client;

struct pubsub_opts opts =
{
	1, 0, 0, MQTTASYNC_TRACE_MAXIMUM, "\n", 100,  	/* debug/app options */
	MQTTVERSION_DEFAULT, NULL, "MRI", 2, 0,NULL,NULL, "localhost", "1883", NULL, 10 /* MQTT options */
};

int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* m);
void onDisconnect(void* context, MQTTAsync_successData* response);
void onConnectFailure(void* context, MQTTAsync_failureData* response);
void onConnect(void* context, MQTTAsync_successData* response);
void onPublishFailure(void* context, MQTTAsync_failureData* response);
void onPublish(void* context, MQTTAsync_successData* response);
void myconnect(MQTTAsync client_pub);
void ConnectPublisher();
int publish(string topicName, string message);
