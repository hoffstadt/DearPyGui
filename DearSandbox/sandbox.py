import dearpygui.dearpygui as dpg
import dearpygui.demo as demo
import dearpygui.themes as themes

dpg.enable_docking()

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, default_font=True):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)

demo.show_demo()

with dpg.window(label="tutorial"):
    dpg.add_button(label="Press me")


dpg.setup_viewport()
dpg.start_dearpygui()
