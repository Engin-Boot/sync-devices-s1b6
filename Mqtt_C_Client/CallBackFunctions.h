#pragma once

#include "../include/MQTTClient.h"

void delivered(void *context, MQTTClient_deliveryToken dt);
int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message);