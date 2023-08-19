Menu Bar
========

DPG contains two types of Menu Bars

Viewport Menu Bar 
    Attached to the viewport

Menu Bar:
    Attached to a specified window

Other than being attached to different parents they both act the same.

A typical menu bar consists of the following components:

Menu Bar:
    The main menu ribbon. Used to contain menus.
Menu:
    Popup windows that are used to contain items in a collapsable fashion.
Menu Item:
    Items that can run callbacks and display a checkmark.

Usage
-----

Items added to the Menu Bar are displayed from left to right. 
Items added to the menus are displayed from top to bottom.

Menus can be nested inside other menus.

Any widget can be added to a menu.

**Viewport Menu Bar Example**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    def print_me(sender):
        print(f"Menu Item: {sender}")

    dpg.create_context()
    dpg.create_viewport(title='Custom Title', width=600, height=200)

    with dpg.viewport_menu_bar():
        with dpg.menu(label="File"):
            dpg.add_menu_item(label="Save", callback=print_me)
            dpg.add_menu_item(label="Save As", callback=print_me)

            with dpg.menu(label="Settings"):
                dpg.add_menu_item(label="Setting 1", callback=print_me, check=True)
                dpg.add_menu_item(label="Setting 2", callback=print_me)

        dpg.add_menu_item(label="Help", callback=print_me)

        with dpg.menu(label="Widget Items"):
            dpg.add_checkbox(label="Pick Me", callback=print_me)
            dpg.add_button(label="Press Me", callback=print_me)
            dpg.add_color_picker(label="Color Me", callback=print_me)

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Menu Bar Example**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    dpg.create_viewport(title='Custom Title', width=600, height=200)

    def print_me(sender):
        print(f"Menu Item: {sender}")

    with dpg.window(label="Tutorial"):
        with dpg.menu_bar():
            with dpg.menu(label="File"):
                dpg.add_menu_item(label="Save", callback=print_me)
                dpg.add_menu_item(label="Save As", callback=print_me)

                with dpg.menu(label="Settings"):
                    dpg.add_menu_item(label="Setting 1", callback=print_me, check=True)
                    dpg.add_menu_item(label="Setting 2", callback=print_me)

            dpg.add_menu_item(label="Help", callback=print_me)

            with dpg.menu(label="Widget Items"):
                dpg.add_checkbox(label="Pick Me", callback=print_me)
                dpg.add_button(label="Press Me", callback=print_me)
                dpg.add_color_picker(label="Color Me", callback=print_me)

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/wiki_images/menus.PNG