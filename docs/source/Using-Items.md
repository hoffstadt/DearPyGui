| [Basics](#basics) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Using-Widgets) |
|----|
||

**UI items** make up the bulk of a _Dear_PyGui_ interface. They are your go-to items for **accepting input**, **displaying information**, and overall **interaction** between user and application. Some containers such as **tree nodes** are also considered UI items.

## Input Items
You can think of an **input item** as your typical text field, slider, or anything that would serve to accept information. All UI items that accept and store data have the `default_value` parameter, which is empty by default. By including a value to this parameter when creating the item, you can change the items's initial starting value. The **type** that this value can be depends on the widget. For example, an `input_int`'s default value needs to be an integer, or `int`. See the below example of a **window** container parenting two **input_text**'s, and two **slider_float**'s - passing one of each a `default_value`:

```python
import dearpygui.dearpygui as dpg


with dpg.window(width=300):
    input_txt1 = dpg.add_input_text()
    # The value for input_text2 will have a starting value
    # of "This is a default value!"
    input_txt2 = dpg.add_input_text(
        label="InputTxt2",
        default_value="This is a default value!"
    )

    slider_float1 = dpg.add_slider_float()
    # The slider for slider_float2 will have a starting value
    # of 50.0.
    slider_float2 = dpg.add_slider_float(
        label="SliderFloat2",
        default_value=50.0,
    )


dpg.start_dearpygui()
```
![](https://github.com/Atlamillias/DearPyGui-Stuff/blob/main/wiki%20images/dpg_using_widgets_ex1.png)

An input item's value is changed by interacting with it. In the above example, moving `slider_float1`'s slider to `30.55` sets its value to `30.55`. Inversely, we can set the position of the slider by changing items's value at runtime. Just like how passing a `default_value` to `slider_float2` changed the slider's initial position, we can pass the widget's id in a call to `set_value` to change it dynamically. Below is another example: a **window** parenting one **slider_int** passing a `default_value` of `15`, but calling `set_value` to change it to `40`.

```python
import dearpygui.dearpygui as dpg


with dpg.window(width=300):
    # Creating a slider_int widget and setting the
    # default value to 15.
    dpg.add_slider_int(default_value=15, id="slider_int")
    
# On second thought, we're gonna set the value to 40
# instead - for no reason in particular...
dpg.set_value("slider_int", 40)

dpg.start_dearpygui()
```
![](https://github.com/Atlamillias/DearPyGui-Stuff/blob/main/wiki%20images/dpg_using_widgets_ex2.png)
