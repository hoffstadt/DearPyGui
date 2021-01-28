from dearpygui.core import *
from dearpygui.simple import *

with window("Test Window"):
    add_text("Press me")
    with popup("Press me", "popup1", modal=True, mousebutton=mvMouseButton_Left):
        add_text("Popuptext")
        add_button("Close", callback=lambda: close_popup("popup1"))

start_dearpygui()