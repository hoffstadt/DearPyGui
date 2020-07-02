from sbApp import *
from sbLog import *
from sbInput import *
from sbPlot import *
from sbDraw import *
from sbWidgets import *
from sbExtension import *
import sbConstants
from math import sin, cos

#testme()

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
add_menu_bar("MenuBar")

#Add the first Menu item to the Menu Bar. We will call this Menu "File".
add_menu("File")

#Adding a Menu inside another Menu will create Sub-Menus. Sub-Menus can be repeated as necessary.
add_menu("Sub-Menu")

#Adding items to the "Sub-Menu" item. Sub Menus are great for grouping similar items.
add_menu_item("Sub-Menu Item 1")

#Horizonal separator lines can be added anywhere to separate items in Sandbox
add_seperator()

#Sprinkel in more sub menu items for a spicy Menu.
add_menu_item("Sub-Menu Item 2")
add_menu_item("Sub-Menu Item 3")

#End the "SubMenu" to close its scope and move back up a directory. Sometimes less is more with menu items.
end_menu()

#Now we can add more items to the "File" menu.
#Setting the callback keywords to a function is how we give widgets functionality.
add_menu_item("Hulk", callback = "HulkAbility")

#Ending an final Menu will allow you to move to the next Menu slot in the MenuBar
end_menu()

#Adding another Menu to the Menu Bar
add_menu("Themes")
add_menu_item("Dark", callback = "DarkTheme")
add_menu_item("Light", callback = "LightTheme")
add_menu_item("Classic")
add_menu_item("Theme Editor")
end_menu()

# standard tools
add_menu("Tools")
add_menu_item("Show Logger", callback="show_logger")
add_menu_item("Show About", callback="show_about")
add_menu_item("Show Metrics", callback="show_metrics")
add_menu_item("Show Source", callback="show_source")
add_menu_item("Show Documentation", callback="show_documentation")
add_menu_item("Show Debug", callback="show_debug")
end_menu()

#Dont forget to end the MenuBar
end_menu_bar()

#We can even set Callbacks after the widget already exists. This can be done at any point in the code after the item has been created.
set_item_callback("Classic", "ClassicTheme")
set_item_callback("Theme Editor", "OpenThemeEditor")

#Callbacks need python functions to run. These are normal python functions, so script them as your heart desires
def DarkTheme(sender):
    set_theme("dark")

def LightTheme(sender):
    set_theme("light")

def ClassicTheme(sender):
    set_theme("classic")

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
    set_theme("Dark")
    hulksColor = (0,50,0,255)
    change_theme_item(sbConstants.mvGuiCol_Tab, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_Button, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_Header, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_MenuBarBg, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_TitleBg, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_FrameBg, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_TextSelectedBg, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_TabActive, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_ButtonActive, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_TitleBgActive, hulksColor[0], hulksColor[1], hulksColor[2], hulksColor[3])
    change_theme_item(sbConstants.mvGuiCol_Text, 100, 0, 0, 255)
    print("Hulk Smash!!!!")

add_progress_bar("Progress", 0, overlay="Progress Bar")
add_button("Modal Window")
add_popup("Modal Window", "ModalPopup", modal=True)
add_simple_plot("SimpleplotModal", (0.3, 0.9, 2.5, 8.9), height = 80)
add_button("Close Modal Window", callback="close_popup")
end_popup()

add_button("Hover me")
add_tooltip("Hover me", "tool_tip")
add_simple_plot("SimpleplotTooltip", (0.3, 0.9, 2.5, 8.9), height = 80)
end_tooltip()

#########################################
#############    Inputs    ##############
#########################################
# NOTES: Input Widgets are items that the user can interact with. They can do things like set and get values of variables for things like Floats, Integers, or Strings. 
# They also have the ability initate callbacks.

#collapsable header for orginization
add_collapsing_header("Inputs")

#each input has a base signature of a name. This is used to interact with the items on the app
add_input_text("Text Input", hint="Hint for description goes here (this is not a default string).", uppercase=True)
add_input_text("Text Input with Multiline", multiline=True)
add_input_int("Input Integer 1", default_value=117)
add_input_float("Input Float 1", default_value=117.0)
add_radio_button("Radio Button", ("First Option", "Second Option", "Third Option"), default_value=2)
add_listbox("Listbox", ("First item", "Second item", "Third item"), default_value=2, height=4)
add_checkbox("Checkbox", default_value=True)
add_color_edit3("Color Edit 3", (50, 100, 200))
add_color_edit4("Color Edit 4", (50, 100, 200, 255))
add_color_picker4("Color Picker 4", (50, 100, 200, 255), width=200, height=200)
add_combo("Combo", ('A', '5', "1.0", "Text"))
add_selectable("Selectible item 1")
add_selectable("Selectible item 2",True)
add_drag_float("Drag Float", default_value=117.0)
add_slider_float("SliderFloat", default_value=0.75, max_value=10, format="%2.5f", vertical=True)
add_slider_int("SliderInt", default_value=70, max_value=1000)
add_slider_float4("SliderFloat4")
add_slider_int4("SliderInt4")
add_drag_int("DragInt")
add_drag_int4("DragInt4")
add_button("Submit All Inputs", callback = "GetAllInputsAndPrint")

#ending Collapsable header
end_collapsing_header()

def GetAllInputsAndPrint(sender):

    #setValue("Input Float 1", 6969)

    print('\n')

    print('Submit All Inputs: Has ran the callback "GetAndPrint"')

    input1 = get_value("Text Input")
    input2 = get_value("Text Input with Multiline")
    input3 = get_value("Input Integer 1")
    input4 = get_value("Input Float 1")
    input5 = get_value("Radio Button")
    input6 = get_value("Listbox")
    input7 = get_value("Checkbox")
    input8 = get_value("Color Edit 4")
    input8a = get_value("Color Edit 3")
    input9 = get_value("Combo")
    input10 = get_value("Selectible item 1")
    input11 = get_value("Drag Float")
    input12 = get_value("SliderFloat")
    input13 = get_value("SliderFloat4")
    input14 = get_value("SliderInt")
    input15 = get_value("SliderInt4")

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
    print("Color Edit 3: ", input8a)
    print("Color Edit 4: {0}, {1}, {2}, {3}".format(input8[0], input8[1], input8[2], input8[3]))
    print("Combo: " + input9)

    if(input10):
        print("Selectable item 1 : Selected")
    else:
        print("Selectable item 1 : Not Selected")
    if(get_value("Selectible item 2")):
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
add_collapsing_header("Container")

add_group("Group")
add_radio_button("Group Radio Button", ("Group Radio Item 1", "Group Radio Item 2", "Group Radio Item 3" ))
end_group()

add_child("Child", width=200, height=100)
add_button("Child Button 1")
add_button("Child Button 2")
add_button("Child Button 3")
add_button("Child Button 4")
add_button("Child Button 5")
end_child()

add_tree_node("First Node")
for i in range(0, 2):
    add_text("First Node Item: " + str(i))
end_tree_node()
add_tree_node("Second Node")
for i in range(0, 2):
    add_text("Second Node Item: " + str(i))
add_tree_node("Inner Second Node")
for i in range(0, 2):
    add_text("InnerSecond Node Item: " + str(i))
end_tree_node()
end_tree_node()

end_collapsing_header()

######################################
#############    Text   ##############
######################################
# NOTES:

add_collapsing_header("Text")

add_text("Regular text")
add_text("Regular text with wrapping ability for long lines of text", wrap = 150)
add_text("Regular text with bullet style", bullet = True)
add_text("Regular text with color twist", color = (255, 0, 0, 255))
add_label_text(" :Label Text 1", "Some old text")
add_label_text(" :Label Text 2", "Some old text with color twist", color = (255, 255, 0, 255))

add_button("Change Text Values", callback = "SetAllLabelText")
end_collapsing_header()

def SetAllLabelText(sender):

    print()

    print("Called by ", sender)
    print('The callback name is "SetAllLabelText"')

    valueGotten1 = get_value(" :Label Text 1")
    valueGotten2 = get_value(" :Label Text 2")
 
    print("The text being replaced in Label Text 1: " + valueGotten1)
    print("The text being replaced in Label Text 2: " + valueGotten2)

    if (valueGotten1 == "Some old text"):
        value1 = "Some new text"
        set_value(" :Label Text 1", value1)
        set_value(" :Label Text 2", "Some new text with color twist")
    else:
        value1 = "Some old text"
        set_value(" :Label Text 1", value1)
        set_value(" :Label Text 2", "Some old text with color twist")

    print()
    
###########################################################
#############   Decorators and Formatting   ##############
###########################################################
# NOTES: Decrators and Formatting can be applied to a single of items or to a container of items

add_collapsing_header("Decorators and Formatting")
add_button("Normal Button")
add_same_line(spacing=10)
add_button("Button on same line")
add_button("Normal Button again##1")
add_indent()
add_button("Button with an indent")
unindent()
add_button("Normal Button again##2")
add_spacing(count=5)
add_button("Button with a vertical spacing above it")
add_button("Normal Button again")
add_seperator()
add_button("Normal Button with a Seperator above it")
end_collapsing_header()

########################################
#############    Logger   ##############
########################################
# NOTES:

#Any command placed in the main loop will be ran on start up. For debuggin purposes we will always open the logger.
show_logger()

add_collapsing_header("Logger")
# log level controll what level of logging the logger with print. 
# 0 prints all levels. 1 prints debug or greater. 2 prints log info or greater. ect 
add_input_int("Log Level", default_value = 0)
add_button("Log Examples", callback = "AllLogExamples")
add_button("Clear Log", callback = "clear_log")
end_collapsing_header()

def AllLogExamples(sender):
    print()
    print("Called by ", sender)
    loglevel = get_value("Log Level")
    print(loglevel)
    set_log_level(loglevel)
    log("log")
    log("Mouse Click Coord: " + str(get_mouse_pos()))
    log_debug("log")
    log_info("log")
    log_warning("log")
    log_error("log")
    print()

#################################################
#############    Drawing and Canvas    ##########
#################################################
# NOTES: 
add_collapsing_header("Drawing Canvas")

add_button("draw on canvas", callback="DrawCanvas")
add_button("clear canvas", callback="ClearCanvas")

add_drawing("drawing2", width=800, height=500)
set_drawing_origin("drawing2", 400, 250)
draw_rectangle("drawing2", (-400, 250), (400, -250), (255, 0, 0, 255), fill=(0, 0, 25, 255), rounding=12, thickness = 1.0, tag="background")
draw_line("drawing2", (10, 10), (100, 100), (255, 0, 0, 255), 1)
draw_triangle("drawing2", (300, 500), (200, 200), (500, 200), (255, 255, 0, 255), thickness = 3.0)
draw_quad("drawing2", (50, 50), (150, 50), (150, 150), (50, 150), (255, 255, 0, 255), thickness = 3.0)
draw_text("drawing2", (50, 300), "Some Text", color=(255, 255, 0, 255), size=15)
draw_text("drawing2", (0, 0), "Origin", color=(255, 255, 0, 255), size=15)
draw_circle("drawing2", (400, 250), 50, (255, 255, 0, 255))
draw_polyline("drawing2", ((300, 500), (200, 200), (500, 700)), (255, 255, 0, 255))
draw_polygon("drawing2", ((363, 471), (100, 498), (50, 220)), (255, 125, 0, 255))
draw_bezier_curve("drawing2", (50, 200), (150, 250), (300, 150), (600, 250), (255, 255, 0, 255), thickness = 2.0)


end_collapsing_header()

def DrawCanvas(sender):
    #draw_rectangle("drawing2", (800, 500), (0, 0), (255, 0, 0), fill=(0, 0, 25, 255), rounding=12, thickness = 1.0)
    draw_line("drawing2", (10, 10), (100, 100), (255, 0, 0), 1)
    draw_circle("drawing2", (400, 250), 50, (255, 255, 0))
    draw_bezier_curve("drawing2", (50, 200), (150, 250), (300, 150), (600, 250), (255, 255, 0), thickness = 2.0)

def ClearCanvas(sender):
    clear_drawing("drawing2")

##########################################################
#############    Plots, Graphs and Charts   ##############
##########################################################
# NOTES: 
add_collapsing_header("Plots, Graphs and Charts")
add_button("Plot data", callback="PlotCallback", tip="new tip")
add_simple_plot("Simpleplot1", (0.3, 0.9, 2.5, 8.9), height = 80)
add_simple_plot("Simpleplot2", (0.3, 0.9, 2.5, 8.9), overlay="Overlaying", height=180, histogram=True)
add_plot("Plot2", "x-axis", "y-axis", width=-1, height=450);
set_plot_ylimits("Plot2", -10, 10)
end_collapsing_header()

add_tab_bar("tabbar1", reorderable=True)
for i in range(0, 10):
    add_tab("tab" + str(i), closable=True)
    end_tab()
end_tab_bar()

def PlotCallback(sender):

    delete_item("Themes", True)

    set_plot_ylimits_auto("Plot2")

    clear_plot("Plot2") # without this it crashes?

    data1 = []
    for i in range(0, 100):
        data1.append([3.14*i/180, cos(3*3.14*i/180)])

    data2 = []
    for i in range(0, 100):
        data2.append([3.14*i/180, sin(2*3.14*i/180)])

    add_text_point("Plot2", "Here", 1.0, 1.0)

    add_line_series("Plot2", "Cos", data1, weight=2)
    add_scatter_series("Plot2", "Sin", data2, marker=sbConstants.mvPlotMarker_Circle)

    delete_item("Child Button 1")

    add_child("Runtime Child", width=200, height=100, before="Simpleplot3")
    add_button("Runtime Button 1")
    add_button("Runtime Button 2")
    end_child()

    move_item_down("Logger")

    add_indent(before="draw on canvas")
    unindent(parent="Drawing Canvas")


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

set_mouse_wheel_callback("WheelCallback")
set_mouse_drag_callback("MouseDragCallback", 10)

def WheelCallback(sender, data):

    if is_item_hovered("drawing2"):
        scale = get_drawing_scale("drawing2")
        set_drawing_scale("drawing2", scale[0]+data*0.1, scale[1]+data*0.1)


def MouseDragCallback(sender, data):

    if is_item_hovered("drawing2") and data == 0:
        origin = get_drawing_origin("drawing2")
        delta = get_mouse_drag_delta()
        set_drawing_origin("drawing2", origin[0] + delta[0], origin[1] - delta[1])

# setting main callback
set_main_callback("MainCallback")
def MainCallback(sender):
    value = get_value("Progress")
    value = value + 0.01
    if value >= 1:
        value = 0
    set_value("Progress", value)

    scale = get_drawing_scale("drawing2")
    draw_rectangle("drawing2", (-400/scale[0], 250/scale[1]), (400/scale[0], -250/scale[1]), (255, 0, 0, 255), fill=(int(255*value), 0, 25, 255), rounding=12, thickness = 1.0, tag="background")

    if is_mouse_button_pressed(sbConstants.mvMouseButton_Left) and is_item_hovered("Secondary Window"):
        print("pressed")
    if is_key_pressed(sbConstants.mvKey_Left): # left arrow key
        print("key pressed")

end_window()

add_window("Secondary Window", 300, 500, autosize=True, default_close=True)
add_simple_plot("Simpleplot3", (0.3, 0.9, 2.5, 8.9), height = 80)
add_popup("Simpleplot3", "plot_popup")
add_simple_plot("SimpleplotPop", (0.3, 0.9, 2.5, 8.9), height = 80)
end_popup()
end_window()