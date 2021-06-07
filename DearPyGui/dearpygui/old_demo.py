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
                selectme = add_selectable(label="selectable", callback=demo_log)
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
                        add_simple_plot(label="Curve",default_value=[0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2])

                add_separator()

                add_text("Value", label="Label", show_label=True)
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
                add_input_floatx(label="input floatx", callback=demo_log, default_value=[1,2,3,4])
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
                demo_help(
                        "Click on the colored square to open a color picker.\n"
                        "Click and hold to use drag and drop.\n"
                        "Right-click on the colored square to show options.\n"
                        "CTRL+click on individual component to input value.\n")
                add_color_edit((102, 179, 0, 128), label="color edit 4", callback=demo_log, uint8=True)
                add_color_edit(default_value=(.5, 1, .25, .1), label="color edit 3", callback=demo_log, m_3component=True, uint8=True, floats=False)
                add_listbox(["Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"], label="listbox", num_items=4, callback=demo_log)
                add_color_button(label="color button")
            
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
                add_text(default_value="1", bullet=True)
                add_same_line()
                add_button(label="Button", small=True)

            with tree_node(label="Text"):

                with tree_node(label="Colored Text"):
                    add_text(default_value="Pink", color=[255, 0, 255])
                    add_text(default_value="Yellow", color=[255, 255, 0])
                with tree_node(label="Word Wrapping"):
                    add_text('This text should automatically wrap on the edge of the window.The current implementation for the text wrapping follows simple rules suited for English and possibly other languages', wrap=0)
                    add_slider_int(label="wrap width", default_value=500, max_value=1000, callback=lambda sender:configure_item("text_text1", wrap=get_value(sender)))
                    add_text('The lazy dong is a good dog. This paragraph should fit within the child. Testing a 1 character word. The quick brown fox jumps over the lazy dog.', id='text_text1', wrap=100)

            with tree_node(label="Images"):
                add_text(default_value="Below we are displaying the font texture (which is the only texture we have access to in this demo).")
                add_image("INTERNAL_DPG_FONT_ATLAS")
                add_text(default_value="Here is an image button using a portion of the font atlas")
                demo_enable_disable()
                add_image_button("INTERNAL_DPG_FONT_ATLAS", width=100, height=100, uv_max=[0.1, 0.1], callback=demo_log)
                add_same_line()
                textdata = []
                for i in range(0, 10000):
                    textdata.append(255/255)
                    textdata.append(0)
                    textdata.append(255/255)
                    textdata.append(255/255)
                # TODO: texture requires a name when it would be nice that it didnt
                #add_texture("#cooltexture", textdata, 100, 100, format=mvTEX_RGBA_INT)
                add_static_texture(100, 100, textdata, id="#cooltexture", parent="mvTextureContainer")
                add_image_button("#cooltexture", width=100, height=100, callback=demo_log)

            with tree_node(label="Text Input"):
                demo_enable_disable()
                add_checkbox(label="readonly", callback=demo_config, user_data="Multi-line Text Input")
                add_checkbox(label="on_enter", callback=demo_config, user_data="Multi-line Text Input")
                with tree_node(id="Multi-line Text Input"):
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
                add_simple_plot(label="Frame Times", default_value=[0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2])
                add_simple_plot(label="Histogram", default_value=(0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2), height=80, histogram=True, minscale=0.0)

                data1 = []
                for i in range(0, 70):
                    data1.append(cos(3.14*6*i/180))
                add_simple_plot(label="Lines", default_value=data1, height=80)
                add_simple_plot(label="Histogram", default_value=data1, height=80, histogram=True)
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

            with tree_node(label="Time/Date Widgets"):

                add_time_picker(label="Time Picker", default_value={'hour': 14, 'min': 32, 'sec': 23})
                add_separator()
                with table(header_row=False):
                    add_table_column()
                    add_table_column()
                    add_table_column()

                    add_date_picker(label="Date Picker1", level=0, default_value={'month_day': 8, 'year':93, 'month':5})
                    add_table_next_column()
                    add_date_picker(label="Date Picker2", level=1, default_value={'month_day': 8, 'year':93, 'month':5})
                    add_table_next_column()
                    add_date_picker(label="Date Picker3", level=2, default_value={'month_day': 8, 'year':93, 'month':5})

            with tree_node(label="Item config/state/info"):
                def log_status_config_state():
                    log_debug(get_item_configuration(test))
                    log_debug(get_item_info(test))
                    log_debug(get_item_state(test))

                test = add_button(label="Log Get info/state/config", callback=log_status_config_state)

                #simple commands related to configuration
                add_text(f"label: {get_item_label(test)}")
                add_text(f"source: {get_item_source(test)}")
                add_text(f"show: {is_item_shown(test)}")
                add_text(f"enabled: {is_item_enabled(test)}")
                add_text(f"width: {get_item_width(test)}")
                add_text(f"height: {get_item_height(test)}")
                add_text(f"callback: {get_item_callback(test)}")
                add_text(f"user_data: {get_item_user_data(test)}")

                #simple commands related to info
                info = get_item_info(test)
                add_text(f"children: {get_item_children(test)}")
                add_text(f"type: {get_item_type(test)}")
                add_text(f"parent: {get_item_parent(test)}")
                add_text(f"container: {is_item_container(test)}")

                #simple commands related to state
                add_text(f"hovered: {is_item_hovered(test)}")
                add_text(f"active: {is_item_active(test)}")
                add_text(f"focused: {is_item_focused(test)}")
                add_text(f"clicked: {is_item_clicked(test)}")
                add_text(f"visible: {is_item_visible(test)}")
                add_text(f"edited: {is_item_edited(test)}")
                add_text(f"activated: {is_item_activated(test)}")
                add_text(f"deactivated: {is_item_deactivated(test)}")
                add_text(f"deactivated_after_edit: {is_item_deactivated_after_edit(test)}")
                add_text(f"toggled_open: {is_item_toggled_open(test)}")
                add_text(f"ok: {is_item_ok(test)}")
                add_text(f"rect_min: {get_item_rect_min(test)}")
                add_text(f"rect_max: {get_item_rect_max(test)}")
                add_text(f"rect_size: {get_item_rect_size(test)}")
                add_text(f"pos: {get_item_pos(test)}")

        with collapsing_header(label="Layout & Scolling##demo"):

            with tree_node(id="Child windows##demo"):

                with child(border=False, width=400):
                    for i in range(0, 100):
                        add_text(f"{i}: scrollable region")

                add_same_line()
                with child(autosize_x=True):
                    with menu_bar():
                        with menu(id="Menu##child##demo"):
                            pass
                    with group(width=-20):
                        for i in range(0, 100):
                            add_button(id=f"{i}##childbutton##demo")

            with tree_node(id="Child Window Flags##demo"):

                with table(header_row=False):
                    add_table_column()
                    add_table_column()
                    add_table_column()

                    add_checkbox(id="autosize_x##demo", callback=lambda sender, data: configure_item("testchild##demo", autosize_x=get_value(sender)))
                    add_checkbox(id="autosize_y##demo", callback=lambda sender, data: configure_item("testchild##demo", autosize_y=get_value(sender)))

                    add_table_next_column()
                    add_checkbox(id="menubar##childdemo", default_value=True, callback=lambda sender, data: configure_item("testchild##demo", menubar=get_value(sender)))
                    add_checkbox(id="no_scrollbar##childdemo", callback=lambda sender, data: configure_item("testchild##demo", no_scrollbar=get_value(sender)))

                    add_table_next_column()
                    add_checkbox(id="horizontal_scrollbar##childdemo", callback=lambda sender, data: configure_item("testchild##demo", horizontal_scrollbar=get_value(sender)))
                    add_checkbox(id="border##childdemo", default_value=True, callback=lambda sender, data: configure_item("testchild##demo", border=get_value(sender)))

                with child(width=500, height=500):
                    with menu_bar():
                        with menu(id="Menu##testchild##demo"):
                            pass
                    for i in range(0, 100):
                        add_text(default_value="A pretty long sentence if you really think about it. It's also pointless.")

            with tree_node(id="Widgets Width##demo"):
                
                add_text("Width=100")
                add_drag_float(id="float##demowidths1", width=100)

                add_text("Width=-100")
                add_drag_float(id="float##demowidths2", width=-100)

                add_text("Width=-1")
                add_drag_float(id="float##demowidths3", width=-1)

                add_text("group with width=75")
                with group(width=75):
                    add_drag_float(id="float##demowidths4")
                    add_drag_float(id="float##demowidths5")
                    add_drag_float(id="float##demowidths6")

            with tree_node(id="Basic Horizontal Layout##demo"):
                add_text("(Use add_same_line(), to keep adding items to the right of the preceding item)")
                add_text("Normal buttons")
                add_same_line()
                add_button(id="Banana##demo")
                add_same_line()
                add_button(id="Apple##demo")
                add_same_line()
                add_button(id="Corniflower##demo")

                add_text("Small buttons")
                add_same_line()
                add_button(id="Like this one##demo", small=True)
                add_same_line()
                add_text("can fit within a text block")

                add_text("Aligned")
                add_same_line(xoffset=150)
                add_text("x=150")
                add_same_line(xoffset=300)
                add_text("x=300")

                add_text(default_value="Aligned")
                add_same_line(xoffset=150)
                add_button(id="x=150##demo1", small=True)
                add_same_line(xoffset=300)
                add_button(id="x=300##demo1", small=True)

                add_checkbox(id="My##demo")
                add_same_line()
                add_checkbox(id="Tailor##demo")
                add_same_line()
                add_checkbox(id="is##demo")
                add_same_line()
                add_checkbox(id="rich##demo")

                add_text("Lists:")
                add_listbox(["AAAA", "BBBB", "CCCC", "DDDD"], default_value="AAAA", width=100)
                add_same_line()
                add_listbox(["AAAA", "BBBB", "CCCC", "DDDD"], default_value="BBBB", width=100)
                add_same_line()
                add_listbox(["AAAA", "BBBB", "CCCC", "DDDD"], default_value="CCCC", width=100)
                add_same_line()
                add_listbox(["AAAA", "BBBB", "CCCC", "DDDD"], default_value="DDDD", width=100)
                

                add_text("Spacing(100):")
                add_button(id="A##demospacing", width=50, height=50)
                add_same_line(spacing=100)
                add_button(id="B##demospacing", width=50, height=50)

            with tree_node(id="Tabs##demo"):
                with tree_node(id="Basic##tabs##demo"):
                    with tab_bar():
                        with tab(id="Avocado##demo"):
                            add_text("This is the avocado tab!")
                        with tab(id="Broccoli##demo"):
                            add_text("This is the broccoli tab!")
                        with tab(id="Cucumber##demo"):
                            add_text("This is the cucumber tab!")
                with tree_node(id="Advanced##tabs##demo"):
                    add_checkbox(id="tab bar reorderable##demo", callback=lambda sender, data: configure_item("Basic Tabbar1##demo", reorderable=get_value(sender)))
                    add_checkbox(id="tab 2 no_reorder##demo", callback=lambda sender, data: configure_item("tab 2##demo", no_reorder=get_value(sender)))
                    add_checkbox(id="tab 2 leading##demo", callback=lambda sender, data: configure_item("tab 2##demo", leading=get_value(sender)))
                    add_checkbox(id="tab 2 trailing##demo", callback=lambda sender, data: configure_item("tab 2##demo", trailing=get_value(sender)))
                    add_checkbox(id="tab button trailing##demo", callback=lambda sender, data: configure_item("+##demo", trailing=get_value(sender)))
                    add_checkbox(id="tab button leading##demo", callback=lambda sender, data: configure_item("+##demo", leading=get_value(sender)))
                    with tab_bar(id="Basic Tabbar1##demo"):
                        with tab(id="tab 1##demo"):
                            add_text("This is the tab 1!")
                        with tab(id="tab 2##demo"):
                            add_text("This is the tab 2!")
                        with tab(id="tab 3##demo"):
                            add_text("This is the tab 3!")
                        with tab(id="tab 4##demo"):
                            add_text("This is the tab 4!")
                        add_tab_button(id="+##demo", callback=lambda sender, data: log_debug("Pressed tab button"))
                        add_tab_button(id="?##demo", callback=lambda sender, data: log_debug("Pressed tab button"))

            with tree_node(id="Groups##demo123"):
                add_text("Groups can be used to bundle widths together so that you can use functions such as is_item_hovered or add_same_line on the whole group.")
                with group(id="group##demotabexamples"):
                    add_button(id="AAA##demogroup")
                    add_button(id="BBB##demogroup")
                    add_button(id="CCC##demogroup")
                with tooltip(parent="group##demotabexamples"):
                    add_text("The group is hovered", wrap=200)

                add_text("Horizontal group:")
                with group(horizontal=True):
                    add_button(id="AAA##demogroup1")
                    add_button(id="BBB##demogroup1")
                    add_button(id="CCC##demogroup1")

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

