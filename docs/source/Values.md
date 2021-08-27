| [Basics](#basics) - [Value App Items](#value-app-items) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Value-and-Data-Storage) |
|----|
||

## Basics
When a widget is added, it creates an associated value by default. Values can be shared between widgets with the same underlying value type. This is accomplished by using the `source` keyword. One of the benefits of this is to have multiple widgets control the same value.
Values are retrieved from the value `get_value`.

Values can be changed manually using `set_value`.

```python
import dearpygui.dearpygui as dpg

with dpg.window(label="Tutorial"):

    dpg.add_checkbox(label="Radio Button1")
    dpg.add_checkbox(label="Radio Button2", source=dpg.last_item())

    dpg.add_input_text(label="Text Input 1")
    dpg.add_input_text(label="Text Input 2", source=dpg.last_item(), password=True)

dpg.start_dearpygui()
```

## Value App Items
There are several "Value" app items that can be used. These are widgets that have no visual component. These include:
* **mvBoolValue**
* **mvColorValue**
* **mvDoubleValue**
* **mvDouble4Value**
* **mvFloatValue**
* **mvFloat4Value**
* **mvFloatVectValue**
* **mvIntValue**
* **mvInt4Value**
* **mvSeriesValue**
* **mvStringValue**

Basic usage can be found below:
```python
import dearpygui.dearpygui as dpg

with dpg.value_registry():
    dpg.add_bool_value(default_value=True, id="bool_value")
    dpg.add_string_value(default_value="Default string", id="string_value")

with dpg.window(label="Tutorial"):

    dpg.add_checkbox(label="Radio Button1", source="bool_value")
    dpg.add_checkbox(label="Radio Button2", source="bool_value")

    dpg.add_input_text(label="Text Input 1", source="string_value")
    dpg.add_input_text(label="Text Input 2", source="string_value", password=True)

dpg.start_dearpygui()
```
