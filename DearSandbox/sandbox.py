import dearpygui.core as dpg
import dearpygui.simple as smpl
import dearpygui.contexts as cxt
import dearpygui.demo as demo

dpg.add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
dpg.set_font("google", 20)

dpg.add_texture_container(id="mvTextureContainer")
dpg.add_static_texture(100, 100, [], parent="mvTextureContainer", file="INTERNAL_DPG_FONT_ATLAS", id="INTERNAL_DPG_FONT_ATLAS")

demo.show_demo()

smpl.start_dearpygui()