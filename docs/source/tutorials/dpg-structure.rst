DPG Structure Overview
======================

.. meta::
   :description lang=en: General overview of the structure of dpg items.

A DPG app will have an overall structure as follows:
    * Viewport
    * Render Loop
    * Items

Viewport
--------

The viewport is the *window* created by the operating system.

Typically the viewport is handled automatically by DPG.
The viewport needs to be explicitly created to be
customized for taskbar icons, custom sizing, decorators.

Lets go back and revisit the first app but create the viewport explicitly
and give it a new title and size.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    vp = dpg.create_viewport(title='Custom Title', width=600, height=200)

    with dpg.window(label="Example Window"):
        dpg.add_text("Hello, world")
        dpg.add_button(label="Save")
        dpg.add_input_text(label="string", default_value="Quick brown fox")
        dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

    dpg.setup_dearpygui(viewport=vp)
    dpg.show_viewport(vp)

    dpg.start_dearpygui()

.. seealso:: for more information on the viewport :doc:`../api-reference/viewport`

Render Loop
-----------

The render loop is responsible for displaying widgets,
partially maintaining state and handling item callbacks.

The render loop is completely handled
by the :py:func:`start_dearpygui <dearpygui.dearpygui.start_dearpygui>` command.

In some cases it's necessary to explicitly create
the render loop for calling python commands that may need to run every frame.

Lets add this into the first app.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    vp = dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui(viewport=vp)

    with dpg.window(label="Example Window"):
        dpg.add_text("Hello, world")
        dpg.add_button(label="Save")
        dpg.add_input_text(label="string", default_value="Quick brown fox")
        dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

    dpg.show_viewport(vp)

    # below replaces, start_dearpygui()
    while dpg.is_dearpygui_running():
        # insert here any code you would like to run in the render loop
        # you can manually stop by using stop_dearpygui()
        dpg.render_dearpygui_frame()

    dpg.cleanup_dearpygui()

.. seealso:: for more information on the render loop :doc:`../api-reference/render-loop`

Items
-----

DPG can be broken down into **Items**, **UI Items**, **Containers**

Items:
    Items are anything in the library.

UI Items:
    Any item in dpg that shows up on the screen (i.e. button, listbox, window, ect).

Containers:
    Although also a UI item containers will be spoken of as their own category.

Primary Window
--------------

DPG can assign one window to be the *primary window* will fill the
viewport and always be drawn behind and other floating windows.

**Code:**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    with dpg.window(id="Primary Window", label="Example Window"):
        dpg.add_text("Hello, world")
        dpg.add_button(label="Save")
        dpg.add_input_text(label="string", default_value="Quick brown fox")
        dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

    dpg.set_primary_window("Primary Window", True)
    dpg.start_dearpygui()

.. seealso:: for more information on the viewport :doc:`../api-reference/primary-window`