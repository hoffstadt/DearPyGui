Render Loop
===========

The render loop run continuously and goes to each and every item in your program and draws it.

Drawing the items is how the dpg appears to update items. DPG does this at the rate of your monitor refresh when 
:py:func:`set_viewport_vsync <dearpygui.set_viewport_vsync>`
is turned on and if vsync is turned off then the render looop will run as fast a possible which typically not ideal.

Running python inside the render loop may greatly reduce 
the frame rate of your application if the functions are computationally expensive.

It is also not recommended to do things like `sleep(1)` in the render loop. 
Although you could introduce a frame rate cap by limiting the speed of the render loop.

For most use cases the render loop does not need to be considered
and is completely handled by :py:func:`start_dearpygui <dearpygui.start_dearpygui>`.

For more advanced use cases full access to the render loop can be accessed like so:

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