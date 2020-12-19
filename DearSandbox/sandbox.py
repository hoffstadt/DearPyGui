from dearpygui.core import *
from dearpygui.simple import *

def widget_editor(sender, data):
    delete_item("group1", children_only=True)
    log_debug(f"{get_value(sender)}")
    log_debug(does_item_exist("label1"))
    #add_label_text("label1", default_value=f"{get_value(sender)}", parent="group1")
    log_debug( get_value("label1"))
    log_debug( get_value("label2"))
with window("Main Window"):
    show_logger()
    add_slider_int("slider1", default_value=3, callback=widget_editor)
    with group("group1"):
        add_label_text("label1", default_value="here")
start_dearpygui(primary_window="Main Window")