import dearpygui.core as dpg
import dearpygui.simple as smpl
import dearpygui.contexts as cxt
import dearpygui.demo as demo

with cxt.font_registry():
    
    with cxt.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, default_font=True):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
        #dpg.add_font_range_hint(dpg.mvFontRangeHint_Japanese)
        #dpg.add_font_range(0x3100, 0x3ff0)
        dpg.add_font_chars([0x3105, 0x3107, 0x3108])
        #dpg.add_char_remap(0x3105, 0x0025)

demo.show_demo()

smpl.start_dearpygui()