from dearpygui.core import *
from dearpygui.simple import *

add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
set_font("google", 20)

set_log_level(0)
show_logger()
show_metrics()

textdata = []
for i in range(0, 16):
    textdata.append(255/255)
    textdata.append(0)
    textdata.append(255/255)
    textdata.append(255/255)
add_texture(4, 4, textdata, id="#statictexture", parent="TextureRegistry", dynamic=True)

with window(label="Testing"):
    plot = add_plot(width=-1, height=-1, equal_aspects=True)
    add_image_series("#statictexture", [0,0], [4, 4], id="static", parent=plot)

vp = create_viewport()
setup_dearpygui(viewport=vp)
show_viewport(vp)
index = 0
while(is_dearpygui_running()):

    render_dearpygui_frame()   

    textdata = []
    for i in range(0, 16):
        textdata.append(255/255)
        textdata.append((index%255)/255)
        textdata.append(255/255)
        textdata.append(255/255)

    #set_value("#statictexture", textdata)

    index+=1


    
cleanup_dearpygui()