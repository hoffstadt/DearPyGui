Tables
======

The table API is a low level API that can be used to create a table.

It can also be used as a layout mechanism. Tables are composed of
multiple components which include columns, rows, next_columns, and the
actual app items to be displayed. The best place to learn about the various
configuration options for the table is by running the demo!

Below is the minimum example for creating a table

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):
        with dpg.table(header_row=False):

            # use add_table_column to add columns to the table,
            # table columns use child slot 0
            dpg.add_table_column()
            dpg.add_table_column()
            dpg.add_table_column()

            # add_table_next_column will jump to the next row
            # once it reaches the end of the columns
            # table next column use slot 1
            for i in range(0, 4):
                with dpg.table_row():
                    for j in range(0, 3):
                        dpg.add_text(f"Row{i} Column{j}")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. note:: The maximum number of columns is 64.

multiple items can go into a single cell by creating a cell as shown

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):

        with dpg.table(header_row=False, resizable=True, policy=dpg.mvTable_SizingStretchProp,
                       borders_outerH=True, borders_innerV=True, borders_innerH=True, borders_outerV=True):

            dpg.add_table_column(label="Header 1")
            dpg.add_table_column(label="Header 2")
            dpg.add_table_column(label="Header 3")

            # once it reaches the end of the columns
            for i in range(0, 4):
                with dpg.table_row():
                    for j in range(0, 3):
                        with dpg.table_cell():
                            dpg.add_button(label=f"Row{i} Column{j} a")
                            dpg.add_button(label=f"Row{i} Column{j} b")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Borders, Background
-------------------

You can control the borders of the table using the
`borders_innerH`, `borders_innerV`, `borders_outerH`,
and `borders_outerV` keywords. You can also turn on alternate
row coloring using the `row_background` keyword.

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):

        with dpg.table(header_row=False, row_background=True,
                       borders_innerH=True, borders_outerH=True, borders_innerV=True,
                       borders_outerV=True):

            # use add_table_column to add columns to the table,
            # table columns use child slot 0
            dpg.add_table_column()
            dpg.add_table_column()
            dpg.add_table_column()

            # add_table_next_column will jump to the next row
            # once it reaches the end of the columns
            # table next column use slot 1
            for i in range(0, 4):
                with dpg.table_row():
                    for j in range(0, 3):
                        dpg.add_text(f"Row{i} Column{j}")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Column Headers
--------------

Column headers are simply shown by setting `header_row` to **True**
and setting the label of the columns.

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):
    
        with dpg.table(header_row=True):

            # use add_table_column to add columns to the table,
            # table columns use slot 0
            dpg.add_table_column(label="Header 1")
            dpg.add_table_column(label="Header 2")
            dpg.add_table_column(label="Header 3")

            # add_table_next_column will jump to the next row
            # once it reaches the end of the columns
            # table next column use slot 1
            for i in range(0, 4):
                with dpg.table_row():
                    for j in range(0, 3):
                        dpg.add_text(f"Row{i} Column{j}")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Resizing
--------

In order for a table to have resizable columns,
the `resizable` and `borders_innerV` keywords must be set to **True**.

You can also set the sizing policy
keyword, `policy`, using the following options


| Policy |
| ---- |
| mvTable_SizingFixedFit |
| mvTable_SizingFixedSame |
| mvTable_SizingStretchProp |
| mvTable_SizingStretchSame |

Stretch
-------

Below is an example of setting the stretch policy for the entire table

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):

        with dpg.table(header_row=False, resizable=True, policy=dpg.mvTable_SizingStretchProp,
                       borders_outerH=True, borders_innerV=True, borders_outerV=True):

            dpg.add_table_column(label="Header 1")
            dpg.add_table_column(label="Header 2")
            dpg.add_table_column(label="Header 3")

            # add_table_next_column will jump to the next row
            # once it reaches the end of the columns
            # table next column use slot 1
            for i in range(0, 4):
                with dpg.table_row():
                    for j in range(0, 3):
                        dpg.add_text(f"Row{i} Column{j}")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Fixed**

Below is an example of setting the fixed fit policy for the entire table

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):

        # Only available if scrollX/scrollY are disabled and stretch columns are not used
        with dpg.table(header_row=False, policy=dpg.mvTable_SizingFixedFit, resizable=True, no_host_extendX=True,
                       borders_innerV=True, borders_outerV=True, borders_outerH=True):

            dpg.add_table_column(label="Header 1")
            dpg.add_table_column(label="Header 2")
            dpg.add_table_column(label="Header 3")
            for i in range(0, 4):
                with dpg.table_row():
                    for j in range(0, 3):
                        dpg.add_button(label=f"Row{i} Column{j} a")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

**Mixed**

You can also set columns individually by using the
`width_fixed` or `width_stretch` keyword along with the
`init_width_or_weight` keyword.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial"):

        with dpg.table(header_row=True, policy=dpg.mvTable_SizingFixedFit, row_background=True, reorderable=True,
                       resizable=True, no_host_extendX=False, hideable=True,
                       borders_innerV=True, delay_search=True, borders_outerV=True, borders_innerH=True,
                       borders_outerH=True):

            dpg.add_table_column(label="AAA", width_fixed=True)
            dpg.add_table_column(label="BBB", width_fixed=True)
            dpg.add_table_column(label="CCC", width_stretch=True, init_width_or_weight=0.0)
            dpg.add_table_column(label="DDD", width_stretch=True, init_width_or_weight=0.0)

            for i in range(0, 5):
                with dpg.table_row():
                    for j in range(0, 4):
                        if j == 2 or j == 3:
                            dpg.add_text(f"Stretch {i},{j}")
                        else:
                            dpg.add_text(f"Fixed {i}, {j}")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Column Options
--------------

There are a large number of options available for table columns
which are best learned through running the demo, these include

====================== ============= ===========
keyword                default value description
====================== ============= ===========
init_width_or_weight   0.0           sets the starting width (fixed policy) or proportion (stretch) of the column.
default_hide           False         Default as a hidden/disabled column.
default_sort           False         Default as a sorting column.
width_stretch          False         Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp).
width_fixed            False         Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable).
no_resize              False         Disable manual resizing.
no_reorder             False         Disable manual reordering this column, this will also prevent other columns from crossing over this column.
no_hide                False         Disable ability to hide/disable this column.
no_clip                False         Disable clipping for this column.
no_sort                False         Disable sorting for this column.
no_sort_ascending      False         Disable ability to sort in the ascending direction.
no_sort_descending     False         Disable ability to sort in the descending direction.
no_header_width        False         Disable header text width contribution to automatic column width.
prefer_sort_ascending  True          Make the initial sort direction Ascending when first sorting on this column (default).
prefer_sort_descending False         Make the initial sort direction Descending when first sorting on this column.
indent_enabled         False         Use current Indent value when entering cell (default for column 0).
indent_disable         False         Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored.
====================== ============= ===========

Sorting
-------

Under construction.

Scrolling
---------

Under construction

Clipping
--------

Using a clipper can help performance with large tables.

Try using the example below with and with out clipping and see
the effect on the framerate listed in metrics.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()


    def clipper_toggle(sender):
        dpg.configure_item("table_clip", clipper=dpg.get_value(sender))


    with dpg.window(label="Tutorial"):
        dpg.add_checkbox(label="clipper", callback=clipper_toggle, default_value=True)

        with dpg.table(header_row=False, tag="table_clip", clipper=True):

            for i in range(5):
                dpg.add_table_column()

                for i in range(7000):
                    with dpg.table_row():
                        for j in range(5):
                            dpg.add_text(f"Row{i} Column{j}")

    dpg.show_metrics()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Filtering
---------

Under construction

Padding
-------
Under construction

Outer Size
----------

Under construction

Column Widths
-------------

Under construction

Rows
----

Under construction

Row Height
----------

Under construction

Search Delay
------------

Under construction