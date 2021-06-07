from dearpygui.core import *
from dearpygui.simple import *
from dearpygui.contexts import *
from dearpygui.themes import *
from math import sin, cos
import random

########################################################################################################################
# Helpers
########################################################################################################################

def demo_help(message):
    """[INTERNAL DPG DEMO COMMAND]: This is a helper function that is intended to make the demo easier to read. not suggested for external use. 
    If this command is appearing in your code consider importing demo as demo to prevent namespace polution.
    
    Creates a hoverable question marker on the same line as the last created item. The message will be displayed as a popup.

    Args:
        message: text that will be displayed in the popup when the "(?)" is hovered.

    Returns:
        None"""
    add_same_line()
    helper = add_text(color=[150, 150, 150], default_value="(?)")
    with tooltip(parent=helper):
        add_text(default_value=message)

def demo_hsv_to_rgb(h: float, s: float, v: float) -> (float, float, float):
    """[INTERNAL DPG DEMO COMMAND]: This is a helper function that is intended to make the demo easier to read. not suggested for external use. 
    If this command is appearing in your code consider importing demo as demo to prevent namespace polution.

    converts hsv to rgb

    Args:
        h: hue.
        s: saturation 
        v: value

    Returns:
        rgb as a tuple of floats
    """
    if s == 0.0: return (v, v, v)
    i = int(h*6.) # XXX assume int() truncates!
    f = (h*6.)-i; p,q,t = v*(1.-s), v*(1.-s*f), v*(1.-s*(1.-f)); i%=6
    if i == 0: return (255*v, 255*t, 255*p)
    if i == 1: return (255*q, 255*v, 255*p)
    if i == 2: return (255*p, 255*v, 255*t)
    if i == 3: return (255*p, 255*q, 255*v)
    if i == 4: return (255*t, 255*p, 255*v)
    if i == 5: return (255*v, 255*p, 255*q)

def demo_recursive_disable(sender, siblings, restart_index=0):
    """[INTERNAL DPG DEMO COMMAND]: This is a helper function that is intended to make the demo easier to read. not suggested for external use. 
    If this command is appearing in your code consider importing demo as demo to prevent namespace polution.
    
    This command will toggle enable/disable for any eligible children of the parent container recursively down the widget tree.
    Args:
        sender: is the widgets name that called the command.
        siblings: children in the same container that this checkbox is in. 
        **restart_index: sets the position of the next item to check so the same item isnt checked multiple times.

    Returns:
        None"""
    for i in range(restart_index,len(siblings)-1, 1):
        if get_item_info(siblings[i])["container"]:
            siblings += get_item_info(siblings.pop(i))["children"]
            demo_recursive_disable(sender, siblings, i-1)
            return
    siblings.remove(sender)
    for item in siblings:
        for key,value in get_item_configuration(item).items():
            if key == 'enabled':
                configure_item(item, enabled = not value)

def demo_enable_disable():
    """[INTERNAL DPG DEMO COMMAND]: This is a helper function that is intended to make the demo easier to read. not suggested for external use. 
    If this command is appearing in your code consider importing demo as demo to prevent namespace polution.
    
    Creates a checkbox that will toggle enable/disable for any eligible children of the parent container recursively down the widget tree.
    Args:
        None

    Returns:
        None"""
    add_checkbox(label="Enable/Disable", default_value=True, callback=lambda sender: demo_recursive_disable(sender,  get_item_info(get_item_info(sender)["parent"])["children"]))
    demo_help('This will toggle the keyword "enable" for any sibling widgets that allow enabled & disabled')

def demo_config(sender, data):
    """[INTERNAL DPG DEMO COMMAND]: This is a helper function that is intended to make the demo easier to read. Not suggested for external use. 
    If this command is appearing in your code consider importing demo as demo to prevent namespace polution.

    Set the user_data as the widget or list of widgets to be modified. Then the label of the sender will be the keyword modified.

    Args:
        sender: is the widgets name that called the command.
        data: data that is populated by user_data. Item as string or items as list that the configuration will be applied to.

    Returns:
        None
    """
    widget_type = get_item_type(sender)
    log_debug(widget_type)
    items = data
    value = get_value(sender)

    if widget_type == "mvAppItemType::mvCheckbox":
        keyword = get_item_configuration(sender)["label"]
        log_debug(keyword)

    elif widget_type == "mvAppItemType::mvRadioButton":
        keyword = get_item_configuration(sender)["items"][value]

    if isinstance(data, list):
        for item in items:
            configure_item(item, **{keyword: value})
    else:
        configure_item(items, **{keyword: value})

def demo_log(sender):
    """[INTERNAL DPG DEMO COMMAND]: This is a helper function that is intended to make the demo easier to read. not suggested for external use. 
    If this command is appearing in your code consider importing demo as demo to prevent namespace polution.
    
    Logs the name and value of the widget that called the command.
    
    Args:
        sender: widgets name that called the command. 
        that will get configured. The second item in the tuple is the keyword that will be set.
    
    Returns:
        None
    """
    #log_debug(f"{sender} ran a callback its value is {get_value(sender)}")
    print(f"{sender} ran a callback its value is {get_value(sender)}")

def demo_main_callback():
    """[INTERNAL DPG DEMO COMMAND]: This is a helper function that is intended to make the demo easier to read. not suggested for external use. 
    If this command is appearing in your code consider importing demo as demo to prevent namespace polution.
    """
    set_value("Mouse Position##demo", str(get_mouse_pos()))
    set_value("Drawing Mouse Position##demo", str(get_drawing_mouse_pos()))
    set_value("Plot Mouse Position##demo", str(get_plot_mouse_pos()))

    # keys
    if is_key_down(mvKey_A):
        set_value("A key Down##demo", "True")
    else:
        set_value("A key Down##demo", "False")

    if is_key_pressed(mvKey_W):
        set_value("W key Pressed##demo", "True")
    else:
        set_value("W key Pressed##demo", "False")

    if is_key_released(mvKey_Q):
        set_value("Q key Released##demo", "True")
    else:
        set_value("Q key Released##demo", "False")

    # mouse dragging
    if is_mouse_button_dragging(mvMouseButton_Left, 1):
        set_value("Left Mouse Dragging##demo", "True")
    else:
        set_value("Left Mouse Dragging##demo", "False")

    if is_mouse_button_dragging(mvMouseButton_Right, 1):
        set_value("Right Mouse Dragging##demo", "True")
    else:
        set_value("Right Mouse Dragging##demo", "False")

    if is_mouse_button_dragging(mvMouseButton_Middle, 1):
        set_value("Middle Mouse Dragging##demo", "True")
    else:
        set_value("Middle Mouse Dragging##demo", "False")

    # mouse down
    if is_mouse_button_down(mvMouseButton_Left):
        set_value("Left Mouse Down##demo", "True")
    else:
        set_value("Left Mouse Down##demo", "False")

    if is_mouse_button_down(mvMouseButton_Right):
        set_value("Right Mouse Down##demo", "True")
    else:
        set_value("Right Mouse Down##demo", "False")

    if is_mouse_button_down(mvMouseButton_Middle):
        set_value("Middle Mouse Down##demo", "True")
    else:
        set_value("Middle Mouse Down##demo", "False")

    # mouse clicked
    if is_mouse_button_clicked(mvMouseButton_Left):
        set_value("Left Mouse Clicked##demo", "True")
    else:
        set_value("Left Mouse Clicked##demo", "False")

    if is_mouse_button_clicked(mvMouseButton_Right):
        set_value("Right Mouse Clicked##demo", "True")
    else:
        set_value("Right Mouse Clicked##demo", "False")

    if is_mouse_button_clicked(mvMouseButton_Middle):
        set_value("Middle Mouse Clicked##demo", "True")
    else:
        set_value("Middle Mouse Clicked##demo", "False")

    # mouse double clicked
    if is_mouse_button_double_clicked(mvMouseButton_Left):
        set_value("Left Mouse Double Clicked##demo", "True")
    else:
        set_value("Left Mouse Double Clicked##demo", "False")

    if is_mouse_button_double_clicked(mvMouseButton_Right):
        set_value("Right Mouse Double Clicked##demo", "True")
    else:
        set_value("Right Mouse Double Clicked##demo", "False")

    if is_mouse_button_double_clicked(mvMouseButton_Middle):
        set_value("Middle Mouse Double Clicked##demo", "True")
    else:
        set_value("Middle Mouse Double Clicked##demo", "False")

    # mouse released
    if is_mouse_button_released(mvMouseButton_Left):
        set_value("Left Mouse Released##demo", "True")
    else:
        set_value("Left Mouse Released##demo", "False")

    if is_mouse_button_released(mvMouseButton_Right):
        set_value("Right Mouse Released##demo", "True")
    else:
        set_value("Right Mouse Released##demo", "False")

    if is_mouse_button_released(mvMouseButton_Middle):
        set_value("Middle Mouse Released##demo", "True")
    else:
        set_value("Middle Mouse Released##demo", "False")

def demo_accelerator_callback(sender, data):

    mapping = {
        "P": mvKey_P,
        "Y": mvKey_Y,
        "T": mvKey_T,
        "SHIFT": mvKey_Shift,
        "CTRL": mvKey_Control,
        }

    def are_all_true(shortcut):
        keys = shortcut.split("+")
        for item in keys:
            if not is_key_down(mapping[item.upper()]):
                return False
        return True

    items = get_all_items()
    for item in items:
        # print(get_item_type(item))
        if get_item_type(item) == "mvAppItemType::MenuItem":
            shortcut = get_item_configuration(item)["shortcut"]
            if shortcut != None and shortcut != "":
                if are_all_true(shortcut):
                    get_item_callback(item)(item, None)

def on_demo_close(sender, data):

    delete_item("Dear PyGui Demo")
    if does_item_exist("Logging Widget On Window##demo"):
        delete_item("Logging Widget On Window##demo")
    set_mouse_down_callback(None)
    set_mouse_drag_callback(None, 10)
    set_mouse_move_callback(None)
    set_mouse_double_click_callback(None)
    set_mouse_click_callback(None)
    set_mouse_release_callback(None)
    set_mouse_wheel_callback(None)
    set_key_down_callback(None)
    set_key_press_callback(None)
    set_key_release_callback(None)
    set_accelerator_callback(None)

def show_demo():

    #set_accelerator_callback(demo_accelerator_callback)

    def demo_file_callback(sender, data):
        print(sender)
        print(data)

    with file_dialog(id="Demo File Dialog", show=False, callback=demo_file_callback):
        add_file_extension(".*", color=(255, 255, 255, 255))
        add_file_extension(".cpp", color=(255, 255, 0, 255))
        add_file_extension(".h", color=(255, 0, 255, 255))

    with window(id="Dear PyGui Demo", width=800, height=800, on_close=on_demo_close):

        with collapsing_header(id="Tooltips##demo"):
            widget = add_text("Hover me for a fancy tooltip")
            with tooltip(id="tool_tip##tooltips", parent=widget):
                add_simple_plot(id="Simpleplot##tooltips##demo", default_value=(0.3, 0.9, 2.5, 8.9), height = 80)

        with collapsing_header(id="Popups, Modal windows, & Dialogs##demo"):

            with tree_node(id="Popups##demo"):
                add_text("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.")
                add_button(id="Select..##popups##demo")
                add_same_line()
                add_text("<None>")
                demo_help("right click for popup, it is set to right mouse button, this can be changed")
                with popup(id="popup1", parent="Select..##popups##demo", ):
                    add_text("Aquariam")
                    add_separator()
                    add_selectable(id="Bream##demo", callback=lambda sender, data: set_value("<None>", sender))
                    add_selectable(id="Haddock##demo", callback=lambda sender, data: set_value("<None>", sender))
                    add_selectable(id="Mackerel##demo", callback=lambda sender, data: set_value("<None>", sender))
                    add_selectable(id="Pollock##demo", callback=lambda sender, data: set_value("<None>", sender))
                    add_selectable(id="Tilefish##demo", callback=lambda sender, data: set_value("<None>", sender))

            with tree_node(id="Modals##demo"):
                add_text("Modal windows are like popups but the user cannot close them by clicking outside.")
                add_button(id="Delete..##modals##demo")
                demo_help("right click for popup modal, it is set to right mouse button, this can be changed ")
                with popup(id="Delete?", parent="Delete..##modals##demo", modal=True):
                    add_text("All those beautiful files will be deleted.\nThis operation cannot be undone!")
                    add_separator()
                    add_checkbox(id="Don't ask me next time##demo")
                    add_button(id="OK##modal##demo", width=75, callback=lambda sender, data: close_popup("Delete?"))
                    add_same_line()
                    add_button(id="Cancel##modal##demo", width=75, callback=lambda sender, data: close_popup("Delete?"))

            with tree_node(id="File Selector##demo"):

                add_button(label="Show File Dialog", callback=lambda: show_item("Demo File Dialog"))

            with tree_node(id="Menus inside a regular window##demo"):
                add_text("Below we are testing adding menu items to a regular window. It's rather unusual but should work")
                add_separator()
                add_menu_item(id="Menu item##demotestingmenus", shortcut="CTRL+M")
                with menu(id="Menu inside a regular window##demo"):
                    add_menu_item(id="Disabled item##demotestingmenus", enabled=False)
                    add_menu_item(id="New##demotestingmenus")

