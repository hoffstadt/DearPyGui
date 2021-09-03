Item Callbacks
==============

Most UI items have a callback which is submitted to a 
**queue of callbacks** when the item is interacted with.

Callbacks are used to give functionality to items. Callbacks 
can either be assigned to the item upon creation or after creation 
using :py:func:`set_item_callback <dearpygui.dearpygui.set_item_callback>` as shown in the code below.

Callbacks in DPG take **sender**, **app_data**, **user_data** arguments.

Sender, App_data
----------------

sender:
    argument is used by DPG to inform the
    callback which item triggered the callback by sending the id.

app_data:
    argument is used DPG to send information
    to the callback. This includes the current value of most basic widgets.

.. code-block:: python
    import dearpygui.dearpygui as dpg
    
    def button_callback(sender, app_data):
        print(f"sender is: {sender}")
        print(f"app_data is: {app_data}")
    
    with dpg.window(label="Tutorial"):
    
        dpg.add_button(label="Apply", callback=button_callback)
    
    dpg.start_dearpygui()

User Data
---------

app_data:
    argument is **Optionally** used to pass
    your own python object into the function.

The python object can be assigned or updated to the keyword *user_data* when the
item is created or after the item is created using
:py:func:`set_item_user_data <dearpygui.dearpygui.set_item_user_data>`

User data can be any python object. 

.. code-block:: python
    import dearpygui.dearpygui as dpg
    
    def button_callback(sender, app_data, user_data):
        print(f"sender is: {sender}")
        print(f"app_data is: {app_data}")
        print(f"user_data is: {user_data}")
    
    with dpg.window(label="Tutorial"):

        #user data set when button is created
        dpg.add_button(label="Apply", callback=button_callback, user_data="Some Data")

        #user data and callback set any time after button has been created
        btn = dpg.add_button(label="Apply 2", )
        dpg.set_item_callback(btn, button_callback)
        dpg.set_item_user_data(btn, "Some Extra User Data")

    dpg.start_dearpygui()
