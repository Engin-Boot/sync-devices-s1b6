import sqlite3
import _thread as thread



def openConnection(dbName):
    dbConnection = sqlite3.connect(dbName)
    return dbConnection

def closeConnection(dbConnection):
    dbConnection.close()
