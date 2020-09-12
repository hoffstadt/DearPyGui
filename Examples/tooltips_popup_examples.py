from dearpygui.dearpygui import *

add_button("Hover me##tooltips")
add_tooltip("Hover me##tooltips", "tool_tip##tooltips")
add_simple_plot("Simpleplot##tooltips", [0.3, 0.9, 2.5, 8.9], height=80)
end()
add_button("Modal Window")
add_popup("Modal Window", "ModalPopup", modal=True)
add_simple_plot("Simpleplot##modal", [0.3, 0.9, 2.5, 8.9], height=80)
add_button("Close Window##modal", callback=close_popup)
end()
add_text("Right Click Me")
add_popup("Right Click Me", "RegularPopup", mousebutton=mvMouseButton_Right)
add_simple_plot("Simpleplot##popup", [0.3, 0.9, 2.5, 8.9], height=80)
end()

start_dearpygui()
