import dearpygui.dearpygui as dpg

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())



with dpg.window(label="tutorial", width=500, height=500):
    
    with dpg.menu_bar(label="group"):

        with dpg.menu(label="menu 1"):
            dpg.add_button(label="b1")
            dpg.add_button(label="b2")
            dpg.add_button(label="b3")
            dpg.add_button(label="b4")
            with dpg.menu(label="menu 2"):
                dpg.add_button(label="b1")
                dpg.add_button(label="b2")
                dpg.add_button(label="b3")
                dpg.add_button(label="b4")

        with dpg.menu(label="menu 3"):
            dpg.add_button(label="b1")
            dpg.add_button(label="b2")
            dpg.add_button(label="b3")
            dpg.add_button(label="b4")
    dpg.add_button(label="postbutton")

dpg.show_tool(dpg.mvTool_ItemRegistry)
dpg.create_viewport()
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()