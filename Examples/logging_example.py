from dearpygui.dearpygui import *

def log_callback(sender, data):
    show_logger()
    clear_log()
    loglevel = get_value("Log Level##logging")
    set_log_level(loglevel)
    print("Python Print")
    log("Trace Message")
    log_debug("Debug Message")
    log_info("Info Message")
    log_warning("Warning Message")
    log_error("Error Message")

add_button("Test Logger", callback=log_callback)
add_same_line(spacing=10)
add_group("LoggingGroup")
add_text("Log Level")
add_radio_button("Log Level##logging", ["Trace", "Debug", "Info", "Warning", "Error", "Off"])
end()

start_dearpygui()
