from sbApp import *
from sbLog import *
from sbInput import *
from sbPlot import *
from sbDraw import *
from sbWidgets import *
import sbConstants
from math import sin, cos

# create some menus
add_menu_bar("MenuBar")
add_menu("Tools")
add_menu_item("Show Logger", callback="show_logger")
add_menu_item("Show About", callback="show_about")
add_menu_item("Show Metrics", callback="show_metrics")
add_menu_item("Show Source", callback="show_source")
add_menu_item("Show Documentation", callback="show_documentation")
end_menu()
end_menu_bar()

add_button("Plot data", callback="PlotCallback")
add_plot("Plot2", "x-axis", "y-axis", -1, 450);

add_listbox("Colormaps", ("Default", "Dark", "Pastel", "Paired", "Viridis", "Plasma", "Hot", "Cool", "Pink", "Jet"), width=500, height=10, callback="colormapCallback")

def colormapCallback(sender):

    value = get_value("Colormaps")
    set_color_map("Plot2", value)

def PlotCallback(sender):

    clear_plot("Plot2")

    data1 = []
    for i in range(0, 100):
        data1.append([3.14*i/180, cos(3*3.14*i/180)])

    data2 = []
    for i in range(0, 100):
        data2.append([3.14*i/180, sin(2*3.14*i/180)])

    add_text_point("Plot2", "Here", 1.0, 1.0)
    add_line_series("Plot2", "Cos", data1, weight=2)
    add_scatter_series("Plot2", "Sin", data2, marker=sbConstants.mvPlotMarker_Circle)


