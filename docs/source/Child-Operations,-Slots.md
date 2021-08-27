
| [Basics](#basics) - [Slots](#slots) - [Basic Example](#basic-example) - [Tips](#tips) |
|----|
||

## Basics
Most app items can have child app items. App items can only be children to valid **container** items with the exception of item event handlers, which can belong to non-container app items. Some related commands can be found below:

| command | description |
|---------|-------------|
| `is_item_container(...)` | checks if an item is a container type |
| `get_item_slot(...)` | returns the item's target slot |
| `get_item_parent(...)` | returns the item's parent's UUID |
| `get_item_children(...)` | returns an item's children |
| `reorder_items(...)` | reorders children in a single call |
| `move_item_up(...)` | moves an item within its slot |
| `move_item_down(...)` | moves an item within its slot |
| `move_item(...)` | moves an item anywhere |
| `set_item_children(...)` | unstaging a staging container |

## Slots
App items are stored in target slots within their parent container. Below is the breakdown of slots:
* _Slot 0_: **mvFileExtension**, **mvFontRangeHint**, **mvNodeLink**, **mvAnnotation**, **mvDragLine**, **mvDragPoint**, **mvLegend**, **mvTableColumn**
* _Slot 1_: All other app items
* _Slot 2_: Draw items
* _Slot 3_: item event handlers
* _Slot 4_: **mvDragPayload**

To query what slot an item belongs to, use `get_item_slot(...)`.

## Basic Example
Below is a simple example that demonstrates some of the above:
```python
import dearpygui.dearpygui as dpg

dpg.setup_registries()

with dpg.window(label="tutorial"):
    dpg.add_button(label="Press me")
    dpg.draw_line((10, 10), (100, 100), color=(255, 0, 0, 255), thickness=1)

# print children    
print(dpg.get_item_children(dpg.last_root()))

# print children in slot 1
print(dpg.get_item_children(dpg.last_root(), 1))

# check draw_line's slot
print(dpg.get_item_slot(dpg.last_item()))


dpg.start_dearpygui()

```

## Tips
* Use the `slot` keyword with `get_item_children(...)` to return just a specific slot.

