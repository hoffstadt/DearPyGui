from dearpygui.core import *
from dearpygui.simple import *
from math import sin, cos
import random

########################################################################################################################
# Helpers
########################################################################################################################

def helpmarker(message):
    add_same_line()
    add_text("(?)", color=[150, 150, 150], tip=message)

def hsv_to_rgb(h: float, s: float, v: float) -> (float, float, float):
    if s == 0.0: return (v, v, v)
    i = int(h*6.) # XXX assume int() truncates!
    f = (h*6.)-i; p,q,t = v*(1.-s), v*(1.-s*f), v*(1.-s*(1.-f)); i%=6
    if i == 0: return (255*v, 255*t, 255*p)
    if i == 1: return (255*q, 255*v, 255*p)
    if i == 2: return (255*p, 255*v, 255*t)
    if i == 3: return (255*p, 255*q, 255*v)
    if i == 4: return (255*t, 255*p, 255*v)
    if i == 5: return (255*v, 255*p, 255*q)

def demo_main_callback(sender, data):

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
        
def show_demo():

    # the render callback is a function that runs every frame
    set_render_callback(demo_main_callback)

    #set_accelerator_callback(demo_accelerator_callback)

    def on_demo_close(sender, data):

        delete_item("Dear PyGui Demo")
        if does_item_exist("Logging Widget On Window##demo"):
            delete_item("Logging Widget On Window##demo")
        set_render_callback(None)
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

    with window("Dear PyGui Demo", x_pos=100, y_pos=100, width=800, height=800, on_close=on_demo_close):

        with menu_bar("MenuBar##demo"):

            with menu("Menu##demo"):
                add_menu_item("New##demomenu")
                add_menu_item("Open##demomenu")
                with menu("Open Recent##demo"):
                    add_menu_item("harrell.c##demo")
                    add_menu_item("patty.h##demo")
                    add_menu_item("nick.py##demo")
                add_menu_item("Save##demo")
                add_menu_item("Save As..#demo")
                add_separator()
                with menu("Options##demomenu"):
                    add_menu_item("Enabled##demo", check=True)
                    with child("childmenu##demo", height=60, autosize_x=True):
                        for i in range(0, 10):
                            add_text(f"Scrolling Text {i}")
                    add_slider_float("Value##demomenu")
                    add_input_float("Input##demomenu")
                    add_combo("Combo##demomenu", items=["Yes", "No", "Maybe"])
                    add_checkbox("Some Option##demomenu")

            with menu("Themes##demo"):
                add_menu_item("Dark", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Light", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Classic", callback = lambda sender, data: set_theme(sender), check=True, shortcut="Ctrl+Shift+T")
                add_menu_item("Dark 2", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Grey", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Dark Grey", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Cherry", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Purple", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Gold", callback = lambda sender, data: set_theme(sender), check=True, shortcut="Ctrl+Shift+P")
                add_menu_item("Red", callback = lambda sender, data: set_theme(sender), check=True, shortcut="Ctrl+Shift+Y")

            with menu("Tools##demo"):
                add_menu_item("Show Logger##demo", callback=show_logger)
                add_menu_item("Show About##demo", callback=show_about)
                add_menu_item("Show Metrics##demo", callback=show_metrics)
                add_menu_item("Show Documentation##demo", callback=show_documentation)
                add_menu_item("Show Debug##demo", callback=show_debug)
                add_menu_item("Show Style Editor##demo", callback=show_style_editor)

        add_text(f'Dear PyGui says hello. ({get_dearpygui_version()})')
        add_text("This code for this demo can be found here: ")
        add_text("https://github.com/hoffstadt/DearPyGui/blob/master/DearPyGui/dearpygui/demo.py")

        with collapsing_header("Window options##demo"):
            with managed_columns("Window Options Col##demo", 3, border=False):
                add_checkbox("No titlebar##demo", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_title_bar=get_value(sender)))
                add_checkbox("No scrollbar##demo", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_scrollbar=get_value(sender)))
                add_checkbox("No menu##demo", callback=lambda sender, data: configure_item("Dear PyGui Demo", menubar=not get_value(sender)))

                add_checkbox("No move##demo", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_move=get_value(sender)))
                add_checkbox("No resize##demo", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_resize=get_value(sender)))
                add_checkbox("No collapse##demo", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_collapse=get_value(sender)))
            
                add_checkbox("No close##demo", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_close=get_value(sender)))
                add_checkbox("No background##demo", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_background=get_value(sender)))
                add_checkbox("No bring to front##demo", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_bring_to_front_on_focus=get_value(sender)))

        with collapsing_header("Widgets##demo"):

            with tree_node("Basic##demo"):
                def log_callback(sender, data):
                    log_debug(f"{sender} ran a callback its value is {get_value(sender)}")
                def toggle_config(sender, data):
                    config_dict = {}
                    for kwarg in data['kwargs']:
                        config_dict[kwarg] = get_value(sender)
                    for item in data['items']:
                        configure_item(item, **config_dict)

                disable_items = ["Button1##demo", "Button2##demo", "Button3##demo", "Button4##demo", "Button5##demo", "Button6##demo"
                    ,"checkbox##demo", "radiobutton##demo", "selectable##demo", "Left##demo", "Right##demo"
                    ,"combo##demo","listbox##demo","input text##demo","input text (w/ hint)##demo"
                    ,"input int##demo", "input float##demo", "input scientific##demo", "input float3##example##demo"
                    ,"drag int", "drag int 0..100##demo", "drag float##demo", "drag small float##demo"
                    ,"slider int##demo", "slider float##demo", "slider angle##demo"]
                add_checkbox("Enable-Disable##basic", default_value=True, callback=toggle_config, callback_data={'kwargs': ['enabled'], 'items': disable_items})
                helpmarker('This will toggle the keyword "enable" for the widgets below that can be enabled & disabled')
                with group("buttons##demo", horizontal=True):
                    add_button("Button1##demo", callback=log_callback)
                    add_button("Button2##demo", callback=log_callback, small=True)
                    add_button("Button3##demo", callback=log_callback, arrow=True)
                    add_button("Button4##demo", callback=log_callback, arrow=True, direction=1)
                    add_button("Button5##demo", callback=log_callback, arrow=True, direction=2)
                    add_button("Button6##demo", callback=log_callback, arrow=True, direction=3)
                add_checkbox("checkbox##demo", callback=log_callback)
                add_radio_button("radiobutton##demo", items=["radio a", "radio b", "radio c"], horizontal=True, callback=log_callback)
                add_selectable("selectable##demo", callback=log_callback)
                for i in range(0, 7):
                    if i > 0:
                        add_same_line()
                    add_button(f"Click##{i}")
                    set_item_color(f"Click##{i}", mvGuiCol_Button, hsv_to_rgb(i/7.0, 0.6, 0.6))
                    set_item_color(f"Click##{i}", mvGuiCol_ButtonHovered, hsv_to_rgb(i/7.0, 0.7, 0.7))
                    set_item_color(f"Click##{i}", mvGuiCol_ButtonActive, hsv_to_rgb(i/7.0, 0.8, 0.8))
                    set_item_style_var(f"Click##{i}", mvGuiStyleVar_FrameRounding, [i*5])
                    set_item_style_var(f"Click##{i}", mvGuiStyleVar_FramePadding, [i*3, i*3])
                    disable_items.append(f"Click##{i}")
                with group("increment_buttons##demo", horizontal=True):
                    add_text("Press a button: ")
                    add_button("Left##demo", arrow=True, direction=mvDir_Left, 
                               callback=lambda sender, data: set_value("value", int(get_value("value"))-1))
                    add_button("Right##demo", arrow=True, direction=mvDir_Right,
                               callback=lambda sender, data: set_value("value", int(get_value("value"))+1))
                    add_text("value", default_value="0")

                add_separator()

                add_label_text("label##demo", default_value="Value")
                add_combo("combo##demo", items=["AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK"], 
                          default_value="AAAA", callback=log_callback)
                add_input_text("input text##demo", default_value="Hello, world!", callback=log_callback)
                helpmarker(
                        "USER:\n"
                        "Hold SHIFT or use mouse to select text.\n"
                        "CTRL+Left/Right to word jump.\n"
                        "CTRL+A or double-click to select all.\n"
                        "CTRL+X,CTRL+C,CTRL+V clipboard.\n"
                        "CTRL+Z,CTRL+Y undo/redo.\n"
                        "ESCAPE to revert.\n\n")
                add_input_text("input text (w/ hint)##demo", hint="enter text here", callback=log_callback)
                add_input_int("input int##demo", callback=log_callback)
                add_input_float("input float##demo", callback=log_callback)
                add_input_float("input scientific##demo", format="%e", callback=log_callback)
                add_input_float3("input float3##example##demo", callback=log_callback)
                add_drag_int("drag int", callback=log_callback)
                helpmarker(
                        "Click and drag to edit value.\n"
                        "Hold SHIFT/ALT for faster/slower edit.\n"
                        "Double-click or CTRL+click to input value.")
                add_drag_int("drag int 0..100##demo", format="%d%%", callback=log_callback)
                add_drag_float("drag float##demo", callback=log_callback)
                add_drag_float("drag small float##demo", default_value=0.0067, format="%.06f ns", callback=log_callback)
                add_slider_int("slider int##demo", max_value=3, callback=log_callback)
                helpmarker("CTRL+click to enter value.")
                add_slider_float("slider float##demo", max_value=1.0, format="ratio = %.3f", callback=log_callback)
                add_slider_int("slider angle##demo", min_value=-360, max_value=360, format="%d deg", callback=log_callback)
                add_color_edit3("color 1##demo", default_value=[255, 0, 51])
                helpmarker(
                        "Click on the colored square to open a color picker.\n"
                        "Click and hold to use drag and drop.\n"
                        "Right-click on the colored square to show options.\n"
                        "CTRL+click on individual component to input value.\n")
                add_color_edit4("color 2##demo", default_value=[102, 179, 0, 128])
                add_listbox("listbox##demo", items=["Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"]
                            , num_items=4, callback=log_callback)

            with tree_node("Bullets##demo"):
                add_text("Bullet point 1", bullet=True)
                add_text("Bullet point 2\nOn multiple lines", bullet=True)
                with tree_node("Tree node"):
                    add_text("Another bullet point", bullet=True)
                add_text("", bullet=True)
                add_same_line()
                add_button("Button##bullets##demo", small=True)

            with tree_node("Text##demo"):

                with tree_node("Colored Text"):
                    add_text("Pink", color=[255, 0, 255])
                    add_text("Yellow", color=[255, 255, 0])
    
            with tree_node("Images##demo"):
                add_text("Below we are displaying the font texture (which is the only texture we have access to in this demo).")
                add_image("image##demo", "INTERNAL_DPG_FONT_ATLAS")
                add_text("Here is an image button using a portion of the font atlas")
                add_image_button("#image##button1", "INTERNAL_DPG_FONT_ATLAS", uv_max=[0.1, 0.1])
                add_same_line()
                textdata = []
                for i in range(0, 10000):
                    textdata.append(255)
                    textdata.append(0)
                    textdata.append(255)
                    textdata.append(255)
                add_texture("#cooltexture", textdata, 100, 100, format=mvTEX_RGBA_INT)
                add_image_button("#image##button2", "#cooltexture")

            with tree_node("Text Input##demo"):
                disable_items = ["##multiline##demo","default##demo", "decimal##demo", "hexdecimal##demo", 
                                 "uppercase##demo", "no blank##demo", "scientific##demo","password##demo", 
                                 "password (w/ hint)##demo", "password (clear)##demo"]
                add_checkbox("Enable-Disable##text_input", default_value=True, callback=toggle_config, callback_data={'kwargs': ['enabled'], 'items': disable_items})
                add_checkbox("readonly##text_input", default_value=False, callback=toggle_config, callback_data={'kwargs': ['readonly'], 'items': disable_items})
                add_checkbox("on_enter##text_input", default_value=False, callback=toggle_config, callback_data={'kwargs': ['on_enter'], 'items': disable_items})
                with tree_node("Multi-line Text Input##demo"):
                    add_input_text("##multiline##demo", multiline=True, default_value="/*\n"
                        " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
                        " the hexadecimal encoding of one offending instruction,\n"
                        " more formally, the invalid operand with locked CMPXCHG8B\n"
                        " instruction bug, is a design flaw in the majority of\n"
                        " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
                        " processors (all in the P5 microarchitecture).\n"
                        "*/\n\n"
                        "label:\n"
                        "\tlock cmpxchg8b eax\n", height=300, callback=log_callback, tab_input=True)

                with tree_node("Filtered Text Input##demo"):
                    add_input_text("default##demo", callback=log_callback)
                    add_input_text("decimal##demo", decimal=True, callback=log_callback)
                    add_input_text("hexdecimal##demo", hexadecimal=True, callback=log_callback)
                    add_input_text("uppercase##demo", uppercase=True, callback=log_callback)
                    add_input_text("no blank##demo", no_spaces=True, callback=log_callback)
                    add_input_text("scientific##demo", scientific=True, callback=log_callback)
            
                with tree_node("Password Input##demo"):
                    add_input_text("password##demo", password=True, source="password", callback=log_callback)
                    add_input_text("password (w/ hint)##demo", password=True, hint="<password>", source="password", callback=log_callback)
                    add_input_text("password (clear)##demo", source="password", callback=log_callback)

            with tree_node("Simple Plot Widgets##demo"):
                add_simple_plot("Frame Times##demo", value=[0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2])
                add_simple_plot("Histogram##demo", value=(0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2), height=80, histogram=True, minscale=0.0)

                data1 = []
                for i in range(0, 70):
                    data1.append(cos(3.14*6*i/180))
                add_simple_plot("Lines##sin##demo", value=data1, height=80)
                add_simple_plot("Histogram##sin##demo", value=data1, height=80, histogram=True)
                add_progress_bar("Progress Bar##demo", default_value=0.78, overlay="78%")
                add_same_line()
                add_text("Progress Bar##text##demo")
                set_value("Progress Bar##text##demo", "Progress Bar")
                add_progress_bar("##Progress Bar##demo", default_value=0.78, overlay="1367/1753")
                set_item_color("##Progress Bar##demo", mvGuiCol_PlotHistogram, [255,0,0, 255])

            with tree_node("Color/Picker Widgets##demo"):
                # wrapper to apply configuration to all items passed in as a list
                def configure_items(names, **kwargs):
                    for name in names:
                        configure_item(name, **kwargs)
                color_edit_names = ["MyColor##1", "MyColor##2"]

                with managed_columns("##demowidgetscolor", 3, border=False):
                    add_checkbox("With Alpha Preview", callback=lambda sender, data: configure_items(color_edit_names, alpha_preview = get_value(sender)))
                    add_checkbox("With Half Alpha Preview", callback=lambda sender, data: configure_items(color_edit_names, alpha_preview_half = get_value(sender)))
                    add_checkbox("With No Small Preview", callback=lambda sender, data: configure_items(color_edit_names, no_small_preview = get_value(sender)))
                    add_checkbox("With No Inputs", callback=lambda sender, data: configure_items(color_edit_names, no_inputs = get_value(sender)))
                    add_checkbox("With No Tooltip", callback=lambda sender, data: configure_items(color_edit_names, no_tooltip = get_value(sender)))
                    add_checkbox("With RGB", callback=lambda sender, data: configure_items(color_edit_names, display_rgb = get_value(sender)))
                    add_checkbox("With HSV", callback=lambda sender, data: configure_items(color_edit_names, display_hsv = get_value(sender)))
                    add_checkbox("With HEX", callback=lambda sender, data: configure_items(color_edit_names, display_hex = get_value(sender)))
                    add_checkbox("With Ints", callback=lambda sender, data: configure_items(color_edit_names, uint8 = get_value(sender)))
                    add_checkbox("With Floats", callback=lambda sender, data: configure_items(color_edit_names, floats = get_value(sender)))
                helpmarker("Right-click on the individual color widget to show options.")
                add_checkbox("With No Drag and Drop", callback=lambda sender, data: configure_items(color_edit_names, no_drag_drop = get_value(sender)))
                helpmarker("Click and drag a preview square, drop on another color widget to apply the color")
                add_checkbox("With No Options Menu", callback=lambda sender, data: configure_items(color_edit_names, no_options = get_value(sender)))
                helpmarker("Right clicking a color widget brings up an options context menu")
                
                add_value("colorvalue", [0.0, 0.0, 0.0, 0.0])
                add_text("Color Widget:")
                add_color_edit3(color_edit_names[0], source="colorvalue")
                
                add_text("Color Widget HSV with Alpha:")
                add_color_edit4(color_edit_names[1], source="colorvalue", display_hsv=True, alpha_preview=True)
                add_text("Color button with Picker:")
                helpmarker("using no inputs and no label leaves only the preview\n"
                           "click the color edit preview will reveal the color picker.")
                add_color_edit4("Color Edit 4##2", source="colorvalue", no_inputs=True, no_label=True)
                
                add_text("Color button with Custom Picker Popup:")
                add_color_edit4("Color Edit 4 (with custom popup)", source="colorvalue", no_inputs=True, no_picker=True)
                helpmarker("we can override the popup with our own custom popup that includes a color pallet")
                with popup("Color Edit 4 (with custom popup)", "custom picker popup", mousebutton=0):
                    add_color_picker4("custom picker", no_tooltip=True, picker_hue_wheel=True)
                    add_text("Color Pallet")
                    for i in range(30):
                        add_color_button(f"color button {i}", hsv_to_rgb(i/30,1,1))
                        if i<9: add_same_line()
                        if i>9 and i<19: add_same_line()
                        if i>19 and i<29: add_same_line()
                
                add_text("Color button only:")
                add_checkbox("no_border", callback=lambda sender, data: configure_item("Color Button", no_border=get_value(sender)))
                add_color_button("Color Button", (255, 50, 255, 0), width=50, height=50)
                with managed_columns("##demowidgetscolor2", 2, border=False):
                    add_checkbox("With Alpha", default_value=True, callback=lambda sender, data: configure_item("Color Picker 4", alpha_preview = get_value(sender)))
                    add_checkbox("With Alpha Bar", default_value=True, callback=lambda sender, data: configure_item("Color Picker 4", alpha_bar = get_value(sender)))
                    add_checkbox("With Side Preview", callback=lambda sender, data: configure_item("Color Picker 4", no_side_preview = get_value(sender)))
                    add_checkbox("Display RGB", callback=lambda sender, data: configure_item("Color Picker 4", display_rgb = get_value(sender)))
                    add_checkbox("Display HSV", callback=lambda sender, data: configure_item("Color Picker 4", display_hsv = get_value(sender)))
                    add_checkbox("Display HEX", callback=lambda sender, data: configure_item("Color Picker 4", display_hex = get_value(sender)))
                def apply_hue(sender, data):
                    log_debug(get_value(sender))
                    if(get_value(sender) == 0): 
                        configure_item("Color Picker 4", picker_hue_bar = True)
                    elif(get_value(sender) == 1): 
                        configure_item("Color Picker 4", picker_hue_wheel = True)
                add_radio_button("Display Type", items=["Hue Bar", "Hue Wheel"], callback=apply_hue)
                add_color_picker4("Color Picker 4", source="colorvalue", alpha_preview= True, alpha_bar=True)
                add_value("list_color_value", [0.5,0.5,0.5,1.0])
                add_color_edit4("Color Edit 4 (float values)", source="list_color_value", alpha_preview= True, floats=True, callback=lambda sender, data: configure_item("float_values", label=f"{get_value('list_color_value')}", color=hsv_to_rgb(get_value('list_color_value')[0],get_value('list_color_value')[1],get_value('list_color_value')[2])))
                helpmarker("Color item values given to the widget as a list will cause the \n"
                           "color item to store and return colors as scalar floats from 0.0-1.0.\n"
                           "setting floats=True will turn the inputs also to a float (although this is not necessary)")
                add_label_text("float_values", default_value="Color List: ",label=f"{get_value('list_color_value')}",color=hsv_to_rgb(get_value('list_color_value')[0],get_value('list_color_value')[1],get_value('list_color_value')[2]))
                add_color_edit4("Color Edit 4 (ints value)", default_value=(125,125,125,255), alpha_preview= True, callback=lambda sender, data: configure_item("ints_values", label=f"({get_value(sender)[0]}, {get_value(sender)[1]}, {get_value(sender)[2]}, {get_value(sender)[3]})",color=get_value(sender)))
                helpmarker("Color item values given to the widget as a tuple will cause the \n"
                           "color item to store and return colors as ints from 0-255.")
                add_label_text("ints_values", default_value="Color Tuple: ", label=f"{get_value('Color Edit 4 (ints value)')}", color=get_value('Color Edit 4 (ints value)'))

            with tree_node("Multi-component Widgets##demo"):
                disable_items = ["input float2##demo", "drag float2##demo", "slider float2##demo", "input int2##demo", "drag int2##demo", 
                                 "slider int2##demo", "input float3##demo", "drag float3##demo", "slider float3##demo", "input int3##demo", 
                                 "drag int3##demo", "slider int3##demo", "input float4##demo", "drag float4##demo", "slider float4##demo", 
                                 "input int4##demo", "drag int4##demo", "slider int4##demo"]
                add_checkbox("Enable-Disable##multi-component_widgets", default_value=True, callback=toggle_config, callback_data={'kwargs': ['enabled'], 'items': disable_items})
                add_input_float2("input float2##demo", source="float2", min_value=0.0, max_value=100.0)
                add_drag_float2("drag float2##demo", source="float2")
                add_slider_float2("slider float2##demo", source="float2")
                add_input_int2("input int2##demo", source="int2", min_value=0, max_value=100)
                add_drag_int2("drag int2##demo", source="int2")
                add_slider_int2("slider int2##demo", source="int2")
                add_spacing()
                add_input_float3("input float3##demo", source="float3", min_value=0.0, max_value=100.0)
                add_drag_float3("drag float3##demo", source="float3")
                add_slider_float3("slider float3##demo", source="float3")
                add_input_int3("input int3##demo", source="int3", min_value=0, max_value=100)
                add_drag_int3("drag int3##demo", source="int3")
                add_slider_int3("slider int3##demo", source="int3")
                add_spacing()
                add_input_float4("input float4##demo", source="float4", min_value=0.0, max_value=100.0)
                add_drag_float4("drag float4##demo", source="float4")
                add_slider_float4("slider float4##demo", source="float4")
                add_input_int4("input int4##demo", source="int4", min_value=0, max_value=100)
                add_drag_int4("drag int4##demo", source="int4")
                add_slider_int4("slider int4##demo", source="int4")

            with tree_node("Vertical Sliders##demo"):

                disable_items = [f"##vi","##vs1##demo","##vs2##demo","##vs3##demo","##vs4##demo"]
                for i in range(0, 7):
                    disable_items.append(f"##v{i}##demo")
                for i in range(0, 3):
                    for j in range(0, 4):
                        disable_items.append(f"##v{j}{i}##demo")
                add_checkbox("Enable-Disable##vertical_sliders", default_value=True, callback=toggle_config, callback_data={'kwargs': ['enabled'], 'items': disable_items})
                add_slider_int(f"##vi", default_value=1, vertical=True, max_value=5, height=160)
                add_same_line()
                with group("v group 1##demo"):
                    values = [ 0.0, 0.60, 0.35, 0.9, 0.70, 0.20, 0.0 ]
                    for i in range(0, 7):
                        if i > 0:
                            add_same_line()
                        add_slider_float(f"##v{i}##demo", default_value=values[i], vertical=True, max_value=1.0, height=160)
                        set_item_color(f"##v{i}##demo", mvGuiCol_FrameBg, hsv_to_rgb(i/7.0, 0.5, 0.5))
                        set_item_color(f"##v{i}##demo", mvGuiCol_FrameBgHovered, hsv_to_rgb(i/7.0, 0.6, 0.5))
                        set_item_color(f"##v{i}##demo", mvGuiCol_FrameBgActive, hsv_to_rgb(i/7.0, 0.7, 0.5))
                        set_item_color(f"##v{i}##demo", mvGuiCol_SliderGrab, hsv_to_rgb(i/7.0, 0.9, 0.9))

                add_same_line()
                with group("v group 2##demo"):
                    for i in range(0, 3):
                        with group(f"v group 2{i}##demo"):
                            values = [ 0.20, 0.80, 0.40, 0.25 ]
                            for j in range(0, 4):
                                add_slider_float(f"##v{j}{i}##demo", default_value=values[j], vertical=True, max_value=1.0, height=50, source=f"v{j}")
                                if i != 3:
                                    add_same_line()   
                            
                add_same_line()
                with group("v group 3##demo"):
                    add_slider_float("##vs1##demo", vertical=True, max_value=1.0, height=160, source="##v1", width=40)
                    add_same_line()
                    add_slider_float("##vs2##demo", vertical=True, max_value=1.0, height=160, source="##v2", width=40)
                    add_same_line()
                    add_slider_float("##vs3##demo", vertical=True, max_value=1.0, height=160, source="##v3", width=40)
                    add_same_line()
                    add_slider_float("##vs4##demo", vertical=True, max_value=1.0, height=160, source="##v4", width=40)

            with tree_node("Time/Date Widgets##demo"):
                add_time_picker("Time Picker##demo", default_value={'hour': 14, 'min': 32, 'sec': 23})
                add_separator()
                with managed_columns("Date Columns##demo", 3):
                    add_date_picker("Date Picker1##demo", level=0, default_value={'month_day': 8, 'year':93, 'month':5})
                    add_date_picker("Date Picker2##demo", level=1, default_value={'month_day': 8, 'year':93, 'month':5})
                    add_date_picker("Date Picker3##demo", level=2, default_value={'month_day': 8, 'year':93, 'month':5})
                add_separator()

        with collapsing_header("Layout & Scolling##demo"):

            with tree_node("Child windows##demo"):

                with child("child##demo", border=False, width=400):
                    for i in range(0, 100):
                        add_text(f"{i}: scrollable region")

                add_same_line()
                with child("decorated child##demo", autosize_x=True):
                    with menu_bar("MenuBarChild##demo"):
                        with menu("Menu##child##demo"):
                            pass
                    with group("decorated child group##demo", width=-20):
                        for i in range(0, 100):
                            add_button(f"{i}##childbutton##demo")

            with tree_node("Child Window Flags##demo"):

                with managed_columns("##childwindowcol", 3, border=False):
                    add_checkbox("autosize_x##demo", callback=lambda sender, data: configure_item("testchild##demo", autosize_x=get_value(sender)))
                    add_checkbox("autosize_y##demo", callback=lambda sender, data: configure_item("testchild##demo", autosize_y=get_value(sender)))
                    add_checkbox("menubar##childdemo", default_value=True, callback=lambda sender, data: configure_item("testchild##demo", menubar=get_value(sender)))
                    add_checkbox("no_scrollbar##childdemo", callback=lambda sender, data: configure_item("testchild##demo", no_scrollbar=get_value(sender)))
                    add_checkbox("horizontal_scrollbar##childdemo", callback=lambda sender, data: configure_item("testchild##demo", horizontal_scrollbar=get_value(sender)))
                    add_checkbox("border##childdemo", default_value=True, callback=lambda sender, data: configure_item("testchild##demo", border=get_value(sender)))
                with child("testchild##demo", width=500, height=500):
                    set_item_color("testchild##demo", mvGuiCol_ChildBg, [255, 0, 0, 100])
                    with menu_bar("MenuBartestChild##demo"):
                        with menu("Menu##testchild##demo"):
                            pass
                    for i in range(0, 100):
                        add_text("A pretty long sentence if you really think about it. It's also pointless.")

            with tree_node("Widgets Width##demo"):
                
                add_text("Width=100")
                add_drag_float("float##demowidths1", width=100)

                add_text("Width=-100")
                add_drag_float("float##demowidths2", width=-100)

                add_text("Width=-1")
                add_drag_float("float##demowidths3", width=-1)

                add_text("group with width=75")
                with group("##demowidgetWidthgroup", width=75):
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

                add_text("Aligned")
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
                    with tab_bar("Basic Tabbar##demo"):
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
                with group("group##demotabexamples", tip="The group is hovered"):
                    add_button("AAA##demogroup")
                    add_button("BBB##demogroup")
                    add_button("CCC##demogroup")

                add_text("Horizontal group:")
                with group("group##demotabexamples1", horizontal=True):
                    add_button("AAA##demogroup1")
                    add_button("BBB##demogroup1")
                    add_button("CCC##demogroup1")

        with collapsing_header("Tooltips##demo"):
            add_text("Hover me for a fancy tooltip")
            with tooltip("Hover me for a fancy tooltip", "tool_tip##tooltips"):
                add_simple_plot("Simpleplot##tooltips##demo", value=(0.3, 0.9, 2.5, 8.9), height = 80)

        with collapsing_header("Popups, Modal windows, & Dialogs##demo"):

            with tree_node("Popups##demo"):
                add_text("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.")
                add_button("Select..##popups##demo")
                add_same_line()
                add_text("<None>")
                helpmarker("right click for popup, it is set to right mouse button, this can be changed")
                with popup("Select..##popups##demo", "popup1"):
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
                helpmarker("right click for popup, it is set to right mouse button, this can be changed ")
                with popup("Delete..##modals##demo", "Delete?", modal=True):
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

        with collapsing_header("Columns##demo"):

            with tree_node("Basic##columns##demo"):
                add_text("This uses managed columns (add_managed_columns)")
                add_text("Without border:")
                add_separator()
                with managed_columns("columns1##demo", 3, border=False):
                    for i in range(0, 14):
                        add_selectable(f"Item {i}##columns1##demo")
                add_separator()

                add_text("With border:")
                add_separator()
                with managed_columns("columns2##demo", 4):
                    add_text("ID")
                    add_text("Name")
                    add_text("Path")
                    with group("Just to get separator in the same cell##demo"):
                        add_text("Hovered")
                        add_separator()

                    add_selectable("0000##demo", span_columns=False)
                    add_text("One")
                    add_text("/path/one")
                    add_text("0")

                    add_selectable("0001##demo", span_columns=True)
                    add_text("Two")
                    add_text("/path/two")
                    add_text("0")

                    add_selectable("0003##demo", span_columns=True)
                    add_text("Three")
                    add_text("/path/three")
                    add_text("0")
                add_separator()

            with tree_node("Borders##columns##demo"):

                add_text("This uses managed columns (add_managed_columns)")
                with managed_columns("Columns3##demo", 4):

                    def replicated_cell(i):
                        with group(f"replicated_group##{i}##demo"):
                            if i % 4 == 0:
                                add_separator()
                            add_text(f"aaa##{i}")
                            add_input_text(f"##inputcolumns{i}")
                            add_button(f"Button##repl{i}##demo")

                    for i in range(0, 12):
                        replicated_cell(i)
                add_separator()

            with tree_node("Mixed items##columns##demo"):
                add_text("This uses raw columns (add_columns/add_next_column)")
                add_separator()
                add_columns("demo##columns", 3)
                add_text("Hello")
                add_next_column()
                add_text("PyGui")
                add_next_column()
                add_text("Sailer")
                add_next_column()
                add_button("Banana##democolumns")
                add_next_column()
                add_button("Apple##democolumns")
                add_next_column()
                add_button("Corniflower##democolumns")
                add_next_column()
                add_next_column()
                add_input_float("red##democoluns")
                add_next_column()
                add_input_float("blue##democoluns")
                add_next_column()
                add_next_column()
                add_text("An extra line here")
                add_next_column()
                add_next_column()
                with collapsing_header("Category A##democolumns"):
                    add_text("blah blah")
                    add_separator()
                add_next_column()
                with collapsing_header("Category B##democolumns"):
                    add_text("blah blah")
                add_next_column()
                with collapsing_header("Category C##democolumns"):
                    add_text("blah blah")
                add_columns("demo##columnsend", 1)
                add_separator()

        with collapsing_header("Drawings##demo"):

            def fancy_drawing(sender, data):

                if sender == "Draw To Demo Window##demo":
                    if get_value(sender):
                        draw_image("Dear PyGui Demo", "INTERNAL_DPG_FONT_ATLAS", pmin=[225,50], pmax=[300, 300], uv_max=[0.1, 0.1], tag="atlas")
                    else:
                        delete_draw_command("Dear PyGui Demo", "atlas")

                elif sender == "Draw To Foreground##demo":
                    if get_value(sender):
                        draw_image("##FOREGROUND", "INTERNAL_DPG_FONT_ATLAS", pmin=[0,0], pmax=[200, 200], uv_max=[0.1, 0.1], tag="atlas")
                    else:
                        delete_draw_command("##FOREGROUND", "atlas")

                elif sender == "Draw To Background##demo":
                    if get_value(sender):
                        draw_image("##BACKGROUND", "INTERNAL_DPG_FONT_ATLAS", pmin=[0,0], pmax=[200, 200], uv_max=[0.1, 0.1], tag="atlas")
                    else:
                        delete_draw_command("##BACKGROUND", "atlas")

            add_checkbox("Draw To Demo Window##demo", callback=fancy_drawing)
            add_checkbox("Draw To Foreground##demo", callback=fancy_drawing)
            add_checkbox("Draw To Background##demo", callback=fancy_drawing)

            add_drawing("drawing##widget##demo", width=900, height=200)
            draw_rectangle("drawing##widget##demo", (0, 0), (900, 200), (255, 0, 0, 255), fill=(0, 0, 25, 255), 
                           rounding=12, thickness = 1.0, tag="square") 
            draw_line("drawing##widget##demo", (10, 10), (100, 100), (255, 0, 0, 255), 1, tag="line command")
            draw_triangle("drawing##widget##demo", (150, 10), (110, 100), (190, 100), (255, 255, 0, 255), thickness = 3.0)
            draw_quad("drawing##widget##demo", (210, 10), (290, 10), (290, 100), (210, 100), (255, 255, 0, 255), thickness = 3.0)
            draw_circle("drawing##widget##demo", (350, 60), 49, (255, 255, 0, 255))
            draw_bezier_curve("drawing##widget##demo", (410, 10), (450, 25), (410, 50), (490, 85), (255, 255, 0, 255), thickness = 2.0)
            draw_arrow("drawing##widget##demo", (510, 10), (590, 80), (255, 0, 0), 4, 10)
            draw_image("drawing##widget##demo", "INTERNAL_DPG_FONT_ATLAS", pmin=[610,10], pmax=[690, 80], uv_max=[0.1, 0.1])
            draw_text("drawing##widget##demo", (50, 300), "Some Text", color=(255, 255, 0, 255), size=15)
            draw_text("drawing##widget##demo", (0, 0), "Origin", color=(255, 255, 0, 255), size=15)
            draw_polygon("drawing##widget##demo", ((710, 10), (780, 50), (730, 75), (710, 10)), (255, 125, 0, 255), thickness=1.0, fill=(255, 125, 0, 50))
            draw_polyline("drawing##widget##demo", ((810, 20), (835, 50), (890, 10)), (255, 255, 0, 255), thickness=1.0)

        with collapsing_header("Plots##demo"):

            with tree_node("Help##plots##demo"):
                add_text("Plotting User Guide")
                add_text("Left click and drag within the plot area to pan X and Y axes.", bullet=True)
                add_indent()
                add_text("Left click and drag on an axis to pan an individual axis.", bullet=True)
                unindent()

                add_text("Scoll in the plot area to zoom both X and Y axes.", bullet=True)
                add_indent()
                add_text("Scroll on an axis to zoom an individual axis.", bullet=True)
                unindent()

                add_text("Right click and drag to box select data.", bullet=True)
                add_indent()
                add_text("Hold Alt to expand box selection horizontally.", bullet=True)
                add_text("Hold Shift to expand box selection vertically.", bullet=True)
                add_text("Left click while box selecting to cance the selection.", bullet=True)
                unindent()

                add_text("Double left click to fit all visible data.", bullet=True)
                add_indent()
                add_text("Double left click on an axis to fit the individual axis", bullet=True)
                unindent()

                add_text("Double right click to open the plot context menu.", bullet=True)
                add_text("Click legend label icons to show/hide plot items.", bullet=True)
                add_text("IMPORTANT: By default, anti-aliased lines are turned OFF.", bullet=True)

            with tree_node("Line Plots##demo"):

                sindatax = []
                sindatay = []
                for i in range(0, 1000):
                    sindatax.append(i/1000)
                    sindatay.append(0.5 + 0.5*sin(50*i/1000))

                # using xy_format
                x2datax = []
                x2datay = []
                for i in range(0, 100):
                    x2datax.append(1/(i+1))
                    x2datay.append((1/(i+1))**2)

                add_text("Anti-aliasing can be enabled from the plot's context menu (see Help).", bullet=True)
                add_plot("Line Plot##demo", x_axis_name="x", y_axis_name="y", height=400)
                add_line_series("Line Plot##demo", "0.5 + 0.5 * sin(x)", sindatax, sindatay)
                add_line_series("Line Plot##demo", "x^2", x2datax, x2datay)

            with tree_node("Stair Plots##demo"):

                sindatax = []
                sindatay = []
                for i in range(0, 10):
                    sindatax.append(i/10)
                    sindatay.append(0.5 + 0.5*sin(50*i/10))

                # using xy_format
                x2datax = []
                x2datay = []
                for i in range(0, 10):
                    x2datax.append(1/(i+1))
                    x2datay.append((1/(i+1))**2)

                add_text("Anti-aliasing can be enabled from the plot's context menu (see Help).", bullet=True)
                add_plot("Stair Plot##demo", x_axis_name="x", y_axis_name="y", height=400)
                add_stair_series("Stair Plot##demo", "0.5 + 0.5 * sin(x)", sindatax, sindatay)
                add_stair_series("Stair Plot##demo", "x^2", x2datax, x2datay)

            with tree_node("Time Plots##demo"):

                timedatax = []
                timedatay = []
                
                time_index = 0
                while time_index < 739497600:
                    timedatax.append(time_index)
                    timedatay.append(time_index/(60*60*24))
                    time_index+=60*60*24*7
                
                add_text("When time is enabled, x-axis values are interpreted as UNIX timestamps in seconds (e.g. 1599243545).", bullet=True)
                add_text("UNIX timestamps are seconds since 00:00:00 UTC on 1 January 1970", bullet=True)
                add_plot("Time Plot##demo", y_axis_name="Days since 1970", height=400, xaxis_time=True)
                add_line_series("Time Plot##demo", "Days", timedatax, timedatay)
                
            with tree_node("Shade Plots##demo"):

                stock_datax = []
                stock_datay2 = []
                stock_data1 = []
                stock_data2 = []
                stock_data3 = []
                stock_data4 = []
                stock_data5 = []
                for i in range(0, 100):
                    stock_datax.append(i)
                    stock_datay2.append(0)
                    stock_data1.append(400 + 50*abs(random.random()))
                    stock_data2.append(275 + 75*abs(random.random()))
                    stock_data3.append(150 + 75*abs(random.random()))
                    stock_data4.append(500 + 75*abs(random.random()))
                    stock_data5.append(600 + 75*abs(random.random()))

                add_plot("Stock Prices##demo", x_axis_name="Days", y_axis_name="Price", height=400)
                add_line_series("Stock Prices##demo", "Stock 1", stock_datax, stock_data1, color=[0, 0, 255, 255])
                add_line_series("Stock Prices##demo", "Stock 2", stock_datax, stock_data2, color=[255, 0, 0, 255])
                add_line_series("Stock Prices##demo", "Stock 3", stock_datax, stock_data3, color=[0, 255, 0, 255])
                add_shade_series("Stock Prices##demo", "Stock 1", stock_datax, stock_data1, fill=[0, 0, 255, 64])
                add_shade_series("Stock Prices##demo", "Stock 2", stock_datax, stock_data2, fill=[255, 0, 0, 64])
                add_shade_series("Stock Prices##demo", "Stock 3", stock_datax, stock_data3, y2=stock_datay2, fill=[0, 255, 0, 64])
                add_shade_series("Stock Prices##demo", "Shade between lines", stock_datax, stock_data5, y2=stock_data4, fill=[255, 255, 100, 64])

            with tree_node("Scatter Plots##demo"):

                scatter_data1x = []
                scatter_data1y = []
                for i in range(0, 100):
                    scatter_data1x.append(i/100)
                    scatter_data1y.append((i + random.random())/100)

                # using xy_format
                scatter_data2x = []
                scatter_data2y = []
                for i in range(0, 100):
                    scatter_data2x.append(0.25 + 0.25*random.random())
                    scatter_data2y.append(0.65 + 0.25*random.random())

                add_plot("Scatter Plot##demo", height=400)
                add_scatter_series("Scatter Plot##demo", "Data 1", scatter_data1x, scatter_data1y)
                add_scatter_series("Scatter Plot##demo", "Data 2", scatter_data2x, scatter_data2y, 
                                   size=7, marker=mvPlotMarker_Square, fill=[255, 0, 0, 100], xy_data_format=True)

            with tree_node("Bar Plots##demo"):

                add_plot("Bar Plot##demo", x_axis_name="Student", y_axis_name="Score", height=400)
                set_plot_xlimits("Bar Plot##demo", 9, 33)
                set_plot_ylimits("Bar Plot##demo", 0, 110)
                set_xticks("Bar Plot##demo", [["S1", 11], ["S2", 21], ["S3", 31]])

                add_bar_series("Bar Plot##demo", "Final Exam", [10, 20, 30], [100, 75, 90], weight=1)
                add_bar_series("Bar Plot##demo", "Midterm Exam", [11, 21, 31], [83, 75, 72], weight=1)
                add_bar_series("Bar Plot##demo", "Course Grade", [12, 22, 32], [42, 68, 23], weight=1)

            with tree_node("Error Bars##demo"):

                add_plot("##errorbars##demo", height=400)

                error1_x = [1, 2, 3, 4, 5]
                error1_y = [1, 2, 5, 3, 4]
                error1_neg = [0.2, 0.4, 0.2, 0.6, 0.4]
                error1_pos = [0.4, 0.2, 0.4, 0.8, 0.6]

                error2_x = [1, 2, 3, 4, 5]
                error2_y = [8, 8, 9, 7, 8]
                error2_neg = [0.2, 0.4, 0.2, 0.6, 0.4]
                error2_pos = [0.4, 0.2, 0.4, 0.8, 0.6]

                add_bar_series("##errorbars##demo", "Bar", error1_x, error1_y, weight=0.25)
                add_error_series("##errorbars##demo", "Bar", error1_x, error1_y, error1_neg, error1_pos)

                add_line_series("##errorbars##demo", "Line", error2_x, error2_y)
                add_error_series("##errorbars##demo", "Line", error2_x, error2_y, error2_neg, error2_pos, color=[0, 255, 0])

            with tree_node("Stem Plots##demo"):
                add_plot("Stem Plots##plot##demo", height=400)

                stem_datax = []
                stem_data1y = []
                stem_data2y = []
                for i in range(0, 51):
                    stem_datax.append(i)
                    stem_data1y.append(1.0 + 0.5*sin(25*i*0.02)*cos(2*i*0.02))
                    stem_data2y.append(0.5 + 0.25*sin(25*i*0.02)*cos(2*i*0.02))

                add_stem_series("Stem Plots##plot##demo", "Data 1", stem_datax, stem_data1y)
                add_stem_series("Stem Plots##plot##demo", "Data 2", stem_datax, stem_data2y, marker=mvPlotMarker_Diamond)

            with tree_node("Pie Charts##demo"):
                add_plot("##PieChart1##demo", no_mouse_pos=True, 
                         xaxis_no_gridlines=True, xaxis_no_tick_marks=True, xaxis_no_tick_labels=True,
                         yaxis_no_gridlines=True, yaxis_no_tick_marks=True, yaxis_no_tick_labels=True, width=250, height=250)
                add_same_line()
                add_plot("##PieChart2##demo", no_mouse_pos=True, 
                         xaxis_no_gridlines=True, xaxis_no_tick_marks=True, xaxis_no_tick_labels=True,
                         yaxis_no_gridlines=True, yaxis_no_tick_marks=True, yaxis_no_tick_labels=True, 
                         width=250, height=250)
                set_plot_xlimits("##PieChart1##demo", 0, 1)
                set_plot_xlimits("##PieChart2##demo", 0, 1)
                set_plot_ylimits("##PieChart1##demo", 0, 1)
                set_plot_ylimits("##PieChart2##demo", 0, 1)
                add_pie_series("##PieChart1##demo", "PieChart1", [0.25, 0.30, 0.30], ["fish", "cow", "chicken"], 0.5, 0.5, 0.5)
                add_pie_series("##PieChart2##demo", "PieChart2", [1, 1, 2, 3, 5], ["A", "B", "C", "D", "E"], 0.5, 0.5, 0.5, 
                               normalize=True, format="%.0f")
                set_color_map("##PieChart2##demo", mvPlotColormap_Deep)

            with tree_node("Heatmaps##demo"):
                add_plot("Heat Plot##demo", show_color_scale=True, scale_min=0.0, scale_max=6.0, 
                         scale_height=400, no_legend=True, 
                         no_mouse_pos=True, xaxis_lock_min=True, xaxis_lock_max=True, xaxis_no_gridlines=True, xaxis_no_tick_marks=True,
                         yaxis_no_gridlines=True, yaxis_no_tick_marks=True, yaxis_lock_min=True, yaxis_lock_max=True, height=400)
                values = [0.8, 2.4, 2.5, 3.9, 0.0, 4.0, 0.0,
                          2.4, 0.0, 4.0, 1.0, 2.7, 0.0, 0.0,
                          1.1, 2.4, 0.8, 4.3, 1.9, 4.4, 0.0,
                          0.6, 0.0, 0.3, 0.0, 3.1, 0.0, 0.0,
                          0.7, 1.7, 0.6, 2.6, 2.2, 6.2, 0.0,
                          1.3, 1.2, 0.0, 0.0, 0.0, 3.2, 5.1,
                          0.1, 2.0, 0.0, 1.4, 0.0, 1.9, 6.3]
                add_heat_series("Heat Plot##demo", "heat data", values, 7, 7, 0, 6)

            with tree_node("Candle Stick Plots##demo"):
                dates = [1546300800,1546387200,1546473600,1546560000,1546819200,1546905600,1546992000,1547078400,1547164800,1547424000,1547510400,1547596800,1547683200,1547769600,1547942400,1548028800,1548115200,1548201600,1548288000,1548374400,1548633600,1548720000,1548806400,1548892800,1548979200,1549238400,1549324800,1549411200,1549497600,1549584000,1549843200,1549929600,1550016000,1550102400,1550188800,1550361600,1550448000,1550534400,1550620800,1550707200,1550793600,1551052800,1551139200,1551225600,1551312000,1551398400,1551657600,1551744000,1551830400,1551916800,1552003200,1552262400,1552348800,1552435200,1552521600,1552608000,1552867200,1552953600,1553040000,1553126400,1553212800,1553472000,1553558400,1553644800,1553731200,1553817600,1554076800,1554163200,1554249600,1554336000,1554422400,1554681600,1554768000,1554854400,1554940800,1555027200,1555286400,1555372800,1555459200,1555545600,1555632000,1555891200,1555977600,1556064000,1556150400,1556236800,1556496000,1556582400,1556668800,1556755200,1556841600,1557100800,1557187200,1557273600,1557360000,1557446400,1557705600,1557792000,1557878400,1557964800,1558051200,1558310400,1558396800,1558483200,1558569600,1558656000,1558828800,1558915200,1559001600,1559088000,1559174400,1559260800,1559520000,1559606400,1559692800,1559779200,1559865600,1560124800,1560211200,1560297600,1560384000,1560470400,1560729600,1560816000,1560902400,1560988800,1561075200,1561334400,1561420800,1561507200,1561593600,1561680000,1561939200,1562025600,1562112000,1562198400,1562284800,1562544000,1562630400,1562716800,1562803200,1562889600,1563148800,1563235200,1563321600,1563408000,1563494400,1563753600,1563840000,1563926400,1564012800,1564099200,1564358400,1564444800,1564531200,1564617600,1564704000,1564963200,1565049600,1565136000,1565222400,1565308800,1565568000,1565654400,1565740800,1565827200,1565913600,1566172800,1566259200,1566345600,1566432000,1566518400,1566777600,1566864000,1566950400,1567036800,1567123200,1567296000,1567382400,1567468800,1567555200,1567641600,1567728000,1567987200,1568073600,1568160000,1568246400,1568332800,1568592000,1568678400,1568764800,1568851200,1568937600,1569196800,1569283200,1569369600,1569456000,1569542400,1569801600,1569888000,1569974400,1570060800,1570147200,1570406400,1570492800,1570579200,1570665600,1570752000,1571011200,1571097600,1571184000,1571270400,1571356800,1571616000,1571702400,1571788800,1571875200,1571961600]
                opens = [1284.7,1319.9,1318.7,1328,1317.6,1321.6,1314.3,1325,1319.3,1323.1,1324.7,1321.3,1323.5,1322,1281.3,1281.95,1311.1,1315,1314,1313.1,1331.9,1334.2,1341.3,1350.6,1349.8,1346.4,1343.4,1344.9,1335.6,1337.9,1342.5,1337,1338.6,1337,1340.4,1324.65,1324.35,1349.5,1371.3,1367.9,1351.3,1357.8,1356.1,1356,1347.6,1339.1,1320.6,1311.8,1314,1312.4,1312.3,1323.5,1319.1,1327.2,1332.1,1320.3,1323.1,1328,1330.9,1338,1333,1335.3,1345.2,1341.1,1332.5,1314,1314.4,1310.7,1314,1313.1,1315,1313.7,1320,1326.5,1329.2,1314.2,1312.3,1309.5,1297.4,1293.7,1277.9,1295.8,1295.2,1290.3,1294.2,1298,1306.4,1299.8,1302.3,1297,1289.6,1302,1300.7,1303.5,1300.5,1303.2,1306,1318.7,1315,1314.5,1304.1,1294.7,1293.7,1291.2,1290.2,1300.4,1284.2,1284.25,1301.8,1295.9,1296.2,1304.4,1323.1,1340.9,1341,1348,1351.4,1351.4,1343.5,1342.3,1349,1357.6,1357.1,1354.7,1361.4,1375.2,1403.5,1414.7,1433.2,1438,1423.6,1424.4,1418,1399.5,1435.5,1421.25,1434.1,1412.4,1409.8,1412.2,1433.4,1418.4,1429,1428.8,1420.6,1441,1460.4,1441.7,1438.4,1431,1439.3,1427.4,1431.9,1439.5,1443.7,1425.6,1457.5,1451.2,1481.1,1486.7,1512.1,1515.9,1509.2,1522.3,1513,1526.6,1533.9,1523,1506.3,1518.4,1512.4,1508.8,1545.4,1537.3,1551.8,1549.4,1536.9,1535.25,1537.95,1535.2,1556,1561.4,1525.6,1516.4,1507,1493.9,1504.9,1506.5,1513.1,1506.5,1509.7,1502,1506.8,1521.5,1529.8,1539.8,1510.9,1511.8,1501.7,1478,1485.4,1505.6,1511.6,1518.6,1498.7,1510.9,1510.8,1498.3,1492,1497.7,1484.8,1494.2,1495.6,1495.6,1487.5,1491.1,1495.1,1506.4]
                highs = [1284.75,1320.6,1327,1330.8,1326.8,1321.6,1326,1328,1325.8,1327.1,1326,1326,1323.5,1322.1,1282.7,1282.95,1315.8,1316.3,1314,1333.2,1334.7,1341.7,1353.2,1354.6,1352.2,1346.4,1345.7,1344.9,1340.7,1344.2,1342.7,1342.1,1345.2,1342,1350,1324.95,1330.75,1369.6,1374.3,1368.4,1359.8,1359,1357,1356,1353.4,1340.6,1322.3,1314.1,1316.1,1312.9,1325.7,1323.5,1326.3,1336,1332.1,1330.1,1330.4,1334.7,1341.1,1344.2,1338.8,1348.4,1345.6,1342.8,1334.7,1322.3,1319.3,1314.7,1316.6,1316.4,1315,1325.4,1328.3,1332.2,1329.2,1316.9,1312.3,1309.5,1299.6,1296.9,1277.9,1299.5,1296.2,1298.4,1302.5,1308.7,1306.4,1305.9,1307,1297.2,1301.7,1305,1305.3,1310.2,1307,1308,1319.8,1321.7,1318.7,1316.2,1305.9,1295.8,1293.8,1293.7,1304.2,1302,1285.15,1286.85,1304,1302,1305.2,1323,1344.1,1345.2,1360.1,1355.3,1363.8,1353,1344.7,1353.6,1358,1373.6,1358.2,1369.6,1377.6,1408.9,1425.5,1435.9,1453.7,1438,1426,1439.1,1418,1435,1452.6,1426.65,1437.5,1421.5,1414.1,1433.3,1441.3,1431.4,1433.9,1432.4,1440.8,1462.3,1467,1443.5,1444,1442.9,1447,1437.6,1440.8,1445.7,1447.8,1458.2,1461.9,1481.8,1486.8,1522.7,1521.3,1521.1,1531.5,1546.1,1534.9,1537.7,1538.6,1523.6,1518.8,1518.4,1514.6,1540.3,1565,1554.5,1556.6,1559.8,1541.9,1542.9,1540.05,1558.9,1566.2,1561.9,1536.2,1523.8,1509.1,1506.2,1532.2,1516.6,1519.7,1515,1519.5,1512.1,1524.5,1534.4,1543.3,1543.3,1542.8,1519.5,1507.2,1493.5,1511.4,1525.8,1522.2,1518.8,1515.3,1518,1522.3,1508,1501.5,1503,1495.5,1501.1,1497.9,1498.7,1492.1,1499.4,1506.9,1520.9]
                lows = [1282.85,1315,1318.7,1309.6,1317.6,1312.9,1312.4,1319.1,1319,1321,1318.1,1321.3,1319.9,1312,1280.5,1276.15,1308,1309.9,1308.5,1312.3,1329.3,1333.1,1340.2,1347,1345.9,1338,1340.8,1335,1332,1337.9,1333,1336.8,1333.2,1329.9,1340.4,1323.85,1324.05,1349,1366.3,1351.2,1349.1,1352.4,1350.7,1344.3,1338.9,1316.3,1308.4,1306.9,1309.6,1306.7,1312.3,1315.4,1319,1327.2,1317.2,1320,1323,1328,1323,1327.8,1331.7,1335.3,1336.6,1331.8,1311.4,1310,1309.5,1308,1310.6,1302.8,1306.6,1313.7,1320,1322.8,1311,1312.1,1303.6,1293.9,1293.5,1291,1277.9,1294.1,1286,1289.1,1293.5,1296.9,1298,1299.6,1292.9,1285.1,1288.5,1296.3,1297.2,1298.4,1298.6,1302,1300.3,1312,1310.8,1301.9,1292,1291.1,1286.3,1289.2,1289.9,1297.4,1283.65,1283.25,1292.9,1295.9,1290.8,1304.2,1322.7,1336.1,1341,1343.5,1345.8,1340.3,1335.1,1341.5,1347.6,1352.8,1348.2,1353.7,1356.5,1373.3,1398,1414.7,1427,1416.4,1412.7,1420.1,1396.4,1398.8,1426.6,1412.85,1400.7,1406,1399.8,1404.4,1415.5,1417.2,1421.9,1415,1413.7,1428.1,1434,1435.7,1427.5,1429.4,1423.9,1425.6,1427.5,1434.8,1422.3,1412.1,1442.5,1448.8,1468.2,1484.3,1501.6,1506.2,1498.6,1488.9,1504.5,1518.3,1513.9,1503.3,1503,1506.5,1502.1,1503,1534.8,1535.3,1541.4,1528.6,1525.6,1535.25,1528.15,1528,1542.6,1514.3,1510.7,1505.5,1492.1,1492.9,1496.8,1493.1,1503.4,1500.9,1490.7,1496.3,1505.3,1505.3,1517.9,1507.4,1507.1,1493.3,1470.5,1465,1480.5,1501.7,1501.4,1493.3,1492.1,1505.1,1495.7,1478,1487.1,1480.8,1480.6,1487,1488.3,1484.8,1484,1490.7,1490.4,1503.1]
                closes = [1283.35,1315.3,1326.1,1317.4,1321.5,1317.4,1323.5,1319.2,1321.3,1323.3,1319.7,1325.1,1323.6,1313.8,1282.05,1279.05,1314.2,1315.2,1310.8,1329.1,1334.5,1340.2,1340.5,1350,1347.1,1344.3,1344.6,1339.7,1339.4,1343.7,1337,1338.9,1340.1,1338.7,1346.8,1324.25,1329.55,1369.6,1372.5,1352.4,1357.6,1354.2,1353.4,1346,1341,1323.8,1311.9,1309.1,1312.2,1310.7,1324.3,1315.7,1322.4,1333.8,1319.4,1327.1,1325.8,1330.9,1325.8,1331.6,1336.5,1346.7,1339.2,1334.7,1313.3,1316.5,1312.4,1313.4,1313.3,1312.2,1313.7,1319.9,1326.3,1331.9,1311.3,1313.4,1309.4,1295.2,1294.7,1294.1,1277.9,1295.8,1291.2,1297.4,1297.7,1306.8,1299.4,1303.6,1302.2,1289.9,1299.2,1301.8,1303.6,1299.5,1303.2,1305.3,1319.5,1313.6,1315.1,1303.5,1293,1294.6,1290.4,1291.4,1302.7,1301,1284.15,1284.95,1294.3,1297.9,1304.1,1322.6,1339.3,1340.1,1344.9,1354,1357.4,1340.7,1342.7,1348.2,1355.1,1355.9,1354.2,1362.1,1360.1,1408.3,1411.2,1429.5,1430.1,1426.8,1423.4,1425.1,1400.8,1419.8,1432.9,1423.55,1412.1,1412.2,1412.8,1424.9,1419.3,1424.8,1426.1,1423.6,1435.9,1440.8,1439.4,1439.7,1434.5,1436.5,1427.5,1432.2,1433.3,1441.8,1437.8,1432.4,1457.5,1476.5,1484.2,1519.6,1509.5,1508.5,1517.2,1514.1,1527.8,1531.2,1523.6,1511.6,1515.7,1515.7,1508.5,1537.6,1537.2,1551.8,1549.1,1536.9,1529.4,1538.05,1535.15,1555.9,1560.4,1525.5,1515.5,1511.1,1499.2,1503.2,1507.4,1499.5,1511.5,1513.4,1515.8,1506.2,1515.1,1531.5,1540.2,1512.3,1515.2,1506.4,1472.9,1489,1507.9,1513.8,1512.9,1504.4,1503.9,1512.8,1500.9,1488.7,1497.6,1483.5,1494,1498.3,1494.1,1488.1,1487.5,1495.7,1504.7,1505.3]

                add_plot("Candle Plot##demo", x_axis_name="Day", y_axis_name="USD", height=400, xaxis_time=True)
                add_candle_series("Candle Plot##demo", "GOOGL", dates, opens, highs, lows, closes)

            with tree_node("Multiple Axes##demo"):

                sindatax = []
                sindatay = []
                for i in range(0, 1000):
                    sindatax.append(i/1000)
                    sindatay.append(0.5 + 0.5*sin(50*i/1000))

                # using xy_format
                x2datax = []
                x2datay = []
                for i in range(0, 100):
                    x2datax.append(1/(i+1))
                    x2datay.append((1/(i+1))**2)

                add_plot("Multi Axes Plot##demo", x_axis_name="x", y_axis_name="y", height=400, yaxis2=True, yaxis3=True)
                add_line_series("Multi Axes Plot##demo", "Series 1", sindatax, sindatay)
                add_line_series("Multi Axes Plot##demo", "Series 2", x2datax, x2datay, axis=1)
                add_line_series("Multi Axes Plot##demo", "Series 3", x2datax, x2datay, axis=2)

            with tree_node("Annotations##demo"):
                
                add_plot("Annotations##plotsdemo", height=400)
                add_annotation("Annotations##plotsdemo", "BL", 0.25, 0.25, -15, 15, color=[255, 255, 0, 255])
                add_annotation("Annotations##plotsdemo", "BR", 0.75, 0.25, 15, 15, color=[255, 255, 0, 255])
                add_annotation("Annotations##plotsdemo", "TR not clampled", 0.75, 0.75, -15, -15, color=[255, 255, 0, 255], clamped=False)
                add_annotation("Annotations##plotsdemo", "TL", 0.25, 0.75, -15, -15, color=[255, 255, 0, 255])
                add_annotation("Annotations##plotsdemo", "Center", 0.5, 0.5, 0, 0, color=[255, 255, 0, 255])

            with tree_node("Infinite Lines##demo"):

                infinite_x_data = [3, 5, 6, 7]
                infinite_y_data = [3, 5, 6, 7]

                add_plot("Infinite Line Plot##demo", height=400)
                add_vline_series("Infinite Line Plot##demo", "vertical", infinite_x_data)
                add_hline_series("Infinite Line Plot##demo", "horizontal", infinite_y_data)

            with tree_node("Drag Lines and Points##demo"):
                
                add_plot("##dragplotsdemo", height=400)
                add_drag_line("##dragplotsdemo", "dline1", source="floatpp", color=[255, 0, 0, 255])
                add_drag_line("##dragplotsdemo", "dline2", source="float4pp", color=[255, 255, 0, 255], y_line=True)
                add_drag_point("##dragplotsdemo", "dpoint1", source="float2pp", color=[255, 0, 255, 255])

            with tree_node("Querying##demo"):
                add_text("Click and drag the middle mouse button!")
                def query(sender, data):
                    set_plot_xlimits("Plot2##demoquery", data[0], data[1])
                    set_plot_ylimits("Plot2##demoquery", data[2], data[3])

                sindatax = []
                sindatay = []
                for i in range(0, 1000):
                    sindatax.append(i/1000)
                    sindatay.append(0.5 + 0.5*sin(50*i/1000))
     
                add_plot("Plot1##demoquery", height=400, query_callback=query, query=True, no_menus=True)
                add_plot("Plot2##demoquery", height=400, query_callback=query, no_menus=True, no_legend=True)
                add_line_series("Plot1##demoquery", "0.5 + 0.5 * sin(x)", sindatax, sindatay)
                add_line_series("Plot2##demoquery", "0.5 + 0.5 * sin(x)", sindatax, sindatay)

            with tree_node("Image Plots##demo"):

                add_plot("Image Plot##demo", height=400)
                add_image_series("Image Plot##demo", "images", "INTERNAL_DPG_FONT_ATLAS", [100, 100], [200, 200])

            with tree_node("Custom Rendering Plots##demo"):
                add_plot("Custom Rendering Plot##demo", height=400)
                draw_line("Custom Rendering Plot##demo", (10, 10), (100, 100), (255, 0, 0, 255), 1, tag="line command")
                draw_triangle("Custom Rendering Plot##demo", (150, 10), (110, 100), (190, 100), (255, 255, 0, 255), thickness = 3.0)
                draw_quad("Custom Rendering Plot##demo", (210, 10), (290, 10), (290, 100), (210, 100), (255, 255, 0, 255), thickness = 3.0)
                draw_circle("Custom Rendering Plot##demo", (350, 60), 49, (255, 255, 0, 255))

        with collapsing_header("Simple Tables##demo"):

            add_button("Delete row 6##demo", callback=lambda sender, data: delete_row("Table##widget##demo", 6))
            add_same_line()
            add_button("Delete col 1##demo", callback=lambda sender, data: delete_column("Table##widget##demo", 1))   
            add_same_line()
            add_button("Add row##demo", callback=lambda sender, data: add_row("Table##widget##demo", ["new1", "new2", "new3", 53]))
            add_same_line()
            add_button("Add col##demo", callback=lambda sender, data: add_column("Table##widget##demo", "New Column", ["new1", "new2", "new3", "new4"]))
            add_same_line()
            add_button("Insert row 5##demo", callback=lambda sender, data: insert_row("Table##widget##demo", 5, ["inew1", "inew2", "inew3", "inew4"]))
            add_same_line()
            add_button("Insert col 1##demo", callback=lambda sender, data:insert_column("Table##widget##demo", 1,  "Inserted Column", ["inew1", "inew2", "inew3", "inew4"]))
            add_same_line()
            add_button("Clear Table##demo", callback=lambda sender, data: clear_table("Table##widget##demo"))
            add_same_line()
            add_button("Hide/Show Headers##demo", 
                       callback=lambda sender, data: configure_item("Table##widget##demo", hide_headers=data),
                       callback_data=lambda:not get_item_configuration("Table##widget##demo")["hide_headers"])
            add_table("Table##widget##demo", ["Column 1", "Column 2", "Column 3", "Column 4"], height=400)

            tabledata = []
            for i in range(0, 10):
                row = []
                for j in range(0, 4):
                    row.append("Item"+str(i)+"-"+str(j))
                tabledata.append(row)

            set_table_data("Table##widget##demo", tabledata)

        with collapsing_header("Node Editor##demo"):

            def add_new_link(sender, data):
                add_node_link("Node Editor 1##demo", "Node A2##demo", "Node A3##demo")

            def delete_new_link(sender, data):
                delete_node_link("Node Editor 1##demo", "Node A2##demo", "Node A3##demo")

            def get_link_info(sender, data):
                print("Selected Nodes: ", get_selected_nodes("Node Editor 1##demo"))
                print("Selected Links: ",get_selected_links("Node Editor 1##demo"))
                print("Links: ", get_links("Node Editor 1##demo"))

            def clear_stuff(sender, data):
                clear_selected_nodes("Node Editor 1##demo")
                clear_selected_links("Node Editor 1##demo")

            def link_callback(sender, data):
                print(data)

            def delink_callback(sender, data):
                print(data)

            add_text("Ctrl+Click to remove a link.", bullet=True)
            with managed_columns("Node Editor Columns##demo", 4):
                add_button("New Link##demo", callback=add_new_link)
                add_button("Delete Link##demo", callback=delete_new_link)
                add_button("Get Info##demo", callback=get_link_info)
                add_button("Clear Selections##demo", callback=clear_stuff)

            with node_editor("Node Editor 1##demo", link_callback=link_callback, delink_callback=delink_callback):

                with node("Node 1##demo", x_pos=10, y_pos=10):

                    with node_attribute("Node A1##demo"):
                        add_input_float("F1##demo", width=150)

                    with node_attribute("Node A2##demo", output=True):
                        add_input_float("F2##demo", width=150)

                with node("Node 2##demo", x_pos=300, y_pos=10, label="Pizza"):

                    with node_attribute("Node A3##demo"):
                        add_input_float("F3##demo", width=200)

                    with node_attribute("Node A4##demo", output=True):
                        add_input_float("F4##demo", width=200)

                with node("Node 3##demo", x_pos=25, y_pos=150):                   
                    with node_attribute("Node A5##demo"):
                        add_input_text("T5##demo", width=200)
                    with node_attribute("Node A6##demo", static=True): 
                        add_simple_plot("Node Plot##demo", value=(0.3, 0.9, 2.5, 8.9), width=200, height=80, histogram=True)

        with collapsing_header("Logging##demo"):

            with tree_node("System Logger##demo"):

                def LogCallback1(sender, data):
                    show_logger()
                    loglevel = get_value("Log Level##logging##demo")
                    set_log_level(loglevel)
                    log("Trace Message")
                    log_debug("Debug Message")
                    log_info("Info Message")
                    log_warning("Warning Message")
                    log_error("Error Message")

                add_button("Test Logger##demo", callback=LogCallback1)
                add_same_line(spacing=10)
                with group("LoggingGroup##demo"):
                    add_text("Log Level##demo", default_value="Log Level")
                    add_radio_button("Log Level##logging##demo", items=("Trace", "Debug", "Info", "Warning", "Error", "Off"))

            with tree_node("Logger Widgets##demo"):
                def LogCallback2(sender, data):
                    loglevel = get_value("Log Level##2logging##demo")
                    set_log_level(loglevel, logger="LoggerWidget##demo")
                    log("Trace Message", logger="LoggerWidget##demo")
                    log_debug("Debug Message", logger="LoggerWidget##demo")
                    log_info("Info Message", logger="LoggerWidget##demo")
                    log_warning("Warning Message", logger="LoggerWidget##demo")
                    log_error("Error Message", logger="LoggerWidget##demo")

                def MoveLoggerToWindow(sender, data):
                    configure_item("LoggerWidget##demo", autosize_y=True)
                    if not does_item_exist("Logging Widget On Window##demo"):
                        with window("Logging Widget On Window##demo"):
                            pass
                    else:
                        show_item("Logging Widget On Window##demo")

                    move_item("LoggerWidget##demo", parent="Logging Widget On Window##demo")

                def MoveLoggerToDemo(sender, data):
                    configure_item("LoggerWidget##demo", autosize_y=False, height=200)
                    move_item("LoggerWidget##demo", parent="Logger Widgets##demo")
                    hide_item("Logging Widget On Window##demo")

                with group("LeftLogGroup##demo"):
                    add_button("Detach##demo2", callback=MoveLoggerToWindow)
                    add_button("Reattach##demo2", callback=MoveLoggerToDemo)
                    add_button("Test Logger##demo2", callback=LogCallback2)
                    add_radio_button("Log Level##2logging##demo", items=("Trace", "Debug", "Info", "Warning", "Error", "Off"))
                add_same_line()
                add_logger("LoggerWidget##demo", autosize_x=True, height=200, auto_scroll=True)

        with collapsing_header("Filtering##demo"):
            add_text("This section is not ready! But will completed sometime during the 0.7.x releases!")

        with collapsing_header("Inputs, Navigation, & Focus##demo"):

            with tree_node("Polling##demoinputs"):
                add_text("Key Polling:")
                add_label_text("A key Down##demo", default_value="False", color=(0,200,255))
                add_label_text("W key Pressed##demo", default_value="False", color=(0,200,255))
                add_label_text("Q key Released##demo", default_value="False", color=(0,200,255))
                add_spacing()
                add_text("Mouse Polling:")
                add_label_text("Mouse Position##demo", default_value="(0,0)", color=(0,200,255))
                add_label_text("Drawing Mouse Position##demo", default_value="(0,0)", color=(0,200,255))
                add_label_text("Plot Mouse Position##demo", default_value="(0,0)", color=(0,200,255))
                add_label_text("Left Mouse Dragging##demo", default_value="False", color=(0,200,255))
                add_label_text("Middle Mouse Dragging##demo", default_value="False", color=(0,200,255))
                add_label_text("Right Mouse Dragging##demo", default_value="False", color=(0,200,255))
                add_label_text("Left Mouse Clicked##demo", default_value="False", color=(0,200,255))
                add_label_text("Middle Mouse Clicked##demo", default_value="False", color=(0,200,255))
                add_label_text("Right Mouse Clicked##demo", default_value="False", color=(0,200,255))
                add_label_text("Left Mouse Double Clicked##demo", default_value="False", color=(0,200,255))
                add_label_text("Middle Mouse Double Clicked##demo", default_value="False", color=(0,200,255))
                add_label_text("Right Mouse Double Clicked##demo", default_value="False", color=(0,200,255))
                add_label_text("Left Mouse Down##demo", default_value="False", color=(0,200,255))
                add_label_text("Middle Mouse Down##demo", default_value="False", color=(0,200,255))
                add_label_text("Right Mouse Down##demo", default_value="False", color=(0,200,255))
                add_label_text("Left Mouse Released##demo", default_value="False", color=(0,200,255))
                add_label_text("Middle Mouse Released##demo", default_value="False", color=(0,200,255))
                add_label_text("Right Mouse Released##demo", default_value="False", color=(0,200,255))

            with tree_node("Event Callbacks##demoinputs"):
                add_text("Note: these only show the last event!")
                set_mouse_down_callback(lambda sender, data: set_value("Mouse down##demoevents", data))
                set_mouse_drag_callback(lambda sender, data: set_value("Mouse drag##demoevents", data), 10)
                set_mouse_move_callback(lambda sender, data: set_value("Mouse pos##demoevents", data))
                set_mouse_double_click_callback(lambda sender, data: set_value("Mouse dblclick##demoevents", data))
                set_mouse_click_callback(lambda sender, data: set_value("Mouse clicked##demoevents", data))
                set_mouse_release_callback(lambda sender, data: set_value("Mouse released##demoevents", data))
                set_mouse_wheel_callback(lambda sender, data: set_value("Mouse wheel##demoevents", data))
                set_key_down_callback(lambda sender, data: set_value("Keys down##demoevents", data))
                set_key_press_callback(lambda sender, data: set_value("Keys pressed##demoevents", data))
                set_key_release_callback(lambda sender, data: set_value("Keys released##demoevents", data))

                add_label_text("Mouse pos##demoevents")
                add_label_text("Mouse drag##demoevents")
                add_label_text("Mouse down##demoevents")
                add_label_text("Mouse clicked##demoevents")
                add_label_text("Mouse dblclick##demoevents")
                add_label_text("Mouse released##demoevents")
                add_label_text("Mouse wheel##demoevents")
                
                add_label_text("Keys down##demoevents")
                add_label_text("Keys pressed##demoevents")
                add_label_text("Keys released##demoevents")

        with collapsing_header("Collapsing Header##demo", default_open=True, closable = True):
            add_text("This header is just to demonstrate default_open and closable.")
