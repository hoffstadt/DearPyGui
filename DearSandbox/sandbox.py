import dearpygui.dearpygui as dpg
import dearpygui.demo as demo

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, tag="custom font"):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())

demo.show_demo()

with dpg.window(label="tutorial", width=500, height=500, show=False):
    dpg.add_button(label="Press me", callback=lambda:dpg.toggle_viewport_fullscreen())

# main loop
dpg.show_viewport()
while dpg.is_dearpygui_running():
    dpg.render_dearpygui_frame()  

dpg.destroy_context()
