| [Basics](#basics) - [Manual Viewport](#manual-viewport) |
|----|
||

## Basics
The _viewport_ is what you traditionally call the _window_. In the case of _Dear PyGui_, we distinguish between the operating system window and the _Dear PyGui_ window by referring to the former as _the viewport_.

Before calling `start_dearpygui()`, you must do the following:
1. Create a viewport, using `create_viewport(...)`.
2. Assign the viewport, using `setup_dearpygui(...)`.
3. Show the viewport, using `show_viewport(...)`.

As a convenience, we offer the command, `setup_viewport()` which handles the above setup steps.

Once the viewport has been created, you can begin configuring the viewport using `configure_viewport(...)` or the helper commands `set_viewport_*`.

### Note
* Large/small icon must be set before showing the viewport (i.e. you will need to setup the viewport manually).

```Python
import dearpygui.dearpygui as dpg

with dpg.window(label="Example Window", width=300):
    dpg.add_text("Hello, world")
    dpg.add_input_text(label='Viewport Title', callback=lambda sender, value: dpg.set_viewport_title(title=value))

dpg.setup_viewport()
dpg.set_viewport_title(title='Custom Title')
dpg.set_viewport_width(500)
dpg.set_viewport_height(200)

dpg.start_dearpygui()
```

## Manual Viewport
A more general approach to viewport creation and modification can be seen below. This is currently required if you need to set the icons.

```Python
import dearpygui.dearpygui as dpg

vp = dpg.create_viewport(title='Custom Title', width=600, height=200) # create viewport takes in config options too!

# must be called before showing viewport
dpg.set_viewport_small_icon("path/to/icon.ico")
dpg.set_viewport_large_icon("path/to/icon.ico")

with dpg.window(label="Example Window", width=500, height=150):
    dpg.add_text("Hello, world")

dpg.setup_dearpygui(viewport=vp)
dpg.show_viewport(vp)
dpg.start_dearpygui()

```
