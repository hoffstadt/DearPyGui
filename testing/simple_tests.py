import unittest
import dearpygui.dearpygui as dpg


class TestSimple(unittest.TestCase):

    def setUp(self):

        dpg.create_context()

        with dpg.window() as self.window_id:

            self.item1 = dpg.add_button(label="item1")
            self.item2 = dpg.add_button(label="item2")
            self.item3 = dpg.add_button(label="item3")
            self.item4 = dpg.add_button(label="item4")
            self.item5 = dpg.add_button(label="item5")

        dpg.setup_dearpygui()

    def tearDown(self):
        dpg.stop_dearpygui()
        dpg.destroy_context()

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

class TestDragDrop(unittest.TestCase):

    # tests applying drag_callback, drop_callback, payload_type and binding items

    def setUp(self):

        dpg.create_context()

        with dpg.window() as self.window_id:

            def testy(sender, app, user):
                print(f"Sender: {dpg.get_item_type(sender)} {sender}, App Data: {app}, User Data:{user}")

            # Menus
            with dpg.menu_bar() as menu_bar:
                dpg.add_menu_item(label="menu item", payload_type="str", drop_callback=testy)
                with dpg.menu(label="menu", payload_type="str", drop_callback=testy):
                    dpg.add_menu_item(label="menu item")


            # basic
            with dpg.collapsing_header(label="basic") as basic:
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

            # color
            with dpg.collapsing_header(label="color") as color:
                with dpg.group() as color:
                    dpg.add_color_button([255,0,0,255])
                    dpg.add_color_edit([255,0,0,255])
                    dpg.add_colormap_button(label="Colormap Button 1")
                    dpg.add_color_picker((255, 0, 255, 255), label="Color Picker", width=200)
                dpg.add_colormap_slider(label="Colormap Slider 1", default_value=0.5, payload_type="str", drop_callback=testy)
                dpg.add_colormap_scale(label="Colormap Spectral", min_scale=-100, max_scale=150, payload_type="str", drop_callback=testy)

            # containers
            with dpg.collapsing_header(label="containers"):
                with dpg.group() as containers:
                    with dpg.collapsing_header():
                        btn = dpg.add_button()
                    with dpg.group(width=150):
                        dpg.add_button()
                    with dpg.tree_node():
                        dpg.add_button()
                with dpg.child_window(width=150, height=100, payload_type="str", drop_callback=testy):
                    pass

            # tab stuff
            with dpg.collapsing_header(label="tab bars"):
                with dpg.tab_bar():

                    with dpg.tab(label="tab", payload_type="str", drop_callback=testy):
                        pass
                    dpg.add_tab_button(label="tab button", payload_type="str", drop_callback=testy, drag_callback=testy)
                    with dpg.drag_payload(parent=dpg.last_item(), drop_data="dropped", drag_data="dragged", user_data="user data", payload_type="str"):
                        dpg.add_text(dpg.get_item_type(dpg.last_item()))
                        dpg.add_text(f"Item ID: {dpg.last_item()}")

            # custom
            with dpg.collapsing_header(label="custom"):
                with dpg.group() as custom:
                    dpg.add_date_picker()
                    dpg.add_knob_float()
                    dpg.add_3d_slider()
                    dpg.add_time_picker()
                dpg.add_loading_indicator(payload_type="str", drop_callback=testy)

            # misc
            with dpg.collapsing_header(label="misc"):
                with dpg.group() as misc:
                    dpg.add_progress_bar(label="progress bar", default_value=.5)

            # node
            with dpg.collapsing_header(label="node"):
                with dpg.node_editor() as node:
                    with dpg.node(pos=[20,20], draggable=False):
                        pass
                    with dpg.node(pos=[100,100], draggable=False):
                        pass

            # plots
            with dpg.collapsing_header(label="plot") as plot:
                with dpg.plot():
                    dpg.add_plot_legend(payload_type="str", drop_callback=testy)
                    dpg.add_plot_axis(dpg.mvXAxis, label="x", payload_type="str", drop_callback=testy)
                    with dpg.plot_axis(dpg.mvYAxis, label="y", payload_type="str", drop_callback=testy):
                        dpg.add_line_series([0,1,2,3,4,5], [0,1,2,3,4,5], label="data")


            self.test_bind_items = dpg.get_item_children(basic, slot=1)
            self.test_bind_items += dpg.get_item_children(color, slot=1)
            self.test_bind_items += dpg.get_item_children(containers, slot=1)
            self.test_bind_items += dpg.get_item_children(custom, slot=1)
            self.test_bind_items += dpg.get_item_children(misc, slot=1)
            self.test_bind_items += dpg.get_item_children(node, slot=1)
            self.test_bind_items += dpg.get_item_children(plot, slot=1)

        dpg.setup_dearpygui()

    def tearDown(self):
        dpg.stop_dearpygui()
        dpg.destroy_context()

    def testBindDragPayload(self):
        def testy(sender, app, user):
            print(f"Sender: {dpg.get_item_type(sender)} {sender}, App Data: {app}, User Data:{user}")

        for item in self.test_bind_items:
            # uncomment these to find where it fails
            #print(f'[TestDragDrop] Attempting bind {dpg.get_item_type(item)}')
            dpg.configure_item(item, payload_type="str", drop_callback=testy, drag_callback=testy)
            with dpg.drag_payload(parent=item, drop_data="dropped", drag_data="dragged", user_data="user data", payload_type="str"):
                dpg.add_text(dpg.get_item_type(item))
                dpg.add_text(f"Item ID: {item}")
            #print(f'[TestDragDrop] Completed bind {dpg.get_item_type(item)}')


class TestItemDetails(unittest.TestCase):
    def setUp(self):
        dpg.create_context()
        self.wndw = dpg.add_window()
        dpg.setup_dearpygui()

    def test_cfg_on_close_in_mvWindowAppItem(self):
        cfg1 = dpg.get_item_configuration(self.wndw)
        self.assertTrue("on_close" in cfg1)
        self.assertTrue(cfg1.get("on_close", 0) is None)

        cb_on_close = lambda sender, adata, udata: ...
        dpg.configure_item(self.wndw, on_close=cb_on_close)
        cfg2 = dpg.get_item_configuration(self.wndw)
        self.assertTrue("on_close" in cfg2)
        self.assertTrue(cfg2.get("on_close", 0) is cb_on_close)

        dpg.configure_item(self.wndw, on_close=None)
        cfg3 = dpg.get_item_configuration(self.wndw)
        self.assertTrue("on_close" in cfg3)
        self.assertTrue(cfg3.get("on_close", 0) is None)

    def test_info_mvItemHandlerRegistry_in_mvAll(self):
        info1 = dpg.get_item_info(self.wndw)
        self.assertTrue("handlers" in info1)
        self.assertTrue(info1.get("handlers", 0) is None)

        ihreg_id = dpg.add_item_handler_registry()
        dpg.bind_item_handler_registry(self.wndw, ihreg_id)
        info2 = dpg.get_item_info(self.wndw)
        self.assertTrue("handlers" in info2)
        self.assertTrue(info2.get("handlers", 0) == ihreg_id)

        dpg.bind_item_handler_registry(self.wndw, 0)
        info3 = dpg.get_item_info(self.wndw)
        self.assertTrue("handlers" in info3)
        self.assertTrue(info3.get("handlers", 0) is None)

    def test_cfg_multiline_in_mvInputText(self):
        input_txt = dpg.add_input_text(parent=self.wndw)
        cfg = dpg.get_item_configuration(input_txt)
        self.assertTrue("multline" not in cfg)
        self.assertTrue("multiline" in cfg)

    def test_cfg_delink_callback_in_mvNodeEditor(self):
        node_editor = dpg.add_node_editor(parent=self.wndw)
        cfg1 = dpg.get_item_configuration(node_editor)
        self.assertTrue("delink_callback" in cfg1)
        self.assertTrue(cfg1.get("delink_callback", 0) is None)

        dl_cb = lambda *args: ...
        dpg.configure_item(node_editor, delink_callback=dl_cb)
        cfg2 = dpg.get_item_configuration(node_editor)
        self.assertTrue("delink_callback" in cfg2)
        self.assertTrue(cfg2.get("delink_callback", 0) is dl_cb)

    def test_cfg_extensions_in_mvFileExtension(self):
        with dpg.file_dialog():
            ext = dpg.add_file_extension("*.*")
        cfg = dpg.get_item_configuration(ext)
        self.assertTrue("extension" in cfg)

    def test_cfg_dragdropdata_in_mvDragPayload(self):
        b = dpg.add_button(parent=self.wndw)
        payload = dpg.add_drag_payload(parent=b)

        cfg = dpg.get_item_configuration(payload)
        self.assertTrue("drag_data" in cfg)
        self.assertTrue("drop_data" in cfg)

    def tearDown(self):
        dpg.stop_dearpygui()
        dpg.destroy_context()


if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'], verbosity=2, exit=should_exit)
