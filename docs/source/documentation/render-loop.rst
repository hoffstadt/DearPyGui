Render Loop
===========

The render loop (or event loop) runs continously and is responsible for polling user input and drawing widgets.

Drawing the items is how the DPG appears to update items. DPG does this at the rate of your monitor refresh when 
:py:func:`set_viewport_vsync <dearpygui.set_viewport_vsync>`
is set **True**. If vsync is set **False** the render loop will run as fast possible.

If you try to run too many computationally expensive operations inside the render loop, you may reduce
the frame rate of your application.

For most use cases the render loop does not need to be considered
and is completely handled by :py:func:`start_dearpygui <dearpygui.start_dearpygui>`.

For more advanced use cases full access to the render loop can be acced like so:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()
    dpg.create_viewport(title='Custom Title', width=600, height=200)
    dpg.setup_dearpygui()

    with dpg.window(label="Example Window"):
        dpg.add_text("Hello, world")

    dpg.show_viewport()

    # below replaces, start_dearpygui()
    while dpg.is_dearpygui_running():
        # insert here any code you would like to run in the render loop
        # you can manually stop by using stop_dearpygui()
        print("this will run every frame")
        dpg.render_dearpygui_frame()

    dpg.destroy_context()