Primary Window
==============

The primary window fills the viewport and resizes with it.

It will also always remain in the background of other windows.

A window can be set as the primary window by using the
:py:func:`set_primary_window <dearpygui.dearpygui.set_primary_window>`
command using the required True/False allows the window to be set or unset.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    with dpg.window(label="Tutorial", id="main_window"):
        dpg.add_checkbox(label="Checkbox")

    dpg.set_primary_window("main_window", True)
    dpg.start_dearpygui()
