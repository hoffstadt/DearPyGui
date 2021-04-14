from dearpygui.core import *
from dearpygui.simple import *
from dearpygui.demo import *

add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
set_font("google", 20)

set_log_level(0)
show_logger()
show_demo()

create_viewport(small_icon="../../Resources/dpg_icon_small.ico", large_icon="../../Resources/dpg_icon_large.ico")
setup_dearpygui()
show_viewport()
while(is_dearpygui_running()):
    render_dearpygui_frame()   
cleanup_dearpygui()