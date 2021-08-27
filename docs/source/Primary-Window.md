| [Basics](#basics) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Primary-Window) |
| ---- |
||

## Basics
The primary window fills the viewport and resizes with it. It will also always remain in the background of other windows. A window can be set as the primary window by using the `set_primary_window` command using the required True/False allows the window to be set or unset.

```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Tutorial", id="main_window"):
    dpg.add_checkbox(label="Checkbox")

dpg.set_primary_window("main_window", True)
dpg.start_dearpygui()
```