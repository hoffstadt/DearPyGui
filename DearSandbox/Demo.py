from dearpygui import *

show_logger()

add_checkbox("Radio Button", data_source="data1")
add_data("data1", True)
add_button("button", callback="print_me")


def print_me(sender, data):
    set_value("Radio Button", False)
    print(get_value("Radio Button"))
    print(get_data("data1"))


start_dearpygui()