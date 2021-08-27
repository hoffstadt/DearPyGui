| [Basics](#basics) - [Adding/Deleting App Items](#adding-and-deleting-app-items) - [Deleting Only Children](#deleting-only-children) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Runtime-Adding-and-Deleting-Widgets) |
| ---- |
||

## Basics
With _Dear PyGui_ you can dynamically add and delete any items at runtime. This can be done by using a callback to run the desired item's `add_***` command and specifying the parent the item will belong to.
By using the **before** keyword when adding a item you can control which item in the parent the new item will come before. Default will place the new widget at the end.

## Adding and Deleting App Items
Below is an example demonstrating adding and deleting app items during runtime:

```python
import dearpygui.dearpygui as dpg

def add_buttons():
    global new_button1, new_button2
    new_button1 = dpg.add_button(label="New Button", before="delete_button", id="new_button1")
    new_button2 = dpg.add_button(label="New Button 2", parent="secondary_window", id="new_button2")

def delete_buttons():
    dpg.delete_item("new_button1")
    dpg.delete_item("new_button2")


with dpg.window(label="Tutorial", pos=(200, 200)):
    dpg.add_button(label="Add Buttons", callback=add_buttons)
    dpg.add_button(label="Delete Buttons", callback=delete_buttons, id="delete_button")

with dpg.window(label="Secondary Window", id="secondary_window", pos=(100, 100)):
    pass

dpg.start_dearpygui()
```

## Deleting Only Children

When deleting a container the container and its children are deleted by default, unless the keyword **children_only** is set to True, i.e.:

```python
import dearpygui.dearpygui as dpg

def delete_children():
    dpg.delete_item("window", children_only=True)

with dpg.window(label="Tutorial", pos=(200, 200), id="window"):
    dpg.add_button(label="Delete Children", callback=delete_children)
    dpg.add_button(label="Button_1")
    dpg.add_button(label="Button_2")
    dpg.add_button(label="Button_3")

dpg.start_dearpygui()
```