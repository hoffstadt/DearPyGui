IO, Handlers, State Polling
===========================

Handlers
--------

Handlers are items that submit a callback when the specified state of an item changes.

Handlers can be activated or deactivated by showing or hiding them.

Handlers are required to be added to a handler registry. 

Item Handlers
-------------

Item handlers listen for states related to a specific item.

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

 Item handlers are required to be added to a item handler registry.

 Item handler registries can be bound to an item. 
 They can be bound to multiple items to prevent having to duplicate handlers for every item.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def change_text(sender, app_data):
        dpg.set_value("text item", f"Mouse Button ID: {app_data}")

    def visible_call(sender, app_data):
        print("I'm visible")

    with dpg.item_handler_registry(tag="widget handler") as handler:
        dpg.add_item_clicked_handler(callback=change_text)
        dpg.add_item_visible_handler(callback=visible_call)

    with dpg.window(width=500, height=300):
        dpg.add_text("Click me with any mouse button", tag="text item")
        dpg.add_text("Close window with arrow to change visible state printing to console", tag="text item 2")

    # bind item handler registry to item
    dpg.bind_item_handler_registry("text item", "widget handler")
    dpg.bind_item_handler_registry("text item 2", "widget handler")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/widget_handlers.gif

Global Handlers (IO Input)
--------------------------

Global handlers listen for actions not tied to a specific item:

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
by input device. They also provide the ability to turn on and off the entire registry.

For example this can allow a mouse input registry or a keyboard input
registry. Registries also give the ability to deactivate all their
children handlers by simply hiding the registry.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def change_text(sender, app_data):
        dpg.set_value("text_item", f"Mouse Button: {app_data[0]}, Down Time: {app_data[1]} seconds")

    with dpg.handler_registry():
        dpg.add_mouse_down_handler(callback=change_text)

    with dpg.window(width=500, height=300):
        dpg.add_text("Press any mouse button", tag="text_item")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/examples_wiki_0.8.x/global_hanlders_global_registries.gif

Polling Item State
------------------

Polling item state is accessible through
:py:func:`get_item_state <dearpygui.dearpygui.get_item_state>`. These can be very powerful
when combined with handlers as shown below.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def change_text(sender, app_data):
        if dpg.is_item_hovered("text item"):
            dpg.set_value("text item", f"Stop Hovering Me, Go away!!")
        else:
            dpg.set_value("text item", f"Hover Me!")

    with dpg.handler_registry():
        dpg.add_mouse_move_handler(callback=change_text)

    with dpg.window(width=500, height=300):
        dpg.add_text("Hover Me!", tag="text item")


    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
