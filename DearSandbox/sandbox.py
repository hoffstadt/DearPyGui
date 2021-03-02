from dearpygui.core import *
from dearpygui.simple import *
import time
import array as array

textdata = []
for i in range(0, 1280 * 720):
	textdata.append(1.0)
	textdata.append((i/(1280* 720)))
	textdata.append(1.0)
	textdata.append(1.0)
img_shape = [1280, 720]

textarray = array.array('f', textdata)
#set_vsync(False)

with window("main##", width=1280, height=720):
	add_text("taketime")
	add_drawing("canvas", width=img_shape[0], height=img_shape[1])
	add_texture("tex_data", textdata, img_shape[0], img_shape[1], format=mvTEX_RGBA_FLOAT)
	draw_image("canvas", "tex_data", [0.0, 0.0], [img_shape[0], img_shape[1]], tag="texture")


def render(sender, data):
	start_time = time.perf_counter()

	add_texture("tex_data", textdata, img_shape[0], img_shape[1], format=mvTEX_RGBA_FLOAT)
	modify_draw_command("canvas", "texture", file="tex_data")

	delay_time = time.perf_counter() - start_time
	millisecond = (delay_time*1000)
	set_value("taketime", f"taketime : {millisecond:.2f}ms")  # 26.5ms == 37fps

show_logger()
set_render_callback(render)
start_dearpygui()
