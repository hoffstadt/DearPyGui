from contextlib import contextmanager
from typing import List, Any, Callable
import dearpygui.core as dpg

@contextmanager
def window(name: str, width: int = -1, height: int = -1, x_pos: int = 200, y_pos: int = 200, 
           autosize: bool = False, no_resize: bool = False, no_title_bar: bool = False, 
           no_move: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, on_close: Callable = None):
    try:
        if label == "__DearPyGuiDefault":
            yield dpg.add_window(name, width=width, height=height, x_pos=x_pos, y_pos=y_pos, autosize=autosize, 
                         no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move, show=show,
                         on_close=on_close)
        else:
            yield dpg.add_window(name, width=width, height=height, x_pos=x_pos, y_pos=y_pos, autosize=autosize, 
                         no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move, label=label, show=show,
                         on_close=on_close)
    finally: dpg.end()

@contextmanager
def menu_bar(name: str, show: bool = True, parent: str = "", before: str = ""):
    try: yield dpg.add_menu_bar(name, show=show, parent=parent, before=before)
    finally: dpg.end()

@contextmanager
def menu(name: str, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "", parent: str = "", before: str = "", enabled: bool = True):
    try: 
        if label == "__DearPyGuiDefault":
            yield dpg.add_menu(name, show=show, tip=tip, parent=parent, before=before, enabled=enabled)
        else:
            yield dpg.add_menu(name, label=label, show=show, tip=tip, parent=parent, before=before, enabled=enabled)
    finally: dpg.end()

@contextmanager
def child(name: str, show: bool = True, tip: str = "", parent: str = "", before: str = "", width: int = 0, height: int = 0, border: bool = True, popup: str = ""):
    try: 
        yield dpg.add_child(name, show=show, tip=tip, parent=parent, before=before, width=width, 
                        height=height, border=border, popup=popup)
    finally: dpg.end()

@contextmanager
def collapsing_header(name: str, default_open: bool = False, closable: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "", parent: str = "", before: str = ""):
    try:
        if label == "__DearPyGuiDefault":
            yield dpg.add_collapsing_header(name, default_open=default_open, closable=closable, label=label, show=show, tip=tip, parent=parent, before=before)
        else:
            yield dpg.add_collapsing_header(name, default_open=default_open, closable=closable, show=show, tip=tip, parent=parent, before=before)
    finally: dpg.end()

@contextmanager
def group(name: str, show: bool = True, tip: str = "", parent: str = "", before: str = "", width: int = 0, 
			  horizontal: bool = False, horizontal_spacing: float = -1.0):
    try:
        yield dpg.add_group(name, show=show, tip=tip, parent=parent, before=before, width=width, horizontal=horizontal,
                        horizontal_spacing=horizontal_spacing)
    finally: dpg.end()

@contextmanager
def tab_bar(name: str, reorderable: bool = False, callback: str = "", callback_data: str = "",  show: bool = True, parent: str = "", before: str = ""):
    try:
        yield dpg.add_tab_bar(name, reorderable=reorderable, callback=callback, callback_data=callback_data, show=show, parent=parent, before=before)
    finally: dpg.end()

@contextmanager
def tab(name: str, closable: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "", parent: str = "", before: str = ""):
    try:
        if label == "__DearPyGuiDefault":
            yield dpg.add_tab(name, closable=closable, label=label, show=show, tip=tip, parent=parent, before=before)
        else:
            yield dpg.add_tab(name, closable=closable, show=show, tip=tip, parent=parent, before=before)
    finally: dpg.end()

@contextmanager
def tree_node(name: str, default_open: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "", parent: str = "", before: str = ""):
    try:
        if label == "__DearPyGuiDefault":
            yield dpg.add_tree_node(name, default_open=default_open, label=label, show=show, tip=tip, parent=parent, before=before)
        else:
            yield dpg.add_tree_node(name, default_open=default_open, show=show, tip=tip, parent=parent, before=before)
    finally: dpg.end()

@contextmanager
def tooltip(tipparent: str, name: str, parent: str = "", before: str = "", show: bool = True):
    try:
        yield dpg.add_tooltip(tipparent, name, parent=parent, before=before, show=show)
    finally: dpg.end()

@contextmanager
def popup(popupparent: str, name: str, mousebutton: int = 1, modal: bool = False, parent: str = "", 
          before: str = "", width: int = 0, height: int = 0, show: bool = True):
    try:
        yield dpg.add_popup(popupparent, name, mousebutton=mousebutton, modal=modal, parent=parent,
                        before=before, width=width, height=height, show=show)
    finally: dpg.end() 

########################################################################################################################
# Old Commands
########################################################################################################################

def set_window_pos(window, x, y):
    dpg.configure_item(window, x_pos=x, y_pos=y)

def get_window_pos(window):
    config = dpg.get_item_configuration(window)
    return [config["x_pos"], config["y_pos"]]

def set_item_name(item, name):
    dpg.configure_item(item, name=name)

def set_item_label(item, label):
    dpg.configure_item(item, label=label)

def set_item_popup(item, popup):
    dpg.configure_item(item, popup=popup)

def set_item_tip(item, tip):
   dpg. configure_item(item, tip=tip)

def show_item(item):
    dpg.configure_item(item, show=True)

def get_item_label(item):
    return dpg.get_item_configuration(item)["label"]

def get_item_popup(item):
    return dpg.get_item_configuration(item)["popup"]

def get_item_tip(item):
    return dpg.get_item_configuration(item)["tip"]

def get_item_width(item):
    return dpg.get_item_configuration(item)["width"]

def get_item_height(item):
    return dpg.get_item_configuration(item)["height"]

def hide_item(item):
    dpg.configure_item(item, show=False)

def set_item_width(item, width):
    dpg.configure_item(item, width=width)

def set_item_height(item, height):
    dpg.configure_item(item, height=height)

def show_item(item):
    dpg.configure_item(item, show=True)

def set_drawing_origin(drawing, x, y):
    dpg.configure_item(drawing, originx=x, originy=y)

def set_drawing_scale(drawing, x, y):
    dpg.configure_item(drawing, scalex=x, scaley=y)

def set_drawing_size(drawing, x, y):
    dpg.configure_item(drawing, width=x, height=y)

def get_drawing_origin(drawing):
    config = dpg.get_item_configuration(drawing)
    return [config["originx"], config["originy"]]

def get_drawing_scale(drawing):
    config = dpg.get_item_configuration(drawing)
    return [config["scalex"], config["scaley"]]

def get_drawing_size(drawing):
    config = dpg.get_item_configuration(drawing)
    return [config["width"], config["height"]]
