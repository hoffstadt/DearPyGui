"""DearPyGui's simple contains wrappings of the core API.
"""
from contextlib import contextmanager
from typing import List, Any, Callable, Union
import dearpygui.core as internal_dpg

########################################################################################################################
# context manager container wrappers
########################################################################################################################


@contextmanager
def managed_columns(name: str, columns: int, *, border: bool = True, show: bool = True, parent: str = "",
                    before: str = ""):
    """Wraps add_managed_columns() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        columns: The number of columns to be created.
        **border: Shows a border.
        **show: Decides if the item is shown of not.
        **parent: Parent this item will be added to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)

    Returns:
        None
    """
    try:
        yield internal_dpg.add_managed_columns(name, columns, border=border, show=show, parent=parent, before=before)

    finally:
        internal_dpg.end()


@contextmanager
def window(name: str, *, width: int = 200, height: int = 200, x_pos: int = 200, y_pos: int = 200, autosize: bool = False,
           no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False,
           no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False,
           no_bring_to_front_on_focus: bool = False, menubar: bool = False, no_close: bool = False,
           no_background: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, collapsed: bool = False,
           on_close: Callable = None):
    """Wraps add_window() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **width: Width of the item.
        **height: Height of the item.
        **x_pos: x position the window will start at
        **y_pos: y position the window will start at
        **autosize: Autosized the window to fit it's items.
        **no_resize: Allows for the window size to be changed or fixed
        **no_title_bar: Title name for the title bar of the window
        **no_move: Allows for the window's position to be changed or fixed
        **no_scrollbar: Disable scrollbars (window can still scroll with mouse or programmatically)
        **no_collapse: Disable user collapsing window by double-clicking on it
        **horizontal_scrollbar: Allow horizontal scrollbar to appear (off by default).
        **no_focus_on_appearing: Disable taking focus when transitioning from hidden to visible state
        **no_bring_to_front_on_focus: Disable bringing window to front when taking focus (e.g. clicking on it or
            programmatically giving it focus)
        **menubar: Decides if the menubar is shown or not.
        **no_close: Decides if the window can be closed.
        **no_background:
        **label: Displayed name of the item.
        **show: sets if the item is shown or not window.
        **collapsed: Collapse the window
        **on_close: Callback ran when window is closed

    Returns:
        None
    """
    try:
        if label == "__DearPyGuiDefault":
            yield internal_dpg.add_window(name, width=width, height=height, x_pos=x_pos, y_pos=y_pos, autosize=autosize,
                                          no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move,
                                          no_scrollbar=no_scrollbar, no_collapse=no_collapse,
                                          horizontal_scrollbar=horizontal_scrollbar,
                                          no_focus_on_appearing=no_focus_on_appearing,
                                          no_bring_to_front_on_focus=no_bring_to_front_on_focus,
                                          menubar=menubar, no_close=no_close, no_background=no_background,
                                          show=show, collapsed=collapsed, on_close=on_close)
        else:
            yield internal_dpg.add_window(name, width=width, height=height, x_pos=x_pos, y_pos=y_pos, autosize=autosize,
                                          no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move,
                                          no_scrollbar=no_scrollbar, no_collapse=no_collapse,
                                          horizontal_scrollbar=horizontal_scrollbar,
                                          no_focus_on_appearing=no_focus_on_appearing,
                                          no_bring_to_front_on_focus=no_bring_to_front_on_focus,
                                          menubar=menubar, no_close=no_close,
                                          no_background=no_background, label=label, show=show, 
                                          collapsed=collapsed, on_close=on_close)
    finally:
        internal_dpg.end()


@contextmanager
def menu_bar(name: str, *, show: bool = True, parent: str = "", before: str = ""):
    """Wraps add_menu_bar() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **show: Decides if the item is shown of not.
        **parent: Parent this item will be added to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)

    Returns:
        None
    """
    try:
        yield internal_dpg.add_menu_bar(name, show=show, parent=parent, before=before)
    finally:
        internal_dpg.end()


@contextmanager
def menu(name: str, *, label: str = "__DearPyGuiDefault", show: bool = True, parent: str = "",
         before: str = "", enabled: bool = True):
    """Wraps add_menu() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **label: Displayed name of the item.
        **show: Decides if the item is shown of not.
        **parent: Parent this item will be added to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **enabled: Will enable or disable the menu.

    Returns:
        None
    """
    try: 
        if label == "__DearPyGuiDefault":
            yield internal_dpg.add_menu(name, show=show, parent=parent, before=before, enabled=enabled)
        else:
            yield internal_dpg.add_menu(name, label=label, show=show, parent=parent,
                                        before=before, enabled=enabled)
    finally:
        internal_dpg.end()


@contextmanager
def child(name: str, *, show: bool = True, tip: str = "", parent: str = "", before: str = "", width: int = 0,
          height: int = 0, border: bool = True, autosize_x: bool = False, autosize_y: bool = False,
          no_scrollbar: bool = False, horizontal_scrollbar: bool = False, menubar: bool = False):
    """Wraps add_child() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        show: Decides if the item is shown of not.
        **tip: Adds a simple tooltip
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **width: Width of the item.
        **height: Height of the item.
        **border: Shows/Hides the border around the sides
        **autosize_x: Autosize the window to fit its items in the x.
        **autosize_y: Autosize the window to fit its items in the y.
        **no_scrollbar: Disable scrollbars (window can still scroll with mouse or programmatically)
        **horizontal_scrollbar: Allow horizontal scrollbar to appear (off by default)
        **menubar: adds a bar to add menus

    Returns:
        None
    """
    try: 
        yield internal_dpg.add_child(name, show=show, tip=tip, parent=parent, before=before, width=width,
                                     height=height, border=border, autosize_x=autosize_x, autosize_y=autosize_y,
                                     no_scrollbar=no_scrollbar, horizontal_scrollbar=horizontal_scrollbar,
                                     menubar=menubar)
    finally:
        internal_dpg.end()


@contextmanager
def collapsing_header(name: str, *, label: str = "__DearPyGuiDefault", show: bool = True, 
                      tip: str = "", parent: str = "", before: str = "",closable: bool = False, 
                      default_open: bool = False, open_on_double_click: bool = False, open_on_arrow: bool = False, 
                      leaf: bool = False, bullet: bool = False):
    """Wraps add_collapsing_header() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **label: Displayed name of the item.
        **show: Decides if the item is shown of not.
        **tip: Adds a simple tooltip
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **closable: Decides if the header can be collapsed.
        **default_open: Decides if item is open by default.
        **open_on_double_click: Need double-click to open node.
        **open_on_arrow: Only open when clicking on the arrow part.
        **leaf: No collapsing, no arrow (use as a convenience for leaf nodes).
        **bullet: Display a bullet instead of arrow.


    Returns:
        None
    """
    try:
        if label == "__DearPyGuiDefault":
            yield internal_dpg.add_collapsing_header(name, show=show, tip=tip, parent=parent, before=before, 
                                                     closable=closable, default_open=default_open, 
                                                     open_on_double_click=open_on_double_click,
                                                     open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet)
        else:
            yield internal_dpg.add_collapsing_header(name, show=show, label=label, tip=tip, parent=parent, before=before, 
                                                     closable=closable, default_open=default_open, 
                                                     open_on_double_click=open_on_double_click,
                                                     open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet)
    finally:
        internal_dpg.end()


@contextmanager
def group(name: str, *, show: bool = True, tip: str = "", parent: str = "", before: str = "", width: int = 0,
          horizontal: bool = False, horizontal_spacing: float = -1.0):
    """Wraps add_group() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **show: Decides if the item is shown of not.
        **tip: Adds a simple tooltip
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **width: Width of the item.
        **horizontal: Adds the items on the same row by default.
        **horizontal_spacing: Decides the spacing for the items.

    Returns:
        None
    """
    try:
        yield internal_dpg.add_group(name, show=show, tip=tip, parent=parent, before=before, width=width,
                                     horizontal=horizontal, horizontal_spacing=horizontal_spacing)
    finally:
        internal_dpg.end()

@contextmanager
def node(name: str, *, label: str = "__DearPyGuiDefault", show: bool = True, draggable: bool = True,
         parent: str = "", before: str = "", x_pos: int = 100, y_pos: int = 100):
    """Wraps add_node() and automates calling end().
    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **label: Displayed name of the item.
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **show: sets if the item is shown or not window.
        **draggable: Allow node to be draggable.
        **x_pos: x position the node will start at
        **y_pos: y position the node will start at
    Returns:
        None
    """
    try:
        if label == "__DearPyGuiDefault":
            yield internal_dpg.add_node(name, show=show, parent=parent, before=before, 
                                                     draggable=draggable, x_pos=x_pos, y_pos=y_pos)
        else:
            yield internal_dpg.add_node(name, label=label, show=show, parent=parent, before=before, 
                                                     draggable=draggable, x_pos=x_pos, y_pos=y_pos)
    finally:
        internal_dpg.end()

@contextmanager
def node_attribute(name: str, *, show: bool = True, output: bool = False,
         static: bool = False, parent: str = "", before: str = ""):
    """Wraps add_node_attribute() and automates calling end().
    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **show: sets if the item is shown or not window.
        **output: Set as output attribute
        **static: Set as static attribute
    Returns:
        None
    """
    try:
        yield internal_dpg.add_node_attribute(name, show=show, parent=parent, before=before, 
                                                    output=output, static=static)

    finally:
        internal_dpg.end()

@contextmanager
def node_editor(name: str, *, show: bool = True, parent: str = "", before: str = "", link_callback: Callable = None, delink_callback: Callable = None):
    """Wraps add_node_editor() and automates calling end().
    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **show: sets if the item is shown or not window.
        **link_callback: Callback ran when a new link is created
        **delink_callback: Callback ran when a link is detached
    Returns:
        None
    """
    try:
        yield internal_dpg.add_node_editor(name, show=show, parent=parent, before=before, link_callback=link_callback, delink_callback=delink_callback)
    finally:
        internal_dpg.end()


@contextmanager
def tab_bar(name: str, *, reorderable: bool = False, callback: Callable = None, callback_data: Any = None,  show: bool = True,
            parent: str = "", before: str = ""):
    """Wraps add_tab_bar() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **reorderable: Allows for moveable tabs.
        **callback: Registers a callback.
        **callback_data: Callback data.
        **show: Decides if the item is shown of not.
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)

    Returns:
        None
    """
    try:
        yield internal_dpg.add_tab_bar(name, reorderable=reorderable, callback=callback, callback_data=callback_data,
                                       show=show, parent=parent, before=before)
    finally:
        internal_dpg.end()


@contextmanager
def tab(name: str, *, closable: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "",
        no_reorder: bool = False, leading: bool = False, trailing: bool = False, no_tooltip: bool = False,
        parent: str = "", before: str = ""):
    """Wraps add_tab() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **closable: creates a button on the tab that can hide the tab.
        **label: Displayed name of the item.
        **show: Decides if the item is shown of not.
        **no_reorder: Disable reordering this tab or having another tab cross over this tab
        **leading: Enforce the tab position to the left of the tab bar (after the tab list popup button)
        **trailing: Enforce the tab position to the right of the tab bar (before the scrolling buttons)
        **no_tooltip: Disable tooltip for the given tab
        **tip: Adds a simple tooltip.
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)

    Returns:
        None
    """
    try:
        if label == "__DearPyGuiDefault":
            yield internal_dpg.add_tab(name, closable=closable, show=show, tip=tip, parent=parent, before=before,
                                       no_reorder=no_reorder, leading=leading, trailing=trailing, no_tooltip=no_tooltip)
        else:
            yield internal_dpg.add_tab(name, closable=closable, label=label, show=show, tip=tip, parent=parent,
                                       before=before, no_reorder=no_reorder, leading=leading, trailing=trailing, no_tooltip=no_tooltip)
    finally:
        internal_dpg.end()


@contextmanager
def tree_node(name: str, *, label: str = "__DearPyGuiDefault", show: bool = True, tip: str = "", parent: str = "", 
              before: str = "", default_open: bool = False, open_on_double_click: bool = False, 
              open_on_arrow: bool = False, leaf: bool = False, bullet: bool = False):
    """Wraps add_tree_node() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **label: Displayed name of the item.
        **show: Decides if the item is shown of not.
        **tip: Adds a simple tooltip.
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **default_open: Decides if item is open by default.
        **open_on_double_click: Need double-click to open node.
        **open_on_arrow: Only open when clicking on the arrow part.
        **leaf: No collapsing, no arrow (use as a convenience for leaf nodes).
        **bullet: Display a bullet instead of arrow.

    Returns:
        None
    """
    try:
        if label == "__DearPyGuiDefault":
            yield internal_dpg.add_tree_node(name, show=show, tip=tip, parent=parent,
                                             before=before, default_open=default_open, 
                                             open_on_double_click=open_on_double_click, 
                                             open_on_arrow=open_on_arrow,
                                             leaf=leaf, bullet=bullet)
        else:
            yield internal_dpg.add_tree_node(name, show=show, tip=tip, parent=parent,
                                             before=before, default_open=default_open, 
                                             open_on_double_click=open_on_double_click, 
                                             open_on_arrow=open_on_arrow,
                                             leaf=leaf, bullet=bullet, label=label)
    finally:
        internal_dpg.end()


@contextmanager
def tooltip(tipparent: str, name: str, *, parent: str = "", before: str = "", show: bool = True):
    """Wraps add_tooltip() and automates calling end().

    Args:
        tipparent: Sets the item's tool tip to be the same as the named item's tool tip.
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **show: Decides if the item is shown of not.

    Returns:
        None
    """
    try:
        yield internal_dpg.add_tooltip(tipparent, name, parent=parent, before=before, show=show)
    finally:
        internal_dpg.end()


@contextmanager
def popup(popupparent: str, name: str, *, mousebutton: int = 1, modal: bool = False, parent: str = "", 
          before: str = "", width: int = 0, height: int = 0, show: bool = True):
    """Wraps add_popup() and automates calling end().

    Args:
        popupparent: Parent that the popup will be assigned to.
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **mousebutton: The mouse code that will trigger the popup. Default is 1 or mvMouseButton_Right.
            (mvMouseButton_Left, mvMouseButton_Right, mvMouseButton_Middle, mvMouseButton_X1, mvMouseButton_X2)
        **modal: Makes the popup modal.
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **width: Width of the item.
        **height: Height of the item.
        **show: Decides if the item is shown of not.

    Returns:
        None
    """
    try:
        yield internal_dpg.add_popup(popupparent, name, mousebutton=mousebutton, modal=modal, parent=parent,
                                     before=before, width=width, height=height, show=show)
    finally:
        internal_dpg.end()

########################################################################################################################
# Old Commands
########################################################################################################################


# window configure
def set_window_pos(window: str, x: int, y: int):
    """Sets the top left corner of the window to the specified position.

    Args:
        window: window to position
        x: The x position.
        y: The y position.

    Returns:
        None
    """
    internal_dpg.configure_item(window, x_pos=x, y_pos=y)


def get_window_pos(window: str) -> Union[List[int], None]:
    """Gets the top left corner of the window to the specified position.

    Args:
        window: window to retrieve position
        x: The x position.
        y: The y position.

    Returns:
        list as [x,y] or None
    """
    config = internal_dpg.get_item_configuration(window)
    return [config["x_pos"], config["y_pos"]]


def show_item(item: str):
    """Shows the item.

    Args:
        item: Item to show.

    Returns:
        None
    """
    internal_dpg.configure_item(item, show=True)


def hide_item(item: str, *, children_only: bool = False):
    """Hides the item.

    Args:
        **item: Item to hide.

    Returns:
        None
    """
    if children_only:
        children = internal_dpg.get_item_children(item)
        for child in children:
            internal_dpg.configure_item(child, show=False)
    else:
        internal_dpg.configure_item(item, show=False)


# item configure commands
def set_item_name(item: str, name: str):
    """Sets the item's name, anything after the characters "##" in the name will not be shown.

        If no label is specified then by default this will be the displayed label.

    Args:
        item: Item name will be applied to.
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.

    Returns:
        None
    """
    internal_dpg.configure_item(item, name=name)


def set_item_label(item: str, label: str):
    """Sets the item's displayed label, anything after the characters "##" in the name will not be shown.

    Args:
        item: Item label will be applied to.
        label: Displayed name of the item.

    Returns:
        None
    """
    internal_dpg.configure_item(item, label=label)


def set_item_tip(item: str, tip: str):
    """Sets the item's tip.

    Args:
        item: Item the tip will be applied to.
        tip: tip to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, tip=tip)


def set_item_width(item: str, width: int):
    """Sets the item's width.

    Args:
        item: Item the Width will be applied to.
        width: Width of the item. width to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, width=width)


def set_item_height(item: str, height: int):
    """Sets the item's height.

    Args:
        item: Item the Height will be applied to.
        height: Height of the item. height to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, height=height)


def get_item_label(item: str) -> Union[str, None]:
    """Gets the item's label.

    Returns:
        label as a string or None
    """
    return internal_dpg.get_item_configuration(item)["label"]

def get_item_tip(item: str) -> Union[str, None]:
    """Gets the item's tip.

    Returns:
        tip as a string or None
    """
    return internal_dpg.get_item_configuration(item)["tip"]


def get_item_width(item: str) -> Union[int, None]:
    """Gets the item's width.

    Returns:
        width as a int or None
    """
    return internal_dpg.get_item_configuration(item)["width"]


def get_item_height(item: str) -> Union[int, None]:
    """Gets the item's height.

    Returns:
        height as a int or None
    """
    return internal_dpg.get_item_configuration(item)["height"]


def set_drawing_size(drawing: str, width: int, height: int):
    """Sets the drawing's size, width and height.

    Args:
        drawing: Drawing that will be set.
        width: x axis width to set
        height: y axis height to set

    Returns:
        None
    """
    internal_dpg.configure_item(drawing, width=width, height=height)


def get_drawing_size(drawing: str) -> Union[List[int], None]:
    """Gets the drawing's scale.

    Args:
        drawing: Drawing that will be set.

    Returns:
        list as [width, height] or None
    """
    config = internal_dpg.get_item_configuration(drawing)
    return [config["width"], config["height"]]

def show_style_editor(sender: str="", data: Any=None) -> None:
    """Shows the standard style editor window

    Returns:
        None
    """
    internal_dpg.configure_item("style##standard", show=True)

def show_metrics(sender: str="", data: Any=None) -> None:
    """Shows the standard metrics window

    Returns:
        None
    """
    internal_dpg.configure_item("metrics##standard", show=True)

def show_about(sender: str="", data: Any=None) -> None:
    """Shows the standard about window

    Returns:
        None
    """
    internal_dpg.configure_item("about##standard", show=True)

def show_debug(sender: str="", data: Any=None) -> None:
    """Shows the standard debug window

    Returns:
        None
    """
    internal_dpg.configure_item("debug##standard", show=True)

def show_documentation(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.configure_item("documentation##standard", show=True)

def show_logger() -> None:
    """Shows the logger window

    Returns:
        None
    """
    internal_dpg.show_logger()
