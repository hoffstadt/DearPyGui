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
addMenuItem("Show Source", callback="ShowSourceCallback")
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
    showLogger()
def ShowSourceCallback(sender):
    showSource()

def PlotCallback(sender):
    clearPlot("Plot2")
    data1 = []
    for i in range(0, 100):
        data1.append([3.14*i/180, cos(3*3.14*i/180)])

    data2 = []
    for i in range(0, 100):
        data2.append([3.14*i/180, sin(2*3.14*i/180)])

    addTextPoint("Plot2", "Here", 1.0, 1.0)
    addLineSeries("Plot2", "Cos", data1, color=(255,0,255), weight=2)
    addScatterSeries("Plot2", "Sin", data2, marker=sbConstants.mvPlotMarker_Circle, fill=(255,0,0), outline=(255,255,0))

def ResizePlotCallback(sender):
    changePlotSize("Plot2", 640, 400)
