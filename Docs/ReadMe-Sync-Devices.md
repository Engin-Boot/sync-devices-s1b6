# <center> Sync-Devices

## <center> MQTT C-Client

- INITIAL REQUIREMENTS :
  
  - __*Mosquitto x64*__ needs to be installed for Broker to be running.
  [Mosquitto Download](https://mosquitto.org/download/)
  
- BUILD SRC FILES
  - `g++ Mqtt_C_Client\*.cpp -I include -L lib -l paho-mqtt3c -o Client` on executing this command a client application will be generated (Be in Root folder before executing this command).
  - Or run Build batch file `Build.bat` in root folder which opens 3 instances of clients.

- STEPS TO RUN APPLICATION

  - To Run several instances of Client call it with different client ID's in different terminals.

    Example : `Client.exe Client-1`

  - Press __p__ to Publish & __q__ to quit.

- STRUCTURE OF CODE:

  - There is one client application which can be allocated with different client-ID by specifying it as a command line argument.

  - The Client app handles the subscription and publishing of messages with other clients.

---

## <center> Python Client

- INITIAL REQUIREMENTS :

  - install paho.mqtt.client package `pip install paho-mqtt`
  
  - __*Mosquitto x64*__ needs to be installed for Broker to be running.
  [Mosquitto Download](https://mosquitto.org/download/)

 - STEPS TO RUN PYTHON CLIENT

    - Execute the python script being in the root folder.

      Command : `python PythonClient\pythonClient.py`

    - Press __p__ to Publish & __q__ to quit.

---

## <center> Python Inventory Application

- INITIAL REQUIREMENTS :

  - install paho.mqtt.client package `pip install paho-mqtt`
  
  - __*Mosquitto x64*__ needs to be installed for Broker to be running.
  [Mosquitto Download](https://mosquitto.org/download/)

 - STEPS TO RUN PYTHON INVENTORY CLIENT

    - Execute the python script being in the root folder.

    - Command :

      `python PythonInventoryApp\inventoryApp.py`

    - Provide the user credentials to send an email alert.

---

USEFUL LINKS:

- __*[MQTT Client library for C](https://www.eclipse.org/paho/files/mqttdoc/MQTTClient/html/index.html)*__
- __*[Python MQTT Client](http://www.steves-internet-guide.com/into-mqtt-python-client/)*__
