"""A collection of helpful objects for DearPyGui.
"""
from contextlib import contextmanager
from typing import Union, Callable, Any, List, Dict, Tuple
from dearpygui import dearpygui as dpg, _dearpygui as internal_dpg


ItemT = Union[int, str]


@contextmanager
def push_container(item: ItemT) -> ItemT:
    """Push an existing item onto the container stack, then pop it from the
    stack. Use with the `with` statement.

    Args:
        * item (int | str): The tag of an item.


    >>> dpg.create_context()
    >>> dpg.create_viewport()
    >>> dpg.setup_dearpygui()
    >>>
    >>> window_id = dpg.add_window(tag="test window")
    >>>
    >>> with push_container(window_id) as window_id:
    ...     window_id
    'test window'

    """
    try:
        internal_dpg.push_container_stack(item)
        yield item
    finally:
        internal_dpg.pop_container_stack(item)


def is_item_root_item(item: ItemT) -> bool:
    """Return True if the item is a top-level item and cannot be parented.
    Return False otherwise.

    Args:
        * item (int | str): The tag of an item.


    >>> dpg.create_context()
    >>> dpg.create_viewport()
    >>> dpg.setup_dearpygui()
    >>>
    >>> window_id = dpg.add_window(tag="test window")
    >>> button_id = dpg.add_button(tag="test button", parent=window_id)
    >>>
    >>> is_item_root_item(window_id)
    True
    >>> is_item_root_item(button_id)
    False

    """
    if internal_dpg.get_item_info(item)["parent"] is None:
        return True
    return False


def get_slot_info(item: ItemT) -> Tuple[int, int]:
    """Return a tuple containing the child slot the item resides in, and the
    index/item's position in that slot.

    Args:
        * item (int | str): The tag of an item.
    
    
    >>> dpg.create_context()
    >>> dpg.create_viewport()
    >>> dpg.setup_dearpygui()
    >>>
    >>> with dpg.window() as window_id:
    ...     button_id = dpg.add_button()
    ...     with dpg.drawlist(500, 500) as drawlist_id:
    ...         circle_id = dpg.draw_circle([200, 200], 100)
    >>>
    >>> get_slot_info(button_id)
    (1, 0)

    """
    parent = internal_dpg.get_item_info(item)["parent"]
    try:
        p_child_slots = internal_dpg.get_item_info(parent)["children"]
    except KeyError:  # `parent` is None
        raise ValueError("The item does not have a parent.")
    
    for slot, children in p_child_slots.items():
        if item in children:
            return slot, children.index(item)
    else:
        # It should never get to this point.
        raise SystemError("The item could not be found in its parent's child slots.")


def get_item_tree(item: ItemT, descendants_only: bool = False) -> List[Union[ItemT, List]]:
    """Return the entire parential tree that includes the item starting from the
    root parent. Non-table items are represented as 2-item lists `[item, [childs]]`
    where each child of that item is also represented as a 2-item list `[item, [childs]]`.
    Table items (only `mvAppItemType::Table`) are represented as a 3-item list
    `[table_item, [rows], [columns]]`.

    `mvFileExtension`, `mvFontRangeHint`, `mvNodeLink`, `mvAnnotation`, `mvDragLine`,
    `mvDragPoint`, `mvDragPayload`, and `mvLegend` items are excluded from the tree.

    Args:
        * item (int | str): The tag of an item.
        * descendants_only (bool, optional): If True, the tree will start from <item>
        and not the item's root parent. Default is False.


    >>> dpg.create_context()
    >>> dpg.create_viewport()
    >>> dpg.setup_dearpygui()
    >>>
    >>>
    >>> with dpg.window(tag=50) as window1_id:
    ...     with dpg.child_window(tag=51) as child1_id:
    ...         button1_id = dpg.add_button(tag=52)
    ...         button2_id = dpg.add_button(tag=53)
    ...         with dpg.tree_node(tag=55) as tree_id:
    ...             text1_id = dpg.add_text(tag=56)
    ...     text2_id   = dpg.add_text(tag=57)
    ...     button3_id = dpg.add_button(tag=54)
    >>>
    >>> # The output for each of these should be identical to each other.
    >>> get_item_tree(button3_id)
    [50, [[51, [[52, []], [53, []], [55, [[56, []]]]]], [57, []], [54, []]]]
    >>> get_item_tree(text1_id)
    [50, [[51, [[52, []], [53, []], [55, [[56, []]]]]], [57, []], [54, []]]]
    >>> get_item_tree(window1_id)
    [50, [[51, [[52, []], [53, []], [55, [[56, []]]]]], [57, []], [54, []]]]
    >>>
    >>>
    >>> with dpg.window(tag=70) as window2_id:
    ...     with dpg.table(tag=71) as table1_id:
    ...         col1_id = dpg.add_table_column(tag=72)
    ...         col2_id = dpg.add_table_column(tag=73)
    ...         with dpg.table_row(tag=74) as row1_id:
    ...             with dpg.table_cell(tag=75) as cell1:
    ...                 button4_id = dpg.add_button(tag=76)
    ...                 text3_id   = dpg.add_text(tag=77)
    ...         with dpg.table_row(tag=78) as row2_id:
    ...             pass
    ...     text4_id = dpg.add_text(tag=79)
    >>>
    >>> # The output for each of these should be identical to each other.
    >>> get_item_tree(text3_id)
    [70, [[71, [[74, [[75, [[76, []], [77, []]]]]], [78, []]]], [79, []]]]
    >>> get_item_tree(table1_id)
    [70, [[71, [[74, [[75, [[76, []], [77, []]]]]], [78, []]]], [79, []]]]
    >>>
    >>> # The output for this should be different from the above.
    >>> get_item_tree(table1_id, True)
    [71, [[74, [[75, [[76, []], [77, []]]]]], [78, []]]]
    """
    def _get_item_tree(_item: ItemT) -> List[Union[ItemT, List]]:
        """Helper function for `get_item_tree`."""
        tree = [_item, []]

        item_info = internal_dpg.get_item_info(_item)
        child_slots = [*item_info["children"].values()]
        # slots 1 (most items) and 2 (draw items)
        children = [*child_slots[1], *child_slots[2]]

        for child in children:
            child_tree = _get_item_tree(child)
            tree[1].append(child_tree)
        if item_info["type"] == "mvAppItemType::Table":
            # table columns are in slot 0
            tree.append([])  # columns
            for child in child_slots[0]:
                child_tree = _get_item_tree(child)
                tree[2].append(child_tree)
        return tree


    root_item = item if descendants_only else None
    current_item = item
    while root_item is None:
        if is_item_root_item(current_item):
            root_item = current_item
        current_item = internal_dpg.get_item_info(current_item)["parent"]

    return _get_item_tree(root_item)



