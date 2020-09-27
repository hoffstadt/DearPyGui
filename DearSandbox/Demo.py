from dearpygui.core import *
from dearpygui.simple import *
from math import sin, cos

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

########################################################################################################################
# Settings and Data Storage
########################################################################################################################
set_log_level(0)

set_main_window_title("DearPyGui Demo")
set_main_window_size(1000, 800)
set_main_window_pos(0, 0)
add_additional_font("C:/dev/DearPyGui/Resources/NotoSerifCJKjp-Medium.otf", 20)

with window("Dear PyGui Demo", x_pos=100, y_pos=100, width=800, height=800):

    with menu_bar("MenuBar"):

        with menu("Themes"):
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

        with menu("Tools"):
            add_menu_item("Show Logger", callback=show_logger)
            add_menu_item("Show About", callback=show_about)
            add_menu_item("Show Metrics", callback=show_metrics)
            add_menu_item("Show Documentation", callback=show_documentation)
            add_menu_item("Show Debug", callback=show_debug)
            add_menu_item("Show Style Editor", callback=show_style_editor)

    add_text(f'Dear PyGui says hello. ({get_dearpygui_version()})')

    with collapsing_header("Window options"):
        add_checkbox("No titlebar", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_title_bar=get_value(sender)))
        add_checkbox("No move", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_move=get_value(sender)))
        add_checkbox("No resize", callback=lambda sender, data: configure_item("Dear PyGui Demo", no_resize=get_value(sender)))

    with collapsing_header("Widgets"):

        with tree_node("Basic"):

            add_button("Button")
            add_checkbox("checkbox")
            add_radio_button("radiobutton", items=["radio a", "radio b", "radio c"], horizontal=True)
        
            for i in range(0, 7):
                if i > 0:
                    add_same_line()
                add_button(f"Click##{i}")
                set_item_color(f"Click##{i}", mvGuiCol_Button, hsv_to_rgb(i/7.0, 0.6, 0.6))
                set_item_color(f"Click##{i}", mvGuiCol_ButtonHovered, hsv_to_rgb(i/7.0, 0.7, 0.7))
                set_item_color(f"Click##{i}", mvGuiCol_ButtonActive, hsv_to_rgb(i/7.0, 0.8, 0.8))

            add_text("Press a button: ")
            add_same_line()
            add_button("Left", arrow=True, direction=mvDir_Left, 
                       callback=lambda sender, data: set_value("value", int(get_value("value"))-1))
            add_same_line()
            add_button("Right", arrow=True, direction=mvDir_Right,
                       callback=lambda sender, data: set_value("value", int(get_value("value"))+1))
            add_same_line()
            add_text("value")
            set_value("value", "0")

            add_separator()

            add_label_text("label", "Value")
            add_combo("combo", items=["AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK"], default_value="AAAA")
            add_input_text("input text", default_value="Hello, world!")
            helpmarker(
                    "USER:\n"
                    "Hold SHIFT or use mouse to select text.\n"
                    "CTRL+Left/Right to word jump.\n"
                    "CTRL+A or double-click to select all.\n"
                    "CTRL+X,CTRL+C,CTRL+V clipboard.\n"
                    "CTRL+Z,CTRL+Y undo/redo.\n"
                    "ESCAPE to revert.\n\n")
            add_input_text("input text (w/ hint)", hint="enter text here")
            add_input_int("input int")
            add_input_float("input float")
            add_input_float("input scientific", format="%e")
            add_input_float3("input float3##example")
            add_drag_int("drag int")
            helpmarker(
                    "Click and drag to edit value.\n"
                    "Hold SHIFT/ALT for faster/slower edit.\n"
                    "Double-click or CTRL+click to input value.")
            add_drag_int("drag int 0..100", format="%d%%")
            add_drag_float("drag float")
            add_drag_float("drag small float", default_value=0.0067, format="%.06f ns")
            add_slider_int("slider int", max_value=3)
            helpmarker("CTRL+click to enter value.")
            add_slider_float("slider float", max_value=1.0, format="ratio = %.3f")
            add_slider_int("slider angle", min_value=-360, max_value=360, format="%d deg")
            add_color_edit3("color 1", default_value=[255, 0, 51])
            helpmarker(
                    "Click on the colored square to open a color picker.\n"
                    "Click and hold to use drag and drop.\n"
                    "Right-click on the colored square to show options.\n"
                    "CTRL+click on individual component to input value.\n")
            add_color_edit4("color 2", default_value=[102, 179, 0, 128])
            add_listbox("listbox", items=["Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"], num_items=4)

        with tree_node("Bullets"):
            add_text("Bullet point 1", bullet=True)
            add_text("Bullet point 2\nOn multiple lines", bullet=True)
            with tree_node("Tree node"):
                add_text("Another bullet point", bullet=True)
            add_text("", bullet=True)
            add_same_line()
            add_button("Button##bullets", small=True)

        with tree_node("Text"):

            with tree_node("Colored Text"):
                add_text("Pink", color=[255, 0, 255])
                add_text("Yellow", color=[255, 255, 0])
    
        with tree_node("Text Input"):
        
            with tree_node("Multi-line Text Input"):
                add_input_text("##multiline", multiline=True, default_value="/*\n"
                    " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
                    " the hexadecimal encoding of one offending instruction,\n"
                    " more formally, the invalid operand with locked CMPXCHG8B\n"
                    " instruction bug, is a design flaw in the majority of\n"
                    " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
                    " processors (all in the P5 microarchitecture).\n"
                    "*/\n\n"
                    "label:\n"
                    "\tlock cmpxchg8b eax\n")

            with tree_node("Filtered Text Input"):
                add_input_text("default")
                add_input_text("decimal", decimal=True)
                add_input_text("hexdecimal", hexadecimal=True)
                add_input_text("uppercase", uppercase=True)
                add_input_text("no blank", no_spaces=True)
                add_input_text("scientific", scientific=True)
            
            with tree_node("Password Input"):
                add_input_text("password", password=True, source="password")
                add_input_text("password (w/ hint)", password=True, hint="<password>", source="password")
                add_input_text("password (clear)", source="password")

        with tree_node("Simple Plot Widgets"):
            add_simple_plot("Frame Times", [0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2])
            add_simple_plot("Histogram", [0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2], height=80, histogram=True, minscale=0.0)

            data1 = []
            for i in range(0, 70):
                data1.append(cos(3.14*6*i/180))
            add_simple_plot("Lines##sin", data1, height=80)
            add_simple_plot("Histogram##sin", data1, height=80, histogram=True)
            add_progress_bar("Progress Bar", value=0.78, overlay="78%")
            add_same_line()
            add_text("Progress Bar##text")
            set_value("Progress Bar##text", "Progress Bar")
            add_progress_bar("##Progress Bar", value=0.78, overlay="1367/1753")

        with tree_node("Color/Picker Widgets"):
            pass

        with tree_node("Multi-component Widgets"):
        
            add_input_float2("input float2", source="float2")
            add_drag_float2("drag float2", source="float2")
            add_slider_float2("slider float2", source="float2")
            add_input_int2("input int2", source="int2")
            add_drag_int2("drag int2", source="int2")
            add_slider_int2("slider int2", source="int2")
            add_spacing()
            add_input_float3("input float3", source="float3")
            add_drag_float3("drag float3", source="float3")
            add_slider_float3("slider float3", source="float3")
            add_input_int3("input int3", source="int3")
            add_drag_int3("drag int3", source="int3")
            add_slider_int3("slider int3", source="int3")
            add_spacing()
            add_input_float4("input float4", source="float4")
            add_drag_float4("drag float4", source="float4")
            add_slider_float4("slider float4", source="float4")
            add_input_int4("input int4", source="int4")
            add_drag_int4("drag int4", source="int4")
            add_slider_int4("slider int4", source="int4")

        with tree_node("Vertical Sliders"):

            add_slider_int(f"##vi", default_value=1, vertical=True, max_value=5, height=160)
            add_same_line()
            with group("v group 1"):
                values = [ 0.0, 0.60, 0.35, 0.9, 0.70, 0.20, 0.0 ]
                for i in range(0, 7):
                    if i > 0:
                        add_same_line()
                    add_slider_float(f"##v{i}", default_value=values[i], vertical=True, max_value=1.0, height=160)
                    set_item_color(f"##v{i}", mvGuiCol_FrameBg, hsv_to_rgb(i/7.0, 0.5, 0.5))
                    set_item_color(f"##v{i}", mvGuiCol_FrameBgHovered, hsv_to_rgb(i/7.0, 0.6, 0.5))
                    set_item_color(f"##v{i}", mvGuiCol_FrameBgActive, hsv_to_rgb(i/7.0, 0.7, 0.5))
                    set_item_color(f"##v{i}", mvGuiCol_SliderGrab, hsv_to_rgb(i/7.0, 0.9, 0.9))

            add_same_line()
            with group("v group 2"):
                for i in range(0, 3):
                    with group(f"v group 2{i}"):
                        values = [ 0.20, 0.80, 0.40, 0.25 ]
                        for j in range(0, 4):
                            add_slider_float(f"##v{j}{i}", default_value=values[j], vertical=True, max_value=1.0, height=50, source=f"v{j}")
                            if i != 3:
                                add_same_line()   
                            
            add_same_line()
            with group("v group 3"):
                add_slider_float("##vs1", vertical=True, max_value=1.0, height=160, source="##v1", width=40)
                add_same_line()
                add_slider_float("##vs2", vertical=True, max_value=1.0, height=160, source="##v2", width=40)
                add_same_line()
                add_slider_float("##vs3", vertical=True, max_value=1.0, height=160, source="##v3", width=40)
                add_same_line()
                add_slider_float("##vs4", vertical=True, max_value=1.0, height=160, source="##v4", width=40)

    with collapsing_header("Layout & Scolling"):

        with tree_node("Child windows"):

            with child("child", border=False, width=400):
                for i in range(0, 100):
                    add_text(f"{i}: scrollable region")

            add_same_line()
            with child("decorated child", width=0):
                with menu_bar("MenuBarChild"):
                    with menu("Menu##child"):
                        pass
                with group("decorated child group", width=-20):
                    for i in range(0, 100):
                        add_button(f"{i}##childbutton")

        with tree_node("Tabs"):
            with tree_node("Basic##tabs"):
                with tab_bar("Basic Tabbar"):
                    with tab("Avocado"):
                        add_text("This is the avocado tab!")
                    with tab("Broccoli"):
                        add_text("This is the broccoli tab!")
                    with tab("Cucumber"):
                        add_text("This is the cucumber tab!")

    with collapsing_header("Popups & Modal windows"):

        with tree_node("Popups"):
            add_text("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.")
            add_button("Select..##popups")
            add_same_line()
            add_text("<None>")
            with popup("Select..##popups", "popup1"):
                add_text("Aquariam")
                add_separator()
                add_selectable("Bream", callback=lambda sender, data: set_value("<None>", sender))
                add_selectable("Haddock", callback=lambda sender, data: set_value("<None>", sender))
                add_selectable("Mackerel", callback=lambda sender, data: set_value("<None>", sender))
                add_selectable("Pollock", callback=lambda sender, data: set_value("<None>", sender))
                add_selectable("Tilefish", callback=lambda sender, data: set_value("<None>", sender))

        with tree_node("Modals"):
            add_text("Modal windows are like popups but the user cannot close them by clicking outside.")
            add_button("Delete..##modals")
            with popup("Delete..##modals", "Delete?", modal=True):
                add_text("All those beautiful files will be deleted.\nThis operation cannot be undone!")
                add_separator()
                add_checkbox("Don't ask me next time")
                add_button("OK##modal", width=75, callback=lambda sender, data: close_popup())
                add_same_line()
                add_button("Cancel##modal", width=75, callback=lambda sender, data: close_popup())

start_dearpygui()
