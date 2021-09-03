Callbacks and Configuration
===============================

.. meta::
   :description lang=en: General overview of the structure of dpg items.

Callbacks
---------

Callbacks give UI items functionality and almost all UI Items
in DPG can run callbacks.

Functions or methods are assigned as UI item callbacks when
an item is created or at a later time using
:py:func:`set_item_callback <dearpygui.dearpygui.set_item_callback>`

Callbacks should typically have up to 3
standard arguments:

sender:
   the *id* of the UI item that submitted the callback

app_data:
   occasionally UI items will send their own data (ex. file dialog)

user_data:
   any python object you want to send to the function

**Code:**

.. code-block:: python
    import dearpygui.dearpygui as dpg

    def button_callback(sender, app_data, user_data):
        print(f"sender is: {sender}")
        print(f"app_data is: {app_data}")
        print(f"user_data is: {user_data}")

    with dpg.window(label="Tutorial"):

        #user data and callback set when button is created
        dpg.add_button(label="Apply", callback=button_callback, user_data="Some Data")

        #user data and callback set any time after button has been created
        btn = dpg.add_button(label="Apply 2", )
        dpg.set_item_callback(btn, button_callback)
        dpg.set_item_user_data(btn, "Some Extra User Data")

    dpg.start_dearpygui()

.. seealso::
    For more information on the item callbacks :doc:`../api-reference/item-callbacks`

Configuration
-------------

The render loop is responsible for displaying widgets,
partially maintaining state and handling item callbacks.

The render loop is completely handled
by the :py:func:`start_dearpygui <dearpygui.dearpygui.start_dearpygui>` command.

In some cases it's necessary to explicitly create
the render loop for calling python commands that may need to run every frame.

Lets add this into the first app.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    vp = dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui(viewport=vp)

    with dpg.window(label="Example Window"):
        dpg.add_text("Hello, world")
        dpg.add_button(label="Save")
        dpg.add_input_text(label="string", default_value="Quick brown fox")
        dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

    dpg.show_viewport(vp)

    # below replaces, start_dearpygui()
    while dpg.is_dearpygui_running():
        # insert here any code you would like to run in the render loop
        # you can manually stop by using stop_dearpygui()
        dpg.render_dearpygui_frame()

    dpg.cleanup_dearpygui()

.. seealso::
    For more information on the item configuration :doc:`../api-reference/item-configuration`