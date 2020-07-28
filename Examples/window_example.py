from dearpygui.dearpygui import *

add_input_text("Window Title##input", default_value="Window Title")
add_input_int("Width", default_value=400)
add_input_int("Height", default_value=500)
add_input_int("Start x", default_value=50)
add_input_int("Start y", default_value=50)
add_checkbox("Autosize")
add_checkbox("Resizable", default_value=True)
add_checkbox("Movable", default_value=True)
add_checkbox("Title bar", default_value=True)
add_button("Create Window", callback="window_creator")
add_button("Close Window", callback="close_window")
hide_item("Close Window")


def window_creator(sender, data):

    title = get_value("Window Title##input")
    width = get_value("Width")
    height = get_value("Height")
    start_x = get_value("Start x")
    start_y = get_value("Start y")
    autosize = get_value("Autosize")
    resizable = get_value("Resizable")
    movable = get_value("Movable")
    title_bar = get_value("Title bar")

    if not title_bar:
        show_item("Close Window")

    add_window(title, width, height, start_x=start_x, start_y=start_y,
               autosize=autosize, resizable=resizable, title_bar=title_bar,
               movable=movable, on_close="on_window_close")
    for i in range(0, 10):
        add_button("button" + str(i))
    end_window()

    hide_item("Create Window")


def on_window_close(sender, data):
    show_item("Create Window")
    delete_item(sender)


def close_window(sender, data):
    show_item("Create Window")
    hide_item("Close Window")
    delete_item(get_value("Window Title##input"))

start_dearpygui()
