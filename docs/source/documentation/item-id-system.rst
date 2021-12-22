Tag System
==========

In DPG, all items must have an associated unique ID (UUID) 
which can either be an integer or a string.

When a item is created, a tag is generated for you automatically.
It is your responsibility to store this tag if you intend on interacting
with the widget at a later time.

Tags allow for modification of the associated item at runtime.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    unique_id = 0 # to be filled out later

    def callback():
        print(dpg.get_value(unique_id))

    with dpg.window(label="Example"):
        dpg.add_button(label="Press me (print to output)", callback=callback)
        unique_id = dpg.add_input_int(label="Input")

    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()


Generated Tags
--------------
The previous example could also be handled by generating the tag beforehand like this:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    unique_tag = dpg.generate_uuid()

    def callback():
        print(dpg.get_value(unique_tag))

    with dpg.window(label="Example"):
        dpg.add_button(label="Press me (print to output)", callback=callback)
        dpg.add_input_int(label="Input", tag=unique_tag)

    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Aliases
-------

An alias is a string that takes the place of the regular **int** tag. 
Aliases can be used anywhere UUID's can be used. 
It is the user's responsibility to make sure aliases are unique.

A simple example can be seen below:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def callback():
        print(dpg.get_value("unique_tag"))

    with dpg.window(label="Example"):
        dpg.add_button(label="Press me (print to output)", callback=callback)
        dpg.add_input_int(default_value=5, label="Input", tag="unique_tag")

    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Recent Tags
-----------
The most recent tag is stored for the last item, container, and root.

This is useful when the last item created may 
be done at run time, is anonymous or sometimes just for convenience.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Example"):
        with dpg.group():
            dpg.add_button(label="View the Terminal for item tags")
            print(dpg.last_item())
            print(dpg.last_container())
            print(dpg.last_root())

    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
