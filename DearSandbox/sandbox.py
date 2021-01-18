from dearpygui.core import *
from dearpygui.simple import *
from dearpygui.demo import *
from math import sin, cos
import random
import time

set_log_level(0)
#enable_docking(shift_only=False, dock_space=True)

set_main_window_title("DearPyGui Demo")
set_main_window_size(1000, 800)
set_main_window_pos(0, 0)
add_additional_font("../../Resources/NotoSerifCJKjp-Medium.otf", 20)

# char remaps
#add_character_remap(0x0041, 0x00A2)
#add_character_remap(0x0061, 0x00AB)

#show_demo()
show_logger()


def delete_annote(sender, data):
    print("cow")

with window("new test"):
    add_image_button("b1", "../../Resources/SpriteMapExample.png", callback=delete_annote)
    with group("blah"):
        add_image_button("b2", "../../Resources/SpriteMapExample.png", callback=delete_annote)

start_dearpygui()

start_dearpygui()