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

def show_demo():

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
        add_text("This demo is not complete but will continue to be added to throughout the 0.4.x releases!")

        with collapsing_header("Window options"):
            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")
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

                add_label_text("label", value="Value")
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
                        "\tlock cmpxchg8b eax\n", height=300)

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
                add_simple_plot("Frame Times", value=[0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2])
                add_simple_plot("Histogram", value=(0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2), height=80, histogram=True, minscale=0.0)

                data1 = []
                for i in range(0, 70):
                    data1.append(cos(3.14*6*i/180))
                add_simple_plot("Lines##sin", value=data1, height=80)
                add_simple_plot("Histogram##sin", value=data1, height=80, histogram=True)
                add_progress_bar("Progress Bar", value=0.78, overlay="78%")
                add_same_line()
                add_text("Progress Bar##text")
                set_value("Progress Bar##text", "Progress Bar")
                add_progress_bar("##Progress Bar", value=0.78, overlay="1367/1753")

            with tree_node("Color/Picker Widgets"):
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

            with tree_node("Time/Date Widgets"):
                add_time_picker("Time Picker", default_value={'hour': 14, 'min': 32, 'sec': 23})
                add_separator()
                with managed_columns("Date Columns", 3):
                    add_date_picker("Date Picker1", level=0, default_value={'month_day': 8, 'year':93, 'month':5})
                    add_date_picker("Date Picker2", level=1, default_value={'month_day': 8, 'year':93, 'month':5})
                    add_date_picker("Date Picker3", level=2, default_value={'month_day': 8, 'year':93, 'month':5})
                add_separator()

        with collapsing_header("Layout & Scolling"):

            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")

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

        with collapsing_header("Tooltips"):
            add_text("Hover me for a fancy tooltip")
            with tooltip("Hover me for a fancy tooltip", "tool_tip##tooltips"):
                add_simple_plot("Simpleplot##tooltips", value=(0.3, 0.9, 2.5, 8.9), height = 80)

        with collapsing_header("Popups, Modal windows, & Dialogs"):

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

            with tree_node("File Selector"):
                def file_selected(sender, data):
                    log_info(data)
                add_button("Select Python File", callback = lambda sender, data: open_file_dialog(file_selected, ".*,.py"))
                add_button("Select C++ File", callback = lambda sender, data: open_file_dialog(file_selected, ".*,.cpp"))

            with tree_node("Directory Selector"):
                def directory_selected(sender, data):
                    log_info(data)
                add_button("Select Directory", callback = lambda sender, data: select_directory_dialog(directory_selected))

        with collapsing_header("Columns"):

            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")

            with tree_node("Basic##columns"):
                add_text("Without border:")
                add_separator()
                with managed_columns("columns1", 3, border=False):
                    for i in range(0, 14):
                        add_selectable(f"Item {i}##columns1")
                add_separator()

                add_text("With border:")
                add_separator()
                with managed_columns("columns2", 4):
                    add_text("ID")
                    add_text("Name")
                    add_text("Path")
                    with group("Just to get separator in the same cell"):
                        add_text("Hovered")
                        add_separator()

                    add_selectable("0000", span_columns=True)
                    add_text("One")
                    add_text("/path/one")
                    add_text("0")

                    add_selectable("0001", span_columns=True)
                    add_text("Two")
                    add_text("/path/two")
                    add_text("0")

                    add_selectable("0003", span_columns=True)
                    add_text("Three")
                    add_text("/path/three")
                    add_text("0")
                add_separator()

            with tree_node("Borders##columns"):

                with managed_columns("Columns3", 4):

                    def replicated_cell(i):
                        with group(f"replicated_group##{i}"):
                            if i % 4 == 0:
                                add_separator()
                            add_text(f"aaa##{i}")
                            add_input_text(f"##inputcolumns{i}")
                            add_button(f"Button##repl{i}")

                    for i in range(0, 12):
                        replicated_cell(i)
                add_separator()

        with collapsing_header("Drawings"):

            def UpdateDrawing(sender, data):
                set_drawing_origin("drawing##widget", get_value("X Origin"), get_value("Y Origin"))
                set_drawing_scale("drawing##widget", get_value("X Scale "), get_value("Y Scale"))

            with group("Drawing Controls Group"):
                add_slider_float("X Origin", vertical=True, min_value = -100, max_value=100, default_value=0, callback=UpdateDrawing)
                add_same_line(spacing=20)
                add_slider_float("Y Origin", vertical=True, min_value = -100, max_value=100, default_value=0, callback=UpdateDrawing)
                add_slider_float("X Scale ", vertical=True, max_value=10, default_value=1, callback=UpdateDrawing)
                add_same_line(spacing=20)
                add_slider_float("Y Scale", vertical=True, max_value=10, default_value=1, callback=UpdateDrawing)
            add_same_line(spacing=20)
            add_drawing("drawing##widget", width=800, height=500)
            draw_rectangle("drawing##widget", (0, 500), (800, 0), (255, 0, 0, 255), fill=(0, 0, 25, 255), rounding=12, thickness = 1.0)
            draw_line("drawing##widget", (10, 10), (100, 100), (255, 0, 0, 255), 1)
            draw_triangle("drawing##widget", (300, 500), (200, 200), (500, 200), (255, 255, 0, 255), thickness = 3.0)
            draw_quad("drawing##widget", (50, 50), (150, 50), (150, 150), (50, 150), (255, 255, 0, 255), thickness = 3.0)
            draw_text("drawing##widget", (50, 300), "Some Text", color=(255, 255, 0, 255), size=15)
            draw_text("drawing##widget", (0, 0), "Origin", color=(255, 255, 0, 255), size=15)
            draw_circle("drawing##widget", (400, 250), 50, (255, 255, 0, 255))
            draw_polyline("drawing##widget", ((320, 490), (185, 200), (500, 710)), (255, 255, 0, 255), thickness=1.0)
            draw_polygon("drawing##widget", ((363, 471), (153, 498), (59, 220), (363, 471)), (255, 125, 0, 255), thickness=1.0, fill=(255, 125, 0, 50))
            draw_bezier_curve("drawing##widget", (50, 200), (150, 250), (300, 150), (600, 250), (255, 255, 0, 255), thickness = 2.0)
            draw_arrow("drawing##widget", (50, 70), (100, 65), (0, 200, 255), 1, 10)

        with collapsing_header("Plots"):

            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")

            with tree_node("Help##plots"):
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

            with tree_node("Line Plots"):

                sindata = []
                for i in range(0, 1000):
                    #sindata.append([3.14*i/180, 0.5+ 0.5*cos(50*3.14*i/180)])
                    sindata.append([i/1000, 0.5 + 0.5*sin(50*i/1000)])

                x2data = []
                for i in range(0, 100):
                    x2data.append([1/(i+1), (1/(i+1))**2])

                add_text("Anit-aliasing can be enabled from the plot's context menu (see Help).", bullet=True)
                add_plot("Line Plot", x_axis_name="x", y_axis_name="y", height=400)
                add_line_series("Line Plot", "0.5 + 0.5 * sin(x)", sindata)
                add_line_series("Line Plot", "x^2", x2data)

            with tree_node("Shade Plots"):

                stock_data1 = []
                stock_data2 = []
                stock_data3 = []
                for i in range(0, 100):
                    stock_data1.append([i, 400 + 50*abs(random.random())])
                    stock_data2.append([i, 275 + 75*abs(random.random())])
                    stock_data3.append([i, 150 + 75*abs(random.random())])

                add_plot("Stock Prices", x_axis_name="Days", y_axis_name="Price", height=400)
                add_line_series("Stock Prices", "Stock 1", stock_data1, color=[0, 0, 255, 255])
                add_line_series("Stock Prices", "Stock 2", stock_data2, color=[255, 0, 0, 255])
                add_line_series("Stock Prices", "Stock 3", stock_data3, color=[0, 255, 0, 255])
                add_shade_series("Stock Prices", "Stock 1", stock_data1, fill=[0, 0, 255, 64])
                add_shade_series("Stock Prices", "Stock 2", stock_data2, fill=[255, 0, 0, 64])
                add_shade_series("Stock Prices", "Stock 3", stock_data3, fill=[0, 255, 0, 64])

            with tree_node("Scatter Plots"):

                scatter_data1 = []
                scatter_data2 = []
                for i in range(0, 100):
                    scatter_data1.append([i/100, (i + random.random())/100])

                for i in range(0, 100):
                    scatter_data2.append([0.25 + 0.25*random.random(), 0.65 + 0.25*random.random()])

                add_plot("Scatter Plot", height=400)
                add_scatter_series("Scatter Plot", "Data 1", scatter_data1)
                add_scatter_series("Scatter Plot", "Data 2", scatter_data2, size=7, marker=mvPlotMarker_Square, fill=[255, 0, 0, 100])

            with tree_node("Bar Plots"):

                add_plot("Bar Plot", x_axis_name="Student", y_axis_name="Score", height=400)
                set_plot_xlimits("Bar Plot", 9, 33)
                set_plot_ylimits("Bar Plot", 0, 110)
                set_xticks("Bar Plot", [["S1", 11], ["S2", 21], ["S3", 31]])
                add_bar_series("Bar Plot", "Final Exam", [[10, 100], [20, 75], [30,90]], weight=1)
                add_bar_series("Bar Plot", "Midterm Exam", [[11, 83], [21, 75], [31,72]], weight=1)
                add_bar_series("Bar Plot", "Course Grade", [[12, 42], [22, 68], [32,23]], weight=1)

            with tree_node("Error Bars"):

                add_plot("##errorbars", height=400)

                error_data1 = [[1, 1, 0.2, 0.4], [2, 2, 0.4, 0.2], [3, 5, 0.2, 0.4], [4, 3, 0.6, 0.8], [5, 4, 0.4, 0.6]] # bars
                error_data2 = [[1, 8, 0.2, 0.4], [2, 8, 0.4, 0.2], [3, 9, 0.2, 0.4], [4, 7, 0.6, 0.8], [5, 8, 0.4, 0.6]] # lines

                add_bar_series("##errorbars", "Bar", error_data1, weight=0.25)
                add_error_series("##errorbars", "Bar", error_data1)

                add_line_series("##errorbars", "Line", error_data2)
                add_error_series("##errorbars", "Line", error_data2, color=[0, 255, 0])

            with tree_node("Stem Plots"):
                add_plot("Stem Plots##plot", height=400)

                stem_data1 = []
                stem_data2 = []
                for i in range(0, 51):
                    stem_data1.append([i * 0.02, 1.0 + 0.5*sin(25*i*0.02)*cos(2*i*0.02)])
                    stem_data2.append([i * 0.02, 0.5 + 0.25*sin(25*i*0.02)*cos(2*i*0.02)])

                add_stem_series("Stem Plots##plot", "Data 1", stem_data1)
                add_stem_series("Stem Plots##plot", "Data 2", stem_data2, marker=mvPlotMarker_Diamond)

            with tree_node("Pie Charts"):
                add_plot("##PieChart1", no_mouse_pos=True, 
                         xaxis_no_gridlines=True, xaxis_no_tick_marks=True, xaxis_no_tick_labels=True,
                         yaxis_no_gridlines=True, yaxis_no_tick_marks=True, yaxis_no_tick_labels=True, width=250, height=250)
                add_same_line()
                add_plot("##PieChart2", no_mouse_pos=True, 
                         xaxis_no_gridlines=True, xaxis_no_tick_marks=True, xaxis_no_tick_labels=True,
                         yaxis_no_gridlines=True, yaxis_no_tick_marks=True, yaxis_no_tick_labels=True, 
                         width=250, height=250)
                set_plot_xlimits("##PieChart1", 0, 1)
                set_plot_xlimits("##PieChart2", 0, 1)
                set_plot_ylimits("##PieChart1", 0, 1)
                set_plot_ylimits("##PieChart2", 0, 1)
                add_pie_series("##PieChart1", "PieChart1", [["fish", 0.25], ["Cow", 0.30], ["Chicken", 0.30]], 0.5, 0.5, 0.5)
                add_pie_series("##PieChart2", "PieChart2", [["A", 1], ["B", 1], ["C", 2], ["D", 3], ["E", 5]], 0.5, 0.5, 0.5, 
                               normalize=True, format="%.0f")
                set_color_map("##PieChart2", mvPlotColormap_Deep)

            with tree_node("Heatmaps"):
                add_plot("Heat Plot", show_color_scale=True, scale_min=0.0, scale_max=6.0, 
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
                add_heat_series("Heat Plot", "heat data", values, 7, 7, 0, 6)

        with collapsing_header("Simple Tables"):

            add_button("Delete row 6", callback=lambda sender, data: delete_row("Table##widget", 6))
            add_same_line()
            add_button("Delete col 1", callback=lambda sender, data: delete_column("Table##widget", 1))   
            add_same_line()
            add_button("Add row ", callback=lambda sender, data: add_row("Table##widget", ["new1", "new2", "new3", 53]))
            add_same_line()
            add_button("Add col ", callback=lambda sender, data: add_column("Table##widget", "New Column", ["new1", "new2", "new3", "new4"]))
            add_same_line()
            add_button("Insert row 5", callback=lambda sender, data: insert_row("Table##widget", 5, ["inew1", "inew2", "inew3", "inew4"]))
            add_same_line()
            add_button("Insert col 1 ", callback=lambda sender, data:insert_column("Table##widget", 1,  "Inserted Column", ["inew1", "inew2", "inew3", "inew4"]))
            add_same_line()
            add_button("Clear Table ", callback=lambda sender, data: clear_table("Table##widget"))
            add_table("Table##widget", ["Column 1", "Column 2", "Column 3", "Column 4"], height=400)

            tabledata = []
            for i in range(0, 10):
                row = []
                for j in range(0, 4):
                    row.append("Item"+str(i)+"-"+str(j))
                tabledata.append(row)

            set_table_data("Table##widget", tabledata)

        with collapsing_header("Logging"):

            def LogCallback(sender, data):
                show_logger()
                clear_log()
                loglevel = get_value("Log Level##logging")
                set_log_level(loglevel)
                log("Trace Message")
                log_debug("Debug Message")
                log_info("Info Message")
                log_warning("Warning Message")
                log_error("Error Message")

            add_button("Test Logger", callback=LogCallback)
            add_same_line(spacing=10)
            with group("LoggingGroup"):
                add_text("Log Level")
                add_radio_button("Log Level##logging", items=("Trace", "Debug", "Info", "Warning", "Error", "Off"))

        with collapsing_header("Filtering"):
            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")

        with collapsing_header("Inputs, Navigation, & Focus"):
            add_text("This section is not ready! But will completed sometime during the 0.4.x releases!")
