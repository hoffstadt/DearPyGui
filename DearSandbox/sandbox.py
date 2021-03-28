from dearpygui.core import *
from dearpygui.simple import *
from dearpygui.demo import *

set_log_level(0)
#enable_docking(shift_only=False, dock_space=True)

set_main_window_title("DearPyGui Demo")
set_main_window_size(1000, 800)
set_main_window_pos(0, 0)
add_additional_font("../../Resources/NotoSerifCJKjp-Medium.otf", 20)

show_demo()
show_logger()
with window("extra"):
    add_input_int("test", callback=lambda sender: log_debug(f"{sender} has ran a callback"))
    add_input_int("testy", max_value=500, min_value=0, callback=lambda sender: log_debug(f"{sender} has ran a callback"), default_value=5)
    add_input_int("testyy", max_value=500, min_value=0, callback=lambda sender: log_debug(f"{sender} has ran a callback"), default_value=0)
    add_input_int("testy_enter", max_value=500, min_value=0, callback=lambda sender: log_debug(f"{sender} has ran a callback"), on_enter=True)
    add_input_int3("testy3", max_value=500, min_value=0, callback=lambda sender: log_debug(f"{sender} has ran a callback"))
    add_input_int3("testy3_enter", max_value=500, min_value=0, callback=lambda sender: log_debug(f"{sender} has ran a callback"), on_enter=True)
    add_slider_int("slide", max_value=500, min_value=0, callback=lambda sender: log_debug(f"{sender} has ran a callback"), clamped=True)
    add_slider_int("slide", max_value=500, min_value=0, callback=lambda sender: log_debug(f"{sender} has ran a callback"), clamped=True, on_enter=True)
setup_dearpygui()
while(is_dearpygui_running()):
    demo_main_callback()
    render_dearpygui_frame()   
cleanup_dearpygui()