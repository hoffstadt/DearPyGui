import dearpygui.dearpygui as dpg
import warnings

warnings.simplefilter('always', DeprecationWarning)

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())

with dpg.window(label="tutorial", width=500, height=500):
    dpg.add_button(label="Test me")

dpg.create_viewport()
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()