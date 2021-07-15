import unittest
import dearpygui.dearpygui as dpg

class TestSimple(unittest.TestCase):

    def setUp(self):

        with dpg.window() as self.window_id:

            self.item1 = dpg.add_button(label="item1")
            self.item2 = dpg.add_button(label="item2")
            self.item3 = dpg.add_button(label="item3")
            self.item4 = dpg.add_button(label="item4")
            self.item5 = dpg.add_button(label="item5")

        dpg.setup_dearpygui()

    def tearDown(self):
        dpg.stop_dearpygui()
        dpg.cleanup_dearpygui()

    def test_moving_items(self):

        children = dpg.get_item_children(self.window_id, 1)

        dpg.move_item_down(self.item3)
        dpg.move_item_up(self.item2)
        dpg.move_item_up(self.item2)
        dpg.move_item_down(self.item5)

        children = dpg.get_item_children(self.window_id, 1)
        self.assertTrue(children[0] == self.item2)
        self.assertTrue(children[1] == self.item1)
        self.assertTrue(children[2] == self.item4)
        self.assertTrue(children[3] == self.item3)
        self.assertTrue(children[4] == self.item5)

        dpg.move_item(self.item5, before=self.item2)
        children = dpg.get_item_children(self.window_id, 1)
        self.assertTrue(children[0] == self.item5)
        self.assertTrue(children[1] == self.item2)
        self.assertTrue(children[2] == self.item1)
        self.assertTrue(children[3] == self.item4)
        self.assertTrue(children[4] == self.item3)



    def test_zelete_items(self):
        
        children = dpg.get_item_children(self.window_id, 1)

if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'], verbosity=2, exit=should_exit)
