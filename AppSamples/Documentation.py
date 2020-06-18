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

commands = []
for item in dir(sbWidgets):
    if not item.startswith("__"):
        commands.append(item)

addListbox("Modules", ("sbApp", "sbLog", "sbInput", "sbPlot", "sbDraw", "sbWidgets"), width=500, height=10)
addListbox("Commands", commands, callback="docCallback", width=500, height=10)
addSpacing()
addChild("DocChild", 300, 200)
addText("Document String", wrap=500)
endChild()
endMainWindow()

def docCallback(sender):
    value = getValue("Commands")
    setValue("Document String", sbWidgets.__dict__[commands[value]].__doc__)
