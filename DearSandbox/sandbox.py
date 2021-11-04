import dearpygui.dearpygui as dpg
#from dearpygui.demo import show_demo
import math

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

transform1 = dpg.create_translation_transform([100, 100, 0])*dpg.create_rotation_transform(math.pi*45.0/180.0, [0, 0, -1])
transform2 = dpg.create_translation_transform([100, 100, 0])*dpg.create_rotation_transform(-math.pi*45.0/180.0, [0, 0, -1])
transform3 = dpg.create_translation_transform([100, 100, 0]) * dpg.create_rotation_transform(math.pi*45.0/180.0, [0, 0, -1])

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, tag="custom font"):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())


#show_demo()

with dpg.window(label="tutorial", width=500, height=500):

    dpg.draw_quad((0, 0), (100, 0), (100, 100), (0, 100), color=(255, 0, 0), thickness=1)
    with dpg.draw_layer(tag="node1"):
        dpg.draw_quad((0, 0), (100, 0), (100, 100), (0, 100), color=(0, 255, 0), thickness=1)
        with dpg.draw_layer(tag="node2"):
            dpg.draw_quad((0, 0), (100, 0), (100, 100), (0, 100), color=(0, 0, 255), thickness=5)
            with dpg.draw_layer(tag="node3"):
                dpg.draw_quad((0, 0), (50, 0), (50, 50), (0, 50), color=(255, 0, 255), thickness=5)
                dpg.draw_arrow((0, 0), (200, 200))

size = 5
with dpg.window(label="Model", width=500, height=500):

    with dpg.draw_layer(tag="3dnode1"):
        dpg.draw_quad((-size, -size, -size), (size, -size, -size), (size, size, -size), (-size, size, -size), color=(0, 255, 0), thickness=1)

with dpg.window(label="Model*Camera", width=500, height=500):

    with dpg.draw_layer(tag="3dnode2"):
        dpg.draw_quad((-size, -size, -size), (size, -size, -size), (size, size, -size), (-size, size, -size), color=(0, 255, 0), thickness=1)

with dpg.window(label="Model*Camera*Projection", width=500, height=500):

    with dpg.draw_layer(tag="3dnode3"):
        dpg.draw_quad((-size, -size, -size), (size, -size, -size), (size, size, -size), (-size, size, -size), color=(0, 255, 0), thickness=1)

with dpg.window(label="Model*Camera*Projection*Viewport", width=500, height=500):

    with dpg.draw_layer(tag="3dnode4"):
        dpg.draw_quad((-size, -size, -size), (size, -size, -size), (size, size, -size), (-size, size, -size), color=(0, 255, 0), thickness=1)

dpg.apply_transform("node1", transform1)
dpg.apply_transform("node2", transform2)
dpg.apply_transform("node3", transform3)

model = dpg.create_translation_transform([0, 0, 0])
camera = dpg.create_lookat_transform([0, 0, 20], [0, 0, 19], [0, -1, 0])
projection = dpg.create_perspective_transform(math.pi*45.0/180.0, 1.0, 0.01, 400)

#vl = 0.0
#vb = -500.0

#vr = 500.0
#vt = 0.0
#viewport = dpg.mvMat4(
#    (vr-vl)/2.0, 0, 0, (vr+vl)/2.0,
#    0, (vt-vb)/2.0, 0, (vt+vb)/2.0,
#    0, 0, 0.5, 0.5,
#    0, 0, 0, 1.0
#    )

dpg.apply_transform("3dnode1", model)
dpg.apply_transform("3dnode2", camera*model)
dpg.apply_transform("3dnode3", projection*camera*model)
dpg.apply_transform("3dnode4",  dpg.create_scale_transform([0.5, 0.5, 0.5])*dpg.create_translation_transform([-1, -1, 0.5])*projection*camera*model)

x = 0
y = 0
z = 0

def callback(sender, app, user):
    global x
    global y
    global z
    if app[0] == dpg.mvKey_A:
        x-=1
    if app[0] == dpg.mvKey_D:
        x+=1
    if app[0] == dpg.mvKey_R:
        y+=1
    if app[0] == dpg.mvKey_F:
        y-=1
    if app[0] == dpg.mvKey_W:
        z+=1
    if app[0] == dpg.mvKey_S:
        z-=1
    dpg.apply_transform("3dnode3", projection*dpg.create_lookat_transform([x, y, z], [x, y, z-1], [0, 1, 0])*model)
    dpg.apply_transform("3dnode4",  dpg.create_scale_transform([250, 250, 0.5])*dpg.create_translation_transform([-1, -1, 0.5])*projection*dpg.create_lookat_transform([x, y, z], [x, y, z-1], [0, 1, 0])*model)

with dpg.handler_registry(tag="__demo_keyboard_handler"):
    k_down = dpg.add_key_down_handler(key=dpg.mvKey_A, callback=callback)
    k_down = dpg.add_key_down_handler(key=dpg.mvKey_D, callback=callback)
    k_down = dpg.add_key_down_handler(key=dpg.mvKey_R, callback=callback)
    k_down = dpg.add_key_down_handler(key=dpg.mvKey_F, callback=callback)
    k_down = dpg.add_key_down_handler(key=dpg.mvKey_W, callback=callback)
    k_down = dpg.add_key_down_handler(key=dpg.mvKey_S, callback=callback)

    k_release = dpg.add_key_release_handler(key=dpg.mvKey_A)
    k_press = dpg.add_key_press_handler(key=dpg.mvKey_A)

# main loop
dpg.show_viewport()
while dpg.is_dearpygui_running():
    dpg.render_dearpygui_frame()  

dpg.destroy_context()
