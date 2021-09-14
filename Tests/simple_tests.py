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
        dpg.setup_registries()

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

    def test_dragPayload(self):
        with dpg.window():

            def testy(sender, app, user):
                print(sender, app, user)

            with dpg.menu_bar() as menu_bar:
                dpg.add_menu_item(label="menu item", payload_type="str", drop_callback=testy)
                with dpg.drag_payload(parent=dpg.last_item(), drop_data="dropped", drag_data="dragged", user_data="user data", payload_type="str"):
                    dpg.add_text(dpg.get_item_type(dpg.last_item()))
                    dpg.add_text(f"Item ID: {dpg.last_item()}")

            with dpg.group() as basic_group:
                dpg.add_image(dpg.mvFontAtlas)
                dpg.add_image_button(dpg.mvFontAtlas)
                dpg.add_text("this is a text widget")
                dpg.add_checkbox(label="checkbox")
                dpg.add_button(label="button")
                dpg.add_input_float(label="input float")
                dpg.add_input_floatx(label="input floatx")
                dpg.add_drag_int(label="drag int")
                dpg.add_drag_intx(label="drag intx")
                dpg.add_input_text(label="input text")
                dpg.add_slider_float(label="slider float")
                dpg.add_slider_floatx(label="slider floatx")
                dpg.add_listbox(label="listbox")
                dpg.add_selectable(label="selectable")
                dpg.add_radio_button(["item 1", "item 2"],label="radio button")

            for item in dpg.get_item_children(basic_group, slot=1):
                dpg.configure_item(item, payload_type="str", drop_callback=testy, drag_callback=testy)
                with dpg.drag_payload(parent=item, drop_data="dropped", drag_data="dragged", user_data="user data", payload_type="str"):
                    dpg.add_text(dpg.get_item_type(item))
                    dpg.add_text(f"Item ID: {item}")

            with dpg.tab_bar():
                dpg.add_tab_button(label="tab button", payload_type="str", drop_callback=testy, drag_callback=testy)
                with dpg.drag_payload(parent=dpg.last_item(), drop_data="dropped", drag_data="dragged", user_data="user data", payload_type="str"):
                    dpg.add_text(dpg.get_item_type(dpg.last_item()))
                    dpg.add_text(f"Item ID: {dpg.last_item()}")

if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'], verbosity=2, exit=should_exit)
