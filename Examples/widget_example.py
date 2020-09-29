from dearpygui.core import *
from dearpygui.simple import *

# callback
def retrieve_values(sender, data):
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

set_main_window_size(800, 800)

add_button("Get Widget Values", callback=retrieve_values)

with tab_bar("Tab Bar##widget"):
    with tab("Basic Widgets##widget"):

        add_button("Button##widget")
        add_checkbox("Checkbox##widget")
        add_combo("Combo##widget", items=["Item 1", "Item 2", "item 3"])
        add_radio_button("Radio Button##widget", items=["Item 1", "Item 2", "item 3"])
        add_listbox("Listbox##widget", items=["Item 1", "Item 2", "item 3"])
        add_progress_bar("Progress Bar##widget", 0.45, overlay="Progress Bar")

        add_text("Text")
        add_selectable("Selectable##widget")
        add_input_text("Input Text##widget")
        add_label_text("Label Text##123", value="value")

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

    with tab("Tables"):
        add_table("Table##widget", ["Column 1", "Column 2", "Column 3", "Column 4"])
        tabledata = []
        for i in range(0, 10):
            row = []
            for j in range(0, 4):
                row.append("Item" + str(i) + "-" + str(j))
            tabledata.append(row)

        set_value("Table##widget", tabledata)

start_dearpygui()
