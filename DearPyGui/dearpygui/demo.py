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

def hsv_to_rgb(h, s, v):
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
    if is_mouse_button_dragging(mvMouseButton_Left, 10):
        set_value("Left Mouse Dragging##demo", "True")
    else:
        set_value("Left Mouse Dragging##demo", "False")

    if is_mouse_button_dragging(mvMouseButton_Right, 10):
        set_value("Right Mouse Dragging##demo", "True")
    else:
        set_value("Right Mouse Dragging##demo", "False")

    if is_mouse_button_dragging(mvMouseButton_Middle, 10):
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
        set_value("Left Mouse Clicked", "False")

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

def show_demo():

    set_render_callback(demo_main_callback)

    def on_demo_close(sender, data):
        delete_item("Dear PyGui Demo")
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

    with window("Dear PyGui Demo", x_pos=100, y_pos=100, width=800, height=800, on_close=on_demo_close):

        with menu_bar("MenuBar##demo"):

            with menu("Themes##demo"):
                add_menu_item("Dark", callback = lambda sender, data: set_theme(sender), check=True,shortcut="Ctrl + D")
                add_menu_item("Light", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Classic", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Dark 2", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Grey", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Dark Grey", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Cherry", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Purple", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Gold", callback = lambda sender, data: set_theme(sender), check=True)
                add_menu_item("Red", callback = lambda sender, data: set_theme(sender), check=True)

            with menu("Tools##demo"):
                add_menu_item("Show Logger##demo", callback=show_logger)
                add_menu_item("Show About##demo", callback=show_about)
                add_menu_item("Show Metrics##demo", callback=show_metrics)
                add_menu_item("Show Documentation##demo", callback=show_documentation)
                add_menu_item("Show Debug##demo", callback=show_debug)
                add_menu_item("Show Style Editor##demo", callback=show_style_editor)

        add_text(f'Dear PyGui says hello. ({get_dearpygui_version()})')
        add_text("This demo is not complete but will continue to be added to throughout the 0.4.x releases!")

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

                add_button("Button##demo")
                add_checkbox("checkbox##demo")
                add_radio_button("radiobutton##demo", items=["radio a", "radio b", "radio c"], horizontal=True)
        
                for i in range(0, 7):
                    if i > 0:
                        add_same_line()
                    add_button(f"Click##{i}")
                    set_item_color(f"Click##{i}", mvGuiCol_Button, hsv_to_rgb(i/7.0, 0.6, 0.6))
                    set_item_color(f"Click##{i}", mvGuiCol_ButtonHovered, hsv_to_rgb(i/7.0, 0.7, 0.7))
                    set_item_color(f"Click##{i}", mvGuiCol_ButtonActive, hsv_to_rgb(i/7.0, 0.8, 0.8))

                add_text("Press a button: ")
                add_same_line()
                add_button("Left##demo", arrow=True, direction=mvDir_Left, 
                           callback=lambda sender, data: set_value("value", int(get_value("value"))-1))
                add_same_line()
                add_button("Right##demo", arrow=True, direction=mvDir_Right,
                           callback=lambda sender, data: set_value("value", int(get_value("value"))+1))
                add_same_line()
                add_text("value")
                set_value("value", "0")

                add_separator()

                add_label_text("label##demo", value="Value")
                add_combo("combo##demo", items=["AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK"], 
                          default_value="AAAA")
                add_input_text("input text##demo", default_value="Hello, world!")
                helpmarker(
                        "USER:\n"
                        "Hold SHIFT or use mouse to select text.\n"
                        "CTRL+Left/Right to word jump.\n"
                        "CTRL+A or double-click to select all.\n"
                        "CTRL+X,CTRL+C,CTRL+V clipboard.\n"
                        "CTRL+Z,CTRL+Y undo/redo.\n"
                        "ESCAPE to revert.\n\n")
                add_input_text("input text (w/ hint)##demo", hint="enter text here")
                add_input_int("input int##demo")
                add_input_float("input float##demo")
                add_input_float("input scientific##demo", format="%e")
                add_input_float3("input float3##example##demo")
                add_drag_int("drag int")
                helpmarker(
                        "Click and drag to edit value.\n"
                        "Hold SHIFT/ALT for faster/slower edit.\n"
                        "Double-click or CTRL+click to input value.")
                add_drag_int("drag int 0..100##demo", format="%d%%")
                add_drag_float("drag float##demo")
                add_drag_float("drag small float##demo", default_value=0.0067, format="%.06f ns")
                add_slider_int("slider int##demo", max_value=3)
                helpmarker("CTRL+click to enter value.")
                add_slider_float("slider float##demo", max_value=1.0, format="ratio = %.3f")
                add_slider_int("slider angle##demo", min_value=-360, max_value=360, format="%d deg")
                add_color_edit3("color 1##demo", default_value=[255, 0, 51])
                helpmarker(
                        "Click on the colored square to open a color picker.\n"
                        "Click and hold to use drag and drop.\n"
                        "Right-click on the colored square to show options.\n"
                        "CTRL+click on individual component to input value.\n")
                add_color_edit4("color 2##demo", default_value=[102, 179, 0, 128])
                add_listbox("listbox##demo", items=["Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"], num_items=4)

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
                add_image("image##demo", "INTERNAL_DPG_FONT_ATLAS", uv_max=[1, 1])
                add_text("Here is an image button using a portion of the font atlas")
                add_image_button("#image##button1", "INTERNAL_DPG_FONT_ATLAS", uv_max=[0.1, 0.1])
                add_same_line()
                add_image_button("#image##button2", "INTERNAL_DPG_FONT_ATLAS", uv_min=[0.1, 0.1], uv_max=[0.2, 0.2])

            with tree_node("Text Input##demo"):
        
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
                        "\tlock cmpxchg8b eax\n", height=300)

                with tree_node("Filtered Text Input##demo"):
                    add_input_text("default##demo")
                    add_input_text("decimal##demo", decimal=True)
                    add_input_text("hexdecimal##demo", hexadecimal=True)
                    add_input_text("uppercase##demo", uppercase=True)
                    add_input_text("no blank##demo", no_spaces=True)
                    add_input_text("scientific##demo", scientific=True)
            
                with tree_node("Password Input##demo"):
                    add_input_text("password##demo", password=True, source="password")
                    add_input_text("password (w/ hint)##demo", password=True, hint="<password>", source="password")
                    add_input_text("password (clear)##demo", source="password")

            with tree_node("Simple Plot Widgets##demo"):
                add_simple_plot("Frame Times##demo", value=[0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2])
                add_simple_plot("Histogram##demo", value=(0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2), height=80, histogram=True, minscale=0.0)

                data1 = []
                for i in range(0, 70):
                    data1.append(cos(3.14*6*i/180))
                add_simple_plot("Lines##sin##demo", value=data1, height=80)
                add_simple_plot("Histogram##sin##demo", value=data1, height=80, histogram=True)
                add_progress_bar("Progress Bar##demo", value=0.78, overlay="78%")
                add_same_line()
                add_text("Progress Bar##text##demo")
                set_value("Progress Bar##text##demo", "Progress Bar")
                add_progress_bar("##Progress Bar##demo", value=0.78, overlay="1367/1753")

            with tree_node("Color/Picker Widgets##demo"):
                # helper for configuring items

                #def configure_items(names, **kwargs):
                #    for name in names:
                #        configure_item(name, **kwargs)
                #
                #add_checkbox("With Alpha Preview", callback=lambda sender, data: configure_items(color_edit_names, alpha_preview = get_value(sender)))
                #add_checkbox("With Half Alpha Preview", callback=lambda sender, data: configure_items(color_edit_names, alpha_preview_half = get_value(sender)))
                #add_checkbox("With No Drag and Drop", callback=lambda sender, data: configure_items(color_edit_names, no_drag_drop = get_value(sender)))
                #helpmarker("Click and drag a preview square, drop on another color widget to apply the color")
                #add_checkbox("With No Options Menu", callback=lambda sender, data: configure_items(color_edit_names, no_options = get_value(sender)))
                #helpmarker("Right clicking a color widget brings up an options context menu")
                #add_checkbox("With No Small Preview", callback=lambda sender, data: configure_items(color_edit_names, no_small_preview = get_value(sender)))
                #add_checkbox("With No Inputs", callback=lambda sender, data: configure_items(color_edit_names, no_inputs = get_value(sender)))
                #add_checkbox("With No Tooltip", callback=lambda sender, data: configure_items(color_edit_names, no_tooltip = get_value(sender)))
                #add_checkbox("With RGB", callback=lambda sender, data: configure_items(color_edit_names, display_rgb = get_value(sender)))
                #add_checkbox("With HSV", callback=lambda sender, data: configure_items(color_edit_names, display_hsv = get_value(sender)))
                #add_checkbox("With HEX", callback=lambda sender, data: configure_items(color_edit_names, display_hex = get_value(sender)))
                #add_checkbox("With Ints", callback=lambda sender, data: configure_items(color_edit_names, uint8 = get_value(sender)))
                #add_checkbox("With Floats", callback=lambda sender, data: configure_items(color_edit_names, floats = get_value(sender)))
                #helpmarker("Right-click on the individual color widget to show options.")
                #
                #add_value("colorvalue", [0.0, 0.0, 0.0, 0.0])
                #color_edit_names = ["MyColor##1", "MyColor##2", "MyColor##3"]
                #add_text("Color Widget:")
                #add_color_edit3(color_edit_names[0], source="colorvalue")
                #
                #add_text("Color Widget HSV with Alpha:")
                #add_color_edit4(color_edit_names[1], source="colorvalue", display_hsv=True, alpha_preview=True)
                #add_text("Color button with Picker:")
                #helpmarker("using no inputs and no label leaves only the preview\n"
                #           "click the color edit preview will reveal the color picker.")
                #add_color_edit4("Color Edit 4##2", source="colorvalue", no_inputs=True, no_label=True)
                #
                #add_text("Color button with Custom Picker Popup:")
                #add_color_edit4("Color Edit 4 (with custom popup)", source="colorvalue", no_inputs=True, no_picker=True, popup="custom picker popup")
                #helpmarker("we can override the popup with our own custom popup that includes a color pallet")
                #with popup("Color Edit 4 (with custom popup)", "custom picker popup", mousebutton=0):
                #    add_color_picker4("custom picker", no_tooltip=True, picker_hue_wheel=True)
                #    add_text("Color Pallet")
                #    for i in range(30):
                #        add_color_button(f"color button {i}", hsv_to_rgb(i/30,1,1))
                #        if i<9: add_same_line()
                #        if i>9 and i<19: add_same_line()
                #        if i>19 and i<29: add_same_line()
                #
                #add_text("Color button only:")
                add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")

            with tree_node("Multi-component Widgets##demo"):
        
                add_input_float2("input float2##demo", source="float2")
                add_drag_float2("drag float2##demo", source="float2")
                add_slider_float2("slider float2##demo", source="float2")
                add_input_int2("input int2##demo", source="int2")
                add_drag_int2("drag int2##demo", source="int2")
                add_slider_int2("slider int2##demo", source="int2")
                add_spacing()
                add_input_float3("input float3##demo", source="float3")
                add_drag_float3("drag float3##demo", source="float3")
                add_slider_float3("slider float3##demo", source="float3")
                add_input_int3("input int3##demo", source="int3")
                add_drag_int3("drag int3##demo", source="int3")
                add_slider_int3("slider int3##demo", source="int3")
                add_spacing()
                add_input_float4("input float4##demo", source="float4")
                add_drag_float4("drag float4##demo", source="float4")
                add_slider_float4("slider float4##demo", source="float4")
                add_input_int4("input int4##demo", source="int4")
                add_drag_int4("drag int4##demo", source="int4")
                add_slider_int4("slider int4##demo", source="int4")

            with tree_node("Vertical Sliders##demo"):

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

            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")

            with tree_node("Child windows##demo"):

                with child("child##demo", border=False, width=400):
                    for i in range(0, 100):
                        add_text(f"{i}: scrollable region")

                add_same_line()
                with child("decorated child##demo", width=0):
                    with menu_bar("MenuBarChild##demo"):
                        with menu("Menu##child##demo"):
                            pass
                    with group("decorated child group##demo", width=-20):
                        for i in range(0, 100):
                            add_button(f"{i}##childbutton##demo")

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
                with popup("Delete..##modals##demo", "Delete?", modal=True):
                    add_text("All those beautiful files will be deleted.\nThis operation cannot be undone!")
                    add_separator()
                    add_checkbox("Don't ask me next time##demo")
                    add_button("OK##modal##demo", width=75, callback=lambda sender, data: close_popup())
                    add_same_line()
                    add_button("Cancel##modal##demo", width=75, callback=lambda sender, data: close_popup())

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

                    add_selectable("0000##demo", span_columns=True)
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
            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")
            def UpdateDrawing(sender, data):
                set_drawing_origin("drawing##widget##demo", get_value("X Origin##demo"), get_value("Y Origin##demo"))
                set_drawing_scale("drawing##widget##demo", get_value("X Scale##demo"), get_value("Y Scale##demo"))

            with group("Drawing Controls Group##demo"):
                add_slider_float("X Origin##demo", vertical=True, min_value = -100, max_value=100, default_value=0, callback=UpdateDrawing)
                add_same_line(spacing=20)
                add_slider_float("Y Origin##demo", vertical=True, min_value = -100, max_value=100, default_value=0, callback=UpdateDrawing)
                add_slider_float("X Scale##demo", vertical=True, max_value=10, default_value=1, callback=UpdateDrawing)
                add_same_line(spacing=20)
                add_slider_float("Y Scale##demo", vertical=True, max_value=10, default_value=1, callback=UpdateDrawing)
            add_same_line(spacing=20)
            add_drawing("drawing##widget##demo", width=800, height=500)
            draw_rectangle("drawing##widget##demo", (0, 500), (800, 0), (255, 0, 0, 255), fill=(0, 0, 25, 255), rounding=12, thickness = 1.0)
            draw_line("drawing##widget##demo", (10, 10), (100, 100), (255, 0, 0, 255), 1)
            draw_triangle("drawing##widget##demo", (300, 500), (200, 200), (500, 200), (255, 255, 0, 255), thickness = 3.0)
            draw_quad("drawing##widget##demo", (50, 50), (150, 50), (150, 150), (50, 150), (255, 255, 0, 255), thickness = 3.0)
            draw_text("drawing##widget##demo", (50, 300), "Some Text", color=(255, 255, 0, 255), size=15)
            draw_text("drawing##widget##demo", (0, 0), "Origin", color=(255, 255, 0, 255), size=15)
            draw_circle("drawing##widget##demo", (400, 250), 50, (255, 255, 0, 255))
            draw_polyline("drawing##widget##demo", ((320, 490), (185, 200), (500, 710)), (255, 255, 0, 255), thickness=1.0)
            draw_polygon("drawing##widget##demo", ((363, 471), (153, 498), (59, 220), (363, 471)), (255, 125, 0, 255), thickness=1.0, fill=(255, 125, 0, 50))
            draw_bezier_curve("drawing##widget##demo", (50, 200), (150, 250), (300, 150), (600, 250), (255, 255, 0, 255), thickness = 2.0)
            draw_arrow("drawing##widget##demo", (50, 70), (100, 65), (0, 200, 255), 1, 10)
            draw_image("drawing##widget##demo", "INTERNAL_DPG_FONT_ATLAS", pmin=[0,400], uv_max=[0.1, 0.1])

        with collapsing_header("Plots##demo"):

            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")

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

                sindata = []
                for i in range(0, 1000):
                    #sindata.append([3.14*i/180, 0.5+ 0.5*cos(50*3.14*i/180)])
                    sindata.append([i/1000, 0.5 + 0.5*sin(50*i/1000)])

                x2data = []
                for i in range(0, 100):
                    x2data.append([1/(i+1), (1/(i+1))**2])

                add_text("Anti-aliasing can be enabled from the plot's context menu (see Help).", bullet=True)
                add_plot("Line Plot##demo", x_axis_name="x", y_axis_name="y", height=400)
                add_line_series("Line Plot##demo", "0.5 + 0.5 * sin(x)", sindata)
                add_line_series("Line Plot##demo", "x^2", x2data)

            with tree_node("Time Plots##demo"):

                timedata = []
                
                time_index = 0
                while time_index < 739497600:
                    timedata.append([time_index, time_index/(60*60*24)])
                    time_index+=60*60*24*7
                
                add_text("When time is enabled, x-axis values are interpreted as UNIX timestamps in seconds (e.g. 1599243545).", bullet=True)
                add_text("UNIX timestamps are seconds since 00:00:00 UTC on 1 January 1970", bullet=True)
                add_plot("Time Plot##demo", y_axis_name="Days since 1970", height=400, xaxis_time=True)
                add_line_series("Time Plot##demo", "Days", timedata)
                

            with tree_node("Shade Plots##demo"):

                stock_data1 = []
                stock_data2 = []
                stock_data3 = []
                for i in range(0, 100):
                    stock_data1.append([i, 400 + 50*abs(random.random())])
                    stock_data2.append([i, 275 + 75*abs(random.random())])
                    stock_data3.append([i, 150 + 75*abs(random.random())])

                add_plot("Stock Prices##demo", x_axis_name="Days", y_axis_name="Price", height=400)
                add_line_series("Stock Prices##demo", "Stock 1", stock_data1, color=[0, 0, 255, 255])
                add_line_series("Stock Prices##demo", "Stock 2", stock_data2, color=[255, 0, 0, 255])
                add_line_series("Stock Prices##demo", "Stock 3", stock_data3, color=[0, 255, 0, 255])
                add_shade_series("Stock Prices##demo", "Stock 1", stock_data1, fill=[0, 0, 255, 64])
                add_shade_series("Stock Prices##demo", "Stock 2", stock_data2, fill=[255, 0, 0, 64])
                add_shade_series("Stock Prices##demo", "Stock 3", stock_data3, fill=[0, 255, 0, 64])

            with tree_node("Scatter Plots##demo"):

                scatter_data1 = []
                scatter_data2 = []
                for i in range(0, 100):
                    scatter_data1.append([i/100, (i + random.random())/100])

                for i in range(0, 100):
                    scatter_data2.append([0.25 + 0.25*random.random(), 0.65 + 0.25*random.random()])

                add_plot("Scatter Plot##demo", height=400)
                add_scatter_series("Scatter Plot##demo", "Data 1", scatter_data1)
                add_scatter_series("Scatter Plot##demo", "Data 2", scatter_data2, size=7, marker=mvPlotMarker_Square, fill=[255, 0, 0, 100])

            with tree_node("Bar Plots##demo"):

                add_plot("Bar Plot##demo", x_axis_name="Student", y_axis_name="Score", height=400)
                set_plot_xlimits("Bar Plot##demo", 9, 33)
                set_plot_ylimits("Bar Plot##demo", 0, 110)
                set_xticks("Bar Plot##demo", [["S1", 11], ["S2", 21], ["S3", 31]])
                add_bar_series("Bar Plot##demo", "Final Exam", [[10, 100], [20, 75], [30,90]], weight=1)
                add_bar_series("Bar Plot##demo", "Midterm Exam", [[11, 83], [21, 75], [31,72]], weight=1)
                add_bar_series("Bar Plot##demo", "Course Grade", [[12, 42], [22, 68], [32,23]], weight=1)

            with tree_node("Error Bars##demo"):

                add_plot("##errorbars##demo", height=400)

                error_data1 = [[1, 1, 0.2, 0.4], [2, 2, 0.4, 0.2], [3, 5, 0.2, 0.4], [4, 3, 0.6, 0.8], [5, 4, 0.4, 0.6]] # bars
                error_data2 = [[1, 8, 0.2, 0.4], [2, 8, 0.4, 0.2], [3, 9, 0.2, 0.4], [4, 7, 0.6, 0.8], [5, 8, 0.4, 0.6]] # lines

                add_bar_series("##errorbars##demo", "Bar", error_data1, weight=0.25)
                add_error_series("##errorbars##demo", "Bar", error_data1)

                add_line_series("##errorbars##demo", "Line", error_data2)
                add_error_series("##errorbars##demo", "Line", error_data2, color=[0, 255, 0])

            with tree_node("Stem Plots##demo"):
                add_plot("Stem Plots##plot##demo", height=400)

                stem_data1 = []
                stem_data2 = []
                for i in range(0, 51):
                    stem_data1.append([i * 0.02, 1.0 + 0.5*sin(25*i*0.02)*cos(2*i*0.02)])
                    stem_data2.append([i * 0.02, 0.5 + 0.25*sin(25*i*0.02)*cos(2*i*0.02)])

                add_stem_series("Stem Plots##plot##demo", "Data 1", stem_data1)
                add_stem_series("Stem Plots##plot##demo", "Data 2", stem_data2, marker=mvPlotMarker_Diamond)

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
                add_pie_series("##PieChart1##demo", "PieChart1", [["fish", 0.25], ["Cow", 0.30], ["Chicken", 0.30]], 0.5, 0.5, 0.5)
                add_pie_series("##PieChart2##demo", "PieChart2", [["A", 1], ["B", 1], ["C", 2], ["D", 3], ["E", 5]], 0.5, 0.5, 0.5, 
                               normalize=True, format="%.0f")
                set_color_map("##PieChart2##demo", mvPlotColormap_Deep)

            with tree_node("Heatmaps##demo"):
                add_plot("Heat Plot##demo", show_color_scale=True, scale_min=0.0, scale_max=6.0, 
                         scale_height=400, no_legend=True, 
                         no_mouse_pos=True, xaxis_lock_min=True, xaxis_lock_max=True, xaxis_no_gridlines=True, xaxis_no_tick_marks=True,
                         yaxis_no_gridlines=True, yaxis_no_tick_marks=True, yaxis_lock_min=True, yaxis_lock_max=True, height=400)
                values = [[0.8, 2.4, 2.5, 3.9, 0.0, 4.0, 0.0],
                          [2.4, 0.0, 4.0, 1.0, 2.7, 0.0, 0.0],
                          [1.1, 2.4, 0.8, 4.3, 1.9, 4.4, 0.0],
                          [0.6, 0.0, 0.3, 0.0, 3.1, 0.0, 0.0],
                          [0.7, 1.7, 0.6, 2.6, 2.2, 6.2, 0.0],
                          [1.3, 1.2, 0.0, 0.0, 0.0, 3.2, 5.1],
                          [0.1, 2.0, 0.0, 1.4, 0.0, 1.9, 6.3]]
                add_heat_series("Heat Plot##demo", "heat data", values, 7, 7, 0, 6)

            with tree_node("Annotations##demo"):
                
                add_plot("Annotations##plotsdemo", height=400)
                add_annotation("Annotations##plotsdemo", "BL", 0.25, 0.25, -15, 15, color=[255, 255, 0, 255])
                add_annotation("Annotations##plotsdemo", "BR", 0.75, 0.25, 15, 15, color=[255, 255, 0, 255])
                add_annotation("Annotations##plotsdemo", "TR not clampled", 0.75, 0.75, -15, -15, color=[255, 255, 0, 255], clamped=False)
                add_annotation("Annotations##plotsdemo", "TL", 0.25, 0.75, -15, -15, color=[255, 255, 0, 255])
                add_annotation("Annotations##plotsdemo", "Center", 0.5, 0.5, 0, 0, color=[255, 255, 0, 255])

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

                def plot_callback(sender, data):
                    clear_plot("Plot1##demoquery")
                    clear_plot("Plot2##demoquery")

                    sindata = []
                    for i in range(0, 1000):
                        sindata.append([i/1000, 0.5 + 0.5*sin(50*i/1000)])

                sindata = []
                for i in range(0, 1000):
                    sindata.append([i/1000, 0.5 + 0.5*sin(50*i/1000)])
     
                add_plot("Plot1##demoquery", height=400, query_callback=query, query=True, no_menus=True)
                add_plot("Plot2##demoquery", height=400, query_callback=query, no_menus=True, no_legend=True)
                add_line_series("Plot1##demoquery", "0.5 + 0.5 * sin(x)", sindata)
                add_line_series("Plot2##demoquery", "0.5 + 0.5 * sin(x)", sindata)

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
            add_table("Table##widget##demo", ["Column 1", "Column 2", "Column 3", "Column 4"], height=400)

            tabledata = []
            for i in range(0, 10):
                row = []
                for j in range(0, 4):
                    row.append("Item"+str(i)+"-"+str(j))
                tabledata.append(row)

            set_table_data("Table##widget##demo", tabledata)

        with collapsing_header("Logging##demo"):

            def LogCallback(sender, data):
                show_logger()
                clear_log()
                loglevel = get_value("Log Level##logging##demo")
                set_log_level(loglevel)
                log("Trace Message")
                log_debug("Debug Message")
                log_info("Info Message")
                log_warning("Warning Message")
                log_error("Error Message")

            add_button("Test Logger##demo", callback=LogCallback)
            add_same_line(spacing=10)
            with group("LoggingGroup##demo"):
                add_text("Log Level##demo")
                add_radio_button("Log Level##logging##demo", items=("Trace", "Debug", "Info", "Warning", "Error", "Off"))

        with collapsing_header("Filtering##demo"):
            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")

        with collapsing_header("Inputs, Navigation, & Focus##demo"):
            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")

            with tree_node("Polling##demoinputs"):
                add_text("Key Polling:")
                add_label_text("A key Down##demo", value="False", color=(0,200,255))
                add_label_text("W key Pressed##demo", value="False", color=(0,200,255))
                add_label_text("Q key Released##demo", value="False", color=(0,200,255))
                add_spacing()
                add_text("Mouse Polling:")
                add_label_text("Mouse Position##demo", value="(0,0)", color=(0,200,255))
                add_label_text("Left Mouse Dragging##demo", value="False", color=(0,200,255))
                add_label_text("Middle Mouse Dragging##demo", value="False", color=(0,200,255))
                add_label_text("Right Mouse Dragging##demo", value="False", color=(0,200,255))
                add_label_text("Left Mouse Clicked##demo", value="False", color=(0,200,255))
                add_label_text("Middle Mouse Clicked##demo", value="False", color=(0,200,255))
                add_label_text("Right Mouse Clicked##demo", value="False", color=(0,200,255))
                add_label_text("Left Mouse Double Clicked##demo", value="False", color=(0,200,255))
                add_label_text("Middle Mouse Double Clicked##demo", value="False", color=(0,200,255))
                add_label_text("Right Mouse Double Clicked##demo", value="False", color=(0,200,255))
                add_label_text("Left Mouse Down##demo", value="False", color=(0,200,255))
                add_label_text("Middle Mouse Down##demo", value="False", color=(0,200,255))
                add_label_text("Right Mouse Down##demo", value="False", color=(0,200,255))
                add_label_text("Left Mouse Released##demo", value="False", color=(0,200,255))
                add_label_text("Middle Mouse Released##demo", value="False", color=(0,200,255))
                add_label_text("Right Mouse Released##demo", value="False", color=(0,200,255))



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