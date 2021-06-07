import dearpygui.core as dpg
import dearpygui.contexts as cxt
import dearpygui.demo_drawlist as drawlist
import dearpygui.demo_widgets as widgets
import dearpygui.demo_layouts as layouts
from dearpygui.common import _log, _config, _help, _hsv_to_rgb

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
                yaxis_id = dpg.add_plot_yaxis(label="y axis", parent=plot_id)
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

        with cxt.collapsing_header(label="Node Editor"):
            pass

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


