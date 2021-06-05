import dearpygui.core as dpg
import dearpygui.contexts as cxt


current_layer = ""
layer_1 = ""
layer_2 = ""
layer_3 = ""

line_group = ""
circle_group = ""

def _set_layer(layer):

    global current_layer 

    if layer == "Layer 1":
        current_layer = layer_1
    elif layer == "Layer 2":
        current_layer = layer_2
    elif layer == "Layer 3":
        current_layer = layer_3
    else:
        current_layer = layer_1

def _switch_group(sender):
    
    data = dpg.get_value(sender)
    if data == "Line":
        dpg.configure_item(line_group, show=True)
        dpg.configure_item(circle_group, show=False)
        dpg.configure_item(ellipse_group, show=False)

    elif data == "Circle":
        dpg.configure_item(line_group, show=False)
        dpg.configure_item(circle_group, show=True)
        dpg.configure_item(ellipse_group, show=False)

    elif data == "Ellipse":
        dpg.configure_item(line_group, show=False)
        dpg.configure_item(circle_group, show=False)
        dpg.configure_item(ellipse_group, show=True)


def show_demo():

    with cxt.collapsing_header(label="Drawlists"):

        with cxt.group():

            dpg.add_radio_button(("Layer 1", "Layer 2", "Layer 3"), default_value="Layer 1", callback=lambda sender: _set_layer(dpg.get_value(sender)))
            dpg.add_listbox(("Line", "Circle", "Ellipse"), label="Draw Item", default_value="Line", width=100,
                            callback=_switch_group)

            with cxt.group(width=200) as g:
                global line_group
                line_group = g
                p1_input = dpg.add_input_intx(label="p1", size=2, default_value=(10, 10))
                p2_input = dpg.add_input_intx(label="p2", size=2, default_value=(100, 100))
                thickness_input = dpg.add_input_int(label="thickness", default_value=1)
                color_input = dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                dpg.add_button(label="Add", callback=lambda:dpg.draw_line(dpg.get_value(p1_input), dpg.get_value(p2_input), thickness=
                                                                          dpg.get_value(thickness_input), color=dpg.get_value(color_input), parent=current_layer))

            with cxt.group(show=False, width=200) as g:
                global circle_group
                circle_group = g
                center_input = dpg.add_input_intx(label="center", size=2, default_value=(100, 100))
                radius_input = dpg.add_input_int(label="radius", default_value=20)
                thickness_input = dpg.add_input_int(label="thickness", default_value=1)
                seg_input = dpg.add_input_int(label="segments", default_value=0)
                color_input = dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                fill_input = dpg.add_color_picker(label="fill", default_value=(0, 0, 0, 0), alpha_bar=True)
                dpg.add_button(label="Add", callback=lambda:dpg.draw_circle(dpg.get_value(center_input), dpg.get_value(radius_input), thickness=
                                                                          dpg.get_value(thickness_input), segments = dpg.get_value(seg_input),
                                                                          color=dpg.get_value(color_input), fill=dpg.get_value(fill_input),
                                                                          parent=current_layer))

            with cxt.group(show=False, width=200) as g:
                global ellipse_group
                ellipse_group = g
                pmin_input = dpg.add_input_intx(label="pmin", size=2, default_value=(50, 50))
                pmax_input = dpg.add_input_intx(label="pmax", size=2, default_value=(60, 70))
                width_input = dpg.add_input_int(label="width", default_value=20)
                thickness_input = dpg.add_input_int(label="thickness", default_value=1)
                seg_input = dpg.add_input_int(label="segments", default_value=0)
                color_input = dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                fill_input = dpg.add_color_picker(label="fill", default_value=(0, 0, 0, 0), alpha_bar=True)
                dpg.add_button(label="Add", callback=lambda:dpg.draw_ellipse(dpg.get_value(pmin_input), dpg.get_value(pmax_input),
                                                                          thickness=dpg.get_value(thickness_input), segments = dpg.get_value(seg_input),
                                                                          color=dpg.get_value(color_input), fill=dpg.get_value(fill_input),
                                                                          parent=current_layer))

        dpg.add_same_line()

        with cxt.drawlist(id="drawlist_demo", width=800, height=500):
            global current_layer, layer_1, layer_2, layer_3
            dpg.draw_rectangle((0, 0), (800, 500), color=(100, 100, 100, 250), thickness=2)
            layer_1 = dpg.add_draw_layer()
            layer_2 = dpg.add_draw_layer()
            layer_3 = dpg.add_draw_layer()
            current_layer = layer_1
