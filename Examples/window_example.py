from dearpygui.core import *
from dearpygui.simple import *
show_logger()
def window_creator(sender, data):

    title = get_value("Window Title##input")
    width = get_value("Width")
    height = get_value("Height")
    start_x = get_value("Start x")
    start_y = get_value("Start y")
    autosize = get_value("No Autosize")
    resizable = get_value("No Resizable")
    movable = get_value("No Movable")
    title_bar = get_value("No Title bar")

    if title_bar:
        show_item("Close Window")

    with window(title, width, height, x_pos=start_x, y_pos=start_y,
               autosize=autosize, no_resize=resizable, no_title_bar=title_bar,
               no_move=movable, on_close=on_window_close):
        for i in range(0, 10):
            add_button("button" + str(i))

    hide_item("Create Window")


def on_window_close(sender, data):
    show_item("Create Window")
    delete_item(sender)


def close_window(sender, data):
    show_item("Create Window")
    hide_item("Close Window")
    delete_item(get_value("Window Title##input"))

add_input_text("Window Title##input", default_value="Window Title")
add_input_int("Width", default_value=400)
add_input_int("Height", default_value=500)
add_input_int("Start x", default_value=50)
add_input_int("Start y", default_value=50)
add_checkbox("No Autosize")
add_checkbox("No Resizable", default_value=True)
add_checkbox("No Movable", default_value=True)
add_checkbox("No Title bar")
add_button("Create Window", callback=window_creator)
add_button("Close Window", callback=close_window)
hide_item("Close Window")

start_dearpygui()
