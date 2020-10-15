#include "include/Subscriber.h"
#include "PET-CTdata.h"

int sub_messageArrived(void* context, char* topic, int topicLen, MQTTAsync_message* message)
{
	string topicName(topic);

	if (topicName == "Patient/Details") {
		cout << "Subscription end: " << (char*)message->payload << endl;
		AllotDetails((char*)message->payload);
	}
	/*if (topicName == "Patient/Add") {
	cout << "Adding information" << (char*)message->payload << endl;
	AllotDetails((char*)message->payload);
	}*/

	fflush(stdout);
	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(topic);
	return 1;
}

void sub_onDisconnect(void* context, MQTTAsync_successData* response)
{
	disconnected = 1;
}

void onSubscribe(void* context, MQTTAsync_successData* response)
{
	subscribed = 1;
}

void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	if (!s_opts.quiet)
		fprintf(stderr, "Subscribe failed, rc %s\n",
			MQTTAsync_strerror(response->code));
	finished = 1;
}

void sub_onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	if (!opts.quiet)
		fprintf(stderr, "Connect failed, rc %s\n", response ? MQTTAsync_strerror(response->code) : "none");
	finished = 1;
}


void sub_onConnect(void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions ropts = MQTTAsync_responseOptions_initializer;
	int rc;

	ropts.onSuccess = onSubscribe;
	ropts.onFailure = onSubscribeFailure;
	ropts.context = client;
	if ((rc = MQTTAsync_subscribe(client, opts.topic, opts.qos, &ropts)) != MQTTASYNC_SUCCESS)
	{
		fprintf(stderr, "Failed to start subscribe, return code %s\n", MQTTAsync_strerror(rc));
		finished = 1;
	}
}

void sub_createClient(MQTTAsync* client, string url, MQTTAsync_createOptions* create_opts) {
	int rc = MQTTAsync_createWithOptions(client, url.c_str(), opts.clientid, MQTTCLIENT_PERSISTENCE_NONE, NULL, create_opts);
	if (rc != MQTTASYNC_SUCCESS)
	{
		fprintf(stderr, "Failed to create client, return code: %s\n", MQTTAsync_strerror(rc));
		exit(EXIT_FAILURE);
	}
}

void sub_setCallbacks(MQTTAsync* client) {
	int rc = MQTTAsync_setCallbacks(*client, *client, NULL, sub_messageArrived, NULL);
	if (rc != MQTTASYNC_SUCCESS)
	{
		fprintf(stderr, "Failed to set callbacks, return code: %s\n", MQTTAsync_strerror(rc));
		exit(EXIT_FAILURE);
	}
}

void sub_connectToBroker(MQTTAsync* client) {
	int rc;
	if ((rc = MQTTAsync_connect(*client, &sub_opts)) != MQTTASYNC_SUCCESS)
	{
		fprintf(stderr, "Failed to start connect, return code %s\n", MQTTAsync_strerror(rc));
		exit(EXIT_FAILURE);
	}
}

int subscribe() {
	MQTTAsync client;
	MQTTAsync_createOptions create_opts = MQTTAsync_createOptions_initializer;
	const char* url = "localhost:1883";

	sub_createClient(&client, url, &create_opts);

	sub_setCallbacks(&client);

	sub_opts.onSuccess = sub_onConnect;
	sub_opts.onFailure = sub_onConnectFailure;
	sub_opts.cleansession = 1;
	sub_opts.keepAliveInterval = s_opts.keepalive;
	sub_opts.MQTTVersion = s_opts.MQTTVersion;
	sub_opts.context = client;
	sub_opts.automaticReconnect = 1;

	sub_connectToBroker(&client);

	while (!subscribed) {
		sleep(100);
	}

	cout << "Subscription to the topic was successful" << endl;
	return 0;
}