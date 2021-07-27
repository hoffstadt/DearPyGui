import dearpygui.dearpygui as dpg
with dpg.window(label="Example Window") as main_window:
    dpg.add_text('music player')

dpg.setup_viewport()
dpg.set_viewport_caption(value=False)
dpg.set_viewport_border(False)
dpg.set_primary_window(main_window, True)
dpg.start_dearpygui()