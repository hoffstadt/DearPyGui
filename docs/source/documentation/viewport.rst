Viewport
========

The viewport is what you traditionally call the window in other GUI libraries.
In the case of Dear PyGui, we distinguish between the operating system window
and the Dear PyGui window by referring to the former as _the viewport_.

Before calling :py:func:`start_dearpygui <dearpygui.dearpygui.start_dearpygui>`, you must do the following:
1. Create a viewport, using :py:func:`create_viewport <dearpygui.dearpygui.create_viewport>`.
2. Assign the viewport, using :py:func:`setup_dearpygui <dearpygui.dearpygui.setup_dearpygui>`.
3. Show the viewport, using :py:func:`show_viewport <dearpygui.dearpygui.show_viewport>`.

As a convenience, we offer the command, :py:func:`setup_viewport <dearpygui.dearpygui.setup_viewport>`
which handles the above setup steps.

Once the viewport has been created, you can begin
configuring the viewport using :py:func:`configure_viewport <dearpygui.dearpygui.configure_viewport>`
or the helper commands *set_viewport_**.

.. note::
    Large/small icon must be set before showing the viewport (i.e. you will need to setup the viewport manually).

.. code-block:: python

    import dearpygui.dearpygui as dpg

    with dpg.window(label="Example Window", width=300):
        dpg.add_text("Hello, world")
        dpg.add_input_text(label='Viewport Title', callback=lambda sender, value: dpg.set_viewport_title(title=value))

    dpg.setup_viewport()
    dpg.set_viewport_title(title='Custom Title')
    dpg.set_viewport_width(500)
    dpg.set_viewport_height(200)

    dpg.start_dearpygui()

Manual Viewport
---------------

A more general approach to viewport creation and modification can be seen below. This is currently required if you need to set the icons.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    vp = dpg.create_viewport(title='Custom Title', width=600, height=200) # create viewport takes in config options too!

    # must be called before showing viewport
    dpg.set_viewport_small_icon("path/to/icon.ico")
    dpg.set_viewport_large_icon("path/to/icon.ico")

    with dpg.window(label="Example Window", width=500, height=150):
        dpg.add_text("Hello, world")

    dpg.setup_dearpygui(viewport=vp)
    dpg.show_viewport(vp)
    dpg.start_dearpygui()

