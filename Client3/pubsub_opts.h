
#pragma once
#if !defined(PUBSUB_OPTS_H)
#define PUBSUB_OPTS_H

#include "include/MQTTAsync.h"
#include "include/MQTTClientPersistence.h"


struct pubsub_opts
{
	/* debug app options */
	int publisher;  /* publisher app? */
	int quiet;
	int verbose;
	MQTTASYNC_TRACE_LEVELS tracelevel;

	char* delimiter;
	int maxdatalen;
	/* MQTT options */
	int MQTTVersion;
	char* topic;
	char* clientid;
	int qos;
	int retained;
	char* username;
	char* password;
	char* host;
	char* port;
	char* connection;
	int keepalive;
};

#endif




