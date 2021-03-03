from dearpygui.core import *
from dearpygui.simple import *

with window("example"):
    with managed_columns("column1", 3):
        for i in range(9):
            add_text(f"Text #1-{i}")

    # set_managed_column_width("column1", 1, 60)

    # prints [0.0, 0.0, 0.0]
    widths = [ get_managed_column_width("column1", i) for i in range(3) ]
    print(widths)

    def slider_callback(sender, data):
        set_managed_column_width("column1", 1, get_value("slider"))

    add_slider_float("slider", default_value=widths[1], callback=slider_callback)

    add_separator()

    with managed_columns("column2", 3):
        for i in range(9):
            add_text(f"Text #2-{i}")

    set_managed_column_width("column2", 1, 67)

def start_callback(sender, data):
    widths = [ get_managed_column_width("column1", i) for i in range(3) ]
    print(widths)

set_start_callback(start_callback)
show_logger()
show_debug()
start_dearpygui()