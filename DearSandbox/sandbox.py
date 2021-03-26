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
with window("extra"):
        add_input_int3('start',min_clamped = True,max_clamped = True,max_value = 50,width=120,tip='H:M:S',callback=lambda: print("here"),on_enter=True,)
show_demo()
show_logger()

start_dearpygui()
