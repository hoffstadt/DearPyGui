Filter Set
==========

The filter set item is a container that can
be used to filter its children based on their *filter_key*.

Most app items have a *filter_key* keyword that can be set when
creating the item. This works by setting the value of the filter set.

The easiest way to understand this is by considering the example below

**Code**

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()


    def callback(sender, filter_string):
        # set value of filter set
        dpg.set_value("filter_id", filter_string)


    with dpg.window(label="about", width =500, height=300):
        dpg.add_input_text(label="Filter (inc, -exc)", callback=callback)
        with dpg.filter_set(id="filter_id"):
            dpg.add_text("aaa1.c", filter_key="aaa1.c", bullet=True)
            dpg.add_text("bbb1.c", filter_key="bbb1.c", bullet=True)
            dpg.add_text("ccc1.c", filter_key="ccc1.c", bullet=True)
            dpg.add_text("aaa2.cpp", filter_key="aaa2.cpp", bullet=True)
            dpg.add_text("bbb2.cpp", filter_key="bbb2.cpp", bullet=True)
            dpg.add_text("ccc2.cpp", filter_key="ccc2.cpp", bullet=True)
            dpg.add_text("abc.h", filter_key="abc.h", bullet=True)
            dpg.add_text("hello, world", filter_key="hello, world", bullet=True)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. note::
    * Display everything with *""*
    * Display lines containing xxx with *"xxx"*
    * Display lines containing xxx or yyy with *"xxx,yyy"*
    * Hide lines containing xxx with *"-xxx"*
