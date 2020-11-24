import unittest
from datalogger import *

# für jede zu testende Funktion wird eine eigene Testklasse angelegt

class TestCollectMeasurements(unittest.TestCase):
    def test_measurements(self):
        # test if a dict is returned
        self.assertIsInstance(collect_mesurements)
