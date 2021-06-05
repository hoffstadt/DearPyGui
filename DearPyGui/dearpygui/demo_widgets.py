import dearpygui.core as dpg
import dearpygui.contexts as cxt
from dearpygui.common import _log, _config, _help, _hsv_to_rgb
from math import sin, cos

def show_demo():

    with cxt.collapsing_header(label="Widgets"):
        
        with cxt.tree_node(label="Basic"):

            with cxt.group(horizontal=True):
                dpg.add_button(label="Button", callback=_log)
                dpg.add_button(label="Button", callback=_log, small=True)
                dpg.add_button(label="Button", callback=_log, arrow=True) # default direction is mvDir_Up
                dpg.add_button(label="Button", callback=_log, arrow=True, direction=dpg.mvDir_Left)
                dpg.add_button(label="Button", callback=_log, arrow=True, direction=dpg.mvDir_Right)
                dpg.add_button(label="Button", callback=_log, arrow=True, direction=dpg.mvDir_Down)

            dpg.add_checkbox(label="checkbox", callback=_log)
            dpg.add_radio_button(("radio a", "radio b", "radio c"), callback=_log, horizontal=True)
            dpg.add_selectable(label="selectable", callback=_log)

            with cxt.group(horizontal=True) as g:

                for i in range(0, 7):
                    button = dpg.add_button(label="Click", callback=_log)
                    dpg.set_theme_style(dpg.mvThemeStyle_Button_Rounding, i*5, item=button)
                    dpg.set_theme_style(dpg.mvThemeStyle_Button_PaddingX, i*3, item=button)
                    dpg.set_theme_style(dpg.mvThemeStyle_Button_PaddingY, i*3, item=button)
                    dpg.set_theme_color(dpg.mvThemeCol_Button_Bg, _hsv_to_rgb(i/7.0, 0.6, 0.6), item=button)
                    dpg.set_theme_color(dpg.mvThemeCol_Button_Active, _hsv_to_rgb(i/7.0, 0.8, 0.8), item=button)
                    dpg.set_theme_color(dpg.mvThemeCol_Button_Hovered, _hsv_to_rgb(i/7.0, 0.7, 0.7), item=button)

            with cxt.group(horizontal=True):

                dpg.add_text("Press a button: ")
                dpg.add_button(arrow=True, direction=dpg.mvDir_Left, callback=lambda: dpg.set_value(widget, int(dpg.get_value(widget))-1))
                dpg.add_button(arrow=True, direction=dpg.mvDir_Right, callback=lambda: dpg.set_value(widget, int(dpg.get_value(widget))+1))
                widget = dpg.add_text("0")

            widget2 = dpg.add_text("hover me")
            with cxt.tooltip(parent=widget2): # note that "parent" is the item the tooltip show's for
                dpg.add_text("I'm a fancy tooltip")

            dpg.add_separator()

            dpg.add_text("Value", label="Label", show_label=True)
            dpg.add_combo(("AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK"), label="combo", default_value="AAAA", callback=_log)
            dpg.add_input_text(label="input text", default_value="Hello, world!", callback=_log)
            _help(
                    "USER:\n"
                    "Hold SHIFT or use mouse to select text.\n"
                    "CTRL+Left/Right to word jump.\n"
                    "CTRL+A or double-click to select all.\n"
                    "CTRL+X,CTRL+C,CTRL+V clipboard.\n"
                    "CTRL+Z,CTRL+Y undo/redo.\n"
                    "ESCAPE to revert.\n\n")
            dpg.add_input_text(label="input text (w/ hint)", hint="enter text here", callback=_log)
            dpg.add_input_int(label="input int", callback=_log)
            dpg.add_input_float(label="input float", callback=_log)
            dpg.add_input_float(label="input scientific", format="%e", callback=_log)
            dpg.add_input_floatx(label="input floatx", callback=_log, default_value=[1,2,3,4])
            dpg.add_drag_int(label="drag int", callback=_log)
            _help(
                    "Click and drag to edit value.\n"
                    "Hold SHIFT/ALT for faster/slower edit.\n"
                    "Double-click or CTRL+click to input value.")
            dpg.add_drag_int(label="drag int 0..100", format="%d%%", callback=_log)
            dpg.add_drag_float(label="drag float", callback=_log)
            dpg.add_drag_float(label="drag small float", default_value=0.0067, format="%.06f ns", callback=_log)
            dpg.add_slider_int(label="slider int", max_value=3, callback=_log)
            _help("CTRL+click to enter value.")
            dpg.add_slider_float(label="slider float", max_value=1.0, format="ratio = %.3f", callback=_log)
            dpg.add_slider_int(label="slider angle", min_value=-360, max_value=360, format="%d deg", callback=_log)
            _help(
                    "Click on the colored square to open a color picker.\n"
                    "Click and hold to use drag and drop.\n"
                    "Right-click on the colored square to show options.\n"
                    "CTRL+click on individual component to input value.\n")
            dpg.add_color_edit((102, 179, 0, 128), label="color edit 4", callback=_log, uint8=True)
            dpg.add_color_edit(default_value=(.5, 1, .25, .1), label="color edit 3", callback=_log, m_3component=True, uint8=True, floats=False)
            dpg.add_listbox(("Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"), label="listbox", num_items=4, callback=_log)
            dpg.add_color_button(label="color button")

        with cxt.tree_node(label="Bullets"):

            dpg.add_text("Bullet point 1", bullet=True)
            dpg.add_text("Bullet point 2\nbullet text can be\nOn multiple lines", bullet=True)

            with cxt.tree_node(label="Tree node"):
                dpg.add_text("Another bullet point", bullet=True)

            dpg.add_text("1", bullet=True)
            dpg.add_same_line()
            dpg.add_button(label="Button", small=True)

        with cxt.tree_node(label="Text"):

            with cxt.tree_node(label="Colored Text"):
                
                dpg.add_text("Pink", color=(255, 0, 255))
                dpg.add_text("Yellow", color=(255, 255, 0))

            with cxt.tree_node(label="Word Wrapping"):

                dpg.add_text('This text should automatically wrap on the edge of the window.The current implementation for the text wrapping follows simple rules suited for English and possibly other languages', wrap=0)
                dpg.add_slider_int(label="wrap width", default_value=500, max_value=1000, 
                                   callback=lambda sender: dpg.configure_item("_demo_wrap", wrap=dpg.get_value(sender)))
                dpg.add_text('The lazy dong is a good dog. This paragraph should fit within the child. Testing a 1 character word. The quick brown fox jumps over the lazy dog.', id="_demo_wrap", wrap=100)

        with cxt.tree_node(label="Text Input"):

            dpg.add_checkbox(label="readonly", callback=_config, user_data="Multi-line Text Input")
            dpg.add_checkbox(label="on_enter", callback=_config, user_data="Multi-line Text Input")
            
            with cxt.tree_node(id="Multi-line Text Input"):

                dpg.add_input_text(multiline=True, default_value="/*\n"
                    " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
                    " the hexadecimal encoding of one offending instruction,\n"
                    " more formally, the invalid operand with locked CMPXCHG8B\n"
                    " instruction bug, is a design flaw in the majority of\n"
                    " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
                    " processors (all in the P5 microarchitecture).\n"
                    "*/\n\n"
                    "label:\n"
                    "\tlock cmpxchg8b eax\n", height=300, callback=_log, tab_input=True)

            with cxt.tree_node(label="Filtered Text Input"):

                dpg.add_input_text(callback=_log, label="default")
                dpg.add_input_text(callback=_log, label="decimal", decimal=True)
                dpg.add_input_text(callback=_log, label="no blank", no_spaces=True)
                dpg.add_input_text(callback=_log, label="uppercase", uppercase=True)
                dpg.add_input_text(callback=_log, label="scientific", scientific=True)
                dpg.add_input_text(callback=_log, label="hexdecimal", hexadecimal=True)
            
            with cxt.tree_node(label="Password Input"):

                password = dpg.add_input_text(label="password", password=True, callback=_log)
                dpg.add_input_text(label="password (w/ hint)", password=True, hint="<password>", source=password, callback=_log)
                dpg.add_input_text(label="password (clear)", source=password, callback=_log)

        with cxt.tree_node(label="Simple Plots"):

            dpg.add_simple_plot(label="Frame Times", default_value=[0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2])
            dpg.add_simple_plot(label="Histogram", default_value=(0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2), height=80, histogram=True, minscale=0.0)

            data1 = []
            for i in range(0, 70):
                data1.append(cos(3.14*6*i/180))

            dpg.add_simple_plot(label="Lines", default_value=data1, height=80)
            dpg.add_simple_plot(label="Histogram", default_value=data1, height=80, histogram=True)
            dpg.add_progress_bar(label="Progress Bar", default_value=0.78, overlay="78%")
            dpg.add_same_line()
            dpg.add_text("Progress Bar")
            bar = dpg.add_progress_bar(default_value=0.78, overlay="1367/1753")
            dpg.set_theme_color(dpg.mvThemeCol_ProgressBar_Bar, (255,0,0, 255), item=bar)

        with cxt.tree_node(label="Multi-component Widgets"):

            for i in range(2, 5):

                with cxt.group():
                    float_source = dpg.add_input_floatx(label=f"input float {i}", min_value=0.0, max_value=100.0, size=i)
                    dpg.add_drag_floatx(label=f"drag float {i}", source=float_source, size=i)
                    dpg.add_slider_floatx(label=f"slider float {i}", source=float_source, size=i)

                with cxt.group():

                    int_source = dpg.add_input_intx(label=f"input int {i}", min_value=0, max_value=100, size=i)
                    dpg.add_drag_intx(label=f"drag int {i}", source=int_source, size=i)
                    dpg.add_slider_intx(label=f"slider int {i}", source=int_source, size=i)
            
                dpg.add_dummy(height=10)

        with cxt.tree_node(label="Vertical Sliders"):

            dpg.add_slider_int(label=" ", default_value=1, vertical=True, max_value=5, height=160)
            dpg.add_same_line()

            with cxt.group(horizontal=True):

                values = [ 0.0, 0.60, 0.35, 0.9, 0.70, 0.20, 0.0 ]

                for i in range(0, 7):
                    widget3 = dpg.add_slider_float(label=" ", default_value=values[i], vertical=True, max_value=1.0, height=160)
                    dpg.set_theme_color(dpg.mvThemeCol_SliderFloat_Bg, _hsv_to_rgb(i/7.0, 0.5, 0.5), item=widget3)
                    dpg.set_theme_color(dpg.mvThemeCol_SliderFloat_Grab, _hsv_to_rgb(i/7.0, 0.9, 0.9), item=widget3)
                    dpg.set_theme_color(dpg.mvThemeCol_SliderFloat_BgActive, _hsv_to_rgb(i/7.0, 0.7, 0.5), item=widget3)
                    dpg.set_theme_color(dpg.mvThemeCol_SliderFloat_BgHovered, _hsv_to_rgb(i/7.0, 0.6, 0.5), item=widget3)

            dpg.add_same_line()
            with cxt.group():
                for i in range(0, 3):
                    with cxt.group(horizontal=True):
                        values = [ 0.20, 0.80, 0.40, 0.25 ]
                        for j in range(0, 4):
                            dpg.add_slider_float(label=" ", default_value=values[j], vertical=True, max_value=1.0, height=50)

            dpg.add_same_line()
            with cxt.group(horizontal=True):
                dpg.add_slider_float(label=" ", vertical=True, max_value=1.0, height=160, width=40)
                dpg.add_slider_float(label=" ", vertical=True, max_value=1.0, height=160, width=40)
                dpg.add_slider_float(label=" ", vertical=True, max_value=1.0, height=160, width=40)
                dpg.add_slider_float(label=" ", vertical=True, max_value=1.0, height=160, width=40)

        with cxt.tree_node(label="Time/Date Widgets"):

            dpg.add_time_picker(label="Time Picker", default_value={'hour': 14, 'min': 32, 'sec': 23})
            dpg.add_separator()
            
            with cxt.table(header_row=False):

                dpg.add_table_column()
                dpg.add_table_column()
                dpg.add_table_column()

                dpg.add_date_picker(label="Date Picker1", level=0, default_value={'month_day': 8, 'year':93, 'month':5})
                dpg.add_table_next_column()
                dpg.add_date_picker(label="Date Picker2", level=1, default_value={'month_day': 8, 'year':93, 'month':5})
                dpg.add_table_next_column()
                dpg.add_date_picker(label="Date Picker3", level=2, default_value={'month_day': 8, 'year':93, 'month':5})

        with cxt.tree_node(label="Loading Indicators"):

            dpg.add_loading_indicator()
            dpg.add_same_line()
            dpg.add_loading_indicator(style=1)

        with cxt.tree_node(label="Knobs"):

            with cxt.group(horizontal=True):

                dpg.add_knob_float(label="K1")
                dpg.add_knob_float(label="K2", default_value=25.0)
                dpg.add_knob_float(label="K3", default_value=50.0)

        with cxt.tree_node(label="2D/3D Sliders"):

            dpg.add_3d_slider(label="3D Slider", scale=0.5)