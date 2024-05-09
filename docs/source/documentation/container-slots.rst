Container Slots & Children
==========================

Most items can have child items. Items
can only be children to valid **container** items.
Some related commands can be found below:

:py:func:`is_item_container <dearpygui.dearpygui.is_item_container>`
    checks if an item is a container type
:py:func:`get_item_slot <dearpygui.dearpygui.get_item_slot>`
    returns the item's slot
:py:func:`get_item_parent <dearpygui.dearpygui.get_item_parent>`
    returns the item's parent UUID
:py:func:`get_item_children <dearpygui.dearpygui.get_item_children>`
    returns an item’s children
:py:func:`reorder_items <dearpygui.dearpygui.reorder_items>`
    reorders children in a single call
:py:func:`move_item_up <dearpygui.dearpygui.move_item_up>`
    moves an item up within its slot
:py:func:`move_item_down <dearpygui.dearpygui.move_item_down>`
    moves an item down within its slot
:py:func:`move_item <dearpygui.dearpygui.move_item>`
    moves an item between containers
:py:func:`set_item_children <dearpygui.dearpygui.set_item_children>`
    unstaging a stage into an item's children slot

Slots
-----

Items are stored in target slots within their parent container.
Below is the breakdown of slots:

Slot 0:
    **mvFileExtension**, **mvFontRangeHint**, **mvNodeLink**, **mvAnnotation**, **mvDragLine**, **mvDragRect**, **mvDragPoint**, **mvLegend**, **mvTableColumn**
Slot 1:
  Most items
Slot 2:
  Draw items
Slot 3:
    **mvDragPayload**

To query what slot an item belongs to, use
:py:func:`get_item_slot <dearpygui.dearpygui.get_item_slot>`.

Basic Example
-------------

Below is a simple example that demonstrates some of the above:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="about", width=400, height=400):
        dpg.add_button(label="Press me")
        dpg.draw_line((0, 10), (100, 100), color=(255, 0, 0, 255), thickness=1)

    # print children
    print(dpg.get_item_children(dpg.last_root()))

    # print children in slot 1
    print(dpg.get_item_children(dpg.last_root(), 1))

    # check draw_line's slot
    print(dpg.get_item_slot(dpg.last_item()))

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

.. note::
    Use the *slot* keyword with
    :py:func:`get_item_children <dearpygui.dearpygui.get_item_children>`
    to return just a specific slot.

.. note::
    Use the *slot* and *children_only* keywords with
    :py:func:`delete_item <dearpygui.dearpygui.delete_item>`
    to delete a specific slot of children from a parent.

