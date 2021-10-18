DPG Structure Overview
======================

.. meta::
   :description lang=en: General overview of the structure of dpg items.

A DPG app will have an overall structure as follows:
    * Setup
    * Context
    * Viewport
    * Render Loop
    * Items
    * Primary Window

Setup
-----

All DPG apps must do 3 things:
    * Create & Destroy context
    * Create & Show Viewport
    * Setup & Start DearPyGui

The Context
-----------

To access any DPG commands the context must be created with :py:func:`create_context <dearpygui.dearpygui.create_context>` 
This should be the first DPG command and it's typical to perform this with an import.

Proper clean up of DPG can be done using :py:func:`destroy_context <dearpygui.dearpygui.destroy_context>`.

Creating and destroying the context and also setup and start dearpygui 
are useful when the DPG needs to be started and stopped multiple times in one python session.

.. warning:: If :py:func:`create_context <dearpygui.dearpygui.create_context>`
    is not first DPG will not start (and will probably crash).

The Viewport
------------

The viewport is the *window* created by the operating system.

The viewport needs to be explicitly created 
using :py:func:`create_viewport <dearpygui.dearpygui.create_viewport>` 
and shown using :py:func:`show_viewport <dearpygui.dearpygui.show_viewport>`

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Example Window"):
        dpg.add_text("Hello, world")
        dpg.add_button(label="Save")
        dpg.add_input_text(label="string", default_value="Quick brown fox")
        dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. seealso:: For more information on the viewport :doc:`../documentation/viewport`

The Render Loop
---------------

The render loop is responsible for displaying items,
partially maintaining state and callbacks.

The render loop is completely handled
by the :py:func:`start_dearpygui <dearpygui.dearpygui.start_dearpygui>` command.

In some cases it's necessary to explicitly create
the render loop so you can call python commands that may need to run every frame. 
Such as per-frame ticker or counter update functions.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Example Window"):
        dpg.add_text("Hello, world")
        dpg.add_button(label="Save")
        dpg.add_input_text(label="string", default_value="Quick brown fox")
        dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()
    dpg.show_viewport()

    # below replaces, start_dearpygui()
    while dpg.is_dearpygui_running():
        # insert here any code you would like to run in the render loop
        # you can manually stop by using stop_dearpygui()
        print("this will run every frame")
        dpg.render_dearpygui_frame()

    dpg.destroy_context()

.. warning:: The manual render loop must be created after :py:func:`setup_dearpygui <dearpygui.dearpygui.setup_dearpygui>`

.. seealso:: For more information on the render loop :doc:`../documentation/render-loop`

Item Overview
-------------

DPG can be broken down into **Items**, **UI Items**, **Containers**

Items:
    Items are anything in the library (i.e. button, registries, windows, ect).

UI Items:
    Any item in DPG that has a visual component (i.e. button, listbox, window, ect).

Containers:
    Items that can hold other items. (i.e. window, groups, registries, ect).

The Primary Window
------------------

DPG can assign one window to be the *primary window*. 
The primary window will fill the
viewport and always be drawn behind other windows.

**Code:**

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

.. seealso:: for more information on the viewport :doc:`../documentation/primary-window`
