Item Callbacks
==============

Most items have a callback which is submitted to a 
**queue of callbacks** when the item is interacted with.

Callbacks are used to give functionality to items. Callbacks 
can either be assigned to the item upon creation or after creation 
using :py:func:`set_item_callback <dearpygui.dearpygui.set_item_callback>` 
as shown in the code below.

Callbacks in DPG can have up to 3 arguments. The first is usually
the sender or item triggering the callback. The second is data sent by
DPG for different reasons. The third is reserved for user specified data.
We refer to this in general terms as: **sender**, **app_data**, and **user_data**.

.. note:: Because they are optional positional arguments you 
    must use the *sender* and *app_data* if you want to use *user_data*
    keyword arguments:

Sender, App_data
----------------

sender:
    argument is used by DPG to inform the
    callback which item triggered the callback by sending the tag
    or 0 if trigger by the application.

app_data:
    argument is used DPG to send information
    to the callback i.e. the current value of most basic widgets.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def button_callback(sender, app_data):
        print(f"sender is: {sender}")
        print(f"app_data is: {app_data}")

    with dpg.window(label="Tutorial"):
        dpg.add_button(label="Apply", callback=button_callback)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

User Data
---------

user_data:
    argument is **Optionally** used to pass
    your own python data into the function.

The python data can be assigned or updated to the keyword *user_data* when the
item is created or after the item is created using
:py:func:`set_item_user_data <dearpygui.dearpygui.set_item_user_data>`

User data can be any python object. 

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def button_callback(sender, app_data, user_data):
        print(f"sender is: {sender}")
        print(f"app_data is: {app_data}")
        print(f"user_data is: {user_data}")

    with dpg.window(label="Tutorial"):
        # user data set when button is created
        dpg.add_button(label="Apply", callback=button_callback, user_data="Some Data")

        # user data and callback set any time after button has been created
        dpg.add_button(label="Apply 2", tag="btn")
        dpg.set_item_callback("btn", button_callback)
        dpg.set_item_user_data("btn", "Some Extra User Data")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
