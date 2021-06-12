import dearpygui.core as dpg
import dearpygui.simple as smpl
import dearpygui.contexts as cxt
import dearpygui.demo as demo

with cxt.font_registry():
    dpg.add_font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, default_font=True)

demo.show_demo()

smpl.start_dearpygui()