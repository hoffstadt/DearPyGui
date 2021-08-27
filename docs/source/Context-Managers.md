| [Basics](#basics) - [Example](#example) - [Benefits](#benefits) - [Context Managers](#context-managers) - [Explicit Parental Assignment](#explicit-parental-assignment) - [Container Stack Operations](#container-stack-operations) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Context-Manager-Containers) |
|----|
||

## Basics
We have added context managers as helpers for most container items. For more detail, see [Parent Deduction & Container Stack](https://github.com/hoffstadt/DearPyGui/wiki/Parent-Deduction-&-Container-Stack).

| Core Command | Context Manager |
|--------------|-----------------|
| **add_table** | **with table(...):** |
| **add_table_row** | **with table_row(...):** |
| **add_window** | **with window(...):** |
| **add_menu_bar** | **with menu_bar(...):** |
| **add_child** | **with child(...):** |
| **add_clipper** | **with clipper(...):** |
| **add_collapsing_header** | **with collapsing_header(...):** |
| **add_colormap_registry** | **with colormap_registry(...):** |
| **add_group** | **with group(...):** |
| **add_node** | **with node(...):** |
| **add_node_attribute** | **with node_attribute(...):** |
| **add_node_editor** | **with node_editor(...):** |
| **add_staging_container** | **with staging_container(...):** |
| **add_tab_bar** | **with tab_bar(...):** |
| **add_tab** | **with tab(...):** |
| **add_tree_node** | **with tree_node(...):** |
| **add_tooltip** | **with tooltip(...):** |
| **add_popup** | **with popup(...):** |
| **add_drag_payload** | **with payload(...):** |
| **add_drawlist** | **with drawlist(...):** |
| **add_draw_layer** | **with draw_layer(...):** |
| **add_viewport_drawlist** | **with viewport_drawlist(...):** |
| **add_file_dialog** | **with file_dialog(...):** |
| **add_filter_set** | **with filter_set(...):** |
| **add_font** | **with font(...):** |
| **add_font_registry** | **with font_registry(...):** |
| **add_handler_registry** | **with handler_registry(...):** |
| **add_plot** | **with plot(...):** |
| **add_subplots** | **with subplots(...):** |
| **add_texture_registry** | **with texture_registry(...):** |
| **add_value_registry** | **with value_registry(...):** |
| **add_theme** | **with theme(...):** |
| **add_item_pool** | **with item_pool(...):** |
| **add_template_registry** | **with template_registry(...):** |

### Benefits:
1. Automatically push the container to the container stack.
2. Automatically pop the container off the container stack.
2. They make the code more readable and structured.

---

### Context Managers:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Main"):

    with dpg.menu_bar():

        with dpg.menu(label="Themes"):
            dpg.add_menu_item(label="Dark")
            dpg.add_menu_item(label="Light")
            dpg.add_menu_item(label="Classic")

            with dpg.menu(label="Other Themes"):
                dpg.add_menu_item(label="Purple")
                dpg.add_menu_item(label="Gold")
                dpg.add_menu_item(label="Red")

        with dpg.menu(label="Tools"):
            dpg.add_menu_item(label="Show Logger")
            dpg.add_menu_item(label="Show About")

        with dpg.menu(label="Oddities"):
            dpg.add_button(label="A Button")
            dpg.add_simple_plot(label="Menu plot", (0.3, 0.9, 2.5, 8.9), height = 80)

dpg.start_dearpygui()
```

---

### Explicit Parental Assignment (using UUIDs):
```python
import dearpygui.dearpygui as dpg

w = dpg.add_window(label="Main")

mb = dpg.add_menu_bar(parent=w)

themes = dpg.add_menu("Themes", parent=mb)
dpg.add_menu_item(label="Dark", parent=themes)
dpg.add_menu_item(label="Light", parent=themes)

other_themes = dpg.add_menu("Other Themes", parent=themes)
dpg.add_menu_item(label="Purple", parent=other_themes)
dpg.add_menu_item(label="Gold", parent=other_themes)
add_menu_item(label="Red", parent=other_themes)

tools = dpg.add_menu(label="Tools", parent=mb)
dpg.add_menu_item(label="Show Logger", parent=tools)
dpg.add_menu_item(label="Show About", parent=tools)

oddities = dpg.add_menu(label="Oddities")
dpg.add_button(label="A Button", parent=oddities)
dpg.add_simple_plot(label="A menu plot", (0.3, 0.9, 2.5, 8.9), height=80, parent=oddities)

dpg.start_dearpygui()
```

---

### Explicit Parental Assignment (using aliases):
```python
import dearpygui.dearpygui as dpg

dpg.add_window(label="Main", id="w")

dpg.add_menu_bar(parent=w, id="mb")

dpg.add_menu("Themes", parent="mb", id="themes")
dpg.add_menu_item(label="Dark", parent="themes")
dpg.add_menu_item(label="Light", parent="themes")

dpg.add_menu("Other Themes", parent="themes", id="other_themes")
dpg.add_menu_item(label="Purple", parent="other_themes")
dpg.add_menu_item(label="Gold", parent="other_themes")
add_menu_item(label="Red", parent="other_themes")

dpg.add_menu(label="Tools", parent="mb", id="tools")
dpg.add_menu_item(label="Show Logger", parent="tools")
dpg.add_menu_item(label="Show About", parent="tools")

dpg.add_menu(label="Oddities", id="Oddities")
dpg.add_button(label="A Button", parent="Oddities")
dpg.add_simple_plot(label="A menu plot", (0.3, 0.9, 2.5, 8.9), height=80, parent="Oddities")

dpg.start_dearpygui()
```

---

### Container Stack Operations:
```python
import dearpygui.dearpygui as dpg

dpg.push_container_stack(dpg.add_window(label="Main"))

dpg.push_container_stack(dpg.add_menu_bar())

dpg.push_container_stack(dpg.add_menu(label="Themes"))
dpg.add_menu_item(label="Dark")
dpg.add_menu_item(label="Light")
dpg.pop_container_stack()

dpg.push_container_stack(dpg.add_menu(label="Tools"))
dpg.add_menu_item(label="Show Logger")
dpg.add_menu_item(label="Show About")
dpg.pop_container_stack()

# remove menu_bar from container stack
dpg.pop_container_stack()

# remove window from container stack
dpg.pop_container_stack()

dpg.start_dearpygui()
```
