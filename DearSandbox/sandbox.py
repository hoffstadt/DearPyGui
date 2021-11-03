import dearpygui.dearpygui as dpg
import math

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

rotation = dpg.create_rotation_transform(math.pi*10.0/180.0, [0, 0, -1])
translation = dpg.create_translation_transform([100, 100, 0])
transform = translation*rotation

with dpg.font_registry():
    with dpg.font("../../Resources/NotoSerifCJKjp-Medium.otf", 20, tag="custom font"):
        dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
    dpg.bind_font(dpg.last_container())

dpg.show_metrics()

with dpg.window(label="tutorial", width=500, height=500):

    dpg.draw_quad((0, 0), (100, 0), (100, 100), (0, 100), color=(0, 255, 255), thickness=1)
    with dpg.draw_layer(tag="node1"):
        dpg.draw_quad((0, 0), (100, 0), (100, 100), (0, 100), color=(255, 0, 255), thickness=1)
        dpg.draw_quad((100, 100), (200, 100), (200, 200), (100, 200), color=(0, 255, 255), thickness=2)
    with dpg.draw_layer(tag="node2"):
        dpg.draw_quad((10, 10), (20, 10), (20, 20), (10, 20), color=(0, 255, 255), thickness=2)



# main loop
dpg.show_viewport()
while dpg.is_dearpygui_running():
    transform = translation * dpg.create_rotation_transform(math.pi*dpg.get_frame_count()*3/180.0, [0, 0, -1])
    dpg.apply_transform("node1", transform)
    dpg.apply_transform("node2", 
                        dpg.create_translation_transform([100+100*math.sin(dpg.get_frame_count()/100), 100+50*math.cos(dpg.get_frame_count()/100), 0]) *
                        dpg.create_rotation_transform(math.pi*dpg.get_frame_count()/180.0, [0, 0, -1]))
    dpg.render_dearpygui_frame()  

dpg.destroy_context()
