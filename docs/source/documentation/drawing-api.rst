Drawing-API
===========

DPG has a low level drawing API that is well suited for primitive drawing,
custom widgets or even dynamic drawings.

Drawing commands can be added to containers like drawlist,
viewport_drawlist, or a window.

A drawlist item is created by calling
:py:func:`add_drawlist <dearpygui.dearpygui.add_drawlist>`
then items can be added by calling their respective draw commands.
The origin for the drawing is in the top left and the y-axis points down.

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):

        with dpg.drawlist(width=300, height=300):  # or you could use dpg.add_drawlist and set parents manually

            dpg.draw_line((10, 10), (100, 100), color=(255, 0, 0, 255), thickness=1)
            dpg.draw_text((0, 0), "Origin", color=(250, 250, 250, 255), size=15)
            dpg.draw_arrow((50, 70), (100, 65), color=(0, 200, 255), thickness=1, size=10)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/draw_items.PNG

Layers
------

Drawlists can also contain layers. Layers are an effective way to group 
drawing items for better control of hiding, Z ordering, ect.

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def toggle_layer2(sender):
        show_value = dpg.get_value(sender)
        dpg.configure_item("layer2", show=show_value)

    with dpg.window(label="Tutorial"):
        dpg.add_checkbox(label="show layer", callback=toggle_layer2, default_value=True)

        with dpg.drawlist(width=300, height=300):

            with dpg.draw_layer():
                dpg.draw_line((10, 10), (100, 100), color=(255, 0, 0, 255), thickness=1)
                dpg.draw_text((0, 0), "Origin", color=(250, 250, 250, 255), size=15)
                dpg.draw_arrow((50, 70), (100, 65), color=(0, 200, 255), thickness=1, size=10)

            with dpg.draw_layer(tag="layer2"):
                dpg.draw_line((10, 60), (100, 160), color=(255, 0, 0, 255), thickness=1)
                dpg.draw_arrow((50, 120), (100, 115), color=(0, 200, 255), thickness=1, size=10)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()



Images
------

Drawlists can display images
of types PNG, JPEG, or BMP (See
:doc:`../documentation/textures` for
more detail). Images are added using draw_image.

Using the keywords **pmin** and **pmax** we can define the
upper left and lower right area of the rectangle that the image
will be drawn onto the canvas. The image will scale to fit the specified area.

With keywords **uv_min** and **uv_max** we can specify normalized texture 
coordinates to use just a portion of the area on the image. The
default of uv_min = [0,0] and uv_max = [1,1] will display the entire
image while uv_min = [0,0] uv_max = [0.5,0.5] will only show the first
quarter of the drawing.

To be able to demonstrate these features you must update the directory
to that of an image on your computer, such as
`SpriteMapExample.png <https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/draw_images.PNG>`_.

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    width, height, channels, data = dpg.load_image('SpriteMapExample.png') # 0: width, 1: height, 2: channels, 3: data

    with dpg.texture_registry():
        dpg.add_static_texture(width, height, data, tag="image_id")

    with dpg.window(label="Tutorial"):

        with dpg.drawlist(width=700, height=700):

            dpg.draw_image("image_id", (0, 400), (200, 600), uv_min=(0, 0), uv_max=(1, 1))
            dpg.draw_image("image_id", (400, 300), (600, 500), uv_min=(0, 0), uv_max=(0.5, 0.5))
            dpg.draw_image("image_id", (0, 0), (300, 300), uv_min=(0, 0), uv_max=(2.5, 2.5))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/draw_images.PNG

Viewport and Window
-------------------

You can also use all the same draw_* drawings commands with a
window as the parent. Similarly you can draw to the viewport foreground
or background by using a viewport_drawlist.

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    # creating font and back viewport drawlists
    with dpg.viewport_drawlist():
        dpg.draw_circle((100, 100), 25, color=(255, 255, 255, 255))

    dpg.add_viewport_drawlist(front=False, tag="viewport_back")

    dpg.draw_circle((200, 200), 25, color=(255, 255, 255, 255), parent="viewport_back")

    with dpg.window(label="Tutorial", width=300, height=300):
        dpg.add_text("Move the window over the drawings to see the effects.", wrap=300)
        dpg.draw_circle((100, 100), 25, color=(255, 255, 255, 255))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/draw_viewport.gif
