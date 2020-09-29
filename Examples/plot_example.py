from dearpygui.core import *
from dearpygui.simple import *
from math import cos, sin

# callbacks

def colormap_callback(sender, data):
    value = get_value("Colormaps")
    set_color_map("Plot", value)


def plot_callback(sender, data):
    clear_plot("Plot")

    data1 = []
    for i in range(0, 100):
        data1.append([3.14 * i / 180, cos(3 * 3.14 * i / 180)])

    data2 = []
    for i in range(0, 100):
        data2.append([3.14 * i / 180, sin(2 * 3.14 * i / 180)])

    add_line_series("Plot", "Cos", data1, weight=2)
    add_area_series("Plot", "Cos", data1, weight=2, fill=[255, 0, 0, 100], color=[255, 0, 0, 100])
    add_scatter_series("Plot", "Sin", data2)

with tab_bar("PlotTabBar"):
    with tab("Plot Widget"):
        add_text("Tips")
        add_text("Double click plot to scale to data", bullet=True)
        add_text("Right click and drag to zoom to an area", bullet=True)
        add_text("Double right click to open settings", bullet=True)
        add_text("Toggle data sets on the legend to hide them", bullet=True)
        add_text("Click and drag in the plot area to pan", bullet=True)
        add_text("Scroll mouse wheel in the plot area to zoom", bullet=True)
        add_text("Click and drag on an axis to just pan that dimension", bullet=True)
        add_text("Scroll mouse wheel on an axis to just scale that dimension", bullet=True)
        add_button("Plot data", callback=plot_callback)
        add_listbox("Colormaps", items=["Default", "Dark", "Pastel", "Paired", "Viridis",
                                  "Plasma", "Hot", "Cool", "Pink", "Jet"],
                    width=500, num_items=3, callback=colormap_callback)
        add_plot("Plot", height=-1)
    with tab("Simple Plots"):
        add_simple_plot("Simpleplot1", value=[0.3, 0.9, 2.5, 8.9], height=300)
        add_simple_plot("Simpleplot2", value=[0.3, 0.9, 2.5, 8.9], overlay="Overlaying",
                        height=180, histogram=True)

start_dearpygui()