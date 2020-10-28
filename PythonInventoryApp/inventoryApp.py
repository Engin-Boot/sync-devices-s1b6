import time

import paho.mqtt.client as mqtt

import Sendmail
import InventoryDbConnection as dbConnection


clientName = "PythonCT-InventoryApp"
brokerIP = "localhost"
topic = "Patient/Details"
#procedureCardiac = "Cardiac"
inventoryWarningThreshold = 5

discCode = 1

#--------------------------------------------------------------------------------------------------

def updateConsumableCount(consumables, conn):
	#global dbConnection
	for item in consumables:
		if(int(item[1]) == 0):
			sendAlert("WARNING!! \n"+item[0]+"STOCK EMPTY")
			return False
		elif((int(item[1])) <= inventoryWarningThreshold):
			sendAlert("WARNING!! \n"+item[0]+" STOCK LOW")
			
		equipName = item[0]
		equipCount = int(item[1]) - 1
		cmd = "UPDATE inventory SET equipmentCount="+str(equipCount)+" WHERE equipmentName='"+item[0]+"'"
		try:
			conn.execute(cmd)
			conn.commit()
		except:
			print("In updateConsumableCont : Database Connection Error")
			return False
			
	return True

def numberOfRows(procedure, conn):
	#global dbConnection
	cmd = "SELECT COUNT(*) FROM inventory WHERE procedure='" + procedure+"'"
	try:
		cursor = conn.execute(cmd)
	except:
		print("In numberOfRows : Database Connection Error")
		return -1
	for rows in cursor:
		numRows = rows[0]
	return numRows

def consumableCount(procedure, conn):
	#global dbConnection
	consumablesUsed = []
	if ( numberOfRows(procedure, conn) > 0):
		cmd = "SELECT equipmentName, equipmentCount FROM inventory WHERE procedure='"+procedure+"'"
		try:
			cursor = conn.execute(cmd)
		except:
			print("In consumableCount : Database Connection Error")
			return False
		
		consumablesUsed = [ row for row in cursor ]
		updateConsumableCount(consumablesUsed, conn)
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

	conn = dbConnection.openConnection('test_inventory_database.db')
	splitPublishedResult(str(message.payload.decode("utf-8")), conn)
	

def disConnectCalled(client,userdata,rc):
	print("Disconnect called with rc = ",rc)
	global discCode
	discCode = rc

    
def splitPublishedResult(msg, conn):
	res = msg.split('/')
	consumableCount(res[2], conn)
	return res[2]


def sendAlert(msg):
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


#-----------------------------------------------------------------------------------------------------

if __name__ == '__main__':
	runApp()