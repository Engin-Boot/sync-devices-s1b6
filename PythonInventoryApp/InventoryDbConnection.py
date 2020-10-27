import sqlite3
import _thread as thread

dbConnection = sqlite3.Connection('')

def openConnection():
    global dbConnection
    #print(type(dbConnection))
    dbConnection = sqlite3.connect('inventory.db')
    return True

def closeConnection():
    global dbConnection
    print("in closeConnection = ",type(dbConnection))
    dbConnection.close()
