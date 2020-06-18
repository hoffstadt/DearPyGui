from sbApp import *
from sbLog import *
from sbInput import *
from sbPlot import *
from sbDraw import *
from sbWidgets import *
import sbConstants
from math import sin, cos

# create some menus
addMenuBar("MenuBar")
addMenu("Tools")
addMenuItem("Show Logger", callback="ShowLoggerCallback")
addMenuItem("Show About", callback="ShowAboutCallback")
addMenuItem("Show Metrics", callback="ShowMetricsCallback")
endMenu()
endMenuBar()

addButton("Plot data", callback="PlotCallback")
addButton("Resize plot", callback="ResizePlotCallback")
addPlot("Plot2", "x-axis", "y-axis", -1, 450);

addListbox("Colormaps", ("Default", "Dark", "Pastel", "Paired", "Viridis", "Plasma", "Hot", "Cool", "Pink", "Jet"), width=500, height=10, callback="colormapCallback")

endMainWindow()

def colormapCallback(sender):
    value = getValue("Colormaps")
    setColorMap("Plot2", value)

def ShowMetricsCallback(sender):
    showMetrics()
def ShowAboutCallback(sender):
    showAbout()
def ShowLoggerCallback(sender):
    ShowLogger()

def PlotCallback(sender):
    clearPlot("Plot2")
    data1 = []
    for i in range(0, 100):
        data1.append([3.14*i/180, cos(3*3.14*i/180)])

    data2 = []
    for i in range(0, 100):
        data2.append([3.14*i/180, sin(2*3.14*i/180)])
    addLineSeries("Plot2", "Cos", data1, style=( 
        (sbConstants.mvPlotStyleVar_Marker, sbConstants.mvPlotMarker_None), ))
    addScatterSeries("Plot2", "Sin", data2, style=( 
        (sbConstants.mvPlotStyleVar_Marker, sbConstants.mvPlotMarker_Asterisk),
        (sbConstants.mvPlotStyleVar_MarkerSize, 4), ))

def ResizePlotCallback(sender):
    changePlotSize("Plot2", 640, 400)
