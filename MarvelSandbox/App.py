from sbApp import *
from sbLog import *
from sbInput import *
from sbPlot import *
import SandboxTheme

# create some menus
addMenuBar("MenuBar")
addMenu("MenuBar", "File")
addMenu("File", "Callbacks")
addMenuItem("Callbacks", "callback 1")
addSeperator("Callbacks")
addMenuItem("Callbacks", "callback 2")
addMenuItem("Callbacks", "callback 3")
endMenu("Callbacks")
endMenu("File")
addMenu("MenuBar", "Edit")
endMenu("Edit")
endMenuBar("MenuBar")

# various widgets
addSpacing("", 10)
addButton("", "Press me")
addCombo("", "combo1", ("A", "B", "C"))
addInputText("", "Testing", hint="a hint")
addGroup("", "Group1")
addRadioButton("Group1", "radiobutton1", ("First Option", "Second Option", "Third Option"))
endGroup("Group1")
addSameLine("")
addChild("", "Child1", 300, 200)
addRadioButton("Child1", "radiobutton2", ("First Option", "Second Option", "Third Option"))
endChild("Child1")
addSpacing("", 10)

addCollapsingHeader("", "Themes")
addButton("Themes", "Use Dark")
addButton("Themes", "Use Light")
addButton("Themes", "Use Classic")

# creating tabs
addTabBar("", "TabBar1")

addTab("TabBar1", "Tab1")
addInputText("Tab1", "Testing1")
addSameLine("Tab1", 0, 20)
addInputText("Tab1", "Testing2")
addColorEdit4("Tab1", "Color1", 1.0, 0, 0, 1.0)
addInputInt("Tab1", "inputint1")
addInputFloat("Tab1", "inputfloat1", default_value=117.0)
addCheckbox("Tab1", "Logger", default_value=True)
addCheckbox("Tab1", "checkbox2")
addListbox("Tab1", "listbox1", ("First item", "Second item", "Third item"))
addText("Tab1", "Some awesome regular text")
addText("Tab1", "Some awesome red text", color=(1.0, 0, 0, 1.0), wrap=100)
addLabelText("Tab1", "Output", "color value", color=(0.0, 1.0, 0, 1.0))
endTab("Tab1")

addTab("TabBar1", "Tab2")
addInputText("Tab2", "Testing3")
addSpacing("Tab2", count=10)
addInputText("Tab2", "Testing4")

addTooltip("Testing4", "Tooltip1")         # start tooltip
addButton("Tooltip1", "A Fancy tooltip 1")
addPlot("Tooltip1", "Plot1", 500, 500);
endTooltip("Tooltip1")                     # end tooltip

endTab("Tab2")

addTab("TabBar1", "Tab3")
addPlot("Tab3", "Plot2", 1100, 800);
endTab("Tab3")

endTabBar("TabBar1")

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
setItemCallback("listbox1", "ItemCallback")
setItemCallback("combo1", "ItemCallback")

# setting a tip
setItemTip("Button1", "A different tip")

# setting item widths
setItemWidth("Testing1", 200)
setItemWidth("Testing2", 200)

# setting main callback
#sandbox.setMainCallback("MainCallback")

def ItemCallback(sender):
    print("Called by ", sender)
    print("value is: ", getValue(sender))
    value = getValue("Color1")
    loglevel = getValue("inputint1")
    SetLogLevel(loglevel)
    changeThemeItem("ImGuiCol_Tab", value[0], value[1], value[2], value[3])
    updateTheme()
    Log("log")
    Log(str(getMousePos()))
    LogDebug("log")
    LogInfo("log")
    LogWarning("log")
    LogError("log")

def LoggerCallback(sender):

    value = getValue("Logger")
    if value == 0:
        TurnOffLogger()
    else:
        TurnOnLogger()

def DarkTheme(sender):
    setTheme("dark")
def LightTheme(sender):
    setTheme("light")
def ClassicTheme(sender):
    setTheme("classic")

def MainCallback(sender):
    print(sender);
