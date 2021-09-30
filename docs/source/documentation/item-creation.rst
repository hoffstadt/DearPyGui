Item Creation
==============

DearPyGui can be broken down into **Items**, **UI Items**, **Containers**

Items
-----

Everything created in _Dear_PyGui_ is an **item**. New items can be created
by calling various *add_\*\*\** or *draw_\*\*\** functions. These commands
return a unique identifier that can be used to later refer to the item.
**UI items** and **containers** are also **items** - but not every **item**
is necessarily a **UI item** or **container**. Most items must be parented by another item.

All items have the following optional parameters: *label*, *id*, *user_data*,
and *use_internal_label*. The *id* is generated automatically and normally does
not need to be included. A *label* serves as the display name for an item, while
*user_data* can be any value and is frequently used for **callbacks**.

.. note:: Event **handlers**, **registries**, **group**, **same_line**, and **themes** are also items.
    These are under-the-hood items for customizing the functionality, flow,
    and overall look of your interface.


Containers
----------

**Container** items are used to parent and store other items
(including other containers).

In addition to creating them by
calling their corresponding *add_\*\*\** function, they can also be
created by calling their context manager function.

.. note:: Containers are more useful (and recommended) when used as context managers.

Below is an example of creating two new **window** items
using their context manager function and starting the application:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    with dpg.window(label="Window1", pos=(0,0)) as window1:
        pass

    with dpg.window(label="Window2", pos=(100,0)) as window2:
        pass

    dpg.start_dearpygui()

.. image:: https://raw.githubusercontent.com/Atlamillias/DearPyGui-Stuff/main/wiki%20images/dpg_creating_appitems_ex2.png

UI Items
--------

**UI items** are items that are considered to be a visual
(and usually interactable) element in your user interface.
These include **buttons**, **sliders**, **inputs**, and even
other containers such as **windows** and **tree nodes**.

Below is an example for creating a **window** container
that parents a few other items:

.. code-block:: python

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

.. image:: https://raw.githubusercontent.com/Atlamillias/DearPyGui-Stuff/main/wiki%20images/dpg_creating_widgets_ex1.png


Runtime Adding and Deleting 
---------------------------

With DPG you can dynamically add and delete any items at runtime.

This can be done by using a callback to run the desired item's *add_\*\*\**
command and specifying the parent the item will belong to.

By using the **before** keyword when adding a item you can control which
item in the parent the new item will come before. Default will place the
new widget at the end.

Below is an example demonstrating adding and deleting app items during runtime:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    def add_buttons():
        global new_button1, new_button2
        new_button1 = dpg.add_button(label="New Button", before="delete_button", id="new_button1")
        new_button2 = dpg.add_button(label="New Button 2", parent="secondary_window", id="new_button2")

    def delete_buttons():
        dpg.delete_item("new_button1")
        dpg.delete_item("new_button2")


    with dpg.window(label="Tutorial", pos=(200, 200)):
        dpg.add_button(label="Add Buttons", callback=add_buttons)
        dpg.add_button(label="Delete Buttons", callback=delete_buttons, id="delete_button")

    with dpg.window(label="Secondary Window", id="secondary_window", pos=(100, 100)):
        pass

    dpg.start_dearpygui()

.. hint::
    When deleting a container the container and its' children are deleted by default,
    unless the keyword **children_only** is set to True, i.e.:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    def delete_children():
        dpg.delete_item("window", children_only=True)

    with dpg.window(label="Tutorial", pos=(200, 200), id="window"):
        dpg.add_button(label="Delete Children", callback=delete_children)
        dpg.add_button(label="Button_1")
        dpg.add_button(label="Button_2")
        dpg.add_button(label="Button_3")

    dpg.start_dearpygui()
