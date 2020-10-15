
#pragma once
#include "MQTTAsync.h"
#include "MQTTClientPersistence.h"
#include "pubsub_opts.h"
#include "Publisher.h"
#include "Subscriber.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>


#if defined(_WIN32)
#include <windows.h>
#define sleep Sleep
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#if defined(_WRS_KERNEL)
#include <OsWrapper.h>
#endif

int main()
{
	MQTTAsync client;
	MQTTAsync_createOptions create_opts = MQTTAsync_createOptions_initializer;
	char* url = NULL;
	int x = subscribe(&client, create_opts, url);
	ConnectPublisher(&client);
	disconnect(&client);
}