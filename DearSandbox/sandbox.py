import dearpygui.dearpygui as dpg
from math import sin, cos

sindatax = []
sindatay = []
cosdatay = []
for i in range(0, 100):
    sindatax.append(i/100)
    sindatay.append(0.5 + 0.5*sin(50*i/100))
    cosdatay.append(0.5 + 0.5*cos(50*i/100))

with dpg.theme() as theme:
    dpg.add_theme_color(dpg.mvPlotCol_MarkerFill, [255, 0, 255, 255], category=dpg.mvThemeCat_Plots)
    dpg.add_theme_color(dpg.mvPlotCol_MarkerOutline, [255, 0, 255, 255], category=dpg.mvThemeCat_Plots)

with dpg.window(label="Tutorial"):

    with dpg.plot(label="Line Series", height=-1, width=-1):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="x")
        yaxis = dpg.add_plot_axis(dpg.mvYAxis, label="y")


        dpg.add_scatter_series(sindatax, sindatay, parent=yaxis)
        dpg.add_scatter_series(sindatax, cosdatay, parent=yaxis)
        dpg.set_item_theme(dpg.last_item(), theme)

dpg.start_dearpygui()
