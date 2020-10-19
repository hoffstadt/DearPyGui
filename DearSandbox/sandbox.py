from dearpygui.core import *
from dearpygui.simple import *
from dearpygui.demo import *
from math import sin, cos
import random
import time

########################################################################################################################
# Settings and Data Storage
########################################################################################################################
set_log_level(0)

set_main_window_title("DearPyGui Demo")
set_main_window_size(1000, 800)
set_main_window_pos(0, 0)
add_additional_font("C:/dev/DearPyGui/Resources/NotoSerifCJKjp-Medium.otf", 20)

# char remaps
#add_character_remap(0x0041, 0x00A2)
#add_character_remap(0x0061, 0x00AB)

#add_image("image1", "C:/dev/DearPyGui/Examples/SpriteMapExample.png")
#add_image_button("image", "C:/dev/DearPyGui/Examples/SpriteMapExample.png")

show_demo()

with window("Asyncronous##dialog", show=True):
    add_data('threadNumber', 0)
    def LongCallback2(sender, data):
        time.sleep(5)
        return data

    def ReturnFromLongProcess(sender, data):
        log_info("Completed process number: " + str(data))
        add_data('threadNumber', get_data('threadNumber')-1)

    def LongAsyncronousCallback(sender, data):
        current_number = get_data('threadNumber')
        run_async_function(LongCallback2, current_number, return_handler=ReturnFromLongProcess)
        log_info("Start process number: " + str(current_number))
        add_data('threadNumber', current_number+1)

    def LongCallback(sender, data):
        time.sleep(5)
        log_info("Done with long process")

    add_button("Start Long Process", callback=LongCallback)
    add_button("Start Long Asyncronous Process", callback=LongAsyncronousCallback)

#start_dearpygui(primary_window="Asyncronous##dialog")
start_dearpygui()
