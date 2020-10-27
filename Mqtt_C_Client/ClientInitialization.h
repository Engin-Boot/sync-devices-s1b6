#pragma once

#include "../include/MQTTClient.h"

static MQTTClient_connectOptions connect_options = MQTTClient_connectOptions_initializer;
static MQTTClient_message publisher_msg = MQTTClient_message_initializer;
static MQTTClient_deliveryToken token;

