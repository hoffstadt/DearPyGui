import dearpygui.core as dpg
import dearpygui.contexts as cxt

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

def show_demo():

    with cxt.collapsing_header(label="Drawlists"):
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
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_line, r, k, layer, False])

            with cxt.group(width=200, show=False) as circle:
                draw_groups["circle"] = circle
                with cxt.group() as r:
                    dpg.add_slider_intx(label="center", size=2, default_value=(100, 100), max_value=800)
                    dpg.add_slider_int(label="radius", default_value=20, max_value=100)
                with cxt.group() as k:
                    dpg.add_slider_int(label="thickness", default_value=1)
                    dpg.add_slider_int(label="segments", default_value=0)
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                    dpg.add_color_picker(label="fill", default_value=(0, 0, 0, 0), alpha_bar=True)
                dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_circle, r, k, layer, False])

            with cxt.group(width=200, show=False) as ellipse:
                draw_groups["ellipse"] = ellipse
                with cxt.group() as r:
                    dpg.add_slider_intx(label="pmin", size=2, default_value=(50, 50), max_value=800)
                    dpg.add_slider_intx(label="pmax", size=2, default_value=(60, 70), max_value=800)
                with cxt.group() as k:
                    dpg.add_slider_int(label="thickness", default_value=1)
                    dpg.add_slider_int(label="segments", default_value=32)
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                    dpg.add_color_picker(label="fill", default_value=(0, 0, 0, 0), alpha_bar=True)
                dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_ellipse, r, k, layer, False])

            with cxt.group(width=200, show=False) as arrow:
                draw_groups["arrow"] = arrow
                with cxt.group() as r:
                    dpg.add_slider_intx(label="p1", size=2, default_value=(10, 10), max_value=800)
                    dpg.add_slider_intx(label="p2", size=2, default_value=(100, 100), max_value=800)
                with cxt.group() as k:
                    dpg.add_input_int(label="thickness", default_value=1)
                    dpg.add_input_int(label="size", default_value=15)
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
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
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
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
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
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
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                    dpg.add_color_picker(label="fill", default_value=(0, 0, 0, 0), alpha_bar=True)
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
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
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
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                    dpg.add_color_picker(label="fill", default_value=(0, 0, 0, 0), alpha_bar=True)
                dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_quad, r, k, layer, False])

            with cxt.group(width=200, show=False) as rectangle:
                draw_groups["rectangle"] = rectangle
                with cxt.group() as r:
                    dpg.add_slider_intx(label="pmin", size=2, default_value=(50, 50), max_value=800)
                    dpg.add_slider_intx(label="pmax", size=2, default_value=(60, 70), max_value=800)
                with cxt.group() as k:
                    dpg.add_slider_int(label="thickness", default_value=1)
                    dpg.add_slider_int(label="rounding", default_value=0)
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                    dpg.add_color_picker(label="fill", default_value=(0, 0, 0, 0), alpha_bar=True)
                dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_rectangle, r, k, layer, False])

            with cxt.group(width=200, show=False) as triangle:
                draw_groups["triangle"] = triangle
                with cxt.group() as r:
                    dpg.add_slider_intx(label="p1", size=2, default_value=(70, 50), max_value=800)
                    dpg.add_slider_intx(label="p2", size=2, default_value=(80, 70), max_value=800)
                    dpg.add_slider_intx(label="p3", size=2, default_value=(60, 60), max_value=800)
                with cxt.group() as k:
                    dpg.add_slider_int(label="thickness", default_value=1)
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                    dpg.add_color_picker(label="fill", default_value=(0, 0, 0, 0), alpha_bar=True)
                dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_triangle, r, k, layer, False])

            with cxt.group(width=200, show=False) as text:
                draw_groups["text"] = text
                with cxt.group() as r:
                    dpg.add_slider_intx(label="pos", size=2, default_value=(70, 50), max_value=800)
                    dpg.add_input_text(label="text", default_value="Hello World")
                with cxt.group() as k:
                    dpg.add_slider_int(label="size", default_value=10)
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
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
                    dpg.add_color_picker(label="color", default_value=(255, 255, 255, 255))
                dpg.add_button(label="Add", callback=_draw, user_data=[dpg.draw_image, r, k, layer, False])

            dpg.configure_item(drawables, items=list(draw_groups.keys()))
        dpg.add_same_line()

        with cxt.drawlist(id="drawlist_demo", width=800, height=500):
            dpg.draw_rectangle((0, 0), (800, 500), color=(100, 100, 100, 250), thickness=2)
            layers["Layer 1"] = dpg.add_draw_layer()
            layers["Layer 2"] = dpg.add_draw_layer()
            layers["Layer 3"] = dpg.add_draw_layer()

            dpg.configure_item(layer, items=list(layers.keys()))