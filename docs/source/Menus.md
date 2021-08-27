| [Basic Usage](#basic-usage) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Menus) |
|----|
||

The menu bar consists of the following components:

* Menu Bar - The main menu ribbon
* Menu - Drop down menus "sub-menus"
* Menu Item - Items that can run callbacks (basically selectables)
* Items are added to the Menu Bar from right to left. Items are added to the menus from top to bottom.

Menus can be nested as necessary.

Any widget can be added to a menu.

## Basic Usage

```python
import dearpygui.dearpygui as dpg

def print_me(sender):
    print(f"Menu Item: {sender}")

with dpg.window(label="Tutorial"):

    with dpg.menu_bar():

        with dpg.menu(label="File"):

            dpg.add_menu_item(label="Save", callback=print_me)
            dpg.add_menu_item(label="Save As", callback=print_me)

            with dpg.menu(label="Settings"):

                dpg.add_menu_item(label="Setting 1", callback=print_me)
                dpg.add_menu_item(label="Setting 2", callback=print_me)

        dpg.add_menu_item(label="Help", callback=print_me)

        with dpg.menu(label="Widget Items"):

            dpg.add_checkbox(label="Pick Me", callback=print_me)
            dpg.add_button(label="Press Me", callback=print_me)
            dpg.add_color_picker(label="Color Me", callback=print_me)

dpg.start_dearpygui()
```

Results:

![](https://github.com/hoffstadt/DearPyGui/blob/assets/wiki_images/menus.PNG)