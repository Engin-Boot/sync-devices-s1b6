import sqlite3

def openConnection(dbName):
    dbConnection = sqlite3.connect(dbName)
    return dbConnection

def closeConnection(dbConnection):
    dbConnection.close()
