
## Installing

Ensure you have at least Python 3.6 (64 bit) installed.

```pip install dearpygui```

## First Run

A variety of examples demonstrating usage can be found in the Examples folder on github. Documentation can be found here and also from within the library by using the `show_documentation()` command.

Using Dear PyGui is a simple as creating a python script like the one below

The main script must always conclude with `start_dearpygui()`.

Code:

```python
import dearpygui.dearpygui as dpg

def save_callback():
    print("Save Clicked")

with dpg.window(label="Example Window"):
    dpg.add_text("Hello, world")
    dpg.add_button(label="Save", callback=save_callback)
    dpg.add_input_text(label="string", default_value="Quick brown fox")
    dpg.add_slider_float(label="float", default_value=0.273, max_value=1)
dpg.start_dearpygui()
```

Result:

![Dear PyGui Example](https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/BasicUsageExample1.PNG)
