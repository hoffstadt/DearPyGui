from contextlib import contextmanager
from dearpygui.dearpygui import *

@contextmanager
def window(name: str, width: int = -1, height: int = -1, start_x: int = 200, start_y: int = 200, 
           autosize: bool = False, resizable: bool = True, title_bar: bool = True, 
           movable: bool = True, hide: bool = False, on_close: str = ""):
    try: 
        yield add_window(name, width, height, start_x=start_x, start_y=start_y, autosize=autosize, 
                         resizable=resizable, title_bar=title_bar, movable=movable, hide=hide,
                         on_close=on_close)
    finally: end()

@contextmanager
def menu_bar(name: str, parent: str = "", before: str = ""):
    try: yield add_menu_bar(name, parent=parent, before=before)
    finally: end()

@contextmanager
def menu(name: str, tip: str = "", parent: str = "", before: str = ""):
    try: yield add_menu(name, tip=tip, parent=parent, before=before)
    finally: end()

@contextmanager
def child(name: str, tip: str = "", parent: str = "", before: str = "", width: int = 0, height: int = 0, border: bool = True):
    try: 
        yield add_child(name, tip=tip, parent=parent, before=before, width=width, 
                        height=height, border=border)
    finally: end()

@contextmanager
def collapsing_header(name: str, default_open: bool = False, closable: bool = False, tip: str = "", parent: str = "", before: str = ""):
    try:
        yield add_collapsing_header(name, default_open=default_open, closable=closable, tip=tip, parent=parent, before=before)
    finally: end()

@contextmanager
def group(name: str, tip: str = "", parent: str = "", before: str = "", width: int = 0, hide: bool = False, 
			  horizontal: bool = False, horizontal_spacing: float = -1.0):
    try:
        yield add_group(name, tip=tip, parent=parent, before=before, width=width, hide=hide, horizontal=horizontal,
                        horizontal_spacing=horizontal_spacing)
    finally: end()

@contextmanager
def tab_bar(name: str, reorderable: bool = False, callback: str = "", parent: str = "", before: str = "", data_source: str = ""):
    try:
        yield add_tab_bar(name, reorderable=reorderable, callback=callback, parent=parent, before=before, data_source=data_source)
    finally: end()

@contextmanager
def tab(name: str, closable: bool = False, tip: str = "", parent: str = "", before: str = ""):
    try:
        yield add_tab(name, closable=closable, tip=tip, parent=parent, before=before)
    finally: end()

@contextmanager
def tree_node(name: str, default_open: bool = False, tip: str = "", parent: str = "", before: str = ""):
    try:
        yield add_tree_node(name, default_open=default_open, tip=tip, parent=parent, before=before)
    finally: end()

@contextmanager
def tooltip(tipparent: str, name: str, parent: str = "", before: str = ""):
    try:
        yield add_tooltip(tipparent, name, parent=parent, before=before)
    finally: end()

@contextmanager
def popup(popupparent: str, name: str, mousebutton: int = 1, modal: bool = False, parent: str = "", 
          before: str = "", width: int = 0, height: int = 0):
    try:
        yield add_popup(popupparent, name, mousebutton=mousebutton, modal=modal, parent=parent,
                        before=before, width=width, height=height)
    finally: end()