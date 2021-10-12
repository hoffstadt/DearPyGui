Tooltips
========

Tooltips are windows that appear when an item is hovered.

Tooltips are containers that can hold any other UI Item.

Tooltips must have the parent argument. This is the *tag* of the parent what will show the tooltip.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):
        dpg.add_text("Hover me", tag="tooltip_parent")

        with dpg.tooltip("tooltip_parent"):
            dpg.add_text("A tooltip")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/wiki_images/tooltips.PNG