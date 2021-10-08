Simple Plots
============

Simple plots take in a list and plot y-axis data against the
number of items in the list. These can be line graphs or histograms
and are demonstrated below

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial", width=500, height=500):
        dpg.add_simple_plot(label="Simpleplot1", default_value=(0.3, 0.9, 0.5, 0.3), height=300)
        dpg.add_simple_plot(label="Simpleplot2", default_value=(0.3, 0.9, 2.5, 8.9), overlay="Overlaying", height=180,
                            histogram=True)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

You can change the simple plot's data using *set_value*.

Here we are using a mouse move handler and each callback that runs will set the
plot data to make it animated!

.. code-block:: python

    import dearpygui.dearpygui as dpg
    from math import sin

    dpg.create_context()


    def update_plot_data(sender, app_data, plot_data):
        mouse_y = app_data[1]
        if len(plot_data) > 100:
            plot_data.pop(0)
        plot_data.append(sin(mouse_y / 30))
        dpg.set_value("plot", plot_data)


    data = []
    with dpg.window(label="Tutorial", width=500, height=500):
        dpg.add_simple_plot(label="Simple Plot", min_scale=-1.0, max_scale=1.0, height=300, tag="plot")

    with dpg.handler_registry():
        dpg.add_mouse_move_handler(callback=update_plot_data, user_data=data)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/wiki_images/simple_plots.PNG