Popups
======

Popups are windows that disappear when clicked outside of the popup's border.

They are typically used as context menus when right-clicking an item or as dialogs.

In DPG, popups are just windows with *popup* set to **True**, *show* set to **False**,
and a *clicked_handler* attached to a widget that shows the window when clicked.

Normally when used, a popup will be shown until you click away from it.
By default, a right click activates the popup.

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):
        dpg.add_text("Right Click Me")

        with dpg.popup(dpg.last_item()):
            dpg.add_text("A popup")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/wiki_images/popups1.PNG

Modal Usage
-----------

When the modal keyword is set to **True**, the popup will be modal.

This prevents the user from interacting with other windows until the popup is closed.
You must hide or delete the popup to remove it.

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):
        dpg.add_text("Left Click Me")

        # check out simple module for details
        with dpg.popup(dpg.last_item(), mousebutton=dpg.mvMouseButton_Left, modal=True, tag="modal_id"):
            dpg.add_button(label="Close", callback=lambda: dpg.configure_item("modal_id", show=False))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/wiki_images/popup2.PNG

Mouse Button options include:
    * _mvMouseButton_Right_
    * _mvMouseButton_Left_
    * _mvMouseButton_Middle_
    * _mvMouseButton_X1_
    * _mvMouseButton_X2_

Window as Dialog Popup
----------------------

This is an example of a window made into a typical dialog.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Delete Files", modal=True, show=False, tag="modal_id", no_title_bar=True):
        dpg.add_text("All those beautiful files will be deleted.\nThis operation cannot be undone!")
        dpg.add_separator()
        dpg.add_checkbox(label="Don't ask me next time")
        with dpg.group(horizontal=True):
            dpg.add_button(label="OK", width=75, callback=lambda: dpg.configure_item("modal_id", show=False))
            dpg.add_button(label="Cancel", width=75, callback=lambda: dpg.configure_item("modal_id", show=False))

    with dpg.window(label="Tutorial"):
        dpg.add_button(label="Open Dialog", callback=lambda: dpg.configure_item("modal_id", show=True))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
