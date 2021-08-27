| [Basics](#basics) - [Images](#images) - [Viewport & Window](#viewport-and-window) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Drawing-API)|
|----|
||

## Basics
_Dear PyGui_ has a low level drawing API that is well suited for primitive drawing, custom widgets or even dynamic drawings.

Drawing commands can be used in containers like drawlist, viewport_drawlist, or a window.

A drawlist widget is created by calling `add_drawlist` then items can be added by calling their respective draw commands. The origin for the drawing is in the top left and the y axis points down.

```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Tutorial"):

    with dpg.drawlist(width=300, height=300): # or you could use dpg.add_drawlist and set parents manually

        dpg.draw_line((10, 10), (100, 100), color=(255, 0, 0, 255), thickness=1)
        dpg.draw_text((0, 0), "Origin", color=(250, 250, 250, 255), size=15)
        dpg.draw_arrow((50, 70), (100, 65), color=(0, 200, 255), thickness=1, size=10)

dpg.start_dearpygui()
```

Results:

![](https://github.com/hoffstadt/DearPyGui/blob/assets/examples_wiki_0.8.x/draw_items.PNG)

## Images

Drawlists can display any textures including images of types PNG, JPEG, or BMP (See [Textures](https://github.com/hoffstadt/DearPyGui/wiki/Textures) for more detail). Images are drawn using draw_image.

Using the keywords **pmin** and **pmax** we can define the upper left and lower right area of the rectangle that the image will be draw onto the canvas. The image will scale to fit the specified area.

With keywords **uv_min** and **uv_max** we can define a scalar number of what area on the image should be drawn to the canvas. The default of uv_min = [0,0] and uv_max = [1,1] will display the entire image while uv_min = [0,0] uv_max = [0.5,0.5] will only show the first quarter of the drawing.

To be able to demonstrate these features you must update the directory to that of an image on your computer, such as [SpriteMapExample.png](https://github.com/hoffstadt/DearPyGui/blob/assets/examples_wiki_0.8.x/draw_images.PNG).

```python
import dearpygui.dearpygui as dpg

width, height, channels, data = dpg.load_image('SpriteMapExample.png') # 0: width, 1: height, 2: channels, 3: data

with dpg.texture_registry():
    dpg.add_static_texture(width, height, data, id="image_id")

with dpg.window(label="Tutorial"):

    with dpg.drawlist(width=700, height=700):

        dpg.draw_image("image_id", (0, 400), (200, 600), uv_min=(0, 0), uv_max=(1, 1))
        dpg.draw_image("image_id", (400, 300), (600, 500), uv_min=(0, 0), uv_max=(0.5, 0.5))
        dpg.draw_image("image_id", (0, 0), (300, 300), uv_min=(0, 0), uv_max=(2.5, 2.5))

dpg.start_dearpygui()
```

Results:

![](https://github.com/hoffstadt/DearPyGui/blob/assets/examples_wiki_0.8.x/draw_images.PNG)

## Viewport and Window

You can also use all the same draw_* drawings commands with a window as the parent. Similarly you can draw to the viewport foreground or background by using a viewport_drawlist.

```python
import dearpygui.dearpygui as dpg

# creating font and back viewport drawlists
dpg.add_viewport_drawlist(id="viewport_front")
dpg.add_viewport_drawlist(front=False, id="viewport_back")

with dpg.window(label="Tutorial", width=300, height=300):
    dpg.add_text("Move the window over the drawings to see the effects.", wrap=300)
    dpg.draw_circle((100, 100), 25, color=(255, 255, 255, 255))
    dpg.draw_circle((100, 100), 25, color=(255, 255, 255, 255), parent="viewport_front")
    dpg.draw_circle((200, 200), 25, color=(255, 255, 255, 255), parent="viewport_back")

dpg.start_dearpygui()
```
Results:
![](https://github.com/hoffstadt/DearPyGui/blob/assets/examples_wiki_0.8.x/draw_viewport.gif)