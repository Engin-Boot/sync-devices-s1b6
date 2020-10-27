# Synchronize information across devices

A medical operation room has equipment from multiple vendors,
each specialized in its function.
E.g., X-Ray, Ultrasound, Patient-monitors, displays

Before an operation, the hospital staff needs to ensure that
the patient information is entered in all of this equipment.
Other information generated during the operation needs to be
synchronized across the devices as well -
E.g., the name of the procedure, consumables used.

A sync mechanism is needed to improve staff efficiency.
Sync information across devices in an operation room,
so that the information needs to be entered only once.

Given that the sync will happen across multiple vendors and platforms,
the MQTT publish-subscribe mechanism is a good starting point.

You can use [Paho MQTT client](https://github.com/eclipse/paho.mqtt.c).
Download and build the source using cbuild.bat

Deliverables:

- At least three Console applications sync information
among themselves.
- The sync mechanism is testable without having to run
all of them and check the console manually.
- One of the subscribers maintains the total number
of patients in a file.

## Extended Functionality

- Make one of the clients in python. You can use [this guide](http://www.steves-internet-guide.com/into-mqtt-python-client/)
to start
- Whenever a 'cardiac' procedure is chosen, a cardiac cathether is used.
Make an inventory application that keeps counting down the number of
catheters in stock.
Send an email alert when the stock is low.
