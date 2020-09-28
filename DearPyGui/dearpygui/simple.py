from contextlib import contextmanager
from typing import List, Any, Callable, Union
import dearpygui.core as internalDPG

########################################################################################################################
# context manager container wrappers
########################################################################################################################

@contextmanager
def managed_columns(name: str, columns: int, border: bool = True, show: bool = True, parent: str = "", before: str = ""):
    try:
        yield internalDPG.add_managed_columns(name, columns, border=border, show=show, parent=parent, before=before)

    finally: internalDPG.end()

@contextmanager
def window(name: str, width: int = 200, height: int = 200, x_pos: int = 200, y_pos: int = 200, autosize: bool = False, 
			   no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False, 
			   no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False, 
			   no_bring_to_front_on_focus: bool = False, menubar: bool = False, no_close: bool = False, no_background: bool = False,
               label: str = "__DearPyGuiDefault", show: bool = True, on_close: Callable = None):
    try:
        if label == "__DearPyGuiDefault":
            yield internalDPG.add_window(name, width=width, height=height, x_pos=x_pos, y_pos=y_pos, autosize=autosize, 
                         no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move, no_scrollbar=no_scrollbar, no_collapse=no_collapse,
                         horizontal_scrollbar=horizontal_scrollbar, no_focus_on_appearing=no_focus_on_appearing, 
                         no_bring_to_front_on_focus=no_bring_to_front_on_focus,
                         menubar=menubar, no_close=no_close, no_background=no_background, show=show, on_close=on_close)
        else:
            yield internalDPG.add_window(name, width=width, height=height, x_pos=x_pos, y_pos=y_pos, autosize=autosize, 
                         no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move, no_scrollbar=no_scrollbar, no_collapse=no_collapse,
                         horizontal_scrollbar=horizontal_scrollbar, no_focus_on_appearing=no_focus_on_appearing, no_bring_to_front_on_focus=no_bring_to_front_on_focus,
                         menubar=menubar, no_close=no_close, no_background=no_background, label=label, show=show, on_close=on_close)
    finally: internalDPG.end()

@contextmanager
def menu_bar(name: str, show: bool = True, parent: str = "", before: str = ""):
    try: yield internalDPG.add_menu_bar(name, show=show, parent=parent, before=before)
    finally: internalDPG.end()

@contextmanager
def menu(name: str, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "", parent: str = "", before: str = "", enabled: bool = True):
    try: 
        if label == "__DearPyGuiDefault":
            yield internalDPG.add_menu(name, show=show, tip=tip, parent=parent, before=before, enabled=enabled)
        else:
            yield internalDPG.add_menu(name, label=label, show=show, tip=tip, parent=parent, before=before, enabled=enabled)
    finally: internalDPG.end()

@contextmanager
def child(name: str, show: bool = True, tip: str = "", parent: str = "", before: str = "", width: int = 0, height: int = 0, border: bool = True, popup: str = ""):
    try: 
        yield internalDPG.add_child(name, show=show, tip=tip, parent=parent, before=before, width=width, 
                        height=height, border=border, popup=popup)
    finally: internalDPG.end()

@contextmanager
def collapsing_header(name: str, default_open: bool = False, closable: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "", parent: str = "", before: str = ""):
    try:
        if label == "__DearPyGuiDefault":
            yield internalDPG.add_collapsing_header(name, default_open=default_open, closable=closable, show=show, tip=tip, parent=parent, before=before)
        else:
            yield internalDPG.add_collapsing_header(name, default_open=default_open, closable=closable, label=label, show=show, tip=tip, parent=parent, before=before)
    finally: internalDPG.end()

@contextmanager
def group(name: str, show: bool = True, tip: str = "", parent: str = "", before: str = "", width: int = 0, 
			  horizontal: bool = False, horizontal_spacing: float = -1.0):
    try:
        yield internalDPG.add_group(name, show=show, tip=tip, parent=parent, before=before, width=width, horizontal=horizontal,
                        horizontal_spacing=horizontal_spacing)
    finally: internalDPG.end()

@contextmanager
def tab_bar(name: str, reorderable: bool = False, callback: str = "", callback_data: str = "",  show: bool = True, parent: str = "", before: str = ""):
    try:
        yield internalDPG.add_tab_bar(name, reorderable=reorderable, callback=callback, callback_data=callback_data, show=show, parent=parent, before=before)
    finally: internalDPG.end()

@contextmanager
def tab(name: str, closable: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "", parent: str = "", before: str = ""):
    try:
        if label == "__DearPyGuiDefault":
            yield internalDPG.add_tab(name, closable=closable, show=show, tip=tip, parent=parent, before=before)
        else:
            yield internalDPG.add_tab(name, closable=closable, label=label, show=show, tip=tip, parent=parent, before=before)
    finally: internalDPG.end()

@contextmanager
def tree_node(name: str, default_open: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "", parent: str = "", before: str = ""):
    try:
        if label == "__DearPyGuiDefault":
            yield internalDPG.add_tree_node(name, default_open=default_open, show=show, tip=tip, parent=parent, before=before)
        else:
            yield internalDPG.add_tree_node(name, default_open=default_open, label=label, show=show, tip=tip, parent=parent, before=before)
    finally: internalDPG.end()

@contextmanager
def tooltip(tipparent: str, name: str, parent: str = "", before: str = "", show: bool = True):
    try:
        yield internalDPG.add_tooltip(tipparent, name, parent=parent, before=before, show=show)
    finally: internalDPG.end()

@contextmanager
def popup(popupparent: str, name: str, mousebutton: int = 1, modal: bool = False, parent: str = "", 
          before: str = "", width: int = 0, height: int = 0, show: bool = True):
    try:
        yield internalDPG.add_popup(popupparent, name, mousebutton=mousebutton, modal=modal, parent=parent,
                        before=before, width=width, height=height, show=show)
    finally: internalDPG.end() 

########################################################################################################################
# Old Commands
########################################################################################################################

# window configure
def set_window_pos(window: str, x: int, y: int):
    internalDPG.configure_item(window, x_pos=x, y_pos=y)

def get_window_pos(window: str) -> Union[List[int], None]:
    config = internalDPG.get_item_configuration(window)
    return [config["x_pos"], config["y_pos"]]

def show_item(item: str):
    internalDPG.configure_item(item, show=True)

def hide_item(item: str, children_only: bool = False):
    if children_only:
        children = internalDPG.get_item_children(item)
        for child in children:
            internalDPG.configure_item(child, show=False)
    else:
        internalDPG.configure_item(item, show=False)


# item configure commands
def set_item_name(item: str, name: str):
    internalDPG.configure_item(item, name=name)

def set_item_label(item: str, label: str):
    internalDPG.configure_item(item, label=label)

def set_item_popup(item: str, popup: str):
    internalDPG.configure_item(item, popup=popup)

def set_item_tip(item: str, tip: str):
    internalDPG.configure_item(item, tip=tip)

def set_item_width(item: str, width: int):
    internalDPG.configure_item(item, width=width)

def set_item_height(item: str, height: int):
    internalDPG.configure_item(item, height=height)

def get_item_label(item: str) -> Union[str, None]:
    return internalDPG.get_item_configuration(item)["label"]

def get_item_popup(item: str) -> Union[str, None]:
    return internalDPG.get_item_configuration(item)["popup"]

def get_item_tip(item: str) -> Union[str, None]:
    return internalDPG.get_item_configuration(item)["tip"]

def get_item_width(item: str) -> Union[int, None]:
    return internalDPG.get_item_configuration(item)["width"]

def get_item_height(item: str) -> Union[int, None]:
    return internalDPG.get_item_configuration(item)["height"]

# drawing configure commands
def set_drawing_origin(drawing: str, x: float, y: float):
    internalDPG.configure_item(drawing, originx=x, originy=y)

def set_drawing_scale(drawing: str, x: float, y: float):
    internalDPG.configure_item(drawing, scalex=x, scaley=y)

def set_drawing_size(drawing: str, x: int, y: int):
    internalDPG.configure_item(drawing, width=x, height=y)

def get_drawing_origin(drawing: str) -> Union[List[float], None]:
    config = internalDPG.get_item_configuration(drawing)
    return [config["originx"], config["originy"]]

def get_drawing_scale(drawing: str) -> Union[List[float], None]:
    config = internalDPG.get_item_configuration(drawing)
    return [config["scalex"], config["scaley"]]

def get_drawing_size(drawing: str) -> Union[List[int], None]:
    config = internalDPG.get_item_configuration(drawing)
    return [config["width"], config["height"]]
