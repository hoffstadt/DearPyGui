from dearpygui.core import *
from dearpygui.simple import *

set_main_window_size(500, 500)

# callback
def retrieve_callback(sender, callback):

    show_logger()
    log_info(get_value("Regular##inputtext"))
    log_info(get_value("With hint##inputtext"))
    log_info(get_value("No spaces##inputtext"))
    log_info(get_value("Uppercase##inputtext"))
    log_info(get_value("Decimal##inputtext"))
    log_info(get_value("Hexadecimal##inputtext"))
    log_info(get_value("Read Only##inputtext"))
    log_info(get_value("Password##inputtext"))
    log_info(get_value("Multiline##inputtext"))

add_text("This example demonstrates the input text widget.", bullet=True)
add_text("Press the 'Retrieve' button to display the inputed values in the logger", wrap = 500, bullet=True)

add_input_text("Regular##inputtext")
add_input_text("With hint##inputtext", hint="A hint")
add_input_text("No spaces##inputtext", no_spaces=True)
add_input_text("Uppercase##inputtext", uppercase=True)
add_input_text("Decimal##inputtext", decimal=True)
add_input_text("Hexadecimal##inputtext", hexadecimal=True)
add_input_text("Read Only##inputtext", readonly=True, default_value="read only")
add_input_text("Password##inputtext", password=True)
add_input_text("Multiline##inputtext", multiline=True)

add_button("Retrieve", callback=retrieve_callback)

start_dearpygui()
