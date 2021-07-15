import unittest
import dearpygui.dearpygui

class TestSimple(unittest.TestCase):

    def test_1(self):
        self.assertTrue(True)

    def test_2(self):
        self.assertTrue(False)

if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'], exit=True, verbosity=2)
