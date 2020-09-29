from dearpygui.core import *
from dearpygui.simple import *

add_button("Hover me##tooltips")
with tooltip("Hover me##tooltips", "tool_tip##tooltips"):
    add_simple_plot("Simpleplot##tooltips", value=[0.3, 0.9, 2.5, 8.9], height=80)

add_button("Modal Window")
with popup("Modal Window", "ModalPopup", modal=True):
    add_simple_plot("Simpleplot##modal", value=[0.3, 0.9, 2.5, 8.9], height=80)
    add_button("Close Window##modal", callback=close_popup)
add_text("Right Click Me")
with popup("Right Click Me", "RegularPopup", mousebutton=mvMouseButton_Right):
    add_simple_plot("Simpleplot##popup", value=[0.3, 0.9, 2.5, 8.9], height=80)

start_dearpygui()
