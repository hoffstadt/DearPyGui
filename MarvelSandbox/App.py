from sbApp import *
from sbLog import *
from sbInput import *
from sbPlot import *
from sbDraw import *
from sbWidgets import *
import sbConstants
import SandboxTheme
from math import sin, cos

# create some menus
addMenuBar("MenuBar")
addMenu("File")
addMenu("Callbacks")
addMenuItem("callback 1", callback="ItemCallbac43k")
addSeperator()
addMenuItem("callback 2", callback="ItemCallback")
addMenuItem("callback 3", callback="ItemCallback")
endMenu()
endMenu()
addMenu("Tools")
addMenuItem("Show Logger", callback="ShowLoggerCallback")
addMenuItem("Show About", callback="ShowAboutCallback")
addMenuItem("Show Metrics", callback="ShowMetricsCallback")
endMenu()
endMenuBar()

addPopup("", "popup7", mousebutton=1)
addText("Popup text Window")
endPopup()


# various widgets
addButton("Press me", callback="ItemCallback")
addPopup("Press me", "popup1", mousebutton=1, modal=True)
addText("Popup text")
addButton("Press me also", callback="CloseModalCallback")
endPopup()
addCombo("combo1", ("A", "B", "C"), callback="ItemCallback")
addInputText("Testing", hint="a hint", callback="ItemCallback")
addInputText( "TestingMul", multiline=True)
addGroup("Group1")
addRadioButton("radiobutton1", ("First Option", "Second Option", "Third Option"))
endGroup()
addSameLine()
addSpacing(10)

addCollapsingHeader("Themes")
addButton("Use Dark", callback="DarkTheme")
indent()
addButton("Use Light", callback="LightTheme")
addButton("Use Classic", callback="ClassicTheme")
unindent()
addChild("Child1", 300, 200)
addRadioButton("radiobutton2", ("First Option", "Second Option", "Third Option"))
addPopup("radiuobutton2", "popup2", mousebutton=1)
addText("RadioButton popup Window")
endPopup()
addSelectable("sel_1")
addSelectable("sel_2")
addSelectable("sel_3", True)
endChild()
endCollapsingHeader()

# creating tabs
addTabBar("TabBar1")

addTab("Tab1", callback="ItemCallback")
addInputText("Testing1", width=200)
addSameLine(0, 20)
addInputText("Testing2", width=300)
addColorEdit4("Color1", 1.0, 0, 0, 1.0)
addInputInt("inputint1")
addInputFloat("inputfloat1", default_value=117.0)
addDrawing("drawing1", 110, 110)
drawLine("drawing1", (10, 10), (100, 100), (1, 0, 0, 1), 1)
addCheckbox("Logger", default_value=True, callback="LoggerCallback")
addCheckbox("OtherWindow", default_value=True, callback="SubWindowCallback")
addCheckbox("checkbox2")
addListbox("listbox1", ("First item", "Second item", "Third item"), default_value=1, callback="ItemCallback")
addText("Some awesome regular text")
addText("Some awesome regular bullet", bullet = True)
addText("Some awesome red text", color=(1.0, 0, 0, 1.0), wrap=100)
addLabelText("Output", "color value", color=(0.0, 1.0, 0, 1.0))
endTab()

addTab("Tab2")
addInputText("Testing3")
addSpacing(10)
addInputText("Testing4")

addTooltip("Testing4", "Tooltip1")         # start tooltip
addButton("A Fancy tooltip 1")
addPlot("Plot1");
endTooltip()                     # end tooltip

addChild("Child2", 300, 200)
addRadioButton("radiobutton23", ("First Option", "Second Option", "Third Option"))
addSelectable("sel_13")
addSelectable("sel_23")
addSelectable("sel_33", True)
endChild()

endTab()

addTab("Tab4")
addSimplePlot("Simpleplot1", (0.3, 0.9, 2.5, 8.9))
addSimplePlot("Simpleplot2", (0.3, 0.9, 2.5, 8.9), True, "Overlaying", 0, 0, 180, True)
endTab()

addTab("DrawingTab")
addDrawing("drawing2", 800, 500)
drawRectangle("drawing2", (0, 0), (800, 500), (1, 0, 0, 1), fill=(0, 0, 0.1, 1), rounding=12, thickness = 1.0)
drawLine("drawing2", (10, 10), (100, 100), (1, 0, 0, 1), 1)
drawTriangle("drawing2", (300, 500), (200, 200), (500, 200), (1, 1, 0, 1), thickness = 3.0)
drawQuad("drawing2", (50, 50), (150, 50), (150, 150), (50, 150), (1, 1, 0, 1), thickness = 3.0)
drawText("drawing2", (50, 300), "Some Text", color=(1, 1, 0,1), size=15)
drawCircle("drawing2", (400, 250), 50, (1, 1, 0,1))
drawPolyline("drawing2", ((300, 500), (200, 200), (500, 700)), (1, 1, 0,1))
drawPolygon("drawing2", ((363, 471), (100, 498), (50, 220)), (1, 0.5, 0,1))
drawBezierCurve("drawing2", (50, 200), (150, 250), (300, 150), (600, 250), (1, 1, 0, 1), thickness = 2.0)
endTab()

addTab("Trees")

addTreeNode("First Node")
for i in range(0, 10):
    addText("First Node Item: " + str(i))
endTreeNode()

addTreeNode("Second Node")
for i in range(0, 10):
    addText("Second Node Item: " + str(i))

addTreeNode("Inner Second Node")
for i in range(0, 10):
    addText("InnerSecond Node Item: " + str(i))
endTreeNode()

endTreeNode()
endTab()

endTabBar()

addWindow("Plotting Window", 500, 500)
addButton("Plot data", callback="PlotCallback", tip="new tip")
addButton("Resize plot", callback="ResizePlotCallback")
addPlot("Plot2", "x-axis", "y-axis", -1, 450);
endWindow()

# setting main callbacks
#setMainCallback("MainCallback")
#setMouseDownCallback("MouseDownCallback")
#setMouseClickCallback("MouseClickCallback")
#setMouseDoubleClickCallback("MouseDoubleClickCallback")
#setKeyDownCallback("KeyDownCallback")
#setKeyPressCallback("KeyPressCallback")
#setKeyReleaseCallback("KeyReleaseClickCallback")

def ShowMetricsCallback(sender):
    showMetrics()
def ShowAboutCallback(sender):
    showAbout()
def ShowLoggerCallback(sender):
    ShowLogger()

def PlotCallback(sender):
    data1 = []
    for i in range(0, 100):
        data1.append([3.14*i/180, cos(3*3.14*i/180)])

    data2 = []
    for i in range(0, 100):
        data2.append([3.14*i/180, sin(2*3.14*i/180)])
    addLineSeries("Plot2", "Simple", data1, style=( (sbConstants.mvPlotStyleVar_Marker, sbConstants.mvPlotMarker_Diamond), ))
    addScatterSeries("Plot2", "Sin", data2)

def ResizePlotCallback(sender):
    changePlotSize("Plot2", 640, 400)

def CloseModalCallback(sender):
    closePopup()

def ItemCallback(sender):
    print("Called by ", sender)
    print("value is: ", getValue(sender))
    value = getValue("Color1")
    loglevel = getValue("inputint1")
    print(loglevel)
    SetLogLevel(loglevel)
    changeThemeItem("ImGuiCol_Tab", value[0], value[1], value[2], value[3])
    updateTheme()
    Log("log")
    Log(str(getMousePos()))
    LogDebug("log")
    LogInfo("log")
    LogWarning("log")
    LogError("log")
    setValue("Output", "234")
    setValue("radiobutton1", 1)
    setValue("checkbox2", True)
    #hideItem("Tab2")
    clearDrawing("drawing2")
    #setStyleItem("Al33pha", 0.5)
    updateStyle()

def SubWindowCallback(sender):

    value = getValue("OtherWindow")
    if value == 0:
        hideItem("win1")
    else:
        showItem("win1")

def LoggerCallback(sender):

    value = getValue("Logger")
    if value == 0:
        pass
    else:
        ShowLogger()

def DarkTheme(sender):
    setTheme("dark")
    showItem("Tab2")
def LightTheme(sender):
    hideItem("Plot2")
    setTheme("light")
def ClassicTheme(sender):
    setTheme("classic")

def MainCallback(sender):
    if isMouseButtonPressed(1):
        print("pressed")
    if isKeyPressed(0x25): # left arrow key
        print("key pressed")

def MouseDownCallback(sender, data):
    LogDebug("MouseDownCallback: " + sender + " " + data)

def MouseClickCallback(sender):
    LogDebug("MouseClickCallback: " + sender)

def MouseDoubleClickCallback(sender):
    LogDebug("MouseDoubleClickCallback: " + sender)

def KeyDownCallback(sender, data):
    LogDebug("KeyDownCallback: " + sender + " " + data)

def KeyPressCallback(sender):
    LogDebug("KeyPressCallback: " + sender)
    if isItemHovered("Press me"):
        LogError("button hovered")
    if isKeyPressed(0x25): # left arrow key
        print("key pressed")

def KeyReleaseClickCallback(sender):
    LogDebug("KeyReleaseCallback: " + sender)

