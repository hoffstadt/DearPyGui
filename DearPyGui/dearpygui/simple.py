"""DearPyGui's simple contains wrappings of the core API.
"""
from contextlib import contextmanager
from typing import List, Any, Callable, Union
import dearpygui.core as internal_dpg

########################################################################################################################
# context manager container wrappers
########################################################################################################################

@contextmanager
def table(*args, header_row: bool = True, width: int = 0, height: int = 0, inner_width: int = 0, show: bool = True, parent: str = "",
		before: str = "", resizable: bool = False, reorderable: bool = False, hideable: bool = False, sortable: bool = False,   
		context_menu_in_body: bool = False, row_background: bool = False, borders_innerH: bool = False, borders_outerH: bool = False,
		borders_innerV: bool = False, borders_outerV: bool = False, policy: int = 0, no_host_extendX: bool = False,
		no_host_extendY: bool = False, no_keep_columns_visible: bool = False, precise_widths: bool = False, no_clip: bool = False,
		pad_outerX: bool = False, no_pad_outerX: bool = False, no_pad_innerX: bool = False, scrollX: bool = False, scrollY: bool = False,
        id:str=''):
    """Wraps add_table() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
			**header_row: show headers at the top of the columns
			**width: 
			**height: 
			**inner_width:
			**show: Attempt to render
			**parent: Parent this item will be added to. (runtime adding)
			**before: This item will be displayed before the specified item in the parent. (runtime adding)
			**resizable: Enable resizing columns
	        **reorderable: Enable reordering columns in header row
	        **hideable: Enable hiding/disabling columns in context menu.
	        **sortable: Enable sorting.
	        **context_menu_in_body: Right-click on columns body/contents will display table context menu.
			**row_background: Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)
	        **borders_innerH: Draw horizontal borders between rows.
	        **borders_outerH: Draw horizontal borders at the top and bottom.
	        **borders_innerV: Draw vertical borders between columns.
	        **borders_outerV: Draw vertical borders on the left and right sides.
	        **policy: sizing policy
			**no_host_extendX: Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.
			**no_host_extendY: Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.
			**no_keep_columns_visible: Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.
			**precise_widths: Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.
			**no_clip: Disable clipping rectangle for every individual columns.
	        **pad_outerX: Default if BordersOuterV is on. Enable outer-most padding. Generally desirable if you have headers.
	        **no_pad_outerX: Default if BordersOuterV is off. Disable outer-most padding.
	        **no_pad_innerX: Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).
	        **scollX: Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this create a child window, ScrollY is currently generally recommended when using ScrollX.
	        **scollY: Enable horizontal vertical.

    Returns:
        None
    """
    try:
        yield internal_dpg.add_table(*args, header_row=header_row, width = width, height = height, inner_width = inner_width,
		    show = show, parent = parent, before = before, resizable = resizable, reorderable = reorderable, hideable = hideable,
		    sortable = sortable, context_menu_in_body = context_menu_in_body, row_background = row_background,
		    borders_innerH = borders_innerH, borders_outerH = borders_outerH, borders_innerV = borders_innerV,
		    borders_outerV = borders_outerV, policy = policy, no_host_extendX = no_host_extendX,
		    no_host_extendY = no_host_extendY, no_keep_columns_visible = no_keep_columns_visible, precise_widths = precise_widths,
		    no_clip = no_clip, pad_outerX = pad_outerX, no_pad_outerX = no_pad_outerX, no_pad_innerX = no_pad_innerX,
		    scrollX = scrollX, scrollY = scrollY, id=id)
    finally:
        internal_dpg.end()

@contextmanager
def window(*args, width: int = 200, height: int = 200, autosize: bool = False,
           no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False,
           no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False,
           no_bring_to_front_on_focus: bool = False, menubar: bool = False, no_close: bool = False,
           no_background: bool = False, label: str = "__DearPyGuiDefault", show: bool = True, collapsed: bool = False,
           on_close: Callable = None, min_size: List[int]=[32, 32], max_size: List[int] = [30000, 30000], id:str=''):
    """Wraps add_window() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **width: Width of the item.
        **height: Height of the item.
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
        **on_close: Callback ran when window is closed
        **min_size: Minimum window size
        **max_size: Maximum window size

    Returns:
        None
    """
    try:
        if label == "__DearPyGuiDefault":
            yield internal_dpg.add_window(*args, width=width, height=height, autosize=autosize,
                                          no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move,
                                          no_scrollbar=no_scrollbar, no_collapse=no_collapse,
                                          horizontal_scrollbar=horizontal_scrollbar,
                                          no_focus_on_appearing=no_focus_on_appearing,
                                          no_bring_to_front_on_focus=no_bring_to_front_on_focus,
                                          menubar=menubar, no_close=no_close, no_background=no_background,
                                          show=show, collapsed=collapsed, on_close=on_close,
                                          min_size=min_size, max_size=max_size, id=id)
        else:
            yield internal_dpg.add_window(*args, width=width, height=height, autosize=autosize,
                                          no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move,
                                          no_scrollbar=no_scrollbar, no_collapse=no_collapse,
                                          horizontal_scrollbar=horizontal_scrollbar,
                                          no_focus_on_appearing=no_focus_on_appearing,
                                          no_bring_to_front_on_focus=no_bring_to_front_on_focus,
                                          menubar=menubar, no_close=no_close,
                                          no_background=no_background, label=label, show=show, 
                                          collapsed=collapsed, on_close=on_close,
                                          min_size=min_size, max_size=max_size, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def menu_bar(*args, show: bool = True, parent: str = "", before: str = "", id:str=''):
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
        yield internal_dpg.add_menu_bar(*args, show=show, parent=parent, before=before, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def menu(*args, label: str = "__DearPyGuiDefault", show: bool = True, parent: str = "",
         before: str = "", enabled: bool = True, id:str=''):
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
            yield internal_dpg.add_menu(*args, show=show, parent=parent, before=before, enabled=enabled, id=id)
        else:
            yield internal_dpg.add_menu(*args, label=label, show=show, parent=parent,
                                        before=before, enabled=enabled, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def child(*args, show: bool = True, parent: str = "", before: str = "", width: int = 0,
          height: int = 0, border: bool = True, autosize_x: bool = False, autosize_y: bool = False,
          no_scrollbar: bool = False, horizontal_scrollbar: bool = False, menubar: bool = False, id:str=''):
    """Wraps add_child() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        show: Decides if the item is shown of not.
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
        yield internal_dpg.add_child(*args, show=show, parent=parent, before=before, width=width,
                                     height=height, border=border, autosize_x=autosize_x, autosize_y=autosize_y,
                                     no_scrollbar=no_scrollbar, horizontal_scrollbar=horizontal_scrollbar,
                                     menubar=menubar, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def collapsing_header(*args, label: str = "__DearPyGuiDefault", show: bool = True,
                     parent: str = "", before: str = "",closable: bool = False, 
                      default_open: bool = False, open_on_double_click: bool = False, open_on_arrow: bool = False, 
                      leaf: bool = False, bullet: bool = False, id:str=''):
    """Wraps add_collapsing_header() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **label: Displayed name of the item.
        **show: Decides if the item is shown of not.
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
            yield internal_dpg.add_collapsing_header(*args, show=show, parent=parent, before=before, 
                                                     closable=closable, default_open=default_open, 
                                                     open_on_double_click=open_on_double_click,
                                                     open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet, id=id)
        else:
            yield internal_dpg.add_collapsing_header(*args, show=show, label=label, parent=parent, before=before, 
                                                     closable=closable, default_open=default_open, 
                                                     open_on_double_click=open_on_double_click,
                                                     open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def group(*args, show: bool = True, parent: str = "", before: str = "", width: int = 0,
          horizontal: bool = False, horizontal_spacing: float = -1.0, id:str=''):
    """Wraps add_group() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **show: Decides if the item is shown of not.
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)
        **width: Width of the item.
        **horizontal: Adds the items on the same row by default.
        **horizontal_spacing: Decides the spacing for the items.

    Returns:
        None
    """
    try:
        yield internal_dpg.add_group(*args, show=show, parent=parent, before=before, width=width,
                                     horizontal=horizontal, horizontal_spacing=horizontal_spacing, id=id)
    finally:
        internal_dpg.end()

@contextmanager
def node(*args, label: str = "__DearPyGuiDefault", show: bool = True, draggable: bool = True,
         parent: str = "", before: str = "", x_pos: int = 100, y_pos: int = 100, id:str=''):
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
            yield internal_dpg.add_node(*args, show=show, parent=parent, before=before, 
                                                     draggable=draggable, x_pos=x_pos, y_pos=y_pos, id=id)
        else:
            yield internal_dpg.add_node(*args, label=label, show=show, parent=parent, before=before, 
                                                     draggable=draggable, x_pos=x_pos, y_pos=y_pos, id=id)
    finally:
        internal_dpg.end()

@contextmanager
def node_attribute(*args, show: bool = True, output: bool = False,
         static: bool = False, parent: str = "", before: str = "", shape: int = 54010, id:str=''):
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
        yield internal_dpg.add_node_attribute(*args, show=show, parent=parent, before=before, 
                                                    output=output, static=static, shape=shape, id=id)

    finally:
        internal_dpg.end()

@contextmanager
def node_editor(*args, show: bool = True, parent: str = "", before: str = "", callback: Callable = None, 
                delink_callback: Callable = None, id:str=''):
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
        yield internal_dpg.add_node_editor(*args, show=show, parent=parent, before=before, 
                                           callback=callback, delink_callback=delink_callback, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def tab_bar(*args, reorderable: bool = False, callback: Callable = None, callback_data: Any = None,  show: bool = True,
            parent: str = "", before: str = "", id:str=''):
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
        yield internal_dpg.add_tab_bar(*args, reorderable=reorderable, callback=callback, callback_data=callback_data,
                                       show=show, parent=parent, before=before, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def tab(*args, closable: bool = False, label: str = "__DearPyGuiDefault", show: bool = True,
        no_reorder: bool = False, leading: bool = False, trailing: bool = False, no_tooltip: bool = False,
        parent: str = "", before: str = "", id:str=''):
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
        **parent: Parent to add this item to. (runtime adding)
        **before: This item will be displayed before the specified item in the parent. (runtime adding)

    Returns:
        None
    """
    try:
        if label == "__DearPyGuiDefault":
            yield internal_dpg.add_tab(*args, closable=closable, show=show, parent=parent, before=before,
                                       no_reorder=no_reorder, leading=leading, trailing=trailing, no_tooltip=no_tooltip, id=id)
        else:
            yield internal_dpg.add_tab(*args, closable=closable, label=label, show=show, parent=parent,
                                       before=before, no_reorder=no_reorder, leading=leading, trailing=trailing, no_tooltip=no_tooltip, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def tree_node(*args, label: str = "__DearPyGuiDefault", show: bool = True, parent: str = "", 
              before: str = "", default_open: bool = False, open_on_double_click: bool = False, 
              open_on_arrow: bool = False, leaf: bool = False, bullet: bool = False, id:str=''):
    """Wraps add_tree_node() and automates calling end().

    Args:
        name: Unique name used to programmatically refer to the item. If label is unused this will be the label,
            anything after "##" that occurs in the name will not be shown on screen.
        **label: Displayed name of the item.
        **show: Decides if the item is shown of not.
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
            yield internal_dpg.add_tree_node(*args, show=show, parent=parent,
                                             before=before, default_open=default_open, 
                                             open_on_double_click=open_on_double_click, 
                                             open_on_arrow=open_on_arrow,
                                             leaf=leaf, bullet=bullet, id=id)
        else:
            yield internal_dpg.add_tree_node(*args, show=show, parent=parent,
                                             before=before, default_open=default_open, 
                                             open_on_double_click=open_on_double_click, 
                                             open_on_arrow=open_on_arrow,
                                             leaf=leaf, bullet=bullet, label=label, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def tooltip(*args, parent: str = "", before: str = "", show: bool = True, id:str=''):
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
        yield internal_dpg.add_tooltip(*args, parent=parent, before=before, show=show, id=id)
    finally:
        internal_dpg.end()


@contextmanager
def popup(*args, mousebutton: int = 1, modal: bool = False, parent: str = "", 
          before: str = "", width: int = 0, height: int = 0, show: bool = True, id:str=''):
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
        yield internal_dpg.add_popup(*args, mousebutton=mousebutton, modal=modal, parent=parent,
                                     before=before, width=width, height=height, show=show, id=id)
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


def is_item_shown(item: str) -> Union[bool, None]:
    """Checks if item is shown.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_configuration(item)["show"]

def is_item_enabled(item: str) -> Union[bool, None]:
    """Checks if item is enabled.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_configuration(item)["enabled"]


def is_item_container(item: str) -> Union[bool, None]:
    """Checks if item is a container.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_info(item)["container"]

def is_item_hovered(item: str) -> Union[bool, None]:
    """Checks if item is hovered.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["hovered"]


def is_item_active(item: str) -> Union[bool, None]:
    """Checks if item is active.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["active"]

def is_item_focused(item: str) -> Union[bool, None]:
    """Checks if item is focused.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["focused"]

def is_item_clicked(item: str) -> Union[bool, None]:
    """Checks if item is clicked.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["clicked"]

def is_item_visible(item: str) -> Union[bool, None]:
    """Checks if item is visible.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["visible"]


def is_item_edited(item: str) -> Union[bool, None]:
    """Checks if item is edited.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["edited"]


def is_item_activated(item: str) -> Union[bool, None]:
    """Checks if item is activated.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["activated"]


def is_item_deactivated(item: str) -> Union[bool, None]:
    """Checks if item is deactivated.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["deactivated"]


def is_item_deactivated_after_edit(item: str) -> Union[bool, None]:
    """Checks if item is deactivated_after_edit.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["deactivated_after_edit"]


def is_item_toggled_open(item: str) -> Union[bool, None]:
    """Checks if item is toggled_open.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["toggled_open"]


def is_item_ok(item: str) -> Union[bool, None]:
    """Checks if item is a ok.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["ok"]


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

def get_item_callback(item: str) -> Union[str, None]:
    """Gets the item's callback.

    Returns:
        callback as a string or None
    """
    return internal_dpg.get_item_configuration(item)["callback"]

def get_item_callback_data(item: str) -> Union[Any, None]:
    """Gets the item's callback data.

    Returns:
        callback data as a python object or None
    """
    return internal_dpg.get_item_configuration(item)["callback_data"]

def get_item_source(item: str) -> Union[str, None]:
    """Gets the item's source.

    Returns:
        source as a string or None
    """
    return internal_dpg.get_item_configuration(item)["source"]


def get_item_parent(item: str) -> Union[str, None]:
    """Gets the item's parent.

    Returns:
        parent as a string or None
    """
    return internal_dpg.get_item_info(item)["parent"]


def get_item_children(item: str) -> Union[List[str], None]:
    """Gets the item's children.

    Returns:
        children as a list of string or None
    """
    return internal_dpg.get_item_info(item)["children"]


def get_item_type(item: str) -> Union[str]:
    """Gets the item's type.

    Returns:
        type as a string or None
    """
    return internal_dpg.get_item_info(item)["type"]


def get_item_rect_size(item: str) -> Union[List[float]]:
    """Gets the item's current size.

    Returns:
        size as a float list
    """
    return internal_dpg.get_item_state(item)["rect_size"]


def get_item_rect_min(item: str) -> Union[float]:
    """Gets the item's current rect_min.

    Returns:
        rect_min as a float
    """
    return internal_dpg.get_item_state(item)["rect_min"]


def get_item_rect_max(item: str) -> Union[float]:
    """Gets the item's current rect_max.

    Returns:
        rect_max as a float
    """
    return internal_dpg.get_item_state(item)["rect_max"]


def get_item_pos(item: str) -> Union[float]:
    """Gets the item's current pos.

    Returns:
        pos as a float
    """
    return internal_dpg.get_item_state(item)["pos"]


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
