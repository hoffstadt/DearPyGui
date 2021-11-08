import dearpygui.dearpygui as dpg
import math

dpg.create_context()
dpg.configure_app(init_file="custom_layout.ini")
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
        self.field_of_view = 45.0
        self.nearClip = 0.01
        self.farClip = 400.0

    def move_handler(self, sender, pos, user):

        if self.moving:
            dx = self.mouse_pos[0] - pos[0]
            dy = self.mouse_pos[1] - pos[1]

            if dx != 0.0 or dy != 0.0:
                self.rotate(dx, dy)

        self.mouse_pos = pos

    def toggle_moving(self):
        self.moving = not self.moving

    def mark_dirty(self):
        self.dirty = True

    def view_matrix(self):
        return dpg.create_fps_matrix(self.pos, self.pitch, self.yaw)

    def projection_matrix(self, width, height):
        return dpg.create_perspective_matrix(math.pi*self.field_of_view/180.0, width/height, self.nearClip, self.farClip)

    def rotate(self, dx, dy):

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
        dpg.set_value("Camera Yaw", str(self.yaw))
        dpg.set_value("Camera Pitch", str(self.pitch))
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

        dpg.set_value("Camera X", str(self.pos[0]))
        dpg.set_value("Camera Y", str(self.pos[1]))
        dpg.set_value("Camera Z", str(self.pos[2]))
        self.dirty = True

    def _set_field_of_view(self, value):
        self.field_of_view = float(value)
        self.dirty = True

    def _set_near(self, value):
        self.nearClip = value
        self.dirty = True

    def _set_far(self, value):
        self.farClip = value
        self.dirty = True

    def show_controls(self):

        with dpg.window(label="Camera Controls", width=500, height=500):

            dpg.add_text(str(self.pos[0]), label="Camera X", show_label=True, tag="Camera X")
            dpg.add_text(str(self.pos[1]), label="Camera Y", show_label=True, tag="Camera Y")
            dpg.add_text(str(self.pos[2]), label="Camera Z", show_label=True, tag="Camera Z")
            dpg.add_text("0", label="Camera Yaw", show_label=True, tag="Camera Yaw")
            dpg.add_text("0", label="Camera Pitch", show_label=True, tag="Camera Pitch")
            dpg.add_slider_float(label="near_clip", min_value=0.1, max_value=100, default_value=self.nearClip, callback=lambda s, a: self._set_near(a))
            dpg.add_slider_float(label="far_clip", min_value=0.2, max_value=1000, default_value=self.farClip, callback=lambda s, a: self._set_far(a))
            dpg.add_text("field_of_view")
            dpg.add_radio_button(["45.0", "60.0", "90.0"], default_value="45.0", label="field of view", callback=lambda s, a: self._set_field_of_view(a))

class Cube:

    def __init__(self, size, alpha, pos=[0, 0, 0]):

        self.verticies = [
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

        self.colors = [
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

        self.outline_color = [255, 255, 255, 0]

        self.triangles = []
        self.depth_clipping = True
        self.perspective_divide = True
        self.cull_mode = dpg.mvCullMode_Back
        self.dirty = False
        self.pos = pos
        self.rot = [0, 0, 0]
        self.scale = [1.0, 1.0, 1.0]
        self.node = dpg.generate_uuid()

    def submit(self):
        with dpg.draw_layer(tag=self.node, depth_clipping=True, cull_mode=dpg.mvCullMode_Back, perspective_divide=True):
            self.triangles.append(dpg.draw_triangle(self.verticies[1],  self.verticies[2],  self.verticies[0], color=self.outline_color, fill=self.colors[0], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[1],  self.verticies[3],  self.verticies[2], color=self.outline_color, fill=self.colors[1], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[7],  self.verticies[5],  self.verticies[4], color=self.outline_color, fill=self.colors[2], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[6],  self.verticies[7],  self.verticies[4], color=self.outline_color, fill=self.colors[3], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[9],  self.verticies[10], self.verticies[8], color=self.outline_color, fill=self.colors[4], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[9],  self.verticies[11], self.verticies[10], color=self.outline_color, fill=self.colors[5], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[15], self.verticies[13], self.verticies[12], color=self.outline_color, fill=self.colors[6], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[14], self.verticies[15], self.verticies[12], color=self.outline_color, fill=self.colors[7], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[18], self.verticies[17], self.verticies[16], color=self.outline_color, fill=self.colors[8], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[19], self.verticies[17], self.verticies[18], color=self.outline_color, fill=self.colors[9], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[21], self.verticies[23], self.verticies[20], color=self.outline_color, fill=self.colors[10], thickness=1))
            self.triangles.append(dpg.draw_triangle(self.verticies[23], self.verticies[22], self.verticies[20], color=self.outline_color, fill=self.colors[11], thickness=1))
  

    def _reconfigure(self):
        dpg.configure_item(self.node, depth_clipping=self.depth_clipping, perspective_divide=self.perspective_divide, cull_mode=self.cull_mode)

    def _set_depth_clipping(self, value):
        self.depth_clipping = value
        self._reconfigure()

    def _set_perspective_divide(self, value):
        self.perspective_divide = value
        self._reconfigure()

    def _set_cull_mode(self, value):

        if value == "mvCullMode_None":
            self.cull_mode = dpg.mvCullMode_None
        elif value == "mvCullMode_Front":
            self.cull_mode = dpg.mvCullMode_Front
        elif value == "mvCullMode_Back":
            self.cull_mode = dpg.mvCullMode_Back
        self._reconfigure()

    def _set_rotation(self, value):
        self.rot = value
        self.dirty = True

    def _set_position(self, value):
        self.pos = value
        self.dirty = True

    def _set_scale(self, value):
        self.scale = value
        self.dirty = True

    def update_clip_space(self, top_left_x, top_left_y, width, height, min_depth, max_depth):
        dpg.set_clip_space(self.node, top_left_x, top_left_y, width, height, min_depth, max_depth)

    def show_controls(self):

        with dpg.window(label="Controls", width=500, height=500):
            dpg.add_checkbox(label="depth_clipping", default_value=self.depth_clipping, callback=lambda s, a: self._set_depth_clipping(a))
            dpg.add_checkbox(label="perspective_divide", default_value=self.perspective_divide, callback=lambda s, a: self._set_perspective_divide(a))
            dpg.add_text("cull_mode")
            dpg.add_radio_button(["mvCullMode_None", "mvCullMode_Back", "mvCullMode_Front"], default_value="mvCullMode_Back", label="cull_mode", callback=lambda s, a: self._set_cull_mode(a))
            dpg.add_slider_floatx(label="Position", size=3, callback=lambda s, a: self._set_position(a))
            dpg.add_slider_floatx(label="Rotation", size=3, min_value=-math.pi, max_value=math.pi, callback=lambda s, a: self._set_rotation(a))
            dpg.add_slider_floatx(label="Scale", size=3, max_value=10.0, default_value=self.scale, callback=lambda s, a: self._set_scale(a))

    def update(self, projection, view):

         model = dpg.create_translation_matrix(self.pos) \
            * dpg.create_rotation_matrix(self.rot[0], [1, 0, 0]) \
            * dpg.create_rotation_matrix(self.rot[1], [0, 1, 0]) \
            * dpg.create_rotation_matrix(self.rot[2], [0, 0, 1]) \
            * dpg.create_scale_matrix(self.scale)
         dpg.apply_transform(self.node,  projection*view*model)

camera = Camera(dpg.mvVec4(0.0, 0.0, 30.0, 1.0), 0.0, 0.0)
cube = Cube(5, 200)
cube.show_controls()
camera.show_controls()

#dpg.show_metrics()

cubes = []
for i in range(0):
    for j in range(3):
        cubes.append(Cube(3, 255, [i*8, j*8, 0]))

#cubes.reverse()
dpg.set_viewport_resize_callback(lambda:camera.mark_dirty())
rect = dpg.generate_uuid()

with dpg.viewport_drawlist(front=False):

    cube.submit()

    for c in cubes:
        c.submit()
    
    dpg.draw_rectangle((0, 0), (10, 10), tag=rect)
    with dpg.draw_layer(tag="gizmo", perspective_divide=True):
        dpg.draw_line((0.0, 0.0, 0.0), (1.0, 0.0, 0.0), color=(255, 0, 0))
        dpg.draw_line((0.0, 0.0, 0.0), (0.0, 1.0, 0.0), color=(0, 255, 0))
        dpg.draw_line((0.0, 0.0, 0.0), (0.0, 0.0, 1.0), color=(0, 0, 255))

with dpg.handler_registry(tag="__demo_keyboard_handler"):
    dpg.add_key_down_handler(key=dpg.mvKey_A, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_D, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_R, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_F, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_W, callback=lambda s, a, u: camera.update(s, a, u))
    dpg.add_key_down_handler(key=dpg.mvKey_S, callback=lambda s, a, u: camera.update(s, a, u))

    dpg.add_mouse_move_handler(callback=lambda s, a, u:camera.move_handler(s, a, u))
    dpg.add_mouse_click_handler(dpg.mvMouseButton_Right, callback=lambda:camera.toggle_moving())
    dpg.add_mouse_release_handler(dpg.mvMouseButton_Right, callback=lambda:camera.toggle_moving())

# main loop
dpg.show_viewport()
while dpg.is_dearpygui_running():

    if camera.dirty or cube.dirty:
        width = dpg.get_viewport_client_width()
        height = dpg.get_viewport_client_height()
        cube.update_clip_space(width/4, height/4, width/2, height/2.0, -1.0, 1.0)
        dpg.configure_item(rect, pmin=(width/4, height/4), pmax=(0.75*width, 0.75*height))
        view = camera.view_matrix()
        projection = camera.projection_matrix(width/2, height/2)
    
        dpg.apply_transform("gizmo",  projection*view)

        cube.update(projection, view)
        for c in cubes:
            c.update(projection, view)
            c.update_clip_space(width/4, height/4, width/2, height/2.0, -1.0, 1.0)

        camera.dirty = False
        cube.dirty = False

    dpg.render_dearpygui_frame()  

dpg.destroy_context()
