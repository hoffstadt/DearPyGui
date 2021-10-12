Item Creation
==============

DPG can be broken down into **Items**, **UI Items**, **Containers**.

Items
-----

Everything created in DPG is an **item**. New items can be created
by calling various *add_\*\*\** or *draw_\*\*\** functions. These commands
return a unique identifier that can be used to later refer to the item.
**UI items** and **containers** are also **items** - but not every **item**
is necessarily a **UI item** or **container**.

All items have the following optional parameters: *label*, *tag*, *user_data*,
and *use_internal_label*. The *tag* is generated automatically or can be specified. 
A *label* serves as the display name for an item. 
*user_data* can be any value and is frequently used for **callbacks**.

.. note:: Event **handlers**, **registries**, **group**, and **themes** are also items.
    These are under-the-hood items for customizing the functionality, flow,
    and overall look of your APP.


Containers
----------

**Container** items that can contain other (allowable) items.

In addition to creating them by
calling their corresponding *add_\*\*\** function, they can also be
created by calling their corresponding context manager.

.. note:: Containers are more useful (and recommended) when used as context managers.

Below is an example of creating two new **window** items
using their context manager and starting the application:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Window1", pos=(0,0)):
        pass

    with dpg.window(label="Window2", pos=(100,0)):
        pass

    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. image:: https://raw.githubusercontent.com/Atlamillias/DearPyGui-Stuff/main/wiki%20images/dpg_creating_appitems_ex2.png

UI Items
--------

**UI items** are items that are considered to be a visual and 
usually interactable element in your user interface.

These include **buttons**, **sliders**, **inputs**, and even
other containers such as **windows** and **tree nodes**.

Below is an example for creating a **window** container
that contains a few other items:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial") as window:
        # When creating items within the scope of the context
        # manager, they are automatically "parented" by the
        # container created in the initial call. So, "window"
        # will be the parent for all of these items.

        button1 = dpg.add_button(label="Press Me!")

        slider_int = dpg.add_slider_int(label="Slide to the left!", width=100)
        slider_float = dpg.add_slider_float(label="Slide to the right!", width=100)

        # An item's unique identifier (tag) is returned when
        # creating items.
        print(f"Printing item tag's: {window}, {button1}, {slider_int}, {slider_float}")

    # If you want to add an item to an existing container, you
    # can specify it by passing the container's tag as the
    # "parent" parameter.
    button2 = dpg.add_button(label="Don't forget me!", parent=window)

    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. image:: https://raw.githubusercontent.com/Atlamillias/DearPyGui-Stuff/main/wiki%20images/dpg_creating_widgets_ex1.png


Runtime Adding and Deleting 
---------------------------

With DPG you can dynamically add, delete, and move items at runtime.

This can be done by using a callback to run the desired item's *add_\*\*\**
command and specifying the parent the item will belong to.

By using the **before** keyword when adding a item you can control which
item in the parent the new item will come before. Default will place the
new widget at the end.

Below is an example demonstrating adding and deleting items during runtime:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def add_buttons():
        global new_button1, new_button2
        new_button1 = dpg.add_button(label="New Button", before="delete_button", tag="new_button1")
        new_button2 = dpg.add_button(label="New Button 2", parent="secondary_window", tag="new_button2")


    def delete_buttons():
        dpg.delete_item("new_button1")
        dpg.delete_item("new_button2")


    with dpg.window(label="Tutorial", pos=(200, 200)):
        dpg.add_button(label="Add Buttons", callback=add_buttons)
        dpg.add_button(label="Delete Buttons", callback=delete_buttons, tag="delete_button")

    with dpg.window(label="Secondary Window", tag="secondary_window", pos=(100, 100)):
        pass

    dpg.create_viewport(title='Custom Title', width=600, height=400)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. hint::
    When deleting a container the container and its' children are deleted by default,
    unless the keyword **children_only** is set to True, i.e.:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def delete_children():
        dpg.delete_item("window", children_only=True)

    with dpg.window(label="Tutorial", pos=(200, 200), tag="window"):
        dpg.add_button(label="Delete Children", callback=delete_children)
        dpg.add_button(label="Button_1")
        dpg.add_button(label="Button_2")
        dpg.add_button(label="Button_3")

    dpg.create_viewport(title='Custom Title', width=600, height=400)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
