import unittest
import os
import sys

impPath = '../PythonInventoryApp/'
sys.path.append(os.path.abspath(impPath))
import inventoryApp as inventory
import InventoryDbConnection as dbConnection

class TestInventoryApp(unittest.TestCase):
    def test_when_send_alert_is_called_with_a_message(self):
        msg = "Warning, low on supplies"
        self.assertEqual(inventory.sendAlert(msg), msg)
    
    def test_when_a_message_recieved_by_inventory_app_and_splitPublishedResult_called(self):
        msg1 = 'Alice/21/cardiac/F'
        msg2 = 'Bob/21/ct/M'
        msg3 = 'Carl/20/mri/m'

        conn = dbConnection.openConnection('test_inventory_database.db')
        self.assertEqual(inventory.splitPublishedResult(msg1, conn), 'cardiac')
        self.assertEqual(inventory.splitPublishedResult(msg2, conn), 'ct')
        self.assertEqual(inventory.splitPublishedResult(msg3, conn), 'mri')
        dbConnection.closeConnection(conn)
    
    def test_when_procedure_recieved_in_message_then_check_if_consumable_used(self):
        procedureWithConsumabe = 'cardiac'
        procedureWithoutConsumable = 'ct'

        conn = dbConnection.openConnection('test_inventory_database.db')
        self.assertTrue(inventory.consumableCount(procedureWithConsumabe, conn))
        self.assertFalse(inventory.consumableCount(procedureWithoutConsumable, conn))
        dbConnection.closeConnection(conn)

    def test_when_a_procedure_is_recieved_in_message_then_check_if_consumable(self):
        procedureWithConsumabe = 'cardiac'
        procedureWithoutConsumable = 'ct'

        conn = dbConnection.openConnection('test_inventory_database.db')
        self.assertGreater(inventory.numberOfRows(procedureWithConsumabe, conn), 0)
        self.assertEqual(inventory.numberOfRows(procedureWithoutConsumable, conn), 0)
        dbConnection.closeConnection(conn)

    def test_when_updateConsumableCount_called_then_update_count_in_database(self):
        consumable1 = [ ('catheter', 10) ]
        consumable2 = [ ('catheter', 0) ]

        conn = dbConnection.openConnection('test_inventory_database.db')
        self.assertTrue(inventory.updateConsumableCount(consumable1, conn))
        self.assertFalse(inventory.updateConsumableCount(consumable2, conn))
        dbConnection.closeConnection(conn)


if __name__ == '__main__':
    unittest.main()
