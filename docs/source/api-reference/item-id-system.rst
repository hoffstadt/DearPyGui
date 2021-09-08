Item ID System
==============

In DPG, all items must have an associated unique ID (UUID).
Id's allow for modifications of the item at runtime.

When a widget is created, an ID is generated for you automatically.
It is your responsibility to store this ID if you intend on interacting
with the widget at a later time.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    unique_id = 0

    def callback():
        print(dpg.get_value(unique_id))

    with dpg.window(label="Example"):

        dpg.add_button(label="Press me", callback=callback)
        unique_id = dpg.add_input_int(label="Input")

    dpg.start_dearpygui()


Generated IDs
-------------
The previous example could also be handled by generating the id beforehand like this:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    unique_id = dpg.generate_uuid()

    def callback():
        print(dpg.get_value(unique_id))

    with dpg.window(label="Example"):

        dpg.add_button(label="Press me", callback=callback)
        dpg.add_input_int(label="Input", id=unique_id)

    dpg.start_dearpygui()

Aliases (added in 0.8.62)
-------------------------

An alias is a string that takes the place of the regular **int** ID. Aliases can be used anywhere UUID's can be used. It is the user's responsibility to make sure aliases are unique. For more details, see [Aliases](https://github.com/hoffstadt/DearPyGui/wiki/Aliases).
A simple example can be seen below:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    def callback():
        print(dpg.get_value("unique_id"))

    with dpg.window(label="Example"):

        dpg.add_button(label="Press me", callback=callback)
        dpg.add_input_int(label="Input", id="unique_id")

    dpg.start_dearpygui()

Recent IDs
-------------
The most recent ID is stored for the last item, container, and root:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    with dpg.window(label="Example"):

        with dpg.group():
            dpg.add_button(label="Press me")
            print(dpg.last_item())
            print(dpg.last_container())
            print(dpg.last_root())

    dpg.start_dearpygui()