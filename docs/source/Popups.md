| [Basics](#basics) - [Regular Usage](#regular-usage) - [Modal Usage](#modal-usage) - [Dialog Usage](#dialog-usage) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Popups) |
| ---- |
||

## Basics
Popups are windows that disappear when clicked off of. They are typically used as context menus when right-clicking a widget or as dialogs. In _Dear PyGui_ popups are just windows with *popup* set to **True**, *show* set to **False**, and a *clicked_handler* attached to a widget that shows the window when clicked.

## Regular Usage
Normally when used, a popup exist until you click away from it. By default, a right click activates the popup. An example is found below:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Tutorial"):

    dpg.add_text("Right Click Me")

    with dpg.popup(dpg.last_item()):
        dpg.add_text("A popup")

dpg.start_dearpygui()
```

Results:

![](https://github.com/hoffstadt/DearPyGui/blob/assets/wiki_images/popups1.PNG)

## Modal Usage
When the modal keyword is set to **True**, the popup will be modal. This prevents the user from interacting with other windows until the popup is closed. To close the popup, you must hide it. Below is an example:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Tutorial"):

    dpg.add_text("Left Click Me")

    # check out simple module for details
    with dpg.popup(dpg.last_item(), mousebutton=dpg.mvMouseButton_Left, modal=True, id="modal_id"):
        dpg.add_button(label="Close", callback=lambda: dpg.configure_item("modal_id", show=False))

dpg.start_dearpygui()
```

Results:

![](https://github.com/hoffstadt/DearPyGui/blob/assets/wiki_images/popup2.PNG)

Mouse Button options include:
* _mvMouseButton_Right_
* _mvMouseButton_Left_
* _mvMouseButton_Middle_
* _mvMouseButton_X1_
* _mvMouseButton_X2_

## Dialog Usage
Simple dialog usage:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Delete Files", modal=True, show=False, id="modal_id"):
    dpg.add_text("All those beautiful files will be deleted.\nThis operation cannot be undone!")
    dpg.add_separator()
    dpg.add_checkbox(label="Don't ask me next time")
    dpg.add_button(label="OK", width=75, callback=lambda: dpg.configure_item("modal_id", show=False))
    dpg.add_same_line()
    dpg.add_button(label="Cancel", width=75, callback=lambda: dpg.configure_item("modal_id", show=False))

with dpg.window(label="Tutorial"):

    dpg.add_button(label="Open Dialog", callback=lambda:dpg.configure_item("modal_id", show=True))

dpg.start_dearpygui()
```