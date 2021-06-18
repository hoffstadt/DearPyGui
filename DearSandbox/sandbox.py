import dearpygui.core as dpg
import dearpygui.simple as smpl
import dearpygui.contexts as cxt
import dearpygui.demo as demo

with cxt.font_registry():
    
    with cxt.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, default_font=True):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)

demo.show_demo()

smpl.start_dearpygui()
