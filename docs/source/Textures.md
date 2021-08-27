| [Basics](#basics) - [Static Textures](#static-textures) - [Dynamic Textures](#dynamic-textures) - [Raw Textures](#raw-textures) - [Formats](#formats) -[Loading Images](#loading-images) |
| ---- |
||

## Basics
_Dear PyGui_ uses the Graphics Processing Unit (GPU) to create the graphical user interface(GUI) you see. To display an image, you must first create a texture with the image data that can then be uploaded to the GPU. These textures belong to a texture registry. We offer 3 types of textures:
* Static
* Dynamic
* Raw

This textures are then used in the following App Items:
* **mvDrawImage**
* **mvImage**
* **mvImageButton**
* **mvImageSeries**

They are always 1D lists or arrays.

## Static Textures
Static textures are used for images that do not change often. They are typically loaded at startup. If they need to be updated, you would delete and recreate them. These accept python lists, tuples, numpy arrays, and any type that supports python's buffer protocol with contiguous data. Below is a simple example:
```python
import dearpygui.dearpygui as dpg

texture_data = []
for i in range(0, 100*100):
    texture_data.append(255/255)
    texture_data.append(0)
    texture_data.append(255/255)
    texture_data.append(255/255)

with dpg.texture_registry():
    dpg.add_static_texture(100, 100, texture_data, id="texture_id")

with dpg.window(label="Tutorial"):
    dpg.add_image("texture_id")

dpg.start_dearpygui()
```

## Dynamic Textures
Dynamic textures are used for small to medium sized textures that can change per frame. These can be updated with `set_value(...)` but the width and height must be the same as when the texture was first created. These are similar to raw textures except these perform safety checks and conversion. Below is a simple example:
```python
import dearpygui.dearpygui as dpg

texture_data = []
for i in range(0, 100*100):
    texture_data.append(255/255)
    texture_data.append(0)
    texture_data.append(255/255)
    texture_data.append(255/255)

with dpg.texture_registry():
    dpg.add_dynamic_texture(100, 100, texture_data, id="texture_id")

def _update_dynamic_textures(sender, app_data, user_data):

    new_color = dpg.get_value(sender)
    new_color[0] = new_color[0]/255
    new_color[1] = new_color[1]/255
    new_color[2] = new_color[2]/255
    new_color[3] = new_color[3]/255

    new_texture_data = []
    for i in range(0, 100*100):
        new_texture_data.append(new_color[0])
        new_texture_data.append(new_color[1])
        new_texture_data.append(new_color[2])
        new_texture_data.append(new_color[3])

    dpg.set_value("texture_id", new_texture_data)

with dpg.window(label="Tutorial"):
    dpg.add_image("texture_id")
    dpg.add_color_picker((255, 0, 255, 255), label="Texture", 
        no_side_preview=True, alpha_bar=True, width=200,
        callback=_update_dynamic_textures)

dpg.start_dearpygui()
```

## Raw Textures
Raw textures are used in the same way as dynamic textures. The main differences:
* Only accepts arrays (numpy, python, etc.)
* No safety checks are performed.
These textures are used for high performance applications that require updating large textures every frame. Below is a simple example:
```python
import dearpygui.dearpygui as dpg
import array

texture_data = []
for i in range(0, 100*100):
    texture_data.append(255/255)
    texture_data.append(0)
    texture_data.append(255/255)
    texture_data.append(255/255)

raw_data = array.array('f', texture_data)

with dpg.texture_registry():
    dpg.add_raw_texture(100, 100, raw_data, format=dpg.mvFormat_Float_rgba, id="texture_id")

def update_dynamic_texture(sender, app_data, user_data):

    new_color = dpg.get_value(sender)
    new_color[0] = new_color[0]/255
    new_color[1] = new_color[1]/255
    new_color[2] = new_color[2]/255
    new_color[3] = new_color[3]/255

    for i in range(0, 100*100*4):
        raw_data[i] = new_color[i % 4]

with dpg.window(label="Tutorial"):
    dpg.add_image("texture_id")
    dpg.add_color_picker((255, 0, 255, 255), label="Texture", 
        no_side_preview=True, alpha_bar=True, width=200,
        callback=update_dynamic_texture)

dpg.start_dearpygui()
```

## Formats
The following formats are currently supported:

| Format | Static Texture | Dynamic Texture | Raw Texture |
|--------|----------------|-----------------|-------------|
| **mvFormat_Float_rgba** | ✅ | ✅ | ✅ |
| **mvFormat_Float_rgb** | - | - | ✅* |
| **mvFormat_Int_rgba** | - | - | - |
| **mvFormat_Int_rgb** | - | - | * |

### Notes:
* **mvFormat_Float_rgb** not currently supported on MacOS
* More formats will be added in the future.

## Loading Images
_Dear PyGui_ provides the function `load_image(...)` for loading image data from a file. This function returns a tuple where:
* 0 -> width
* 1 -> height
* 2 -> channels
* 3 -> data (1D array, mvBuffer)
On failure, returns **None**.

The accepted file types include:
* JPEG (no 12-bit-per-channel JPEG OR JPEG with arithmetic coding)
* PNG
* BMP
* PSD
* GIF
* HDR
* PIC
* PPM
* PGM

A simple example can be found below:
```python
import dearpygui.dearpygui as dpg

width, height, channels, data = dpg.load_image("Somefile.png")

with dpg.texture_registry():
    dpg.add_static_texture(width, height, data, id="texture_id")

with dpg.window(label="Tutorial"):
    dpg.add_image("texture_id")

dpg.start_dearpygui()
```
