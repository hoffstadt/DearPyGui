from contextlib import contextmanager
from functools import wraps
import dearpygui.dearpygui as gui

def wrap_container(container):
    @contextmanager
    @wraps(container)
    def container_context(*args,**kwargs):
        try: yield container(*args,**kwargs)
        finally: end()
    return container_context

window = wrap_container(gui.add_window)
menu_bar = wrap_container(gui.add_menu_bar)
menu = wrap_container(gui.add_menu)
child = wrap_container(gui.add_child)
collapsing_header = wrap_container(gui.add_collapsing_header)
group = wrap_container(gui.add_group)
tab_bar = wrap_container(gui.add_tab_bar)
tab = wrap_container(gui.add_tab)
tree_node = wrap_container(gui.add_tree_node)
tooltip = wrap_container(gui.add_tooltip)
popup = wrap_container(gui.add_popup)
