| [Callback Basics](#callback-basics) - [User Data](#user-data) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Widget-Callbacks)|
|----|
||

## Callback Basics

Most UI items have a callback which is submitted to a **queue of callbacks** when the item is interacted with.

Callbacks are used to give functionality to items. Callbacks can either be assigned to the item upon creation or after creation using `set_item_callback` as shown in the code below.

Callbacks in _Dear PyGui_ take **sender**, **app_data**, **user_data** arguments.

The **sender** argument is used by _Dear PyGui_ to inform the callback which item triggered the callback by sending the id.

The **app_data** argument is used _Dear PyGui_ to send information to the callback. This includes the current value of most basic widgets.

The **user_data** argument is data passed to callback when using the **user_data** keyword.

```python
import dearpygui.dearpygui as dpg

def button_callback(sender, app_data, user_data):
    print(f"sender is: {sender}")
    print(f"app_data is: {app_data}")
    print(f"user_data is: {user_data}")

with dpg.window(label="Tutorial"):

    dpg.add_button(label="Apply", callback=button_callback, user_data="Some Data")

dpg.start_dearpygui() 
```

## User Data
In order to send data to the **user_data** keyword of your callback, you need to use the **user_data** keyword when you create the item (it actually just needs a second positional argument). This can be any python object. 
