import dearpygui.core as dpg
import dearpygui.contexts as cxt
import dearpygui.demo_widgets as widgets
from dearpygui.logger import mvLogger
from dearpygui.common import _log, _config, _help, _hsv_to_rgb
from math import sin, cos
import random

demo_texture_container = dpg.add_texture_container()
demo_static_texture_1 = ""
demo_static_texture_2 = ""
demo_static_texture_3 = ""
demo_dynamic_texture_1 = ""
demo_dynamic_texture_2 = ""

def _create_static_textures():
    
    global demo_static_texture_1, demo_static_texture_2, demo_static_texture_3

    ## create static textures
    texture_data1 = []
    for i in range(0, 100*100):
        texture_data1.append(255/255)
        texture_data1.append(0)
        texture_data1.append(255/255)
        texture_data1.append(255/255)

    texture_data2 = []
    for i in range(0, 50*50):
        texture_data2.append(255/255)
        texture_data2.append(255/255)
        texture_data2.append(0)
        texture_data2.append(255/255)

    texture_data3 = []
    for row in range(0, 50):
        for column in range(0, 50):
            texture_data3.append(255/255)
            texture_data3.append(0)
            texture_data3.append(0)
            texture_data3.append(255/255)
        for column in range(0, 50):
            texture_data3.append(0)
            texture_data3.append(255/255)
            texture_data3.append(0)
            texture_data3.append(255/255)
    for row in range(0, 50):
        for column in range(0, 50):
            texture_data3.append(0)
            texture_data3.append(0)
            texture_data3.append(255/255)
            texture_data3.append(255/255)
        for column in range(0, 50):
            texture_data3.append(255/255)
            texture_data3.append(255/255)
            texture_data3.append(0)
            texture_data3.append(255/255)

    demo_static_texture_1 = dpg.add_static_texture(100, 100, texture_data1, parent=demo_texture_container)
    demo_static_texture_2 = dpg.add_static_texture(50, 50, texture_data2, parent=demo_texture_container)
    demo_static_texture_3 = dpg.add_static_texture(100, 100, texture_data3, parent=demo_texture_container)

def _create_dynamic_textures():
    
    global demo_dynamic_texture_1, demo_dynamic_texture_2

    ## create dynamic textures
    texture_data1 = []
    for i in range(0, 100*100):
        texture_data1.append(255/255)
        texture_data1.append(0)
        texture_data1.append(255/255)
        texture_data1.append(255/255)

    texture_data2 = []
    for i in range(0, 50*50):
        texture_data2.append(255/255)
        texture_data2.append(255/255)
        texture_data2.append(0)
        texture_data2.append(255/255)

    demo_dynamic_texture_1 = dpg.add_dynamic_texture(100, 100, texture_data1, parent=demo_texture_container)
    demo_dynamic_texture_2 = dpg.add_dynamic_texture(50, 50, texture_data2, parent=demo_texture_container)

def _update_dynamic_textures(sender, app_data, user_data):

    global demo_dynamic_texture_1, demo_dynamic_texture_2

    new_color = dpg.get_value(sender)
    new_color[0] = new_color[0]/255
    new_color[1] = new_color[1]/255
    new_color[2] = new_color[2]/255
    new_color[3] = new_color[3]/255

    if user_data == 1:
        texture_data = []
        for i in range(0, 100*100):
            texture_data.append(new_color[0])
            texture_data.append(new_color[1])
            texture_data.append(new_color[2])
            texture_data.append(new_color[3])
        dpg.set_value(demo_dynamic_texture_1, texture_data)

    elif user_data == 2:
        texture_data = []
        for i in range(0, 50*50):
            texture_data.append(new_color[0])
            texture_data.append(new_color[1])
            texture_data.append(new_color[2])
            texture_data.append(new_color[3])
        dpg.set_value(demo_dynamic_texture_2, texture_data)

def show_demo():

    logger = mvLogger()
    logger.log_level = 0
    logger.log("trace message boi")
    logger.log_debug("debug message boi")
    logger.log_info("info message boi")
    logger.log_warning("warning message boi")
    logger.log_error("error message boi")
    logger.log_critical("critical message boi")

    def event_handler(sender, data):
        logger.log(f"{sender} '\t' {data}")

    with cxt.handler_registry(show=False) as keyboard_handler:
        key_down_handler = dpg.add_key_down_handler(-1, callback=event_handler)
        key_release_handler = dpg.add_key_release_handler(-1, callback=event_handler)
        key_press_handler = dpg.add_key_press_handler(-1, callback=event_handler)
    with cxt.handler_registry(show=False) as mouse_handler:
        mouse_wheel_handler = dpg.add_mouse_wheel_handler(callback=event_handler)
        mouse_click_handler = dpg.add_mouse_click_handler(-1, callback=event_handler)
        mouse_double_click_handler = dpg.add_mouse_double_click_handler(-1, callback=event_handler)
        mouse_release_handler = dpg.add_mouse_release_handler(-1, callback=event_handler)
        mouse_drag_handler = dpg.add_mouse_drag_handler(0, 10.0, callback=event_handler)
        mouse_down_handler = dpg.add_mouse_down_handler(-1, callback=event_handler)
        mouse_move_handler = dpg.add_mouse_move_handler(callback=event_handler)

    _create_static_textures()
    _create_dynamic_textures()

    with cxt.window(id="Dear PyGui Demo", width=800, height=800) as demo_id:
    
        with cxt.menu_bar():

            with cxt.menu(label="File"):

                dpg.add_menu_item(label="New")
                dpg.add_menu_item(label="Open")

                with cxt.menu(label="Open Recent"):

                    dpg.add_menu_item(label="harrel.c")
                    dpg.add_menu_item(label="patty.h")
                    dpg.add_menu_item(label="nick.py")

                dpg.add_menu_item(label="Save")
                dpg.add_menu_item(label="Save As...")

                with cxt.menu(label="Settings"):

                    dpg.add_menu_item(label="Option 1", callback=_log)
                    dpg.add_menu_item(label="Option 2", check=True, callback=_log)
                    dpg.add_menu_item(label="Option 3", check=True, default_value=True, callback=_log)

                    with cxt.child(height=60, autosize_x=True):
                        for i in range(0, 10):
                            dpg.add_text(f"Scolling Text{i}")

                    dpg.add_slider_float(label="Slider Float")
                    dpg.add_input_int(label="Input Int")
                    dpg.add_combo(("Yes", "No", "Maybe"), label="Combo")

            with cxt.menu(label="Tools"):

                dpg.add_menu_item(label="Show About", callback=lambda:dpg.show_tool("mvAboutWindow"))
                dpg.add_menu_item(label="Show Metrics", callback=lambda:dpg.show_tool("mvMetricsWindow"))
                dpg.add_menu_item(label="Show Documentation", callback=lambda:dpg.show_tool("mvDocWindow"))
                dpg.add_menu_item(label="Show Debug", callback=lambda:dpg.show_tool("mvDebugWindow"))
                dpg.add_menu_item(label="Show Style Editor", callback=lambda:dpg.show_tool("mvStyleWindow"))
                dpg.add_menu_item(label="Show Font Manager", callback=lambda:dpg.show_tool("mvFontManager"))
                dpg.add_menu_item(label="Show Item Registry", callback=lambda:dpg.show_tool("mvItemRegistry"))

        dpg.add_text(f'Dear PyGui says hello. ({dpg.get_dearpygui_version()})')
        dpg.add_text("This code for this demo can be found here: ")
        dpg.add_text("https://github.com/hoffstadt/DearPyGui/blob/master/DearPyGui/dearpygui/demo.py")

        with cxt.collapsing_header(label="Window Options"):

            with cxt.table(header_row=False):

                dpg.add_table_column()
                dpg.add_table_column()
                dpg.add_table_column()

                dpg.add_checkbox(label="no_title_bar", callback=_config, user_data=demo_id)
                dpg.add_checkbox(label="no_scrollbar", callback=_config, user_data=demo_id)
                dpg.add_checkbox(label="menubar", default_value=True, callback=_config, user_data=demo_id)

                dpg.add_table_next_column()
                dpg.add_checkbox(label="no_move", callback=_config, user_data=demo_id)
                dpg.add_checkbox(label="no_resize", callback=_config, user_data=demo_id)
                dpg.add_checkbox(label="no_collapse", callback=_config, user_data=demo_id)

                dpg.add_table_next_column()
                dpg.add_checkbox(label="no_close", callback=_config, user_data=demo_id)
                dpg.add_checkbox(label="no_background", callback=_config, user_data=demo_id)
                dpg.add_checkbox(label="no_bring_to_front_on_focus", callback=_config, user_data=demo_id)

        widgets.show_demo()

        with cxt.collapsing_header(label="Textures & Images"):
        
            with cxt.tree_node(label="Help"):

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
                dpg.add_text("'Showing' it, opens a manager to inspect the textures within.", bullet=True, indent=50)
                dpg.add_same_line()
                dpg.add_button(label="Press Here", small=True, callback=lambda:dpg.configure_item(demo_texture_container, show=True))
                dpg.add_separator()

            with cxt.tree_node(label="Static Textures"):

                dpg.add_separator()
                dpg.add_text("ABOUT STATIC TEXTURES:")
                dpg.add_text("Can NOT be modified after creation.", bullet=True, indent=20)
                dpg.add_text("Can be loaded from a file using the 'file' keyword.", bullet=True, indent=20)
                dpg.add_separator()

                with cxt.group(horizontal=True):

                    with cxt.group():
                        dpg.add_text("Image Button")
                        dpg.add_image_button(demo_static_texture_1)

                    with cxt.group():
                        dpg.add_text("Image")
                        dpg.add_image(demo_static_texture_2)

                    with cxt.group():
                        dpg.add_text("Image (texture size)")
                        dpg.add_image(demo_static_texture_3)

                    with cxt.group():
                        dpg.add_text("Image (2x texture size)")
                        dpg.add_image(demo_static_texture_3, width=200, height=200)

                dpg.add_image("INTERNAL_DPG_FONT_ATLAS")

            with cxt.tree_node(label="Dynamic Textures"):

                dpg.add_separator()
                dpg.add_text("ABOUT DYNAMIC TEXTURES:")
                dpg.add_text("Can be modified after creation with 'set_value'.", bullet=True, indent=20)
                dpg.add_text("New data must be the same dimensions as the original", bullet=True, indent=20)
                dpg.add_separator()

                with cxt.group():
                    dpg.add_color_picker((255, 0, 255, 255), label="Texture 1", 
                            no_side_preview=True, alpha_bar=True, width=200,
                            callback=_update_dynamic_textures, user_data=1)
                    dpg.add_text("Image Button")
                    dpg.add_image_button(demo_dynamic_texture_1, width=100, height=100)

                dpg.add_same_line()

                with cxt.group():
                    dpg.add_color_picker((255, 255, 0, 255), label="Texture 2", 
                                         no_side_preview=True, alpha_bar=True, width=200,
                                         callback=_update_dynamic_textures, user_data=2)
                    dpg.add_text("Image")
                    dpg.add_image(demo_dynamic_texture_2)

            with cxt.tree_node(label="Image Series (plots)"):

                plot_id=dpg.add_plot(label="Image Plot", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x axis", parent=plot_id)
                yaxis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y axis", parent=plot_id)
                dpg.add_image_series("INTERNAL_DPG_FONT_ATLAS", [300, 300], [400, 400], label="font atlas", parent=yaxis_id)
                dpg.add_image_series(demo_static_texture_1, [0, 0], [100, 100], label="static 1", parent=yaxis_id)
                dpg.add_image_series(demo_static_texture_2, [150, 150], [200, 200], label="static 2", parent=yaxis_id)
                dpg.add_image_series(demo_static_texture_3, [200, -150], [300, -50], label="static 3", parent=yaxis_id)
                dpg.add_image_series(demo_dynamic_texture_1, [-200, 100], [-100, 200], label="dynamic 1", parent=yaxis_id)
                dpg.add_image_series(demo_dynamic_texture_2, [-200, -100], [-150, -50], label="dynamic 2", parent=yaxis_id)

            with cxt.tree_node(label="Drawlists"):

                with cxt.drawlist(width=400, height=300):
                    dpg.draw_rectangle((0, 0), (400, 300), color=(100, 100, 100, 250), thickness=2)
                    dpg.draw_image(demo_static_texture_3, [0, 0], [100, 100])
                    dpg.draw_image(demo_dynamic_texture_1, [200, 100], [300, 200])

        with cxt.collapsing_header(label="Popups & Modal Windows"):
            pass

        with cxt.collapsing_header(label="Tooltips"):
            pass

        with cxt.collapsing_header(label="Tables"):

            with cxt.tree_node(label="Basic"):

                # basic usage of the table api
                with cxt.table(header_row=False):

                    # use add_table_column to add columns to the table,
                    # table columns use slot 0
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()

                    # add_table_next_column will jump to the next row
                    # once it reaches the end of the columns
                    # table next column use slot 1
                    for i in range(0, 4):
                        for j in range(0, 3):
                            dpg.add_text(f"Row{i} Column{j}")
                            if not (i == 3 and j == 2):
                                dpg.add_table_next_column()

            with cxt.tree_node(label="Borders, background") as section:

                with cxt.table(header_row=False, row_background=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True) as table_id:

                    dpg.add_table_column(label="Header 1")
                    dpg.add_table_column(label="Header 2")
                    dpg.add_table_column(label="Header 3")

                    for i in range(0, 5):
                        for j in range(0, 3):
                            dpg.add_text(f"Row{i} Column{j}")
                            if not (i == 4 and j == 2):
                                dpg.add_table_next_column()

                    dpg.add_checkbox(label="row_background", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, row_background=dpg.get_value(sender)))
                    dpg.add_checkbox(label="borders_innerH", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_innerH=dpg.get_value(sender)))
                    dpg.add_checkbox(label="borders_innerV", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_innerV=dpg.get_value(sender)))
                    dpg.add_checkbox(label="borders_outerH", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_outerH=dpg.get_value(sender)))
                    dpg.add_checkbox(label="borders_outerV", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_outerV=dpg.get_value(sender)))
                    dpg.add_checkbox(label="header_row", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, header_row=dpg.get_value(sender)))

            with cxt.tree_node(label="Colors"):
                dpg.add_text("Coming soon...")

            with cxt.tree_node(label="Resizable, stretch"):

                with cxt.table(header_row=False, resizable=True,
                            borders_outerH=True, borders_innerV=True, borders_outerV=True) as table_id:

                    dpg.add_table_column(label="Header 1")
                    dpg.add_table_column(label="Header 2")
                    dpg.add_table_column(label="Header 3")

                    for i in range(0, 5):
                        for j in range(0, 3):
                            dpg.add_text(f"Row{i} Column{j}")
                            if not (i == 4 and j == 2):
                                dpg.add_table_next_column()

                dpg.add_checkbox(label="borders_innerV", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_innerV=dpg.get_value(sender)))
                dpg.add_checkbox(label="borders_outerV", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_outerV=dpg.get_value(sender)))
                dpg.add_checkbox(label="resizable", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, resizable=dpg.get_value(sender)))

            with cxt.tree_node(label="Resizable, fixed"):

                dpg.add_text("Only available if scrollX/scrollY are disabled and stretch columns are not used")
                with cxt.table(header_row=False, policy=dpg.mvTable_SizingFixedFit, resizable=True, no_host_extendX=False, 
                            borders_innerV=True, borders_outerV=True,borders_outerH=True) as table_id:

                    dpg.add_table_column(label="Header 1")
                    dpg.add_table_column(label="Header 2")
                    dpg.add_table_column(label="Header 3")

                    for i in range(0, 5):
                        for j in range(0, 3):
                            dpg.add_text(f"Row{i} Column{j}")
                            if not (i == 4 and j == 2):
                                dpg.add_table_next_column()

                dpg.add_checkbox(label="no_host_extendX", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_host_extendX=dpg.get_value(sender)))

            with cxt.tree_node(label="Resizable, mixed"):

                with cxt.table(header_row=True, policy=dpg.mvTable_SizingFixedFit, row_background=True, reorderable=True, 
                            resizable=True, no_host_extendX=False, hideable=True, 
                            borders_innerV=True, borders_outerV=True, borders_innerH=True, borders_outerH=True):

                    dpg.add_table_column(label="AAA", width_fixed=True)
                    dpg.add_table_column(label="BBB", width_fixed=True)
                    dpg.add_table_column(label="CCC", width_stretch=True, init_width_or_weight=0.0)

                    for i in range(0, 5):
                        for j in range(0, 3):
                            if j == 2:
                                dpg.add_text(f"Stretch {i}, {j}")
                            else:
                                dpg.add_text(f"Fixed {i}, {j}")
                            if not (i == 4 and j == 2):
                                dpg.add_table_next_column()

                with cxt.table(header_row=True, policy=dpg.mvTable_SizingFixedFit, row_background=True, reorderable=True, 
                            resizable=True, no_host_extendX=False, hideable=True, 
                            borders_innerV=True, borders_outerV=True, borders_innerH=True, borders_outerH=True):

                    dpg.add_table_column(label="AAA", width_fixed=True)
                    dpg.add_table_column(label="BBB", width_fixed=True)
                    dpg.add_table_column(label="CCC", width_stretch=True, init_width_or_weight=0.0)
                    dpg.add_table_column(label="DDD", width_stretch=True, init_width_or_weight=0.0)

                    for i in range(0, 5):
                        for j in range(0, 4):
                            if j == 2 or j == 3:
                                dpg.add_text(f"Stretch {i},{j}")
                            else:
                                dpg.add_text(f"Fixed {i}, {j}")
                            if not (i == 4 and j == 3):
                                dpg.add_table_next_column()

            with cxt.tree_node(label="Columns Options"):

                with cxt.table(header_row=True, no_host_extendX=True,
                                borders_innerH=True, borders_outerH=True, borders_innerV=True,
                                borders_outerV=True, row_background=True, hideable=True, reorderable=True,
                                resizable=True, sortable=True, policy=dpg.mvTable_SizingFixedFit,
                                scrollX=True, scrollY=True) as table_id:

                    c1 = dpg.add_table_column(label="One", default_sort=True)
                    c2 = dpg.add_table_column(label="Two")
                    c3 = dpg.add_table_column(label="Three", default_hide=True)

                    for i in range(0, 7):
                        dpg.add_text("Indented One", indent=5*i)
                        dpg.add_table_next_column()
                        dpg.add_text("Hello Two")
                        dpg.add_table_next_column()
                        dpg.add_text("Hello Three")
                        dpg.add_table_next_column()
                    dpg.add_text("Indented One", indent=5*(i+1))
                    dpg.add_table_next_column()
                    dpg.add_text("Hello Two")
                    dpg.add_table_next_column()
                    dpg.add_text("Hello Three")

                # options table
                with cxt.table(header_row=False, before=table_id):

                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()

                    def add_option_column(column, item, value1, value2):

                        dpg.add_text("'" + column + "'")
                        dpg.add_checkbox(label="default_hide", default_value=value1, callback=lambda sender: dpg.configure_item(item, default_hide=dpg.get_value(sender)))
                        dpg.add_checkbox(label="default_sort", default_value=value2, callback=lambda sender: dpg.configure_item(item, default_sort=dpg.get_value(sender)))
                        dpg.add_checkbox(label="width_stretch", callback=lambda sender: dpg.configure_item(item, width_stretch=dpg.get_value(sender)))
                        dpg.add_checkbox(label="width_fixed", callback=lambda sender: dpg.configure_item(item, width_fixed=dpg.get_value(sender)))
                        dpg.add_checkbox(label="no_resize", callback=lambda sender: dpg.configure_item(item, no_resize=dpg.get_value(sender)))
                        dpg.add_checkbox(label="no_reorder", callback=lambda sender: dpg.configure_item(item, no_reorder=dpg.get_value(sender)))
                        dpg.add_checkbox(label="no_hide", callback=lambda sender: dpg.configure_item(item, no_hide=dpg.get_value(sender)))
                        dpg.add_checkbox(label="no_clip", callback=lambda sender: dpg.configure_item(item, no_clip=dpg.get_value(sender)))
                        dpg.add_checkbox(label="no_sort", callback=lambda sender: dpg.configure_item(item, no_sort=dpg.get_value(sender)))
                        dpg.add_checkbox(label="no_sort_ascending", callback=lambda sender: dpg.configure_item(item, no_sort_ascending=dpg.get_value(sender)))
                        dpg.add_checkbox(label="no_sort_descending", callback=lambda sender: dpg.configure_item(item, no_sort_descending=dpg.get_value(sender)))
                        dpg.add_checkbox(label="no_header_width", callback=lambda sender: dpg.configure_item(item, no_header_width=dpg.get_value(sender)))
                        dpg.add_checkbox(label="prefer_sort_ascending", callback=lambda sender: dpg.configure_item(item, prefer_sort_ascending=dpg.get_value(sender)))
                        dpg.add_checkbox(label="prefer_sort_descending", callback=lambda sender: dpg.configure_item(item, prefer_sort_descending=dpg.get_value(sender)))
                        dpg.add_checkbox(label="indent_enable", callback=lambda sender: dpg.configure_item(item, indent_enable=dpg.get_value(sender)))
                        dpg.add_checkbox(label="indent_disable", callback=lambda sender: dpg.configure_item(item, indent_disable=dpg.get_value(sender)))

                    add_option_column("One", c1, False, True)
                    dpg.add_table_next_column()
                    add_option_column("Two", c2, False, False)
                    dpg.add_table_next_column()
                    add_option_column("Three", c3, True, False)

            with cxt.tree_node(label="Columns widths"):

                with cxt.table(header_row=True, resizable=True,
                            borders_outerH=True, borders_innerH=True, 
                            borders_outerV=True):

                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="Three")

                    t11 = dpg.add_text("(w: 0.0f)")
                    dpg.add_table_next_column()
                    t22 = dpg.add_text("(w: 0.0f)")
                    dpg.add_table_next_column()
                    t33 = dpg.add_text("(w: 0.0f)")
                    dpg.add_table_next_column()

                    for i in range(0, 3):
                        for j in range(0, 3):
                            dpg.add_text(f"Hello {i}, {j}")
                            if not (i == 2 and j == 2):
                                dpg.add_table_next_column()

                    dpg.add_visible_handler(t11, callback=lambda:dpg.set_value(t11, "(w: " + str(dpg.get_item_state(t11)["content_region_avail"][0]) + ")"))
                    dpg.add_visible_handler(t22, callback=lambda:dpg.set_value(t22, "(w: " + str(dpg.get_item_state(t22)["content_region_avail"][0]) + ")"))
                    dpg.add_visible_handler(t33, callback=lambda:dpg.set_value(t33, "(w: " + str(dpg.get_item_state(t33)["content_region_avail"][0]) + ")"))

                with cxt.table(header_row=False) as table_id:

                    dpg.add_table_column(width_fixed=True, init_width_or_weight=100)
                    dpg.add_table_column(width_fixed=True, init_width_or_weight=200)
                    dpg.add_table_column(width_fixed=True, init_width_or_weight=300)
                    dpg.add_table_column(width_fixed=True, init_width_or_weight=400)

                    t1 = dpg.add_text("(w: 0.0f)")
                    dpg.add_table_next_column()
                    t2 = dpg.add_text("(w: 0.0f)")
                    dpg.add_table_next_column()
                    t3 = dpg.add_text("(w: 0.0f)")
                    dpg.add_table_next_column()
                    t4 = dpg.add_text("(w: 0.0f)")
                    dpg.add_table_next_column()

                    for i in range(0, 4):
                        for j in range(0, 4):
                            dpg.add_text(f"Hello {i}, {j}")
                            if not (i == 3 and j == 3):
                                dpg.add_table_next_column()

                    dpg.add_visible_handler(t1, callback=lambda:dpg.set_value(t1, "(w: " + str(dpg.get_item_state(t1)["content_region_avail"][0]) + ")"))
                    dpg.add_visible_handler(t2, callback=lambda:dpg.set_value(t2, "(w: " + str(dpg.get_item_state(t2)["content_region_avail"][0]) + ")"))
                    dpg.add_visible_handler(t3, callback=lambda:dpg.set_value(t3, "(w: " + str(dpg.get_item_state(t3)["content_region_avail"][0]) + ")"))
                    dpg.add_visible_handler(t4, callback=lambda:dpg.set_value(t4, "(w: " + str(dpg.get_item_state(t4)["content_region_avail"][0]) + ")"))

                dpg.add_checkbox(label="no_keep_columns_visible", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_keep_columns_visible=dpg.get_value(sender)))
                dpg.add_checkbox(label="borders_innerV", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_innerV=dpg.get_value(sender)))
                dpg.add_checkbox(label="borders_outerV", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_outerV=dpg.get_value(sender)))
 
            with cxt.tree_node(label="Row height"):

                with cxt.table(header_row=False, borders_outerH=True, borders_outerV=True):

                    dpg.add_table_column()

                    for i in range(0, 10):
                        with cxt.table_row(height=i*5 + 25):
                            dpg.add_text("height = " + str(i*5 + 25))

            with cxt.tree_node(label="Padding"):

                with cxt.table(header_row=False, resizable=True,
                            hideable=True, reorderable=True, borders_outerV=True, borders_innerH=True) as table_id:

                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="three")

                    for i in range(0, 5):
                        for j in range(0, 3):
                            dpg.add_button(label=f"Hello {i}, {j}", width=-1)
                            if not (i == 4 and j == 2):
                                dpg.add_table_next_column()

                dpg.add_checkbox(label="pad_outerX", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, pad_outerX=dpg.get_value(sender)))
                dpg.add_checkbox(label="no_pad_outerX", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_pad_outerX=dpg.get_value(sender)))
                dpg.add_checkbox(label="no_pad_innerX", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_pad_innerX=dpg.get_value(sender)))
                dpg.add_checkbox(label="borders_outerV", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_outerV=dpg.get_value(sender)))
                dpg.add_checkbox(label="borders_innerV", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, borders_innerV=dpg.get_value(sender)))
                dpg.add_checkbox(label="header_row", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, header_row=dpg.get_value(sender)))

            with cxt.tree_node(label="Reorderable, hideable, with headers"):

                with cxt.table(header_row=True, resizable=True,
                            hideable=True, reorderable=True) as table_id:

                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="three")

                    for i in range(0, 5):
                        for j in range(0, 3):
                            dpg.add_text(f"Hello {i}, {j}")
                            if not (i == 4 and j == 2):
                                dpg.add_table_next_column()

                dpg.add_checkbox(label="hideable", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, hideable=dpg.get_value(sender)))
                dpg.add_checkbox(label="reorderable", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, reorderable=dpg.get_value(sender)))
                dpg.add_checkbox(label="resizable", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, resizable=dpg.get_value(sender)))

            with cxt.tree_node(label="Outer Size"):

                with cxt.table(header_row=False, no_host_extendX=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, context_menu_in_body=True, row_background=True,
                            policy=dpg.mvTable_SizingFixedFit, height=150) as table_id:
                
                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="three")

                    for i in range(0, 10):
                        for j in range(0, 3):
                            dpg.add_text(f"Cell {i}, {j}")
                            if not (i == 9 and j == 2):
                                dpg.add_table_next_column()

                dpg.add_checkbox(label="no_host_extendX", default_value=True, before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_host_extendX=dpg.get_value(sender)))
                dpg.add_checkbox(label="no_host_extendY", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, no_host_extendY=dpg.get_value(sender)))


                dpg.add_text("Using explicit size:")
                with cxt.table(header_row=False, no_host_extendX=True,
                            borders_innerH=True, borders_outerH=True, borders_innerV=True,
                            borders_outerV=True, context_menu_in_body=True, row_background=True,
                            policy=dpg.mvTable_SizingFixedFit, height=300, width=300):
                
                    dpg.add_table_column(label="One")
                    dpg.add_table_column(label="Two")
                    dpg.add_table_column(label="three")

                    for i in range(0, 6):
                        for j in range(0, 3):
                            dpg.add_text(f"Cell {i}, {j}")
                            if not (i == 9 and j == 2):
                                dpg.add_table_next_column()

            with cxt.tree_node(label="Scrolling, Clipping"):

                def on_open(sender, app_data, user_data):

                    first_item_id = "$$DEMO_Table_Scrolling, Clipping"

                    if dpg.does_item_exist(first_item_id):
                        return

                    dpg.push_container_stack(dpg.get_item_info(sender)["parent"])

                    # locking mutex to ensure the table and columns are added in the same frame
                    dpg.lock_mutex()

                    # without clipping
                    dpg.add_text("Without Clipper", id=first_item_id)
                    with cxt.table(header_row=True, no_host_extendX=True,
                                borders_innerH=True, borders_outerH=True, borders_innerV=True,
                                borders_outerV=True, context_menu_in_body=True, row_background=True,
                                policy=dpg.mvTable_SizingFixedFit, height=300,
                                scrollY=True):

                        dpg.add_table_column(label="1")
                        dpg.add_table_column(label="2")
                        dpg.add_table_column(label="3")

                        # unlocking mutex, data can be added across multiple frames
                        dpg.unlock_mutex()

                        for i in range(0, 100):
                            with cxt.table_row():
                                dpg.add_input_int(label=" ", step=0)
                                dpg.add_button(label=f"Cell {i}, 1")
                                dpg.add_text(f"Cell {i}, 2")

                    # locking mutex to ensure the table and columns are added in the same frame
                    dpg.lock_mutex()

                    # with clipping
                    dpg.add_text("Using Clipper")
                    with cxt.table(header_row=True, no_host_extendX=True,
                                borders_innerH=True, borders_outerH=True, borders_innerV=True,
                                borders_outerV=True, context_menu_in_body=True, row_background=True,
                                policy=dpg.mvTable_SizingFixedFit, height=300,
                                scrollY=True):

                        dpg.add_table_column(label="1")
                        dpg.add_table_column(label="2")
                        dpg.add_table_column(label="3")

                        # unlocking mutex, data can be added across multiple frames
                        dpg.unlock_mutex()

                        for i in range(0, 100):
                            with cxt.clipper():
                                with cxt.table_row():
                                    dpg.add_input_int(label=" ", step=0)
                                    dpg.add_button(label=f"Cell {i}, 1")
                                    dpg.add_text(f"Cell {i}, 2")
                    dpg.add_checkbox(label="resizable", before=table_id, default_value=True, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, resizable=dpg.get_value(sender)))

                    # locking mutex to ensure the table and columns are added in the same frame
                    dpg.lock_mutex()

                    # Freezing rows/columns
                    dpg.add_text("Freezing rows/columns")
                    with cxt.table(header_row=True, borders_innerH=True, borders_outerH=True, borders_innerV=True,
                                borders_outerV=True, row_background=True, height=300, freeze_rows=1, freeze_columns=1,
                                scrollY=True, scrollX=True, policy=dpg.mvTable_SizingFixedFit):

                        dpg.add_table_column(label="1", width=50)
                        dpg.add_table_column(label="2", width=50)
                        dpg.add_table_column(label="3", width=50)
                        dpg.add_table_column(label="4", width=50)
                        dpg.add_table_column(label="5", width=50)
                        dpg.add_table_column(label="6", width=50)
                        dpg.add_table_column(label="7", width=50)

                        # unlocking mutex, data can be added across multiple frames
                        dpg.unlock_mutex()

                        for i in range(0, 100):
                                with cxt.table_row():
                                    dpg.add_text(f"Cell {i}, 0")
                                    dpg.add_button(label=f"Cell {i}, 1")
                                    dpg.add_text(f"Cell {i}, 2")
                                    dpg.add_text(f"Cell {i}, 3")
                                    dpg.add_text(f"Cell {i}, 4")
                                    dpg.add_text(f"Cell {i}, 5")
                                    dpg.add_text(f"Cell {i}, 6")

                    dpg.pop_container_stack()

                dpg.add_activated_handler(dpg.top_container_stack(), callback=on_open)

            with cxt.tree_node(label="Sorting"):

                def sort_callback(sender, app_data):

                    children = dpg.get_item_info(sender)["children"][1]
                    
                    oldList = []
                    col1 = []
                    col2 = []
                    i = 0
                    j = 0
                    while i < len(children)-5:
                        row = []

                        col1.append(children[i])
                        col2.append(children[i+2])

                        row.append(children[i])
                        row.append(children[i+1])
                        row.append(children[i+2])
                        row.append(children[i+3])
                        row.append(children[i+4])
                        row.append(children[i+5])
                        row.append(j)
                        oldList.append(row)
                        i+=6
                        j+=1
                        
                    col1values = dpg.get_values(col1)
                    col2values = dpg.get_values(col2)

                    def col1_sorter(e):
                        return col1values[e[6]]
                    def col2_sorter(e):
                        return col2values[e[6]]

                    reverse = False
                    if app_data[0][1] < 0:
                        reverse = True

                    if app_data[0][0] == dpg.get_item_info(sender)["children"][0][0]:
                        oldList.sort(key=col1_sorter, reverse=reverse)
                    elif app_data[0][0] == dpg.get_item_info(sender)["children"][0][1]:
                        oldList.sort(key=col2_sorter, reverse=reverse)

                    single_list = []
                    for row in oldList:
                        for cell in range(0, len(row)-1):
                            single_list.append(row[cell])
                        
                    dpg.reorder_items(sender, 1, single_list)

                def on_open(sender, app_data, user_data):

                    first_item_id = "$$DEMO_Table_Sorting"

                    if dpg.does_item_exist(first_item_id):
                        return

                    dpg.push_container_stack(dpg.get_item_info(sender)["parent"])

                    # locking mutex to ensure the table and columns are added in the same frame
                    dpg.lock_mutex()

                    dpg.add_text("Sorting", id=first_item_id)
                    with cxt.table(header_row=True, no_host_extendX=True,
                                borders_innerH=True, borders_outerH=True, borders_innerV=True,
                                borders_outerV=True, context_menu_in_body=True, row_background=True,
                                policy=dpg.mvTable_SizingFixedFit, height=500, sortable=True, callback=sort_callback,
                                scrollY=True) as table_id:

                        dpg.add_table_column(label="One")
                        dpg.add_table_column(label="Two")
                        dpg.add_table_column(label="Three")

                        # unlocking mutex, data can be added across multiple frames
                        dpg.unlock_mutex()

                        for i in range(0, 1000):
                                dpg.add_input_int(label=" ", step=0)
                                dpg.add_table_next_column()
                                dpg.add_text(f"Cell {i}, 1")
                                dpg.add_table_next_column()
                                dpg.add_checkbox(label=f"Cell {i}, 2")
                                if i != 999:
                                    dpg.add_table_next_column()

                    dpg.add_checkbox(label="sort_multi", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, sort_multi=dpg.get_value(sender)))
                    dpg.add_checkbox(label="sort_tristate", before=table_id, user_data=table_id, callback=lambda sender, app_data, user_data:dpg.configure_item(user_data, sort_tristate=dpg.get_value(sender)))

                    dpg.pop_container_stack()

                dpg.add_activated_handler(dpg.top_container_stack(), callback=on_open)

            with cxt.tree_node(label="Sizing Policy"):

                def callback(sender, app_data, user_data):

                    if user_data[8] == "resizable":
                        value = dpg.get_value(sender)
                        for i in range(0, 8):
                            dpg.configure_item(user_data[i], resizable=value)

                    elif user_data[8] == "no_host_extendX":
                        value = dpg.get_value(sender)
                        for i in range(0, 8):
                            dpg.configure_item(user_data[i], no_host_extendX=value)

                    elif user_data[8] == "policy":

                        value = dpg.get_value(sender)
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

                    with cxt.table(header_row=False, policy=policy,
                                borders_innerH=True, borders_outerH=True, borders_innerV=True,
                                borders_outerV=True, row_background=True) as table_id1:
                
                        dpg.add_table_column()
                        dpg.add_table_column()
                        dpg.add_table_column()

                        for i in range(0, 8):
                            dpg.add_text("Oh dear")
                            dpg.add_table_next_column()
                        dpg.add_text("Oh dear")

                    with cxt.table(header_row=False, policy=policy,
                                borders_innerH=True, borders_outerH=True, borders_innerV=False,
                                borders_outerV=True, row_background=True) as table_id2:
                
                        dpg.add_table_column()
                        dpg.add_table_column()
                        dpg.add_table_column()

                        for i in range(0, 2):
                            dpg.add_text("AAAA")
                            dpg.add_table_next_column()
                            dpg.add_text("BBBBBBBB")
                            dpg.add_table_next_column()
                            dpg.add_text("CCCCCCCCCCCC")
                            dpg.add_table_next_column()
                        dpg.add_text("AAAA")
                        dpg.add_table_next_column()
                        dpg.add_text("BBBBBBBB")
                        dpg.add_table_next_column()
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

        with cxt.collapsing_header(label="Plots"):

            sindatax = []
            sindatay = []
            for i in range(0, 100):
                sindatax.append(i/100)
                sindatay.append(0.5 + 0.5*sin(50*i/100))
        
            with cxt.tree_node(label="Help"):

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

            with cxt.tree_node(label="Line Series"):

                dpg.add_text("Anti-aliasing can be enabled from the plot's context menu (see Help).", bullet=True)
                plot_id = dpg.add_plot(label="Line Series", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id)
                dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=axis_id)

            with cxt.tree_node(label="Stair Series"):

                plot_id = dpg.add_plot(label="Stair Plot", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id)
                dpg.add_stair_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=axis_id)

            with cxt.tree_node(label="Shade Series"):

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

                plot_id = dpg.add_plot(label="Stock Prices", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="Days", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="Price", parent=plot_id)
                #dpg.add_line_series(stock_datax, stock_data1, label="Stock 1", color=[0, 0, 255, 255], parent=axis_id)
                #dpg.add_line_series(stock_datax, stock_data2, label="Stock 2", color=[255, 0, 0, 255], parent=axis_id)
                #dpg.add_line_series(stock_datax, stock_data3, label="Stock 3", color=[0, 255, 0, 255], parent=axis_id)
                #dpg.add_shade_series(stock_datax, stock_data1, label="Stock 1", fill=[0, 0, 255, 64], parent=axis_id)
                #dpg.add_shade_series(stock_datax, stock_data2, label="Stock 2", fill=[255, 0, 0, 64], parent=axis_id)
                #dpg.add_shade_series(stock_datax, stock_data3, label="Stock 3", y2=stock_datay2, fill=[0, 255, 0, 64], parent=axis_id)
                #dpg.add_shade_series(stock_datax, stock_data5, y2=stock_data4, label="Shade between lines", fill=[255, 255, 100, 64], parent=axis_id)

            with cxt.tree_node(label="Scatter Series"):

                plot_id = dpg.add_plot(label="Scatter Series", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id)
                dpg.add_scatter_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=axis_id)

            with cxt.tree_node(label="Stem Series"):

                plot_id = dpg.add_plot(label="Stem Series", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id)
                dpg.add_stem_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=axis_id)

            with cxt.tree_node(label="Bar Series"):

                plot_id = dpg.add_plot(label="Bar Series", height=400)
                dpg.add_plot_legend(parent=plot_id)
                xaxis_id = dpg.add_plot_axis(dpg.mvXAxis, label="Student", no_gridlines=True, parent=plot_id)
                yaxis_id = dpg.add_plot_axis(dpg.mvYAxis, label="Score", parent=plot_id)
                dpg.set_axis_limits(xaxis_id, 9, 33)
                dpg.set_axis_limits(yaxis_id, 0, 110)
                dpg.set_axis_ticks(xaxis_id, [["S1", 11], ["S2", 21], ["S3", 31]])

                dpg.add_bar_series([10, 20, 30], [100, 75, 90], label="Final Exam", weight=1, parent=yaxis_id)
                dpg.add_bar_series([11, 21, 31], [83, 75, 72], label="Midterm Exam", weight=1, parent=yaxis_id)
                dpg.add_bar_series([12, 22, 32], [42, 68, 23], label="Course Grade", weight=1, parent=yaxis_id)

            with cxt.tree_node(label="Area Series"):

                plot_id = dpg.add_plot(label="Area Series", height=400)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id)
                dpg.add_area_series([1,5,3],[0,0,3], fill=[255,50,100,190], parent=axis_id)

            with cxt.tree_node(label="Infinite Lines"):

                infinite_x_data = (3, 5, 6, 7)
                infinite_y_data = (3, 5, 6, 7)

                plot_id = dpg.add_plot(label="Infinite Lines", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id)
                dpg.add_vline_series(infinite_x_data, label="vertical", parent=axis_id)
                dpg.add_hline_series(infinite_y_data, label="horizontal", parent=axis_id)

            with cxt.tree_node(label="Image Series"):

                plot_id=dpg.add_plot(label="Image Plot", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                yaxis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y axis", parent=plot_id)
                dpg.add_image_series("INTERNAL_DPG_FONT_ATLAS", [300, 300], [400, 400], label="font atlas", parent=yaxis_id)
                dpg.add_image_series(demo_static_texture_2, [150, 150], [200, 200], label="static 2", parent=yaxis_id)
                dpg.add_image_series(demo_dynamic_texture_1, [-200, 100], [-100, 200], label="dynamic 1", parent=yaxis_id)

            with cxt.tree_node(label="Multi Axes Plot"):

                plot_id = dpg.add_plot(label="Multi Axes Plot", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis1_id = dpg.add_plot_axis(dpg.mvYAxis, label="y1", parent=plot_id)
                axis2_id = dpg.add_plot_axis(dpg.mvYAxis, label="y2", parent=plot_id)
                axis3_id = dpg.add_plot_axis(dpg.mvYAxis, label="y3", parent=plot_id)
                dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=axis1_id)
                dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=axis2_id)
                dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=axis3_id)

            with cxt.tree_node(label="Annotations"):

                plot_id = dpg.add_plot(label="Annotations", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id)
                dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=axis_id)
                dpg.add_plot_annotation(label="BL", default_value=(0.25, 0.25), offset=(-15, 15), color=[255, 255, 0, 255], parent=plot_id)
                dpg.add_plot_annotation(label="BR", default_value=(0.75, 0.25), offset=(15, 15), color=[255, 255, 0, 255], parent=plot_id)
                dpg.add_plot_annotation(label="TR not clampled", default_value=(0.75, 0.75), offset=(-15, -15), color=[255, 255, 0, 255], clamped=False, parent=plot_id)
                dpg.add_plot_annotation(label="TL", default_value=(0.25, 0.75), offset=(-15, -15), color=[255, 255, 0, 255], parent=plot_id)
                dpg.add_plot_annotation(label="Center", default_value=(0.5, 0.5), color=[255, 255, 0, 255], parent=plot_id)

            with cxt.tree_node(label="Drag Lines/Points"):

                plot_id = dpg.add_plot(label="Annotations", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id)
                dpg.add_drag_line(label="dline1", color=[255, 0, 0, 255], parent=plot_id)
                dpg.add_drag_line(label="dline2", color=[255, 255, 0, 255], vertical=False, parent=plot_id)
                dpg.add_drag_point(label="dpoint1", color=[255, 0, 255, 255], parent=plot_id)
                dpg.add_drag_point(label="dpoint2", color=[255, 0, 255, 255], parent=plot_id)

            with cxt.tree_node(label="Querying"):

                dpg.add_text("Click and drag the middle mouse button!")
                def query(sender, app_data, user_data):
                    dpg.set_axis_limits(user_data[0], app_data[0], app_data[1])
                    dpg.set_axis_limits(user_data[1], app_data[2], app_data[3])

                plot_id1 = dpg.add_plot(no_title=True, height=400, callback=query, query=True, no_menus=True)
                plot_id2 = dpg.add_plot(no_title=True, height=400, no_menus=True)
                xaxis_id1 = dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id1)
                xaxis_id2 = dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id2)
                yaxis_id1 = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id1)
                yaxis_id2 = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id2)
                dpg.configure_item(plot_id1, user_data=(xaxis_id2,yaxis_id2))
                dpg.add_line_series(sindatax, sindatay, parent=yaxis_id1)
                dpg.add_line_series(sindatax, sindatay, parent=yaxis_id2)

            with cxt.tree_node(label="Time Series"):

                timedatax = []
                timedatay = []
                
                time_index = 0
                while time_index < 739497600:
                    timedatax.append(time_index)
                    timedatay.append(time_index/(60*60*24))
                    time_index+=60*60*24*7
                
                dpg.add_text("When time is enabled, x-axis values are interpreted as UNIX timestamps in seconds (e.g. 1599243545).", bullet=True)
                dpg.add_text("UNIX timestamps are seconds since 00:00:00 UTC on 1 January 1970", bullet=True)
                plot_id = dpg.add_plot(label="Time Plot", height=400)
                dpg.add_plot_axis(dpg.mvXAxis, label="Date", parent=plot_id, time=True)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="Days since 1970", parent=plot_id)
                dpg.add_line_series(timedatax, timedatay, label="Days", parent=axis_id)

            with cxt.tree_node(label="Candle Stick Series"):

                dates = [1546300800,1546387200,1546473600,1546560000,1546819200,1546905600,1546992000,1547078400,1547164800,1547424000,1547510400,1547596800,1547683200,1547769600,1547942400,1548028800,1548115200,1548201600,1548288000,1548374400,1548633600,1548720000,1548806400,1548892800,1548979200,1549238400,1549324800,1549411200,1549497600,1549584000,1549843200,1549929600,1550016000,1550102400,1550188800,1550361600,1550448000,1550534400,1550620800,1550707200,1550793600,1551052800,1551139200,1551225600,1551312000,1551398400,1551657600,1551744000,1551830400,1551916800,1552003200,1552262400,1552348800,1552435200,1552521600,1552608000,1552867200,1552953600,1553040000,1553126400,1553212800,1553472000,1553558400,1553644800,1553731200,1553817600,1554076800,1554163200,1554249600,1554336000,1554422400,1554681600,1554768000,1554854400,1554940800,1555027200,1555286400,1555372800,1555459200,1555545600,1555632000,1555891200,1555977600,1556064000,1556150400,1556236800,1556496000,1556582400,1556668800,1556755200,1556841600,1557100800,1557187200,1557273600,1557360000,1557446400,1557705600,1557792000,1557878400,1557964800,1558051200,1558310400,1558396800,1558483200,1558569600,1558656000,1558828800,1558915200,1559001600,1559088000,1559174400,1559260800,1559520000,1559606400,1559692800,1559779200,1559865600,1560124800,1560211200,1560297600,1560384000,1560470400,1560729600,1560816000,1560902400,1560988800,1561075200,1561334400,1561420800,1561507200,1561593600,1561680000,1561939200,1562025600,1562112000,1562198400,1562284800,1562544000,1562630400,1562716800,1562803200,1562889600,1563148800,1563235200,1563321600,1563408000,1563494400,1563753600,1563840000,1563926400,1564012800,1564099200,1564358400,1564444800,1564531200,1564617600,1564704000,1564963200,1565049600,1565136000,1565222400,1565308800,1565568000,1565654400,1565740800,1565827200,1565913600,1566172800,1566259200,1566345600,1566432000,1566518400,1566777600,1566864000,1566950400,1567036800,1567123200,1567296000,1567382400,1567468800,1567555200,1567641600,1567728000,1567987200,1568073600,1568160000,1568246400,1568332800,1568592000,1568678400,1568764800,1568851200,1568937600,1569196800,1569283200,1569369600,1569456000,1569542400,1569801600,1569888000,1569974400,1570060800,1570147200,1570406400,1570492800,1570579200,1570665600,1570752000,1571011200,1571097600,1571184000,1571270400,1571356800,1571616000,1571702400,1571788800,1571875200,1571961600]
                opens = [1284.7,1319.9,1318.7,1328,1317.6,1321.6,1314.3,1325,1319.3,1323.1,1324.7,1321.3,1323.5,1322,1281.3,1281.95,1311.1,1315,1314,1313.1,1331.9,1334.2,1341.3,1350.6,1349.8,1346.4,1343.4,1344.9,1335.6,1337.9,1342.5,1337,1338.6,1337,1340.4,1324.65,1324.35,1349.5,1371.3,1367.9,1351.3,1357.8,1356.1,1356,1347.6,1339.1,1320.6,1311.8,1314,1312.4,1312.3,1323.5,1319.1,1327.2,1332.1,1320.3,1323.1,1328,1330.9,1338,1333,1335.3,1345.2,1341.1,1332.5,1314,1314.4,1310.7,1314,1313.1,1315,1313.7,1320,1326.5,1329.2,1314.2,1312.3,1309.5,1297.4,1293.7,1277.9,1295.8,1295.2,1290.3,1294.2,1298,1306.4,1299.8,1302.3,1297,1289.6,1302,1300.7,1303.5,1300.5,1303.2,1306,1318.7,1315,1314.5,1304.1,1294.7,1293.7,1291.2,1290.2,1300.4,1284.2,1284.25,1301.8,1295.9,1296.2,1304.4,1323.1,1340.9,1341,1348,1351.4,1351.4,1343.5,1342.3,1349,1357.6,1357.1,1354.7,1361.4,1375.2,1403.5,1414.7,1433.2,1438,1423.6,1424.4,1418,1399.5,1435.5,1421.25,1434.1,1412.4,1409.8,1412.2,1433.4,1418.4,1429,1428.8,1420.6,1441,1460.4,1441.7,1438.4,1431,1439.3,1427.4,1431.9,1439.5,1443.7,1425.6,1457.5,1451.2,1481.1,1486.7,1512.1,1515.9,1509.2,1522.3,1513,1526.6,1533.9,1523,1506.3,1518.4,1512.4,1508.8,1545.4,1537.3,1551.8,1549.4,1536.9,1535.25,1537.95,1535.2,1556,1561.4,1525.6,1516.4,1507,1493.9,1504.9,1506.5,1513.1,1506.5,1509.7,1502,1506.8,1521.5,1529.8,1539.8,1510.9,1511.8,1501.7,1478,1485.4,1505.6,1511.6,1518.6,1498.7,1510.9,1510.8,1498.3,1492,1497.7,1484.8,1494.2,1495.6,1495.6,1487.5,1491.1,1495.1,1506.4]
                highs = [1284.75,1320.6,1327,1330.8,1326.8,1321.6,1326,1328,1325.8,1327.1,1326,1326,1323.5,1322.1,1282.7,1282.95,1315.8,1316.3,1314,1333.2,1334.7,1341.7,1353.2,1354.6,1352.2,1346.4,1345.7,1344.9,1340.7,1344.2,1342.7,1342.1,1345.2,1342,1350,1324.95,1330.75,1369.6,1374.3,1368.4,1359.8,1359,1357,1356,1353.4,1340.6,1322.3,1314.1,1316.1,1312.9,1325.7,1323.5,1326.3,1336,1332.1,1330.1,1330.4,1334.7,1341.1,1344.2,1338.8,1348.4,1345.6,1342.8,1334.7,1322.3,1319.3,1314.7,1316.6,1316.4,1315,1325.4,1328.3,1332.2,1329.2,1316.9,1312.3,1309.5,1299.6,1296.9,1277.9,1299.5,1296.2,1298.4,1302.5,1308.7,1306.4,1305.9,1307,1297.2,1301.7,1305,1305.3,1310.2,1307,1308,1319.8,1321.7,1318.7,1316.2,1305.9,1295.8,1293.8,1293.7,1304.2,1302,1285.15,1286.85,1304,1302,1305.2,1323,1344.1,1345.2,1360.1,1355.3,1363.8,1353,1344.7,1353.6,1358,1373.6,1358.2,1369.6,1377.6,1408.9,1425.5,1435.9,1453.7,1438,1426,1439.1,1418,1435,1452.6,1426.65,1437.5,1421.5,1414.1,1433.3,1441.3,1431.4,1433.9,1432.4,1440.8,1462.3,1467,1443.5,1444,1442.9,1447,1437.6,1440.8,1445.7,1447.8,1458.2,1461.9,1481.8,1486.8,1522.7,1521.3,1521.1,1531.5,1546.1,1534.9,1537.7,1538.6,1523.6,1518.8,1518.4,1514.6,1540.3,1565,1554.5,1556.6,1559.8,1541.9,1542.9,1540.05,1558.9,1566.2,1561.9,1536.2,1523.8,1509.1,1506.2,1532.2,1516.6,1519.7,1515,1519.5,1512.1,1524.5,1534.4,1543.3,1543.3,1542.8,1519.5,1507.2,1493.5,1511.4,1525.8,1522.2,1518.8,1515.3,1518,1522.3,1508,1501.5,1503,1495.5,1501.1,1497.9,1498.7,1492.1,1499.4,1506.9,1520.9]
                lows = [1282.85,1315,1318.7,1309.6,1317.6,1312.9,1312.4,1319.1,1319,1321,1318.1,1321.3,1319.9,1312,1280.5,1276.15,1308,1309.9,1308.5,1312.3,1329.3,1333.1,1340.2,1347,1345.9,1338,1340.8,1335,1332,1337.9,1333,1336.8,1333.2,1329.9,1340.4,1323.85,1324.05,1349,1366.3,1351.2,1349.1,1352.4,1350.7,1344.3,1338.9,1316.3,1308.4,1306.9,1309.6,1306.7,1312.3,1315.4,1319,1327.2,1317.2,1320,1323,1328,1323,1327.8,1331.7,1335.3,1336.6,1331.8,1311.4,1310,1309.5,1308,1310.6,1302.8,1306.6,1313.7,1320,1322.8,1311,1312.1,1303.6,1293.9,1293.5,1291,1277.9,1294.1,1286,1289.1,1293.5,1296.9,1298,1299.6,1292.9,1285.1,1288.5,1296.3,1297.2,1298.4,1298.6,1302,1300.3,1312,1310.8,1301.9,1292,1291.1,1286.3,1289.2,1289.9,1297.4,1283.65,1283.25,1292.9,1295.9,1290.8,1304.2,1322.7,1336.1,1341,1343.5,1345.8,1340.3,1335.1,1341.5,1347.6,1352.8,1348.2,1353.7,1356.5,1373.3,1398,1414.7,1427,1416.4,1412.7,1420.1,1396.4,1398.8,1426.6,1412.85,1400.7,1406,1399.8,1404.4,1415.5,1417.2,1421.9,1415,1413.7,1428.1,1434,1435.7,1427.5,1429.4,1423.9,1425.6,1427.5,1434.8,1422.3,1412.1,1442.5,1448.8,1468.2,1484.3,1501.6,1506.2,1498.6,1488.9,1504.5,1518.3,1513.9,1503.3,1503,1506.5,1502.1,1503,1534.8,1535.3,1541.4,1528.6,1525.6,1535.25,1528.15,1528,1542.6,1514.3,1510.7,1505.5,1492.1,1492.9,1496.8,1493.1,1503.4,1500.9,1490.7,1496.3,1505.3,1505.3,1517.9,1507.4,1507.1,1493.3,1470.5,1465,1480.5,1501.7,1501.4,1493.3,1492.1,1505.1,1495.7,1478,1487.1,1480.8,1480.6,1487,1488.3,1484.8,1484,1490.7,1490.4,1503.1]
                closes = [1283.35,1315.3,1326.1,1317.4,1321.5,1317.4,1323.5,1319.2,1321.3,1323.3,1319.7,1325.1,1323.6,1313.8,1282.05,1279.05,1314.2,1315.2,1310.8,1329.1,1334.5,1340.2,1340.5,1350,1347.1,1344.3,1344.6,1339.7,1339.4,1343.7,1337,1338.9,1340.1,1338.7,1346.8,1324.25,1329.55,1369.6,1372.5,1352.4,1357.6,1354.2,1353.4,1346,1341,1323.8,1311.9,1309.1,1312.2,1310.7,1324.3,1315.7,1322.4,1333.8,1319.4,1327.1,1325.8,1330.9,1325.8,1331.6,1336.5,1346.7,1339.2,1334.7,1313.3,1316.5,1312.4,1313.4,1313.3,1312.2,1313.7,1319.9,1326.3,1331.9,1311.3,1313.4,1309.4,1295.2,1294.7,1294.1,1277.9,1295.8,1291.2,1297.4,1297.7,1306.8,1299.4,1303.6,1302.2,1289.9,1299.2,1301.8,1303.6,1299.5,1303.2,1305.3,1319.5,1313.6,1315.1,1303.5,1293,1294.6,1290.4,1291.4,1302.7,1301,1284.15,1284.95,1294.3,1297.9,1304.1,1322.6,1339.3,1340.1,1344.9,1354,1357.4,1340.7,1342.7,1348.2,1355.1,1355.9,1354.2,1362.1,1360.1,1408.3,1411.2,1429.5,1430.1,1426.8,1423.4,1425.1,1400.8,1419.8,1432.9,1423.55,1412.1,1412.2,1412.8,1424.9,1419.3,1424.8,1426.1,1423.6,1435.9,1440.8,1439.4,1439.7,1434.5,1436.5,1427.5,1432.2,1433.3,1441.8,1437.8,1432.4,1457.5,1476.5,1484.2,1519.6,1509.5,1508.5,1517.2,1514.1,1527.8,1531.2,1523.6,1511.6,1515.7,1515.7,1508.5,1537.6,1537.2,1551.8,1549.1,1536.9,1529.4,1538.05,1535.15,1555.9,1560.4,1525.5,1515.5,1511.1,1499.2,1503.2,1507.4,1499.5,1511.5,1513.4,1515.8,1506.2,1515.1,1531.5,1540.2,1512.3,1515.2,1506.4,1472.9,1489,1507.9,1513.8,1512.9,1504.4,1503.9,1512.8,1500.9,1488.7,1497.6,1483.5,1494,1498.3,1494.1,1488.1,1487.5,1495.7,1504.7,1505.3]

                plot_id = dpg.add_plot(label="Candle Series", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="Day", time=True, parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="USD", parent=plot_id)
                dpg.add_candle_series(dates, opens, closes, lows, highs, label="GOOGL", parent=axis_id)

            with cxt.tree_node(label="Heatmaps"):

                plot_id = dpg.add_plot(label="Heat Series", no_mouse_pos=True, height=400)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", lock_min=True, lock_max=True, no_gridlines=True, no_tick_marks=True, parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", no_gridlines=True, no_tick_marks=True, lock_min=True, lock_max=True, parent=plot_id)

                values = [0.8, 2.4, 2.5, 3.9, 0.0, 4.0, 0.0,
                          2.4, 0.0, 4.0, 1.0, 2.7, 0.0, 0.0,
                          1.1, 2.4, 0.8, 4.3, 1.9, 4.4, 0.0,
                          0.6, 0.0, 0.3, 0.0, 3.1, 0.0, 0.0,
                          0.7, 1.7, 0.6, 2.6, 2.2, 6.2, 0.0,
                          1.3, 1.2, 0.0, 0.0, 0.0, 3.2, 5.1,
                          0.1, 2.0, 0.0, 1.4, 0.0, 1.9, 6.3]
                dpg.add_heat_series(values, 7, 7, scale_min=0, scale_max=6, parent=axis_id)

            with cxt.tree_node(label="Pie Charts"):

                plot_id1 = dpg.add_plot(no_title=True, no_mouse_pos=True, width=250, height=250)
                dpg.add_same_line()
                plot_id2 = dpg.add_plot(no_title=True, no_mouse_pos=True, width=250, height=250)

                xaxis_id1 = dpg.add_plot_axis(dpg.mvXAxis, label="", no_gridlines=True, no_tick_marks=True, no_tick_labels=True, parent=plot_id1)
                xaxis_id2 = dpg.add_plot_axis(dpg.mvXAxis, label="", no_gridlines=True, no_tick_marks=True, no_tick_labels=True, parent=plot_id2)
                yaxis_id1 = dpg.add_plot_axis(dpg.mvYAxis, label="", no_gridlines=True, no_tick_marks=True, no_tick_labels=True, parent=plot_id1)
                yaxis_id2 = dpg.add_plot_axis(dpg.mvYAxis, label="", no_gridlines=True, no_tick_marks=True, no_tick_labels=True, parent=plot_id2)

                dpg.set_axis_limits(xaxis_id1, 0, 1)
                dpg.set_axis_limits(xaxis_id2, 0, 1)
                dpg.set_axis_limits(yaxis_id1, 0, 1)
                dpg.set_axis_limits(yaxis_id2, 0, 1)

                dpg.add_pie_series(0.5, 0.5, 0.5, [0.25, 0.30, 0.30], ["fish", "cow", "chicken"], parent=yaxis_id1)
                dpg.add_pie_series(0.5, 0.5, 0.5, [1, 1, 2, 3, 5], ["A", "B", "C", "D", "E"], normalize=True, format="%.0f", parent=yaxis_id2)

            with cxt.tree_node(label="Error Series"):

                error1_x = [1, 2, 3, 4, 5]
                error1_y = [1, 2, 5, 3, 4]
                error1_neg = [0.2, 0.4, 0.2, 0.6, 0.4]
                error1_pos = [0.4, 0.2, 0.4, 0.8, 0.6]

                error2_x = [1, 2, 3, 4, 5]
                error2_y = [8, 8, 9, 7, 8]
                error2_neg = [0.2, 0.4, 0.2, 0.6, 0.4]
                error2_pos = [0.4, 0.2, 0.4, 0.8, 0.6]

                dpg.add_text("Anti-aliasing can be enabled from the plot's context menu (see Help).", bullet=True)
                plot_id = dpg.add_plot(label="Error Series", height=400)
                dpg.add_plot_legend(parent=plot_id)
                dpg.add_plot_axis(dpg.mvXAxis, label="x", parent=plot_id)
                axis_id = dpg.add_plot_axis(dpg.mvYAxis, label="y", parent=plot_id)

                dpg.add_bar_series(error1_x, error1_y, label="Bar", weight=0.25, parent=axis_id)
                dpg.add_error_series(error1_x, error1_y, error1_neg, error1_pos, label="Bar", parent=axis_id)
                dpg.add_line_series(error2_x, error2_y, label="Line", parent=axis_id)
                #dpg.add_error_series(error2_x, error2_y, error2_neg, error2_pos, label="Line", color=[0, 255, 0], parent=axis_id)
                dpg.add_error_series(error2_x, error2_y, error2_neg, error2_pos, label="Line", parent=axis_id)

        with cxt.collapsing_header(label="Node Editor"):

            dpg.add_text("Ctrl+Click to remove a link.", bullet=True)

            with cxt.node_editor(callback=lambda sender, app_data: dpg.add_node_link(app_data[0], app_data[1], parent=sender), 
                             delink_callback=lambda sender, app_data: dpg.delete_item(app_data)):

                with cxt.node(label="Node 1", pos=[10, 10]):

                    with cxt.node_attribute():
                        dpg.add_input_float(label="F1", width=150)

                    with cxt.node_attribute(output=True):
                        dpg.add_input_float(label="F2", width=150)

                with cxt.node(label="Node 2", pos=[300, 10]):

                    with cxt.node_attribute() as na2:
                        dpg.add_input_float(label="F3", width=200)

                    with cxt.node_attribute(output=True):
                        dpg.add_input_float(label="F4", width=200)

                with cxt.node(label="Node 3", pos=[25, 150]):                                  
                    with cxt.node_attribute():
                        dpg.add_input_text(label="T5", width=200)
                    with cxt.node_attribute(static=True): 
                        dpg.add_simple_plot(label="Node Plot", default_value=(0.3, 0.9, 2.5, 8.9), width=200, height=80, histogram=True)

        with cxt.collapsing_header(label= "Filtering"):

            dpg.add_text("Filter usage:\n"
                        "  \"\"               display all lines\n"
                        "  \"xxx\"         display lines containing \"xxx\"\n"
                        "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
                        "  \"-xxx\"        hide lines containing \"xxx\"")


            with cxt.filter_set() as filter_id:
                dpg.add_text("aaa1.c", filter_key="aaa1.c", bullet=True)
                dpg.add_text("bbb1.c", filter_key="bbb1.c", bullet=True)
                dpg.add_text("ccc1.c", filter_key="ccc1.c", bullet=True)
                dpg.add_text("aaa2.cpp", filter_key="aaa2.cpp", bullet=True)
                dpg.add_text("bbb2.cpp", filter_key="bbb2.cpp", bullet=True)
                dpg.add_text("ccc2.cpp", filter_key="ccc2.cpp", bullet=True)
                dpg.add_text("abc.h", filter_key="abc.h", bullet=True)
                dpg.add_text("hello, world", filter_key="hello, world", bullet=True)

            dpg.add_input_text(label="Filter (inc, -exc)", before=filter_id, user_data=filter_id, callback=lambda sender, app_data, user_data: dpg.set_value(user_data, dpg.get_value(sender)))

        with cxt.collapsing_header(label="Drawing API"):
            draw_groups={}
            layers={}

            def _switch_group(sender):
                for v in draw_groups.values():
                    dpg.configure_item(v, show=False)
                dpg.configure_item(draw_groups[dpg.get_value(sender)], show=True)

            def _draw(sender, app_data, user_data):
                args = []
                current_layer = layers[dpg.get_value(user_data[3])]
                kwargs = {'parent':current_layer}
                for item in dpg.get_item_info(user_data[1])["children"][1]:
                    args.append(dpg.get_value(item))
                for item in dpg.get_item_info(user_data[2])["children"][1]:
                    kwargs[dpg.get_item_configuration(item)["label"]] = dpg.get_value(item)

                # If a drawing command needs to take in a list of points dont unpack args with *
                points = []
                if user_data[4]:
                    user_data[0](args, **kwargs)
                else:
                    user_data[0](*args, **kwargs)

            with cxt.group(width=200) as inputs:
                layer = dpg.add_radio_button(default_value="Layer 1")
                dpg.add_button(label="Clear Layer", callback=lambda: dpg.delete_item(layers[dpg.get_value(layer)], children_only=True))
                drawables = dpg.add_listbox(label="Draw Item", default_value="Line", width=100, num_items=5, callback=_switch_group)

                with cxt.group(width=200) as line:
                    draw_groups["line"] = line
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="p1", size=2, default_value=(10, 10), max_value=800)
                        dpg.add_slider_intx(label="p2", size=2, default_value=(100, 100), max_value=800)
                    with cxt.group() as k:
                        dpg.add_input_int(label="thickness", default_value=1)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_line, r, k, layer, False])

                with cxt.group(width=200, show=False) as circle:
                    draw_groups["circle"] = circle
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="center", size=2, default_value=(100, 100), max_value=800)
                        dpg.add_slider_int(label="radius", default_value=20, max_value=100)
                    with cxt.group() as k:
                        dpg.add_slider_int(label="thickness", default_value=1)
                        dpg.add_slider_int(label="segments", default_value=0)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                        dpg.add_color_picker((0, 0, 0, 0), label="fill", alpha_bar=True)
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_circle, r, k, layer, False])

                with cxt.group(width=200, show=False) as ellipse:
                    draw_groups["ellipse"] = ellipse
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="pmin", size=2, default_value=(50, 50), max_value=800)
                        dpg.add_slider_intx(label="pmax", size=2, default_value=(60, 70), max_value=800)
                    with cxt.group() as k:
                        dpg.add_slider_int(label="thickness", default_value=1)
                        dpg.add_slider_int(label="segments", default_value=32)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                        dpg.add_color_picker((0, 0, 0, 0), label="fill", alpha_bar=True)
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_ellipse, r, k, layer, False])

                with cxt.group(width=200, show=False) as arrow:
                    draw_groups["arrow"] = arrow
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="p1", size=2, default_value=(10, 10), max_value=800)
                        dpg.add_slider_intx(label="p2", size=2, default_value=(100, 100), max_value=800)
                    with cxt.group() as k:
                        dpg.add_input_int(label="thickness", default_value=1)
                        dpg.add_input_int(label="size", default_value=15)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_arrow, r, k, layer, False])

                with cxt.group(width=200, show=False) as bezier_cubic:
                    draw_groups["bezier cubic"] = bezier_cubic
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="p1", size=2, default_value=(10, 10), max_value=800)
                        dpg.add_slider_intx(label="p2", size=2, default_value=(50, 10), max_value=800)
                        dpg.add_slider_intx(label="p3", size=2, default_value=(10, 40), max_value=800)
                        dpg.add_slider_intx(label="p4", size=2, default_value=(100, 50), max_value=800)
                    with cxt.group() as k:
                        dpg.add_input_int(label="thickness", default_value=1)
                        dpg.add_slider_int(label="segments", default_value=0)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_bezier_cubic, r, k, layer, False])

                with cxt.group(width=200, show=False) as bezier_quadratic:
                    draw_groups["bezier quadratic"] = bezier_quadratic
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="p1", size=2, default_value=(10, 10), max_value=800)
                        dpg.add_slider_intx(label="p2", size=2, default_value=(50, 30), max_value=800)
                        dpg.add_slider_intx(label="p3", size=2, default_value=(10, 40), max_value=800)
                    with cxt.group() as k:
                        dpg.add_input_int(label="thickness", default_value=1)
                        dpg.add_slider_int(label="segments", default_value=0)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_bezier_quadratic, r, k, layer, False])

                with cxt.group(width=200, show=False) as polygon:
                    draw_groups["polygon"] = polygon
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="point 1", size=2, default_value=(10, 10), max_value=800)
                        dpg.add_slider_intx(label="point 2", size=2, default_value=(50, 10), max_value=800)
                        dpg.add_slider_intx(label="point 3", size=2, default_value=(50, 40), max_value=800)
                        dpg.add_slider_intx(label="point 4", size=2, default_value=(30, 60), max_value=800)
                        dpg.add_slider_intx(label="point 5", size=2, default_value=(10, 10), max_value=800)
                    with cxt.group() as k:
                        dpg.add_input_int(label="thickness", default_value=1)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                        dpg.add_color_picker((0, 0, 0, 0), label="fill", alpha_bar=True)
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_polygon, r, k, layer, True])

                with cxt.group(width=200, show=False) as polyline:
                    draw_groups["polyline"] = polyline
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="point 1", size=2, default_value=(10, 10), max_value=800)
                        dpg.add_slider_intx(label="point 2", size=2, default_value=(40, 40), max_value=800)
                        dpg.add_slider_intx(label="point 3", size=2, default_value=(20, 10), max_value=800)
                        dpg.add_slider_intx(label="point 4", size=2, default_value=(30, 60), max_value=800)
                        dpg.add_slider_intx(label="point 5", size=2, default_value=(120, 120), max_value=800)
                    with cxt.group() as k:
                        dpg.add_checkbox(label="closed")
                        dpg.add_input_int(label="thickness", default_value=1)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_polyline, r, k, layer, True])

                with cxt.group(width=200, show=False) as quad:
                    draw_groups["quad"] = quad
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="p1", size=2, default_value=(20, 20), max_value=800)
                        dpg.add_slider_intx(label="p2", size=2, default_value=(70, 20), max_value=800)
                        dpg.add_slider_intx(label="p3", size=2, default_value=(60, 60), max_value=800)
                        dpg.add_slider_intx(label="p4", size=2, default_value=(10, 60), max_value=800)
                    with cxt.group() as k:
                        dpg.add_input_int(label="thickness", default_value=1)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                        dpg.add_color_picker((0, 0, 0, 0), label="fill", alpha_bar=True)
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_quad, r, k, layer, False])

                with cxt.group(width=200, show=False) as rectangle:
                    draw_groups["rectangle"] = rectangle
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="pmin", size=2, default_value=(50, 50), max_value=800)
                        dpg.add_slider_intx(label="pmax", size=2, default_value=(60, 70), max_value=800)
                    with cxt.group() as k:
                        dpg.add_slider_int(label="thickness", default_value=1)
                        dpg.add_slider_int(label="rounding", default_value=0)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                        dpg.add_color_picker((0, 0, 0, 0), label="fill", alpha_bar=True)
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_rectangle, r, k, layer, False])

                with cxt.group(width=200, show=False) as triangle:
                    draw_groups["triangle"] = triangle
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="p1", size=2, default_value=(70, 50), max_value=800)
                        dpg.add_slider_intx(label="p2", size=2, default_value=(80, 70), max_value=800)
                        dpg.add_slider_intx(label="p3", size=2, default_value=(60, 60), max_value=800)
                    with cxt.group() as k:
                        dpg.add_slider_int(label="thickness", default_value=1)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                        dpg.add_color_picker((0, 0, 0, 0), label="fill", alpha_bar=True)
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_triangle, r, k, layer, False])

                with cxt.group(width=200, show=False) as text:
                    draw_groups["text"] = text
                    with cxt.group() as r:
                        dpg.add_slider_intx(label="pos", size=2, default_value=(70, 50), max_value=800)
                        dpg.add_input_text(label="text", default_value="Hello World")
                    with cxt.group() as k:
                        dpg.add_slider_int(label="size", default_value=10)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_text, r, k, layer, False])

                with cxt.group(width=200, show=False) as image:
                    draw_groups["image"] = image
                    with cxt.group() as r:
                        dpg.add_input_text(label="file", default_value="INTERNAL_DPG_FONT_ATLAS")
                        dpg.add_slider_intx(label="pmin", size=2, default_value=(50, 50), max_value=800)
                        dpg.add_slider_intx(label="pmax", size=2, default_value=(300, 300), max_value=800)
                    with cxt.group() as k:
                        dpg.add_slider_floatx(label="uv_min", size=2, default_value=(0.0, 0.0), max_value=2.0, min_value=-1.0)
                        dpg.add_slider_floatx(label="uv_max", size=2, default_value=(1.0, 1.0), max_value=2.0, min_value=-1.0)
                        dpg.add_color_picker((255, 255, 255, 255), label="color")
                    dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_image, r, k, layer, False])

                dpg.configure_item(drawables, items=list(draw_groups.keys()))
            dpg.add_same_line()

            with cxt.drawlist(id="drawlist_demo", width=800, height=500):
                dpg.draw_rectangle((0, 0), (800, 500), color=(100, 100, 100, 250), thickness=2)
                layers["Layer 1"] = dpg.add_draw_layer()
                layers["Layer 2"] = dpg.add_draw_layer()
                layers["Layer 3"] = dpg.add_draw_layer()

                dpg.configure_item(layer, items=list(layers.keys()))

        with cxt.collapsing_header(label="Layouts"):
            with cxt.tree_node(label="Ordered pack style"):
                dpg.add_button(label="Button 1")
                dpg.add_button(label="Button 2")
                dpg.add_button(label="Button 3")

            with cxt.tree_node(label="Out of order pack style"):
                with cxt.child(id="out of order pack parent", width=100, height=50):
                    pass
                dpg.add_button(id="out of order button 1", label="Button 1")
                dpg.add_button(label="Button 2")
                dpg.add_button(label="Button 3", before="out of order button 1")
                dpg.add_button(label="Button 4", parent="out of order pack parent")

            with cxt.tree_node(id="Absolute Position Placment"):
                dpg.add_button(label="Set Button 2 Pos", callback=lambda: dpg.set_item_pos(B2, x=50, y=125))
                dpg.add_button(label="Reset Button 2 Pos", callback=lambda: dpg.reset_pos(B2))
                dpg.add_button(label="Button 1", pos=[50,50], width=75, height=75)
                B2 = dpg.add_button(label="Button 2", width=75, height=75)
                dpg.add_button(label="Button 3")
        
            with cxt.tree_node(id="Grid Layout using Table API"):
                dpg.add_text("Tables can be used to layout items in an equally spaced grid pattern.")
                dpg.add_text("See tables section for more detail on tables.")
                dpg.add_checkbox(label="resizable", callback=_config, user_data="layout_demo_table")
                dpg.add_checkbox(label="borders_innerH", callback=_config, user_data="layout_demo_table", default_value=True)
                dpg.add_checkbox(label="borders_outerH", callback=_config, user_data="layout_demo_table", default_value=True)
                dpg.add_checkbox(label="borders_innerV", callback=_config, user_data="layout_demo_table", default_value=True)
                dpg.add_checkbox(label="borders_outerV", callback=_config, user_data="layout_demo_table", default_value=True)
                with cxt.table(id="layout_demo_table", header_row=False, borders_innerH=True, borders_outerH=True, borders_innerV=True, borders_outerV=True):
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_table_column()
                    dpg.add_button(label="Button 1")
                    dpg.add_table_next_column()
                    dpg.add_button(label="Button 2")
                    dpg.add_table_next_column()
                    dpg.add_button(label="Button 3")
                    dpg.add_table_next_column()
                    dpg.add_table_next_column()
                    dpg.add_table_next_column()
                    dpg.add_button(label="Button 4")
                    dpg.add_table_next_column()
                    dpg.add_button(label="Button 5")

            with cxt.tree_node(label="Containers"):
                with cxt.tree_node(label="Tree Nodes"):
                    with cxt.tree_node(label="Tree Node (selectable)", selectable=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with cxt.tree_node(label="Tree Node (bullet)", bullet=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                with cxt.tree_node(label="Groups"):
                    dpg.add_text("Groups are used to control child items placement, width, and provide a hit box for things like is the set of items are hovered, ect...")
                    with cxt.group(id="group 1", horizontal=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with cxt.group(width=150):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with cxt.group():
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                with cxt.tree_node(label="Child windows"):
                    dpg.add_text("Child windows are basically embedded windows and provide much more structure and control of the containing items than groups.")
                    with cxt.group(horizontal=True):
                        dpg.add_checkbox(label="autosize_x", callback=_config, user_data="demo_layout_child")
                        dpg.add_checkbox(label="autosize_y", callback=_config, user_data="demo_layout_child")
                        dpg.add_checkbox(label="menubar", callback=_config, user_data="demo_layout_child")
                        dpg.add_checkbox(label="no_scrollbar", callback=_config, user_data="demo_layout_child")
                        dpg.add_checkbox(label="horizontal_scrollbar", callback=_config, user_data="demo_layout_child")
                        dpg.add_checkbox(label="border", default_value=True, callback=_config, user_data="demo_layout_child")
                    with cxt.child(id="demo_layout_child", width=200, height=200):
                        with cxt.menu_bar():
                            with cxt.menu(label="Menu"):
                                pass
                        for i in range(0, 20):
                            dpg.add_text(default_value="A pretty long sentence if you really think about it. It's also pointless. we need this to be even longer")
                    with cxt.child(autosize_x=True, height=130, menubar=True):
                        with cxt.menu_bar():
                            dpg.add_menu(label="Menu Options")
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with cxt.child(width=100, height=150, horizontal_scrollbar=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                        dpg.add_button(label="Button 4", width=150)
                        dpg.add_button(label="Button 5")
                        dpg.add_button(label="Button 6")
                    dpg.add_same_line()
                    with cxt.child(width=100, height=110):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                with cxt.tree_node(label="Collapsing Headers"):
                    with cxt.collapsing_header(label="Collapsing Header"):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with cxt.collapsing_header(label="Collapsing Header (close)", closable=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with cxt.collapsing_header(label="Collapsing Header (bullet)", bullet=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                    with cxt.collapsing_header(label="Collapsing Header (leaf)", leaf=True):
                        dpg.add_button(label="Button 1")
                        dpg.add_button(label="Button 2")
                        dpg.add_button(label="Button 3")
                with cxt.tree_node(label="Tab Bar"):
                    with cxt.tab_bar(reorderable=True)as tab_bar:
                        with cxt.tab(label="Tab 1"):
                            dpg.add_button(label="Tab 1 Button 1")
                            dpg.add_button(label="Tab 1 Button 2")
                            dpg.add_button(label="Tab 1 Button 3")
                        with cxt.tab(label="Tab 2"):
                            dpg.add_button(label="Tab 2 Button 1")
                            dpg.add_button(label="Tab 2 Button 2")
                            dpg.add_button(label="Tab 2 Button 3")
                        dpg.add_tab_button(label="Tab Button", callback=lambda: dpg.add_tab(parent=tab_bar))

            with cxt.tree_node(label="Simple Layouts"):
                dpg.add_text("Containers can be nested for advanced layout options")
                with cxt.child(width=500, height=320):
                    with cxt.menu_bar():
                        dpg.add_menu(label="Menu Options")
                    with cxt.child(autosize_x=True, height=95):
                        with cxt.group(horizontal=True):
                            dpg.add_button(label="Header 1", width=75, height=75)
                            dpg.add_button(label="Header 2", width=75, height=75)
                            dpg.add_button(label="Header 3", width=75, height=75)
                    with cxt.child(autosize_x=True, height=175):
                        with cxt.group(horizontal=True, width=0):
                            with cxt.child(width=102, height=150):
                                with cxt.tree_node(label="Nav 1"):
                                    dpg.add_button(label="Button 1")
                                with cxt.tree_node(label="Nav 2"):
                                    dpg.add_button(label="Button 2")
                                with cxt.tree_node(label="Nav 3"):
                                    dpg.add_button(label="Button 3")
                            with cxt.child(width=300, height=150):
                                dpg.add_button(label="Button 1")
                                dpg.add_button(label="Button 2")
                                dpg.add_button(label="Button 3")
                            with cxt.child(width=50, height=150):
                                dpg.add_button(label="B1", width=25, height=25)
                                dpg.add_button(label="B2", width=25, height=25)
                                dpg.add_button(label="B3", width=25, height=25)
                    with cxt.group(horizontal=True):
                        dpg.add_button(label="Footer 1", width=175)
                        dpg.add_text("Footer 2")
                        dpg.add_button(label="Footer 3", width=175)

        with cxt.collapsing_header(label="Handler"):
            # key handlers are assigned at top of file. handlers must be defined outside of containers
            dpg.add_text("This will activate logging of all handlers.")
            dpg.add_checkbox(label="show", default_value=False, callback=_config, user_data=[keyboard_handler, mouse_handler])
            with cxt.tree_node(label="Keyboard Handlers"):
                dpg.add_checkbox(label="show", default_value=False, callback=_config, user_data=keyboard_handler)
                dpg.add_text("Key Down Handler")
                dpg.add_button(label="Set Key to all", callback=lambda sender: dpg.configure_item(key_down_handler, key=-1))
                dpg.add_button(label='Set Key to "A"', callback=lambda sender: dpg.configure_item(key_down_handler, key=dpg.mvKey_A))
                dpg.add_text("Key Release Handler")
                dpg.add_button(label="Set Key to all", callback=lambda sender: dpg.configure_item(key_release_handler, key=-1))
                dpg.add_button(label='Set Key to "A"', callback=lambda sender: dpg.configure_item(key_release_handler, key=dpg.mvKey_A))
            with cxt.tree_node(label="Mouse Handlers"):

                #TODO: do we have ability to set multiple keys or CTRL+SHIFT+T ect...like three key triggers as previously acomplished with nested is_down functions?
                #if so does it scale up to 6 key combinations?
                #dpg.add_button(label='Set Key to "A or B"', callback=lambda sender: dpg.configure_item(key_down_handler, key=(dpg.mvKey_B, dpg.mvKey_A)))

