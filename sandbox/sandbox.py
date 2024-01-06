
import dearpygui.dearpygui as dpg
import time

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

i = 0

def resize_viewport_callback():
    dpg.set_value("ViewportWidthText", f"Viewport Width: {dpg.get_viewport_width()}")
    dpg.set_value("ViewportHeightText", f"Viewport Height: {dpg.get_viewport_height()}")
    dpg.set_value("LastViewportSizeUpdateText", f"Time: {time.time_ns()}")
    global i
    print(i)
    i+=1

dpg.set_viewport_resize_callback(resize_viewport_callback)

with dpg.window(label="Example Window", tag="Window"):
    dpg.add_text("Viewport Width: ", tag="ViewportWidthText")
    dpg.add_text("Viewport Height: ", tag="ViewportHeightText")
    dpg.add_text("Time: ", tag="LastViewportSizeUpdateText")

dpg.set_primary_window("Window", True)

dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()