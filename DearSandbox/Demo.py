from dearpygui import *
from contextlib import contextmanager
from functools import wraps
from math import sin, cos

########################################################################################################################
# Helpers
########################################################################################################################

def add_item_to_check(item):
    add_label_text(item + "##checklist", value="Not Checked", color=[255, 0, 0])


def item_checked(item):
    delete_item(item)
    add_label_text(item + "##checklist", value="Not Checked", color=[255, 0, 0], parent="CompleteChecklistGroup")

########################################################################################################################
# Context Managers : normally in dearpygui.wrappers
########################################################################################################################

def wrap_container(container):
    @contextmanager
    @wraps(container)
    def container_context(*args,**kwargs):
        try: yield container(*args,**kwargs)
        finally: end()
    return container_context

window = wrap_container(add_window)
menu_bar = wrap_container(add_menu_bar)
menu = wrap_container(add_menu)
child = wrap_container(add_child)
collapsing_header = wrap_container(add_collapsing_header)
group = wrap_container(add_group)
tab_bar = wrap_container(add_tab_bar)
tab = wrap_container(add_tab)
tree_node = wrap_container(add_tree_node)
tooltip = wrap_container(add_tooltip)
popup = wrap_container(add_popup)


def recursively_show(container):
    for item in get_item_children(container):
        if get_item_children(item):
            show_item(item)
            recursively_show(item)
        else:
            show_item(item)

def try_login(sender, data):
    print(sender)
    print(data)
    if get_value('Password') == 'password':
        delete_item('Login Window')
        recursively_show('MainWindow')
    else:
        show_item('Incorrect Password.')

def position_login_window(sender, data):
    main_width = get_item_width('MainWindow')
    main_height = get_item_height('MainWindow')
    login_width = get_item_width('Login Window')
    login_height = get_item_height('Login Window')
    set_window_pos('Login Window', (main_width/2 - login_width/2), (main_height/2 - login_height/2))

with window('Login Window', title_bar=False, movable=False, autosize=True, resizable=False):
    add_input_text('Password', hint='hover me for password', password=True, tip='the password is "password"')
    add_button('Login', callback=try_login)
    add_text('Incorrect Password.', color=[255, 0, 0], parent='Login Window')
    hide_item('Incorrect Password.')
    set_render_callback(position_login_window, 'Login Window')

with menu_bar('menu bar'):
    with menu('menu 1'):
        add_menu_item('menu item')
    with menu('menu 2'):
        pass

add_text('Congrats!, you may now use the app')
add_button('Button 1')
hide_item('MainWindow', children_only=True)
start_dearpygui()