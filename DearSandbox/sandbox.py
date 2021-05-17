import dearpygui.core as dpg
import dearpygui.simple as smpl
import dearpygui.demo as demo

dpg.add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
dpg.set_font("google", 20)

dpg.add_texture_container(id="mvTextureContainer")

demo.show_demo()

smpl.start_dearpygui()