import time

import paho.mqtt.client as mqtt

clientName = "PythonCT"
brokerIP = "localhost"
topic = "Patient/Details"

#--------------------------------------------------------------------------------------------------
  
def brokerAcknowledgementReceived(client,userdata,flags,rc):  #to verify acknowledgement
    if rc == 0:
        print("Acknowledgement receieved from Broker\n")

    else:
        print("Bad Connection\n")        


def messageFromPublisher(client,userdata,message):
    print()
    print("Message received: ",str(message.payload.decode("utf-8")))
    print("Topic : ",message.topic)
    print()


def disConnectCalled(client,userdata,rc):
    print("Disconnect called with rc = ",rc)

#----------------------------------------------------------------------------------------------------

client = mqtt.Client(clientName) #client instance

#bind callbacks for message from other publisher and connection eith broker
client.on_connect = brokerAcknowledgementReceived        
client.on_message = messageFromPublisher
client.on_disconnect = disConnectCalled

client.connect(brokerIP)    #connect to Broker
time.sleep(2)
client.loop_start()

while(1):
    client.subscribe(topic)
    time.sleep(5)
    option = input("Enter p to Publish: ")
    if(option == 'p'):
        msg = input("Enter message to be entered: (Name/Age/Procedure-Name/Gender/)")
        client.publish(topic,msg)
    elif(option == 'q'):
        break   

client.loop_stop()
client.disconnect()

#-----------------------------------------------------------------------------------------------------










