from dearpygui.core import *
from dearpygui.simple import *
from dearpygui.demo import *

set_log_level(0)
#enable_docking(shift_only=False, dock_space=True)

set_viewport_title("DearPyGui Demo")
set_viewport_size(1000, 800)
set_viewport_pos(0, 0)
add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
set_font("google", 20)

show_demo()
show_logger()

setup_dearpygui()
while(is_dearpygui_running()):
    demo_main_callback()
    render_dearpygui_frame()   
cleanup_dearpygui()