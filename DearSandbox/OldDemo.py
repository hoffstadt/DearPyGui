from dearpygui.core import *
from dearpygui.simple import *
from time import sleep
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
# Widgets
########################################################################################################################
with window("Widgets##dialog", 500, 500, show=False):

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
            add_value("float", [0.3, 45, 33, 44])
            add_button("Button##widget")

            with group("Color Buttons", horizontal=True):
                add_color_button("ColorButton1", [255, 0, 0, 150])
                add_color_button("ColorButton2", [0, 255, 0, 150])
                add_color_button("ColorButton3", [0, 0, 255, 150])

            add_checkbox("Checkbox##widget", source="bool")
            add_combo("Combo##widget", items=("Item 1", "Item 2", "item 3"), source="string")
            add_radio_button("Radio Button##widget", items=("Item 1", "Item 2", "item 3"), horizontal=True, source="int")
            add_listbox("Listbox##widget", items=("Item 1", "Item 2", "item 3"), source="int")
            add_progress_bar("Progress Bar##widget", 0.45, overlay="Progress Bar", height = 100, source="float")
            add_text("Text")
            add_selectable("Selectable##widget", source="bool")
            add_input_text("Input Text##widget", source="string")
            add_label_text("Label Text##123", "value", source="string")
            add_color_edit3("Color Edit3##widget", source = "float3")
            add_color_edit4("Color Edit4##widget", source = "float4")
            add_color_picker3("Color Picker3##widget", width=300, source = "float3")
            add_color_picker4("Color Picker4##widget", width=300, source = "float4")
            add_input_float("Input Float##widget", source="float")
            add_input_float2("Input Float2##widget", source="float2")
            add_input_float3("Input Float3##widget", source="float3")
            add_input_float4("Input Float4##widget", source="float4")
            add_input_int("Input Int##widget", source="int")
            add_input_int2("Input Int2##widget", source="int2")
            add_input_int3("Input Int3##widget", source = "int3")
            add_input_int4("Input Int4##widget", source = "int4")
            add_drag_float("Drag Float##widget", source="float")
            add_drag_float2("Drag Float2##widget", source="float2")
            add_drag_float3("Drag Float3##widget", source="float3")
            add_drag_float4("Drag Float4##widget", source="float4")
            add_drag_int("Drag Int##widget", source = "int")
            add_drag_int2("Drag Int2##widget", source = "int2")
            add_drag_int3("Drag Int3##widget", source = "int3")
            add_drag_int4("Drag Int4##widget", source = "int4")
            add_slider_float("Slider Float##widget", source="float")
            add_slider_float2("Slider Float2##widget", source="float2")
            add_slider_float3("Slider Float3##widget", source="float3")
            add_slider_float4("Slider Float4##widget", source="float4")
            add_slider_int("Slider Int##widget", source="int")
            add_slider_int2("Slider Int2##widget", source="int2")
            add_slider_int3("Slider Int3##widget", source = "int3")
            add_slider_int4("Slider Int4##widget", source = "int4")

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

            with child("Child2##widget", width=220, height=100):
                with menu_bar("ChildMenuBar"):
                    add_text("Decoration")
                for i in range(0, 10):
                    add_text("Item " + str(i) + " belonging to a child2")

        with tab("Time/Date Widgets##widget"):
            add_time_picker("Time Picker", default_value={'hour': 14, 'min': 32, 'sec': 23})
            add_separator()
            add_date_picker("Date Picker1", level=0, default_value={'month_day': 8, 'year':93, 'month':5})
            add_separator()
            add_date_picker("Date Picker2", level=1, default_value={'month_day': 8, 'year':93, 'month':5})
            add_separator()
            add_date_picker("Date Picker3", level=2, default_value={'month_day': 8, 'year':93, 'month':5})
            add_separator()

########################################################################################################################
# Asyncronous
########################################################################################################################
with window("Asyncronous##dialog", show=False):
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


def mouse_move_callback(sender, data):

    print(sender, " : ", data)

set_mouse_release_callback(mouse_move_callback)

setup_dearpygui()
while is_dearpygui_running():
    render_dearpygui_frame()
cleanup_dearpygui()
