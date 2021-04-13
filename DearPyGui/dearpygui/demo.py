from dearpygui.core import *
from dearpygui.simple import *
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
    parent = add_text(color=[150, 150, 150], default_value="(?)")
    add_tooltip(parent=parent)
    add_text(default_value=message)
    end()

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
        if is_item_container(siblings[i]):
            siblings += get_item_children(siblings.pop(i))
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
    add_checkbox(label="Enable/Disable", default_value=True, callback=lambda sender: demo_recursive_disable(sender, get_item_children(get_item_parent(sender))))
    demo_help('This will toggle the keyword "enable" for any sibling widgets that allow enabled & disabled')

def demo_config(sender, data):
    """[INTERNAL DPG DEMO COMMAND]: This is a helper function that is intended to make the demo easier to read. not suggested for external use. 
    If this command is appearing in your code consider importing demo as demo to prevent namespace polution.

    Applies configuration of the data in callback_data.

    Args:
        sender: is the widgets name that called the command.
        data: data that is populated by callback_data. Item as string or items as list that the configuration will be applied to.

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
        configure_item(item, **{keyword: value})

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
    log_debug(f"{sender} ran a callback its value is {get_value(sender)}")

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

    with window("Dear PyGui Demo", x_pos=100, y_pos=100, width=800, height=800, on_close=on_demo_close):

        with menu_bar():

            with menu(label="Menu"):
                add_menu_item(label="New")
                add_menu_item(label="Open")

                with menu(label="Open Recent"):
                    add_menu_item(label="harrell.c")
                    add_menu_item(label="patty.h")
                    add_menu_item(label="nick.py")
                add_menu_item(label="Save")
                add_menu_item(label="Save As..")
                add_separator()

                with menu(label="Settings"):
                    demo_enable_disable()
                    add_menu_item(label="Option 1", callback=demo_log)
                    add_menu_item(label="Option 2", check=True, callback=demo_log)
                    add_menu_item(label="Option 3", check=True, callback=demo_log, default_value=True)
                    with child(height=60, autosize_x=True):
                        for i in range(0, 10):
                            add_text(default_value=f"Scrolling Text {i}")
                    add_slider_float(label="Slider Float")
                    add_input_float(label="Input Int")
                    add_combo(["Yes", "No", "Maybe"], label="Combo")

            with menu(label="Tools"):
                add_menu_item(label="Show Logger", callback=show_logger)
                add_menu_item(label="Show About", callback=show_about)
                add_menu_item(label="Show Metrics", callback=show_metrics)
                add_menu_item(label="Show Documentation", callback=show_documentation)
                add_menu_item(label="Show Debug", callback=show_debug)
                add_menu_item(label="Show Style Editor", callback=show_style_editor)

        add_text(default_value=f'Dear PyGui says hello. ({get_dearpygui_version()})')
        add_text(default_value="This code for this demo can be found here: ")
        add_text(default_value="https://github.com/hoffstadt/DearPyGui/blob/master/DearPyGui/dearpygui/demo.py")

        with collapsing_header(label="Window options"):
            with table(header_row=False):
                add_table_column()
                add_table_column()
                add_table_column()
                
                # TODO: when autogenerated names are done and labels are set the label is correctly displayed however when the label is retrieved out of the config dict 
                # the label is the autogenerated name
                add_checkbox(label="no_title_bar", callback=demo_config, callback_data="Dear PyGui Demo")
                add_checkbox(label="no_scrollbar", callback=demo_config, callback_data="Dear PyGui Demo")
                add_checkbox(label="menubar", callback=demo_config, callback_data="Dear PyGui Demo", default_value=True)

                add_table_next_column()
                add_checkbox(label="no_move", callback=demo_config, callback_data="Dear PyGui Demo")
                add_checkbox(label="no_resize", callback=demo_config, callback_data="Dear PyGui Demo")
                add_checkbox(label="no_collapse", callback=demo_config, callback_data="Dear PyGui Demo")
            
                add_table_next_column()
                add_checkbox(label="no_close", callback=demo_config, callback_data="Dear PyGui Demo")
                add_checkbox(label="no_background", callback=demo_config, callback_data="Dear PyGui Demo")
                add_checkbox(label="no_bring_to_front_on_focus", callback=demo_config, callback_data="Dear PyGui Demo")
        
        with collapsing_header(label="Widgets"):

            with tree_node(label="Basic"):

                demo_enable_disable()

                with group(horizontal=True):
                    add_button(label="Button", callback=demo_log)
                    add_button(label="Button", callback=demo_log, small=True)
                    add_button(label="Button", callback=demo_log, arrow=True)
                    add_button(label="Button", callback=demo_log, arrow=True, direction=4010)
                    add_button(label="Button", callback=demo_log, arrow=True, direction=4020)
                    add_button(label="Button", callback=demo_log, arrow=True, direction=4030)
                
                add_checkbox(label="checkbox", callback=demo_log)
                add_radio_button(["radio a", "radio b", "radio c"], callback=demo_log, horizontal=True)
                add_selectable(label="selectable", callback=demo_log)

                # TODO: when items whose colors are set then disabled the disable color is not found so its clear
                # probably because if you dont set disable at the item level when the searcher finds color it grabs an empty disable member

                # demonstrate the ability to programmatically create buttons and store their id to do something. 
                # In this case we are adding to the list of items that will be disabled
                log_items = []
                for i in range(0, 7):
                    if i > 0:
                        add_same_line()
                    button_widget = add_button(label="Click", callback=lambda: log_debug(log_items))
                    set_theme_style(mvThemeStyle_Button_Rounding, i*5, item=button_widget)
                    set_theme_style(mvThemeStyle_Button_PaddingX, i*3, item=button_widget)
                    set_theme_style(mvThemeStyle_Button_PaddingY, i*3, item=button_widget)
                    set_theme_color(mvThemeCol_Button_Bg, demo_hsv_to_rgb(i/7.0, 0.6, 0.6), item=button_widget)
                    set_theme_color(mvThemeCol_Button_Active, demo_hsv_to_rgb(i/7.0, 0.8, 0.8), item=button_widget)
                    set_theme_color(mvThemeCol_Button_Hovered, demo_hsv_to_rgb(i/7.0, 0.7, 0.7), item=button_widget)
                    log_items.append(button_widget)

                with group(horizontal=True):
                    add_text(default_value="Press a button: ")
                    add_button(arrow=True, direction=mvDir_Left, callback=lambda: set_value(widget, int(get_value(widget))-1))
                    add_button(arrow=True, direction=mvDir_Right, callback=lambda: set_value(widget, int(get_value(widget))+1))
                    widget = add_text(default_value="0")

                with group(horizontal=True):
                    parent = add_text(default_value="hover me")
                    with tooltip(parent=parent):
                        add_text(default_value="I'm a tooltip")
                    add_dummy(width=10)
                    parent = add_text(default_value="or me")
                    with tooltip(parent=parent):
                        add_text(default_value="I'm a fancy tooltip")
                        add_simple_plot(label="Curve",value=[0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2])

                add_separator()

                add_label_text(label="Label", default_value="Value")
                add_combo(["AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK"], label="combo", default_value="AAAA", callback=demo_log)
                add_input_text(label="input text", default_value="Hello, world!", callback=demo_log)
                demo_help(
                        "USER:\n"
                        "Hold SHIFT or use mouse to select text.\n"
                        "CTRL+Left/Right to word jump.\n"
                        "CTRL+A or double-click to select all.\n"
                        "CTRL+X,CTRL+C,CTRL+V clipboard.\n"
                        "CTRL+Z,CTRL+Y undo/redo.\n"
                        "ESCAPE to revert.\n\n")
                add_input_text(label="input text (w/ hint)", hint="enter text here", callback=demo_log)
                add_input_int(label="input int", callback=demo_log)
                add_input_float(label="input float", callback=demo_log)
                add_input_float(label="input scientific", format="%e", callback=demo_log)
                add_input_floatx(label="input floatx", callback=demo_log)
                add_drag_int(label="drag int", callback=demo_log)
                demo_help(
                        "Click and drag to edit value.\n"
                        "Hold SHIFT/ALT for faster/slower edit.\n"
                        "Double-click or CTRL+click to input value.")
                add_drag_int(label="drag int 0..100", format="%d%%", callback=demo_log)
                add_drag_float(label="drag float", callback=demo_log)
                add_drag_float(label="drag small float", default_value=0.0067, format="%.06f ns", callback=demo_log)
                add_slider_int(label="slider int", max_value=3, callback=demo_log)
                demo_help("CTRL+click to enter value.")
                add_slider_float(label="slider float", max_value=1.0, format="ratio = %.3f", callback=demo_log)
                add_slider_int(label="slider angle", min_value=-360, max_value=360, format="%d deg", callback=demo_log)
                add_color_edit(label="color edit 3", default_value=[255, 0, 51], callback=demo_log)
                demo_help(
                        "Click on the colored square to open a color picker.\n"
                        "Click and hold to use drag and drop.\n"
                        "Right-click on the colored square to show options.\n"
                        "CTRL+click on individual component to input value.\n")
                add_color_edit([102, 179, 0, 128], label="color edit 4", callback=demo_log)
                add_listbox(["Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"], label="listbox", num_items=4, callback=demo_log)

            with tree_node(label="Trees"):
                with tree_node(label="Basic Trees"):
                    with tree_node(label="Child 0", default_open=True):
                        add_text(default_value="blah blah")
                        add_same_line()
                        add_button(label="button")
                    with tree_node(label="Child 1"):
                        add_text(default_value="blah blah")
                        add_same_line()
                        add_button(label="button")
                    with tree_node(label="Child 2"):
                        add_text(default_value="blah blah")
                        add_same_line()
                        add_button(label="button")
                    with tree_node(label="Child 3"):
                        add_text(default_value="blah blah")
                        add_same_line()
                        add_button(label="button")
                    with tree_node(label="Child 4"):
                        add_text(default_value="blah blah")
                        add_same_line()
                        add_button(label="button")
                with tree_node(label="Advanced, with selectable nodes"):
                    demo_help("This is a more typical looking tree with selectable nodes.\n"
                               "Click to select, CTRL+Click to toggle, click on arrows or double-click to open.")

            with tree_node(label="Bullets"):
                add_text(default_value="Bullet point 1", bullet=True)
                add_text(default_value="Bullet point 2\nbullet text can be\nOn multiple lines", bullet=True)
                with tree_node(label="Tree node"):
                    add_text(default_value="Another bullet point", bullet=True)
                # TODO: discuss the below behavior. intended was to set the label to an empty string but 
                # because label is checking if empty or not to use name we get the auto name showing
                add_text(default_value="1", bullet=True)
                add_same_line()
                add_button(label="Button", small=True)

            with tree_node(label="Text"):

                with tree_node(label="Colored Text"):
                    add_text(default_value="Pink", color=[255, 0, 255])
                    add_text(default_value="Yellow", color=[255, 255, 0])
    
            with tree_node(label="Images"):
                add_text(default_value="Below we are displaying the font texture (which is the only texture we have access to in this demo).")
                add_image("INTERNAL_DPG_FONT_ATLAS")
                add_text(default_value="Here is an image button using a portion of the font atlas")
                demo_enable_disable()
                add_image_button("INTERNAL_DPG_FONT_ATLAS",uv_max=[0.1, 0.1], callback=demo_log)
                add_same_line()
                textdata = []
                for i in range(0, 10000):
                    textdata.append(255)
                    textdata.append(0)
                    textdata.append(255)
                    textdata.append(255)
                # TODO: texture requires a name when it would be nice that it didnt
                add_texture("#cooltexture", textdata, 100, 100, format=mvTEX_RGBA_INT)
                add_image_button("#cooltexture", callback=demo_log)

            with tree_node(label="Text Input"):
                demo_enable_disable()
                add_checkbox(label="readonly", callback=demo_config, callback_data="Multi-line Text Input")
                add_checkbox(label="on_enter", callback=demo_config, callback_data="Multi-line Text Input")
                with tree_node("Multi-line Text Input"):
                    add_input_text(multiline=True, default_value="/*\n"
                        " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
                        " the hexadecimal encoding of one offending instruction,\n"
                        " more formally, the invalid operand with locked CMPXCHG8B\n"
                        " instruction bug, is a design flaw in the majority of\n"
                        " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
                        " processors (all in the P5 microarchitecture).\n"
                        "*/\n\n"
                        "label:\n"
                        "\tlock cmpxchg8b eax\n", height=300, callback=demo_log, tab_input=True)

                with tree_node(label="Filtered Text Input"):
                    add_input_text(callback=demo_log, label="default")
                    add_input_text(callback=demo_log, label="decimal", decimal=True)
                    add_input_text(callback=demo_log, label="no blank", no_spaces=True)
                    add_input_text(callback=demo_log, label="uppercase", uppercase=True)
                    add_input_text(callback=demo_log, label="scientific", scientific=True)
                    add_input_text(callback=demo_log, label="hexdecimal", hexadecimal=True)
            
                with tree_node(label="Password Input"):
                    password = add_input_text(label="password", password=True, callback=demo_log)
                    add_input_text(label="password (w/ hint)", password=True, hint="<password>", source=password, callback=demo_log)
                    add_input_text(label="password (clear)", source=password, callback=demo_log)

            with tree_node(label="Simple Plot Widgets"):
                add_simple_plot(label="Frame Times", value=[0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2])
                add_simple_plot(label="Histogram", value=(0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2), height=80, histogram=True, minscale=0.0)

                data1 = []
                for i in range(0, 70):
                    data1.append(cos(3.14*6*i/180))
                add_simple_plot(label="Lines", value=data1, height=80)
                add_simple_plot(label="Histogram", value=data1, height=80, histogram=True)
                add_progress_bar(label="Progress Bar", default_value=0.78, overlay="78%")
                add_same_line()
                add_text(default_value="Progress Bar")
                bar = add_progress_bar(default_value=0.78, overlay="1367/1753")
                set_theme_color(mvThemeCol_ProgressBar_Bar, [255,0,0, 255], item=bar )
           
            with tree_node(label="Multi-component Widgets"):
                demo_enable_disable()
                input_float_2 = add_input_floatx(label="input float 2", min_value=0.0, max_value=100.0)
                add_drag_floatx(label="drag float 2", source=input_float_2)
                add_slider_floatx(label="slider float 2", source=input_float_2)
                input_int_2 = add_input_intx(label="input int 2", min_value=0, max_value=100)
                add_drag_intx(label="drag int 2", source=input_int_2)
                add_slider_intx(label="slider int 2", source=input_int_2)
                add_spacing()
                input_float_3 = add_input_floatx(label="input float 3", min_value=0.0, max_value=100.0)
                add_drag_floatx(label="drag float 3", source=input_float_3)
                add_slider_floatx(label="slider float 3", source=input_float_3)
                input_int_3 = add_input_intx(label="input intx", min_value=0, max_value=100)
                add_drag_intx(label="drag int 3", source=input_int_3)
                add_slider_intx(label="slider int 3", source=input_int_3)
                add_spacing()
                input_float_4 = add_input_floatx(label="input float 4", min_value=0.0, max_value=100.0)
                add_drag_floatx(label="drag float 4", source=input_float_4)
                add_slider_floatx(label="slider float 4", source=input_float_4)
                input_int_4 = add_input_intx(label="input int 4", min_value=0, max_value=100)
                add_drag_intx(label="drag int 4", source=input_int_4)
                add_slider_intx(label="slider intx", source=input_int_4)

            with tree_node(label="Vertical Sliders"):
                demo_enable_disable()
                add_slider_int(label="", default_value=1, vertical=True, max_value=5, height=160)
                add_same_line()

                with group():
                    values = [ 0.0, 0.60, 0.35, 0.9, 0.70, 0.20, 0.0 ]
                    for i in range(0, 7):
                        if i > 0:
                            add_same_line()
                        widget = add_slider_float(label="", default_value=values[i], vertical=True, max_value=1.0, height=160)
                        set_theme_color(mvThemeCol_SliderFloat_Bg, demo_hsv_to_rgb(i/7.0, 0.5, 0.5), item=widget)
                        set_theme_color(mvThemeCol_SliderFloat_Grab, demo_hsv_to_rgb(i/7.0, 0.9, 0.9), item=widget)
                        set_theme_color(mvThemeCol_SliderFloat_BgActive, demo_hsv_to_rgb(i/7.0, 0.7, 0.5), item=widget)
                        set_theme_color(mvThemeCol_SliderFloat_BgHovered, demo_hsv_to_rgb(i/7.0, 0.6, 0.5), item=widget)

                # TODO: Find out why the final slider is throwing a error for duplicate name when the name should be autogenerated
                add_same_line()
                with group():
                    for i in range(0, 3):
                        with group():
                            values = [ 0.20, 0.80, 0.40, 0.25 ]
                            for j in range(0, 4):
                                add_slider_float(label="", default_value=values[j], vertical=True, max_value=1.0, height=50)
                                if i != 3:
                                    add_same_line()   
                # TODO: Should the label default just be an empty string when the item isnt given a name or a label, having to evter label="" is annoying, when you dont want a label to show
                add_same_line()
                with group(horizontal=True):
                    add_slider_float(label="", vertical=True, max_value=1.0, height=160, width=40)
                    add_slider_float(label="", vertical=True, max_value=1.0, height=160, width=40)
                    add_slider_float(label="", vertical=True, max_value=1.0, height=160, width=40)
                    add_slider_float(label="", vertical=True, max_value=1.0, height=160, width=40)

        with collapsing_header("Layout & Scolling##demo"):

            with tree_node("Child windows##demo"):

                with child(border=False, width=400):
                    for i in range(0, 100):
                        add_text(f"{i}: scrollable region")

                add_same_line()
                with child(autosize_x=True):
                    with menu_bar():
                        with menu("Menu##child##demo"):
                            pass
                    with group(width=-20):
                        for i in range(0, 100):
                            add_button(f"{i}##childbutton##demo")

            with tree_node("Child Window Flags##demo"):

                with table(header_row=False):
                    add_table_column()
                    add_table_column()
                    add_table_column()

                    add_checkbox("autosize_x##demo", callback=lambda sender, data: configure_item("testchild##demo", autosize_x=get_value(sender)))
                    add_checkbox("autosize_y##demo", callback=lambda sender, data: configure_item("testchild##demo", autosize_y=get_value(sender)))

                    add_table_next_column()
                    add_checkbox("menubar##childdemo", default_value=True, callback=lambda sender, data: configure_item("testchild##demo", menubar=get_value(sender)))
                    add_checkbox("no_scrollbar##childdemo", callback=lambda sender, data: configure_item("testchild##demo", no_scrollbar=get_value(sender)))

                    add_table_next_column()
                    add_checkbox("horizontal_scrollbar##childdemo", callback=lambda sender, data: configure_item("testchild##demo", horizontal_scrollbar=get_value(sender)))
                    add_checkbox("border##childdemo", default_value=True, callback=lambda sender, data: configure_item("testchild##demo", border=get_value(sender)))

                with child(width=500, height=500):
                    with menu_bar():
                        with menu("Menu##testchild##demo"):
                            pass
                    for i in range(0, 100):
                        add_text(default_value="A pretty long sentence if you really think about it. It's also pointless.")

            with tree_node("Widgets Width##demo"):
                
                add_text("Width=100")
                add_drag_float("float##demowidths1", width=100)

                add_text("Width=-100")
                add_drag_float("float##demowidths2", width=-100)

                add_text("Width=-1")
                add_drag_float("float##demowidths3", width=-1)

                add_text("group with width=75")
                with group(width=75):
                    add_drag_float("float##demowidths4")
                    add_drag_float("float##demowidths5")
                    add_drag_float("float##demowidths6")

            with tree_node("Basic Horizontal Layout##demo"):
                add_text("(Use add_same_line(), to keep adding items to the right of the preceding item)")
                add_text("Normal buttons")
                add_same_line()
                add_button("Banana##demo")
                add_same_line()
                add_button("Apple##demo")
                add_same_line()
                add_button("Corniflower##demo")

                add_text("Small buttons")
                add_same_line()
                add_button("Like this one##demo", small=True)
                add_same_line()
                add_text("can fit within a text block")

                add_text("Aligned")
                add_same_line(xoffset=150)
                add_text("x=150")
                add_same_line(xoffset=300)
                add_text("x=300")

                add_text(default_value="Aligned")
                add_same_line(xoffset=150)
                add_button("x=150##demo1", small=True)
                add_same_line(xoffset=300)
                add_button("x=300##demo1", small=True)

                add_checkbox("My##demo")
                add_same_line()
                add_checkbox("Tailor##demo")
                add_same_line()
                add_checkbox("is##demo")
                add_same_line()
                add_checkbox("rich##demo")

                add_text("Lists:")
                add_listbox("##demolistbox1", items=["AAAA", "BBBB", "CCCC", "DDDD"], default_value=0, width=100)
                add_same_line()
                add_listbox("##demolistbox2", items=["AAAA", "BBBB", "CCCC", "DDDD"], default_value=1, width=100)
                add_same_line()
                add_listbox("##demolistbox3", items=["AAAA", "BBBB", "CCCC", "DDDD"], default_value=2, width=100)
                add_same_line()
                add_listbox("##demolistbox4", items=["AAAA", "BBBB", "CCCC", "DDDD"], default_value=3, width=100)
                

                add_text("Spacing(100):")
                add_button("A##demospacing", width=50, height=50)
                add_same_line(spacing=100)
                add_button("B##demospacing", width=50, height=50)

            with tree_node("Tabs##demo"):
                with tree_node("Basic##tabs##demo"):
                    with tab_bar():
                        with tab("Avocado##demo"):
                            add_text("This is the avocado tab!")
                        with tab("Broccoli##demo"):
                            add_text("This is the broccoli tab!")
                        with tab("Cucumber##demo"):
                            add_text("This is the cucumber tab!")
                with tree_node("Advanced##tabs##demo"):
                    add_checkbox("tab bar reorderable##demo", callback=lambda sender, data: configure_item("Basic Tabbar1##demo", reorderable=get_value(sender)))
                    add_checkbox("tab 2 no_reorder##demo", callback=lambda sender, data: configure_item("tab 2##demo", no_reorder=get_value(sender)))
                    add_checkbox("tab 2 leading##demo", callback=lambda sender, data: configure_item("tab 2##demo", leading=get_value(sender)))
                    add_checkbox("tab 2 trailing##demo", callback=lambda sender, data: configure_item("tab 2##demo", trailing=get_value(sender)))
                    add_checkbox("tab button trailing##demo", callback=lambda sender, data: configure_item("+##demo", trailing=get_value(sender)))
                    add_checkbox("tab button leading##demo", callback=lambda sender, data: configure_item("+##demo", leading=get_value(sender)))
                    with tab_bar("Basic Tabbar1##demo"):
                        with tab("tab 1##demo"):
                            add_text("This is the tab 1!")
                        with tab("tab 2##demo"):
                            add_text("This is the tab 2!")
                        with tab("tab 3##demo"):
                            add_text("This is the tab 3!")
                        with tab("tab 4##demo"):
                            add_text("This is the tab 4!")
                        add_tab_button("+##demo", callback=lambda sender, data: log_debug("Pressed tab button"))
                        add_tab_button("?##demo", callback=lambda sender, data: log_debug("Pressed tab button"))

            with tree_node("Groups##demo123"):
                add_text("Groups can be used to bundle widths together so that you can use functions such as is_item_hovered or add_same_line on the whole group.")
                with group("group##demotabexamples"):
                    add_button("AAA##demogroup")
                    add_button("BBB##demogroup")
                    add_button("CCC##demogroup")
                with tooltip(parent="group##demotabexamples"):
                    add_text("The group is hovered", wrap=200)

                add_text("Horizontal group:")
                with group(horizontal=True):
                    add_button("AAA##demogroup1")
                    add_button("BBB##demogroup1")
                    add_button("CCC##demogroup1")

        with collapsing_header("Tooltips##demo"):
            add_text("Hover me for a fancy tooltip")
            with tooltip("tool_tip##tooltips", parent="Hover me for a fancy tooltip"):
                add_simple_plot("Simpleplot##tooltips##demo", value=(0.3, 0.9, 2.5, 8.9), height = 80)

        with collapsing_header("Popups, Modal windows, & Dialogs##demo"):

            with tree_node("Popups##demo"):
                add_text("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.")
                add_button("Select..##popups##demo")
                add_same_line()
                add_text("<None>")
                demo_help("right click for popup, it is set to right mouse button, this can be changed")
                with popup("popup1", parent="Select..##popups##demo", ):
                    add_text("Aquariam")
                    add_separator()
                    add_selectable("Bream##demo", callback=lambda sender, data: set_value("<None>", sender))
                    add_selectable("Haddock##demo", callback=lambda sender, data: set_value("<None>", sender))
                    add_selectable("Mackerel##demo", callback=lambda sender, data: set_value("<None>", sender))
                    add_selectable("Pollock##demo", callback=lambda sender, data: set_value("<None>", sender))
                    add_selectable("Tilefish##demo", callback=lambda sender, data: set_value("<None>", sender))

            with tree_node("Modals##demo"):
                add_text("Modal windows are like popups but the user cannot close them by clicking outside.")
                add_button("Delete..##modals##demo")
                demo_help("right click for popup, it is set to right mouse button, this can be changed ")
                with popup("Delete?", parent="Delete..##modals##demo", modal=True):
                    add_text("All those beautiful files will be deleted.\nThis operation cannot be undone!")
                    add_separator()
                    add_checkbox("Don't ask me next time##demo")
                    add_button("OK##modal##demo", width=75, callback=lambda sender, data: close_popup("Delete?"))
                    add_same_line()
                    add_button("Cancel##modal##demo", width=75, callback=lambda sender, data: close_popup("Delete?"))

            with tree_node("File Selector##demo"):
                def file_selected(sender, data):
                    log_info(data)
                add_button("Select Python File##demo", callback = lambda sender, data: open_file_dialog(file_selected, ".*,.py"))
                add_button("Select C++ File##demo", callback = lambda sender, data: open_file_dialog(file_selected, ".*,.cpp"))

            with tree_node("Directory Selector##demo"):
                def directory_selected(sender, data):
                    log_info(data)
                add_button("Select Directory##demo", callback = lambda sender, data: select_directory_dialog(directory_selected))

            with tree_node("Menus inside a regular window##demo"):
                add_text("Below we are testing adding menu items to a regular window. It's rather unusual but should work")
                add_separator()
                add_menu_item("Menu item##demotestingmenus", shortcut="CTRL+M")
                with menu("Menu inside a regular window##demo"):
                    add_menu_item("Disabled item##demotestingmenus", enabled=False)
                    add_menu_item("New##demotestingmenus")

        with collapsing_header("Tables##demo"):

            with tree_node("Basic##tables##demo"):

                # basic usage of the table api
                with table(header_row=False):

                    # use add_table_column to add columns to the table
                    add_table_column()
                    add_table_column()
                    add_table_column()

                    # add_table_next_column will jump to the next row
                    # once it reaches the end of the columns
                    for i in range(0, 4):
                        for j in range(0, 3):
                            add_text(f"Row{i} Column{j}")
                            if not (i == 3 and j == 2):
                                add_table_next_column()

            with tree_node("Borders, background##tables##demo"):

                add_checkbox("row_background##tables2##demo", default_value=True, callback=lambda sender:configure_item("table2##demo", row_background=get_value(sender)))
                add_checkbox("borders_innerH##tables2##demo", default_value=True, callback=lambda sender:configure_item("table2##demo", borders_innerH=get_value(sender)))
                add_checkbox("borders_innerV##tables2##demo", default_value=True, callback=lambda sender:configure_item("table2##demo", borders_innerV=get_value(sender)))
                add_checkbox("borders_outerH##tables2##demo", default_value=True, callback=lambda sender:configure_item("table2##demo", borders_outerH=get_value(sender)))
                add_checkbox("borders_outerV##tables2##demo", default_value=True, callback=lambda sender:configure_item("table2##demo", borders_outerV=get_value(sender)))
                add_checkbox("header_row##tables2##demo", default_value=False, callback=lambda sender:configure_item("table2##demo", header_row=get_value(sender)))

                with table("table2##demo", header_row=False, row_background=True,
                          borders_innerH=True, borders_outerH=True, borders_innerV=True,
                           borders_outerV=True):
                    add_table_column("Header 1##democolumns2")
                    add_table_column("Header 2##democolumns2")
                    add_table_column("Header 3##democolumns2")

                    for i in range(0, 5):
                        for j in range(0, 3):
                            add_text(default_value=f"Row{i} Column{j}")
                            if not (i == 4 and j == 2):
                                add_table_next_column()

            with tree_node("Resizable, stretch##tables##demo"):

                add_checkbox("borders_innerV##tables3##demo", default_value=True, callback=lambda sender:configure_item("table3##demo", borders_innerV=get_value(sender)))
                add_checkbox("borders_outerV##tables3##demo", default_value=True, callback=lambda sender:configure_item("table3##demo", borders_outerV=get_value(sender)))
                add_checkbox("resizable##tables3##demo", default_value=True, callback=lambda sender:configure_item("table3##demo", resizable=get_value(sender)))

                with table("table3##demo", header_row=False, resizable=True,
                           borders_outerH=True, borders_innerV=True, borders_outerV=True):
                    add_table_column("Header 1##democolumns3")
                    add_table_column("Header 2##democolumns3")
                    add_table_column("Header 3##democolumns3")

                    for i in range(0, 5):
                        for j in range(0, 3):
                            add_text(default_value=f"Row{i} Column{j}")
                            if not (i == 4 and j == 2):
                                add_table_next_column()

            with tree_node("Resizable, fixed##tables##demo"):

                add_checkbox("no_host_extendX##tables3##demo", callback=lambda sender:configure_item("table4##demo", no_host_extendX=get_value(sender)))

                with table("table4##demo", header_row=False, policy=8192, resizable=True, no_host_extendX=False, 
                           borders_innerV=True, borders_outerV=True,borders_outerH=True):
                    add_table_column("Header 1##democolumns4")
                    add_table_column("Header 2##democolumns4")
                    add_table_column("Header 3##democolumns4")

                    for i in range(0, 5):
                        for j in range(0, 3):
                            add_text(default_value=f"Row{i} Column{j}")
                            if not (i == 4 and j == 2):
                                add_table_next_column()

            with tree_node("Resizable, mixed##tables##demo"):

                with table("table5##demo", header_row=True, policy=mvTable_SizingFixedFit, row_background=True, reorderable=True, 
                           resizable=True, no_host_extendX=False, hideable=True, 
                           borders_innerV=True, borders_outerV=True, borders_innerH=True, borders_outerH=True):
                    add_table_column("AAA##democolumns5", width_fixed=True)
                    add_table_column("BBB##democolumns5", width_fixed=True)
                    add_table_column("CCC##democolumns5", width_stretch=True, init_width_or_weight=0.0)

                    for i in range(0, 5):
                        for j in range(0, 3):
                            if j == 2:
                                add_text(f"Stretch {i}, {j}")
                            else:
                                add_text(f"Fixed {i}, {j}")
                            if not (i == 4 and j == 2):
                                add_table_next_column()

                with table("table6##demo", header_row=True, policy=mvTable_SizingFixedFit, row_background=True, reorderable=True, 
                           resizable=True, no_host_extendX=False, hideable=True, 
                           borders_innerV=True, borders_outerV=True, borders_innerH=True, borders_outerH=True):
                    add_table_column("AAA##democolumns6", width_fixed=True)
                    add_table_column("BBB##democolumns6", width_fixed=True)
                    add_table_column("CCC##democolumns6", width_stretch=True, init_width_or_weight=0.0)
                    add_table_column("DDD##democolumns6", width_stretch=True, init_width_or_weight=0.0)

                    for i in range(0, 5):
                        for j in range(0, 4):
                            if j == 2 or j == 3:
                                add_text(default_value=f"Stretch {i},{j}")
                            else:
                                add_text(default_value=f"Fixed {i}, {j}")
                            if not (i == 4 and j == 3):
                                add_table_next_column()
                              
            with tree_node("Reorderable, hideable, with headers##tables##demo"):

                add_checkbox("hideable##tables7##demo", default_value=True, callback=lambda sender:configure_item("table7##demo", hideable=get_value(sender)))
                add_checkbox("reorderable##tables7##demo", default_value=True, callback=lambda sender:configure_item("table7##demo", reorderable=get_value(sender)))
                add_checkbox("resizable##tables7##demo", default_value=True, callback=lambda sender:configure_item("table7##demo", resizable=get_value(sender)))

                with table("table7##demo", header_row=True, resizable=True,
                           hideable=True, reorderable=True):
                    add_table_column("One##democolumns7")
                    add_table_column("Two##democolumns7")
                    add_table_column("three##democolumns7")

                    for i in range(0, 5):
                        for j in range(0, 3):
                            add_text(f"Hello {i}, {j}")
                            if not (i == 4 and j == 2):
                                add_table_next_column()

            with tree_node("Outer Size##tables##demo"):

                add_checkbox("no_host_extendX##tables8##demo", default_value=True, callback=lambda sender:configure_item("table8##demo", no_host_extendX=get_value(sender)))
                add_checkbox("no_host_extendY##tables8##demo", default_value=False, callback=lambda sender:configure_item("table8##demo", no_host_extendY=get_value(sender)))

                with table("table8##demo", header_row=False, no_host_extendX=True,
                           borders_innerH=True, borders_outerH=True, borders_innerV=True,
                           borders_outerV=True, context_menu_in_body=True, row_background=True,
                           policy=mvTable_SizingFixedFit, height=150):
                    add_table_column("One##democolumns8")
                    add_table_column("Two##democolumns8")
                    add_table_column("three##democolumns8")

                    for i in range(0, 10):
                        for j in range(0, 3):
                            add_text(f"Cell {i}, {j}")
                            if not (i == 9 and j == 2):
                                add_table_next_column()

                add_text("Using explicit size:")
                with table("table9##demo", header_row=False, no_host_extendX=True,
                           borders_innerH=True, borders_outerH=True, borders_innerV=True,
                           borders_outerV=True, context_menu_in_body=True, row_background=True,
                           policy=mvTable_SizingFixedFit, height=0, width=300):
                    add_table_column("One##democolumns9")
                    add_table_column("Two##democolumns9")
                    add_table_column("three##democolumns9")

                    for i in range(0, 6):
                        for j in range(0, 3):
                            add_text(default_value=f"Cell {i}, {j}")
                            if not (i == 9 and j == 2):
                                add_table_next_column()

        with collapsing_header("Drawings##demo"):

            drawing = add_drawing(width=900, height=200) 
            draw_line((10, 10), (100, 100), (255, 0, 0, 255), 1)
            #draw_rectangle((0, 0), (900, 200), (255, 0, 0, 255), fill=(0, 0, 25, 255), rounding=12, thickness = 1.0) 
            #draw_triangle((150, 10), (110, 100), (190, 100), (255, 255, 0, 255), thickness = 3.0)
            #draw_quad((210, 10), (290, 10), (290, 100), (210, 100), (255, 255, 0, 255), thickness = 3.0)
            #draw_circle((350, 60), 49, (255, 255, 0, 255))
            #draw_bezier_curve((410, 10), (450, 25), (410, 50), (490, 85), (255, 255, 0, 255), thickness = 2.0)
            #draw_arrow((510, 10), (590, 80), (255, 0, 0), 4, 10)
            #draw_image("INTERNAL_DPG_FONT_ATLAS", pmin=[610,10], pmax=[690, 80], uv_max=[0.1, 0.1])
            #draw_text((50, 300), "Some Text", color=(255, 255, 0, 255), size=15)
            #draw_text((0, 0), "Origin", color=(255, 255, 0, 255), size=15)
            #draw_polygon(((710, 10), (780, 50), (730, 75), (710, 10)), (255, 125, 0, 255), thickness=1.0, fill=(255, 125, 0, 50))
            #draw_polyline(((810, 20), (835, 50), (890, 10)), (255, 255, 0, 255), thickness=1.0)