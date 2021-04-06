from dearpygui.core import *
from dearpygui.simple import *

set_log_level(0)
#enable_docking(shift_only=False, dock_space=True)

add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
set_font("google", 20)

show_logger()
show_metrics()

with window("Testing", width=500, height=500):

    add_button("Always On Top", callback=lambda:configure_viewport(always_on_top=True))
    add_button("Move Window", callback=lambda:configure_viewport(x_pos=500))
    add_button("Size Window", callback=lambda:configure_viewport(width=500))
    add_button("Remove Max", callback=lambda:configure_viewport(maximized_box=False))

create_viewport(title="Dear PyGui Demo", width=1000, height=800,
                x_pos=0, y_pos=0)

setup_dearpygui()
show_viewport()
while(is_dearpygui_running()):
    render_dearpygui_frame()   
cleanup_dearpygui()