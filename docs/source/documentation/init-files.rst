Init Files
==========

**Init** files are used to preserve the following data between application sessions

* window positions
* window sizes
* window collapse state
* table column widths
* table column ordering
* table column visible state
* table column sorting state

.. note:: Init files use the tag of the window. Make sure the tag does not
    change between sessions by generating the tag beforehand or specifying it as a string.

Creating init files
-------------------

Use :py:func:`save_init_file <dearpygui.dearpygui.save_init_file>` while
    your application is running.

.. note:: windows and tables can individually opt out of having their settings saved with the
    `no_saved_settings` keyword.

Loading init files
------------------

Use :py:func:`configure_app <dearpygui.dearpygui.configure_app>` before creating the viewport.

Below is an example of using **init** files to preserve settings between sessions.

* Position the windows
* Press the save button and the init file will be saved in the current working directory
* Restart the app and see your windows in the previous positions.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def save_init():
        dpg.save_init_file("dpg.ini")

    dpg.configure_app(init_file="dpg.ini")  # default file is 'dpg.ini'
    with dpg.window(label="about", tag="main window"):
        dpg.add_button(label="Save Window pos", callback=lambda: save_init)

    with dpg.window(label="about", tag="side window"):
        dpg.add_button(label="Press me")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

