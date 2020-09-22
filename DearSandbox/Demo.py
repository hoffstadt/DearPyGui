# if using this file outside of DPG development comment this line out
# and uncomment the next line
from core import *
# from dearpygui.core import *


from time import sleep
from contextlib import contextmanager
from functools import wraps
from math import sin, cos

########################################################################################################################
# Helpers
########################################################################################################################

def add_item_to_check(item):
    add_label_text(item + "##checklist", value="Not Checked", color=[255, 0, 0])


def item_checked(item):
    delete_item(item)
    add_label_text(item + "##checklist", value="Not Checked", color=[255, 0, 0], parent="CompleteChecklistGroup")

########################################################################################################################
# Context Managers : normally in dearpygui.simple
########################################################################################################################

def wrap_container(container):
    @contextmanager
    @wraps(container)
    def container_context(*args,**kwargs):
        try: yield container(*args,**kwargs)
        finally: end()
    return container_context

window = wrap_container(add_window)
menu_bar = wrap_container(add_menu_bar)
menu = wrap_container(add_menu)
child = wrap_container(add_child)
collapsing_header = wrap_container(add_collapsing_header)
group = wrap_container(add_group)
tab_bar = wrap_container(add_tab_bar)
tab = wrap_container(add_tab)
tree_node = wrap_container(add_tree_node)
tooltip = wrap_container(add_tooltip)
popup = wrap_container(add_popup)

########################################################################################################################
# Old Commands
########################################################################################################################

def set_window_pos(window, x, y):
    configure_item(window, x_pos=x, y_pos=y)

def get_window_pos(window):
    config = get_item_configuration(window)
    return [config["xpos"], config["ypos"]]

def set_item_name(item, name):
    configure_item(item, name=name)

def set_item_label(item, label):
    configure_item(item, label=label)

def set_item_popup(item, popup):
    configure_item(item, popup=popup)

def set_item_tip(item, tip):
    configure_item(item, tip=tip)

def show_item(item):
    configure_item(item, show=True)

def get_item_label(item):
    return get_item_configuration(item)["label"]

def get_item_popup(item):
    return get_item_configuration(item)["popup"]

def get_item_tip(item):
    return get_item_configuration(item)["tip"]

def get_item_width(item):
    return get_item_configuration(item)["width"]

def get_item_height(item):
    return get_item_configuration(item)["height"]

def hide_item(item):
    configure_item(item, show=False)

def set_item_width(item, width):
    configure_item(item, width=width)

def set_item_height(item, height):
    configure_item(item, height=height)

def show_item(item):
    configure_item(item, show=True)

def set_drawing_origin(drawing, x, y):
    configure_item(drawing, originx=x, originy=y)

def set_drawing_scale(drawing, x, y):
    configure_item(drawing, scalex=x, scaley=y)

def set_drawing_size(drawing, x, y):
    configure_item(drawing, width=x, height=y)

def get_drawing_origin(drawing):
    config = get_item_configuration(drawing)
    return [config["originx"], config["originy"]]

def get_drawing_scale(drawing):
    config = get_item_configuration(drawing)
    return [config["scalex"], config["scaley"]]

def get_drawing_size(drawing):
    config = get_item_configuration(drawing)
    return [config["width"], config["height"]]


########################################################################################################################
# Settings and Data Storage
########################################################################################################################
set_log_level(0)

add_data("DataStorage1", {"Key": 34})
add_data("DataStorage2", [23, 42, 53, 5])

set_main_window_title("DearPyGui Demo")
set_main_window_size(1000, 800)
set_main_window_pos(0, 0)
add_additional_font("C:/dev/DearPyGui/Resources/NotoSerifCJKjp-Medium.otf", 20)


########################################################################################################################
# Menu
########################################################################################################################
with menu_bar("MenuBar"):

    def ThemeCallback(sender, data):
        set_theme(sender)

    with menu("Themes"):
        add_menu_item("Dark", callback = ThemeCallback, check=True,shortcut="Ctrl + D")
        add_menu_item("Light", callback = ThemeCallback, check=True)
        add_menu_item("Classic", callback = ThemeCallback, check=True)
        add_menu_item("Dark 2", callback = ThemeCallback, check=True)
        add_menu_item("Grey", callback = ThemeCallback, check=True)
        add_menu_item("Dark Grey", callback = ThemeCallback, check=True)
        add_menu_item("Cherry", callback = ThemeCallback, check=True)
        add_menu_item("Purple", callback = ThemeCallback, check=True)
        add_menu_item("Gold", callback = ThemeCallback, check=True)
        add_menu_item("Red", callback = ThemeCallback, check=True)

    with menu("Tools"):
        add_menu_item("Show Logger", callback=show_logger)
        add_menu_item("Show About", callback=show_about)
        add_menu_item("Show Metrics", callback=show_metrics)
        add_menu_item("Show Documentation", callback=show_documentation)
        add_menu_item("Show Debug", callback=show_debug)
        add_menu_item("Show Style Editor", callback=show_style_editor)

    with menu("Oddities"):
        add_button("A Button")
        add_simple_plot("A menu plot", (0.3, 0.9, 2.5, 8.9), height = 80)
        add_table("A Menu Table", ["Column 1", "Column 2", "Column 3", "Column 4"])
        set_item_height("A Menu Table", 200)
        tabledata1 = []
        for i in range(0, 10):
            row = []
            for j in range(0, 4):
                row.append("Item"+str(i)+"-"+str(j))
            tabledata1.append(row)

        set_value("A Menu Table", tabledata1)

########################################################################################################################
# checklist
########################################################################################################################
with child("ChecklistGroup", width=300):
    add_item_to_check("Widgets")
    add_item_to_check("Drawing API")
    add_item_to_check("Plots, Graphs and Charts")
    add_item_to_check("Logging")
    add_item_to_check("Asyncronous")
    add_item_to_check("Input Polling")
    add_item_to_check("Input Text")
    add_item_to_check("Text Widget")
    add_item_to_check("Tooltips/Popups")
    add_item_to_check("Tables")
    add_item_to_check("Open File")
    add_item_to_check("Open Directory")

add_same_line()

with child("CompleteChecklistGroup", width=300):
    pass

add_same_line()

########################################################################################################################
# Launchers
########################################################################################################################
def Launcher(sender, data):
    delete_item(sender + "##checklist")
    add_label_text(sender + "##checklist", value="Checked", color=[0, 255, 0], parent="CompleteChecklistGroup")
    show_item(sender + "##dialog")

def TestFileCallback(sender, data):
    print("Called")
    print(data)

def OpenFile(sender, data):
    open_file_dialog(TestFileCallback, ".*,.py")
    Launcher(sender, data)

def OpenDirectory(sender, data):
    select_directory_dialog(TestFileCallback)
    Launcher(sender, data)

with group("Launch Group", width=200):
    add_button("Widgets", callback=Launcher)
    add_button("Drawing API", callback=Launcher)
    add_button("Plots, Graphs and Charts", callback=Launcher)
    add_button("Logging", callback=Launcher)
    add_button("Asyncronous", callback=Launcher)
    add_button("Input Polling", callback=Launcher)
    add_button("Input Text", callback=Launcher)
    add_button("Text Widget", callback=Launcher)
    add_button("Tooltips/Popups", callback=Launcher)
    add_button("Tables", callback=Launcher)
    add_button("Open File", callback=OpenFile)
    add_button("Open Directory", callback=OpenDirectory)
    #add_image_button("image", "C:/Users/Jonathan Hoffstadt/Desktop/avacado.PNG")
   
#add_image("image1", "C:/dev/DearPyGui/Examples/SpriteMapExample.png")
#add_image_button("image", "C:/dev/DearPyGui/Examples/SpriteMapExample.png")

########################################################################################################################
# Tables
########################################################################################################################
def closeit(sender, data):
    items = get_windows()
    for item in items:
        print(item)
    print("closing tables")

    print(does_item_exist("Widgets"))
    callback = get_item_callback("Widgets")
    callback("Widgets", None)

with window("Tables##dialog", 500, 500, hide=True, on_close=closeit):

    def ClearTable(sender, data):
        clear_table("Table##widget")

    def DeleteRow(sender, data):
        delete_row("Table##widget", 6)

    def DeleteCol(sender, data):
        delete_column("Table##widget", 1)

    def AddRow(sender, data):
        add_row("Table##widget", ["new1", "new2", "new3", 53])

    def AddCol(sender, data):
        add_column("Table##widget", "New Column", ["new1", "new2", "new3", "new4"])

    def InsertRow(sender, data):
        insert_row("Table##widget", 5, ["inew1", "inew2", "inew3", "inew4"])

    def InsertCol(sender, data):
        insert_column("Table##widget", 1,  "Inserted Column", ["inew1", "inew2", "inew3", "inew4"])

    add_button("Delete row 6", callback=DeleteRow)
    add_button("Delete col 1", callback=DeleteCol)
    add_button("Add row ", callback=AddRow)
    add_button("Add col ", callback=AddCol)
    add_button("Insert row 5", callback=InsertRow)
    add_button("Insert col 1 ", callback=InsertCol)
    add_button("Clear Table ", callback=ClearTable)
    add_table("Table##widget", ["Column 1", "Column 2", "Column 3", "Column 4"])

    tabledata = []
    for i in range(0, 10):
        row = []
        for j in range(0, 4):
            row.append("Item"+str(i)+"-"+str(j))
        tabledata.append(row)

    set_value("Table##widget", tabledata)

########################################################################################################################
# Tooltips and Popups
########################################################################################################################
with window("Tooltips/Popups##dialog", 200, 200, hide=True):

    add_button("Hover me##tooltips")
    with tooltip("Hover me##tooltips", "tool_tip##tooltips"):
        add_simple_plot("Simpleplot##tooltips", (0.3, 0.9, 2.5, 8.9), height = 80)
    
    add_button("Modal Window")
    with popup("Modal Window", "ModalPopup", modal=True):
        add_simple_plot("Simpleplot##modal", (0.3, 0.9, 2.5, 8.9), height = 80)
        add_button("Close Window##modal", callback=close_popup)
    add_text("Right Click Me", tip="not hooked up")

########################################################################################################################
# Text
########################################################################################################################
with window("Text Widget##dialog", 200, 200, x_pos=0, y_pos=0, hide=True, no_resize=True, no_title_bar=True, no_move=False):
    add_text("Regular")
    add_text("Wrapped at 100 pixels", wrap=100)
    add_text("Color", color=(0, 200, 255))
    add_text("Bullet", bullet=True)

########################################################################################################################
# Input Text
########################################################################################################################
with window("Input Text##dialog", 500, 500, autosize=True, hide=True):
    add_input_text("Regular##inputtext")
    add_input_text("With hint##inputtext", hint="A hint")
    add_input_text("No spaces##inputtext", no_spaces=True)
    add_input_text("Uppercase##inputtext", uppercase=True)
    add_input_text("Decimal##inputtext", decimal=True)
    add_input_text("Hexdecimal##inputtext", hexadecimal=True)
    add_input_text("Read Only##inputtext", readonly=True, default_value="read only")
    add_input_text("Password##inputtext", password=True)
    add_input_text("Multiline##inputtext", multiline=True)

########################################################################################################################
# Widgets
########################################################################################################################
with window("Widgets##dialog", 500, 500, hide=True):

    def RetrieveValues(sender, data):

        # update checklist
        delete_item("Widgets##checklist")
        add_label_text("Widgets##checklist", value="Checked", color=[0, 255, 0], parent="ChecklistGroup")

        show_logger()

        items = get_item_children("Basic Widgets##widget")
        for item in items:
            log_info(item + ":\t" + str(get_value(item)))

        items = get_item_children("Time/Date Widgets##widget")
        for item in items:
            log_info(item + ":\t" + str(get_value(item)))

    add_button("Get Widget Values", callback=RetrieveValues)

    with tab_bar("Tab Bar##widget"):

        with tab("Basic Widgets##widget"):
            add_button("Button##widget")
            add_checkbox("Checkbox##widget")
            add_combo("Combo##widget", ("Item 1", "Item 2", "item 3"))
            add_radio_button("Radio Button##widget", ("Item 1", "Item 2", "item 3"), horizontal=True)
            add_listbox("Listbox##widget", ("Item 1", "Item 2", "item 3"))
            add_progress_bar("Progress Bar##widget", 0.45, overlay="Progress Bar", height = 100)
            add_text("Text")
            add_selectable("Selectable##widget")
            add_input_text("Input Text##widget")
            add_label_text("Label Text##123", "value")
            add_color_edit3("Color Edit3##widget", data_source = "DataStorage2")
            add_color_edit4("Color Edit4##widget")
            add_color_picker3("Color Picker3##widget", width=300, data_source = "DataStorage2")
            add_color_picker4("Color Picker4##widget", width=300)
            add_input_float("Input Float##widget")
            add_input_float2("Input Float2##widget")
            add_input_float3("Input Float3##widget")
            add_input_float4("Input Float4##widget")
            add_input_int("Input Int##widget")
            add_input_int2("Input Int2##widget")
            add_input_int3("Input Int3##widget", data_source = "DataStorage2")
            add_input_int4("Input Int4##widget")
            add_drag_float("Drag Float##widget")
            add_drag_float2("Drag Float2##widget")
            add_drag_float3("Drag Float3##widget")
            add_drag_float4("Drag Float4##widget")
            add_drag_int("Drag Int##widget")
            add_drag_int2("Drag Int2##widget")
            add_drag_int3("Drag Int3##widget", data_source = "DataStorage2")
            add_drag_int4("Drag Int4##widget")
            add_slider_float("Slider Float##widget")
            add_slider_float2("Slider Float2##widget")
            add_slider_float3("Slider Float3##widget")
            add_slider_float4("Slider Float4##widget")
            add_slider_int("Slider Int##widget")
            add_slider_int2("Slider Int2##widget")
            add_slider_int3("Slider Int3##widget", data_source = "DataStorage2")
            add_slider_int4("Slider Int4##widget")

        with tab("Container Widgets"):

            with tree_node("Tree Node1##widget"):
                for i in range(0, 3):
                    add_text("Item" + str(i))

            with tree_node("Tree Node2##widget"):
                for i in range(0, 3):
                    add_text("Item" + str(i))

            with collapsing_header("Collapsing Header##widget"):
                for i in range(0, 10):
                    add_text("Item " + str(i) + " belonging to a collapsing header")

            with child("Child##widget", width=220, height=100):
                for i in range(0, 10):
                    add_text("Item " + str(i) + " belonging to a child")

            add_same_line(spacing=50)

            with group("Group##widget"):
                add_text("Group")
                for i in range(0, 3):
                    add_button("Button" + str(i) + "##widgetgroup")

        with tab("Time/Date Widgets##widget"):
            add_time_picker("Time Picker")
            add_separator()
            add_date_picker("Date Picker1", level=0, default_value={'month_day': 8, 'year':93, 'month':5})
            add_separator()
            add_date_picker("Date Picker2", level=1, default_value={'month_day': 8, 'year':93, 'month':5})
            add_separator()
            add_date_picker("Date Picker3", level=2, default_value={'month_day': 8, 'year':93, 'month':5})
            add_separator()


########################################################################################################################
# Logger
########################################################################################################################
with window("Logging##dialog", 500, 500, autosize=True, hide=True):

    def LogCallback(sender, data):
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

    add_button("Test Logger", callback=LogCallback)
    add_same_line(spacing=10)
    with group("LoggingGroup"):
        add_text("Log Level")
        add_radio_button("Log Level##logging", ("Trace", "Debug", "Info", "Warning", "Error", "Off"))

########################################################################################################################
# Plots
########################################################################################################################
with window("Plots, Graphs and Charts##dialog", 500, 500, hide=True):

    def PlotCallback(sender, data):

        data1 = []
        for i in range(0, 100):
            data1.append([3.14*i/180, cos(3*3.14*i/180)])

        data2 = [[0.5, -0.5], [1, -0.5], [1, -1], [0.5, -1]]

        data3 = []
        for i in range(0, 25):
            data3.append([3.14*i/180, 10*cos(3*3.14*i/180), 0.5*cos(3*3.14*i/180), 0.5*sin(3*3.14*i/180)])

        # error charts
        add_error_series("Error Plot", "Errors1", data3)
        add_line_series("Error Plot", "Errors2", data3)

        # regular charts
        add_line_series("Plot", "Line", data1, weight=2)
        add_shade_series("Plot", "Shade", data1, weight=2, fill=[255, 0, 0, 100])
        add_scatter_series("Plot", "Scatter", data1)
        add_stem_series("Plot", "Stem", data1)
        add_area_series("Plot", "Area", data2, [255, 255, 0], [255, 255, 0, 100])

        # pie charts
        add_pie_series("PieChart1", "PieChart1", [["fish", 0.25], ["Cow", 0.30], ["Chicken", 0.30]], 0.5, 0.5, 0.5, normalize=True)
        set_plot_xlimits("PieChart1", 0, 1)
        set_plot_ylimits("PieChart1", 0, 1)

        # bar charts
        add_bar_series("BarChart", "Final Exam", [[10, 100], [20, 75], [30,90]], weight=1)
        add_bar_series("BarChart", "Midterm Exam", [[11, 83], [21, 75], [31,72]], weight=1)
        add_bar_series("BarChart", "Course Grade", [[12, 42], [22, 68], [32,23]], weight=1)

        # heat charts
        values = [[0.8, 2.4, 2.5, 3.9, 0.0, 4.0, 0.0],
                  [2.4, 0.0, 4.0, 1.0, 2.7, 0.0, 0.0],
                  [1.1, 2.4, 0.8, 4.3, 1.9, 4.4, 0.0],
                  [0.6, 0.0, 0.3, 0.0, 3.1, 0.0, 0.0],
                  [0.7, 1.7, 0.6, 2.6, 2.2, 6.2, 0.0],
                  [1.3, 1.2, 0.0, 0.0, 0.0, 3.2, 5.1],
                  [0.1, 2.0, 0.0, 1.4, 0.0, 1.9, 6.3]]
        add_heat_series("Heat Plot", "heat data", values, 7, 7, 0, 6)

    def colormapCallback(sender, data):
        value = get_value("Colormaps")
        set_color_map("Plot", value)

    with tab_bar("PlotTabBar"):

        with tab("Plot Widget"):
            add_button("Plot data", callback=PlotCallback)
            add_listbox("Colormaps", ("Default", "Dark", "Pastel", "Paired", "Viridis", "Plasma", "Hot", 
                                      "Cool", "Pink", "Jet"), width=500, height=3, callback=colormapCallback)
            add_plot("Plot", "x-axis", "y-axis", height=-1)

        with tab("Simple Plots"):
            add_simple_plot("Simpleplot1", (0.3, 0.9, 2.5, 8.9))
            add_simple_plot("Simpleplot2", (0.3, 0.9, 2.5, 8.9), overlay="Overlaying", height=180, histogram=True)

        with tab("Pie Chart"):
            add_plot("PieChart1", "", "", no_mouse_pos=True, 
                     xaxis_no_gridlines=True, xaxis_no_tick_marks=True, xaxis_no_tick_labels=True,
                     yaxis_no_gridlines=True, yaxis_no_tick_marks=True, yaxis_no_tick_labels=True)

        with tab("Bar Chart"):
            add_plot("BarChart", "Student", "Score", height=-1)
            set_xticks("BarChart", [["S1", 10], ["S2", 20], ["S3", 30]])

        with tab("Error Plots"):
            add_plot("Error Plot", "x-axis", "y-axis", height=-1)

        with tab("Heat Plots"):
            #add_plot("Heat Plot", "", "", show_color_scale=True)
            add_plot("Heat Plot", "", "", show_color_scale=True, scale_min=0.0, scale_max=6.0, 
                     scale_height=500, no_legend=True, 
                     no_mouse_pos=True, xaxis_lock_min=True, xaxis_lock_max=True, xaxis_no_gridlines=True, xaxis_no_tick_marks=True,
                     yaxis_no_gridlines=True, yaxis_no_tick_marks=True, yaxis_lock_min=True, yaxis_lock_max=True)

########################################################################################################################
# Canvas
########################################################################################################################
with window("Drawing API##dialog", autosize=True, hide=True):

    def UpdateDrawing(sender, data):
        set_drawing_origin("drawing##widget", get_value("X Origin"), get_value("Y Origin"))
        set_drawing_scale("drawing##widget", get_value("X Scale "), get_value("Y Scale"))

    with group("Drawing Controls Group"):
        add_slider_float("X Origin", vertical=True, min_value = -100, max_value=100, default_value=0, callback=UpdateDrawing)
        add_same_line(spacing=20)
        add_slider_float("Y Origin", vertical=True, min_value = -100, max_value=100, default_value=0, callback=UpdateDrawing)
        add_slider_float("X Scale ", vertical=True, max_value=10, default_value=1, callback=UpdateDrawing)
        add_same_line(spacing=20)
        add_slider_float("Y Scale", vertical=True, max_value=10, default_value=1, callback=UpdateDrawing)
    add_same_line(spacing=20)
    add_drawing("drawing##widget", width=800, height=500)
    draw_rectangle("drawing##widget", (0, 500), (800, 0), (255, 0, 0, 255), fill=(0, 0, 25, 255), rounding=12, thickness = 1.0)
    draw_line("drawing##widget", (10, 10), (100, 100), (255, 0, 0, 255), 1)
    draw_triangle("drawing##widget", (300, 500), (200, 200), (500, 200), (255, 255, 0, 255), thickness = 3.0)
    draw_quad("drawing##widget", (50, 50), (150, 50), (150, 150), (50, 150), (255, 255, 0, 255), thickness = 3.0)
    draw_text("drawing##widget", (50, 300), "Some Text", color=(255, 255, 0, 255), size=15)
    draw_text("drawing##widget", (0, 0), "Origin", color=(255, 255, 0, 255), size=15)
    draw_circle("drawing##widget", (400, 250), 50, (255, 255, 0, 255))
    draw_polyline("drawing##widget", ((320, 490), (185, 200), (500, 710)), (255, 255, 0, 255), thickness=1.0)
    draw_polygon("drawing##widget", ((363, 471), (153, 498), (59, 220), (363, 471)), (255, 125, 0, 255), thickness=1.0, fill=(255, 125, 0, 50))
    draw_bezier_curve("drawing##widget", (50, 200), (150, 250), (300, 150), (600, 250), (255, 255, 0, 255), thickness = 2.0)
    draw_arrow("drawing##widget", (50, 70), (100, 65), (0, 200, 255), 1, 10)
    #draw_image("drawing##widget", "C:/dev/DearPyGui/Examples/SpriteMapExample.png", pmin=[200, 200])

########################################################################################################################
# Asyncronous
########################################################################################################################
with window("Asyncronous##dialog", hide=True):
    add_data('threadNumber', 0)
    def LongCallback2(sender, data):
        sleep(5)
        return data

    def ReturnFromLongProcess(sender, data):
        log_info("Completed process number: " + str(data))
        add_data('threadNumber', get_data('threadNumber')-1)

    def LongAsyncronousCallback(sender, data):
        current_number = get_data('threadNumber')
        run_async_function(LongCallback2, current_number, return_handler=ReturnFromLongProcess)
        log_info("Start process number: " + str(current_number))
        add_data('threadNumber', current_number+1)

    def LongCallback(sender, data):
        sleep(5)
        log_info("Done with long process")

    add_button("Start Long Process", callback=LongCallback)
    add_button("Start Long Asyncronous Process", callback=LongAsyncronousCallback)

########################################################################################################################
# Input Polling
########################################################################################################################
with window("Input Polling##dialog", hide=True, autosize=True):
    set_window_pos("Input Polling##dialog", 700, 50)
    add_text("Key Polling")
    add_label_text("A key Down", "False", color=(0,200,255))
    add_label_text("W key Pressed", "False", color=(0,200,255))
    add_label_text("Q key Released", "False", color=(0,200,255))
    add_spacing()
    add_text("Mouse Polling")
    add_label_text("Mouse Position", "(0,0)", color=(0,200,255))
    add_label_text("Left Mouse Dragging", "False", color=(0,200,255))
    add_label_text("Middle Mouse Dragging", "False", color=(0,200,255))
    add_label_text("Right Mouse Dragging", "False", color=(0,200,255))
    add_label_text("Left Mouse Clicked", "False", color=(0,200,255))
    add_label_text("Middle Mouse Clicked", "False", color=(0,200,255))
    add_label_text("Right Mouse Clicked", "False", color=(0,200,255))
    add_label_text("Left Mouse Double Clicked", "False", color=(0,200,255))
    add_label_text("Middle Mouse Double Clicked", "False", color=(0,200,255))
    add_label_text("Right Mouse Double Clicked", "False", color=(0,200,255))
    add_label_text("Left Mouse Down", "False", color=(0,200,255))
    add_label_text("Middle Mouse Down", "False", color=(0,200,255))
    add_label_text("Right Mouse Down", "False", color=(0,200,255))
    add_label_text("Left Mouse Released", "False", color=(0,200,255))
    add_label_text("Middle Mouse Released", "False", color=(0,200,255))
    add_label_text("Right Mouse Released", "False", color=(0,200,255))


def mouse_move_callback(sender, data):

    print(sender, " : ", data)

def InputPollingMainCallback(sender, data):

    set_value("Mouse Position", str(get_mouse_pos()))

    # keys
    if is_key_down(mvKey_A):
        set_value("A key Down", "True")
    else:
        set_value("A key Down", "False")

    if is_key_pressed(mvKey_W):
        set_value("W key Pressed", "True")
    else:
        set_value("W key Pressed", "False")

    if is_key_released(mvKey_Q):
        set_value("Q key Released", "True")
    else:
        set_value("Q key Released", "False")

    # mouse dragging
    if is_mouse_button_dragging(mvMouseButton_Left, 10):
        set_value("Left Mouse Dragging", "True")
    else:
        set_value("Left Mouse Dragging", "False")

    if is_mouse_button_dragging(mvMouseButton_Right, 10):
        set_value("Right Mouse Dragging", "True")
    else:
        set_value("Right Mouse Dragging", "False")

    if is_mouse_button_dragging(mvMouseButton_Middle, 10):
        set_value("Middle Mouse Dragging", "True")
    else:
        set_value("Middle Mouse Dragging", "False")

    # mouse down
    if is_mouse_button_down(mvMouseButton_Left):
        set_value("Left Mouse Down", "True")
    else:
        set_value("Left Mouse Down", "False")

    if is_mouse_button_down(mvMouseButton_Right):
        set_value("Right Mouse Down", "True")
    else:
        set_value("Right Mouse Down", "False")

    if is_mouse_button_down(mvMouseButton_Middle):
        set_value("Middle Mouse Down", "True")
    else:
        set_value("Middle Mouse Down", "False")

    # mouse clicked
    if is_mouse_button_clicked(mvMouseButton_Left):
        set_value("Left Mouse Clicked", "True")
    else:
        set_value("Left Mouse Clicked", "False")

    if is_mouse_button_clicked(mvMouseButton_Right):
        set_value("Right Mouse Clicked", "True")
    else:
        set_value("Right Mouse Clicked", "False")

    if is_mouse_button_clicked(mvMouseButton_Middle):
        set_value("Middle Mouse Clicked", "True")
    else:
        set_value("Middle Mouse Clicked", "False")

    # mouse double clicked
    if is_mouse_button_double_clicked(mvMouseButton_Left):
        set_value("Left Mouse Double Clicked", "True")
    else:
        set_value("Left Mouse Double Clicked", "False")

    if is_mouse_button_double_clicked(mvMouseButton_Right):
        set_value("Right Mouse Double Clicked", "True")
    else:
        set_value("Right Mouse Double Clicked", "False")

    if is_mouse_button_double_clicked(mvMouseButton_Middle):
        set_value("Middle Mouse Double Clicked", "True")
    else:
        set_value("Middle Mouse Double Clicked", "False")

    # mouse released
    if is_mouse_button_released(mvMouseButton_Left):
        set_value("Left Mouse Released", "True")
    else:
        set_value("Left Mouse Released", "False")

    if is_mouse_button_released(mvMouseButton_Right):
        set_value("Right Mouse Released", "True")
    else:
        set_value("Right Mouse Released", "False")

    if is_mouse_button_released(mvMouseButton_Middle):
        set_value("Middle Mouse Released", "True")
    else:
        set_value("Middle Mouse Released", "False")

set_render_callback(InputPollingMainCallback, "Input Polling##dialog")
set_mouse_move_callback(mouse_move_callback, "Input Polling##dialog")
set_mouse_release_callback(mouse_move_callback)

setup_dearpygui()
while is_dearpygui_running():
    render_dearpygui_frame()
cleanup_dearpygui()
