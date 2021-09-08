Init Files
==========

**Init** files are used to preserve the following data between application sessions

* window positions
* window sizes
* window collapse state
* window docking
* table column widths
* table column ordering
* table column visible state
* table column sorting state

.. note:: Init files use the ID of the window. Make sure the ID does not
    change between sessions by generating the ID beforehand.

Creating init files
-------------------

There are two procedures for creating **init** files:
1. Use :py:func:`save_init_file <dearpygui.dearpygui.save_init_file>` while
    your application is running.
2. Temporarily add :py:func:`set_init_file <dearpygui.dearpygui.set_init_file>`
    to your application before starting DPG.

.. note:: windows and tables can individually opt out of having their settings saved with the
    `no_saved_settings` keyword.

Loading init files
------------------

There are two procedures for loading **init** files:
1. Use :py:func:`load_init_file <dearpygui.dearpygui.load_init_file>`before starting DPG.
2. Use :py:func:`set_init_file <dearpygui.dearpygui.set_init_file>` before starting DPG.

.. note:: Procedure 2 will overwrite the **init** file.

Basic Usage
-----------

Below is an example of using **init** files to preserve settings between sessions

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.set_init_file()  # default file is 'dpg.ini'

    with dpg.window(label="about"):
        dpg.add_button(label="Press me")

    dpg.start_dearpygui()

