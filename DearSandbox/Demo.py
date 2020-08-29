from dearpygui import *
from math import sin, cos

set_log_level(0)

show_debug()
show_logger()

#add_image("im", "C:/dev/DearPyGui/Examples/SpriteMapExample.png")

add_drawing("canvas", width=500, height=500)
#draw_image("canvas", "C:/dev/DearPyGui/Examples/SpriteMapExample.png", pmin=[0,0])
setup_dearpygui()
while is_dearpygui_running():
    clear_drawing("canvas")
    
    draw_image("canvas", "C:/dev/DearPyGui/Examples/SpriteMapExample.png", pmin=[0,0])
    #draw_circle("canvas", [100,100], 50, [255,25,25,255])
    render_dearpygui_frame()
cleanup_dearpygui()