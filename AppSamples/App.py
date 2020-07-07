from sbApp import *
from sbLog import *
from sbInput import *
from sbPlot import *
from sbDraw import *
from sbWidgets import *
import sbConstants
from math import sin, cos

set_log_level(0)

add_menu_bar("MenuBar")

add_menu("Themes")
add_menu_item("Dark", callback = "ThemeCallback")
add_menu_item("Light", callback = "ThemeCallback")
add_menu_item("Classic", callback = "ThemeCallback")
end_menu()

add_menu("Tools")
add_menu_item("Show Logger", callback="show_logger")
add_menu_item("Show About", callback="show_about")
add_menu_item("Show Metrics", callback="show_metrics")
add_menu_item("Show Source", callback="show_source")
add_menu_item("Show Documentation", callback="show_documentation")
add_menu_item("Show Debug", callback="show_debug")
end_menu()

end_menu_bar()

add_button("Widgets##launcher", callback="Launcher")
add_button("Drawing##launcher", callback="Launcher")
add_button("Plotting##launcher", callback="Launcher")
add_button("Logging##launcher", callback="Launcher")
add_button("Asyncronous##launcher", callback="Launcher")

# widget testing
add_window("Widgets", 500, 500, hide=True)

add_button("Get Widget Values", callback="RetrieveValues")

add_tab_bar("Tab Bar##widget")
add_tab("Basic Widgets##widget")

add_button("Button##widget")
add_checkbox("Checkbox##widget")
add_combo("Combo##widget", ("Item 1", "Item 2", "item 3"))
add_radio_button("Radio Button##widget", ("Item 1", "Item 2", "item 3"))
add_listbox("Listbox##widget", ("Item 1", "Item 2", "item 3"))
add_progress_bar("Progress Bar##widget", 0.45, overlay="Progress Bar")

add_text("Text")
add_selectable("Selectable##widget")
add_input_text("Input Text##widget")
add_label_text("Label Text", "value")

add_color_edit3("Color Edit3##widget")
add_color_edit4("Color Edit4##widget")
add_color_picker3("Color Picker3##widget", width=300)
add_color_picker4("Color Picker4##widget", width=300)

add_input_float("Input Float##widget")
add_input_float2("Input Float2##widget")
add_input_float3("Input Float3##widget")
add_input_float4("Input Float4##widget")
add_input_int("Input Int##widget")
add_input_int2("Input Int2##widget")
add_input_int3("Input Int3##widget")
add_input_int4("Input Int4##widget")

add_drag_float("Drag Float##widget")
add_drag_float2("Drag Float2##widget")
add_drag_float3("Drag Float3##widget")
add_drag_float4("Drag Float4##widget")
add_drag_int("Drag Int##widget")
add_drag_int2("Drag Int2##widget")
add_drag_int3("Drag Int3##widget")
add_drag_int4("Drag Int4##widget")

add_slider_float("Slider Float##widget")
add_slider_float2("Slider Float2##widget")
add_slider_float3("Slider Float3##widget")
add_slider_float4("Slider Float4##widget")
add_slider_int("Slider Int##widget")
add_slider_int2("Slider Int2##widget")
add_slider_int3("Slider Int3##widget")
add_slider_int4("Slider Int4##widget")

end_tab()

add_tab("Container Widgets")

add_tree_node("Tree Node1##widget")
for i in range(0, 3):
    add_text("Item" + str(i))
end_tree_node()
add_tree_node("Tree Node2##widget")
for i in range(0, 3):
    add_text("Item" + str(i))
end_tree_node()

add_child("Child##widget", width=220, height=100)
for i in range(0, 10):
    add_text("Item " + str(i) + " belonging to a child")
end_child()

add_same_line(spacing=50)
add_group("Group##widget")
add_text("Group")
for i in range(0, 3):
    add_button("Button" + str(i) + "##widgetgroup")
end_group()

end_tab()
end_tab_bar()

end_window()

# logger testing
add_window("Logging", 500, 500, autosize=True, hide=True)
add_button("Test Logger", callback="LogCallback")
add_same_line(spacing=10)
add_group("LoggingGroup")
add_text("Log Level")
add_radio_button("Log Level##logging", ("Trace", "Debug", "Info", "Warning", "Error", "Off"))
end_group()
end_window()

# plot testing
add_window("Plots, Graphs and Charts", 500, 500, hide=True)
add_tab_bar("PlotTabBar")
add_tab("Plot Widget")
add_button("Plot data", callback="PlotCallback")
add_plot("Plot", "x-axis", "y-axis", height=-1);
end_tab()
add_tab("Simple Plots")
add_simple_plot("Simpleplot1", (0.3, 0.9, 2.5, 8.9), height = 80)
add_simple_plot("Simpleplot2", (0.3, 0.9, 2.5, 8.9), overlay="Overlaying", height=180, histogram=True)
end_tab()
end_tab_bar()
end_window()

# drawing testing
add_window("Drawing API", autosize=True, hide=True)
add_group("Drawing Controls Group")
add_slider_float("X Origin", vertical=True, min_value = -100, max_value=100, default_value=0, callback="UpdateDrawing")
add_same_line(spacing=20)
add_slider_float("Y Origin", vertical=True, min_value = -100, max_value=100, default_value=0, callback="UpdateDrawing")
add_slider_float("X Scale ", vertical=True, max_value=10, default_value=1, callback="UpdateDrawing")
add_same_line(spacing=20)
add_slider_float("Y Scale", vertical=True, max_value=10, default_value=1, callback="UpdateDrawing")
end_group()
add_same_line(spacing=20)
add_drawing("drawing##widget", width=800, height=500)
draw_rectangle("drawing##widget", (0, 500), (800, 0), (255, 0, 0, 255), fill=(0, 0, 25, 255), rounding=12, thickness = 1.0)
draw_line("drawing##widget", (10, 10), (100, 100), (255, 0, 0, 255), 1)
draw_triangle("drawing##widget", (300, 500), (200, 200), (500, 200), (255, 255, 0, 255), thickness = 3.0)
draw_quad("drawing##widget", (50, 50), (150, 50), (150, 150), (50, 150), (255, 255, 0, 255), thickness = 3.0)
draw_text("drawing##widget", (50, 300), "Some Text", color=(255, 255, 0, 255), size=15)
draw_text("drawing##widget", (0, 0), "Origin", color=(255, 255, 0, 255), size=15)
draw_circle("drawing##widget", (400, 250), 50, (255, 255, 0, 255))
draw_polyline("drawing##widget", ((300, 500), (200, 200), (500, 700)), (255, 255, 0, 255))
draw_polygon("drawing##widget", ((363, 471), (100, 498), (50, 220)), (255, 125, 0, 255))
draw_bezier_curve("drawing##widget", (50, 200), (150, 250), (300, 150), (600, 250), (255, 255, 0, 255), thickness = 2.0)
end_window()

# asyncronous testing
add_window("Asyncrounous Testing", hide=True)
add_button("Start Long Process", callback="LongCallback")
add_button("Start Long Asyncronous Process", callback="LongAsyncronousCallback")
end_window()

# callbacks
def UpdateDrawing(sender):
    set_drawing_origin("drawing##widget", get_value("X Origin"), get_value("Y Origin"))
    set_drawing_scale("drawing##widget", get_value("X Scale "), get_value("Y Scale"))

def LongAsyncronousCallback(sender):
    run_async_function("LongCallback2", "some_data", return_handler="ReturnFromLongProcess")

def LongCallback(sender):
    for i in range(0, 10000000):
        pass
    log_info("Done with long process")

def LongCallback2(data):
    for i in range(0, 10000000):
        pass
    log_info("Done with process from " + data)
    return 42

def ReturnFromLongProcess(data):
    log_info("Data returned to main thread: " + str(data))

def Launcher(sender):

    if sender == "Widgets##launcher":
        show_item("Widgets")
    elif sender == "Drawing##launcher":
        show_item("Drawing API")
    elif sender == "Plotting##launcher":
        show_item("Plots, Graphs and Charts")
    elif sender == "Logging##launcher":
        show_item("Logging")
    elif sender == "Asyncronous##launcher":
        show_item("Asyncrounous Testing")

def ThemeCallback(sender):

    if sender == "Dark":
        set_theme("dark")
    elif sender == "Light":
        set_theme("light")
    else:
        set_theme("classic")

def LogCallback(sender):
    show_logger()
    clear_log()
    loglevel = get_value("Log Level##logging")
    set_log_level(loglevel)
    print("Python Print")
    log("Trace Message")
    log_debug("Debug Message")
    log_info("Info Message")
    log_warning("Warning Message")
    log_error("Error Message")

def PlotCallback(sender):

    clear_plot("Plot")

    data1 = []
    for i in range(0, 100):
        data1.append([3.14*i/180, cos(3*3.14*i/180)])

    data2 = []
    for i in range(0, 100):
        data2.append([3.14*i/180, sin(2*3.14*i/180)])

    add_line_series("Plot", "Cos", data1, weight=2)
    add_scatter_series("Plot", "Sin", data2, marker=sbConstants.mvPlotMarker_Circle)

def RetrieveValues(sender):

    show_logger()
    log_info("Checkbox: " + str(get_value("Checkbox##widget")))
    log_info("Combo: " + str(get_value("Combo##widget")))
    log_info("Radio Button: " + str(get_value("Radio Button##widget")))
    log_info("Listbox: " + str(get_value("Listbox##widget")))
    log_info("Progress Bar: " + str(get_value("Progress Bar##widget")))
    log_info("Selectable: " + str(get_value("Selectable##widget")))
    log_info("Input Text: " + str(get_value("Input Text##widget")))
    log_info("Input Float: " + str(get_value("Input Float##widget")))
    log_info("Input Float2: " + str(get_value("Input Float2##widget")))
    log_info("Input Float3: " + str(get_value("Input Float3##widget")))
    log_info("Input Float4: " + str(get_value("Input Float4##widget")))
    log_info("Input Int: " + str(get_value("Input Int##widget")))
    log_info("Input Int2: " + str(get_value("Input Int2##widget")))
    log_info("Input Int3: " + str(get_value("Input Int3##widget")))
    log_info("Input Int4: " + str(get_value("Input Int4##widget")))
    log_info("Drag Float: " + str(get_value("Drag Float##widget")))
    log_info("Drag Float2: " + str(get_value("Drag Float2##widget")))
    log_info("Drag Float3: " + str(get_value("Drag Float3##widget")))
    log_info("Drag Float4: " + str(get_value("Drag Float4##widget")))
    log_info("Drag Int: " + str(get_value("Drag Int##widget")))
    log_info("Drag Int2: " + str(get_value("Drag Int2##widget")))
    log_info("Drag Int3: " + str(get_value("Drag Int3##widget")))
    log_info("Drag Int4: " + str(get_value("Drag Int4##widget")))
    log_info("Slider Float: " + str(get_value("Slider Float##widget")))
    log_info("Slider Float2: " + str(get_value("Slider Float2##widget")))
    log_info("Slider Float3: " + str(get_value("Slider Float3##widget")))
    log_info("Slider Float4: " + str(get_value("Slider Float4##widget")))
    log_info("Slider Int: " + str(get_value("Slider Int##widget")))
    log_info("Slider Int2: " + str(get_value("Slider Int2##widget")))
    log_info("Slider Int3: " + str(get_value("Slider Int3##widget")))
    log_info("Slider Int4: " + str(get_value("Slider Int4##widget")))
    log_info("Color Edit3: " + str(get_value("Color Edit3##widget")))
    log_info("Color Edit4: " + str(get_value("Color Edit4##widget")))
    log_info("Color Picker3: " + str(get_value("Color Picker3##widget")))
    log_info("Color Picker4: " + str(get_value("Color Picker4##widget")))
    log_info("Tab Bar: " + str(get_value("Tab Bar##widget")))
