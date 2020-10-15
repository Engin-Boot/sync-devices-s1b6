/*******************************************************************************
* Copyright (c) 2012, 2020 IBM Corp., and others
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v2.0
* and Eclipse Distribution License v1.0 which accompany this distribution.
*
* The Eclipse Public License is available at
*   https://www.eclipse.org/legal/epl-2.0/
* and the Eclipse Distribution License is available at
*   http://www.eclipse.org/org/documents/edl-v10.php.
*
* Contributors:
*    Ian Craggs - initial contribution
*    Guilherme Maciel Ferreira - add keep alive option
*    Ian Craggs - add full capability
*******************************************************************************/

#include "MQTTAsync.h"
#include "pubsub_opts.h"

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <windows.h>
#define sleep Sleep
#else
#include <unistd.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#if defined(_WRS_KERNEL)
#include <OsWrapper.h>
#endif

volatile int toStop = 0;

struct pubsub_opts publisherOpts =
{
	1, 0, 0, MQTTASYNC_TRACE_MAXIMUM, "\n", 100,  	/* debug/app options */
	NULL, NULL, 1, 0, 0, /* message options */
	MQTTVERSION_DEFAULT, NULL, "Client2main", 0, 0, NULL, NULL, "localhost", "1883", NULL, 10, /* MQTT options */
	NULL, NULL, 0, 0, /* will options */
	0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* TLS options */
	0,{ NULL, NULL }, /* MQTT V5 options */
};

MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;
MQTTProperty property;
MQTTProperties props = MQTTProperties_initializer;


void mysleep(int ms)
{
#if defined(_WIN32)
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}

void cfinish(int sig)
{
	signal(SIGINT, NULL);
	toStop = 1;
}


int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* m)
{
	/* not expecting any messages */
	return 1;
}


static int disconnected = 0;

void onDisconnect5(void* context, MQTTAsync_successData5* response)
{
	disconnected = 1;
}

void onDisconnect(void* context, MQTTAsync_successData* response)
{
	disconnected = 1;
}


static int connected = 0;
void myconnect(MQTTAsync client);
int mypublish(MQTTAsync client, int datalen, char* data);

void onConnectFailure5(void* context, MQTTAsync_failureData5* response)
{
	fprintf(stderr, "Connect failed, rc %s reason code %s\n",
		MQTTAsync_strerror(response->code),
		MQTTReasonCode_toString(response->reasonCode));
	connected = -1;

	MQTTAsync client = (MQTTAsync)context;
}

void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	fprintf(stderr, "Connect failed, rc %s\n", response ? MQTTAsync_strerror(response->code) : "none");
	connected = -1;

	MQTTAsync client = (MQTTAsync)context;
}


void onConnect5(void* context, MQTTAsync_successData5* response)
{
	MQTTAsync client = (MQTTAsync)context;
	int rc = 0;

	if (publisherOpts.verbose)
		printf("Connected\n");

	if (publisherOpts.null_message == 1)
		rc = mypublish(client, 0, "");
	else if (publisherOpts.message)
		rc = mypublish(client, (int)strlen(publisherOpts.message), publisherOpts.message);
	/*else if (publisherOpts.filename)
	{
	int data_len = 0;
	char* buffer = readfile(&data_len, &publisherOpts);

	if (buffer == NULL)
	toStop = 1;
	else
	{
	rc = mypublish(client, data_len, buffer);
	free(buffer);
	}
	}*/

	connected = 1;
}

void onConnect(void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	int rc = 0;

	if (publisherOpts.verbose)
		printf("Connected\n");

	if (publisherOpts.null_message == 1)
		rc = mypublish(client, 0, "");
	else if (publisherOpts.message)
		rc = mypublish(client, (int)strlen(publisherOpts.message), publisherOpts.message);
	/*else if (publisherOpts.filename)
	{
	int data_len = 0;
	char* buffer = readfile(&data_len, &publisherOpts);

	if (buffer == NULL)
	toStop = 1;
	else
	{
	rc = mypublish(client, data_len, buffer);
	free(buffer);
	}
	}*/

	connected = 1;
}


static int published = 0;

void onPublishFailure5(void* context, MQTTAsync_failureData5* response)
{
	if (publisherOpts.verbose)
		fprintf(stderr, "Publish failed, rc %s reason code %s\n",
			MQTTAsync_strerror(response->code),
			MQTTReasonCode_toString(response->reasonCode));
	published = -1;
}

void onPublishFailure(void* context, MQTTAsync_failureData* response)
{
	if (publisherOpts.verbose)
		fprintf(stderr, "Publish failed, rc %s\n", MQTTAsync_strerror(response->code));
	published = -1;
}


void onPublish5(void* context, MQTTAsync_successData5* response)
{
	if (publisherOpts.verbose)
		printf("Publish succeeded, reason code %s\n",
			MQTTReasonCode_toString(response->reasonCode));

	if (publisherOpts.null_message || publisherOpts.message || publisherOpts.filename)
		toStop = 1;

	published = 1;
}


void onPublish(void* context, MQTTAsync_successData* response)
{
	if (publisherOpts.verbose)
		printf("Publish succeeded\n");

	if (publisherOpts.null_message || publisherOpts.message || publisherOpts.filename)
		toStop = 1;

	published = 1;
}


static int onSSLError(const char *str, size_t len, void *context)
{
	MQTTAsync client = (MQTTAsync)context;
	return fprintf(stderr, "SSL error: %s\n", str);
}

static unsigned int onPSKAuth(const char* hint,
	char* identity,
	unsigned int max_identity_len,
	unsigned char* psk,
	unsigned int max_psk_len,
	void* context)
{
	int psk_len;
	int k, n;

	int rc = 0;
	struct pubsub_opts* opts = (pubsub_opts*)context;

	/* printf("Trying TLS-PSK auth with hint: %s\n", hint);*/

	if (opts->psk == NULL || opts->psk_identity == NULL)
	{
		/* printf("No PSK entered\n"); */
		goto exit;
	}

	/* psk should be array of bytes. This is a quick and dirty way to
	* convert hex to bytes without input validation */
	psk_len = (int)strlen(opts->psk) / 2;
	if ((unsigned int)psk_len > max_psk_len)
	{
		fprintf(stderr, "PSK too long\n");
		goto exit;
	}
	for (k = 0, n = 0; k < psk_len; k++, n += 2)
	{
		sscanf_s(&opts->psk[n], "%2hhx", &psk[k]);
	}

	/* identity should be NULL terminated string */
	strncpy_s(identity, 500, opts->psk_identity, max_identity_len);
	if (identity[max_identity_len - 1] != '\0')
	{
		fprintf(stderr, "Identity too long\n");
		goto exit;
	}

	/* Function should return length of psk on success. */
	rc = psk_len;

exit:
	return rc;
}

void myconnect(MQTTAsync client)
{
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_SSLOptions ssl_opts = MQTTAsync_SSLOptions_initializer;
	MQTTAsync_willOptions will_opts = MQTTAsync_willOptions_initializer;
	int rc = 0;

	if (publisherOpts.verbose)
		printf("Connecting\n");
	if (publisherOpts.MQTTVersion == MQTTVERSION_5)
	{
		MQTTAsync_connectOptions conn_opts5 = MQTTAsync_connectOptions_initializer5;
		conn_opts = conn_opts5;
		conn_opts.onSuccess5 = onConnect5;
		conn_opts.onFailure5 = onConnectFailure5;
		conn_opts.cleanstart = 1;
	}
	else
	{
		conn_opts.onSuccess = onConnect;
		conn_opts.onFailure = onConnectFailure;
		conn_opts.cleansession = 1;
	}
	conn_opts.keepAliveInterval = publisherOpts.keepalive;
	conn_opts.username = publisherOpts.username;
	conn_opts.password = publisherOpts.password;
	conn_opts.MQTTVersion = publisherOpts.MQTTVersion;
	conn_opts.context = client;
	conn_opts.automaticReconnect = 1;

	if (publisherOpts.will_topic) 	/* will options */
	{
		will_opts.message = publisherOpts.will_payload;
		will_opts.topicName = publisherOpts.will_topic;
		will_opts.qos = publisherOpts.will_qos;
		will_opts.retained = publisherOpts.will_retain;
		conn_opts.will = &will_opts;
	}

	if (publisherOpts.connection && (strncmp(publisherOpts.connection, "ssl://", 6) == 0 ||
		strncmp(publisherOpts.connection, "wss://", 6) == 0))
	{
		if (publisherOpts.insecure)
			ssl_opts.verify = 0;
		else
			ssl_opts.verify = 1;
		ssl_opts.CApath = publisherOpts.capath;
		ssl_opts.keyStore = publisherOpts.cert;
		ssl_opts.trustStore = publisherOpts.cafile;
		ssl_opts.privateKey = publisherOpts.key;
		ssl_opts.privateKeyPassword = publisherOpts.keypass;
		ssl_opts.enabledCipherSuites = publisherOpts.ciphers;
		ssl_opts.ssl_error_cb = onSSLError;
		ssl_opts.ssl_error_context = client;
		ssl_opts.ssl_psk_cb = onPSKAuth;
		ssl_opts.ssl_psk_context = &publisherOpts;
		conn_opts.ssl = &ssl_opts;
	}

	connected = 0;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		fprintf(stderr, "Failed to start connect, return code %s\n", MQTTAsync_strerror(rc));
		exit(EXIT_FAILURE);
	}
}


int mypublish(MQTTAsync client, int datalen, char* data)
{
	int rc;

	if (publisherOpts.verbose)
		printf("Publishing data of length %d\n", datalen);

	rc = MQTTAsync_send(client, publisherOpts.topic, datalen, data, publisherOpts.qos, publisherOpts.retained, &pub_opts);
	if (publisherOpts.verbose && rc != MQTTASYNC_SUCCESS && !publisherOpts.quiet)
		fprintf(stderr, "Error from MQTTAsync_send: %s\n", MQTTAsync_strerror(rc));

	return rc;
}


void trace_callback(enum MQTTASYNC_TRACE_LEVELS level, char* message)
{
	fprintf(stderr, "Trace : %d, %s\n", level, message);
}


//int main()
//{
void ConnectPublisher(MQTTAsync client)
{

	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
	MQTTAsync_createOptions create_opts = MQTTAsync_createOptions_initializer;
	char* buffer = NULL;
	char* url = NULL;
	int url_allocated = 0;
	int rc = 0;
	const char* version = NULL;

	if (publisherOpts.MQTTVersion >= MQTTVERSION_5)
	{
		pub_opts.onSuccess5 = onPublish5;
		pub_opts.onFailure5 = onPublishFailure5;

		if (publisherOpts.message_expiry > 0)
		{
			property.identifier = MQTTPROPERTY_CODE_MESSAGE_EXPIRY_INTERVAL;
			property.value.integer4 = publisherOpts.message_expiry;
			MQTTProperties_add(&props, &property);
		}
		if (publisherOpts.user_property.name)
		{
			property.identifier = MQTTPROPERTY_CODE_USER_PROPERTY;
			property.value.data.data = publisherOpts.user_property.name;
			property.value.data.len = (int)strlen(publisherOpts.user_property.name);
			property.value.value.data = publisherOpts.user_property.value;
			property.value.value.len = (int)strlen(publisherOpts.user_property.value);
			MQTTProperties_add(&props, &property);
		}
		pub_opts.properties = props;
	}
	else
	{
		pub_opts.onSuccess = onPublish;
		pub_opts.onFailure = onPublishFailure;
	}

	myconnect(client);

	while (!toStop)
	{
		int data_len = 0;
		int delim_len = 0;

		if (publisherOpts.stdin_lines)
		{
			buffer = (char*)malloc(publisherOpts.maxdatalen);

			delim_len = (int)strlen(publisherOpts.delimiter);
			do
			{
				buffer[data_len++] = getchar();
				if (data_len > delim_len)
				{
					if (strncmp(publisherOpts.delimiter, &buffer[data_len - delim_len], delim_len) == 0)
						break;
				}
			} while (data_len < publisherOpts.maxdatalen);

			rc = mypublish(client, data_len, buffer);
		}
		else
			mysleep(100);
	}

	if (publisherOpts.message == 0 && publisherOpts.null_message == 0 && publisherOpts.filename == 0)
		free(buffer);



	//return EXIT_SUCCESS;
}
