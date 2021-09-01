First Steps
===========

.. meta::
   :description lang=en: The starting point for the tutorial set.

Tutorials will give a broad overview and working knowledge of DPG. Tutorials can not cover every detail
so refer to the Reference API on each topic to learn more.

Installing
-----------

Python 3.6 (64 bit) or above is required.

.. code-block:: cmd

    pip install dearpygui

First Run
---------

Lets check the pip install by creating a window, adding some Widgets.
and starting the render loop with :py:func:`start_dearpygui <dearpygui.dearpygui.start_dearpygui>`.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    with dpg.window(label="Example Window"):
        dpg.add_text("Hello, world")
        dpg.add_button(label="Save")
        dpg.add_input_text(label="string", default_value="Quick brown fox")
        dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

    dpg.start_dearpygui()

**Result:**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/BasicUsageExample1.PNG

.. note:: The main script must always conclude with :py:func:`start_dearpygui <dearpygui.dearpygui.show_documentation>`.
