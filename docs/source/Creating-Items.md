| [Basics](#basics) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Creating-Widgets) |
|----|
||

DearPyGui can be broken down into a few of categories:
* **Items**
* **UI Items**
* **Containers**


## Items
Everything created in _Dear_PyGui_ is an **item**. New items can be created by calling various `add_*` or `draw_*` functions. These commands return a unique identifier that can be used to later refer to the item. **UI items** and **containers** are also **items** - but not every **item** is necessarily a **UI item** or **container**. Most items must be parented by another item.

All items have the following optional parameters: `label`, `id`, `user_data`, and `use_internal_label`. The `id` is generated automatically and normally does not need to be included. A `label` serves as the display name for an item, while `user_data` can be any value and is frequently used for **callbacks**.


## Containers
**Container** items act as -- well, *containers*. They are used to parent and store other items (including other containers). In addition to creating them by calling their corresponding `add_` function, they can also be created by calling their appropriately-named context manager function. Using these functions are the most convenient (and recommended) way to create new containers.

Below is an example of creating two new **window** items using their context manager function and starting the application:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Window1", pos=(0,0)) as window1:
    ...

with dpg.window(label="Window2", pos=(100,0)) as window2:
    ...

dpg.start_dearpygui()
```
![](https://github.com/Atlamillias/DearPyGui-Stuff/blob/main/wiki%20images/dpg_creating_appitems_ex2.png)

## UI Items
**UI items** are items that are considered to be a visual (and usually interactable) element in your user interface. These include **buttons**, **sliders**, **inputs**, and even other containers such as **windows** and **tree nodes**.

Below is an example for creating a **window** container that parents a few other items:

```python

import dearpygui.dearpygui as dpg


with dpg.window(label="Tutorial") as window:
    # When creating items within the scope of the context
    # manager, they are automatically "parented" by the
    # container created in the initial call. So, "window"
    # will be the parent for all of these items.

    button1 = dpg.add_button(label="Press Me!")

    slider_int = dpg.add_slider_int(label="Slide to the left!",width=100)
    dpg.add_same_line(spacing=10)
    slider_float = dpg.add_slider_float(label="Slide to the right!",width=100)

    # An item's unique identifier (id) is returned when
    # creating items.
    print(f"Printing item id's: {window}, {button1}, {slider_int}, {slider_float}")

# If you want to add an item to an existing container, you
# can specify which by passing the container's id as the
# "parent" parameter.
button2=dpg.add_button(label="Don't forget me!", parent=window)

dpg.start_dearpygui()

```
![](https://github.com/Atlamillias/DearPyGui-Stuff/blob/main/wiki%20images/dpg_creating_widgets_ex1.png)

## Other Items (i.e. everything else)
Event **handlers**, **registries**, **group**, **same_line**, and **themes** are also items. These are under-the-hood items for customizing the functionality, flow, and overall look of your interface.


