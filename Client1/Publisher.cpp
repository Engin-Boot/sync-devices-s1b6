
#include"include/Publisher.h"


int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* m)
{
	/* not expecting any messages */
	return 1;
}

void onDisconnect(void* context, MQTTAsync_successData* response)
{
	pub_disconnected = 1;
}


void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	fprintf(stderr, "Connect failed, rc %s\n", response ? MQTTAsync_strerror(response->code) : "none");
	pub_connected = -1;

}


void onConnect(void* context, MQTTAsync_successData* response)
{

	int rc = 0;

	if (opts.verbose)
		printf("Connected\n");

	pub_connected = 1;
}


void onPublishFailure(void* context, MQTTAsync_failureData* response)
{
	if (opts.verbose)
		fprintf(stderr, "Publish failed, rc %s\n", MQTTAsync_strerror(response->code));
	pub_published = -1;
}



void onPublish(void* context, MQTTAsync_successData* response)
{
	if (opts.verbose)
		printf("Publish succeeded\n");

	pub_published = 1;
}

int publish(string topicName, string message) {
	int rc;
	int datalen = (int)message.length();

	rc = MQTTAsync_send(pub_client, topicName.c_str(), datalen, message.c_str(), opts.qos, opts.retained, &pub_opts);
	if (rc != MQTTASYNC_SUCCESS)
		fprintf(stderr, "Error from MQTTAsync_send: %s\n", MQTTAsync_strerror(rc));
	return rc;
}

void myconnect(MQTTAsync client)
{
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc = 0;

	if (opts.verbose)
		printf("Connecting\n");

	conn_opts.keepAliveInterval = opts.keepalive;
	conn_opts.username = opts.username;
	conn_opts.password = opts.password;
	conn_opts.MQTTVersion = opts.MQTTVersion;
	conn_opts.context = pub_client;
	conn_opts.automaticReconnect = 1;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.onSuccess = onConnect;

	
	pub_connected = 0;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		fprintf(stderr, "Failed to start connect, return code %s\n", MQTTAsync_strerror(rc));
		exit(EXIT_FAILURE);
	}
}


void ConnectPublisher()
{

	MQTTAsync_createOptions create_opts = MQTTAsync_createOptions_initializer;
	char* url = "localhost:1883";
	int rc = 0;
	
	
	create_opts.sendWhileDisconnected = 1;

	rc = MQTTAsync_createWithOptions(&pub_client, url, opts.clientid, MQTTCLIENT_PERSISTENCE_NONE, NULL, &create_opts);
	if (rc != MQTTASYNC_SUCCESS)
	{
		fprintf(stderr, "Failed to create client_pub, return code: %s\n", MQTTAsync_strerror(rc));
		exit(EXIT_FAILURE);
	}

	rc = MQTTAsync_setCallbacks(pub_client, pub_client, NULL, messageArrived, NULL);
	if (rc != MQTTASYNC_SUCCESS)
	{
		fprintf(stderr, "Failed to set callbacks, return code: %s\n", MQTTAsync_strerror(rc));
		exit(EXIT_FAILURE);
	}

	pub_opts.onSuccess = onPublish;
	pub_opts.onFailure = onPublishFailure;

	myconnect(pub_client);

}


