from contextlib import contextmanager
import dearpygui.core as gui

@contextmanager
def window(name: str, width: int = -1, height: int = -1, x_pos: int = 200, y_pos: int = 200, 
           autosize: bool = False, no_resize: bool = False, no_title_bar: bool = False, 
           no_move: bool = True, hide: bool = False, on_close: str = ""):
    try: 
        yield gui.add_window(name, width, height, x_pos=x_pos, y_pos=y_pos, autosize=autosize, 
                         no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move, hide=hide,
                         on_close=on_close)
    finally: gui.end()

@contextmanager
def menu_bar(name: str, parent: str = "", before: str = ""):
    try: yield gui.add_menu_bar(name, parent=parent, before=before)
    finally: gui.end()

@contextmanager
def menu(name: str, tip: str = "", parent: str = "", before: str = ""):
    try: yield gui.add_menu(name, tip=tip, parent=parent, before=before)
    finally: gui.end()

@contextmanager
def child(name: str, tip: str = "", parent: str = "", before: str = "", width: int = 0, height: int = 0, border: bool = True):
    try: 
        yield gui.add_child(name, tip=tip, parent=parent, before=before, width=width, 
                        height=height, border=border)
    finally: gui.end()

@contextmanager
def collapsing_header(name: str, default_open: bool = False, closable: bool = False, tip: str = "", parent: str = "", before: str = ""):
    try:
        yield gui.add_collapsing_header(name, default_open=default_open, closable=closable, tip=tip, parent=parent, before=before)
    finally: gui.end()

@contextmanager
def group(name: str, tip: str = "", parent: str = "", before: str = "", width: int = 0, hide: bool = False, 
			  horizontal: bool = False, horizontal_spacing: float = -1.0):
    try:
        yield gui.add_group(name, tip=tip, parent=parent, before=before, width=width, hide=hide, horizontal=horizontal,
                        horizontal_spacing=horizontal_spacing)
    finally: gui.end()

@contextmanager
def tab_bar(name: str, reorderable: bool = False, callback: str = "", parent: str = "", before: str = "", data_source: str = ""):
    try:
        yield gui.add_tab_bar(name, reorderable=reorderable, callback=callback, parent=parent, before=before, data_source=data_source)
    finally: gui.end()

@contextmanager
def tab(name: str, closable: bool = False, tip: str = "", parent: str = "", before: str = ""):
    try:
        yield gui.add_tab(name, closable=closable, tip=tip, parent=parent, before=before)
    finally: gui.end()

@contextmanager
def tree_node(name: str, default_open: bool = False, tip: str = "", parent: str = "", before: str = ""):
    try:
        yield gui.add_tree_node(name, default_open=default_open, tip=tip, parent=parent, before=before)
    finally: gui.end()

@contextmanager
def tooltip(tipparent: str, name: str, parent: str = "", before: str = ""):
    try:
        yield gui.add_tooltip(tipparent, name, parent=parent, before=before)
    finally: gui.end()

@contextmanager
def popup(popupparent: str, name: str, mousebutton: int = 1, modal: bool = False, parent: str = "", 
          before: str = "", width: int = 0, height: int = 0):
    try:
        yield gui.add_popup(popupparent, name, mousebutton=mousebutton, modal=modal, parent=parent,
                        before=before, width=width, height=height)
    finally: gui.end() 
