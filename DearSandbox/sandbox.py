import dearpygui.dearpygui as dpg
import dearpygui.demo as demo

dpg.setup_registries()

with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, default_font=True):
    dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)

demo.show_demo()

dpg.use_init_file()
dpg.start_dearpygui()
