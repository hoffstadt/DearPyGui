import dearpygui.dearpygui as dpg
import dearpygui.demo as demo
import warnings

warnings.simplefilter('always', DeprecationWarning)

dpg.enable_docking()

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())

demo.show_demo()

with dpg.window(label="tutorial", width=500, height=500):
    dpg.add_button(label="Press me")

dpg.create_viewport()
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
