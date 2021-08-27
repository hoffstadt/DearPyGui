| [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Plots) - [Basics](#basics) - [Tips](#tips) - [Updating Series](#updating-series) - [Axis Limits](#axis-limits) - [Custom Axis Labels](#custom-axis-labels) - [Multiple Y Axes](#multiple-y-axes) - [Annotations](#annotations) - [Drag Points & Lines](#drag-points-and-lines) - [Querying](#querying) - [Custom Context Menus](#custom-context-menus) - [Colors & Styles](#colors-and-styles) - [Colormaps](#colormaps) - [Gallery](#gallery) |
| ---- |
||
## Basics
Plots are composed of multiple components. These include plot axes, data series, and an optional legend. Below is the minimal example for creating a plot:
```python
import dearpygui.dearpygui as dpg
from math import sin

sindatax = []
sindatay = []
for i in range(0, 100):
    sindatax.append(i/100)
    sindatay.append(0.5 + 0.5*sin(50*i/100))

with dpg.window(label="Tutorial"):

    # create plot
    with dpg.plot(label="Line Series", height=400, width=400):

        # optionally create legend
        dpg.add_plot_legend()

        # REQUIRED: create x and y axes
        dpg.add_plot_axis(dpg.mvXAxis, label="x")
        dpg.add_plot_axis(dpg.mvYAxis, label="y", id="y_axis")

        # series belong to a y axis
        dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent="y_axis")

dpg.start_dearpygui()
```

## Tips

* Click & Drag: to pan the plot
* Click & Drag on Axis: to pan the plot in one direction
* Double Click: scales plot to data
* Right Click & Drag: to zoom to an area
* Double Right Click: opens settings
* Shift + Right Click & Drag: to zoom to an area that fills a current axis
* Scroll Mouse Wheel: zooms
* Scroll Mouse Wheel on Axis: zooms only that axis
* Toggle data sets on the legend to hide them


## Updating Series
You can update the series on a plot by either adding a series using the same name or by clearing the plot. This is shown below:

```python
import dearpygui.dearpygui as dpg
from math import sin, cos

sindatax = []
sindatay = []
for i in range(0, 100):
    sindatax.append(i/100)
    sindatay.append(0.5 + 0.5*sin(50*i/100))

def update_series():

    cosdatax = []
    cosdatay = []
    for i in range(0, 100):
        cosdatax.append(i/100)
        cosdatay.append(0.5 + 0.5*cos(50*i/100))
    dpg.set_value("series_id", [cosdatax, cosdatay])

with dpg.window(label="Tutorial"):

    dpg.add_button(label="Update Series", callback=update_series)

    with dpg.plot(label="Line Series", height=400, width=400):
        dpg.add_plot_axis(dpg.mvXAxis, label="x")
        dpg.add_plot_axis(dpg.mvYAxis, label="y")
        dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=dpg.last_item(), id="series_id")

dpg.start_dearpygui()
```

## Axis Limits
The following commands can be used to control the plot axes limits:
* `set_axis_limits(...)`
* `get_axis_limits(...)`
* `set_axis_limits_auto(...)`
* `fit_axis_data(...)`

An example demonstrating some of this can be found below:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Tutorial", width=400, height=400):
    
    with dpg.group(horizontal=True):
        dpg.add_button(label="fit y", callback=lambda: dpg.fit_axis_data("yaxis"))
        dpg.add_button(label="unlock x limits", callback=lambda: dpg.set_axis_limits_auto("xaxis"))
        dpg.add_button(label="unlock y limits", callback=lambda: dpg.set_axis_limits_auto("yaxis"))
        dpg.add_button(label="print limits x", callback=lambda: print(dpg.get_axis_limits("xaxis")))
        dpg.add_button(label="print limits y", callback=lambda: print(dpg.get_axis_limits("yaxis")))

    with dpg.plot(label="Bar Series", height=-1, width=-1):
        dpg.add_plot_legend()

        # create x axis
        dpg.add_plot_axis(dpg.mvXAxis, label="Student", no_gridlines=True, id="xaxis")
        dpg.set_axis_limits(dpg.last_item(), 9, 33)
        dpg.set_axis_ticks(dpg.last_item(), (("S1", 11), ("S2", 21), ("S3", 31)))

        # create y axis
        dpg.add_plot_axis(dpg.mvYAxis, label="Score", id="yaxis")
        dpg.set_axis_limits("yaxis", 0, 110)

        # add series to y axis
        dpg.add_bar_series([10, 20, 30], [100, 75, 90], label="Final Exam", weight=1, parent="yaxis")
        dpg.add_bar_series([11, 21, 31], [83, 75, 72], label="Midterm Exam", weight=1, parent="yaxis")
        dpg.add_bar_series([12, 22, 32], [42, 68, 23], label="Course Grade", weight=1, parent="yaxis")

dpg.start_dearpygui()
```

## Custom Axis Labels
Custom labels can be set per axis using `set_axis_ticks(...)`. They can be reset with `reset_axis_ticks(...)`. An example can be found below:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Tutorial", width=400, height=400):

    with dpg.plot(label="Bar Series", height=-1, width=-1):
        
        dpg.add_plot_legend()

        # create x axis
        dpg.add_plot_axis(dpg.mvXAxis, label="Student", no_gridlines=True)
        dpg.set_axis_ticks(dpg.last_item(), (("S1", 11), ("S2", 21), ("S3", 31)))

        # create y axis
        dpg.add_plot_axis(dpg.mvYAxis, label="Score", id="yaxis_id")

        # add series to y axis
        dpg.add_bar_series([10, 20, 30], [100, 75, 90], label="Final Exam", weight=1, parent="yaxis_id")
        dpg.add_bar_series([11, 21, 31], [83, 75, 72], label="Midterm Exam", weight=1, parent="yaxis_id")
        dpg.add_bar_series([12, 22, 32], [42, 68, 23], label="Course Grade", weight=1, parent="yaxis_id")

dpg.start_dearpygui()
```

## Multiple Y Axes
In _Dear PyGui_ you can have up to 3 Y axes. Below is an example:
```python
import dearpygui.dearpygui as dpg
from math import sin

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

dpg.start_dearpygui()
```
## Annotations
Annotations can be used to mark locations on a plot. They do NOT belong to an axis in the same manner that series do. They are owned by the plot. The coordinates correspond to the 1st y axis. They are clamped by default. Below is an example:
```python
import dearpygui.dearpygui as dpg
from math import sin

sindatax = []
sindatay = []
for i in range(0, 100):
    sindatax.append(i/100)
    sindatay.append(0.5 + 0.5*sin(50*i/100))

with dpg.window(label="Tutorial", width=400, height=400):

    with dpg.plot(label="Annotations", height=-1, width=-1):

        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="x")
        dpg.add_plot_axis(dpg.mvYAxis, label="y")
        dpg.add_line_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent=dpg.last_item())

        # annotations belong to the plot NOT axis
        dpg.add_plot_annotation(label="BL", default_value=(0.25, 0.25), offset=(-15, 15), color=[255, 255, 0, 255])
        dpg.add_plot_annotation(label="BR", default_value=(0.75, 0.25), offset=(15, 15), color=[255, 255, 0, 255])
        dpg.add_plot_annotation(label="TR not clampled", default_value=(0.75, 0.75), offset=(-15, -15), color=[255, 255, 0, 255], clamped=False)
        dpg.add_plot_annotation(label="TL", default_value=(0.25, 0.75), offset=(-15, -15), color=[255, 255, 0, 255])
        dpg.add_plot_annotation(label="Center", default_value=(0.5, 0.5), color=[255, 255, 0, 255])

dpg.start_dearpygui()
```

## Drag Points and Lines
Similar to annotations, drag lines/points belong to the plot and the values correspond to the 1st y axis. These items can be moved by clicking and dragging. You can also set a callback to be ran when they are interacted with! Below is a simple example:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Tutorial", width=400, height=400):

    with dpg.plot(label="Drag Lines/Points", height=-1, width=-1):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="x")
        dpg.set_axis_limits(dpg.last_item(), -5, 5)
        dpg.add_plot_axis(dpg.mvYAxis, label="y")
        dpg.set_axis_limits(dpg.last_item(), -5, 5)

        # drag lines/points belong to the plot NOT axis
        dpg.add_drag_line(label="dline1", color=[255, 0, 0, 255], default_value=2.0)
        dpg.add_drag_line(label="dline2", color=[255, 255, 0, 255], vertical=False, default_value=-2)
        dpg.add_drag_point(label="dpoint1", color=[255, 0, 255, 255], default_value=(1.0, 1.0))
        dpg.add_drag_point(label="dpoint2", color=[255, 0, 255, 255], default_value=(-1.0, 1.0))

dpg.start_dearpygui()
```

## Querying
Querying allows the user to select a region of the plot by clicking and dragging the middle mouse button. Querying requires setting `query` to **True** when creating the plot. If you would like to be notified when the user is querying, you just set the callback of the plot. DPG will send the query area through the `app_data` argument as `(x_min, x_max, y_min, y_max)`. Alternatively, you can poll the plot for the query area by calling: `get_plot_query_area(...)` and `is_plot_queried(...)`. Below is an example using the callback:

```python
import dearpygui.dearpygui as dpg
from math import sin

sindatax = []
sindatay = []
for i in range(0, 100):
    sindatax.append(i/100)
    sindatay.append(0.5 + 0.5*sin(50*i/100))

with dpg.window(label="Tutorial", width=400, height=400):

    dpg.add_text("Click and drag the middle mouse button!")
    def query(sender, app_data, user_data):
        dpg.set_axis_limits("xaxis_id2", app_data[0], app_data[1])
        dpg.set_axis_limits("yaxis_id2", app_data[2], app_data[3])

    # plot 1
    with dpg.plot(no_title=True, height=400, callback=query, query=True, no_menus=True, width=-1):
        dpg.add_plot_axis(dpg.mvXAxis, label="x")
        dpg.add_plot_axis(dpg.mvYAxis, label="y")
        dpg.add_line_series(sindatax, sindatay, parent=dpg.last_item())

    # plot 2
    with dpg.plot(no_title=True, height=400, no_menus=True, width=-1):
        dpg.add_plot_axis(dpg.mvXAxis, label="x1", id="xaxis_id2")
        dpg.add_plot_axis(dpg.mvYAxis, label="y1", id="yaxis_id2")
        dpg.add_line_series(sindatax, sindatay, parent="yaxis_id2")

dpg.start_dearpygui()
```

## Custom Context Menus
Plot series are actually containers! If you add widgets to a plot series, they will show up when you right-click the series in the legend. Below is an example:
```python
import dearpygui.dearpygui as dpg
from math import sin

sindatax = []
sindatay = []
for i in range(0, 100):
    sindatax.append(i/100)
    sindatay.append(0.5 + 0.5*sin(50*i/100))

with dpg.window(label="Tutorial", width=400, height=400):

    # create plot
    dpg.add_text("Right click a series in the legend!")
    with dpg.plot(label="Line Series", height=-1, width=-1):

        dpg.add_plot_legend()

        dpg.add_plot_axis(dpg.mvXAxis, label="x")
        dpg.add_plot_axis(dpg.mvYAxis, label="y", id="yaxis")

        # series 1
        dpg.add_line_series(sindatax, sindatay, label="series 1", parent="yaxis", id="series_1")
        dpg.add_button(label="Delete Series 1", parent=dpg.last_item(), callback=lambda: dpg.delete_item("series_1"))

        # series 2
        dpg.add_line_series(sindatax, sindatay, label="series 2", parent="yaxis", id="series_2")
        dpg.add_button(label="Delete Series 2", parent=dpg.last_item(), callback=lambda: dpg.delete_item("series_2"))

dpg.start_dearpygui()
```

## Colors and Styles
The color and styles of a plot and series can be changed using theme app item (see [this](https://github.com/hoffstadt/DearPyGui/wiki/Themes) for more details). Below is a simple example demonstrating this:
```python
import dearpygui.dearpygui as dpg
from math import sin

sindatax = []
sindatay = []
for i in range(0, 100):
    sindatax.append(i/100)
    sindatay.append(0.5 + 0.5*sin(50*i/100))

with dpg.window(label="Tutorial"):

    # create a theme for the plot
    with dpg.theme(id="plot_theme"):
        dpg.add_theme_color(dpg.mvPlotCol_XAxisGrid, (0, 255, 0), category=dpg.mvThemeCat_Plots)
        dpg.add_theme_style(dpg.mvPlotStyleVar_MarkerSize, 5, category=dpg.mvThemeCat_Plots)

    # create plot
    with dpg.plot(label="Line Series", height=-1, width=-1):

        # apply theme to plot
        dpg.set_item_theme(dpg.last_item(), "plot_theme")

        # optionally create legend
        dpg.add_plot_legend()

        # REQUIRED: create x and y axes
        dpg.add_plot_axis(dpg.mvXAxis, label="x")
        dpg.add_plot_axis(dpg.mvYAxis, label="y", id="yaxis")

        # create a theme for the series
        with dpg.theme(id="series_theme"):
            dpg.add_theme_color(dpg.mvPlotCol_Line, (0, 255, 0), category=dpg.mvThemeCat_Plots)
            dpg.add_theme_style(dpg.mvPlotStyleVar_Marker, dpg.mvPlotMarker_Diamond, category=dpg.mvThemeCat_Plots)

        # series belong to a y axis
        dpg.add_stem_series(sindatax, sindatay, label="0.5 + 0.5 * sin(x)", parent="yaxis")

        # apply theme to series
        dpg.set_item_theme(dpg.last_item(), "series_theme")

dpg.start_dearpygui()
``` 

## Colormaps
Under construction

## Gallery

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/controls.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/dnd.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/pie.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/query.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/bars.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/rt.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/stem.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/markers.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/shaded.gif" width="270">

<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/candle.gif" width="270"> <img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/heat.gif" width="270">
<img src="https://raw.githubusercontent.com/wiki/epezent/implot/screenshots3/tables.gif" width="270">