from dearpygui.dearpygui import *

# callbacks
def drag_callback(sender, data):

    pos = get_mouse_pos(local=True)
    height = get_item_height("Canvas Window")
    add_data("center", [pos[0], -pos[1]+height-40])


def render(sender, data):
    
    set_vsync(get_value("Use Vsync"))
    center = get_data("center")
    horizontal_direction = get_data("horizontal direction")
    vertical_direction = get_data("vertical direction")
    use_delta = get_value("Use Delta Time")
    hspeed = get_value("Horizontal Speed")
    vspeed = get_value("Vertical Speed")
    width = get_item_width("Canvas Window")
    height = get_item_height("Canvas Window")
    x = center[0]
    y = center[1]
    set_value("Center X", x/(width-20))
    set_value("Center Y", y/(height-40))
    set_drawing_size("Canvas", width-19, height-39)

    draw_rectangle("Canvas", [0, 1], [width-20, height-40], color=[0, 0, 255], fill=[0, 0, 255, 50], tag="background")

    # check if ball is crossing x boundary
    if x + 25 > width-20:
        horizontal_direction = "left"
    if x - 25 < 0:
        horizontal_direction = "right"

    # update ball horizontal position
    if horizontal_direction == "left":
        if use_delta:
            x = x - hspeed*get_delta_time()
        else:
            x -= hspeed
    else:
        if use_delta:
            x = x + hspeed*get_delta_time()
        else:
            x += hspeed

    # check if ball is crossing y boundary
    if y + 25 > height-40:
        vertical_direction = "down"
    if y - 25 < 1:
        vertical_direction = "up"

    # update ball vertical position
    if vertical_direction == "down":
        if use_delta:
            y = y - vspeed*get_delta_time()
        else:
            y -= vspeed
    else:
        if use_delta:
            y = y + vspeed*get_delta_time()
        else:
            y += vspeed

    draw_circle("Canvas", [x, y], 25, [255, 100, 0], fill=[255, 100, 0], segments=20, tag="ball")

    add_data("center", [x, y])
    add_data("horizontal direction", horizontal_direction)
    add_data("vertical direction", vertical_direction)

set_main_window_size(800, 800)
add_data("center", [25, 25])
add_data("horizontal direction", "right")
add_data("vertical direction", "up")

add_text("Tips")
add_text("The render callback is ran every frame.", bullet=True)
add_text("The handler keyword is to tell the render callback which frame needs to be active to run.", bullet=True)
add_text("Delta time is the time between frames.", bullet=True)
add_text("If delta time is checked, the position calculation uses real time (speed in pixel per second)", bullet=True)
add_text("If delta time is not checked, the position calculation is frame rate based(speed in pixel per frame)", bullet=True)
add_text("The threshold in the mouse drag callback is the distance a drag needs to be (in pixels) to register.", bullet=True)

add_spacing()
add_separator()
add_spacing()

add_slider_int("Horizontal Speed", default_value=1)
add_slider_int("Vertical Speed", default_value=1)
add_checkbox("Use Delta Time")
add_checkbox("Use Vsync", default_value=1)
add_progress_bar("Center X", 0, overlay="x position")
add_progress_bar("Center Y", 0, overlay="y position")

add_window("Canvas Window", width=320, height=340)
add_drawing("Canvas", width=301, height=301)
draw_circle("Canvas", [25, 25], 25, [255, 100, 0], fill=[255, 100, 0], segments=20, tag="ball")
draw_rectangle("Canvas", [0, 1], [300, 300], color=[0, 0, 255], fill=[0, 0, 255, 50], tag="background")
end()

set_render_callback(render)
set_render_callback(render, handler="Canvas Window")
set_mouse_drag_callback(drag_callback, 10, handler="Canvas Window")

start_dearpygui()
