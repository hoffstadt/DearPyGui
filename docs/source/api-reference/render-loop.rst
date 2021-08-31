Render Loop
===========

Basics
------

For most use cases the render loop does not need to be considered
and is completely handled by :py:func:`start_dearpygui <dearpygui.dearpygui.show_documentation>.

Manual Render Loop
------------------

For more advanced use cases full access to the render loop can be accessed like so:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.setup_viewport()

    with dpg.window(label="Example Window", width=500, height=150):
        dpg.add_text("Hello, world")


    # below replaces, start_dearpygui()

    while dpg.is_dearpygui_running():
        # you can manually stop by using stop_dearpygui()
        dpg.render_dearpygui_frame()

    dpg.cleanup_dearpygui()
