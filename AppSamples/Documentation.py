import sbWidgets
import sbApp
import sbLog
import sbInput
import sbPlot
import sbDraw
from sbApp import *
from sbLog import *
from sbInput import *
from sbPlot import *
from sbDraw import *
from sbWidgets import *
import sbConstants
from math import sin, cos
import inspect

addMenuBar("MenuBar")
addMenu("Tools")
addMenuItem("Show Logger", callback="ShowLoggerCallback")
addMenuItem("Show About", callback="ShowAboutCallback")
addMenuItem("Show Metrics", callback="ShowMetricsCallback")
addMenuItem("Show Source", callback="ShowSourceCallback")
endMenu()
endMenuBar()

def ShowMetricsCallback(sender):
    showMetrics()

def ShowSourceCallback(sender):
    showSource()

def ShowAboutCallback(sender):
    showAbout()

def ShowLoggerCallback(sender):
    showLogger()

constants = []
for item in dir(sbConstants):
    if not item.startswith("__"):
        constants.append(item)

def getCommands(mod):
    commands = []
    for item in dir(mod):
        if not item.startswith("__"):
            commands.append(item)
    return commands

addGroup("ListGroup")
addListbox("Modules", ("sbApp", "sbLog", "sbInput", "sbPlot", "sbDraw", "sbWidgets", "sbConstants"), 
           width=500, height=7, callback="modCallback")
addSpacing()
addListbox("Commands##app", getCommands(sbApp), callback="commandCallback", width=500, height=30)
addListbox("Commands##log", getCommands(sbLog), callback="commandCallback", width=500, height=30)
addListbox("Commands##input", getCommands(sbInput), callback="commandCallback", width=500, height=30)
addListbox("Commands##widgets", getCommands(sbWidgets), callback="commandCallback", width=500, height=30)
addListbox("Commands##plot", getCommands(sbPlot), callback="commandCallback", width=500, height=30)
addListbox("Commands##draw", getCommands(sbDraw), callback="commandCallback", width=500, height=30)
addListbox("Commands##constants", constants, callback="commandCallback", width=500, height=30)
endGroup()

addSameLine()
addChild("DocChild", 500, 600)
addText("Document String", wrap=400, color=(255, 255, 0))
endChild()

hideItem("Commands##log")
hideItem("Commands##input")
hideItem("Commands##widgets")
hideItem("Commands##plot")
hideItem("Commands##draw")
hideItem("Commands##constants")

def modCallback(sender):
    value = getValue("Modules")
    hideItem("Commands##app")
    hideItem("Commands##log")
    hideItem("Commands##input")
    hideItem("Commands##widgets")
    hideItem("Commands##plot")
    hideItem("Commands##draw")
    hideItem("Commands##constants")

    if value == 0:
        showItem("Commands##app")
    elif value == 1:
        showItem("Commands##log")
    elif value == 2:
        showItem("Commands##input")
    elif value == 3:
        showItem("Commands##plot")
    elif value == 4:
        showItem("Commands##draw")
    elif value == 5:
        showItem("Commands##widgets")
    elif value == 6:
        showItem("Commands##constants")

  
def commandCallback(sender):

    value = getValue(sender)
    if sender == "Commands##app":
        setValue("Document String", sbApp.__dict__[getCommands(sbApp)[value]].__doc__)
    elif sender == "Commands##log":
        setValue("Document String", sbLog.__dict__[getCommands(sbLog)[value]].__doc__)
    elif sender == "Commands##input":
        setValue("Document String", sbInput.__dict__[getCommands(sbInput)[value]].__doc__)
    elif sender == "Commands##widgets":
        setValue("Document String", sbWidgets.__dict__[getCommands(sbWidgets)[value]].__doc__)
    elif sender == "Commands##plot":
        setValue("Document String", sbPlot.__dict__[getCommands(sbPlot)[value]].__doc__)
    elif sender == "Commands##draw":
        setValue("Document String", sbDraw.__dict__[getCommands(sbDraw)[value]].__doc__)
    elif sender == "Commands##constants":
        setValue("Document String", "A constant")

