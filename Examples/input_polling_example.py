from dearpygui.dearpygui import *

add_text("Key Polling")
add_label_text("A key Down", "False", color=[0, 200, 255])
add_label_text("W key Pressed", "False", color=[0, 200, 255])
add_label_text("Q key Released", "False", color=[0, 200, 255])
add_spacing()
add_text("Mouse Polling")
add_label_text("Mouse Position", "(0,0)", color=[0, 200, 255])
add_label_text("Left Mouse Dragging", "False", color=[0, 200, 255])
add_label_text("Middle Mouse Dragging", "False", color=[0, 200, 255])
add_label_text("Right Mouse Dragging", "False", color=[0, 200, 255])
add_label_text("Left Mouse Clicked", "False", color=[0, 200, 255])
add_label_text("Middle Mouse Clicked", "False", color=[0, 200, 255])
add_label_text("Right Mouse Clicked", "False", color=[0, 200, 255])
add_label_text("Left Mouse Double Clicked", "False", color=[0, 200, 255])
add_label_text("Middle Mouse Double Clicked", "False", color=[0, 200, 255])
add_label_text("Right Mouse Double Clicked", "False", color=[0, 200, 255])
add_label_text("Left Mouse Down", "False", color=[0, 200, 255])
add_label_text("Middle Mouse Down", "False", color=[0, 200, 255])
add_label_text("Right Mouse Down", "False", color=[0, 200, 255])
add_label_text("Left Mouse Released", "False", color=[0, 200, 255])
add_label_text("Middle Mouse Released", "False", color=[0, 200, 255])
add_label_text("Right Mouse Released", "False", color=[0, 200, 255])

set_render_callback("main_callback", "MainWindow")


def main_callback(sender, data):

    set_value("Mouse Position", str(get_mouse_pos()))

    # keys
    if is_key_down(mvKey_A):
        set_value("A key Down", "True")
    else:
        set_value("A key Down", "False")

    if is_key_pressed(mvKey_W):
        set_value("W key Pressed", "True")
    else:
        set_value("W key Pressed", "False")

    if is_key_released(mvKey_Q):
        set_value("Q key Released", "True")
    else:
        set_value("Q key Released", "False")

    # mouse dragging
    if is_mouse_button_dragging(mvMouseButton_Left, 10):
        set_value("Left Mouse Dragging", "True")
    else:
        set_value("Left Mouse Dragging", "False")

    if is_mouse_button_dragging(mvMouseButton_Right, 10):
        set_value("Right Mouse Dragging", "True")
    else:
        set_value("Right Mouse Dragging", "False")

    if is_mouse_button_dragging(mvMouseButton_Middle, 10):
        set_value("Middle Mouse Dragging", "True")
    else:
        set_value("Middle Mouse Dragging", "False")

    # mouse down
    if is_mouse_button_down(mvMouseButton_Left):
        set_value("Left Mouse Down", "True")
    else:
        set_value("Left Mouse Down", "False")

    if is_mouse_button_down(mvMouseButton_Right):
        set_value("Right Mouse Down", "True")
    else:
        set_value("Right Mouse Down", "False")

    if is_mouse_button_down(mvMouseButton_Middle):
        set_value("Middle Mouse Down", "True")
    else:
        set_value("Middle Mouse Down", "False")

    # mouse clicked
    if is_mouse_button_clicked(mvMouseButton_Left):
        set_value("Left Mouse Clicked", "True")
    else:
        set_value("Left Mouse Clicked", "False")

    if is_mouse_button_clicked(mvMouseButton_Right):
        set_value("Right Mouse Clicked", "True")
    else:
        set_value("Right Mouse Clicked", "False")

    if is_mouse_button_clicked(mvMouseButton_Middle):
        set_value("Middle Mouse Clicked", "True")
    else:
        set_value("Middle Mouse Clicked", "False")

    # mouse double clicked
    if is_mouse_button_double_clicked(mvMouseButton_Left):
        set_value("Left Mouse Double Clicked", "True")
    else:
        set_value("Left Mouse Double Clicked", "False")

    if is_mouse_button_double_clicked(mvMouseButton_Right):
        set_value("Right Mouse Double Clicked", "True")
    else:
        set_value("Right Mouse Double Clicked", "False")

    if is_mouse_button_double_clicked(mvMouseButton_Middle):
        set_value("Middle Mouse Double Clicked", "True")
    else:
        set_value("Middle Mouse Double Clicked", "False")

    # mouse released
    if is_mouse_button_released(mvMouseButton_Left):
        set_value("Left Mouse Released", "True")
    else:
        set_value("Left Mouse Released", "False")

    if is_mouse_button_released(mvMouseButton_Right):
        set_value("Right Mouse Released", "True")
    else:
        set_value("Right Mouse Released", "False")

    if is_mouse_button_released(mvMouseButton_Middle):
        set_value("Middle Mouse Released", "True")
    else:
        set_value("Middle Mouse Released", "False")

start_dearpygui()
