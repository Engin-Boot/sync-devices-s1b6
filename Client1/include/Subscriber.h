
#pragma once
int subscribe(MQTTAsync client, MQTTAsync_createOptions create_opts, char* url);
void disconnect(MQTTAsync *client);