import dearpygui.core as dpg
import dearpygui.simple as smpl
import dearpygui.contexts as cxt
import dearpygui.demo as demo

dpg.add_font("google", "../../Resources/NotoSerifCJKjp-Medium.otf", 20)
dpg.set_font("google", 20)

main_texture_container = dpg.generate_uuid()

dpg.add_texture_container(id=main_texture_container)
dpg.add_static_texture(100, 100, [], parent=main_texture_container, file=dpg.mvFontAtlas, id=dpg.mvFontAtlas)

with cxt.window(label="New IDs"):

    dpg.add_button(label="Press me")

demo.show_demo()

smpl.start_dearpygui()