import dearpygui.core as dpg
import dearpygui.contexts as cxt
from dearpygui.common import _help

demo_texture_container = dpg.add_texture_container()
demo_static_texture_1 = ""
demo_static_texture_2 = ""
demo_static_texture_3 = ""
demo_dynamic_texture_1 = ""
demo_dynamic_texture_2 = ""

def _create_static_textures():
    
    global demo_static_texture_1, demo_static_texture_2, demo_static_texture_3

    ## create static textures
    texture_data1 = []
    for i in range(0, 100*100):
        texture_data1.append(255/255)
        texture_data1.append(0)
        texture_data1.append(255/255)
        texture_data1.append(255/255)

    texture_data2 = []
    for i in range(0, 50*50):
        texture_data2.append(255/255)
        texture_data2.append(255/255)
        texture_data2.append(0)
        texture_data2.append(255/255)

    texture_data3 = []
    for row in range(0, 50):
        for column in range(0, 50):
            texture_data3.append(255/255)
            texture_data3.append(0)
            texture_data3.append(0)
            texture_data3.append(255/255)
        for column in range(0, 50):
            texture_data3.append(0)
            texture_data3.append(255/255)
            texture_data3.append(0)
            texture_data3.append(255/255)
    for row in range(0, 50):
        for column in range(0, 50):
            texture_data3.append(0)
            texture_data3.append(0)
            texture_data3.append(255/255)
            texture_data3.append(255/255)
        for column in range(0, 50):
            texture_data3.append(255/255)
            texture_data3.append(255/255)
            texture_data3.append(0)
            texture_data3.append(255/255)

    demo_static_texture_1 = dpg.add_static_texture(100, 100, texture_data1, parent=demo_texture_container)
    demo_static_texture_2 = dpg.add_static_texture(50, 50, texture_data2, parent=demo_texture_container)
    demo_static_texture_3 = dpg.add_static_texture(100, 100, texture_data3, parent=demo_texture_container)

def _create_dynamic_textures():
    
    global demo_dynamic_texture_1, demo_dynamic_texture_2

    ## create dynamic textures
    texture_data1 = []
    for i in range(0, 100*100):
        texture_data1.append(255/255)
        texture_data1.append(0)
        texture_data1.append(255/255)
        texture_data1.append(255/255)

    texture_data2 = []
    for i in range(0, 50*50):
        texture_data2.append(255/255)
        texture_data2.append(255/255)
        texture_data2.append(0)
        texture_data2.append(255/255)

    demo_dynamic_texture_1 = dpg.add_dynamic_texture(100, 100, texture_data1, parent=demo_texture_container)
    demo_dynamic_texture_2 = dpg.add_dynamic_texture(50, 50, texture_data2, parent=demo_texture_container)

def _update_dynamic_textures(sender, app_data, user_data):

    global demo_dynamic_texture_1, demo_dynamic_texture_2

    new_color = dpg.get_value(sender)
    new_color[0] = new_color[0]/255
    new_color[1] = new_color[1]/255
    new_color[2] = new_color[2]/255
    new_color[3] = new_color[3]/255

    if user_data == 1:
        texture_data = []
        for i in range(0, 100*100):
            texture_data.append(new_color[0])
            texture_data.append(new_color[1])
            texture_data.append(new_color[2])
            texture_data.append(new_color[3])
        dpg.set_value(demo_dynamic_texture_1, texture_data)

    elif user_data == 2:
        texture_data = []
        for i in range(0, 50*50):
            texture_data.append(new_color[0])
            texture_data.append(new_color[1])
            texture_data.append(new_color[2])
            texture_data.append(new_color[3])
        dpg.set_value(demo_dynamic_texture_2, texture_data)

def show_demo():

    _create_static_textures()
    _create_dynamic_textures()

    with cxt.collapsing_header(label="Textures & Images"):
        
        with cxt.tree_node(label="Help"):

            dpg.add_separator()
            dpg.add_text("ABOUT TEXTURES:")
            dpg.add_text("Textures are buffers of RGBA data.", bullet=True, indent=20)
            dpg.add_text("Textures are used by 'image based' widgets:", bullet=True, indent=20)
            dpg.add_text("add_image", bullet=True, indent=50)
            dpg.add_text("add_image_button", bullet=True, indent=50)
            dpg.add_text("draw_image", bullet=True, indent=50)
            dpg.add_text("add_image_series", bullet=True, indent=50)
            dpg.add_text("Textures are themselves widgets.", bullet=True, indent=20)
            dpg.add_text("Textures must be a child of a texture container widget.", bullet=True, indent=20)
            dpg.add_text("Textures can be either static or dynamic (see following sections).", bullet=True, indent=20)

            dpg.add_separator()
            dpg.add_text("PROGRAMMER GUIDE:")
            dpg.add_text("'image based' widgets hold a reference to a texture widget.", bullet=True, indent=20)
            dpg.add_text("Deleting the texture widget will not affect widget's using it.", bullet=True, indent=50)
            dpg.add_text("Textures are only free'd from the GPU when the reference count reaches 0.", bullet=True, indent=50)
            dpg.add_text("The texture container widget is a root (has no parent).", bullet=True, indent=20)
            dpg.add_text("The texture container widget is hidden by default.", bullet=True, indent=20)
            dpg.add_text("'Showing' it, opens a manager to inspect the textures within.", bullet=True, indent=50)
            dpg.add_same_line()
            dpg.add_button(label="Press Here", small=True, callback=lambda:dpg.configure_item(demo_texture_container, show=True))
            dpg.add_separator()

        with cxt.tree_node(label="Static Textures"):

            dpg.add_separator()
            dpg.add_text("ABOUT STATIC TEXTURES:")
            dpg.add_text("Can NOT be modified after creation.", bullet=True, indent=20)
            dpg.add_text("Can be loaded from a file using the 'file' keyword.", bullet=True, indent=20)
            dpg.add_separator()

            with cxt.group(horizontal=True):

                with cxt.group():
                    dpg.add_text("Image Button")
                    dpg.add_image_button(demo_static_texture_1)

                with cxt.group():
                    dpg.add_text("Image")
                    dpg.add_image(demo_static_texture_2)

                with cxt.group():
                    dpg.add_text("Image (texture size)")
                    dpg.add_image(demo_static_texture_3)

                with cxt.group():
                    dpg.add_text("Image (2x texture size)")
                    dpg.add_image(demo_static_texture_3, width=200, height=200)

            dpg.add_image("INTERNAL_DPG_FONT_ATLAS")

        with cxt.tree_node(label="Dynamic Textures"):

            dpg.add_separator()
            dpg.add_text("ABOUT DYNAMIC TEXTURES:")
            dpg.add_text("Can be modified after creation with 'set_value'.", bullet=True, indent=20)
            dpg.add_text("New data must be the same dimensions as the original", bullet=True, indent=20)
            dpg.add_separator()

            with cxt.group():
                dpg.add_color_picker(label="Texture 1", default_value=(255, 0, 255, 255), 
                        no_side_preview=True, alpha_bar=True, width=200,
                        callback=_update_dynamic_textures, user_data=1)
                dpg.add_text("Image Button")
                dpg.add_image_button(demo_dynamic_texture_1, width=100, height=100)

            dpg.add_same_line()

            with cxt.group():
                dpg.add_color_picker(label="Texture 2", default_value=(255, 255, 0, 255), 
                                     no_side_preview=True, alpha_bar=True, width=200,
                                     callback=_update_dynamic_textures, user_data=2)
                dpg.add_text("Image")
                dpg.add_image(demo_dynamic_texture_2)

        with cxt.tree_node(label="Image Series (plots)"):

            plot_id=dpg.add_plot(label="Image Plot", height=400)
            yaxis_id = dpg.add_plot_yaxis(label="y axis", parent=plot_id)
            dpg.add_image_series("INTERNAL_DPG_FONT_ATLAS", [300, 300], [400, 400], label="font atlas", parent=yaxis_id)
            dpg.add_image_series(demo_static_texture_1, [0, 0], [100, 100], label="static 1", parent=yaxis_id)
            dpg.add_image_series(demo_static_texture_2, [150, 150], [200, 200], label="static 2", parent=yaxis_id)
            dpg.add_image_series(demo_static_texture_3, [200, -150], [300, -50], label="static 3", parent=yaxis_id)
            dpg.add_image_series(demo_dynamic_texture_1, [-200, 100], [-100, 200], label="dynamic 1", parent=yaxis_id)
            dpg.add_image_series(demo_dynamic_texture_2, [-200, -100], [-150, -50], label="dynamic 2", parent=yaxis_id)

        with cxt.tree_node(label="Drawlists"):

            with cxt.drawlist(width=400, height=300):
                dpg.draw_rectangle((0, 0), (400, 300), color=(100, 100, 100, 250), thickness=2)
                dpg.draw_image(demo_static_texture_3, [0, 0], [100, 100])
                dpg.draw_image(demo_dynamic_texture_1, [200, 100], [300, 200])


