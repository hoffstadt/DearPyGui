from sbApp import *
from sbLog import *
from sbInput import *
from sbPlot import *
from sbDraw import *
import SandboxTheme

# create some menus
addMenuBar("MenuBar")
addMenu("File")
addMenu("Callbacks")
addMenuItem("callback 1")
addSeperator()
addMenuItem("callback 2")
addMenuItem("callback 3")
endMenu()
endMenu()
addMenu("Edit")
endMenu()
endMenuBar()

# various widgets
addSpacing(10)
addButton("Press me")
addCombo("combo1", ("A", "B", "C"))
addInputText("Testing", hint="a hint")
addInputText( "TestingMul", multiline=True)
addGroup("Group1")
addRadioButton("radiobutton1", ("First Option", "Second Option", "Third Option"))
endGroup()
addSameLine()
addChild("Child1", 300, 200)
addRadioButton("radiobutton2", ("First Option", "Second Option", "Third Option"))
endChild()
addSpacing(10)

addCollapsingHeader("Themes")
addButton("Use Dark")
indent()
addButton("Use Light")
addButton("Use Classic")
unindent()
endCollapsingHeader()

# creating tabs
addTabBar("TabBar1")

addTab("Tab1")
addInputText("Testing1")
addSameLine(0, 20)
addInputText("Testing2")
addColorEdit4("Color1", 1.0, 0, 0, 1.0)
addInputInt("inputint1")
addInputFloat("inputfloat1", default_value=117.0)
addDrawing("drawing1", 110, 110)
drawLine("drawing1", (10, 10), (100, 100), (1, 0, 0, 1), 1)
addCheckbox("Logger", default_value=True)
addCheckbox("OtherWindow", default_value=True)
addCheckbox("checkbox2")
addListbox("listbox1", ("First item", "Second item", "Third item"), default_value=1)
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
addPlot("Plot1", 500, 500);
endTooltip()                     # end tooltip

endTab()

addTab("Tab3")
addPlot("Plot2", 1100, 800);
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

endTabBar()

addWindow("win1", 300, 200)
addInputText("winTesting", hint="a hint")
addInputText("winTestingMul", multiline=True)
endWindow()

# setting call backs
setItemCallback("Press me", "ItemCallback")
setItemCallback("Testing", "ItemCallback")
setItemCallback("Tab1", "ItemCallback")
setItemCallback("callback 1", "ItemCallback")
setItemCallback("callback 2", "ItemCallback")
setItemCallback("callback 3", "ItemCallback")
setItemCallback("Use Dark", "DarkTheme")
setItemCallback("Use Light", "LightTheme")
setItemCallback("Use Classic", "ClassicTheme")
setItemCallback("Logger", "LoggerCallback")
setItemCallback("OtherWindow", "SubWindowCallback")
setItemCallback("listbox1", "ItemCallback")
setItemCallback("combo1", "ItemCallback")

# setting a tip
setItemTip("Button1", "A different tip")

# setting item widths
setItemWidth("Testing1", 200)
setItemWidth("Testing2", 200)

# setting main callback
#setMainCallback("MainCallback")
setMouseDownCallback("MouseDownCallback")
setMouseClickCallback("MouseClickCallback")
setMouseDoubleClickCallback("MouseDoubleClickCallback")
setKeyDownCallback("KeyDownCallback")
setKeyPressCallback("KeyPressCallback")
setKeyReleaseCallback("KeyReleaseClickCallback")

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
    hideItem("Tab2")
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
        TurnOffLogger()
    else:
        TurnOnLogger()

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
    if isKeyPressed(0x25):
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

def KeyReleaseClickCallback(sender):
    LogDebug("KeyReleaseCallback: " + sender)

