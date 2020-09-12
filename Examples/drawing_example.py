from dearpygui.dearpygui import *

def update_drawing(sender, data):
    set_drawing_origin("drawing##widget", get_value("X Origin"), get_value("Y Origin"))
    set_drawing_scale("drawing##widget", get_value("X Scale "), get_value("Y Scale"))

add_group("Drawing Controls Group")
add_slider_float("X Origin", vertical=True, min_value=-100, max_value=100, default_value=0, callback=update_drawing)
add_same_line(spacing=20)
add_slider_float("Y Origin", vertical=True, min_value=-100, max_value=100, default_value=0, callback=update_drawing)
add_slider_float("X Scale ", vertical=True, max_value=10, default_value=1, callback=update_drawing)
add_same_line(spacing=20)
add_slider_float("Y Scale", vertical=True, max_value=10, default_value=1, callback=update_drawing)
end()
add_same_line(spacing=20)
add_drawing("drawing##widget", width=800, height=500)
draw_rectangle("drawing##widget", [0, 500], [800, 0], [255, 0, 0, 255], fill=[0, 0, 25, 255], rounding=12,
               thickness=1.0)
draw_line("drawing##widget", [10, 10], [100, 100], [255, 0, 0, 255], 1)
draw_triangle("drawing##widget", [300, 500], [200, 200], [500, 200], [255, 255, 0, 255], thickness=3.0)
draw_quad("drawing##widget", [50, 50], [150, 50], [150, 150], [50, 150], [255, 255, 0, 255], thickness=3.0)
draw_text("drawing##widget", [50, 300], "Some Text", color=[255, 255, 0, 255], size=15)
draw_text("drawing##widget", [0, 0], "Origin", color=[255, 255, 0, 255], size=15)
draw_circle("drawing##widget", [400, 250], 50, [255, 255, 0, 255])
draw_polyline("drawing##widget", [[300, 500], [200, 200], [500, 700]], [255, 255, 0, 255])
draw_polygon("drawing##widget", [[363, 471], [100, 498], [50, 220]], [255, 125, 0, 255])
draw_bezier_curve("drawing##widget", [50, 200], [150, 250], [300, 150], [600, 250], [255, 255, 0, 255], thickness=2.0)
draw_arrow("drawing##widget", [50, 70], [100, 65], [0, 200, 255], 1, 10)

start_dearpygui()
