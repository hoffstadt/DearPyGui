import dearpygui.core as dpg
import dearpygui.simple as smpl
import dearpygui.contexts as cxt
from math import sin, cos
import random

def event_handler(sender, data):
    print(sender, "\t", data)

with cxt.handler_registry(show=False):

    dpg.add_key_down_handler(dpg.mvKey_A, callback=event_handler)
    dpg.add_key_release_handler(dpg.mvKey_B, callback=event_handler)
    dpg.add_key_press_handler(dpg.mvKey_C, callback=event_handler)
    dpg.add_mouse_wheel_handler(callback=event_handler)
    dpg.add_mouse_click_handler(-1, callback=event_handler)
    dpg.add_mouse_double_click_handler(-1, callback=event_handler)
    dpg.add_mouse_release_handler(-1, callback=event_handler)
    dpg.add_mouse_drag_handler(0, 10.0, callback=event_handler)
    dpg.add_mouse_down_handler(-1, callback=event_handler)
    dpg.add_mouse_move_handler(callback=event_handler)
    

dpg.add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
dpg.set_font("google", 20)

dpg.add_texture_container(id="mvTextureContainer")
dpg.add_static_texture(100, 100, [], parent="mvTextureContainer", file="INTERNAL_DPG_FONT_ATLAS", id="INTERNAL_DPG_FONT_ATLAS")

# temporary, just to ensure the font atlas is ready
# for the demo.
#import dearpygui.demo as demo

#demo.show_demo()

#dpg.show_tool("mvItemRegistry")

sindatax = []
sindatay = []
for i in range(0, 1000):
    sindatax.append(i/1000)
    sindatay.append(0.5 + 0.5*sin(50*i/1000))

with cxt.window(label="Drag Drop Testing"):

    plot_id = ""
    yaxis1 = ""
    yaxis2 = ""
    legend_id = ""

    def drag_callback(sender, data):
        pass
        #dpg.set_value("source name", sender)

    def drop_callback(sender, data):
        if sender == plot_id or sender == legend_id:
            dpg.add_line_series(data[0], data[1], label="0.5 + 0.5 * sin(x)", parent=yaxis1)
        else:
            dpg.add_line_series(data[0], data[1], label="0.5 + 0.5 * sin(x)", parent=sender)

    b1 = dpg.add_button(label="Source Button", drag_callback=drag_callback)
    b2 = dpg.add_button(label="Target1 Button", drop_callback=drop_callback)
    b3 = dpg.add_button(label="Target2 Button", drop_callback=drop_callback, payload_type="plots123")
    
    with cxt.drag_payload(parent=b1, payload_type="plots", drag_data=[sindatax, sindatay]):
        #dpg.add_text("none", label="Source: ", id="source name")
        dpg.add_simple_plot(default_value=sindatay)



    plot_id = dpg.add_plot(label="Line Plot", x_axis_name="x", y_axis_name="y", height=400, drop_callback=drop_callback, payload_type="plots1")
    yaxis1 = dpg.add_plot_yaxis(label="New Y1 axis", parent=plot_id, drop_callback=drop_callback, payload_type="plots")
    yaxis2 = dpg.add_plot_yaxis(label="New Y2 axis", parent=plot_id, drop_callback=drop_callback, payload_type="plots")
    #dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=yaxis1)
    #dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x2)", parent=yaxis2)
    legend_id = dpg.add_plot_legend(parent=plot_id, drop_callback=drop_callback, payload_type="plots")

smpl.start_dearpygui()