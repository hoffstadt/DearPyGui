Tooltips
--------

Tooltips are windows that appear when a widget is hovered.
In DPG tooltips are containers that can contain any other widget.
Tooltips are 1 of 2 widgets in which the first argument is not the name of the tooltip,
but widget that the tooltip is associated with.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    with dpg.window(label="Tutorial"):

        dpg.add_text("Hover me", id="tooltip_parent")

        with dpg.tooltip("tooltip_parent"):
            dpg.add_text("A tooltip")

    dpg.start_dearpygui()

**Results**

.. image:: https://raw.githubusercontent.com/hoffstadt/DearPyGui/assets/wiki_images/tooltips.PNG