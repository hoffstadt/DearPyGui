import dearpygui.dearpygui as dpg
import math

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

pwidth, pheight, channels, data = dpg.load_image("../../Resources/SpriteMapExample.png")

with dpg.texture_registry(show=False):
    dpg.add_static_texture(pwidth, pheight, data, tag="texture_tag")

class Camera:

    def __init__(self, pos=dpg.mvVec4(0.0, 0.0, 0.0, 1.0), pitch=0.0, yaw=0.0):
       
        self.pos = pos
        self.pitch = pitch
        self.yaw = yaw
        self.moving = False
        self.mouse_pos = [0, 0]
        self.dirty = False
        self.travel_speed = 0.3
        self.rotation_speed = 0.004
        self.lock_rotation = False

    def move_handler(self, sender, pos, user):

        if self.moving:
            dx = self.mouse_pos[0] - pos[0]
            dy = self.mouse_pos[1] - pos[1]

            if dx != 0.0 or dy != 0.0:
                self.rotate(dx, dy)

        self.mouse_pos = pos

    def toggle_moving(self):
        self.moving = not self.moving

    def toggle_rotation_lock(self, value):
        self.lock_rotation = value

    def mark_dirty(self):
        self.dirty = True

    def view_matrix(self):
        return dpg.create_fps_matrix(self.pos, self.pitch, self.yaw)

    def projection_matrix(self, width, height):
        return dpg.create_perspective_matrix(math.pi*45.0/180.0, width/height, 0.01, 400)

    def rotate(self, dx, dy):

        if self.lock_rotation:
            return

        self.yaw = self.yaw + dx * self.rotation_speed 

        twoPi = 2.0*math.pi
        mod = math.fmod(self.yaw, twoPi)
        if mod > math.pi:
            self.yaw = mod - twoPi
        elif mod < -math.pi:
            self.yaw = mod + twoPi

        self.pitch = self.pitch + dy * self.rotation_speed 
        if self.pitch < 0.995 * -math.pi / 2.0:
            self.pitch = 0.995 * -math.pi / 2.0
        elif self.pitch > 0.995 * math.pi / 2.0:
            self.pitch = 0.995 * math.pi / 2.0

        self.dirty = True

    def update(self, sender, key, user):

        if key[0] == dpg.mvKey_A:
            self.pos[0] = self.pos[0] - self.travel_speed * math.cos(self.yaw);
            self.pos[2] = self.pos[2] + self.travel_speed * math.sin(self.yaw);
        if key[0] == dpg.mvKey_D:
            self.pos[0] = self.pos[0] + self.travel_speed * math.cos(self.yaw);
            self.pos[2] = self.pos[2] - self.travel_speed * math.sin(self.yaw);
        if key[0] == dpg.mvKey_R:
            self.pos[1] = self.pos[1] + self.travel_speed;
        if key[0] == dpg.mvKey_F:
            self.pos[1] = self.pos[1] - self.travel_speed;
        if key[0] == dpg.mvKey_W:
            self.pos[0] = self.pos[0] - self.travel_speed * math.sin(self.yaw)
            self.pos[2] = self.pos[2] - self.travel_speed * math.cos(self.yaw)
        if key[0] == dpg.mvKey_S:
            self.pos[0] = self.pos[0] + self.travel_speed * math.sin(self.yaw)
            self.pos[2] = self.pos[2] + self.travel_speed * math.cos(self.yaw)

        self.dirty = True


size = 5
alpha = 200

verticies = [
        [-size, -size, -size],  # 0 near side
        [ size, -size, -size],  # 1
        [-size,  size, -size],  # 2
        [ size,  size, -size],  # 3
        [-size, -size,  size],  # 4 far side
        [ size, -size,  size],  # 5
        [-size,  size,  size],  # 6
        [ size,  size,  size],  # 7
        [-size, -size, -size],  # 8 left side
        [-size,  size, -size],  # 9
        [-size, -size,  size],  # 10
        [-size,  size,  size],  # 11
        [ size, -size, -size],  # 12 right side
        [ size,  size, -size],  # 13
        [ size, -size,  size],  # 14
        [ size,  size,  size],  # 15
        [-size, -size, -size],  # 16 bottom side
        [ size, -size, -size],  # 17
        [-size, -size,  size],  # 18
        [ size, -size,  size],  # 19
        [-size,  size, -size],  # 20 top side
        [ size,  size, -size],  # 21
        [-size,  size,  size],  # 22
        [ size,  size,  size],  # 23
    ]

colors = [
        [255,   0,   0, alpha],
        [255, 255,   0, alpha],
        [255, 255, 255, alpha],
        [255,   0, 255, alpha],
        [  0, 255,   0, alpha],
        [  0, 255, 255, alpha],
        [  0,   0, 255, alpha],
        [  0, 125,   0, alpha],
        [128,   0,   0, alpha],
        [128,  70,   0, alpha],
        [128, 255, 255, alpha],
        [128,   0, 128, alpha]
    ]

outline_color = [255, 255, 255, 0]

camera = Camera(dpg.mvVec4(0.0, 0.0, 30.0, 1.0), 0.0, 0.0)

dpg.set_viewport_resize_callback(lambda:camera.mark_dirty())

with dpg.window(label="Controls", width=500, height=500):

    dpg.add_text("0", label="Camera X", show_label=True, tag="Camera X")
    dpg.add_text("0", label="Camera Y", show_label=True, tag="Camera Y")
    dpg.add_text("0", label="Camera Z", show_label=True, tag="Camera Z")
    dpg.add_text("0", label="Camera Yaw", show_label=True, tag="Camera Yaw")
    dpg.add_text("0", label="Camera Pitch", show_label=True, tag="Camera Pitch")
    dpg.add_checkbox(label="Lock Camera Rotation", callback=lambda s, a: camera.toggle_rotation_lock(a))

with dpg.viewport_drawlist(front=False):

    with dpg.draw_layer(tag="node"):
        dpg.draw_triangle(verticies[1],  verticies[2],  verticies[0],  depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[0], thickness=1)
        dpg.draw_triangle(verticies[1],  verticies[3],  verticies[2],  depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[1], thickness=1)
        dpg.draw_triangle(verticies[7],  verticies[5],  verticies[4],  depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[2], thickness=1)
        dpg.draw_triangle(verticies[6],  verticies[7],  verticies[4],  depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[3], thickness=1)
        dpg.draw_triangle(verticies[9],  verticies[10], verticies[8],  depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[4], thickness=1)
        dpg.draw_triangle(verticies[9],  verticies[11], verticies[10], depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[5], thickness=1)
        dpg.draw_triangle(verticies[15], verticies[13], verticies[12], depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[6], thickness=1)
        dpg.draw_triangle(verticies[14], verticies[15], verticies[12], depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[7], thickness=1)
        dpg.draw_triangle(verticies[18], verticies[17], verticies[16], depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[8], thickness=1)
        dpg.draw_triangle(verticies[19], verticies[17], verticies[18], depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[9], thickness=1)
        dpg.draw_triangle(verticies[21], verticies[23], verticies[20], depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[10], thickness=1)
        dpg.draw_triangle(verticies[23], verticies[22], verticies[20], depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True, color=outline_color, fill=colors[11], thickness=1)
    with dpg.draw_layer(tag="node2"):
        dpg.draw_image_quad("texture_tag", [10, 0], [10+pwidth/16, 0], [10+pwidth/16, 10+pheight/16], [10, 10+pheight/16], uv1=[0, 1], uv2=[1, 1], uv3=[1, 0], uv4=[0, 0])

with dpg.handler_registry(tag="__demo_keyboard_handler"):
    dpg.add_key_down_handler(key=dpg.mvKey_A, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_D, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_R, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_F, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_W, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_S, callback=lambda s, a, u: camera.update(s, a, u))

    dpg.add_mouse_move_handler(callback=lambda s, a, u:camera.move_handler(s, a, u))
    dpg.add_mouse_click_handler(dpg.mvMouseButton_Left, callback=lambda:camera.toggle_moving())
    dpg.add_mouse_release_handler(dpg.mvMouseButton_Left, callback=lambda:camera.toggle_moving())

# main loop
dpg.show_viewport()
while dpg.is_dearpygui_running():

    if camera.dirty:
        width = dpg.get_viewport_client_width()
        height = dpg.get_viewport_client_height()

        model = dpg.create_translation_matrix([0, 0, 0])
        model2 = dpg.create_rotation_matrix(math.pi*45.0/180.0, [0, 0, 1])
        view = camera.view_matrix()
        projection = camera.projection_matrix(width, height)
    
        viewport_transform = dpg.mvMat4(
            width/2.0,        0.0,   0.0,  width/2.0,
                  0.0, -height/2.0,   0.0, height/2.0,
                  0.0,        0.0,   0.5,        0.5,
                  0.0,        0.0,   0.0,        1.0
            )

        dpg.apply_transform("node",  viewport_transform*projection*view*model)
        dpg.apply_transform("node2",  viewport_transform*projection*view*model2)

        dpg.set_value("Camera X", str(camera.pos[0]))
        dpg.set_value("Camera Y", str(camera.pos[1]))
        dpg.set_value("Camera Z", str(camera.pos[2]))
        dpg.set_value("Camera Yaw", str(camera.yaw))
        dpg.set_value("Camera Pitch", str(camera.pitch))

        camera.dirty = False

    dpg.render_dearpygui_frame()  

dpg.destroy_context()
