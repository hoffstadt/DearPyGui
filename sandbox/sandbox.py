import dearpygui.dearpygui as dpg
import dearpygui.demo as demo
from dearpygui_ext.logger import mvLogger

dpg.create_context()
#dpg.configure_app(manual_callback_management=True)
dpg.create_viewport()
dpg.setup_dearpygui()

log = mvLogger()
log.log("log")
log.log_debug("log debug")
log.log_info("log info")
log.log_warning("log warning")
log.log_error("log error")
log.log_critical("log critical")

with dpg.font_registry():
    with dpg.font("../../assets/NotoSerifCJKjp-Medium.otf", 20, tag="custom font"):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())

demo.show_demo()

# main loop
dpg.show_viewport()
while dpg.is_dearpygui_running():
    #dpg.run_callbacks(dpg.get_callback_queue())
    dpg.render_dearpygui_frame()  

dpg.destroy_context()