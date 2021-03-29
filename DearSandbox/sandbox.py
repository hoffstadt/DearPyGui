from dearpygui.core import *
from dearpygui.simple import *
from dearpygui.demo import *

set_log_level(0)
#enable_docking(shift_only=False, dock_space=True)

set_main_window_title("DearPyGui Demo")
set_main_window_size(1000, 800)
set_main_window_pos(0, 0)
add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)

show_demo()
show_logger()

setup_dearpygui()
while(is_dearpygui_running()):
    demo_main_callback()
    render_dearpygui_frame()   
cleanup_dearpygui()