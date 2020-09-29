from dearpygui.core import *
from dearpygui.simple import *

add_text("Tips")
add_text("The data storage system allows you to save python objects and "
         "retrieve them later. Usually between different callbacks", bullet=True)
add_text("It can also be used to have widgets with similar underlying "
         "data to share their data live.", bullet=True)
add_text("Move one of the widgets around", bullet=True)

add_spacing()
add_separator()
add_spacing()

add_slider_float("Slider 1", default_value=0.5, vertical=True, source="float1", max_value=1)
add_progress_bar("Progress 1", 0.5, source="float1")

add_spacing(count=5)
add_separator()
add_spacing(count=5)

add_input_float3("Input Float3", default_value=[23, 42, 53, 255], source="color1")
add_drag_float3("Drag Float3", min_value=0, max_value=255, speed=1, source="color1")
add_slider_float3("Slider Float3", min_value=0, max_value=255, source="color1")
add_color_edit3("Color Edit3",source="color1")
add_color_picker3("Color Picker3", width=300,source="color1")

start_dearpygui()