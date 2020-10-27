@echo off

g++ Mqtt_C_Client\*.cpp -I include -L lib -l paho-mqtt3c -o Client
move Client.exe bin
cd bin
start cmd /k Client.exe Client-1 
start cmd /k Client.exe Client-2 
start cmd /k Client.exe Client-3 

exit