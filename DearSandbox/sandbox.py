import dearpygui.dearpygui as dpg
import threading
import time
import random

width = 400
height = 400
#grid_arr = np.random.rand(width, height, 4)
grid_arr = dpg.mvBuffer(width*height*4)
grid_arr.clear_value(random.random())

with dpg.texture_registry():
    image_id = dpg.add_raw_texture(width,
                                   height,
                                   grid_arr,
                                   format=dpg.mvFormat_Float_rgba)

with dpg.window(label="Tutorial"):
    with dpg.drawlist(width=width, height=height):
        dpg.draw_image(image_id, (0, 0), (width, height))


def setter():
    # don't move onto update loop until starting DPG
    while not dpg.is_dearpygui_running():
        time.sleep(0.01)

    # no need to sleep, the dpg call will bound the thread
    while dpg.is_dearpygui_running():

        b1 = dpg.mvBuffer(640000)
        b1.clear_value(random.random())
        dpg.set_value(image_id, b1)
        #time.sleep(0.01)



t = threading.Thread(target=setter)
t.start()

dpg.start_dearpygui()