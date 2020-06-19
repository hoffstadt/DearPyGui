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

ShowLogger()

def getCommands(mod):
    commands = []
    for item in dir(mod):
        if not item.startswith("__"):
            commands.append(item)
    return commands

addListbox("Modules", ("sbApp", "sbLog", "sbInput", "sbPlot", "sbDraw", "sbWidgets"), 
           width=500, height=10, callback="modCallback")

addListbox("Commands##app", getCommands(sbApp), callback="commandCallback", width=500, height=10)
addListbox("Commands##log", getCommands(sbLog), callback="commandCallback", width=500, height=10)
addListbox("Commands##input", getCommands(sbInput), callback="commandCallback", width=500, height=10)
addListbox("Commands##widgets", getCommands(sbWidgets), callback="commandCallback", width=500, height=10)
addListbox("Commands##plot", getCommands(sbPlot), callback="commandCallback", width=500, height=10)
addListbox("Commands##draw", getCommands(sbDraw), callback="commandCallback", width=500, height=10)
addSpacing()
addChild("DocChild", 300, 200)
addText("Document String", wrap=500)
endChild()
endMainWindow()

hideItem("Commands##log")
hideItem("Commands##input")
hideItem("Commands##widgets")
hideItem("Commands##plot")
hideItem("Commands##draw")

def modCallback(sender):
    value = getValue("Modules")
    print(value)
    hideItem("Commands##app")
    hideItem("Commands##log")
    hideItem("Commands##input")
    hideItem("Commands##widgets")
    hideItem("Commands##plot")
    hideItem("Commands##draw")

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

