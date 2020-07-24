# Dear PyGui
[![build status](https://github.com/RaylockLLC/DearPyGui/workflows/Windows/badge.svg)](https://github.com/RaylockLLC/DearPyGui/actions?workflow=Windows)

<sub>(This library is available under a free and permissive license)</sub>

Dear PyGui is a **simple to use (but powerful) Python GUI framework**, built on top of **Dear ImGui**. Although the API may seem similar to Dear ImGui, _DearPyGui_ does not use the immediate mode paradigm, but instead provides a wrapping that simulates a traditional retained mode GUI.

In the same manner Dear ImGui provides a simple way to create tools for game developers, _DearPyGui_ provides a simple way for python developers to **create quick and powerful GUIs for scripts**. It's well suited for **beginners** too!

### Simple Usage

 ---
 
**DearSandbox is an all included framework containing its own embedded python interpreter.**

Code:
```Python
from dearpygui.dearpygui import *

add_text("Hello world")
add_button("Save", callback="SaveCallback")
add_input_text("string")
add_float_slider("float")

start_dearpygui()
```


### Upcoming Changes

---

Some upcoming features:
- An object oriented interface
- Additional Plot types (bar graph, pie chart, etc.)
- Support for window docking and multiple viewports.
- More complete theme and style system.
- 3D widget.
- MacOs and Linux support.
- Custom Widget API.

### License

---

DearPyGui is licensed under the [MIT License](https://github.com/RaylockLLC/DearPyGui/blob/master/LICENSE).
