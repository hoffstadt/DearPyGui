from sbApp import *
from sbLog import *
from sbInput import *
from sbPlot import *
from sbDraw import *
from sbWidgets import *
import sbConstants
import SandboxTheme
from math import sin, cos

####################################################
#############    About All Widgets    ##############
####################################################
# Widgets are items that allow the you to create a GUI. They can be secorators, inputs, containers, MenuBars, Plots, ect. The base inputs to every widget is a "Name". 
# A Name is used to refer to the widget when assigning callbacks, getting and setting values, showing, hiding, and any other functions that can acto on a widget.
# Container widgets are special and when a container widget is added there must always be a paired method to end the container. 

##################################################################################
##############    MenuBars - Creating "File | Themes" Menu Items    ##############
##################################################################################
# NOTES: Menu Bar appears at the top of the app. Only one may be created at a time. Menu Bars have 
# containers called Menus. It is important we remember to calle the end for every container widget we use

#Create the Menu Bar and give it a name. We will call this MenuBar "Menu Bar".
addMenuBar("MenuBar")

#Add the first Menu item to the Menu Bar. We will call this Menu "File".
addMenu("File")

#Adding a Menu inside another Menu will create Sub-Menus. Sub-Menus can be repeated as necessary.
addMenu("Sub-Menu")

#Adding items to the "Sub-Menu" item. Sub Menus are great for grouping similar items.
addMenuItem("Sub-Menu Item 1")

#Horizonal separator lines can be added anywhere to separate items in Sandbox
addSeperator()

#Sprinkel in more sub menu items for a spicy Menu.
addMenuItem("Sub-Menu Item 2")
addMenuItem("Sub-Menu Item 3")

#End the "SubMenu" to close its scope and move back up a directory. Sometimes less is more with menu items.
endMenu()

#Now we can add more items to the "File" menu.
#Setting the callback keywords to a function is how we give widgets functionality.
addMenuItem("Hulk", callback = "HulkAbility")

#Ending an final Menu will allow you to move to the next Menu slot in the MenuBar
endMenu()

#Adding another Menu to the Menu Bar
addMenu("Themes")
addMenuItem("Dark", callback = "DarkTheme")
addMenuItem("Light", callback = "LightTheme")
addMenuItem("Classic")
addMenuItem("Theme Editor")
endMenu()

# standard tools
addMenu("Tools")
addMenuItem("Show Logger", callback="ShowLoggerCallback")
addMenuItem("Show About", callback="ShowAboutCallback")
addMenuItem("Show Metrics", callback="ShowMetricsCallback")
endMenu()

#Dont forget to end the MenuBar
endMenuBar()

#We can even set Callbacks after the widget already exists. This can be done at any point in the code after the item has been created.
setItemCallback("Classic", "ClassicTheme")
setItemCallback("Theme Editor", "OpenThemeEditor")

#Callbacks need python functions to run. These are normal python functions, so script them as your heart desires
def DarkTheme(sender):
    setTheme("dark")

def LightTheme(sender):
    setTheme("light")

def ClassicTheme(sender):
    setTheme("classic")

def ShowMetricsCallback(sender):
    showMetrics()

def ShowAboutCallback(sender):
    showAbout()

def ShowLoggerCallback(sender):
    ShowLogger()

def OpenThemeEditor(sender):
    #this is where the theme editor will be turned on once created. it will run similar to logger except the open theme call back will se the Value to 1. and a new window will appear. 
    #after done editing the user will select "Apply" or "Cancel" and those callbacks will be ran and also the value will be set to 0 and the theme edtor closed.
    value = 0
    if value == 0:
        #CloseThemeEditor
        pass
    else:
        #CloseThemeEditor
        pass

#An advanced function for the "HulkAbility" callback showing that we can edit theme items. Finilize changes by calling "updateTheme". This is only some of the items that can be colored.
def HulkAbility(sender):
    setTheme("Dark")
    hulksColor = [0,50,0,255]
    changeThemeItem(sbConstants.mvGuiCol_Tab, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_Button, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_Header, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_MenuBarBg, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_TitleBg, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_FrameBg, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_TextSelectedBg, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_TabActive, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_ButtonActive, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_TitleBgActive, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    changeThemeItem(sbConstants.mvGuiCol_Text, 100, 0, 0, 255)
    print("Hulk Smash!!!!")

addProgressBar("Progress", 0, overlay="Progress Bar")

#########################################
#############    Inputs    ##############
#########################################
# NOTES: Input Widgets are items that the user can interact with. They can do things like set and get values of variables for things like Floats, Integers, or Strings. 
# They also have the ability initate callbacks.

#collapsable header for orginization
addCollapsingHeader("Inputs")

#each input has a base signature of a name. This is used to interact with the items on the app
addInputText("Text Input", hint="Hint for description goes here (this is not a default string).")
addInputText("Text Input with Multiline", multiline=True)
addInputInt("Input Integer 1", default_value=117)
addInputFloat("Input Float 1", default_value=117.0)
addRadioButton("Radio Button", ("First Option", "Second Option", "Third Option"), default_value=2)
addListbox("Listbox", ("First item", "Second item", "Third item"), default_value=2, height=4)
addCheckbox("Checkbox", default_value=True)
addColorEdit4("Color Edit 4", 50, 100, 56, 255)
addCombo("Combo", ('A', '5', "1.0", "Text"))
addSelectable("Selectible item 1")
addSelectable("Selectible item 2",True)
addDragFloat("Drag Float", default_value=117.0)
addSliderFloat("SliderFloat", default_value=0.75, max_value=10, format="%2.5f", vertical=True)
addSliderInt("SliderInt", default_value=70, max_value=1000)
addSliderFloat4("SliderFloat4")
addSliderInt4("SliderInt4")
addDragInt("DragInt")
addDragInt4("DragInt4")
addButton("Submit All Inputs", callback = "GetAllInputsAndPrint")

#ending Collapsable header
endCollapsingHeader()

def GetAllInputsAndPrint(sender):

    setValue("Input Float 1", 6969)

    print('\n')

    print('Submit All Inputs: Has ran the callback "GetAndPrint"')

    input1 = getValue("Text Input")
    input2 = getValue("Text Input with Multiline")
    input3 = getValue("Input Integer 1")
    input4 = getValue("Input Float 1")
    input5 = getValue("Radio Button")
    input6 = getValue("Listbox")
    input7 = getValue("Checkbox")
    input8 = getValue("Color Edit 4")
    input9 = getValue("Combo")
    input10 = getValue("Selectible item 1")
    input11 = getValue("Drag Float")
    input12 = getValue("SliderFloat")
    input13 = getValue("SliderFloat4")
    input14 = getValue("SliderInt")
    input15 = getValue("SliderInt4")

    print("Text Widget: " + input1)
    print("Text Input Widget with Multiline: ", input2)
    print("Input Integer 1: " + str(input3))
    print("Input Float 1: %1.2f" % input4)
    print("Radio Button: %i" % input5)
    print("Listbox: {}".format(input6))
    if input7 == True:
        print("Checkbox: True")
    else:
        print("Checkbox: False")
    print("Color Edit 4: {0}, {1}, {2}, {3}".format(input8[0], input8[1], input8[2], input8[3]))
    print("Combo: " + input9)

    if(input10):
        print("Selectable item 1 : Selected")
    else:
        print("Selectable item 1 : Not Selected")
    if(getValue("Selectible item 2")):
        print("Selectable item 1 : Selected")
    else:
        print("Selectable item 1 : Not Selected")

    print("Drag Float: " + str(input11))
    print("SliderFloat: " + str(input12))
    print("SliderFloat4: " + str(input13))
    print("SliderInt: " + str(input14))
    print("SliderInt4: " + str(input15))

    print('\n')

    for i in range(0, 10000000):
        pass
    print("done")

#############################################
#############    Containers    ##############
#############################################
# NOTES: although any widgets can be placed in containers.
#Containers are useful because setting formatting or attributes for a Container will propagate to the children.
addCollapsingHeader("Container")

addGroup("Group")
addRadioButton("Group Radio Button", ("Group Radio Item 1", "Group Radio Item 2", "Group Radio Item 3" ))
endGroup()

addChild("Child", 200, 100)
addButton("Child Button 1")
addButton("Child Button 2")
endChild()

addTreeNode("First Node")
for i in range(0, 2):
    addText("First Node Item: " + str(i))
endTreeNode()
addTreeNode("Second Node")
for i in range(0, 2):
    addText("Second Node Item: " + str(i))
addTreeNode("Inner Second Node")
for i in range(0, 2):
    addText("InnerSecond Node Item: " + str(i))
endTreeNode()
endTreeNode()

endCollapsingHeader()

######################################
#############    Text   ##############
######################################
# NOTES:

addCollapsingHeader("Text")

addText("Regular text")
addText("Regular text with wrapping ability for long lines of text", wrap = 150)
addText("Regular text with bullet style", bullet = True)
addText("Regular text with color twist", color = (1.0, 0.0, 0.0, 1.0))
addLabelText(" :Label Text 1", "Some old text")
addLabelText(" :Label Text 2", "Some old text with color twist", color = (1.0, 0.0, 0.0, 1.0))

addButton("Change Text Values", callback = "SetAllLabelText")
endCollapsingHeader()

def SetAllLabelText(sender):

    print()

    print("Called by ", sender)
    print('The callback name is "SetAllLabelText"')

    valueGotten1 = getValue(" :Label Text 1")
    valueGotten2 = getValue(" :Label Text 2")
 
    print("The text being replaced in Label Text 1: " + valueGotten1)
    print("The text being replaced in Label Text 2: " + valueGotten2)

    if (valueGotten1 == "Some old text"):
        value1 = "Some new text"
        setValue(" :Label Text 1", value1)
        setValue(" :Label Text 2", "Some new text with color twist")
    else:
        value1 = "Some old text"
        setValue(" :Label Text 1", value1)
        setValue(" :Label Text 2", "Some old text with color twist")

    print()
    
###########################################################
#############   Decorators and Formatting   ##############
###########################################################
# NOTES: Decrators and Formatting can be applied to a single of items or to a container of items

addCollapsingHeader("Decorators and Formatting")
addButton("Normal Button")
addSameLine(spacing=10)
addButton("Button on same line")
addButton("Normal Button again##1")
indent()
addButton("Button with an indent")
unindent()
addButton("Normal Button again##2")
addSpacing(5)
addButton("Button with a vertical spacing above it")
addButton("Normal Button again")
addSeperator()
addButton("Normal Button with a Seperator above it")
endCollapsingHeader()

########################################
#############    Logger   ##############
########################################
# NOTES:

#Any command placed in the main loop will be ran on start up. For debuggin purposes we will always open the logger.
ShowLogger()

addCollapsingHeader("Logger")
# log level controll what level of logging the logger with print. 
# 0 prints all levels. 1 prints debug or greater. 2 prints log info or greater. ect 
addInputInt("Log Level", default_value = 0)
addButton("Log Examples", callback = "AllLogExamples")
addButton("Clear Log", callback = "clearsLog")
endCollapsingHeader()

def AllLogExamples(sender):
    print()
    print("Called by ", sender)
    #print("value is: ", getValue(sender))
    loglevel = getValue("Log Level")
    print(loglevel)
    SetLogLevel(loglevel)
    Log("log")
    Log("Mouse Click Coord: " + str(getMousePos()))
    LogDebug("log")
    LogInfo("log")
    LogWarning("log")
    LogError("log")
    print()

def clearsLog(sender):
    ClearLog()

#################################################
#############    Drawing and Canvas    ##########
#################################################
# NOTES: 
addCollapsingHeader("Drawing Canvas")

addButton("draw on canvas", callback="DrawCanvas")
addButton("clear canvas", callback="ClearCanvas")

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

endCollapsingHeader()

def DrawCanvas(sender):
    drawRectangle("drawing2", (0, 0), (800, 500), (1, 0, 0, 1), fill=(0, 0, 0.1, 1), rounding=12, thickness = 1.0)
    drawRectangle("drawing2", (0, 0), (800, 500), (1, 0, 0, 1), fill=(0, 0, 0.1, 1), rounding=12, thickness = 1.0)
    drawLine("drawing2", (10, 10), (100, 100), (1, 0, 0, 1), 1)
    drawCircle("drawing2", (400, 250), 50, (1, 1, 0,1))
    drawBezierCurve("drawing2", (50, 200), (150, 250), (300, 150), (600, 250), (1, 1, 0, 1), thickness = 2.0)
def ClearCanvas(sender):
    clearDrawing("drawing2")

##########################################################
#############    Plots, Graphs and Charts   ##############
##########################################################
# NOTES: 
addCollapsingHeader("Plots, Graphs and Charts")
addButton("Plot data", callback="PlotCallback", tip="new tip")
addSimplePlot("Simpleplot1", (0.3, 0.9, 2.5, 8.9), height = 80)
addSimplePlot("Simpleplot2", (0.3, 0.9, 2.5, 8.9), overlay="Overlaying", height=180, histogram=True)
addPlot("Plot2", "x-axis", "y-axis", -1, 450);
endCollapsingHeader()

def PlotCallback(sender):
    data1 = []
    for i in range(0, 100):
        data1.append([3.14*i/180, cos(3*3.14*i/180)])

    data2 = []
    for i in range(0, 100):
        data2.append([3.14*i/180, sin(2*3.14*i/180)])

    addTextPoint("Plot2", "Here", 1.0, 1.0)

    addLineSeries("Plot2", "Cos", data1, color=(1,0,1), weight=2)
    addScatterSeries("Plot2", "Sin", data2, marker=sbConstants.mvPlotMarker_Circle, fill=(1,0,0), outline=(1,1,0))


###########################################
#############    Tool Tips   ##############
###########################################
# NOTES: 
# setting a tip
#setItemTip("Button1", "A different tip")

# setting item widths
#setItemWidth("Testing1", 200)
#setItemWidth("Testing2", 200)

#######################################################
#############    Interactive Callbacks   ##############
#######################################################
# NOTES:


# setting main callback
setMainCallback("MainCallback")
def MainCallback(sender):
    value = getValue("Progress")
    value = value + 0.01
    if value >= 1:
        value = 0
    setValue("Progress", value)

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

endMainWindow()