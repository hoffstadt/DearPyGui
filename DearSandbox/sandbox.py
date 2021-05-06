from dearpygui.core import *
from dearpygui.demo import *

add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
set_font("google", 20)

add_texture_container(id="mvTextureContainer")

show_demo()

vp = create_viewport(small_icon="../../Resources/dpg_icon_small.ico", large_icon="../../Resources/dpg_icon_large.ico")
setup_dearpygui(viewport=vp)
show_viewport(vp)
while(is_dearpygui_running()):
    render_dearpygui_frame()   
cleanup_dearpygui()