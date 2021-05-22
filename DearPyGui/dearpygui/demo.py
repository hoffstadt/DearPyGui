import dearpygui.core as dpg
import dearpygui.contexts as cxt
import dearpygui.demo_tables as tables
import dearpygui.demo_plots as plots
import dearpygui.demo_drawlist as drawlist
import dearpygui.demo_nodes as nodes
import dearpygui.demo_widgets as widgets
import dearpygui.demo_layouts as layouts
import dearpygui.demo_textures as textures
from dearpygui.common import _log, _config, _help, _hsv_to_rgb

def show_demo():
    dpg.configure_item("Dear PyGui Demo", show=True)

with cxt.window(label="Dear PyGui Demo Logger", width=500, height=500):
    dpg.add_logger(id="Demo Logger", log_level=0, autosize_x=True, autosize_y=True)

with cxt.window(id="Dear PyGui Demo", width=800, height=800, show=False) as demo_id:
    
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

    dpg.add_text(default_value=f'Dear PyGui says hello. ({dpg.get_dearpygui_version()})')
    dpg.add_text(default_value="This code for this demo can be found here: ")
    dpg.add_text(default_value="https://github.com/hoffstadt/DearPyGui/blob/master/DearPyGui/dearpygui/demo.py")

    with cxt.collapsing_header(label="Window Options"):

        with cxt.table(header_row=False):

            dpg.add_table_column()
            dpg.add_table_column()
            dpg.add_table_column()

            dpg.add_checkbox(label="no_title_bar", callback=_config, callback_data=demo_id)
            dpg.add_checkbox(label="no_scrollbar", callback=_config, callback_data=demo_id)
            dpg.add_checkbox(label="menubar", default_value=True, callback=_config, callback_data=demo_id)

            dpg.add_table_next_column()
            dpg.add_checkbox(label="no_move", callback=_config, callback_data=demo_id)
            dpg.add_checkbox(label="no_resize", callback=_config, callback_data=demo_id)
            dpg.add_checkbox(label="no_collapse", callback=_config, callback_data=demo_id)

            dpg.add_table_next_column()
            dpg.add_checkbox(label="no_close", callback=_config, callback_data=demo_id)
            dpg.add_checkbox(label="no_background", callback=_config, callback_data=demo_id)
            dpg.add_checkbox(label="no_bring_to_front_on_focus", callback=_config, callback_data=demo_id)

    widgets.show_demo()

    layouts.show_demo()

    textures.show_demo()

    with cxt.collapsing_header(label="Popups & Modal Windows"):
        pass

    with cxt.collapsing_header(label="Tooltips"):
        pass

    tables.show_demo()
    drawlist.show_demo()
    plots.show_demo()
    nodes.show_demo()
