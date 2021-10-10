Plots
=====

Plots are composed of multiple components. 

Y-axis:
    This is a container and is the parent of all the data series that are added to the plot. 
    The plot can have multiple Y-axis at one time.

X-axis:
    This is the x data scale.
    
Series:
    These are the containers for the data you wish to display.
    data series need to be added as a child of a Y-axis to be displayed on the plot.
    There are many different types of data series avaliable.
    Series also can contain UI Items that will be displayed when right-clicking the series label in the legend

Legend (optional):
    This is a normal legend and alos allows the user to toggle which data series are visible.


Plots have some functionality built in:
    * Click & Drag on plot: to pan the plot
    * Click & Drag on Axis: to pan the plot in one direction
    * Double Click: scales plot to data
    * Right Click & Drag: to zoom to an area
    * Double Right Click: opens settings
    * Shift + Right Click & Drag: to zoom to an area that fills a current axis
    * Scroll Mouse Wheel: zooms
    * Scroll Mouse Wheel on Axis: zooms only that axis
    * Toggle data sets on the legend to hide them

.. code-block:: python

    import dearpygui.dearpygui as dpg
    from math import sin

    dpg.create_context()

    # creating data
    sindatax = []
    sindatay = []
    for i in range(0, 500):
        sindatax.append(i / 1000)
        sindatay.append(0.5 + 0.5 * sin(50 * i / 1000))

    with dpg.window(label="Tutorial"):
        # create plot
        with dpg.plot(label="Line Series", height=400, width=400):
            # optionally create legend
            dpg.add_plot_legend()

            # REQUIRED: create x and y axes
            dpg.add_plot_axis(dpg.mvXAxis, label="x")
            dpg.add_plot_axis(dpg.mvYAxis, label="y", tag="y_axis")

            # series belong to a y axis
            dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent="y_axis")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()


Updating Series Data
--------------------

You can change a series on a plot by 
    * setting the series value
    * deleting that specific series item from they y-axis and adding it again
    * deleting all the series items from they y-axis and adding that specific series again

.. code-block:: python

    import dearpygui.dearpygui as dpg
    from math import sin, cos

    dpg.create_context()

    sindatax = []
    sindatay = []
    for i in range(0, 500):
        sindatax.append(i / 1000)
        sindatay.append(0.5 + 0.5 * sin(50 * i / 1000))


    def update_series():

        cosdatax = []
        cosdatay = []
        for i in range(0, 500):
            cosdatax.append(i / 1000)
            cosdatay.append(0.5 + 0.5 * cos(50 * i / 1000))
        dpg.set_value('series_tag', [cosdatax, cosdatay])
        dpg.set_item_label('series_tag', "0.5 + 0.5 * cos(x)")

    with dpg.window(label="Tutorial", tag="win"):
        dpg.add_button(label="Update Series", callback=update_series)
        # create plot
        with dpg.plot(label="Line Series", height=400, width=400):
            # optionally create legend
            dpg.add_plot_legend()

            # REQUIRED: create x and y axes
            dpg.add_plot_axis(dpg.mvXAxis, label="x")
            dpg.add_plot_axis(dpg.mvYAxis, label="y", tag="y_axis")

            # series belong to a y axis
            dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent="y_axis", tag="series_tag")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()


Axis Limits
-----------
The following commands can be used to control the plot axes limits

* `set_axis_limits(...)`
* `get_axis_limits(...)`
* `set_axis_limits_auto(...)`
* `fit_axis_data(...)`

An example demonstrating some of this can be found below:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial", width=400, height=400):
        with dpg.group(horizontal=True):
            dpg.add_button(label="fit y", callback=lambda: dpg.fit_axis_data("y_axis"))
            dpg.add_button(label="unlock x limits", callback=lambda: dpg.set_axis_limits_auto("x_axis"))
            dpg.add_button(label="unlock y limits", callback=lambda: dpg.set_axis_limits_auto("y_axis"))
            dpg.add_button(label="print limits x", callback=lambda: print(dpg.get_axis_limits("x_axis")))
            dpg.add_button(label="print limits y", callback=lambda: print(dpg.get_axis_limits("y_axis")))

        with dpg.plot(label="Bar Series", height=-1, width=-1):
            dpg.add_plot_legend()

            # create x axis
            dpg.add_plot_axis(dpg.mvXAxis, label="Student", no_gridlines=True, tag="x_axis")
            dpg.set_axis_limits(dpg.last_item(), 9, 33)
            dpg.set_axis_ticks(dpg.last_item(), (("S1", 11), ("S2", 21), ("S3", 31)))

            # create y axis
            dpg.add_plot_axis(dpg.mvYAxis, label="Score", tag="y_axis")
            dpg.set_axis_limits("y_axis", 0, 110)

            # add series to y axis
            dpg.add_bar_series([10, 20, 30], [100, 75, 90], label="Final Exam", weight=1, parent="y_axis")
            dpg.add_bar_series([11, 21, 31], [83, 75, 72], label="Midterm Exam", weight=1, parent="y_axis")
            dpg.add_bar_series([12, 22, 32], [42, 68, 23], label="Course Grade", weight=1, parent="y_axis")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Custom Axis Labels
------------------

Custom labels can be set per axis using
:py:func:`set_axis_ticks <dearpygui.dearpygui.set_axis_ticks>`. They can be reset with
:py:func:`reset_axis_ticks <dearpygui.dearpygui.reset_axis_ticks>`.
An example can be found below

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial", width=400, height=400):
        with dpg.plot(label="Bar Series", height=-1, width=-1):
            dpg.add_plot_legend()

            # create x axis
            dpg.add_plot_axis(dpg.mvXAxis, label="Student", no_gridlines=True)
            dpg.set_axis_ticks(dpg.last_item(), (("S1", 11), ("S2", 21), ("S3", 31)))

            # create y axis
            dpg.add_plot_axis(dpg.mvYAxis, label="Score", tag="yaxis_tag")

            # add series to y axis
            dpg.add_bar_series([10, 20, 30], [100, 75, 90], label="Final Exam", weight=1, parent="yaxis_tag")
            dpg.add_bar_series([11, 21, 31], [83, 75, 72], label="Midterm Exam", weight=1, parent="yaxis_tag")
            dpg.add_bar_series([12, 22, 32], [42, 68, 23], label="Course Grade", weight=1, parent="yaxis_tag")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Multiple Y Axes
---------------

Plots can contain up to Three Y-axis for different data that needs a different scale.

.. code-block:: python

    import dearpygui.dearpygui as dpg
    from math import sin

    dpg.create_context()

    sindatax = []
    sindatay = []
    for i in range(0, 100):
        sindatax.append(i / 100)
        sindatay.append(0.5 + 0.5 * sin(50 * i / 100))

    with dpg.window(label="Tutorial", width=400, height=400):
        with dpg.plot(label="Multi Axes Plot", height=400, width=-1):
            dpg.add_plot_legend()

            # create x axis
            dpg.add_plot_axis(dpg.mvXAxis, label="x")

            # create y axis 1
            dpg.add_plot_axis(dpg.mvYAxis, label="y1")
            dpg.add_line_series(sindatax, sindatay, label="y1 lines", parent=dpg.last_item())

            # create y axis 2
            dpg.add_plot_axis(dpg.mvYAxis, label="y2")
            dpg.add_stem_series(sindatax, sindatay, label="y2 stem", parent=dpg.last_item())

            # create y axis 3
            dpg.add_plot_axis(dpg.mvYAxis, label="y3 scatter")
            dpg.add_scatter_series(sindatax, sindatay, label="y3", parent=dpg.last_item())

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Annotations
-----------

Annotations can be used to mark locations on a plot.

Annotations are owned by the plot and their coordinates correspond to the 1st y axis.

They are clamped by default.

.. code-block:: python

    import dearpygui.dearpygui as dpg
    from math import sin

    dpg.create_context()


    sindatax = []
    sindatay = []
    for i in range(0, 100):
        sindatax.append(i / 100)
        sindatay.append(0.5 + 0.5 * sin(50 * i / 100))

    with dpg.window(label="Tutorial", width=400, height=400):
        with dpg.plot(label="Annotations", height=-1, width=-1):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="x")
            dpg.add_plot_axis(dpg.mvYAxis, label="y")
            dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=dpg.last_item())

            # annotations belong to the plot NOT axis
            dpg.add_plot_annotation(label="BL", default_value=(0.25, 0.25), offset=(-15, 15), color=[255, 255, 0, 255])
            dpg.add_plot_annotation(label="BR", default_value=(0.75, 0.25), offset=(15, 15), color=[255, 255, 0, 255])
            dpg.add_plot_annotation(label="TR not clampled", default_value=(0.75, 0.75), offset=(-15, -15),
                                    color=[255, 255, 0, 255], clamped=False)
            dpg.add_plot_annotation(label="TL", default_value=(0.25, 0.75), offset=(-15, -15), color=[255, 255, 0, 255])
            dpg.add_plot_annotation(label="Center", default_value=(0.5, 0.5), color=[255, 255, 0, 255])

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Drag Points and Lines
---------------------

Drag lines/points are owned by the plot and their coordinates correspond to the 1st y axis. 
These items can be moved by clicking and dragging. 

You can also set a callback to be ran when they
are interacted with!

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def print_val(sender):
        print(dpg.get_value(sender))

    with dpg.window(label="Tutorial", width=400, height=400):
        with dpg.plot(label="Drag Lines/Points", height=-1, width=-1):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="x")
            dpg.set_axis_limits(dpg.last_item(), -5, 5)
            dpg.add_plot_axis(dpg.mvYAxis, label="y")
            dpg.set_axis_limits(dpg.last_item(), -5, 5)

            # drag lines/points belong to the plot NOT axis
            dpg.add_drag_line(label="dline1", color=[255, 0, 0, 255], default_value=2.0, callback=print_val)
            dpg.add_drag_line(label="dline2", color=[255, 255, 0, 255], vertical=False, default_value=-2, callback=print_val)
            dpg.add_drag_point(label="dpoint1", color=[255, 0, 255, 255], default_value=(1.0, 1.0), callback=print_val)
            dpg.add_drag_point(label="dpoint2", color=[255, 0, 255, 255], default_value=(-1.0, 1.0), callback=print_val)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Querying
--------

Querying allows the user to select a region of the plot by
clicking and dragging the middle mouse button.

Querying requires setting *query* to **True** when creating the plot.

The callback of the plot will run when the plot is being queried.

The query area is sent through the *app_data* argument as *(x_min, x_max, y_min, y_max)*.

It is also possible to poll the plot for the query area by calling:
:py:func:`get_plot_query_area <dearpygui.dearpygui.get_plot_query_area>` and
:py:func:`is_plot_queried <dearpygui.dearpygui.is_plot_queried>`.

Below is an example using the callback

.. code-block:: python

    import dearpygui.dearpygui as dpg
    from math import sin

    dpg.create_context()

    sindatax = []
    sindatay = []
    for i in range(0, 100):
        sindatax.append(i / 100)
        sindatay.append(0.5 + 0.5 * sin(50 * i / 100))

    with dpg.window(label="Tutorial", width=400, height=600):
        dpg.add_text("Click and drag the middle mouse button over the top plot!")


        def query(sender, app_data, user_data):
            dpg.set_axis_limits("xaxis_tag2", app_data[0], app_data[1])
            dpg.set_axis_limits("yaxis_tag2", app_data[2], app_data[3])


        # plot 1
        with dpg.plot(no_title=True, height=200, callback=query, query=True, no_menus=True, width=-1):
            dpg.add_plot_axis(dpg.mvXAxis, label="x")
            dpg.add_plot_axis(dpg.mvYAxis, label="y")
            dpg.add_line_series(sindatax, sindatay, parent=dpg.last_item())

        # plot 2
        with dpg.plot(no_title=True, height=200, no_menus=True, width=-1):
            dpg.add_plot_axis(dpg.mvXAxis, label="x1", tag="xaxis_tag2")
            dpg.add_plot_axis(dpg.mvYAxis, label="y1", tag="yaxis_tag2")
            dpg.add_line_series(sindatax, sindatay, parent="yaxis_tag2")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Custom Context Menus
--------------------

Plot series are actually containers! 

Adding UI Items to a plot series,
they will show up when right-clicking the series in the legend.

.. code-block:: python

    import dearpygui.dearpygui as dpg
    from math import sin

    dpg.create_context()

    sindatax = []
    sindatay = []
    for i in range(0, 100):
        sindatax.append(i / 100)
        sindatay.append(0.5 + 0.5 * sin(50 * i / 100))

    with dpg.window(label="Tutorial", width=400, height=400):
        # create plot
        dpg.add_text("Right click a series in the legend!")
        with dpg.plot(label="Line Series", height=-1, width=-1):
            dpg.add_plot_legend()

            dpg.add_plot_axis(dpg.mvXAxis, label="x")
            dpg.add_plot_axis(dpg.mvYAxis, label="y", tag="yaxis")

            # series 1
            dpg.add_line_series(sindatax, sindatay, label="series 1", parent="yaxis", tag="series_1")
            dpg.add_button(label="Delete Series 1", parent=dpg.last_item(), callback=lambda: dpg.delete_item("series_1"))

            # series 2
            dpg.add_line_series(sindatax, sindatay, label="series 2", parent="yaxis", tag="series_2")
            dpg.add_button(label="Delete Series 2", parent=dpg.last_item(), callback=lambda: dpg.delete_item("series_2"))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Colors and Styles
-----------------

The color and styles of a plot and series can be changed using theme app item

.. seealso::
    For more information on item values :doc:`../documentation/themes`

.. code-block:: python

    import dearpygui.dearpygui as dpg
    from math import sin

    dpg.create_context()

    sindatax = []
    sindatay = []
    for i in range(0, 100):
        sindatax.append(i / 100)
        sindatay.append(0.5 + 0.5 * sin(50 * i / 100))
    sindatay2 = []
    for i in range(0, 100):
        sindatay2.append(2 + 0.5 * sin(50 * i / 100))

    with dpg.window(label="Tutorial", width=500, height=400):
        # create a theme for the plot
        with dpg.theme(tag="plot_theme"):
            with dpg.theme_component(dpg.mvStemSeries):
                dpg.add_theme_color(dpg.mvPlotCol_Line, (150, 255, 0))
                dpg.add_theme_style(dpg.mvPlotStyleVar_Marker, dpg.mvPlotMarker_Diamond, category=dpg.mvThemeCat_Plots)
                dpg.add_theme_style(dpg.mvPlotStyleVar_MarkerSize, 7, category=dpg.mvThemeCat_Plots)

            with dpg.theme_component(dpg.mvScatterSeries):
                dpg.add_theme_color(dpg.mvPlotCol_Line, (60, 150, 200))
                dpg.add_theme_style(dpg.mvPlotStyleVar_Marker, dpg.mvPlotMarker_Square, category=dpg.mvThemeCat_Plots)
                dpg.add_theme_style(dpg.mvPlotStyleVar_MarkerSize, 4, category=dpg.mvThemeCat_Plots)

        # create plot
        with dpg.plot(tag="plot", label="Line Series", height=-1, width=-1):

            # optionally create legend
            dpg.add_plot_legend()

            # REQUIRED: create x and y axes
            dpg.add_plot_axis(dpg.mvXAxis, label="x")
            dpg.add_plot_axis(dpg.mvYAxis, label="y", tag="yaxis")

            # series belong to a y axis
            dpg.add_stem_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent="yaxis", tag="series_data")
            dpg.add_scatter_series(sindatax, sindatay2, label="2 + 0.5 * sin(x)", parent="yaxis", tag="series_data2")

            # apply theme to series
            dpg.bind_item_theme("series_data", "plot_theme")
            dpg.bind_item_theme("series_data2", "plot_theme")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Colormaps
---------

Under construction

Gallery
-------

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/controls.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/dnd.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/pie.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/query.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/bars.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/rt.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/stem.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/markers.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/shaded.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/candle.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/heat.gif

.. image:: https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/tables.gif