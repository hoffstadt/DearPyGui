Values
======

When an item is created, it creates an associated value by default. 
Values can be shared between items with the same underlying value type. 
This is accomplished by using the `source` keyword. One of the benefits of 
this is to have multiple items control the same value.

Values are retrieved from the value :py:func:`get_value <dearpygui.dearpygui.get_value>`.

Values can be changed manually using :py:func:`set_value <dearpygui.dearpygui.set_value>`.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):
        dpg.add_checkbox(label="Radio Button1", tag="R1")
        dpg.add_checkbox(label="Radio Button2", source="R1")

        dpg.add_input_text(label="Text Input 1")
        dpg.add_input_text(label="Text Input 2", source=dpg.last_item(), password=True)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Value Items
-----------
There are several "Value" items that can be used. These are
items that have no visual component. These include:

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

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.value_registry():
        dpg.add_bool_value(default_value=True, tag="bool_value")
        dpg.add_string_value(default_value="Default string", tag="string_value")

    with dpg.window(label="Tutorial"):
        dpg.add_checkbox(label="Radio Button1", source="bool_value")
        dpg.add_checkbox(label="Radio Button2", source="bool_value")

        dpg.add_input_text(label="Text Input 1", source="string_value")
        dpg.add_input_text(label="Text Input 2", source="string_value", password=True)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
