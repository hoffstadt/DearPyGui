from dearpygui.core import *
from dearpygui.simple import *

set_log_level(0)
#enable_docking(shift_only=False, dock_space=True)


add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
set_font("google", 20)

show_logger()

with window("Testing"):

    #add_button("Always On Top", callback=lambda:configure_viewport(always_on_top=True))
    #add_button("Move Window", callback=lambda:configure_viewport(x_pos=500))
    #add_button("Size Window", callback=lambda:configure_viewport(width=500))
    #add_button("Remove Max", callback=lambda:configure_viewport(maximized_box=False))

    add_plot("Plot", height=400)
    add_slider_floatx("source", size=2)
    add_drag_line("dline1", color=[255, 0, 0, 255], parent="Plot")
    add_drag_line("dline2", color=[255, 255, 0, 255], vertical=False, parent="Plot")
    add_drag_point("dpoint1", color=[255, 0, 255, 255], parent="Plot", source="source")

    add_plot_annotation("BL", default_value=(0.25, 0.25), offset=(-15, 15), color=[255, 255, 0, 255], parent="Plot")
    add_plot_annotation("BR", default_value=(0.75, 0.25), offset=(15, 15), color=[255, 255, 0, 255], parent="Plot")
    add_plot_annotation("TR not clampled", default_value=(0.75, 0.75), offset=(-15, -15), color=[255, 255, 0, 255], clamped=False, parent="Plot")
    add_plot_annotation("TL", default_value=(0.25, 0.75), offset=(-15, -15), color=[255, 255, 0, 255], parent="Plot")
    add_plot_annotation("Center", default_value=(0.5, 0.5), color=[255, 255, 0, 255], parent="Plot")

create_viewport(title="Dear PyGui Demo", width=1000, height=800,
                x_pos=0, y_pos=0)

setup_dearpygui()
show_viewport()
while(is_dearpygui_running()):
    render_dearpygui_frame()   
cleanup_dearpygui()