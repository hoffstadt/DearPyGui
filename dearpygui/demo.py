import dearpygui.dearpygui as dpg
import math
from math import sin, cos
import random
import webbrowser

def _help(message):
    last_item = dpg.last_item()
    group = dpg.add_group(horizontal=True)
    dpg.move_item(last_item, parent=group)
    dpg.capture_next_item(lambda s: dpg.move_item(s, parent=group))
    t = dpg.add_text("(?)", color=[0, 255, 0])
    with dpg.tooltip(t):
        dpg.add_text(message)

def _hyperlink(text, address):
    b = dpg.add_button(label=text, callback=lambda:webbrowser.open(address))
    dpg.bind_item_theme(b, "__demo_hyperlinkTheme")

def _config(sender, keyword, user_data):

    widget_type = dpg.get_item_type(sender)
    items = user_data

    if widget_type == "mvAppItemType::mvRadioButton":
        value = True

    else:
        keyword = dpg.get_item_label(sender)
        value = dpg.get_value(sender)

    if isinstance(user_data, list):
        for item in items:
            dpg.configure_item(item, **{keyword: value})
    else:
        dpg.configure_item(items, **{keyword: value})

def _add_config_options(item, columns, *names, **kwargs):
    
    if columns == 1:
        if 'before' in kwargs:
            for name in names:
                dpg.add_checkbox(label=name, callback=_config, user_data=item, before=kwargs['before'], default_value=dpg.get_item_configuration(item)[name])
        else:
            for name in names:
                dpg.add_checkbox(label=name, callback=_config, user_data=item, default_value=dpg.get_item_configuration(item)[name])

    else:

        if 'before' in kwargs:
            dpg.push_container_stack(dpg.add_table(header_row=False, before=kwargs['before']))
        else:
            dpg.push_container_stack(dpg.add_table(header_row=False))

        for i in range(columns):
            dpg.add_table_column()

        for i in range(int(len(names)/columns)):

            with dpg.table_row():
                for j in range(columns):
                    dpg.add_checkbox(label=names[i*columns + j], 
                                        callback=_config, user_data=item, 
                                        default_value=dpg.get_item_configuration(item)[names[i*columns + j]])
        dpg.pop_container_stack()

def _add_config_option(item, default_value, *names):
    dpg.add_radio_button(names, default_value=default_value, callback=_config, user_data=item)

def _hsv_to_rgb(h, s, v):
    if s == 0.0: return (v, v, v)
    i = int(h*6.) # XXX assume int() truncates!
    f = (h*6.)-i; p,q,t = v*(1.-s), v*(1.-s*f), v*(1.-s*(1.-f)); i%=6
    if i == 0: return (255*v, 255*t, 255*p)
    if i == 1: return (255*q, 255*v, 255*p)
    if i == 2: return (255*p, 255*v, 255*t)
    if i == 3: return (255*p, 255*q, 255*v)
    if i == 4: return (255*t, 255*p, 255*v)
    if i == 5: return (255*v, 255*p, 255*q)

def _create_static_textures():
   
    ## create static textures
    texture_data1 = []
    for i in range(100*100):
        texture_data1.append(255/255)
        texture_data1.append(0)
        texture_data1.append(255/255)
        texture_data1.append(255/255)

    texture_data2 = []
    for i in range(50*50):
        texture_data2.append(255/255)
        texture_data2.append(255/255)
        texture_data2.append(0)
        texture_data2.append(255/255)

    texture_data3 = []
    for row in range(50):
        for column in range(50):
            texture_data3.append(255/255)
            texture_data3.append(0)
            texture_data3.append(0)
            texture_data3.append(255/255)
        for column in range(50):
            texture_data3.append(0)
            texture_data3.append(255/255)
            texture_data3.append(0)
            texture_data3.append(255/255)
    for row in range(50):
        for column in range(50):
            texture_data3.append(0)
            texture_data3.append(0)
            texture_data3.append(255/255)
            texture_data3.append(255/255)
        for column in range(50):
            texture_data3.append(255/255)
            texture_data3.append(255/255)
            texture_data3.append(0)
            texture_data3.append(255/255)

    dpg.add_static_texture(100, 100, texture_data1, parent="__demo_texture_container", tag="__demo_static_texture_1", label="Static Texture 1")
    dpg.add_static_texture(50, 50, texture_data2, parent="__demo_texture_container", tag="__demo_static_texture_2", label="Static Texture 2")
    dpg.add_static_texture(100, 100, texture_data3, parent="__demo_texture_container", tag="__demo_static_texture_3", label="Static Texture 3")

def _create_dynamic_textures():
    
    ## create dynamic textures
    texture_data1 = []
    for i in range(100*100):
        texture_data1.append(255/255)
        texture_data1.append(0)
        texture_data1.append(255/255)
        texture_data1.append(255/255)

    texture_data2 = []
    for i in range(50*50):
        texture_data2.append(255/255)
        texture_data2.append(255/255)
        texture_data2.append(0)
        texture_data2.append(255/255)

    dpg.add_dynamic_texture(100, 100, texture_data1, parent="__demo_texture_container", tag="__demo_dynamic_texture_1")
    dpg.add_dynamic_texture(50, 50, texture_data2, parent="__demo_texture_container", tag="__demo_dynamic_texture_2")

def _update_dynamic_textures(sender, app_data, user_data):

    new_color = app_data
    new_color[0] = new_color[0]
    new_color[1] = new_color[1]
    new_color[2] = new_color[2]
    new_color[3] = new_color[3]

    if user_data == 1:
        texture_data = []
        for i in range(100*100):
            texture_data.append(new_color[0])
            texture_data.append(new_color[1])
            texture_data.append(new_color[2])
            texture_data.append(new_color[3])
        dpg.set_value("__demo_dynamic_texture_1", texture_data)

    elif user_data == 2:
        texture_data = []
        for i in range(50*50):
            texture_data.append(new_color[0])
            texture_data.append(new_color[1])
            texture_data.append(new_color[2])
            texture_data.append(new_color[3])
        dpg.set_value("__demo_dynamic_texture_2", texture_data)

def _on_demo_close(sender, app_data, user_data):
    dpg.delete_item(sender)
    dpg.delete_item("__demo_texture_container")
    dpg.delete_item("__demo_colormap_registry")
    dpg.delete_item("__demo_hyperlinkTheme")
    dpg.delete_item("__demo_theme_progressbar")
    dpg.delete_item("stock_theme1")
    dpg.delete_item("stock_theme2")
    dpg.delete_item("stock_theme3")
    dpg.delete_item("stock_theme4")
    dpg.delete_item("stem_theme1")
    dpg.delete_item("__demo_keyboard_handler")
    dpg.delete_item("__demo_mouse_handler")
    dpg.delete_item("__demo_filedialog")
    dpg.delete_item("__demo_stage1")
    dpg.delete_item("__demo_popup1")
    dpg.delete_item("__demo_popup2")
    dpg.delete_item("__demo_popup3")
    dpg.delete_item("__demo_item_reg3")
    dpg.delete_item("__demo_item_reg6")
    dpg.delete_item("__demo_item_reg7")
    dpg.delete_item("demoitemregistry")
    for i in range(7):
        dpg.delete_item("__demo_theme"+str(i))
        dpg.delete_item("__demo_theme2_"+str(i))
    for i in range(5):
        dpg.delete_item("__demo_item_reg1_"+str(i))
        dpg.delete_item("__demo_item_reg2_"+str(i))
    for i in range(3):
        dpg.delete_item("__demo_item_reg4_"+str(i))
    for i in range(4):
        dpg.delete_item("__demo_item_reg5_"+str(i))

def show_demo():

    dpg.add_texture_registry(label="Demo Texture Container", tag="__demo_texture_container")
    dpg.add_colormap_registry(label="Demo Colormap Registry", tag="__demo_colormap_registry")

    with dpg.theme(tag="__demo_hyperlinkTheme"):
        with dpg.theme_component(dpg.mvButton):
            dpg.add_theme_color(dpg.mvThemeCol_Button, [0, 0, 0, 0])
            dpg.add_theme_color(dpg.mvThemeCol_ButtonActive, [0, 0, 0, 0])
            dpg.add_theme_color(dpg.mvThemeCol_ButtonHovered, [29, 151, 236, 25])
            dpg.add_theme_color(dpg.mvThemeCol_Text, [29, 151, 236])

    def _log(sender, app_data, user_data):
        print(f"sender: {sender}, \t app_data: {app_data}, \t user_data: {user_data}")

    _create_static_textures()
    _create_dynamic_textures()

    with dpg.window(label="Dear PyGui Demo", width=800, height=800, on_close=_on_demo_close, pos=(100, 100), tag="__demo_id"):
    
        with dpg.menu_bar():

            with dpg.menu(label="Menu"):

                dpg.add_text("This menu is just for show!")
                dpg.add_menu_item(label="New")
                dpg.add_menu_item(label="Open")

                with dpg.menu(label="Open Recent"):

                    dpg.add_menu_item(label="harrel.c")
                    dpg.add_menu_item(label="patty.h")
                    dpg.add_menu_item(label="nick.py")

                dpg.add_menu_item(label="Save")
                dpg.add_menu_item(label="Save As...")

                with dpg.menu(label="Settings"):

                    dpg.add_menu_item(label="Option 1", callback=_log)
                    dpg.add_menu_item(label="Option 2", check=True, callback=_log)
                    dpg.add_menu_item(label="Option 3", check=True, default_value=True, callback=_log)

                    with dpg.child_window(height=60, autosize_x=True, delay_search=True):
                        for i in range(10):
                            dpg.add_text(f"Scolling Text{i}")

                    dpg.add_slider_float(label="Slider Float")
                    dpg.add_input_int(label="Input Int")
                    dpg.add_combo(("Yes", "No", "Maybe"), label="Combo")

            with dpg.menu(label="Tools"):

                dpg.add_menu_item(label="Show About", callback=lambda:dpg.show_tool(dpg.mvTool_About))
                dpg.add_menu_item(label="Show Metrics", callback=lambda:dpg.show_tool(dpg.mvTool_Metrics))
                dpg.add_menu_item(label="Show Documentation", callback=lambda:dpg.show_tool(dpg.mvTool_Doc))
                dpg.add_menu_item(label="Show Debug", callback=lambda:dpg.show_tool(dpg.mvTool_Debug))
                dpg.add_menu_item(label="Show Style Editor", callback=lambda:dpg.show_tool(dpg.mvTool_Style))
                dpg.add_menu_item(label="Show Font Manager", callback=lambda:dpg.show_tool(dpg.mvTool_Font))
                dpg.add_menu_item(label="Show Item Registry", callback=lambda:dpg.show_tool(dpg.mvTool_ItemRegistry))

            with dpg.menu(label="Settings"):

                dpg.add_menu_item(label="Wait For Input", check=True, callback=lambda s, a: dpg.configure_app(wait_for_input=a))
                dpg.add_menu_item(label="Toggle Fullscreen", callback=lambda:dpg.toggle_viewport_fullscreen())

        with dpg.group(horizontal=True):
            dpg.add_loading_indicator(circle_count=4)
            with dpg.group():
                dpg.add_text(f'Dear PyGui says hello. ({dpg.get_dearpygui_version()})')
                with dpg.group(horizontal=True):
                    dpg.add_text("The code for the most recent version of this demo can be found here:")
                    _hyperlink("demo.py", "https://github.com/hoffstadt/DearPyGui/blob/master/dearpygui/demo.py")

        with dpg.collapsing_header(label="Window Options"):

            _add_config_options("__demo_id", 3, 
                                        "no_title_bar", "no_scrollbar", "menubar", 
                                        "no_move", "no_resize", "no_collapse",
                                        "no_close", "no_background", "no_bring_to_front_on_focus"
                                        )
  
        with dpg.collapsing_header(label="Widgets"):
        
            with dpg.tree_node(label="Basic"):

                with dpg.group(horizontal=True):
                    dpg.add_button(label="Button", callback=_log)
                    dpg.add_button(label="Button", callback=_log, small=True)
                    dpg.add_button(label="Button", callback=_log, arrow=True) # default direction is mvDir_Up
                    dpg.add_button(label="Button", callback=_log, arrow=True, direction=dpg.mvDir_Left)
                    dpg.add_button(label="Button", callback=_log, arrow=True, direction=dpg.mvDir_Right)
                    dpg.add_button(label="Button", callback=_log, arrow=True, direction=dpg.mvDir_Down)

                dpg.add_checkbox(label="checkbox", callback=_log)
                dpg.add_radio_button(("radio a", "radio b", "radio c"), callback=_log, horizontal=True)
                dpg.add_selectable(label="selectable", callback=_log)

                with dpg.group(horizontal=True):

                    for i in range(7):

                        with dpg.theme(tag="__demo_theme"+str(i)):
                            with dpg.theme_component(dpg.mvButton):
                                dpg.add_theme_color(dpg.mvThemeCol_Button, _hsv_to_rgb(i/7.0, 0.6, 0.6))
                                dpg.add_theme_color(dpg.mvThemeCol_ButtonActive, _hsv_to_rgb(i/7.0, 0.8, 0.8))
                                dpg.add_theme_color(dpg.mvThemeCol_ButtonHovered, _hsv_to_rgb(i/7.0, 0.7, 0.7))
                                dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, i*5)
                                dpg.add_theme_style(dpg.mvStyleVar_FramePadding, i*3, i*3)

                        dpg.add_button(label="Click", callback=_log)
                        dpg.bind_item_theme(dpg.last_item(), "__demo_theme"+str(i))

                with dpg.group(horizontal=True):

                    dpg.add_text("Press a button: ")
                    widget = dpg.add_text("0")
                    dpg.add_button(arrow=True, direction=dpg.mvDir_Left, user_data=widget, callback=lambda s, a, u: dpg.set_value(u, int(dpg.get_value(u))-1))
                    dpg.add_button(arrow=True, direction=dpg.mvDir_Right, user_data=widget, callback=lambda s, a, u: dpg.set_value(u, int(dpg.get_value(u))+1))

                dpg.add_text("hover me")
                with dpg.tooltip(dpg.last_item()):
                    dpg.add_text("I'm a simple tooltip!")

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
                dpg.add_input_float(label="input float", callback=_log, format="%.06f")
                dpg.add_input_float(label="input float scientific", format="%e", callback=_log)
                dpg.add_input_floatx(label="input floatx", callback=_log, default_value=[1,2,3,4])
                dpg.add_input_double(label="input double", callback=_log, format="%.14f")
                dpg.add_input_doublex(label="input doublex", callback=_log, default_value=[1,2,3,4], format="%.14f")
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
                dpg.add_slider_double(label="slider double", max_value=1.0, format="ratio = %.14f", callback=_log)
                dpg.add_slider_int(label="slider angle", min_value=-360, max_value=360, format="%d deg", callback=_log)
                _help(
                        "Click on the colored square to open a color picker.\n"
                        "Click and hold to use drag and drop.\n"
                        "Right-click on the colored square to show options.\n"
                        "CTRL+click on individual component to input value.\n")
                dpg.add_color_edit((102, 179, 0, 128), label="color edit 4", callback=_log)
                dpg.add_color_edit(default_value=(.5, 1, .25, .1), label="color edit 4", callback=_log)
                dpg.add_listbox(("Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"), label="listbox", num_items=4, callback=_log)
                dpg.add_color_button()

            with dpg.tree_node(label="Combo"):

                items = ("A","B","C","D","E","F","G","H","I","J","K","L","M" "O","P","Q","R","S","T","U","V","W","X","Y","Z")
                combo_id = dpg.add_combo(items, label="combo", height_mode=dpg.mvComboHeight_Small)
                _add_config_options(combo_id, 1, "popup_align_left", "no_arrow_button", "no_preview")

            with dpg.tree_node(label="Color Picker & Edit"):

                _color_picker_id = dpg.generate_uuid()
                _color_edit_id = dpg.generate_uuid()

                def _color_picker_configs(sender, value, user_data):

                    _old_config = dpg.get_item_configuration(user_data)

                    if user_data ==_color_picker_id:
                        picker_mode = _old_config["picker_mode"]
                    else:
                        display_mode = _old_config["display_mode"]
                    alpha_preview = _old_config["alpha_preview"]
                    display_type = _old_config["display_type"]
                    input_mode = _old_config["input_mode"]

                    # picker_mode
                    if value == "mvColorPicker_bar":
                        picker_mode=dpg.mvColorPicker_bar
                    elif value == "mvColorPicker_wheel":
                        picker_mode = dpg.mvColorPicker_wheel

                    # alpha_preview
                    elif value == "mvColorEdit_AlphaPreviewNone":
                        alpha_preview=dpg.mvColorEdit_AlphaPreviewNone
                    elif value == "mvColorEdit_AlphaPreview":
                        alpha_preview = dpg.mvColorEdit_AlphaPreview
                    elif value == "mvColorEdit_AlphaPreviewHalf":
                        alpha_preview = dpg.mvColorEdit_AlphaPreviewHalf

                    # display_type
                    elif value == "mvColorEdit_uint8":
                        display_type=dpg.mvColorEdit_uint8
                    elif value == "mvColorEdit_float":
                        display_type = dpg.mvColorEdit_float

                    # display_type
                    elif value == "mvColorEdit_uint8":
                        display_type=dpg.mvColorEdit_uint8
                    elif value == "mvColorEdit_float":
                        display_type = dpg.mvColorEdit_float

                    # display_mode
                    elif value == "mvColorEdit_rgb":
                        display_mode=dpg.mvColorEdit_rgb
                    elif value == "mvColorEdit_hsv":
                        display_mode = dpg.mvColorEdit_hsv
                    elif value == "mvColorEdit_hex":
                        display_mode = dpg.mvColorEdit_hex

                    if user_data ==_color_picker_id:
                        dpg.configure_item(user_data, 
                                           picker_mode=picker_mode, 
                                           alpha_preview=alpha_preview,
                                           display_type=display_type,
                                           input_mode=input_mode
                                           )
                    else:
                        dpg.configure_item(user_data, 
                                           alpha_preview=alpha_preview,
                                           display_type=display_type,
                                           input_mode=input_mode,
                                           display_mode=display_mode
                                           )

                dpg.add_text("Color Picker")

                with dpg.group(horizontal=True):
                    _before_id = dpg.add_text("picker_mode:")
                    dpg.add_radio_button(("mvColorPicker_bar", "mvColorPicker_wheel"), callback=_color_picker_configs, 
                                         user_data=_color_picker_id, horizontal=True)
                
                with dpg.group(horizontal=True):
                    dpg.add_text("alpha_preview:")
                    dpg.add_radio_button(("mvColorEdit_AlphaPreviewNone", "mvColorEdit_AlphaPreview", "mvColorEdit_AlphaPreviewHalf"), callback=_color_picker_configs, 
                                     user_data=_color_picker_id, horizontal=True)

                with dpg.group(horizontal=True):
                    dpg.add_text("display_type:")
                    dpg.add_radio_button(("mvColorEdit_uint8", "mvColorEdit_float"), callback=_color_picker_configs, 
                                         user_data=_color_picker_id, horizontal=True)

                with dpg.group(horizontal=True):
                    dpg.add_text("input_mode:")
                    dpg.add_radio_button(("mvColorEdit_input_rgb", "mvColorEdit_input_hsv"), callback=_color_picker_configs, 
                                         user_data=_color_picker_id, horizontal=True)

                dpg.add_color_picker((255, 0, 255, 255), label="Color Picker", 
                        width=200, tag=_color_picker_id)

                _add_config_options(_color_picker_id, 3, 
                                    "no_alpha", "no_side_preview", "display_rgb",
                                    "display_hsv", "display_hex",
                                    "no_small_preview", "no_inputs", "no_tooltip",
                                    "no_label", "alpha_bar", before=_before_id)

                dpg.add_separator()

                dpg.add_text("Color Edit")

                with dpg.group(horizontal=True):
                    _before_id = dpg.add_text("alpha_preview:")
                    dpg.add_radio_button(("mvColorEdit_AlphaPreviewNone", "mvColorEdit_AlphaPreview", "mvColorEdit_AlphaPreviewHalf"), callback=_color_picker_configs, 
                                         user_data=_color_edit_id, horizontal=True)

                with dpg.group(horizontal=True):
                    dpg.add_text("display_type:")
                    dpg.add_radio_button(("mvColorEdit_uint8", "mvColorEdit_float"), callback=_color_picker_configs, 
                                         user_data=_color_edit_id, horizontal=True)

                with dpg.group(horizontal=True):
                    dpg.add_text("display_mode:")
                    dpg.add_radio_button(("mvColorEdit_rgb", "mvColorEdit_hsv", "mvColorEdit_hex"), callback=_color_picker_configs, 
                                         user_data=_color_edit_id, horizontal=True)

                with dpg.group(horizontal=True):
                    dpg.add_text("input_mode:")
                    dpg.add_radio_button(("mvColorEdit_input_rgb", "mvColorEdit_input_hsv"), callback=_color_picker_configs, 
                                         user_data=_color_edit_id, horizontal=True)

                dpg.add_color_edit((255, 0, 255, 255), label="Color Edit", 
                        width=200, tag=_color_edit_id)

                _add_config_options(_color_edit_id, 3, 
                                    "no_alpha", "no_picker",
                                    "no_options", "no_inputs", "no_small_preview",
                                    "no_tooltip", "no_label", 
                                    "no_drag_drop", "alpha_bar", before=_before_id)
   
            with dpg.tree_node(label="Colormaps"):
                dpg.add_text("Notes")
                dpg.add_text("Colormaps belong to a mvColorMapRegistry.", bullet=True, indent=20)
                with dpg.group(horizontal=True):
                    dpg.add_text("Showing the registry will help with debugging. Press ", bullet=True, indent=20)
                    dpg.add_button(label="here", small=True, callback=lambda:dpg.show_item("__demo_colormap_registry"))
                dpg.add_text("Colormaps are applied with 'bind_colormap(item_id, colormap_id)", bullet=True, indent=20)
                dpg.add_text("Colormaps can be applied to mvPlot, mvColorMapButton, mvColorMapSlider, mvColorMapScale", bullet=True, indent=20)
                dpg.add_text("Colormaps can be sampled with 'sample_colormap(colormap_id, t)' (0<t<1)", bullet=True, indent=20)
                dpg.add_text("Colormaps can be sampled by index with 'get_colormap_color(colormap_id, index)'", bullet=True, indent=20)
                dpg.add_colormap([[0, 0, 0, 255], [255, 255, 255, 255]], False, tag="__demo_colormap_1", parent="__demo_colormap_registry", label="Demo 1")
                dpg.add_colormap([[255, 0, 0], [0, 255, 0], [0, 0, 255]], True, tag="__demo_colormap_2", parent="__demo_colormap_registry", label="Demo 2")
                dpg.add_colormap_button(label="Colormap Button 1")
                dpg.bind_colormap(dpg.last_item(), "__demo_colormap_1")
                dpg.add_colormap_button(label="Colormap Button 2")
                dpg.bind_colormap(dpg.last_item(), "__demo_colormap_2")
                dpg.add_colormap_slider(label="Colormap Slider 1", default_value=0.5)
                dpg.bind_colormap(dpg.last_item(), "__demo_colormap_1")
                dpg.add_colormap_slider(label="Colormap Slider 2")
                dpg.bind_colormap(dpg.last_item(), "__demo_colormap_2")
                with dpg.group(horizontal=True):
                    dpg.add_colormap_scale(label="Colormap Slider 1")
                    dpg.bind_colormap(dpg.last_item(), "__demo_colormap_1")
                    dpg.add_colormap_scale(label="Colormap Slider 2")
                    dpg.bind_colormap(dpg.last_item(), "__demo_colormap_2")
                    dpg.add_colormap_scale(label="Colormap Spectral", min_scale=-100, max_scale=150)
                    dpg.bind_colormap(dpg.last_item(), dpg.mvPlotColormap_Spectral)

            with dpg.tree_node(label="List Boxes"):
                items = ("A","B","C","D","E","F","G","H","I","J","K","L","M" "O","P","Q","R","S","T","U","V","W","X","Y","Z")
                listbox_1 = dpg.add_listbox(items, label="listbox 1 (full)")
                listbox_2 = dpg.add_listbox(items, label="listbox 2", width=200)
                dpg.add_input_int(label="num_items",callback=_config, user_data=[listbox_1, listbox_2], before = listbox_1)
                dpg.add_slider_int(label="width", default_value=200, callback=_config, user_data=listbox_2, before = listbox_1, max_value=500)
        
            with dpg.tree_node(label="Selectables"):
                
                with dpg.tree_node(label="Basic"):
                    dpg.add_selectable(label="1. I am selectable")
                    dpg.add_text("2. I am not selectable")

                with dpg.tree_node(label="Selection State: Single"):

                    def _selection(sender, app_data, user_data):
                        for item in user_data:
                            if item != sender:
                               dpg.set_value(item, False)
                    items = (
                        dpg.add_selectable(label="1. I am selectable"),
                        dpg.add_selectable(label="2. I am selectable"),
                        dpg.add_selectable(label="3. I am selectable"),
                        dpg.add_selectable(label="4. I am selectable"),
                        dpg.add_selectable(label="5. I am selectable"),
                        )

                    for item in items:
                        dpg.configure_item(item, callback=_selection, user_data=items)

            with dpg.tree_node(label="Bullets"):

                dpg.add_text("Bullet point 1", bullet=True)
                dpg.add_text("Bullet point 2\nbullet text can be\nOn multiple lines", bullet=True)
                with dpg.tree_node(label="Tree node"):
                    dpg.add_text("Another bullet point", bullet=True)
                
                with dpg.group(horizontal=True):
                    dpg.add_text("1", bullet=True)
                    dpg.add_button(label="Button", small=True)

            with dpg.tree_node(label="Text"):

                with dpg.tree_node(label="Colored Text"):
                
                    dpg.add_text("Pink", color=(255, 0, 255))
                    dpg.add_text("Yellow", color=(255, 255, 0))

                with dpg.tree_node(label="Word Wrapping"):

                    paragraph1 = 'This text should automatically wrap on the edge of the window.The current implementation for the text wrapping follows simple rules suited for English and possibly other languages'
                    paragraph2 = 'The lazy dong is a good dog. This paragraph should fit within the child. Testing a 1 character word. The quick brown fox jumps over the lazy dog.'

                    dpg.add_text(paragraph1, wrap=0)
                    widget_id = dpg.add_slider_int(label="wrap width", default_value=500, max_value=1000, 
                                       callback=lambda s, a, u: dpg.configure_item(u, wrap=a))
                    dpg.add_text(paragraph2, wrap=500)
                    dpg.configure_item(widget_id, user_data=dpg.last_item())

            with dpg.tree_node(label="Text Input"):

                with dpg.tree_node(label="Multi-line Text Input"):

                    paragraph = """/*\n
                        The Pentium F00F bug, shorthand for F0 0F C7 C8,\n
                        the hexadecimal encoding of one offending instruction,\n
                        more formally, the invalid operand with locked CMPXCHG8B\n
                        instruction bug, is a design flaw in the majority of\n
                        Intel Pentium, Pentium MMX, and Pentium OverDrive\n
                        processors (all in the P5 microarchitecture).\n
                        */\n\n
                        label:\n
                        \tlock cmpxchg8b eax\n"""

                    dpg.add_input_text(label="input text", multiline=True, default_value=paragraph, height=300, callback=_log, tab_input=True)

                    _add_config_options(dpg.last_item(), 1, 
                            "readonly", "on_enter")

                with dpg.tree_node(label="Filtered Text Input"):

                    dpg.add_input_text(callback=_log, label="default")
                    dpg.add_input_text(callback=_log, label="decimal", decimal=True)
                    dpg.add_input_text(callback=_log, label="no blank", no_spaces=True)
                    dpg.add_input_text(callback=_log, label="uppercase", uppercase=True)
                    dpg.add_input_text(callback=_log, label="scientific", scientific=True)
                    dpg.add_input_text(callback=_log, label="hexdecimal", hexadecimal=True)
            
                with dpg.tree_node(label="Password Input"):

                    password = dpg.add_input_text(label="password", password=True, callback=_log)
                    dpg.add_input_text(label="password (w/ hint)", password=True, hint="<password>", source=password, callback=_log)
                    dpg.add_input_text(label="password (clear)", source=password, callback=_log)

            with dpg.tree_node(label="Simple Plots"):

                data = (0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2)
                dpg.add_simple_plot(label="Frame Times", default_value=data)
                dpg.add_simple_plot(label="Histogram", default_value=data, height=80, histogram=True, min_scale=0.0)

                data1 = []
                for i in range(70):
                    data1.append(cos(3.14*6*i/180))

                dpg.add_simple_plot(label="Lines", default_value=data1, height=80)
                dpg.add_simple_plot(label="Histogram", default_value=data1, height=80, histogram=True)
                with dpg.group(horizontal=True):
                    dpg.add_progress_bar(label="Progress Bar", default_value=0.78, overlay="78%")
                    dpg.add_text("Progress Bar")
                with dpg.theme(tag="__demo_theme_progressbar"):
                    with dpg.theme_component(dpg.mvProgressBar):
                        dpg.add_theme_color(dpg.mvThemeCol_PlotHistogram, (255,0,0, 255))
                dpg.add_progress_bar(default_value=0.78, overlay="1367/1753")
                dpg.bind_item_theme(dpg.last_item(), "__demo_theme_progressbar")

            with dpg.tree_node(label="Multi-component Widgets"):

                for i in range(2, 5):

                    with dpg.group():
                        float_source = dpg.add_input_floatx(label=f"input float {i}", min_value=0.0, max_value=100.0, size=i)
                        dpg.add_drag_floatx(label=f"drag float {i}", source=float_source, size=i)
                        dpg.add_slider_floatx(label=f"slider float {i}", source=float_source, size=i)

                    with dpg.group():
                        double_source = dpg.add_input_doublex(label=f"input double {i}", min_value=0.0, max_value=100.0, size=i)
                        dpg.add_drag_doublex(label=f"drag double {i}", source=double_source, size=i)
                        dpg.add_slider_doublex(label=f"slider double {i}", source=double_source, size=i)

                    with dpg.group():

                        int_source = dpg.add_input_intx(label=f"input int {i}", min_value=0, max_value=100, size=i)
                        dpg.add_drag_intx(label=f"drag int {i}", source=int_source, size=i)
                        dpg.add_slider_intx(label=f"slider int {i}", source=int_source, size=i)
            
                    dpg.add_spacer(height=10)

            with dpg.tree_node(label="Vertical Sliders"):

                with dpg.group(horizontal=True):
                    dpg.add_slider_int(label=" ", default_value=1, vertical=True, max_value=5, height=160)
                    dpg.add_slider_double(label=" ", default_value=1.0, vertical=True, max_value=5.0, height=160)

                    with dpg.group(horizontal=True):

                        with dpg.group(horizontal=True):

                            values = [ 0.0, 0.60, 0.35, 0.9, 0.70, 0.20, 0.0 ]

                            for i in range(7):

                                with dpg.theme(tag="__demo_theme2_"+str(i)):
                                    with dpg.theme_component(0):
                                        dpg.add_theme_color(dpg.mvThemeCol_FrameBg, _hsv_to_rgb(i/7.0, 0.5, 0.5))
                                        dpg.add_theme_color(dpg.mvThemeCol_SliderGrab, _hsv_to_rgb(i/7.0, 0.9, 0.9))
                                        dpg.add_theme_color(dpg.mvThemeCol_FrameBgActive, _hsv_to_rgb(i/7.0, 0.7, 0.5))
                                        dpg.add_theme_color(dpg.mvThemeCol_FrameBgHovered, _hsv_to_rgb(i/7.0, 0.6, 0.5))

                                dpg.add_slider_float(label=" ", default_value=values[i], vertical=True, max_value=1.0, height=160)
                                dpg.bind_item_theme(dpg.last_item(), "__demo_theme2_"+str(i))

                        with dpg.group():
                            for i in range(3):
                                with dpg.group(horizontal=True):
                                    values = [ 0.20, 0.80, 0.40, 0.25 ]
                                    for j in range(4):
                                        dpg.add_slider_float(label=" ", default_value=values[j], vertical=True, max_value=1.0, height=50)

                        with dpg.group(horizontal=True):
                            dpg.add_slider_float(label=" ", vertical=True, max_value=1.0, height=160, width=40)
                            dpg.add_slider_float(label=" ", vertical=True, max_value=1.0, height=160, width=40)
                            dpg.add_slider_float(label=" ", vertical=True, max_value=1.0, height=160, width=40)
                            dpg.add_slider_float(label=" ", vertical=True, max_value=1.0, height=160, width=40)

            with dpg.tree_node(label="Time/Date Widgets"):

                dpg.add_time_picker(default_value={'hour': 14, 'min': 32, 'sec': 23})
                dpg.add_separator()
            
                with dpg.table(header_row=False):

                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()

                    with dpg.table_row():
                        dpg.add_date_picker(level=dpg.mvDatePickerLevel_Day, default_value={'month_day': 8, 'year':93, 'month':5})
                        dpg.add_date_picker(level=dpg.mvDatePickerLevel_Month, default_value={'month_day': 8, 'year':93, 'month':5})
                        dpg.add_date_picker(level=dpg.mvDatePickerLevel_Year, default_value={'month_day': 8, 'year':93, 'month':5})

            with dpg.tree_node(label="Loading Indicators"):

                with dpg.group(horizontal=True):
                    dpg.add_loading_indicator()
                    dpg.add_loading_indicator(style=1)

            with dpg.tree_node(label="Knobs"):

                with dpg.group(horizontal=True):

                    dpg.add_knob_float(label="K1")
                    dpg.add_knob_float(label="K2", default_value=25.0)
                    dpg.add_knob_float(label="K3", default_value=50.0)

            with dpg.tree_node(label="2D/3D Sliders"):

                dpg.add_3d_slider(label="3D Slider", scale=0.5)

        with dpg.collapsing_header(label="Layout & Scrolling"):

            with dpg.tree_node(label="Widgets Width"):
                
                dpg.add_text("Width=100")
                dpg.add_drag_float(label="float", width=100)

                dpg.add_text("Width=-100")
                dpg.add_drag_float(label="float", width=-100)

                dpg.add_text("Width=-1")
                dpg.add_drag_float(label="float", width=-1)

                dpg.add_text("group with width=75")
                with dpg.group(width=75):
                    dpg.add_drag_float(label="float")
                    dpg.add_drag_float(label="float")
                    dpg.add_drag_float(label="float")

            with dpg.tree_node(label="Basic Horizontal Layout"):

                dpg.add_text("(Use groups with horizontal set to True to keep adding items to the right of the preceding item)", bullet=True)
                with dpg.group(horizontal=True):
                    dpg.add_text("Normal buttons")
                    dpg.add_button(label="Banana")
                    dpg.add_button(label="Apple")
                    dpg.add_button(label="Corniflower")

                with dpg.group(horizontal=True):
                    dpg.add_text("Small buttons")
                    dpg.add_button(label="Like this one", small=True)
                    dpg.add_text("can fit within a text block")

                with dpg.group(horizontal=True, xoffset=150):
                    dpg.add_text("Aligned")
                    dpg.add_text("x=150")
                    dpg.add_text("x=300")

                with dpg.group(horizontal=True, xoffset=150):
                    dpg.add_text(default_value="Aligned")
                    dpg.add_button(label="x=150", small=True)
                    dpg.add_button(label="x=300", small=True)

                with dpg.group(horizontal=True):
                    dpg.add_checkbox(label="My")
                    dpg.add_checkbox(label="Tailor")
                    dpg.add_checkbox(label="is")
                    dpg.add_checkbox(label="rich")

                dpg.add_text("Lists:")
                with dpg.group(horizontal=True):
                    dpg.add_listbox(("AAAA", "BBBB", "CCCC", "DDDD"), default_value="AAAA", width=100, label="")
                    dpg.add_listbox(("AAAA", "BBBB", "CCCC", "DDDD"), default_value="BBBB", width=100, label="")
                    dpg.add_listbox(("AAAA", "BBBB", "CCCC", "DDDD"), default_value="CCCC", width=100, label="")
                    dpg.add_listbox(("AAAA", "BBBB", "CCCC", "DDDD"), default_value="DDDD", width=100, label="")
                
                dpg.add_text("Spacing(100):")
                with dpg.group(horizontal=True, horizontal_spacing=100):
                    dpg.add_button(label="A", width=50, height=50)
                    dpg.add_button(label="B", width=50, height=50)

            with dpg.tree_node(label="Ordered pack style"):
                dpg.add_button(label="Button 1")
                dpg.add_button(label="Button 2")
                dpg.add_button(label="Button 3")

            with dpg.tree_node(label="Absolute Position Placement"):
                dpg.add_button(label="Set Button 2 Pos", callback=lambda: dpg.set_item_pos(B2, (50, 125)))
                dpg.add_button(label="Reset Button 2 Pos", callback=lambda: dpg.reset_pos(B2))
                dpg.add_button(label="Button 1", pos=[50,50], width=75, height=75)
                B2 = dpg.add_button(label="Button 2", width=75, height=75)
                dpg.add_button(label="Button 3")
        
            with dpg.tree_node(label="Grid Layout using Table API"):
                layout_demo_table = dpg.generate_uuid()
                dpg.add_text("Tables can be used to layout items in an equally spaced grid pattern.")
                dpg.add_text("See tables section for more detail on tables.")
                dpg.add_checkbox(label="resizable", callback=_config, user_data=layout_demo_table)
                dpg.add_checkbox(label="borders_innerH", callback=_config, user_data=layout_demo_table, default_value=True)
                dpg.add_checkbox(label="borders_outerH", callback=_config, user_data=layout_demo_table, default_value=True)
                dpg.add_checkbox(label="borders_innerV", callback=_config, user_data=layout_demo_table, default_value=True)
                dpg.add_checkbox(label="borders_outerV", callback=_config, user_data=layout_demo_table, default_value=True)
                with dpg.table(tag=layout_demo_table, header_row=False, borders_innerH=True, 
                               borders_outerH=True, borders_innerV=True, borders_outerV=True):
                    
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()

                    with dpg.table_row():
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    
                    with dpg.table_row():
                        dpg.add_table_cell()
                        dpg.add_button(label="Button 4")
                        dpg.add_button(label="Button 5")

            with dpg.tree_node(label="Containers"):

                with dpg.tree_node(label="Tree Nodes"):
                    with dpg.tree_node(label="Tree Node (selectable)", selectable=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with dpg.tree_node(label="Tree Node (bullet)", bullet=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                
                with dpg.tree_node(label="Groups"):
                    dpg.add_text("Groups are used to control child items placement, width, and provide a hit box for things like is the set of items are hovered, ect...")
                    with dpg.group(horizontal=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with dpg.group(width=150):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with dpg.group():
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                
                with dpg.tree_node(label="Child windows"):
                    demo_layout_child = dpg.generate_uuid()
                    dpg.add_text("Child windows are basically embedded windows and provide much more structure and control of the containing items than groups.")
                    with dpg.group(horizontal=True):
                        dpg.add_checkbox(label="autosize_x", callback=_config, user_data=demo_layout_child)
                        dpg.add_checkbox(label="autosize_y", callback=_config, user_data=demo_layout_child)
                        dpg.add_checkbox(label="menubar", callback=_config, user_data=demo_layout_child)
                        dpg.add_checkbox(label="no_scrollbar", callback=_config, user_data=demo_layout_child)
                        dpg.add_checkbox(label="horizontal_scrollbar", callback=_config, user_data=demo_layout_child)
                        dpg.add_checkbox(label="border", default_value=True, callback=_config, user_data=demo_layout_child)
                    with dpg.child_window(tag=demo_layout_child, width=200, height=200):
                        with dpg.menu_bar():
                            with dpg.menu(label="Menu"):
                                pass
                        for i in range(20):
                            dpg.add_text(default_value="A pretty long sentence if you really think about it. It's also pointless. we need this to be even longer")
                    with dpg.child_window(autosize_x=True, height=130, menubar=True):
                        with dpg.menu_bar():
                            dpg.add_menu(label="Menu Options")
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with dpg.group(horizontal=True):
                        with dpg.child_window(width=100, height=150, horizontal_scrollbar=True):
                            dpg.add_button(label="Button 1")
                            dpg.add_button(label="Button 2")
                            dpg.add_button(label="Button 3")
                            dpg.add_button(label="Button 4", width=150)
                            dpg.add_button(label="Button 5")
                            dpg.add_button(label="Button 6")
                        with dpg.child_window(width=100, height=110):
                            dpg.add_button(label="Button 1")
                            dpg.add_button(label="Button 2")
                            dpg.add_button(label="Button 3")
                
                with dpg.tree_node(label="Collapsing Headers"):
                    with dpg.collapsing_header(label="Collapsing Header"):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with dpg.collapsing_header(label="Collapsing Header (close)", closable=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with dpg.collapsing_header(label="Collapsing Header (bullet)", bullet=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with dpg.collapsing_header(label="Collapsing Header (leaf)", leaf=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                
                with dpg.tree_node(label="Tabs"):

                    with dpg.tree_node(label="Basic"):

                        with dpg.tab_bar():
                            
                            with dpg.tab(label="Avocado"):
                                dpg.add_text("This is the avocado tab!")
                            
                            with dpg.tab(label="Broccoli"):
                                dpg.add_text("This is the broccoli tab!")

                            with dpg.tab(label="Cucumber"):
                                dpg.add_text("This is the cucumber tab!")

                    with dpg.tree_node(label="Advanced"):

                        with dpg.tab_bar() as tb:

                            with dpg.tab(label="tab 1"):
                                dpg.add_text("This is the tab 1!")

                            with dpg.tab(label="tab 2") as t2:
                                dpg.add_text("This is the tab 2!")

                            with dpg.tab(label="tab 3"):
                                dpg.add_text("This is the tab 3!")

                            with dpg.tab(label="tab 4"):
                                dpg.add_text("This is the tab 4!")

                            tbb = dpg.add_tab_button(label="+")
                            dpg.add_tab_button(label="?")

                            #dpg.add_checkbox(before=tb, label="tab bar reorderable", user_data=tb, callback=lambda s, a, u: dpg.configure_item(u, reorderable=dpg.get_value(s)))
                            #dpg.add_checkbox(before=tb, label="tab 2 no_reorder", user_data=t2, callback=lambda s, a, u: dpg.configure_item(u, no_reorder=dpg.get_value(s)))
                            #dpg.add_checkbox(before=tb, label="tab 2 leading", user_data=t2, callback=lambda s, a, u: dpg.configure_item(u, leading=dpg.get_value(s)))
                            #dpg.add_checkbox(before=tb, label="tab 2 trailing", user_data=t2, callback=lambda s, a, u: dpg.configure_item(u, trailing=dpg.get_value(s)))
                            #dpg.add_checkbox(before=tb, label="tab button trailing", user_data=tbb, callback=lambda s, a, u: dpg.configure_item(u, trailing=dpg.get_value(s)))
                            #dpg.add_checkbox(before=tb, label="tab button leading", user_data=tbb, callback=lambda s, a, u: dpg.configure_item(u, leading=dpg.get_value(s)))

            with dpg.tree_node(label="Simple Layouts"):
                dpg.add_text("Containers can be nested for advanced layout options")
                with dpg.child_window(width=500, height=320, menubar=True):
                    with dpg.menu_bar():
                        dpg.add_menu(label="Menu Options")
                    with dpg.child_window(autosize_x=True, height=95):
                        with dpg.group(horizontal=True):
                            dpg.add_button(label="Header 1", width=75, height=75)
                            dpg.add_button(label="Header 2", width=75, height=75)
                            dpg.add_button(label="Header 3", width=75, height=75)
                    with dpg.child_window(autosize_x=True, height=175):
                        with dpg.group(horizontal=True, width=0):
                            with dpg.child_window(width=102, height=150):
                                with dpg.tree_node(label="Nav 1"):
                                    dpg.add_button(label="Button 1")
                                with dpg.tree_node(label="Nav 2"):
                                    dpg.add_button(label="Button 2")
                                with dpg.tree_node(label="Nav 3"):
                                    dpg.add_button(label="Button 3")
                            with dpg.child_window(width=300, height=150):
                                dpg.add_button(label="Button 1")
                                dpg.add_button(label="Button 2")
                                dpg.add_button(label="Button 3")
                            with dpg.child_window(width=50, height=150):
                                dpg.add_button(label="B1", width=25, height=25)
                                dpg.add_button(label="B2", width=25, height=25)
                                dpg.add_button(label="B3", width=25, height=25)
                    with dpg.group(horizontal=True):
                        dpg.add_button(label="Footer 1", width=175)
                        dpg.add_text("Footer 2")
                        dpg.add_button(label="Footer 3", width=175)

            with dpg.tree_node(label="Scrolling"):

                def _update_xscroll_info(sender, app_data, user_data):
                    
                    with dpg.mutex():
                        x_scroll = dpg.get_x_scroll(user_data[1])
                        max_scroll = dpg.get_x_scroll_max(user_data[1])
                        dpg.set_value(user_data[0], str(x_scroll) + "/" + str(max_scroll))

                def _update_yscroll_info(sender, app_data, user_data):
                    
                    with dpg.mutex():
                        y_scroll = dpg.get_y_scroll(user_data[1])
                        max_scroll = dpg.get_y_scroll_max(user_data[1])
                        dpg.set_value(user_data[0], str(y_scroll) + "/" + str(max_scroll))

                with dpg.table(header_row=False):

                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()

                    children = []
                    text_items = ("Top", "25%", "Center", "75%", "Bottom")
                    track_items = (0.0, 0.25, 0.5, 0.75, 1.0)

                    with dpg.table_row():
                        for i in range(5):
                            with dpg.table_cell():
                                dpg.add_text(text_items[i])
                                with dpg.child_window(height=200, delay_search=True) as _child_id:
                                    for j in range(25):
                                        if j == 13:
                                            dpg.add_text("Item " + str(j), color=(255, 255, 0), tracked=True, track_offset=track_items[i])
                                        else:
                                            dpg.add_text("Item " + str(j))
                        
                                _text_id = dpg.add_text("0/0")
                                with dpg.item_handler_registry(tag="__demo_item_reg1_"+str(i)):
                                    dpg.add_item_visible_handler(user_data=[_text_id, _child_id], callback=_update_yscroll_info)
                                dpg.bind_item_handler_registry(_text_id, dpg.last_container())

                for i in range(5):
                    dpg.add_text(text_items[i])
                    with dpg.group(horizontal=True):
                        with dpg.child_window(height=50, horizontal_scrollbar=True, width=-200, delay_search=True) as _child_id:
                            with dpg.group(horizontal=True):
                                for j in range(25):
                                    if j == 13:
                                        dpg.add_text("Item " + str(j), color=(255, 255, 0), tracked=True, track_offset=track_items[i])
                                    else:
                                        dpg.add_text("Item " + str(j))
                        _text_id = dpg.add_text("0/0")
                        with dpg.item_handler_registry(tag="__demo_item_reg2_"+str(i)):
                            dpg.add_item_visible_handler(user_data=[_text_id, _child_id], callback=_update_xscroll_info)
                        dpg.bind_item_handler_registry(_text_id, dpg.last_container())

                with dpg.child_window(height=50, horizontal_scrollbar=True, width=-200) as _child_id:
                    with dpg.group(horizontal=True):
                        for j in range(25):
                            dpg.add_text("Item " + str(j))

                def _scroll_programmatically(sender, app_data, user_data):

                    with dpg.mutex():
                        x_scroll = dpg.get_x_scroll(user_data[1])
                        max_scroll = dpg.get_x_scroll_max(user_data[1])
                        if user_data[0] == "left" and x_scroll > 10:
                            dpg.set_x_scroll(user_data[1], x_scroll-10)
                        elif user_data[0] == "left":
                            dpg.set_x_scroll(user_data[1], 0)

                        if user_data[0] == "right" and x_scroll < max_scroll-10:
                            dpg.set_x_scroll(user_data[1], x_scroll+10)
                        elif user_data[0] == "right":
                            dpg.set_x_scroll(user_data[1], max_scroll)

                with dpg.group(horizontal=True):
                    dpg.add_button(label="<<", small=True, user_data=["left", _child_id], callback=_scroll_programmatically)
                    dpg.add_text("Scroll from code")
                    dpg.add_button(label=">>", small=True, user_data=["right", _child_id], callback=_scroll_programmatically)
                    _text_id = dpg.add_text("0/0")
                with dpg.item_handler_registry(tag="__demo_item_reg3"):
                    dpg.add_item_visible_handler(user_data=[_text_id, _child_id], callback=_update_xscroll_info)
                dpg.bind_item_handler_registry(_text_id, dpg.last_container())

        with dpg.collapsing_header(label="Textures & Images"):
        
            with dpg.tree_node(label="Help"):

                dpg.add_separator()
                dpg.add_text("ABOUT TEXTURES:")
                dpg.add_text("Textures are buffers of RGBA data.", bullet=True, indent=20)
                dpg.add_text("Textures are used by 'image based' widgets:", bullet=True, indent=20)
                dpg.add_text("add_image", bullet=True, indent=50)
                dpg.add_text("add_image_button", bullet=True, indent=50)
                dpg.add_text("draw_image", bullet=True, indent=50)
                dpg.add_text("add_image_series", bullet=True, indent=50)
                dpg.add_text("Textures are themselves widgets.", bullet=True, indent=20)
                dpg.add_text("Textures must be a child of a texture container widget.", bullet=True, indent=20)
                dpg.add_text("Textures can be either static or dynamic (see following sections).", bullet=True, indent=20)

                dpg.add_separator()
                dpg.add_text("PROGRAMMER GUIDE:")
                dpg.add_text("'image based' widgets hold a reference to a texture widget.", bullet=True, indent=20)
                dpg.add_text("Deleting the texture widget will not affect widget's using it.", bullet=True, indent=50)
                dpg.add_text("Textures are only free'd from the GPU when the reference count reaches 0.", bullet=True, indent=50)
                dpg.add_text("The texture container widget is a root (has no parent).", bullet=True, indent=20)
                dpg.add_text("The texture container widget is hidden by default.", bullet=True, indent=20)
                with dpg.group(horizontal=True):
                    dpg.add_text("'Showing' it, opens a manager to inspect the textures within.", bullet=True, indent=50)
                    dpg.add_button(label="Press Here", small=True, callback=lambda:dpg.configure_item("__demo_texture_container", show=True))
                dpg.add_separator()

            with dpg.tree_node(label="Static Textures"):

                dpg.add_separator()
                dpg.add_text("ABOUT STATIC TEXTURES:")
                dpg.add_text("Can NOT be modified after creation.", bullet=True, indent=20)
                dpg.add_text("Can be loaded from a file using the 'file' keyword.", bullet=True, indent=20)
                dpg.add_separator()

                with dpg.group(horizontal=True):

                    with dpg.group():
                        dpg.add_text("Image Button")
                        dpg.add_image_button("__demo_static_texture_1")

                    with dpg.group():
                        dpg.add_text("Image")
                        dpg.add_image("__demo_static_texture_2")

                    with dpg.group():
                        dpg.add_text("Image (texture size)")
                        dpg.add_image("__demo_static_texture_3")

                    with dpg.group():
                        dpg.add_text("Image (2x texture size)")
                        dpg.add_image("__demo_static_texture_3", width=200, height=200)

                dpg.add_image(dpg.mvFontAtlas)

            with dpg.tree_node(label="Dynamic Textures"):

                dpg.add_separator()
                dpg.add_text("ABOUT DYNAMIC TEXTURES:")
                dpg.add_text("Can be modified after creation with 'set_value'.", bullet=True, indent=20)
                dpg.add_text("New data must be the same dimensions as the original", bullet=True, indent=20)
                dpg.add_separator()

                with dpg.group(horizontal=True):

                    with dpg.group():
                        dpg.add_color_picker((255, 0, 255, 255), label="Texture 1", 
                                no_side_preview=True, alpha_bar=True, width=200,
                                callback=_update_dynamic_textures, user_data=1)
                        dpg.add_text("Image Button")
                        dpg.add_image_button("__demo_dynamic_texture_1", width=100, height=100)


                    with dpg.group():
                        dpg.add_color_picker((255, 255, 0, 255), label="Texture 2", 
                                             no_side_preview=True, alpha_bar=True, width=200,
                                             callback=_update_dynamic_textures, user_data=2)
                        dpg.add_text("Image")
                        dpg.add_image("__demo_dynamic_texture_2")

            with dpg.tree_node(label="Image Series (plots)"):

                with dpg.plot(label="Image Plot", height=400, width=-1):
                    dpg.add_plot_legend()
                    dpg.add_plot_axis(dpg.mvXAxis, label="x axis")
                    with dpg.plot_axis(dpg.mvYAxis, label="y axis"):
                        dpg.add_image_series(dpg.mvFontAtlas, [300, 300], [400, 400], label="font atlas")
                        dpg.add_image_series("__demo_static_texture_1", [0, 0], [100, 100], label="static 1")
                        dpg.add_image_series("__demo_static_texture_2", [150, 150], [200, 200], label="static 2")
                        dpg.add_image_series("__demo_static_texture_3", [200, -150], [300, -50], label="static 3")
                        dpg.add_image_series("__demo_dynamic_texture_1", [-200, 100], [-100, 200], label="dynamic 1")
                        dpg.add_image_series("__demo_dynamic_texture_2", [-200, -100], [-150, -50], label="dynamic 2")

            with dpg.tree_node(label="Drawlists"):

                with dpg.drawlist(width=400, height=300):
                    dpg.draw_rectangle((0, 0), (400, 300), color=(100, 100, 100, 250), thickness=2)
                    dpg.draw_image("__demo_static_texture_3", [0, 0], [100, 100])
                    dpg.draw_image("__demo_dynamic_texture_1", [200, 100], [300, 200])

        with dpg.collapsing_header(label="Popups & Modal Windows"):
            
            with dpg.tree_node(label="Popups"):

                popup_values = ["Bream", "Haddock", "Mackerel", "Pollock", "Tilefish"]

                dpg.add_text("This is a light wrapper over a window.", bullet=True)
                dpg.add_text("For more control over a modal|popup window use a normal window with the modal|popup keyword and set the item handler and mouse events manually.", bullet=True)
                dpg.add_text("By default a popup will shrink fit the items it contains.This is useful for context windows, and simple modal window popups.", bullet=True)
                dpg.add_text("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.", bullet=True)
                with dpg.group(horizontal=True):
                    b = dpg.add_button(label="Right Click...")
                    t = dpg.add_text("<None>")
                    with dpg.popup(b, tag="__demo_popup1"):
                        dpg.add_text("Aquariam")
                        dpg.add_separator()
                        for i in popup_values:
                            dpg.add_selectable(label=i, user_data=[t, i], callback=lambda s, a, u: dpg.set_value(u[0], u[1]))
                dpg.add_text("A Popup with minimum size that cannot be moved", bullet=True)
                with dpg.group(horizontal=True):
                    b = dpg.add_button(label="Right Click...")
                    t = dpg.add_text("<None>")
                    with dpg.popup(b, tag="__demo_popup2", no_move=True, min_size=[300,400]):
                        dpg.add_text("Aquariam")
                        dpg.add_separator()
                        for i in popup_values:
                            dpg.add_selectable(label=i, user_data=[t, i], callback=lambda s, a, u: dpg.set_value(u[0], u[1]))

            with dpg.tree_node(label="Modals"):
                dpg.add_text("Modal windows are like popups but the user cannot close them by clicking outside.")
                dpg.add_text("This popup uses left click")
                dpg.add_button(label="Left Click...")
                with dpg.popup(dpg.last_item(), modal=True, mousebutton=dpg.mvMouseButton_Left, tag="__demo_popup3"):
                    dpg.add_text("All those beautiful files will be deleted.\nThis operation cannot be undone!")
                    dpg.add_separator()
                    dpg.add_checkbox(label="Don't ask me next time")
                    with dpg.group(horizontal=True):
                        dpg.add_button(label="OK", width=75, callback=lambda: dpg.configure_item("__demo_popup3", show=False))
                        dpg.add_button(label="Cancel", width=75, callback=lambda: dpg.configure_item("__demo_popup3", show=False))

            with dpg.tree_node(label="File/Directory Selector"):

                with dpg.file_dialog(label="Demo File Dialog", width=300, height=400, show=False, callback=lambda s, a, u : print(s, a, u), tag="__demo_filedialog"):
                    dpg.add_file_extension(".*", color=(255, 255, 255, 255))
                    dpg.add_file_extension("Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp}", color=(0, 255, 255, 255))
                    dpg.add_file_extension(".cpp", color=(255, 255, 0, 255))
                    dpg.add_file_extension(".h", color=(255, 0, 255, 255), custom_text="header")
                    dpg.add_file_extension("Python(.py){.py}", color=(0, 255, 0, 255))
                    #dpg.add_button(label="Button on file dialog")

                dpg.add_button(label="Show File Selector", user_data=dpg.last_container(), callback=lambda s, a, u: dpg.configure_item(u, show=True))

        with dpg.collapsing_header(label="Tooltips"):

            dpg.add_text("Hover me for a fancy tooltip")
            with dpg.tooltip(dpg.last_item()):
                dpg.add_simple_plot(label="", default_value=(0.3, 0.9, 2.5, 8.9), height = 80)

        with dpg.collapsing_header(label="Tables"):

            with dpg.tree_node(label="Basic"):

                # basic usage of the table api
                with dpg.table(header_row=False):

                    # use add_table_column to add columns to the table,
                    # table columns use slot 0
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()

                    # add_table_next_column will jump to the next row
                    # once it reaches the end of the columns
                    # table next column use slot 1
                    for i in range(4):

                        with dpg.table_row():
                            for j in range(3):
                                dpg.add_text(f"Row{i} Column{j}")

            with dpg.tree_node(label="Borders, background") as section:

                with dpg.table(header_row=False, row_background=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, delay_search=True) as table_id:

                    dpg.add_table_column(label="Header 1")
                    dpg.add_table_column(label="Header 2")
                    dpg.add_table_column(label="Header 3")

                    for i in range(5):
                        with dpg.table_row():
                            for j in range(3):
                                dpg.add_text(f"Row{i} Column{j}")

                _add_config_options(table_id, 2, 
                                            "row_background", "borders_innerH", "borders_innerV", 
                                            "borders_outerH", "borders_outerV", "header_row", before=table_id)

            with dpg.tree_node(label="Colors"):

                dpg.add_text("Highlighting Rows, Columns, Cells:")
                with dpg.table(header_row=False, row_background=True,
                             delay_search=True) as table_id:

                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()

                    for i in range(6):
                        with dpg.table_row():
                            for j in range(6):
                                dpg.add_text(f"R{i} C{j}")

                    for i in range(6):
                        dpg.highlight_table_row(table_id, i, [255, 0, 0, 100])
                        
                    dpg.highlight_table_column(table_id, 4, [0, 255, 0, 100])
                    
                    dpg.highlight_table_cell(table_id, 1, 1, [0, 0, 255, 100])
                    dpg.highlight_table_cell(table_id, 2, 1, [0, 0, 255, 100])
                    dpg.highlight_table_cell(table_id, 1, 2, [0, 0, 255, 100])
                    dpg.highlight_table_cell(table_id, 2, 2, [0, 0, 255, 100])

                _add_config_options(table_id, 1, "row_background", before=table_id)

                dpg.add_text("Coloring rows:")
                with dpg.table(header_row=False, row_background=True,
                             delay_search=True) as table_id:

                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()

                    for i in range(6):
                        with dpg.table_row():
                            for j in range(6):
                                dpg.add_text(f"R{i} C{j}")

                    dpg.highlight_table_cell(table_id, 1, 1, [0, 255, 0, 100])
                    dpg.highlight_table_cell(table_id, 2, 1, [0, 255, 0, 100])
                    dpg.highlight_table_cell(table_id, 1, 2, [0, 255, 0, 100])
                    dpg.highlight_table_cell(table_id, 2, 2, [0, 255, 0, 100])

                    for i in range(6):
                        dpg.set_table_row_color(table_id, i, [255, 0, 0, i*255/8])

            with dpg.tree_node(label="Resizable, stretch"):

                with dpg.table(header_row=False, resizable=True, delay_search=True,
                            borders_outerH=True, borders_innerV=True, borders_outerV=True) as table_id:

                    dpg.add_table_column(label="Header 1")
                    dpg.add_table_column(label="Header 2")
                    dpg.add_table_column(label="Header 3")

                    for i in range(5):
                        with dpg.table_row():
                            for j in range(3):
                                dpg.add_text(f"Row{i} Column{j}")

                _add_config_options(table_id, 1, 
                            "borders_innerV", "borders_outerV", "resizable", before=table_id)

            with dpg.tree_node(label="Resizable, fixed"):

                dpg.add_text("Only available if scrollX/scrollY are disabled and stretch columns are not used")
                with dpg.table(header_row=False, policy=dpg.mvTable_SizingFixedFit, resizable=True, no_host_extendX=False, 
                            borders_innerV=True, delay_search=True, borders_outerV=True,borders_outerH=True) as table_id:

                    dpg.add_table_column(label="Header 1")
                    dpg.add_table_column(label="Header 2")
                    dpg.add_table_column(label="Header 3")

                    for i in range(5):
                        with dpg.table_row():
                            for j in range(3):
                                dpg.add_text(f"Row{i} Column{j}")

                _add_config_options(table_id, 1, "no_host_extendX", before=table_id)
 
            with dpg.tree_node(label="Resizable, mixed"):

                with dpg.table(header_row=True, policy=dpg.mvTable_SizingFixedFit, row_background=True, reorderable=True, 
                            resizable=True, no_host_extendX=False, hideable=True, 
                            borders_innerV=True, delay_search=True, borders_outerV=True, borders_innerH=True, borders_outerH=True):

                    dpg.add_table_column(label="AAA", width_fixed=True)
                    dpg.add_table_column(label="BBB", width_fixed=True)
                    dpg.add_table_column(label="CCC", width_stretch=True, init_width_or_weight=0.0)

                    for i in range(5):
                        with dpg.table_row():
                            for j in range(3):
                                if j == 2:
                                    dpg.add_text(f"Stretch {i}, {j}")
                                else:
                                    dpg.add_text(f"Fixed {i}, {j}")

                with dpg.table(header_row=True, policy=dpg.mvTable_SizingFixedFit, row_background=True, reorderable=True, 
                            resizable=True, no_host_extendX=False, hideable=True, 
                            borders_innerV=True, delay_search=True, borders_outerV=True, borders_innerH=True, borders_outerH=True):

                    dpg.add_table_column(label="AAA", width_fixed=True)
                    dpg.add_table_column(label="BBB", width_fixed=True)
                    dpg.add_table_column(label="CCC", width_stretch=True, init_width_or_weight=0.0)
                    dpg.add_table_column(label="DDD", width_stretch=True, init_width_or_weight=0.0)

                    for i in range(5):
                        with dpg.table_row():
                            for j in range(4):
                                if j == 2 or j == 3:
                                    dpg.add_text(f"Stretch {i},{j}")
                                else:
                                    dpg.add_text(f"Fixed {i}, {j}")

            with dpg.tree_node(label="Columns Options"):

                with dpg.table(header_row=True, no_host_extendX=True,
                                borders_innerH=True, borders_outerH=True, borders_innerV=True,
                                borders_outerV=True, row_background=True, hideable=True, reorderable=True,
                                resizable=True, sortable=True, policy=dpg.mvTable_SizingFixedFit,
                                scrollX=True, delay_search=True, scrollY=True):

                    c1 = dpg.add_table_column(label="One", default_sort=True)
                    c2 = dpg.add_table_column(label="Two")
                    c3 = dpg.add_table_column(label="Three", default_hide=True)

                    for i in range(8):
                        with dpg.table_row():
                            dpg.add_text("Indented One", indent=5*i)
                            dpg.add_text("Hello Two")
                            dpg.add_text("Hello Three")

                # options table
                with dpg.table(header_row=False, show=True):

                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()

                    options = (
                        "default_hide", 
                        "default_sort",
                        "width_stretch",
                        "width_fixed",
                        "no_resize",
                        "no_reorder",
                        "no_hide",
                        "no_clip",
                        "no_sort",
                        "no_sort_ascending",
                        "no_sort_descending",
                        "no_header_width",
                        "prefer_sort_ascending",
                        "prefer_sort_descending")

                    with dpg.table_row():
                        with dpg.table_cell():
                            dpg.add_text("One")
                            _add_config_options(c1, 1, *options)
                        with dpg.table_cell():
                            dpg.add_text("Two")
                            _add_config_options(c2, 1, *options)
                        with dpg.table_cell():
                            dpg.add_text("Three")
                            _add_config_options(c3, 1, *options)

            with dpg.tree_node(label="Columns widths"):

                with dpg.table(header_row=True, resizable=True,
                            borders_outerH=True, borders_innerH=True, 
                            borders_outerV=True, delay_search=True):

                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="Three")

                    with dpg.table_row():
                        for i in range(3):
                            _text_id = dpg.add_text("(w: 0.0f)")
                            with dpg.item_handler_registry(tag="__demo_item_reg4_"+str(i)):
                                dpg.add_item_visible_handler(user_data = _text_id, callback=lambda s, a, u:dpg.set_value(u, "(w: " + str(dpg.get_item_state(u)["content_region_avail"][0]) + ")"))
                            dpg.bind_item_handler_registry(_text_id, dpg.last_container())

                    for i in range(3):
                        with dpg.table_row():
                            for j in range(3):
                                dpg.add_text(f"Hello {i}, {j}")

                with dpg.table(header_row=False, delay_search=True) as table_id:

                    dpg.add_table_column(width_fixed=True, init_width_or_weight=100)
                    dpg.add_table_column(width_fixed=True, init_width_or_weight=200)
                    dpg.add_table_column(width_fixed=True, init_width_or_weight=300)
                    dpg.add_table_column(width_fixed=True, init_width_or_weight=400)

                    with dpg.table_row():
                        for i in range(4):
                            _text_id = dpg.add_text("(w: 0.0f)")
                            with dpg.item_handler_registry(tag="__demo_item_reg5_"+str(i)):
                                dpg.add_item_visible_handler(user_data = _text_id, callback=lambda s, a, u:dpg.set_value(u, "(w: " + str(dpg.get_item_state(u)["content_region_avail"][0]) + ")"))
                            dpg.bind_item_handler_registry(_text_id, dpg.last_container())

                    for i in range(4):
                        with dpg.table_row():
                            for j in range(4):
                                dpg.add_text(f"Hello {i}, {j}")

                _add_config_options(table_id, 1, 
                            "no_keep_columns_visible", "borders_innerV", "borders_outerV", before=table_id)

            with dpg.tree_node(label="Row height"):

                with dpg.table(header_row=False, borders_outerH=True, borders_outerV=True, delay_search=True):

                    dpg.add_table_column()

                    for i in range(10):
                        with dpg.table_row(height=i*5 + 25):
                            dpg.add_text("height = " + str(i*5 + 25))

            with dpg.tree_node(label="Padding"):

                with dpg.table(header_row=False, resizable=True, delay_search=True,
                            hideable=True, reorderable=True, borders_outerV=True, borders_innerH=True) as table_id:

                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="three")

                    for i in range(5):
                        with dpg.table_row():
                            for j in range(3):
                                dpg.add_button(label=f"Hello {i}, {j}", width=-1)

                _add_config_options(table_id, 3, 
                                            "pad_outerX", "no_pad_outerX", "no_pad_innerX", 
                                            "borders_outerV", "borders_innerV", "header_row", before=table_id)

            with dpg.tree_node(label="Reorderable, hideable, with headers"):

                with dpg.table(header_row=True, resizable=True, delay_search=True,
                            hideable=True, reorderable=True) as table_id:

                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="three")

                    for i in range(5):
                        with dpg.table_row():
                            for j in range(3):
                                dpg.add_text(f"Hello {i}, {j}")

                _add_config_options(table_id, 3, 
                                            "hideable", "reorderable", "resizable", before=table_id)

            with dpg.tree_node(label="Outer Size"):

                with dpg.table(header_row=False, no_host_extendX=True, delay_search=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, context_menu_in_body=True, row_background=True,
                            policy=dpg.mvTable_SizingFixedFit, height=150) as table_id:
                
                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="three")

                    for i in range(10):
                        with dpg.table_row():
                            for j in range(3):
                                dpg.add_text(f"Cell {i}, {j}")

                _add_config_options(table_id, 3, 
                                            "no_host_extendX", "no_host_extendY", "resizable", before=table_id)

                dpg.add_text("Using explicit size:")
                with dpg.table(header_row=False, no_host_extendX=True, delay_search=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, context_menu_in_body=True, row_background=True,
                            policy=dpg.mvTable_SizingFixedFit, height=300, width=300):
                
                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="three")

                    for i in range(6):
                        with dpg.table_row():
                            for j in range(3):
                                dpg.add_text(f"Cell {i}, {j}")

            with dpg.tree_node(label="Scrolling, Clipping"):

                # without clipping
                dpg.add_text("Without Clipper")
                with dpg.table(header_row=True, no_host_extendX=True, delay_search=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, context_menu_in_body=True, row_background=True,
                            policy=dpg.mvTable_SizingFixedFit, height=300,
                            scrollY=True):

                    dpg.add_table_column(label="1")
                    dpg.add_table_column(label="2")
                    dpg.add_table_column(label="3")

                    for i in range(25):
                        with dpg.table_row():
                            dpg.add_input_int(label=" ", step=0)
                            dpg.add_button(label=f"Cell {i}, 1")
                            dpg.add_text(f"Cell {i}, 2")

                # with clipping
                dpg.add_text("Using Clipper")
                with dpg.table(header_row=True, no_host_extendX=True, delay_search=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, context_menu_in_body=True, row_background=True,
                            policy=dpg.mvTable_SizingFixedFit, height=300,
                            scrollY=True, clipper=True) as table_id:

                    dpg.add_table_column(label="1")
                    dpg.add_table_column(label="2")
                    dpg.add_table_column(label="3")

                    for i in range(25):
                        with dpg.table_row():
                            dpg.add_input_int(label=" ", step=0)
                            dpg.add_button(label=f"Cell {i}, 1")
                            dpg.add_text(f"Cell {i}, 2")

                dpg.add_checkbox(label="resizable", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, resizable=app_data))

                # Freezing rows/columns
                dpg.add_text("Freezing rows/columns")
                with dpg.table(header_row=True, borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, row_background=True, height=300, freeze_rows=1, freeze_columns=1,
                            scrollY=True, scrollX=True, policy=dpg.mvTable_SizingFixedFit, delay_search=True):

                    dpg.add_table_column(label="1", width=50)
                    dpg.add_table_column(label="2", width=50)
                    dpg.add_table_column(label="3", width=50)
                    dpg.add_table_column(label="4", width=50)
                    dpg.add_table_column(label="5", width=50)
                    dpg.add_table_column(label="6", width=50)
                    dpg.add_table_column(label="7", width=50)

                    for i in range(25):
                        with dpg.table_row():
                            dpg.add_text(f"Cell {i}, 0")
                            dpg.add_button(label=f"Cell {i}, 1")
                            dpg.add_text(f"Cell {i}, 2")
                            dpg.add_text(f"Cell {i}, 3")
                            dpg.add_text(f"Cell {i}, 4")
                            dpg.add_text(f"Cell {i}, 5")
                            dpg.add_text(f"Cell {i}, 6")

            with dpg.tree_node(label="Filtering"):

                # with filtering
                dpg.add_text("Using Filter (column 3)")
                _filter_table_id = dpg.generate_uuid()
                dpg.add_input_text(label="Filter (inc, -exc)", user_data=_filter_table_id, callback=lambda s, a, u: dpg.set_value(u, dpg.get_value(s)))
                with dpg.table(header_row=True, no_host_extendX=True, delay_search=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, context_menu_in_body=True, row_background=True,
                            policy=dpg.mvTable_SizingFixedFit, height=300,
                            scrollY=True, tag=_filter_table_id) as table_id:

                    dpg.add_table_column(label="1")
                    dpg.add_table_column(label="2")
                    dpg.add_table_column(label="3")

                    for i in range(25):
                        with dpg.table_row(filter_key=f"Cell {i}, 1"):
                            dpg.add_input_int(label=" ", step=0)
                            dpg.add_button(label=f"Cell {i}, 1")
                            dpg.add_text(f"Cell {i}, 2")

                dpg.add_checkbox(label="resizable", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, resizable=app_data))

            with dpg.tree_node(label="Sorting"):

                def _sort_callback(sender, sort_specs):

                    # sort_specs scenarios:
                    #   1. no sorting -> sort_specs == None
                    #   2. single sorting -> sort_specs == [[column_id, direction]]
                    #   3. multi sorting -> sort_specs == [[column_id, direction], [column_id, direction], ...]
                    #
                    # notes:
                    #   1. direction is ascending if == 1
                    #   2. direction is ascending if == -1

                    # no sorting case
                    if sort_specs is None: return

                    rows = dpg.get_item_children(sender, 1)

                    # create a list that can be sorted based on first cell
                    # value, keeping track of row and value used to sort
                    sortable_list = []
                    for row in rows:
                        first_cell = dpg.get_item_children(row, 1)[0]
                        sortable_list.append([row, dpg.get_value(first_cell)])

                    def _sorter(e):
                        return e[1]

                    sortable_list.sort(key=_sorter, reverse=sort_specs[0][1] < 0)

                    # create list of just sorted row ids
                    new_order = []
                    for pair in sortable_list:
                        new_order.append(pair[0])
                    
                    dpg.reorder_items(sender, 1, new_order)

                with dpg.table(header_row=True, no_host_extendX=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, context_menu_in_body=True, row_background=True,
                            policy=dpg.mvTable_SizingFixedFit, height=500, sortable=True, callback=_sort_callback,
                            scrollY=True, delay_search=True, tag="__demo_sorting_table"):

                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two", no_sort=True)
                    dpg.add_table_column(label="Three", no_sort=True)

                    for i in range(25):
                        with dpg.table_row():
                            dpg.add_input_int(label=" ", step=0, default_value=i)
                            dpg.add_text(f"Cell {i}, 1")
                            dpg.add_checkbox(label=f"Cell {i}, 2")


                #dpg.add_checkbox(label="sort_multi", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, sort_multi=app_data))
                dpg.add_checkbox(label="sort_tristate", before="__demo_sorting_table", callback=lambda sender, app_data, user_data:dpg.configure_item("__demo_sorting_table", sort_tristate=app_data))

            with dpg.tree_node(label="Sizing Policy"):

                def callback(sender, value, user_data):

                    if user_data[8] == "resizable":
                        for i in range(8):
                            dpg.configure_item(user_data[i], resizable=value)

                    elif user_data[8] == "no_host_extendX":
                        for i in range(8):
                            dpg.configure_item(user_data[i], no_host_extendX=value)

                    elif user_data[8] == "policy":

                        if value == "mvTable_SizingFixedFit":
                            dpg.configure_item(user_data[user_data[9]], policy=dpg.mvTable_SizingFixedFit)
                            dpg.configure_item(user_data[user_data[9]+1], policy=dpg.mvTable_SizingFixedFit)
                        elif value == "mvTable_SizingFixedSame":
                            dpg.configure_item(user_data[user_data[9]], policy=dpg.mvTable_SizingFixedSame)
                            dpg.configure_item(user_data[user_data[9]+1], policy=dpg.mvTable_SizingFixedSame)
                        elif value == "mvTable_SizingStretchProp":
                            dpg.configure_item(user_data[user_data[9]], policy=dpg.mvTable_SizingStretchProp)
                            dpg.configure_item(user_data[user_data[9]+1], policy=dpg.mvTable_SizingStretchProp)
                        elif value == "mvTable_SizingStretchSame":
                            dpg.configure_item(user_data[user_data[9]], policy=dpg.mvTable_SizingStretchSame)
                            dpg.configure_item(user_data[user_data[9]+1], policy=dpg.mvTable_SizingStretchSame)

                def create_table_set(policy):

                    with dpg.table(header_row=False, policy=policy, delay_search=True,
                                borders_innerH=True, borders_outerH=True, borders_innerV=True,
                                borders_outerV=True, row_background=True) as table_id1:
                
                        dpg.add_table_column()
                        dpg.add_table_column()
                        dpg.add_table_column()

                        for i in range(8):
                            with dpg.table_row():
                                dpg.add_text("Oh dear")
                                dpg.add_text("Oh dear")
                                dpg.add_text("Oh dear")

                    with dpg.table(header_row=False, policy=policy,
                                borders_innerH=True, borders_outerH=True, borders_innerV=False,
                                borders_outerV=True, row_background=True) as table_id2:
                
                        dpg.add_table_column()
                        dpg.add_table_column()
                        dpg.add_table_column()

                        for i in range(3):
                            with dpg.table_row():
                                dpg.add_text("AAAA")
                                dpg.add_text("BBBBBBBB")
                                dpg.add_text("CCCCCCCCCCCC")

                    return table_id1, table_id2

                table_id1, table_id12 = create_table_set(dpg.mvTable_SizingFixedFit)
                table_id2, table_id22 = create_table_set(dpg.mvTable_SizingFixedSame)
                table_id3, table_id32 = create_table_set(dpg.mvTable_SizingStretchProp)
                table_id4, table_id42 = create_table_set(dpg.mvTable_SizingStretchSame)

                user_data = [table_id1, table_id12, table_id2, table_id22, table_id3, table_id32, table_id4, table_id42]

                sizing_options = ("mvTable_SizingFixedFit", "mvTable_SizingFixedSame", "mvTable_SizingStretchProp", "mvTable_SizingStretchSame")

                combo1 = dpg.add_combo(sizing_options, label="Sizing Policy", default_value=sizing_options[0], before=table_id1, user_data=user_data + ["policy", 0], callback=callback)
                dpg.add_combo(sizing_options, label="Sizing Policy", default_value=sizing_options[1], before=table_id2, user_data=user_data + ["policy", 2], callback=callback)
                dpg.add_combo(sizing_options, label="Sizing Policy", default_value=sizing_options[2], before=table_id3, user_data=user_data + ["policy", 4], callback=callback)
                dpg.add_combo(sizing_options, label="Sizing Policy", default_value=sizing_options[3], before=table_id4, user_data=user_data + ["policy", 6], callback=callback)  
                dpg.add_checkbox(label="resizable", before=combo1, user_data=user_data + ["resizable"], callback=callback)
                dpg.add_checkbox(label="no_host_extendX", before=combo1, user_data=user_data + ["no_host_extendX"], callback=callback)

        with dpg.collapsing_header(label="Plots"):

            sindatax = []
            sindatay = []
            cosdatay = []
            for i in range(100):
                sindatax.append(i/100)
                sindatay.append(0.5 + 0.5*sin(50*i/100))
                cosdatay.append(0.5 + 0.75*cos(50*i/100))
        
            with dpg.tab_bar():

                with dpg.tab(label="Series"):

                    with dpg.tree_node(label="Line Series"):

                        dpg.add_text("Anti-aliasing can be enabled from the plot's context menu (see Help).", bullet=True)
                
                        # create plot
                        with dpg.plot(label="Line Series", height=400, width=-1):

                            # optionally create legend
                            dpg.add_plot_legend()

                            # REQUIRED: create x and y axes
                            dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            
                            with dpg.plot_axis(dpg.mvYAxis, label="y"):

                                # series belong to a y axis
                                dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)")

                    with dpg.tree_node(label="Stair Series"):

                        with dpg.plot(label="Stair Plot", height=400, width=-1):
                            dpg.add_plot_legend()
                            dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            with dpg.plot_axis(dpg.mvYAxis, label="y"):
                                dpg.add_stair_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)")

                    with dpg.tree_node(label="Shade Series"):

                        stock_datax = []
                        stock_datay2 = []
                        stock_data1 = []
                        stock_data2 = []
                        stock_data3 = []
                        stock_data4 = []
                        stock_data5 = []
                        for i in range(100):
                            stock_datax.append(i)
                            stock_datay2.append(0)
                            stock_data1.append(400 + 50*abs(random.random()))
                            stock_data2.append(275 + 75*abs(random.random()))
                            stock_data3.append(150 + 75*abs(random.random()))
                            stock_data4.append(500 + 75*abs(random.random()))
                            stock_data5.append(600 + 75*abs(random.random()))

                        with dpg.theme(tag="stock_theme1"):
                            with dpg.theme_component(0):
                                dpg.add_theme_color(dpg.mvPlotCol_Line, (0, 0, 255), category=dpg.mvThemeCat_Plots)
                                dpg.add_theme_color(dpg.mvPlotCol_Fill, (0, 0, 255, 64), category=dpg.mvThemeCat_Plots)

                        with dpg.theme(tag="stock_theme2"):
                            with dpg.theme_component(0):
                                dpg.add_theme_color(dpg.mvPlotCol_Line, (255, 0, 0), category=dpg.mvThemeCat_Plots)
                                dpg.add_theme_color(dpg.mvPlotCol_Fill, (255, 0, 0, 64), category=dpg.mvThemeCat_Plots)

                        with dpg.theme(tag="stock_theme3"):
                            with dpg.theme_component(0):
                                dpg.add_theme_color(dpg.mvPlotCol_Line, (0, 255, 0), category=dpg.mvThemeCat_Plots)
                                dpg.add_theme_color(dpg.mvPlotCol_Fill, (0, 255, 0, 64), category=dpg.mvThemeCat_Plots)

                        with dpg.theme(tag="stock_theme4"):
                            with dpg.theme_component(0):
                                dpg.add_theme_color(dpg.mvPlotCol_Fill, (255, 255, 100, 64), category=dpg.mvThemeCat_Plots)

                        with dpg.plot(label="Stock Prices", height=400, width=-1):
                            dpg.add_plot_legend()
                            xaxis = dpg.add_plot_axis(dpg.mvXAxis, label="Days")
                            with dpg.plot_axis(dpg.mvYAxis, label="Price"):
                                dpg.add_line_series(stock_datax, stock_data1, label="Stock 1")
                                dpg.bind_item_theme(dpg.last_item(), "stock_theme1")
                                dpg.add_line_series(stock_datax, stock_data2, label="Stock 2")
                                dpg.bind_item_theme(dpg.last_item(), "stock_theme2")
                                dpg.add_line_series(stock_datax, stock_data3, label="Stock 3")
                                dpg.bind_item_theme(dpg.last_item(), "stock_theme3")
                                dpg.add_shade_series(stock_datax, stock_data1, label="Stock 1")
                                dpg.bind_item_theme(dpg.last_item(), "stock_theme1")
                                dpg.add_shade_series(stock_datax, stock_data2, label="Stock 2")
                                dpg.bind_item_theme(dpg.last_item(), "stock_theme2")
                                dpg.add_shade_series(stock_datax, stock_data3, label="Stock 3", y2=stock_datay2)
                                dpg.bind_item_theme(dpg.last_item(), "stock_theme3")
                                dpg.add_shade_series(stock_datax, stock_data5, y2=stock_data4, label="Shade between lines")
                                dpg.bind_item_theme(dpg.last_item(), "stock_theme4")
                                dpg.fit_axis_data(dpg.top_container_stack())
                            dpg.fit_axis_data(xaxis)

                    with dpg.tree_node(label="Scatter Series"):

                        with dpg.plot(label="Scatter Series", height=400, width=-1):
                            dpg.add_plot_legend()
                            dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            with dpg.plot_axis(dpg.mvYAxis, label="y"):
                                dpg.add_scatter_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)")

                    with dpg.tree_node(label="Stem Series"):

                        with dpg.theme(tag="stem_theme1"):
                            with dpg.theme_component(0):
                                dpg.add_theme_color(dpg.mvPlotCol_Line, (0, 255, 0), category=dpg.mvThemeCat_Plots)
                                dpg.add_theme_style(dpg.mvPlotStyleVar_Marker, dpg.mvPlotMarker_Diamond, category=dpg.mvThemeCat_Plots)

                        with dpg.plot(label="Stem Series", height=400, width=-1):
                            dpg.add_plot_legend()
                            dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            with dpg.plot_axis(dpg.mvYAxis, label="y"):
                                dpg.add_stem_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)")
                                dpg.add_stem_series(sindatax, cosdatay, label="0.5 + 0.75 * cos(x)")
                                dpg.bind_item_theme(dpg.last_item(), "stem_theme1")

                    with dpg.tree_node(label="Bar Series"):

                        with dpg.plot(label="Bar Series", height=400, width=-1):

                            dpg.add_plot_legend()

                            # create x axis
                            dpg.add_plot_axis(dpg.mvXAxis, label="Student", no_gridlines=True)
                            dpg.set_axis_limits(dpg.last_item(), 9, 33)
                            dpg.set_axis_ticks(dpg.last_item(), (("S1", 11), ("S2", 21), ("S3", 31)))
                
                            # create y axis
                            with dpg.plot_axis(dpg.mvYAxis, label="Score"):
                                dpg.set_axis_limits(dpg.last_item(), 0, 110)
                                dpg.add_bar_series([10, 20, 30], [100, 75, 90], label="Final Exam", weight=1)
                                dpg.add_bar_series([11, 21, 31], [83, 75, 72], label="Midterm Exam", weight=1)
                                dpg.add_bar_series([12, 22, 32], [42, 68, 23], label="Course Grade", weight=1)

                    with dpg.tree_node(label="Area Series"):

                        with dpg.plot(label="Area Series", height=400, width=-1):
                            xaxis = dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            with dpg.plot_axis(dpg.mvYAxis, label="y"):
                                dpg.add_area_series([1,5,3],[0,0,3], fill=[255,50,100,190])
                                dpg.fit_axis_data(dpg.top_container_stack())
                            dpg.fit_axis_data(xaxis)

                    with dpg.tree_node(label="Infinite Lines"):

                        infinite_x_data = (3, 5, 6, 7)
                        infinite_y_data = (3, 5, 6, 7)

                        with dpg.plot(label="Infinite Lines", height=400, width=-1):
                            dpg.add_plot_legend()
                            xaxis = dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            with dpg.plot_axis(dpg.mvYAxis, label="y"):
                                dpg.add_vline_series(infinite_x_data, label="vertical")
                                dpg.add_hline_series(infinite_y_data, label="horizontal")
                                dpg.fit_axis_data(dpg.top_container_stack())
                            dpg.fit_axis_data(xaxis)

                    with dpg.tree_node(label="Image Series"):

                        with dpg.plot(label="Image Plot", height=400, width=-1):
                            dpg.add_plot_legend()
                            xaxis = dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            with dpg.plot_axis(dpg.mvYAxis, label="y axis"):
                                dpg.add_image_series(2, [300, 300], [400, 400], label="font atlas")
                                dpg.add_image_series("__demo_static_texture_2", [150, 150], [200, 200], label="static 2")
                                dpg.add_image_series("__demo_dynamic_texture_1", [-200, 100], [-100, 200], label="dynamic 1")
                                dpg.fit_axis_data(dpg.top_container_stack())
                            dpg.fit_axis_data(xaxis)

                    with dpg.tree_node(label="Candle Stick Series"):

                        dates = [1546300800,1546387200,1546473600,1546560000,1546819200,1546905600,1546992000,1547078400,1547164800,1547424000,1547510400,1547596800,1547683200,1547769600,1547942400,1548028800,1548115200,1548201600,1548288000,1548374400,1548633600,1548720000,1548806400,1548892800,1548979200,1549238400,1549324800,1549411200,1549497600,1549584000,1549843200,1549929600,1550016000,1550102400,1550188800,1550361600,1550448000,1550534400,1550620800,1550707200,1550793600,1551052800,1551139200,1551225600,1551312000,1551398400,1551657600,1551744000,1551830400,1551916800,1552003200,1552262400,1552348800,1552435200,1552521600,1552608000,1552867200,1552953600,1553040000,1553126400,1553212800,1553472000,1553558400,1553644800,1553731200,1553817600,1554076800,1554163200,1554249600,1554336000,1554422400,1554681600,1554768000,1554854400,1554940800,1555027200,1555286400,1555372800,1555459200,1555545600,1555632000,1555891200,1555977600,1556064000,1556150400,1556236800,1556496000,1556582400,1556668800,1556755200,1556841600,1557100800,1557187200,1557273600,1557360000,1557446400,1557705600,1557792000,1557878400,1557964800,1558051200,1558310400,1558396800,1558483200,1558569600,1558656000,1558828800,1558915200,1559001600,1559088000,1559174400,1559260800,1559520000,1559606400,1559692800,1559779200,1559865600,1560124800,1560211200,1560297600,1560384000,1560470400,1560729600,1560816000,1560902400,1560988800,1561075200,1561334400,1561420800,1561507200,1561593600,1561680000,1561939200,1562025600,1562112000,1562198400,1562284800,1562544000,1562630400,1562716800,1562803200,1562889600,1563148800,1563235200,1563321600,1563408000,1563494400,1563753600,1563840000,1563926400,1564012800,1564099200,1564358400,1564444800,1564531200,1564617600,1564704000,1564963200,1565049600,1565136000,1565222400,1565308800,1565568000,1565654400,1565740800,1565827200,1565913600,1566172800,1566259200,1566345600,1566432000,1566518400,1566777600,1566864000,1566950400,1567036800,1567123200,1567296000,1567382400,1567468800,1567555200,1567641600,1567728000,1567987200,1568073600,1568160000,1568246400,1568332800,1568592000,1568678400,1568764800,1568851200,1568937600,1569196800,1569283200,1569369600,1569456000,1569542400,1569801600,1569888000,1569974400,1570060800,1570147200,1570406400,1570492800,1570579200,1570665600,1570752000,1571011200,1571097600,1571184000,1571270400,1571356800,1571616000,1571702400,1571788800,1571875200,1571961600]
                        opens = [1284.7,1319.9,1318.7,1328,1317.6,1321.6,1314.3,1325,1319.3,1323.1,1324.7,1321.3,1323.5,1322,1281.3,1281.95,1311.1,1315,1314,1313.1,1331.9,1334.2,1341.3,1350.6,1349.8,1346.4,1343.4,1344.9,1335.6,1337.9,1342.5,1337,1338.6,1337,1340.4,1324.65,1324.35,1349.5,1371.3,1367.9,1351.3,1357.8,1356.1,1356,1347.6,1339.1,1320.6,1311.8,1314,1312.4,1312.3,1323.5,1319.1,1327.2,1332.1,1320.3,1323.1,1328,1330.9,1338,1333,1335.3,1345.2,1341.1,1332.5,1314,1314.4,1310.7,1314,1313.1,1315,1313.7,1320,1326.5,1329.2,1314.2,1312.3,1309.5,1297.4,1293.7,1277.9,1295.8,1295.2,1290.3,1294.2,1298,1306.4,1299.8,1302.3,1297,1289.6,1302,1300.7,1303.5,1300.5,1303.2,1306,1318.7,1315,1314.5,1304.1,1294.7,1293.7,1291.2,1290.2,1300.4,1284.2,1284.25,1301.8,1295.9,1296.2,1304.4,1323.1,1340.9,1341,1348,1351.4,1351.4,1343.5,1342.3,1349,1357.6,1357.1,1354.7,1361.4,1375.2,1403.5,1414.7,1433.2,1438,1423.6,1424.4,1418,1399.5,1435.5,1421.25,1434.1,1412.4,1409.8,1412.2,1433.4,1418.4,1429,1428.8,1420.6,1441,1460.4,1441.7,1438.4,1431,1439.3,1427.4,1431.9,1439.5,1443.7,1425.6,1457.5,1451.2,1481.1,1486.7,1512.1,1515.9,1509.2,1522.3,1513,1526.6,1533.9,1523,1506.3,1518.4,1512.4,1508.8,1545.4,1537.3,1551.8,1549.4,1536.9,1535.25,1537.95,1535.2,1556,1561.4,1525.6,1516.4,1507,1493.9,1504.9,1506.5,1513.1,1506.5,1509.7,1502,1506.8,1521.5,1529.8,1539.8,1510.9,1511.8,1501.7,1478,1485.4,1505.6,1511.6,1518.6,1498.7,1510.9,1510.8,1498.3,1492,1497.7,1484.8,1494.2,1495.6,1495.6,1487.5,1491.1,1495.1,1506.4]
                        highs = [1284.75,1320.6,1327,1330.8,1326.8,1321.6,1326,1328,1325.8,1327.1,1326,1326,1323.5,1322.1,1282.7,1282.95,1315.8,1316.3,1314,1333.2,1334.7,1341.7,1353.2,1354.6,1352.2,1346.4,1345.7,1344.9,1340.7,1344.2,1342.7,1342.1,1345.2,1342,1350,1324.95,1330.75,1369.6,1374.3,1368.4,1359.8,1359,1357,1356,1353.4,1340.6,1322.3,1314.1,1316.1,1312.9,1325.7,1323.5,1326.3,1336,1332.1,1330.1,1330.4,1334.7,1341.1,1344.2,1338.8,1348.4,1345.6,1342.8,1334.7,1322.3,1319.3,1314.7,1316.6,1316.4,1315,1325.4,1328.3,1332.2,1329.2,1316.9,1312.3,1309.5,1299.6,1296.9,1277.9,1299.5,1296.2,1298.4,1302.5,1308.7,1306.4,1305.9,1307,1297.2,1301.7,1305,1305.3,1310.2,1307,1308,1319.8,1321.7,1318.7,1316.2,1305.9,1295.8,1293.8,1293.7,1304.2,1302,1285.15,1286.85,1304,1302,1305.2,1323,1344.1,1345.2,1360.1,1355.3,1363.8,1353,1344.7,1353.6,1358,1373.6,1358.2,1369.6,1377.6,1408.9,1425.5,1435.9,1453.7,1438,1426,1439.1,1418,1435,1452.6,1426.65,1437.5,1421.5,1414.1,1433.3,1441.3,1431.4,1433.9,1432.4,1440.8,1462.3,1467,1443.5,1444,1442.9,1447,1437.6,1440.8,1445.7,1447.8,1458.2,1461.9,1481.8,1486.8,1522.7,1521.3,1521.1,1531.5,1546.1,1534.9,1537.7,1538.6,1523.6,1518.8,1518.4,1514.6,1540.3,1565,1554.5,1556.6,1559.8,1541.9,1542.9,1540.05,1558.9,1566.2,1561.9,1536.2,1523.8,1509.1,1506.2,1532.2,1516.6,1519.7,1515,1519.5,1512.1,1524.5,1534.4,1543.3,1543.3,1542.8,1519.5,1507.2,1493.5,1511.4,1525.8,1522.2,1518.8,1515.3,1518,1522.3,1508,1501.5,1503,1495.5,1501.1,1497.9,1498.7,1492.1,1499.4,1506.9,1520.9]
                        lows = [1282.85,1315,1318.7,1309.6,1317.6,1312.9,1312.4,1319.1,1319,1321,1318.1,1321.3,1319.9,1312,1280.5,1276.15,1308,1309.9,1308.5,1312.3,1329.3,1333.1,1340.2,1347,1345.9,1338,1340.8,1335,1332,1337.9,1333,1336.8,1333.2,1329.9,1340.4,1323.85,1324.05,1349,1366.3,1351.2,1349.1,1352.4,1350.7,1344.3,1338.9,1316.3,1308.4,1306.9,1309.6,1306.7,1312.3,1315.4,1319,1327.2,1317.2,1320,1323,1328,1323,1327.8,1331.7,1335.3,1336.6,1331.8,1311.4,1310,1309.5,1308,1310.6,1302.8,1306.6,1313.7,1320,1322.8,1311,1312.1,1303.6,1293.9,1293.5,1291,1277.9,1294.1,1286,1289.1,1293.5,1296.9,1298,1299.6,1292.9,1285.1,1288.5,1296.3,1297.2,1298.4,1298.6,1302,1300.3,1312,1310.8,1301.9,1292,1291.1,1286.3,1289.2,1289.9,1297.4,1283.65,1283.25,1292.9,1295.9,1290.8,1304.2,1322.7,1336.1,1341,1343.5,1345.8,1340.3,1335.1,1341.5,1347.6,1352.8,1348.2,1353.7,1356.5,1373.3,1398,1414.7,1427,1416.4,1412.7,1420.1,1396.4,1398.8,1426.6,1412.85,1400.7,1406,1399.8,1404.4,1415.5,1417.2,1421.9,1415,1413.7,1428.1,1434,1435.7,1427.5,1429.4,1423.9,1425.6,1427.5,1434.8,1422.3,1412.1,1442.5,1448.8,1468.2,1484.3,1501.6,1506.2,1498.6,1488.9,1504.5,1518.3,1513.9,1503.3,1503,1506.5,1502.1,1503,1534.8,1535.3,1541.4,1528.6,1525.6,1535.25,1528.15,1528,1542.6,1514.3,1510.7,1505.5,1492.1,1492.9,1496.8,1493.1,1503.4,1500.9,1490.7,1496.3,1505.3,1505.3,1517.9,1507.4,1507.1,1493.3,1470.5,1465,1480.5,1501.7,1501.4,1493.3,1492.1,1505.1,1495.7,1478,1487.1,1480.8,1480.6,1487,1488.3,1484.8,1484,1490.7,1490.4,1503.1]
                        closes = [1283.35,1315.3,1326.1,1317.4,1321.5,1317.4,1323.5,1319.2,1321.3,1323.3,1319.7,1325.1,1323.6,1313.8,1282.05,1279.05,1314.2,1315.2,1310.8,1329.1,1334.5,1340.2,1340.5,1350,1347.1,1344.3,1344.6,1339.7,1339.4,1343.7,1337,1338.9,1340.1,1338.7,1346.8,1324.25,1329.55,1369.6,1372.5,1352.4,1357.6,1354.2,1353.4,1346,1341,1323.8,1311.9,1309.1,1312.2,1310.7,1324.3,1315.7,1322.4,1333.8,1319.4,1327.1,1325.8,1330.9,1325.8,1331.6,1336.5,1346.7,1339.2,1334.7,1313.3,1316.5,1312.4,1313.4,1313.3,1312.2,1313.7,1319.9,1326.3,1331.9,1311.3,1313.4,1309.4,1295.2,1294.7,1294.1,1277.9,1295.8,1291.2,1297.4,1297.7,1306.8,1299.4,1303.6,1302.2,1289.9,1299.2,1301.8,1303.6,1299.5,1303.2,1305.3,1319.5,1313.6,1315.1,1303.5,1293,1294.6,1290.4,1291.4,1302.7,1301,1284.15,1284.95,1294.3,1297.9,1304.1,1322.6,1339.3,1340.1,1344.9,1354,1357.4,1340.7,1342.7,1348.2,1355.1,1355.9,1354.2,1362.1,1360.1,1408.3,1411.2,1429.5,1430.1,1426.8,1423.4,1425.1,1400.8,1419.8,1432.9,1423.55,1412.1,1412.2,1412.8,1424.9,1419.3,1424.8,1426.1,1423.6,1435.9,1440.8,1439.4,1439.7,1434.5,1436.5,1427.5,1432.2,1433.3,1441.8,1437.8,1432.4,1457.5,1476.5,1484.2,1519.6,1509.5,1508.5,1517.2,1514.1,1527.8,1531.2,1523.6,1511.6,1515.7,1515.7,1508.5,1537.6,1537.2,1551.8,1549.1,1536.9,1529.4,1538.05,1535.15,1555.9,1560.4,1525.5,1515.5,1511.1,1499.2,1503.2,1507.4,1499.5,1511.5,1513.4,1515.8,1506.2,1515.1,1531.5,1540.2,1512.3,1515.2,1506.4,1472.9,1489,1507.9,1513.8,1512.9,1504.4,1503.9,1512.8,1500.9,1488.7,1497.6,1483.5,1494,1498.3,1494.1,1488.1,1487.5,1495.7,1504.7,1505.3]

                        with dpg.plot(label="Candle Series", height=400, width=-1):
                            dpg.add_plot_legend()
                            xaxis = dpg.add_plot_axis(dpg.mvXAxis, label="Day", time=True)
                            with dpg.plot_axis(dpg.mvYAxis, label="USD"):
                                dpg.add_candle_series(dates, opens, closes, lows, highs, label="GOOGL", time_unit=dpg.mvTimeUnit_Day)
                                dpg.fit_axis_data(dpg.top_container_stack())
                            dpg.fit_axis_data(xaxis)

                    with dpg.tree_node(label="Heatmaps"):

                        values = (0.8, 2.4, 2.5, 3.9, 0.0, 4.0, 0.0,
                                  2.4, 0.0, 4.0, 1.0, 2.7, 0.0, 0.0,
                                  1.1, 2.4, 0.8, 4.3, 1.9, 4.4, 0.0,
                                  0.6, 0.0, 0.3, 0.0, 3.1, 0.0, 0.0,
                                  0.7, 1.7, 0.6, 2.6, 2.2, 6.2, 0.0,
                                  1.3, 1.2, 0.0, 0.0, 0.0, 3.2, 5.1,
                                  0.1, 2.0, 0.0, 1.4, 0.0, 1.9, 6.3)
                        with dpg.group(horizontal=True):
                            dpg.add_colormap_scale(min_scale=0, max_scale=6, height=400)
                            with dpg.plot(label="Heat Series", no_mouse_pos=True, height=400, width=-1):
                                dpg.add_plot_axis(dpg.mvXAxis, label="x", lock_min=True, lock_max=True, no_gridlines=True, no_tick_marks=True)
                                with dpg.plot_axis(dpg.mvYAxis, label="y", no_gridlines=True, no_tick_marks=True, lock_min=True, lock_max=True):
                                    dpg.add_heat_series(values, 7, 7, scale_min=0, scale_max=6)

                    with dpg.tree_node(label="Pie Charts"):

                        with dpg.group(horizontal=True):

                            # create plot 1
                            with dpg.plot(no_title=True, no_mouse_pos=True, width=250, height=250):

                                # create legend
                                dpg.add_plot_legend()

                                # create x axis
                                dpg.add_plot_axis(dpg.mvXAxis, label="", no_gridlines=True, no_tick_marks=True, no_tick_labels=True)
                                dpg.set_axis_limits(dpg.last_item(), 0, 1)

                                # create y axis
                                with dpg.plot_axis(dpg.mvYAxis, label="", no_gridlines=True, no_tick_marks=True, no_tick_labels=True):
                                    dpg.set_axis_limits(dpg.last_item(), 0, 1)
                                    dpg.add_pie_series(0.5, 0.5, 0.5, [0.25, 0.30, 0.30], ["fish", "cow", "chicken"])

                            # plot 2
                            with dpg.plot(no_title=True, no_mouse_pos=True, width=250, height=250):

                                # create legend
                                dpg.add_plot_legend()

                                # create x axis
                                dpg.add_plot_axis(dpg.mvXAxis, label="", no_gridlines=True, no_tick_marks=True, no_tick_labels=True)
                                dpg.set_axis_limits(dpg.last_item(), 0, 1)

                                # create y axis
                                with dpg.plot_axis(dpg.mvYAxis, label="", no_gridlines=True, no_tick_marks=True, no_tick_labels=True):
                                    dpg.set_axis_limits(dpg.last_item(), 0, 1)
                                    dpg.add_pie_series(0.5, 0.5, 0.5, [1, 1, 2, 3, 5], ["A", "B", "C", "D", "E"], normalize=True, format="%.0f")

                    with dpg.tree_node(label="Error Series"):

                        error1_x = [1, 2, 3, 4, 5]
                        error1_y = [1, 2, 5, 3, 4]
                        error1_neg = [0.2, 0.4, 0.2, 0.6, 0.4]
                        error1_pos = [0.4, 0.2, 0.4, 0.8, 0.6]

                        error2_x = [1, 2, 3, 4, 5]
                        error2_y = [8, 8, 9, 7, 8]
                        error2_neg = [0.2, 0.4, 0.2, 0.6, 0.4]
                        error2_pos = [0.4, 0.2, 0.4, 0.8, 0.6]

                        dpg.add_text("Anti-aliasing can be enabled from the plot's context menu (see Help).", bullet=True)
               
                        with dpg.plot(label="Error Series", height=400, width=-1):
                            dpg.add_plot_legend()
                            xaxis = dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            with dpg.plot_axis(dpg.mvYAxis, label="y"):

                                dpg.add_bar_series(error1_x, error1_y, label="Bar", weight=0.25)
                                dpg.add_error_series(error1_x, error1_y, error1_neg, error1_pos, label="Bar")
                                dpg.add_line_series(error2_x, error2_y, label="Line")
                                dpg.add_error_series(error2_x, error2_y, error2_neg, error2_pos, label="Line")
                                dpg.fit_axis_data(dpg.top_container_stack())
                            dpg.fit_axis_data(xaxis)

                with dpg.tab(label="Subplots"):

                    with dpg.tree_node(label="Basic"):

                        with dpg.subplots(3, 3, label="My Subplots", width=-1, height=-1, row_ratios=[5.0, 1.0, 1.0], column_ratios=[5.0, 1.0, 1.0]) as subplot_id:

                                for i in range(9):
                                    with dpg.plot(no_title=True):
                                        dpg.add_plot_axis(dpg.mvXAxis, label="", no_tick_labels=True)
                                        with dpg.plot_axis(dpg.mvYAxis, label="", no_tick_labels=True):
                                            dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)")

                        _add_config_options(subplot_id, 1, 
                                                    "no_resize", "no_title", before=subplot_id)

                    with dpg.tree_node(label="Item Sharing"):

                        with dpg.subplots(2, 3, label="", width=-1, height=-1) as subplot_id:

                                dpg.add_plot_legend()

                                for i in range(6):
                                    with dpg.plot(no_title=True):
                                        dpg.add_plot_axis(dpg.mvXAxis, label="")
                                        with dpg.plot_axis(dpg.mvYAxis, label=""):
                                            dpg.add_line_series(sindatax, sindatay, label="data" + str(i))

                        _add_config_options(subplot_id, 1, 
                                                    "column_major", before=subplot_id)

                    with dpg.tree_node(label="Linked Axes"):

                        with dpg.subplots(2, 2, label="", width=-1, height=-1) as subplot_id:

                                dpg.add_plot_legend()

                                for i in range(4):
                                    with dpg.plot(no_title=True):
                                        dpg.add_plot_axis(dpg.mvXAxis, label="")
                                        with dpg.plot_axis(dpg.mvYAxis, label=""):
                                            dpg.add_line_series(sindatax, sindatay, label="data" + str(i))

                        _add_config_options(subplot_id, 2, 
                                                    "no_align", "link_rows", "link_columns",
                                                    "link_all_x", "link_all_y", before=subplot_id)

                with dpg.tab(label="Axes"):

                    with dpg.tree_node(label="Time Axes"):

                        timedatax = []
                        timedatay = []
                
                        time_index = 0
                        while time_index < 739497600:
                            timedatax.append(time_index)
                            timedatay.append(time_index/(60*60*24))
                            time_index+=60*60*24*7
                
                        dpg.add_text("When time is enabled, x-axis values are interpreted as UNIX timestamps in seconds (e.g. 1599243545).", bullet=True)
                        dpg.add_text("UNIX timestamps are seconds since 00:00:00 UTC on 1 January 1970", bullet=True)
                
                        with dpg.plot(label="Time Plot", height=400, width=-1):
                            xaxis = dpg.add_plot_axis(dpg.mvXAxis, label="Date", time=True)
                            with dpg.plot_axis(dpg.mvYAxis, label="Days since 1970"):
                                dpg.add_line_series(timedatax, timedatay, label="Days")
                                dpg.fit_axis_data(dpg.top_container_stack())
                            dpg.fit_axis_data(xaxis)

                    with dpg.tree_node(label="Multi Axes Plot"):

                        with dpg.plot(label="Multi Axes Plot", height=400, width=-1):

                            dpg.add_plot_legend()

                            # create x axis
                            dpg.add_plot_axis(dpg.mvXAxis, label="x")

                            # create y axis 1
                            with dpg.plot_axis(dpg.mvYAxis, label="y1"):
                                dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)")

                            # create y axis 2
                            with dpg.plot_axis(dpg.mvYAxis, label="y2"):
                                dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)")
                
                            # create y axis 3
                            with dpg.plot_axis(dpg.mvYAxis, label="y3"):
                                dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)")

                with dpg.tab(label="Tools"):

                    with dpg.tree_node(label="Querying"):

                        dpg.add_text("Click and drag the middle mouse button!")
                        def query(sender, app_data, user_data):
                            dpg.set_axis_limits(user_data[0], app_data[0], app_data[1])
                            dpg.set_axis_limits(user_data[1], app_data[2], app_data[3])

                        # plot 1
                        with dpg.plot(no_title=True, height=400, callback=query, query=True, no_menus=True, width=-1) as plot_id:
                            dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            with dpg.plot_axis(dpg.mvYAxis, label="y"):
                                dpg.add_line_series(sindatax, sindatay)

                        # plot 2
                        with dpg.plot(no_title=True, height=400, no_menus=True, width=-1):          
                            xaxis_id2 = dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            yaxis_id2 = dpg.add_plot_axis(dpg.mvYAxis, label="y")
                            dpg.add_line_series(sindatax, sindatay, parent=yaxis_id2)

                            # set plot 1 user data to axis so the query callback has access
                            dpg.configure_item(plot_id, user_data=(xaxis_id2,yaxis_id2))

                    with dpg.tree_node(label="Drag Lines/Points"):

                        with dpg.plot(label="Drag Lines/Points", height=400, width=-1):
                            dpg.add_plot_legend()
                            dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            dpg.add_plot_axis(dpg.mvYAxis, label="y")

                            # drag lines/points belong to the plot NOT axis
                            dpg.add_drag_line(label="dline1", color=[255, 0, 0, 255])
                            dpg.add_drag_line(label="dline2", color=[255, 255, 0, 255], vertical=False)
                            dpg.add_drag_point(label="dpoint1", color=[255, 0, 255, 255])
                            dpg.add_drag_point(label="dpoint2", color=[255, 0, 255, 255])

                    with dpg.tree_node(label="Annotations"):

                        with dpg.plot(label="Annotations", height=400, width=-1):

                            dpg.add_plot_legend()
                            dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            dpg.add_plot_axis(dpg.mvYAxis, label="y")
                            dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=dpg.last_item())

                            # annotations belong to the plot NOT axis
                            dpg.add_plot_annotation(label="BL", default_value=(0.25, 0.25), offset=(-15, 15), color=[255, 255, 0, 255])
                            dpg.add_plot_annotation(label="BR", default_value=(0.75, 0.25), offset=(15, 15), color=[255, 255, 0, 255])
                            dpg.add_plot_annotation(label="TR not clampled", default_value=(0.75, 0.75), offset=(-15, -15), color=[255, 255, 0, 255], clamped=False)
                            dpg.add_plot_annotation(label="TL", default_value=(0.25, 0.75), offset=(-15, -15), color=[255, 255, 0, 255])
                            dpg.add_plot_annotation(label="Center", default_value=(0.5, 0.5), color=[255, 255, 0, 255])

                    with dpg.tree_node(label="Drag & Drop"):

                        sindatax = []
                        sindatay = []
                        for i in range(100):
                            sindatax.append(i/100)
                            sindatay.append(0.5 + 0.5*sin(50*i/100))

                        with dpg.group(horizontal=True):
                            with dpg.group():

                                dpg.add_text("Sources:")

                                dpg.add_button(label="Source 1")
                                with dpg.drag_payload(parent=dpg.last_item(), drag_data=(sindatax, sindatay, "Source 1"), payload_type="plotting"):
                                    dpg.add_text("Source 1")
                                    dpg.add_simple_plot(label="", default_value=sindatay)

                            def _legend_drop(sender, app_data, user_data):
                                parent = dpg.get_item_info(sender)["parent"]
                                yaxis2 = dpg.get_item_info(parent)["children"][1][2]
                                dpg.add_line_series(app_data[0], app_data[1], label=app_data[2], parent=yaxis2)
                                dpg.add_button(label="Delete Series", user_data = dpg.last_item(), parent=dpg.last_item(), callback=lambda s, a, u: dpg.delete_item(u))

                            def _plot_drop(sender, app_data, user_data):
                                yaxis1 = dpg.get_item_info(sender)["children"][1][0]
                                dpg.add_line_series(app_data[0], app_data[1], label=app_data[2], parent=yaxis1)
                                dpg.add_button(label="Delete Series", user_data = dpg.last_item(), parent=dpg.last_item(), callback=lambda s, a, u: dpg.delete_item(u))

                            def _axis_drop(sender, app_data, user_data):
                                dpg.add_line_series(app_data[0], app_data[1], label=app_data[2], parent=sender)
                                dpg.add_button(label="Delete Series", user_data = dpg.last_item(), parent=dpg.last_item(), callback=lambda s, a, u: dpg.delete_item(u))

                            with dpg.plot(label="Drag/Drop Plot", height=400, width=-1, drop_callback=_plot_drop, payload_type="plotting"):
                                dpg.add_plot_legend(drop_callback=_legend_drop, payload_type="plotting")
                                dpg.add_plot_axis(dpg.mvXAxis, label="x")

                                # create y axes with drop callbacks
                                dpg.add_plot_axis(dpg.mvYAxis, label="y1", drop_callback=_axis_drop, payload_type="plotting")
                                dpg.add_plot_axis(dpg.mvYAxis, label="y2", drop_callback=_axis_drop, payload_type="plotting")
                                dpg.add_plot_axis(dpg.mvYAxis, label="y3", drop_callback=_axis_drop, payload_type="plotting")

                with dpg.tab(label="Custom"):

                    with dpg.tree_node(label="Custom Context Menus"):

                        dpg.add_text("Right-click the series in the legend.", bullet=True)
                        dpg.add_text("Series are containers. Children will show up in the custom context menu.", bullet=True)
                
                        # create plot
                        with dpg.plot(label="Line Series", height=400, width=-1):

                            dpg.add_plot_legend()

                            dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            with dpg.plot_axis(dpg.mvYAxis, label="y"):

                                # series 1
                                dpg.add_line_series(sindatax, sindatay, label="series 1")
                                dpg.add_button(label="Delete Series 1", user_data = dpg.last_item(), parent=dpg.last_item(), callback=lambda s, a, u: dpg.delete_item(u))

                                # series 2
                                dpg.add_line_series(sindatax, sindatay, label="series 2")
                                dpg.add_button(label="Delete Series 2", user_data = dpg.last_item(), parent=dpg.last_item(), callback=lambda s, a, u: dpg.delete_item(u))

                    with dpg.tree_node(label="Custom Rendering"):

                        # create plot
                        with dpg.plot(label="Custom Rendering", height=400, width=-1):

                            dpg.add_plot_axis(dpg.mvXAxis, label="x")
                            dpg.add_plot_axis(dpg.mvYAxis, label="y")

                            dpg.draw_line((0, 0), (1, 2), color=(255, 0, 0, 255), thickness=0.01)
                            dpg.draw_text((0, 0), "Origin", color=(250, 250, 250, 255), size=0.1)

                    with dpg.tree_node(label="Custom Series"):

                        def _custom_series_callback(sender, app_data):
                            _helper_data = app_data[0]
                            transformed_x = app_data[1]
                            transformed_y = app_data[2]
                            mouse_x_plot_space = _helper_data["MouseX_PlotSpace"]
                            mouse_y_plot_space = _helper_data["MouseY_PlotSpace"]
                            mouse_x_pixel_space = _helper_data["MouseX_PixelSpace"]
                            mouse_y_pixel_space = _helper_data["MouseY_PixelSpace"]
                            dpg.delete_item(sender, children_only=True, slot=2)
                            dpg.push_container_stack(sender)
                            dpg.configure_item("demo_custom_series", tooltip=False)
                            for i in range(0, len(transformed_x)):
                                dpg.draw_text((transformed_x[i]+15, transformed_y[i]-15), str(i), size=20)
                                dpg.draw_circle((transformed_x[i], transformed_y[i]), 15, fill=(50+i*5, 50+i*50, 0, 255))
                                if mouse_x_pixel_space < transformed_x[i]+15 and mouse_x_pixel_space > transformed_x[i]-15 and mouse_y_pixel_space > transformed_y[i]-15 and mouse_y_pixel_space < transformed_y[i]+15:
                                    dpg.draw_circle((transformed_x[i], transformed_y[i]), 30)
                                    dpg.configure_item("demo_custom_series", tooltip=True)
                                    dpg.set_value("demo_custom_series_text", "Current Point: " + str(i))
                            dpg.pop_container_stack()
                        
                        # create plot
                        dpg.add_text("Hover an item for a custom tooltip!")
                        with dpg.plot(label="Custom Series", height=400, width=-1):
                            dpg.add_plot_legend()
                            xaxis = dpg.add_plot_axis(dpg.mvXAxis)
                            with dpg.plot_axis(dpg.mvYAxis):
                                with dpg.custom_series([0.0, 1.0, 2.0, 4.0, 5.0], [0.0, 10.0, 20.0, 40.0, 50.0], 2, label="Custom Series", callback=_custom_series_callback, tag="demo_custom_series"):
                                    dpg.add_text("Current Point: ", tag="demo_custom_series_text")
                                dpg.fit_axis_data(dpg.top_container_stack())

                with dpg.tab(label="Help"):

                    dpg.add_text("Plotting User Guide")
                    dpg.add_text("Left click and drag within the plot area to pan X and Y axes.", bullet=True)
                    dpg.add_text("Left click and drag on an axis to pan an individual axis.", bullet=True, indent=20)
                    dpg.add_text("Scoll in the plot area to zoom both X and Y axes.", bullet=True)
                    dpg.add_text("Scroll on an axis to zoom an individual axis.", bullet=True, indent=20)
                    dpg.add_text("Right click and drag to box select data.", bullet=True)
                    dpg.add_text("Hold Alt to expand box selection horizontally.", bullet=True, indent=20)
                    dpg.add_text("Hold Shift to expand box selection vertically.", bullet=True, indent=20)
                    dpg.add_text("Left click while box selecting to cancel the selection.", bullet=True, indent=20)
                    dpg.add_text("Double left click to fit all visible data.", bullet=True)
                    dpg.add_text("Double left click on an axis to fit the individual axis", bullet=True, indent=20)
                    dpg.add_text("Double right click to open the plot context menu.", bullet=True)
                    dpg.add_text("Click legend label icons to show/hide plot items.", bullet=True)
                    dpg.add_text("IMPORTANT: By default, anti-aliased lines are turned OFF.", bullet=True)

        with dpg.collapsing_header(label="Node Editor"):

            dpg.add_text("Ctrl+Click to remove a link.", bullet=True)

            with dpg.node_editor(callback=lambda sender, app_data: dpg.add_node_link(app_data[0], app_data[1], parent=sender), 
                             delink_callback=lambda sender, app_data: dpg.delete_item(app_data), minimap=True, minimap_location=dpg.mvNodeMiniMap_Location_BottomRight):

                with dpg.node(label="Node 1", pos=[10, 10]):

                    with dpg.node_attribute():
                        dpg.add_input_float(label="F1", width=150)

                    with dpg.node_attribute(attribute_type=dpg.mvNode_Attr_Output):
                        dpg.add_input_float(label="F2", width=150)

                with dpg.node(label="Node 2", pos=[300, 10]):

                    with dpg.node_attribute() as na2:
                        dpg.add_input_float(label="F3", width=200)

                    with dpg.node_attribute(attribute_type=dpg.mvNode_Attr_Output):
                        dpg.add_input_float(label="F4", width=200)

                with dpg.node(label="Node 3", pos=[25, 150]):                                  
                    with dpg.node_attribute():
                        dpg.add_input_text(label="T5", width=200)
                    with dpg.node_attribute(attribute_type=dpg.mvNode_Attr_Static): 
                        dpg.add_simple_plot(label="Node Plot", default_value=(0.3, 0.9, 2.5, 8.9), width=200, height=80, histogram=True)

        with dpg.collapsing_header(label= "Filtering"):

            dpg.add_text("Filter usage:\n"
                        "  \"\"               display all lines\n"
                        "  \"xxx\"         display lines containing \"xxx\"\n"
                        "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
                        "  \"-xxx\"        hide lines containing \"xxx\"")

            dpg.add_input_text(label="Filter (inc, -exc)", callback=lambda s, a: dpg.set_value("__demo_filter", a))
            with dpg.filter_set(tag="__demo_filter"):
                dpg.add_text("aaa1.c", filter_key="aaa1.c", bullet=True)
                dpg.add_text("bbb1.c", filter_key="bbb1.c", bullet=True)
                dpg.add_text("ccc1.c", filter_key="ccc1.c", bullet=True)
                dpg.add_text("aaa2.cpp", filter_key="aaa2.cpp", bullet=True)
                dpg.add_text("bbb2.cpp", filter_key="bbb2.cpp", bullet=True)
                dpg.add_text("ccc2.cpp", filter_key="ccc2.cpp", bullet=True)
                dpg.add_text("abc.h", filter_key="abc.h", bullet=True)
                dpg.add_text("hello, world", filter_key="hello, world", bullet=True)   

        with dpg.collapsing_header(label="Drawing API"):

            with dpg.tree_node(label="Basic"):
                draw_thickness = 3.0
                draw_size = 36
                draw_spacing = 10
                draw_rounding = draw_size/5.0
                draw_color = [255, 255, 0]
                with dpg.drawlist(width=300, height=200):
                    draw_x = 4
                    draw_y = 4
                    for i in range(2):

                        _draw_t = draw_thickness
                        if i == 0:
                            _draw_t = 1.0
                        dpg.draw_circle([draw_x + draw_size*0.5, draw_y + draw_size*0.5], draw_size*0.5, thickness=_draw_t, color=draw_color)
                        draw_x = draw_x + draw_spacing + draw_size
                        dpg.draw_rectangle([draw_x, draw_y], [draw_size + draw_x, draw_size + draw_y], thickness=_draw_t, color=draw_color)
                        draw_x = draw_x + draw_spacing + draw_size
                        dpg.draw_rectangle([draw_x, draw_y], [draw_size + draw_x, draw_size + draw_y], rounding=draw_rounding, thickness=_draw_t, color=draw_color)
                        draw_x = draw_x + draw_spacing + draw_size
                        dpg.draw_triangle([draw_x+draw_size*0.5,draw_y], [draw_x+draw_size, draw_y+draw_size-0.5], [draw_x, draw_y+draw_size-0.5], thickness=_draw_t, color=draw_color)
                        draw_x = draw_x + draw_spacing + draw_size
                        dpg.draw_line([draw_x, draw_y], [draw_x + draw_size, draw_y], thickness=_draw_t, color=draw_color)
                        draw_x = draw_x + draw_spacing + draw_size
                        dpg.draw_line([draw_x, draw_y], [draw_x, draw_y + draw_size], thickness=_draw_t, color=draw_color)
                        draw_x = draw_x + draw_spacing + draw_size
                        dpg.draw_line([draw_x, draw_y], [draw_x + draw_size, draw_y + draw_size], thickness=_draw_t, color=draw_color)
                        draw_x = draw_x + draw_spacing + draw_size
                        dpg.draw_bezier_quadratic([draw_x, draw_y + draw_size * 0.6], 
                                                  [draw_x + draw_size*0.5, draw_y - draw_size * 0.4], 
                                                  [draw_x + draw_size, draw_y + draw_size], thickness=_draw_t, color=draw_color)
                        draw_x = draw_x + draw_spacing + draw_size
                        dpg.draw_bezier_cubic([draw_x, draw_y], 
                                              [draw_x + draw_size*1.3, draw_y + draw_size * 0.3], 
                                              [draw_x+draw_size - draw_size*1.3, draw_y + draw_size - draw_size*0.3],
                                              [draw_x + draw_size, draw_y + draw_size], thickness=_draw_t, color=draw_color)
                        draw_x = 4
                        draw_y = draw_y + draw_size + draw_spacing

                    _draw_t = 0.0

                    dpg.draw_circle([draw_x + draw_size*0.5, draw_y + draw_size*0.5], draw_size*0.5, thickness=_draw_t, color=draw_color, fill=draw_color)
                    draw_x = draw_x + draw_spacing + draw_size
                    dpg.draw_rectangle([draw_size + draw_x, draw_size + draw_y], [draw_x, draw_y], thickness=_draw_t, color=draw_color, fill=draw_color)
                    draw_x = draw_x + draw_spacing + draw_size
                    dpg.draw_rectangle([draw_x, draw_y], [draw_size + draw_x, draw_size + draw_y], rounding=draw_rounding, thickness=_draw_t, color=draw_color, fill=draw_color)
                    draw_x = draw_x + draw_spacing + draw_size
                    dpg.draw_triangle([draw_x+draw_size*0.5,draw_y], [draw_x+draw_size, draw_y+draw_size-0.5], [draw_x, draw_y+draw_size-0.5], thickness=_draw_t, color=draw_color, fill=draw_color)
                    draw_x = draw_x + draw_spacing + draw_size
                    dpg.draw_rectangle([draw_size + draw_x, draw_size + draw_y], [draw_x, draw_y], color=[0, 0, 0, 0], thickness=_draw_t, color_upper_left=[0, 0, 0], color_upper_right=[255, 0, 0], color_bottom_left=[255, 255, 0], color_bottom_right=[0, 255, 0], multicolor=True)

            with dpg.tree_node(label="Draw Nodes & 3D transforms"):

                with dpg.group(horizontal=True):
                    with dpg.drawlist(width=500, height=500, tag="_demo_advanced_drawing"):

                        with dpg.draw_layer():

                            dpg.draw_circle([250, 250], 150, color=[0, 255, 0])
                            dpg.draw_circle([250, 250], 200, color=[0, 255, 255])
                            with dpg.draw_node(tag="_drawing_sun"):
                                dpg.apply_transform(dpg.last_item(), dpg.create_translation_matrix([250, 250]))
                                dpg.draw_circle([0, 0], 15, color=[255, 255, 0], fill=[255, 255, 0])
                                with dpg.draw_node(tag="_drawing_planet1", user_data=45.0):
                                    dpg.apply_transform(dpg.last_item(), dpg.create_rotation_matrix(math.pi*45.0/180.0 , [0, 0, -1])*dpg.create_translation_matrix([150, 0]))
                                    dpg.draw_circle([0, 0], 10, color=[0, 255, 0], fill=[0, 255, 0])
                                    dpg.draw_circle([0, 0], 25, color=[255, 0, 255])
                                    with dpg.draw_node(tag="_drawing_moon1", user_data=45.0):
                                        dpg.apply_transform(dpg.last_item(), dpg.create_rotation_matrix(math.pi*45.0/180.0 , [0, 0, -1])*dpg.create_translation_matrix([25, 0]))
                                        dpg.draw_circle([0, 0], 5, color=[255, 0, 255], fill=[255, 0, 255])

                                with dpg.draw_node(tag="_drawing_planet2", user_data=0.0):
                                    dpg.apply_transform(dpg.last_item(), dpg.create_rotation_matrix(math.pi*0.0/180.0 , [0, 0, -1])*dpg.create_translation_matrix([200, 0]))
                                    dpg.draw_circle([0, 0], 10, color=[0, 255, 255], fill=[0, 255, 255])
                                    dpg.draw_circle([0, 0], 25, color=[255, 0, 255])
                                    dpg.draw_circle([0, 0], 45, color=[255, 255, 255])
                                    with dpg.draw_node(tag="_drawing_moon2", user_data=45.0):
                                        dpg.apply_transform(dpg.last_item(), dpg.create_rotation_matrix(math.pi*45.0/180.0 , [0, 0, -1])*dpg.create_translation_matrix([25, 0]))
                                        dpg.draw_circle([0, 0], 5, color=[255, 0, 255], fill=[255, 0, 255])

                                    with dpg.draw_node(tag="_drawing_moon3", user_data=120.0):
                                        dpg.apply_transform(dpg.last_item(), dpg.create_rotation_matrix(math.pi*120.0/180.0 , [0, 0, -1])*dpg.create_translation_matrix([45, 0]))
                                        dpg.draw_circle([0, 0], 5, color=[255, 255, 255], fill=[255, 255, 255])

                    def _demo_live_drawing():
                        planet_rot1 = dpg.get_item_user_data("_drawing_planet1") + 1.0
                        planet_rot2 = dpg.get_item_user_data("_drawing_planet2") + 2.0
                        moon_rot1 = dpg.get_item_user_data("_drawing_moon1") + 3.0
                        moon_rot2 = dpg.get_item_user_data("_drawing_moon2") + 7.0
                        moon_rot3 = dpg.get_item_user_data("_drawing_moon3") + 10.0
                        dpg.apply_transform("_drawing_planet1", dpg.create_rotation_matrix(math.pi*planet_rot1/180.0 , [0, 0, -1])*dpg.create_translation_matrix([150, 0]))
                        dpg.apply_transform("_drawing_planet2", dpg.create_rotation_matrix(math.pi*planet_rot2/180.0 , [0, 0, -1])*dpg.create_translation_matrix([200, 0]))
                        dpg.apply_transform("_drawing_moon1", dpg.create_rotation_matrix(math.pi*moon_rot1/180.0 , [0, 0, -1])*dpg.create_translation_matrix([25, 0]))
                        dpg.apply_transform("_drawing_moon2", dpg.create_rotation_matrix(math.pi*moon_rot2/180.0 , [0, 0, -1])*dpg.create_translation_matrix([25, 0]))
                        dpg.apply_transform("_drawing_moon3", dpg.create_rotation_matrix(math.pi*moon_rot3/180.0 , [0, 0, 1])*dpg.create_translation_matrix([45, 0]))
                        dpg.set_item_user_data("_drawing_planet1", planet_rot1)
                        dpg.set_item_user_data("_drawing_planet2", planet_rot2)
                        dpg.set_item_user_data("_drawing_moon1", moon_rot1)
                        dpg.set_item_user_data("_drawing_moon2", moon_rot2)
                        dpg.set_item_user_data("_drawing_moon3", moon_rot3)

                    with dpg.item_handler_registry(tag="__demo_item_reg6"):
                        dpg.add_item_visible_handler(callback=_demo_live_drawing)
                    dpg.bind_item_handler_registry("_demo_advanced_drawing", dpg.last_container())


                    size = 5
                    demo_verticies = [
                            [-size, -size, -size],  # 0 near side
                            [ size, -size, -size],  # 1
                            [-size,  size, -size],  # 2
                            [ size,  size, -size],  # 3
                            [-size, -size,  size],  # 4 far side
                            [ size, -size,  size],  # 5
                            [-size,  size,  size],  # 6
                            [ size,  size,  size],  # 7
                            [-size, -size, -size],  # 8 left side
                            [-size,  size, -size],  # 9
                            [-size, -size,  size],  # 10
                            [-size,  size,  size],  # 11
                            [ size, -size, -size],  # 12 right side
                            [ size,  size, -size],  # 13
                            [ size, -size,  size],  # 14
                            [ size,  size,  size],  # 15
                            [-size, -size, -size],  # 16 bottom side
                            [ size, -size, -size],  # 17
                            [-size, -size,  size],  # 18
                            [ size, -size,  size],  # 19
                            [-size,  size, -size],  # 20 top side
                            [ size,  size, -size],  # 21
                            [-size,  size,  size],  # 22
                            [ size,  size,  size],  # 23
                        ]

                    demo_colors = [
                            [255,   0,   0, 150],
                            [255, 255,   0, 150],
                            [255, 255, 255, 150],
                            [255,   0, 255, 150],
                            [  0, 255,   0, 150],
                            [  0, 255, 255, 150],
                            [  0,   0, 255, 150],
                            [  0, 125,   0, 150],
                            [128,   0,   0, 150],
                            [128,  70,   0, 150],
                            [128, 255, 255, 150],
                            [128,   0, 128, 150]
                        ]

                    with dpg.drawlist(width=500, height=500, tag="_demo_advanced_drawing_3d"):

                        with dpg.draw_layer(depth_clipping=False, cull_mode=dpg.mvCullMode_Back, perspective_divide=True):
                            dpg.set_clip_space(dpg.last_item(), 0, 0, 500, 500, -1.0, 1.0)
                            with dpg.draw_node(tag="_demo_3dNode1", user_data=[0, 0, 0]):
                                dpg.draw_triangle(demo_verticies[1],  demo_verticies[2],  demo_verticies[0], color=[0,0,0.0], fill=demo_colors[0])
                                dpg.draw_triangle(demo_verticies[1],  demo_verticies[3],  demo_verticies[2], color=[0,0,0.0], fill=demo_colors[1])
                                dpg.draw_triangle(demo_verticies[7],  demo_verticies[5],  demo_verticies[4], color=[0,0,0.0], fill=demo_colors[2])
                                dpg.draw_triangle(demo_verticies[6],  demo_verticies[7],  demo_verticies[4], color=[0,0,0.0], fill=demo_colors[3])
                                dpg.draw_triangle(demo_verticies[9],  demo_verticies[10], demo_verticies[8], color=[0,0,0.0], fill=demo_colors[4])
                                dpg.draw_triangle(demo_verticies[9],  demo_verticies[11], demo_verticies[10], color=[0,0,0.0], fill=demo_colors[5])
                                dpg.draw_triangle(demo_verticies[15], demo_verticies[13], demo_verticies[12], color=[0,0,0.0], fill=demo_colors[6])
                                dpg.draw_triangle(demo_verticies[14], demo_verticies[15], demo_verticies[12], color=[0,0,0.0], fill=demo_colors[7])
                                dpg.draw_triangle(demo_verticies[18], demo_verticies[17], demo_verticies[16], color=[0,0,0.0], fill=demo_colors[8])
                                dpg.draw_triangle(demo_verticies[19], demo_verticies[17], demo_verticies[18], color=[0,0,0.0], fill=demo_colors[9])
                                dpg.draw_triangle(demo_verticies[21], demo_verticies[23], demo_verticies[20], color=[0,0,0.0], fill=demo_colors[10])
                                dpg.draw_triangle(demo_verticies[23], demo_verticies[22], demo_verticies[20], color=[0,0,0.0], fill=demo_colors[11])

                with dpg.group(horizontal=True):

                        with dpg.drawlist(width=500, height=500):

                            with dpg.draw_layer(depth_clipping=False, cull_mode=dpg.mvCullMode_Front, perspective_divide=True):
                                dpg.set_clip_space(dpg.last_item(), 0, 0, 500, 500, -1.0, 1.0)
                                with dpg.draw_node(tag="_demo_3dNode2", user_data=[0, 0, 0]):
                                    dpg.draw_triangle(demo_verticies[1],  demo_verticies[2],  demo_verticies[0], color=[0,0,0.0], fill=demo_colors[0])
                                    dpg.draw_triangle(demo_verticies[1],  demo_verticies[3],  demo_verticies[2], color=[0,0,0.0], fill=demo_colors[1])
                                    dpg.draw_triangle(demo_verticies[7],  demo_verticies[5],  demo_verticies[4], color=[0,0,0.0], fill=demo_colors[2])
                                    dpg.draw_triangle(demo_verticies[6],  demo_verticies[7],  demo_verticies[4], color=[0,0,0.0], fill=demo_colors[3])
                                    dpg.draw_triangle(demo_verticies[9],  demo_verticies[10], demo_verticies[8], color=[0,0,0.0], fill=demo_colors[4])
                                    dpg.draw_triangle(demo_verticies[9],  demo_verticies[11], demo_verticies[10], color=[0,0,0.0], fill=demo_colors[5])
                                    dpg.draw_triangle(demo_verticies[15], demo_verticies[13], demo_verticies[12], color=[0,0,0.0], fill=demo_colors[6])
                                    dpg.draw_triangle(demo_verticies[14], demo_verticies[15], demo_verticies[12], color=[0,0,0.0], fill=demo_colors[7])
                                    dpg.draw_triangle(demo_verticies[18], demo_verticies[17], demo_verticies[16], color=[0,0,0.0], fill=demo_colors[8])
                                    dpg.draw_triangle(demo_verticies[19], demo_verticies[17], demo_verticies[18], color=[0,0,0.0], fill=demo_colors[9])
                                    dpg.draw_triangle(demo_verticies[21], demo_verticies[23], demo_verticies[20], color=[0,0,0.0], fill=demo_colors[10])
                                    dpg.draw_triangle(demo_verticies[23], demo_verticies[22], demo_verticies[20], color=[0,0,0.0], fill=demo_colors[11])

                        with dpg.drawlist(width=500, height=500):

                            with dpg.draw_layer(depth_clipping=False, cull_mode=dpg.mvCullMode_None, perspective_divide=True):
                                dpg.set_clip_space(dpg.last_item(), 0, 0, 500, 500, -1.0, 1.0)
                                with dpg.draw_node(tag="_demo_3dNode3", user_data=[0, 0, 0]):
                                    dpg.draw_triangle(demo_verticies[1],  demo_verticies[2],  demo_verticies[0], color=[0,0,0.0], fill=demo_colors[0])
                                    dpg.draw_triangle(demo_verticies[1],  demo_verticies[3],  demo_verticies[2], color=[0,0,0.0], fill=demo_colors[1])
                                    dpg.draw_triangle(demo_verticies[7],  demo_verticies[5],  demo_verticies[4], color=[0,0,0.0], fill=demo_colors[2])
                                    dpg.draw_triangle(demo_verticies[6],  demo_verticies[7],  demo_verticies[4], color=[0,0,0.0], fill=demo_colors[3])
                                    dpg.draw_triangle(demo_verticies[9],  demo_verticies[10], demo_verticies[8], color=[0,0,0.0], fill=demo_colors[4])
                                    dpg.draw_triangle(demo_verticies[9],  demo_verticies[11], demo_verticies[10], color=[0,0,0.0], fill=demo_colors[5])
                                    dpg.draw_triangle(demo_verticies[15], demo_verticies[13], demo_verticies[12], color=[0,0,0.0], fill=demo_colors[6])
                                    dpg.draw_triangle(demo_verticies[14], demo_verticies[15], demo_verticies[12], color=[0,0,0.0], fill=demo_colors[7])
                                    dpg.draw_triangle(demo_verticies[18], demo_verticies[17], demo_verticies[16], color=[0,0,0.0], fill=demo_colors[8])
                                    dpg.draw_triangle(demo_verticies[19], demo_verticies[17], demo_verticies[18], color=[0,0,0.0], fill=demo_colors[9])
                                    dpg.draw_triangle(demo_verticies[21], demo_verticies[23], demo_verticies[20], color=[0,0,0.0], fill=demo_colors[10])
                                    dpg.draw_triangle(demo_verticies[23], demo_verticies[22], demo_verticies[20], color=[0,0,0.0], fill=demo_colors[11])

                        def _demo_live_3D_drawing():
                            view_matrix = dpg.create_fps_matrix([0, 0, 50], 0.0, 0.0)
                            proj_matrix = dpg.create_perspective_matrix(math.pi*45.0/180.0, 500/500, 0.1, 100)
                            cube_rot = dpg.get_item_user_data("_demo_3dNode1")
                            cube_rot[0] = cube_rot[0] + 1.0
                            cube_rot[1] = cube_rot[1] + 2.0
                            cube_rot[2] = cube_rot[2] + 3.0
                            model_matrix = dpg.create_rotation_matrix(math.pi*cube_rot[0]/180.0 , [1, 0, 0])*\
                                                    dpg.create_rotation_matrix(math.pi*cube_rot[1]/180.0 , [0, 1, 0])*\
                                                    dpg.create_rotation_matrix(math.pi*cube_rot[2]/180.0 , [0, 0, 1])
                            dpg.apply_transform("_demo_3dNode1", proj_matrix*view_matrix*model_matrix)
                            dpg.apply_transform("_demo_3dNode2", proj_matrix*view_matrix*model_matrix)
                            dpg.apply_transform("_demo_3dNode3", proj_matrix*view_matrix*model_matrix)
                            dpg.set_item_user_data("_demo_3dNode1", cube_rot)

                        with dpg.item_handler_registry(tag="__demo_item_reg7"):
                            dpg.add_item_visible_handler(callback=_demo_live_3D_drawing)
                        dpg.bind_item_handler_registry("_demo_advanced_drawing_3d", dpg.last_container())

        with dpg.collapsing_header(label="Inputs & Events"):

            with dpg.tree_node(label="Global"):

                with dpg.handler_registry(show=False, tag="__demo_keyboard_handler"):
                    k_down = dpg.add_key_down_handler(key=dpg.mvKey_A)
                    k_release = dpg.add_key_release_handler(key=dpg.mvKey_A)
                    k_press = dpg.add_key_press_handler(key=dpg.mvKey_A)

                with dpg.handler_registry(show=False, tag="__demo_mouse_handler"):
                    m_wheel = dpg.add_mouse_wheel_handler()
                    m_click = dpg.add_mouse_click_handler(button=dpg.mvMouseButton_Left)
                    m_double_click = dpg.add_mouse_double_click_handler(button=dpg.mvMouseButton_Left)
                    m_release = dpg.add_mouse_release_handler(button=dpg.mvMouseButton_Left)
                    m_drag = dpg.add_mouse_drag_handler(button=dpg.mvMouseButton_Left)
                    m_down = dpg.add_mouse_down_handler(button=dpg.mvMouseButton_Left)
                    m_move = dpg.add_mouse_move_handler()

                dpg.add_checkbox(label="Activate Keyboard Handlers (A key)", callback=lambda s, a: dpg.configure_item("__demo_keyboard_handler", show=a))
                dpg.add_checkbox(label="Activate Mouse Handlers (left button)", callback=lambda s, a: dpg.configure_item("__demo_mouse_handler", show=a))
                kh_down = dpg.add_text("Key id:  seconds:", label="Key Down Handler:", show_label=True)
                kh_release = dpg.add_text("Key id:", label="Key Release Handler:", show_label=True)
                kh_press = dpg.add_text("Key id:", label="Key Press Handler:", show_label=True)
                mh_click = dpg.add_text("Mouse id:", label="Mouse Click Handler", show_label=True)
                mh_double = dpg.add_text("Mouse id:", label="Mouse Double Click Handler", show_label=True)
                mh_down = dpg.add_text("Mouse id:  seconds:", label="Mouse Down Handler", show_label=True)
                mh_release = dpg.add_text("Mouse id:", label="Mouse Release Handler", show_label=True)
                mh_wheel = dpg.add_text("Mouse id:", label="Mouse Wheel Handler", show_label=True)
                mh_move = dpg.add_text("Mouse pos:", label="Mouse Move Handler", show_label=True)
                mh_drag = dpg.add_text("Mouse id:  delta:", label="Mouse Drag Handler", show_label=True)

            with dpg.tree_node(label="Item"):

                dpg.add_text("Interact with following widgets and check output!")
                dpg.add_text("Note: Only the click, hover are added. Others are commented out. Check the code!")
                cb = dpg.add_checkbox(label="Check me!", tag="democheck")

                with dpg.item_handler_registry(tag="demoitemregistry"):
                    dpg.add_item_clicked_handler(0, callback=lambda s, a, u: print(f"clicked_handler: {s} '\t' {a} '\t' {u}"))
                    dpg.add_item_clicked_handler(1, callback=lambda s, a, u: print(f"clicked_handler: {s} '\t' {a} '\t' {u}"))
                    dpg.add_item_hover_handler(callback=lambda s, a, u: print(f"hover_handler: {s} '\t' {a} '\t' {u}"))
                dpg.bind_item_handler_registry(cb, "demoitemregistry")

            def _event_handler(sender, data):
                type=dpg.get_item_info(sender)["type"]
                if type=="mvAppItemType::mvKeyDownHandler":
                    dpg.set_value(kh_down, f"Key id: {data[0]}, Seconds:{data[1]}")
                elif type=="mvAppItemType::mvKeyReleaseHandler":
                    dpg.set_value(kh_release, f"Key id: {data}")
                elif type=="mvAppItemType::mvKeyPressHandler":
                    dpg.set_value(kh_press, f"Key id: {data} + Shift: {dpg.is_key_down(dpg.mvKey_Shift)}")
                elif type=="mvAppItemType::mvMouseClickHandler":
                     dpg.set_value(mh_click, f"Mouse id: {data} + Shift: {dpg.is_key_down(dpg.mvKey_Shift)}")
                elif type=="mvAppItemType::mvMouseDoubleClickHandler":
                    dpg.set_value(mh_double, f"Mouse id: {data}")
                elif type=="mvAppItemType::mvMouseDownHandler":
                    dpg.set_value(mh_down, f"Mouse id: {data[0]}, Seconds:{data[1]}")
                elif type=="mvAppItemType::mvMouseReleaseHandler":
                    dpg.set_value(mh_release, f"Mouse id: {data}")
                elif type=="mvAppItemType::mvMouseWheelHandler":
                    dpg.set_value(mh_wheel, f"Mouse id: {data}")
                elif type=="mvAppItemType::mvMouseMoveHandler":
                    dpg.set_value(mh_move, f"Mouse pos: {data}")
                elif type=="mvAppItemType::mvMouseDragHandler":
                    dpg.set_value(mh_drag, f"Mouse id: {data[0]}, Delta:{[data[1], data[2]]}")

            for handler in dpg.get_item_children("__demo_keyboard_handler", 1):
                dpg.set_item_callback(handler, _event_handler)

            for handler in dpg.get_item_children("__demo_mouse_handler", 1):
                dpg.set_item_callback(handler, _event_handler)

        with dpg.collapsing_header(label="Drag & Drop"):
           
           with dpg.tree_node(label="Help"):

                dpg.add_text("Adding a drag_payload to a widget makes it source.", bullet=True)
                dpg.add_text("Adding a drop_callback to a widget makes it target.", bullet=True)
                dpg.add_text("Compatibility is determined by the 'payload_type'.", bullet=True)
                dpg.add_text("The 'payload_type' must be less than 32 characters.", bullet=True)
                dpg.add_text("A 'drag_callback' can be used to notify a source during a DND event.", bullet=True)
                dpg.add_text("A 'drag_payload' is a container. Its children are what is shown when dragging.", bullet=True)

           with dpg.tree_node(label="Simple"):

                with dpg.group(horizontal=True, xoffset=200):
                    with dpg.group():

                        dpg.add_text("Int Sources:")

                        dpg.add_button(label="Source 1: 25")
                        with dpg.drag_payload(parent=dpg.last_item(), drag_data=25, payload_type="ints"):
                            dpg.add_text("25")

                        dpg.add_button(label="Source 2: 33")
                        with dpg.drag_payload(parent=dpg.last_item(), drag_data=33, payload_type="ints"):
                            dpg.add_text("33")

                        dpg.add_button(label="Source 3: 111")
                        with dpg.drag_payload(parent=dpg.last_item(), drag_data=111, payload_type="ints"):
                            dpg.add_text("111")

                    with dpg.group():

                        dpg.add_text("Float Sources:")
                        dpg.add_button(label="Source 1: 43.7")
                        with dpg.drag_payload(parent=dpg.last_item(), drag_data=43.7, payload_type="floats"):
                            dpg.add_text("43.7")

                        dpg.add_button(label="Source 2: 99.8")
                        with dpg.drag_payload(parent=dpg.last_item(), drag_data=99.8, payload_type="floats"):
                            dpg.add_text("99.8")

                        dpg.add_button(label="Source 3: -23.4")
                        with dpg.drag_payload(parent=dpg.last_item(), drag_data=-23.4, payload_type="floats"):
                            dpg.add_text("-23.4")

                    with dpg.group():

                        dpg.add_text("Targets:")

                        dpg.add_input_int(label="Int Target", payload_type="ints", width=100, step=0, drop_callback=lambda s, a: dpg.set_value(s, a))
                        dpg.add_input_float(label="Float Target", payload_type="floats", width=100, step=0, drop_callback=lambda s, a: dpg.set_value(s, a))

        with dpg.collapsing_header(label="Advanced"):

            with dpg.tree_node(label="Help (READ ME FIRST)"):
                dpg.add_text("These topics are for advanced users.", bullet=True)
                dpg.add_text("Make sure you know what you are doing.", bullet=True) #Can we remove this?

            with dpg.tree_node(label="Staging"):

                dpg.add_text("Staging can be used to create items without traditional parents.", bullet=True)
                dpg.add_text("A stage is a root container that will not attempt to render its children.", bullet=True)
                dpg.add_text("Regular parent deduction rules still apply.", bullet=True)
                dpg.add_text("Staging can be useful for wrapping a set of items.", bullet=True)
                dpg.add_text("You can use most DPG commands on staged items.", bullet=True)
                dpg.add_text("You can place any item into a stage.", bullet=True)
                dpg.add_text("Items can be unstaged with 'move_item' and 'unstage'.", bullet=True)
                dpg.add_text("'unstage' will also delete the stage.", bullet=True)
                dpg.add_text("A 'stage is a special container that when used with `unstage` will delete itself.", bullet=True)

                def _unstage_items(sender, app_data, user_data):

                    # push the child back onto the container stack
                    dpg.push_container_stack(user_data[1])

                    # this will 'unpack' the staging container (regular parent deduction rules apply)
                    dpg.unstage(user_data[0])

                    # pop the child back off the container stack
                    dpg.pop_container_stack()

                # when unstaging a stage, it 'unpacks' itself
                with dpg.stage(tag="__demo_stage1"):
                    dpg.add_button(label="Staged Button 1")
                    dpg.add_button(label="Staged Button 2")
                    dpg.add_button(label="Staged Button 3")

                ub1 = dpg.add_button(label="Unstage buttons", callback=_unstage_items)
                child_id = dpg.add_child_window(height=200, width=200)
                dpg.set_item_user_data(ub1, ["__demo_stage1", child_id])

            with dpg.tree_node(label="Manual Mutex Control"):

                dpg.add_text("DPG has a 'rendering' thread and a 'callback' thread.", bullet=True)
                dpg.add_text("Only 1 thread can hold the mutex.", bullet=True)
                dpg.add_text("The rendering thread grabs the mutex right before drawing.", bullet=True)
                dpg.add_text("The callback thread grabs the mutex when you call a DPG command.", bullet=True)
                dpg.add_text("If a callback calls multiple DPG commands, they will most likely execute over a few frames.", bullet=True)
                dpg.add_text("To ensure multiple commands run within the same frame, you can lock/unlock the mutex manually.", bullet=True)

                staged_container = dpg.generate_uuid()

                def _callback_auto_mutex(sender, app_data, user_data):
                    
                    for i in range(100):
                        dpg.add_text("Item: " + str(i), parent=user_data)

                def _callback_manual_mutex(sender, app_data, user_data):

                    dpg.lock_mutex() # you could also use with dpg.mutex()
                    dpg.push_container_stack(dpg.add_stage(id=staged_container))
                    for i in range(100):
                        dpg.add_text("Item: " + str(i))
                    dpg.pop_container_stack()
                    dpg.unlock_mutex()

                    dpg.set_item_children(user_data, staged_container, 1)

                with dpg.group(horizontal=True):
                    b1 = dpg.add_button(label="Add 100 items")
                    b2 = dpg.add_button(label="Add 100 items (mutex)")
                    b3 = dpg.add_button(label="Delete Items", callback=lambda s, a, u: dpg.delete_item(u, children_only=True))
                dpg.add_child_window(height=500, width=-1)
                dpg.configure_item(b1, user_data=dpg.last_item(), callback=_callback_auto_mutex)
                dpg.configure_item(b2, user_data=dpg.last_item(), callback=_callback_manual_mutex)
                dpg.configure_item(b3, user_data=dpg.last_item())
            if dpg.get_platform() == dpg.mvPlatform_Windows or dpg.get_platform() == dpg.mvPlatform_Linux:
                with dpg.tree_node(label="Output Framebuffer"):
                    def _framebuffer_callback(sender, app_data):
                        dpg.show_item("__demo_texture_container")
                        dpg.add_dynamic_texture(app_data.get_width(), app_data.get_height(), app_data, parent="__demo_texture_container")
                    dpg.add_text("Outputs frame buffer an mvBuffer object, creates a dynamic texture, and shows the texture registry (check final item)")
                    dpg.add_button(label="Output Framebuffer", callback=lambda:dpg.output_frame_buffer(callback=_framebuffer_callback))