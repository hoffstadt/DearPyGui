Viewport
========

The viewport is what you traditionally call the window in other GUI libraries.

In the case of Dear PyGui we call the operating system window *the viewport*
and the Dear PyGui window as *the window*.

Before calling :py:func:`start_dearpygui <dearpygui.dearpygui.start_dearpygui>`, you must do the following:
    1. Create a viewport, using :py:func:`create_viewport <dearpygui.dearpygui.create_viewport>`.
    2. Assign the viewport, using :py:func:`setup_dearpygui <dearpygui.dearpygui.setup_dearpygui>`.
    3. Show the viewport, using :py:func:`show_viewport <dearpygui.dearpygui.show_viewport>`.
    4. calling :py:func:`start_dearpygui <dearpygui.dearpygui.start_dearpygui>`

Once the viewport has been created, you can begin
configuring the viewport using :py:func:`configure_viewport <dearpygui.dearpygui.configure_viewport>`
or the helper commands *set_viewport_\*\*\**.

.. note::
    Large/small icon must be set before showing the viewport (i.e. you will need to setup the viewport manually).

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Example Window", width=500, height=150):
        dpg.add_text("Hello, world")

    dpg.create_viewport(title='Custom Title', width=600, height=200)  # create viewport takes in config options too!

    # must be called before showing viewport
    dpg.set_viewport_small_icon("path/to/icon.ico")
    dpg.set_viewport_large_icon("path/to/icon.ico")

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
