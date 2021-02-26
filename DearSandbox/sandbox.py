from dearpygui.core import *
from dearpygui.simple import *


def log_values(sender):
    caller_value = get_value(sender)
    log_info(message=f"{sender}: {caller_value}", logger="Example Logger")


def enable_widgets():
    log_warning("Enabling all widgets", logger="Example Logger")
    configure_item("slider_int_input_allow_clamped", enabled=True)
    configure_item("slider_int_no_input", enabled=True)
    configure_item("slider_int_no_input_start_disabled_clamped", enabled=True)


def disable_widgets():
    log_warning("Disabling all widgets", logger="Example Logger")
    configure_item("slider_int_input_allow_clamped", enabled=False)
    configure_item("slider_int_no_input", enabled=False)
    configure_item("slider_int_no_input_start_disabled_clamped", enabled=False)


with window("Example Window"):
    add_button("Enable all inputs", callback=enable_widgets)
    add_same_line()
    add_button("Disable all inputs", callback=disable_widgets)
    add_spacing()
    add_separator()
    add_spacing()
    add_text("All sliders below have a minimum clamped value of 0 and a max value of 15.")
    add_slider_int("slider_int_input_allow_clamped", min_value=0, max_value=15, clamped=True, callback=log_values)
    add_slider_int("slider_int_no_input", min_value=0, max_value=15, callback=log_values, no_input=True, clamped=True)
    add_slider_int("slider_int_no_input_start_disabled_clamped", min_value=0, max_value=15, callback=log_values,
                   no_input=True, enabled=False, clamped=True)
    add_logger(name="Example Logger", autosize_x=True, autosize_y=True)


start_dearpygui(primary_window="Example Window")