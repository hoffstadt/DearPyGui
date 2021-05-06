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
        id:str='', indent=-1, callback: Callable = None, sort_multi: bool = False, sort_tristate: bool = False):
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
        widget = internal_dpg.add_table(*args, header_row=header_row, width = width, height = height, inner_width = inner_width,
		    show = show, parent = parent, before = before, resizable = resizable, reorderable = reorderable, hideable = hideable,
		    sortable = sortable, context_menu_in_body = context_menu_in_body, row_background = row_background,
		    borders_innerH = borders_innerH, borders_outerH = borders_outerH, borders_innerV = borders_innerV,
		    borders_outerV = borders_outerV, policy = policy, no_host_extendX = no_host_extendX,
		    no_host_extendY = no_host_extendY, no_keep_columns_visible = no_keep_columns_visible, precise_widths = precise_widths,
		    no_clip = no_clip, pad_outerX = pad_outerX, no_pad_outerX = no_pad_outerX, no_pad_innerX = no_pad_innerX,
		    scrollX = scrollX, scrollY = scrollY, id=id, indent=indent, callback=callback, sort_multi=sort_multi,
            sort_tristate=sort_tristate)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def table_row(*args, id:str=''):
    try:
        widget = internal_dpg.add_table_row(*args, id=id)
        internal_dpg.push_container_stack(widget)
        yield widget

    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def window(*args, width: int = 200, height: int = 200, autosize: bool = False,
           no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False,
           no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False,
           no_bring_to_front_on_focus: bool = False, menubar: bool = False, no_close: bool = False,
           no_background: bool = False, label: str = '', show: bool = True, collapsed: bool = False,
           modal: bool = False, popup: bool = False,
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
        **modal: Sets window to modal mode
        **popup: Sets window to popup mode

    Returns:
        None
    """
    try:

        widget = internal_dpg.add_window(*args, width=width, height=height, autosize=autosize,
                                        no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move,
                                        no_scrollbar=no_scrollbar, no_collapse=no_collapse,
                                        horizontal_scrollbar=horizontal_scrollbar,
                                        no_focus_on_appearing=no_focus_on_appearing,
                                        no_bring_to_front_on_focus=no_bring_to_front_on_focus,
                                        menubar=menubar, no_close=no_close,
                                        no_background=no_background, label=label, show=show, 
                                        collapsed=collapsed, on_close=on_close,
                                        min_size=min_size, max_size=max_size, id=id, modal=modal,
                                        popup=popup)
        internal_dpg.push_container_stack(widget)
        yield widget

    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def menu_bar(*args, show: bool = True, parent: str = "", before: str = "", id:str='', indent=-1):
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
        widget = internal_dpg.add_menu_bar(*args, show=show, parent=parent, before=before, id=id, indent=indent)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def menu(*args, label: str = "", show: bool = True, parent: str = "",
         before: str = "", enabled: bool = True, id:str='', indent=-1):
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
        widget = internal_dpg.add_menu(*args, label=label, show=show, parent=parent,
                                    before=before, enabled=enabled, id=id, indent=indent)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def child(*args, show: bool = True, parent: str = "", before: str = "", width: int = 0,
          height: int = 0, border: bool = True, autosize_x: bool = False, autosize_y: bool = False,
          no_scrollbar: bool = False, horizontal_scrollbar: bool = False, menubar: bool = False, id:str='', 
          indent=-1):
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
        widget = internal_dpg.add_child(*args, show=show, parent=parent, before=before, width=width,
                                     height=height, border=border, autosize_x=autosize_x, autosize_y=autosize_y,
                                     no_scrollbar=no_scrollbar, horizontal_scrollbar=horizontal_scrollbar,
                                     menubar=menubar, id=id, indent=indent)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def collapsing_header(*args, label: str = "", show: bool = True,
                     parent: str = "", before: str = "",closable: bool = False, 
                      default_open: bool = False, open_on_double_click: bool = False, open_on_arrow: bool = False, 
                      leaf: bool = False, bullet: bool = False, id:str='', indent=-1):
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
        widget = internal_dpg.add_collapsing_header(*args, show=show, label=label, parent=parent, before=before, 
                                                    closable=closable, default_open=default_open, 
                                                    open_on_double_click=open_on_double_click,
                                                    open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet, id=id,
                                                    indent=indent)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def group(*args, show: bool = True, parent: str = "", before: str = "", width: int = 0,
          horizontal: bool = False, horizontal_spacing: float = -1.0, id:str='', indent=-1):
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
        widget = internal_dpg.add_group(*args, show=show, parent=parent, before=before, width=width,
                                     horizontal=horizontal, horizontal_spacing=horizontal_spacing, id=id,
                                     indent=indent)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def node(*args, label: str = "", show: bool = True, draggable: bool = True,
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
        widget = internal_dpg.add_node(*args, label=label, show=show, parent=parent, before=before, 
                                                    draggable=draggable, x_pos=x_pos, y_pos=y_pos, id=id)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def node_attribute(*args, show: bool = True, output: bool = False,
         static: bool = False, parent: str = "", before: str = "", shape: int = 54010, id:str=''
         , indent=-1):
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
        widget = internal_dpg.add_node_attribute(*args, show=show, parent=parent, before=before, 
                                                    output=output, static=static, shape=shape, id=id,
                                                    indent=indent)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

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
        widget = internal_dpg.add_node_editor(*args, show=show, parent=parent, before=before, 
                                           callback=callback, delink_callback=delink_callback, id=id)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def staging_container(*args, id:str=''):
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
        widget = internal_dpg.add_staging_container(id=id)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def tab_bar(*args, reorderable: bool = False, callback: Callable = None, callback_data: Any = None,  show: bool = True,
            parent: str = "", before: str = "", id:str='', indent=-1):
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
        widget = internal_dpg.add_tab_bar(*args, reorderable=reorderable, callback=callback, callback_data=callback_data,
                                       show=show, parent=parent, before=before, id=id, indent=indent)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def tab(*args, closable: bool = False, label: str = "", show: bool = True,
        no_reorder: bool = False, leading: bool = False, trailing: bool = False, no_tooltip: bool = False,
        parent: str = "", before: str = "", id:str='', indent=-1):
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
        widget = internal_dpg.add_tab(*args, closable=closable, label=label, show=show, parent=parent,
                                    before=before, no_reorder=no_reorder, leading=leading, 
                                    trailing=trailing, no_tooltip=no_tooltip, id=id, indent=indent)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def tree_node(*args, label: str = "", show: bool = True, parent: str = "", 
              before: str = "", default_open: bool = False, open_on_double_click: bool = False, 
              open_on_arrow: bool = False, leaf: bool = False, bullet: bool = False, id:str='',
              selectable: bool = False, indent=-1):
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
        widget = internal_dpg.add_tree_node(*args, show=show, parent=parent,
                                            before=before, default_open=default_open, 
                                            open_on_double_click=open_on_double_click, 
                                            open_on_arrow=open_on_arrow,
                                            leaf=leaf, bullet=bullet, label=label, id=id, selectable=selectable,
                                            indent=indent)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


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
        widget = internal_dpg.add_tooltip(*args, parent=parent, before=before, show=show, id=id)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


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
        widget = internal_dpg.add_popup(*args, mousebutton=mousebutton, modal=modal, parent=parent,
                                     before=before, width=width, height=height, show=show, id=id)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

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
        children = get_item_children(item)
        for child in children:
            internal_dpg.configure_item(child, show=False)
    else:
        internal_dpg.configure_item(item, show=False)

def enable_item(item: str):
    """Enables the item.

    Args:
        **item: Item to enable.

    Returns:
        None
    """
    internal_dpg.configure_item(item, enabled=True)


def disable_item(item: str):
    """Disables the item.

    Args:
        **item: Item to disable.

    Returns:
        None
    """
    internal_dpg.configure_item(item, enable=False)


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


def set_item_label(item: str, label: str):
    """Sets the item's displayed label, anything after the characters "##" in the name will not be shown.

    Args:
        item: Item label will be applied to.
        label: Displayed name to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, label=label)


def set_item_source(item: str, source: str):
    """Sets the item's value, to the source's value. Widget's value will now be "linked" to source's value.

    Args:
        item: Item to me linked.
        source: Source to link to.

    Returns:
        None
    """
    internal_dpg.configure_item(item, source=source)


def set_item_width(item: str, width: int):
    """Sets the item's width.

    Args:
        item: Item the Width will be applied to.
        width: Width to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, width=width)


def set_item_height(item: str, height: int):
    """Sets the item's height.

    Args:
        item: Item the Height will be applied to.
        height: Height to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, height=height)

def set_item_callback(item: str, callback: Callable):
    """Sets the item's callack.

    Args:
        item: Item the callback will be applied to.
        callback: Callback to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, callback=callback)


def set_item_callback_data(item: str, callback_data: Any):
    """Sets the item's callack_data to any python object.

    Args:
        item: Item the callback will be applied to.
        callback_data: Callback_data to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, callback_data=callback_data)


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


def get_item_rect_size(item: str) -> Union[List[int]]:
    """Gets the item's current size.

    Returns:
        size as a float list
    """
    return internal_dpg.get_item_state(item)["rect_size"]


def get_item_rect_min(item: str) -> Union[List[int]]:
    """Gets the item's current rect_min.

    Returns:
        rect_min as a float
    """
    return internal_dpg.get_item_state(item)["rect_min"]


def get_item_rect_max(item: str) -> Union[List[int]]:
    """Gets the item's current rect_max.

    Returns:
        rect_max as a float
    """
    return internal_dpg.get_item_state(item)["rect_max"]


def get_item_pos(item: str) -> Union[List[int]]:
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
    internal_dpg.show_tool("mvStyleWindow")

def show_metrics(sender: str="", data: Any=None) -> None:
    """Shows the standard metrics window

    Returns:
        None
    """
    internal_dpg.show_tool("mvMetricsWindow")

def show_about(sender: str="", data: Any=None) -> None:
    """Shows the standard about window

    Returns:
        None
    """
    internal_dpg.show_tool("mvAboutWindow")

def show_debug(sender: str="", data: Any=None) -> None:
    """Shows the standard debug window

    Returns:
        None
    """
    internal_dpg.show_tool("mvDebugWindow")

def show_documentation(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool("mvDocWindow")

def show_font_manager(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool("mvFontManager")

def show_item_registry(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool("mvItemRegistry")

def set_theme() -> None:
    """sets theme

    Returns:
        None
    """
    if("dark"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 128, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 15, 135, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 26, 102, 191, 199)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 26, 102, 191, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 36, 36, 36, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 61, 133, 224, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 61, 133, 224, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 15, 135, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 15, 135, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 66, 150, 250, 79)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 66, 150, 250, 79)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 66, 150, 250, 79)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 156, 156, 156, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 255, 110, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 10, 10, 10, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 41, 74, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 0, 0, 0, 130)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 36, 36, 36, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 15, 15, 15, 240)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 26, 102, 191, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 66, 150, 250, 79)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 204, 204, 204, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 66, 150, 250, 79)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 128, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 15, 135, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 61, 133, 224, 30)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 61, 133, 224, 150)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 35, 35, 35, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 62, 62, 62, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 75, 75, 75, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 75, 75, 75, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 199, 199, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 255, 255, 50, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 48, 48, 51, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 79, 79, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 59, 59, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 26, 102, 191, 199)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 26, 102, 191, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 61, 133, 224, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 61, 133, 224, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 41, 74, 122, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 255, 255, 255, 200)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 66, 150, 250, 255)
    elif("classic"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 89, 102, 156, 158)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 102, 122, 181, 201)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 117, 138, 204, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 230, 230, 230, 128)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 86, 86, 174, 200)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 115, 115, 230, 204)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 103, 103, 185, 215)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 115, 115, 230, 204)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 102, 122, 181, 201)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 115, 115, 230, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 102, 102, 140, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 102, 102, 204, 77)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 102, 102, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 255, 255, 255, 77)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 255, 255, 255, 77)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 89, 102, 156, 158)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 102, 122, 181, 201)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 117, 138, 204, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 89, 102, 156, 158)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 102, 122, 181, 201)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 117, 138, 204, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 102, 102, 230, 115)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 115, 115, 230, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 135, 135, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 128, 128, 128, 153)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 230, 230, 230, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 102, 102, 230, 115)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 102, 102, 204, 77)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 102, 102, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 102, 102, 230, 115)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 115, 115, 230, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 135, 135, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 89, 102, 156, 158)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 102, 122, 181, 201)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 102, 102, 204, 77)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 102, 102, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 69, 69, 138, 212)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 82, 82, 161, 222)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 102, 102, 204, 51)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 102, 102, 140, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 0, 0, 0, 179)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 102, 102, 204, 77)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 102, 102, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 179, 179, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 255, 255, 255, 41)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 199, 209, 255, 153)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 199, 209, 255, 230)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 51, 51, 51, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 102, 102, 204, 77)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 102, 102, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 115, 115, 230, 204)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 135, 135, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 115, 115, 230, 204)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 102, 122, 181, 201)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 102, 122, 181, 201)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 102, 102, 230, 115)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 102, 102, 204, 77)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 102, 102, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 102, 122, 181, 201)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 117, 138, 204, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 128, 128, 128, 153)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 28, 28, 36, 235)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 69, 69, 138, 212)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 102, 102, 204, 51)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 82, 82, 161, 222)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 82, 82, 161, 222)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 69, 69, 138, 212)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 102, 102, 204, 51)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 153, 153, 179, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 179, 179, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 69, 69, 138, 212)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 102, 102, 204, 51)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 255, 255, 255, 77)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 120, 120, 176, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 107, 105, 163, 176)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 255, 255, 255, 77)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 105, 99, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 230, 230, 230, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 110, 110, 110, 99)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 128, 128, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 89, 102, 156, 158)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 102, 122, 181, 201)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 117, 138, 204, 255)
    elif("light"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 15, 135, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 195, 203, 213, 237)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 152, 186, 225, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 219, 219, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 250, 250, 250, 135)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 176, 176, 176, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 125, 125, 125, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 125, 125, 125, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 66, 150, 250, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 117, 138, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 66, 150, 250, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 117, 138, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 15, 135, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 15, 135, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 66, 150, 250, 242)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 66, 150, 250, 242)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 66, 150, 250, 79)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 99, 99, 99, 158)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 66, 150, 250, 79)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 250, 250, 250, 135)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 176, 176, 176, 204)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 125, 125, 125, 204)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 125, 125, 125, 255)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 66, 150, 250, 79)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 250, 250, 250, 135)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 176, 176, 176, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 125, 125, 125, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 125, 125, 125, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 99, 99, 99, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 255, 110, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 255, 115, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 245, 245, 245, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 209, 209, 209, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 255, 255, 255, 130)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 219, 219, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 240, 240, 240, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 250, 250, 250, 135)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 176, 176, 176, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 125, 125, 125, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 125, 125, 125, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 36, 112, 204, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 204, 204, 204, 143)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 66, 150, 250, 242)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 51, 51, 51, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 250, 250, 250, 135)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 176, 176, 176, 204)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 125, 125, 125, 204)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 125, 125, 125, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 66, 150, 250, 79)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 250, 250, 250, 135)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 176, 176, 176, 204)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 125, 125, 125, 204)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 125, 125, 125, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 15, 135, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 99, 99, 99, 158)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 255, 255, 255, 250)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 250, 250, 250, 135)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 245, 245, 245, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 255, 255, 255, 130)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 209, 209, 209, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 209, 209, 209, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 245, 245, 245, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 255, 255, 255, 130)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 36, 112, 204, 199)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 36, 112, 204, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 245, 245, 245, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 255, 255, 255, 130)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 66, 150, 250, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 117, 138, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 66, 150, 250, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 117, 138, 204, 153)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 0, 0, 0, 77)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 66, 150, 250, 102)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 15, 135, 250, 255)
    elif("purple"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 236, 164, 0, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 67, 72, 69, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 114, 90, 193, 204)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 165, 141, 244, 204)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 140, 116, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 114, 90, 193, 153)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 165, 141, 244, 153)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 140, 116, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 192, 185, 252, 204)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 165, 141, 244, 204)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 192, 185, 252, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 0, 0, 0, 51)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 133, 138, 135, 128)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 108, 113, 110, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 159, 164, 161, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 133, 138, 135, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 262, 190, 26, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 262, 190, 26, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 236, 164, 0, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 114, 90, 193, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 165, 141, 244, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 140, 116, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 236, 164, 0, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 114, 90, 193, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 165, 141, 244, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 140, 116, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 236, 164, 0, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 236, 164, 0, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 141, 134, 201, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 192, 185, 252, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 141, 134, 201, 204)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 133, 138, 135, 128)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 108, 113, 110, 204)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 159, 164, 161, 204)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 133, 138, 135, 255)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 141, 134, 201, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 192, 185, 252, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 114, 90, 193, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 165, 141, 244, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 133, 138, 135, 128)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 108, 113, 110, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 159, 164, 161, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 133, 138, 135, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 287, 215, 51, 204)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 287, 215, 51, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 31, 36, 33, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 57, 62, 59, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 31, 36, 33, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 133, 138, 135, 128)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 108, 113, 110, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 159, 164, 161, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 133, 138, 135, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 159, 164, 161, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 114, 90, 193, 51)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 165, 141, 244, 51)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 140, 116, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 0, 0, 0, 153)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 133, 138, 135, 128)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 108, 113, 110, 204)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 159, 164, 161, 204)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 133, 138, 135, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 192, 185, 252, 204)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 192, 185, 252, 204)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 165, 141, 244, 204)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 165, 141, 244, 204)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 141, 134, 201, 204)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 133, 138, 135, 128)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 108, 113, 110, 204)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 159, 164, 161, 204)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 133, 138, 135, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 67, 72, 69, 204)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 165, 141, 244, 204)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 140, 116, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 31, 36, 33, 230)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 0, 0, 0, 51)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 133, 138, 135, 128)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 31, 36, 33, 204)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 57, 62, 59, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 57, 62, 59, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 31, 36, 33, 204)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 184, 189, 186, 204)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 159, 164, 161, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 31, 36, 33, 204)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 262, 190, 26, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 192, 185, 252, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 167, 160, 227, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 236, 164, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 262, 190, 26, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 236, 164, 0, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 220, 225, 222, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 236, 164, 0, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 141, 134, 201, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 133, 138, 135, 204)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 204)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 114, 90, 193, 204)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 165, 141, 244, 204)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 140, 116, 219, 255)
    elif("cherry"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 219, 237, 227, 71)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 181, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 181, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 51, 56, 69, 148)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 51, 56, 69, 120)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 23, 38, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 181, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 181, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 116, 50, 77, 194)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 181, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 116, 50, 77, 194)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 23, 38, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 116, 50, 77, 194)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 23, 38, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 219, 237, 227, 161)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 219, 237, 227, 161)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 59, 51, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 51, 56, 69, 191)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 51, 56, 69, 120)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 33, 36, 43, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 23, 38, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 120, 196, 212, 10)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 204, 204, 204, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 23, 38, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 219, 237, 227, 161)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 116, 50, 77, 194)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 23, 38, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 219, 237, 227, 71)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 51, 56, 69, 148)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 59, 51, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 51, 56, 69, 191)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 59, 51, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 51, 56, 69, 191)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 59, 51, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 51, 56, 69, 191)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 181, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 181, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 137, 122, 65, 41)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 116, 50, 77, 255)
    elif("dark grey"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 222, 222, 222, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 128, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 112, 112, 112, 102)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 107, 107, 107, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 240, 240, 240, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 184, 184, 184, 199)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 117, 117, 117, 242)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 232, 232, 232, 64)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 179, 179, 179, 204)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 179, 179, 179, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 255, 255, 255, 0)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 36, 36, 36, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 219, 219, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 219, 219, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 222, 222, 222, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 112, 112, 112, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 107, 107, 107, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 222, 222, 222, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 112, 112, 112, 102)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 107, 107, 107, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 222, 222, 222, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 222, 222, 222, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 179, 179, 179, 79)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 179, 179, 179, 204)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 122, 128, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 240, 240, 240, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 179, 179, 179, 79)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 179, 179, 179, 79)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 179, 179, 179, 204)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 122, 128, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 112, 112, 112, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 184, 184, 184, 199)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 117, 117, 117, 242)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 184, 184, 184, 199)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 117, 117, 117, 242)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 10, 10, 10, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 74, 74, 74, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 0, 0, 0, 130)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 36, 36, 36, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 15, 15, 15, 240)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 232, 232, 232, 64)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 207, 207, 207, 171)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 117, 117, 117, 242)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 204, 204, 204, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 179, 179, 179, 204)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 122, 128, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 179, 179, 179, 204)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 184, 184, 184, 199)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 179, 179, 179, 79)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 128, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 107, 107, 107, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 255, 255, 255, 0)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 10, 10, 10, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 0, 0, 0, 130)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 74, 74, 74, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 74, 74, 74, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 10, 10, 10, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 0, 0, 0, 130)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 184, 184, 184, 199)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 10, 10, 10, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 0, 0, 0, 130)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 219, 219, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 102, 102, 102, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 46, 46, 46, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 219, 219, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 222, 222, 222, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 222, 222, 222, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 112, 112, 112, 102)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 107, 107, 107, 255)
    elif("dark 2"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 230, 230, 230, 212)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 0, 50, 200, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 3, 3, 5, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 179, 179, 179, 158)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 77, 77, 77, 214)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 179, 179, 179, 158)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 77, 77, 77, 214)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 97, 158, 212, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 230, 230, 230, 212)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 97, 158, 212, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 0, 0, 0, 212)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 0, 0, 0, 222)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 102, 102, 204, 51)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 3, 3, 5, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 23, 23, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 255, 255, 255, 217)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 255, 255, 255, 230)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 51, 51, 51, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 97, 158, 212, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_HeaderBg, 48, 48, 51, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBgAlt, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Item, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderStrong, 79, 79, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderLight, 59, 59, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderHovered, 0, 0, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 61, 133, 224, 30)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 61, 133, 224, 150)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 35, 35, 35, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 0, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 62, 62, 62, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 75, 75, 75, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 75, 75, 75, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 199, 199, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 255, 255, 50, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 77, 176, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 0, 0, 0, 200)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 102, 102, 102, 100)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 0, 0, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 0, 0, 0, 150)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 102, 102, 102, 50)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 179, 179, 179, 158)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 77, 77, 77, 214)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 179, 179, 179, 158)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 77, 77, 77, 214)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 255, 255, 255, 200)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 66, 150, 250, 255)
    elif("grey"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 230, 230, 230, 212)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 97, 158, 212, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 3, 3, 5, 204)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 179, 179, 179, 158)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 77, 77, 77, 214)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 179, 179, 179, 158)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 77, 77, 77, 214)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 97, 158, 212, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 230, 230, 230, 212)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 97, 158, 212, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 0, 0, 0, 212)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 0, 0, 0, 222)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 102, 102, 204, 51)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 3, 3, 5, 204)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 23, 23, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 97, 158, 212, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 255, 255, 255, 217)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 255, 255, 255, 153)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 255, 255, 255, 230)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 51, 51, 51, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 97, 158, 212, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 140, 135, 140, 130)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 143, 143, 143, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 143, 143, 143, 232)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 153, 153, 153, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 77, 176, 255, 135)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 13, 13, 26, 217)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 51, 64, 77, 153)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 0, 0, 0, 212)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 102, 102, 204, 51)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 0, 0, 0, 222)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 0, 0, 0, 222)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 0, 0, 0, 212)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 102, 102, 204, 51)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 112, 156, 219, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 97, 158, 212, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 0, 0, 0, 212)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 102, 102, 204, 51)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 179, 179, 179, 158)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 77, 77, 77, 214)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 230, 204, 204, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 230, 166, 166, 115)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 179, 179, 179, 158)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 77, 77, 77, 214)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 230, 230, 230, 230)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 0, 0, 255, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 0, 0, 3, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 179, 179, 179, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 122, 184, 227, 125)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 128, 176, 252, 173)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 204, 128, 128, 255)
    elif("gold"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 112, 112, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 15, 15, 15, 135)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 54, 54, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 207, 212, 207, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 237, 166, 36, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 54, 54, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 15, 15, 15, 135)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 54, 54, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 207, 212, 207, 255)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 237, 166, 36, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 15, 15, 15, 135)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 54, 54, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 207, 212, 207, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 156, 156, 156, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 255, 110, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 0, 0, 0, 130)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 15, 15, 15, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 15, 15, 15, 135)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 54, 54, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 207, 212, 207, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 54, 54, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 204, 204, 204, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 15, 15, 15, 135)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 54, 54, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 207, 212, 207, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 237, 166, 36, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 15, 15, 15, 135)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 54, 54, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 207, 212, 207, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 112, 112, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 54, 54, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 15, 15, 15, 135)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 0, 0, 0, 130)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 0, 0, 0, 130)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 0, 0, 0, 130)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 28, 28, 28, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 199, 140, 54, 255)
    elif("red"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 186, 191, 189, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 120, 56, 56, 166)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 214, 168, 168, 168)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 214, 168, 168, 168)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 214, 168, 168, 166)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 214, 168, 168, 166)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 87, 41, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 181, 99, 99, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 214, 168, 168, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 181, 99, 99, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 214, 168, 168, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 120, 56, 56, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 120, 56, 56, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 214, 168, 168, 166)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 214, 168, 168, 0)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 214, 168, 168, 166)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 214, 168, 168, 0)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 120, 56, 56, 166)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_InlayText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxis, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_XAxisGrid, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis2, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid2, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxis3, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_YAxisGrid3, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Selection, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Query, 0, 255, 112, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Crosshairs, 255, 255, 255, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 156, 156, 156, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 255, 110, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 120, 56, 56, 171)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 120, 56, 56, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 120, 56, 56, 171)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 87, 41, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 23, 23, 23, 240)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 214, 168, 168, 168)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 214, 168, 168, 168)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 204, 204, 204, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 214, 168, 168, 166)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 214, 168, 168, 0)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 214, 168, 168, 166)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 79, 79, 79, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 186, 191, 189, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 20, 20, 20, 240)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelector, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_BoxSelectorOutline, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridBackground, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_GridLine, 5, 5, 5, 135)
        internal_dpg.set_theme_color(mvThemeCol_Node_Background, 120, 56, 56, 171)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundHovered, 120, 56, 56, 171)
        internal_dpg.set_theme_color(mvThemeCol_Node_BackgroundSelected, 120, 56, 56, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_Outline, 143, 129, 49, 0)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBar, 143, 129, 49, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarHovered, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Node_TitleBarSelected, 255, 0, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_Pin, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_NodeAttr_PinHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 120, 56, 56, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 120, 56, 56, 171)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 120, 56, 56, 171)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 120, 56, 56, 171)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 120, 56, 56, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 181, 99, 99, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 214, 168, 168, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 214, 168, 168, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 214, 168, 168, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 181, 99, 99, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 214, 168, 168, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 66, 150, 250, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Annotation, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Line_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Scatter_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stem_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerOutline, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Stair_MarkerFill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Bar_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Error, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_VLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_HLine, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Shade_Fill, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Label, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_Area_Line, 0, 0, 0, -255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 120, 56, 56, 166)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 51, 51, 51, 128)



