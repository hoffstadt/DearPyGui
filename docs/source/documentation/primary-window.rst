Primary Window
==============

The primary window fills the viewport and resizes with it.

It will also always remain in the background of other windows.

A window can be set as the primary window by using the
:py:func:`set_primary_window <dearpygui.dearpygui.set_primary_window>`
command using the required True/False allows the window to be set or unset.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(tag="Primary Window"):
        dpg.add_text("Hello, world")
        dpg.add_button(label="Save")
        dpg.add_input_text(label="string", default_value="Quick brown fox")
        dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.set_primary_window("Primary Window", True)
    dpg.start_dearpygui()
    dpg.destroy_context()
