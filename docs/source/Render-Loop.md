| [Basics](#basics) |
| ---- |
||

## Basics
For most use cases the render loop does not need to be considered and is completely handled by `start_dearpygui()`. 
For more advanced use cases full access to the render loop can be accessed like so:


```Python
import dearpygui.dearpygui as dpg

dpg.setup_viewport()

with dpg.window(label="Example Window", width=500, height=150):
    dpg.add_text("Hello, world")


# below replaces, start_dearpygui()

while dpg.is_dearpygui_running():
    # you can manually stop by using stop_dearpygui()
    dpg.render_dearpygui_frame()

dpg.cleanup_dearpygui()
```