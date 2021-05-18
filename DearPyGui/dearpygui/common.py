import dearpygui.core as dpg
import dearpygui.contexts as cxt
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