IO, Handlers, State Polling
===========================

Handlers
--------

Handlers are items that listen for certain events then submit a callback when that event occurs.
Handlers can be activated or deactivated by showing or hiding them. 

UI Item Handlers
----------------

UI item handlers listen for events related to a UI item

Events:
 * Activated
 * Active
 * Clicked
 * Deactivated
 * Deactivated After Edited
 * Focus
 * Hover
 * Resize
 * Toggled
 * Visible

.. code-block:: python

    import dearpygui.dearpygui as dpg

    def change_text(sender, app_data):
        dpg.set_value("text_item", f"Mouse Button ID: {app_data}")

    with dpg.window(width=500, height=300):
        dpg.add_text("Click me with any mouse button", id="text_item")
        dpg.add_clicked_handler(text_widget, callback=change_text)

    dpg.start_dearpygui()

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/widget_handlers.gif

Global Handlers (IO Input)
---------------

Global handlers listen for events related to the viewport such as:

**Keys:**
 * Down
 * Press
 * Release

**Mouse:**
 * Click
 * Double Click
 * Down
 * Drag
 * Move
 * Release
 * Wheel

Global handlers are required to be added to a handler registry. 
Registries provide a grouping aspect to handlers allowing separation
by input device.

For example this can allow a mouse registry or a keyboard
registry ect. Registries also give the ability to deactivate all their
children handlers by simply turning off the show keyword in the registry.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    def change_text(sender, app_data):
        dpg.set_value("text_item", f"Mouse Button: {app_data[0]}, Down Time: {app_data[1]} seconds")

    with dpg.window(width=500, height=300):
        dpg.add_text("Press any mouse button", id="text_item")
        with dpg.handler_registry():
            dpg.add_mouse_down_handler(callback=change_text)

    dpg.start_dearpygui()

**However** if you call
:py:func:`setup_registries <dearpygui.dearpygui.setup_registries>`
before using global handlers this will allow
the creation of global handlers anywhere.

This is because setup_registries creates a default registry that
handlers can fall back to.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.setup_registries()

    def change_text(sender, app_data):
        dpg.set_value("text_item", f"Mouse Button: {app_data[0]}, Down Time: {app_data[1]} seconds")

    with dpg.window(width=500, height=300):
        dpg.add_text("Press any mouse button")
        dpg.add_mouse_down_handler(callback=change_text)

    dpg.start_dearpygui()

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/global_hanlders_global_registries.gif

Polling Item State
------------------

Polling item state is accessible through
:py:func:`get_item_state <dearpygui.dearpygui.get_item_state>`
or all the light wrappers provided. These can be very powerful
when combined with handlers as shown below.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.setup_registries()

    def change_text(sender, app_data):
        if dpg.is_item_hovered("text_item"):
            dpg.set_value("text_item", f"Stop Hovering Me, Go away!!")
        else:
            dpg.set_value(user_data, f"Hover Me!")

    with dpg.window(width=500, height=300):
        dpg.add_text("Hover Me!", id="text_item")
        dpg.add_mouse_move_handler(callback=change_text)

    dpg.start_dearpygui()
