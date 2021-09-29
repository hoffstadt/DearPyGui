Container Stack
===============

Unless an item is a root type or staging mode is active,
all app items need to belong to a valid container item.
An item's parent is deduced through the following process:

1. If item is a root, no parent needed; finished.
2. Check *before* keyword, if used skip to 6 using parent of "before" item.
3. Check *parent* keyword, if used skip to 6.
4. Check container stack, if used skip to 6.
5. If parent is not deduced and staging is active, add item to staging; finished.
6. Check if parent is compatible.
7. Check if parent accepts.
8. If runtime, add item using runtime methods; finished.
9. If startup, add item using startup methods; finished.

Container items can be manually pushed onto the container stack using
:py:func:`push_container_stack <dearpygui.dearpygui.push_container_stack>`
and popped off using
:py:func:`pop_container_stack <dearpygui.dearpygui.pop_container_stack>`.

This process is automated when using :doc:`../documentation/container-context-manager`.
Below is a simple example demonstrating manual stack operations:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.push_container_stack(dpg.add_window(label="Tutorial"))

    dpg.push_container_stack(dpg.add_menu_bar())

    dpg.push_container_stack(dpg.add_menu(label="Themes"))
    dpg.add_menu_item(label="Dark")
    dpg.add_menu_item(label="Light")
    dpg.pop_container_stack()

    # remove menu_bar from container stack
    dpg.pop_container_stack()

    # remove window from container stack
    dpg.pop_container_stack()

    dpg.start_dearpygui()

Explicit Parental Assignment
----------------------------

Parents can be explicitly assigned using the *parent* keyword.
This is most often used for adding new items at runtime.
The above example can be shown again below using explicit parent assignment:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.add_window(label="Tutorial", id="window")

    dpg.add_menu_bar(parent="window", id="menu_bar")

    dpg.add_menu("Themes", parent="menu_bar", id="themes")
    dpg.add_menu_item(label="Dark", parent="themes")
    dpg.add_menu_item(label="Light", parent="themes")

    dpg.start_dearpygui()


Context Managers
----------------

Context managers can be used to simplify the above example. All the context managers can be found [here](https://github.com/hoffstadt/DearPyGui/wiki/Context-Managers) but a simple example can be found below:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    with dpg.window(label="Tutorial"):

        with dpg.menu_bar():

            with dpg.menu(label="Themes"):
                dpg.add_menu_item(label="Dark")
                dpg.add_menu_item(label="Light")
                dpg.add_menu_item(label="Classic")

    dpg.start_dearpygui()

**Benefits**
1. Automatically push the container to the container stack.
2. Automatically pop the container off the container stack.
3. They make the code more readable and structured.