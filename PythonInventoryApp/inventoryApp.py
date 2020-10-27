import paho.mqtt.client as mqtt
import time

import Sendmail
import commons
import InventoryDbConnection as inventory




clientName = "PythonCT-InventoryApp"
brokerIP = "localhost"
topic = "Patient/Details"
#procedureCardiac = "Cardiac"
inventoryWarningThreshold = 5

discCode = 1

#--------------------------------------------------------------------------------------------------

def updateConsumableCount(consumables):
	global dbConnection
	for item in consumables:
		if((int(item[1])) <= inventoryWarningThreshold):
			sendAlert("WARNING!! \n"+item[0]+" STOCK LOW")
			return False
		else:
			equipName = item[0]
			equipCount = int(item[1]) - 1
			cmd = "UPDATE inventory SET equipmentCount="+str(equipCount)+" WHERE equipmentName='"+item[0]+"'"
			inventory.dbConnection.execute(cmd)
			inventory.dbConnection.commit()
	return True

def numberOfRows(procedure):
	global dbConnection
	cmd = commons.countRows + " WHERE procedure='" + procedure+"'"
	cursor = inventory.dbConnection.execute(cmd)
	for rows in cursor:
		numRows = rows[0]
	return numRows

def consumableCount(procedure):
	global dbConnection
	consumablesUsed = []
	if ( numberOfRows(procedure) > 0):
		cmd = "SELECT equipmentName, equipmentCount FROM inventory WHERE procedure='"+procedure+"'"
		cursor = inventory.dbConnection.execute(cmd)
		consumablesUsed = [ row for row in cursor ]
		updateConsumableCount(consumablesUsed)
		#print(consumablesUsed)
		return True
	else:
		return False

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

	inventory.openConnection()
	splitPublishedResult(str(message.payload.decode("utf-8")))
	

def disConnectCalled(client,userdata,rc):
	print("Disconnect called with rc = ",rc)
	global discCode
	discCode = rc

    
def splitPublishedResult(msg):
	global discCode
	res = msg.split('/')
	#procedure.append(res[2])
	consumableCount(res[2])
	return res[2]


def sendAlert(msg):
	print("ALERT")
	Sendmail.sendMail(msg)
	return msg  
    

#----------------------------------------------------------------------------------------------------
def runApp():
	client = mqtt.Client(clientName) #client instance
	#bind callbacks for message from other publisher and connection with broker and on disconnect
	client.on_connect = brokerAcknowledgementReceived        
	client.on_message = messageFromPublisher
	client.on_disconnect = disConnectCalled

	client.connect(brokerIP)    #connect to Broker
	time.sleep(2)
	client.loop_start()

	while(1):
		client.subscribe(topic)
		time.sleep(2)
		if discCode == 0 :
			break  

	client.loop_stop()
	time.sleep(1)
	client.disconnect()

	print(procedure)
	print(checkForCardiac(procedure))


#-----------------------------------------------------------------------------------------------------

if __name__ == '__main__':
	runApp()