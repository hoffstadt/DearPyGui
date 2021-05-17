import dearpygui.core as dpg
import dearpygui.contexts as cxt
import dearpygui.demo_tables as tables
import dearpygui.demo_plots as plots
import dearpygui.demo_drawlist as drawlist
import dearpygui.demo_nodes as nodes
import dearpygui.demo_widget as widgets
import dearpygui.demo_layouts as layouts
from math import sin, cos

def _help(message):
    """ Simple Helper """
    dpg.add_same_line()
    helper = dpg.add_text(color=[150, 150, 150], default_value="(?)")
    with cxt.tooltip(parent=helper):
        dpg.add_text(default_value=message)

def _log(sender, data):
    dpg.log_debug(f"Sender was {sender}.", logger="Demo Logger")
    dpg.log_debug(f"Data was {data}.", logger="Demo Logger")

def _config(sender, data):

    widget_type = dpg.get_item_info(sender)["type"]
    items = data
    value = dpg.get_value(sender)

    if widget_type == "mvAppItemType::mvCheckbox":
        keyword = dpg.get_item_configuration(sender)["label"]

    elif widget_type == "mvAppItemType::mvRadioButton":
        keyword = dpg.get_item_configuration(sender)["items"][value]

    if isinstance(data, list):
        for item in items:
            dpg.configure_item(item, **{keyword: value})
    else:
        dpg.configure_item(items, **{keyword: value})

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

    with cxt.collapsing_header(label="Popups & Modal Windows"):
        pass

    with cxt.collapsing_header(label="Tooltips"):
        pass

    tables.show_demo()
    drawlist.show_demo()
    plots.show_demo()
    nodes.show_demo()
