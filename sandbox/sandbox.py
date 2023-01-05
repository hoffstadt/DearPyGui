import dearpygui.dearpygui as dpg

dpg.create_context()
def texture_from_file(image_location: str):
    width, height, channels, data = dpg.load_image(image_location)
    with dpg.texture_registry(): texture_id = dpg.add_static_texture(width, height, data)
    return texture_id

with dpg.window():
    dpg.add_button(label="double", tag="double_button", callback=lambda: dpg.configure_item("group_1", width=100))
    dpg.add_button(label="double height", tag="double_button_height", callback=lambda: dpg.configure_item("group_1", height=100))
    with dpg.group(horizontal=True, tag="group_1", width=50):
        dpg.add_image(tag="image", texture_tag=texture_from_file("pycharm.png"))
dpg.create_viewport()
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.show_item_registry()
while dpg.is_dearpygui_running():
    dpg.render_dearpygui_frame()
dpg.destroy_context()