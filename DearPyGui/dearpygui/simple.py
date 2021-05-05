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
    if("classic"):
        pass
    elif("light"):
        pass
    elif("cherry"):
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 219, 237, 227, 71)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 46, 89, 148, 220)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 51, 105, 173, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 51, 56, 69, 148)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 51, 56, 69, 120)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 23, 38, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 125, 80, 98, 172)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 125, 80, 98, 172)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 120, 196, 212, 36)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 116, 50, 77, 194)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 128, 19, 65, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 116, 50, 77, 194)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 219, 237, 227, 54)
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
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 219, 237, 227, 54)
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
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 33, 35, 40, 199)
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
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 26, 102, 191, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 120, 196, 212, 10)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 204, 204, 204, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 23, 38, 41, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 219, 237, 227, 54)
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
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_HeaderBg, 48, 48, 51, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBgAlt, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Item, 116, 50, 77, 194)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderStrong, 79, 79, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderLight, 59, 59, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderHovered, 26, 102, 191, 199)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderActive, 26, 102, 191, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 33, 35, 40, 199)
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
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 219, 237, 227, 71)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 116, 50, 77, 219)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 51, 56, 69, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 51, 56, 69, 230)
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
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 125, 80, 98, 172)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 116, 50, 77, 199)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 116, 50, 77, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 125, 80, 98, 172)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 33, 35, 40, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 219, 237, 227, 199)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 116, 50, 77, 110)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 51, 56, 69, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 219, 237, 227, 54)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 33, 35, 40, 199)
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
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 46, 89, 148, 220)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 66, 150, 250, 204)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 51, 105, 173, 255)
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
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 156, 156, 156, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 255, 110, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 186, 153, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 255, 153, 0, 255)
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
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 186, 153, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 51, 54, 56, 138)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 110, 110, 128, 128)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_HeaderBg, 48, 48, 51, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBgAlt, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Item, 179, 179, 179, 79)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemHovered, 117, 120, 122, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemActive, 107, 107, 107, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderStrong, 79, 79, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderLight, 59, 59, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderHovered, 184, 184, 184, 199)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderActive, 130, 130, 130, 255)
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
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 184, 184, 184, 199)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 130, 130, 130, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 255, 255, 255, 15)
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
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 255, 255, 255, 200)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 66, 150, 250, 255)
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
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 0, 0, 255, 255)
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
        internal_dpg.set_theme_color(mvThemeCol_Table_HeaderBg, 48, 48, 51, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderStrong, 79, 79, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderLight, 59, 59, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 0, 0, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 204, 128, 128, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 255, 255, 255, 15)
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
        internal_dpg.set_theme_color(mvThemeCol_InputText_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_TextHighlight, 186, 186, 186, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Hint, 102, 102, 102, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputText_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_InputText_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_Button_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Bg, 138, 138, 138, 89)
        internal_dpg.set_theme_color(mvThemeCol_Button_Hovered, 133, 133, 133, 150)
        internal_dpg.set_theme_color(mvThemeCol_Button_Active, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_Button_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_Button_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Mark, 166, 166, 166, 255)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_RadioButton_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_TabBar, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Bg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgHovered, 102, 102, 102, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_BgActive, 84, 84, 84, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tab_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_Tab_PopupBg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Bg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_Menu_BgActive, 133, 133, 133, 150)
        internal_dpg.set_theme_color(mvThemeCol_Menu_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_MenuItem_BgHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Bg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_MenuBar, 85, 85, 85, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_Child_Scrollbar, 61, 61, 61, 135)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrab, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabHovered, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_Child_ScrollbarGrabActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Grab, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_GrabActive, 163, 163, 163, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloat_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Grab, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_GrabActive, 163, 163, 163, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_SliderInt_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_DragFloat_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_DragInt_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_TextHighlight, 186, 186, 186, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBg, 138, 138, 138, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgHovered, 133, 133, 133, 150)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_ButtonBgActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_InputFloat_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_TextHighlight, 186, 186, 186, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBg, 138, 138, 138, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgHovered, 133, 133, 133, 150)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_ButtonBgActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_InputInt_BorderShadow, 186, 186, 186, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_TextHighlight, 186, 186, 186, 89)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Hovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Active, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_PopupBg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_ColorEdit_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Hovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Active, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_DragDrop, 255, 255, 0, 230)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_ColorPicker_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Bg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Tooltip_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Bg, 97, 97, 97, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BgActive, 194, 194, 194, 196)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_CollapsingHeader_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_Separator, 0, 0, 0, 35)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Hovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Active, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Marker, 166, 166, 166, 255)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_CheckBox_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Selected, 97, 97, 97, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Hovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Active, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_Scrollbar, 61, 61, 61, 135)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrab, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabHovered, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_Listbox_ScrollbarGrabActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_Text_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Hovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupBg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupSelected, 97, 97, 97, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_PopupActive, 194, 194, 194, 196)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonBg, 138, 138, 138, 89)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ButtonHovered, 133, 133, 133, 150)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Scrollbar, 61, 61, 61, 135)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrab, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabHovered, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_ScrollbarGrabActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_Combo_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_Combo_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_Plot_FrameBg, 255, 255, 255, 18)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBg, 0, 0, 0, 128)
        internal_dpg.set_theme_color(mvThemeCol_Plot_PlotBorder, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBg, 20, 20, 20, 255)
        internal_dpg.set_theme_color(mvThemeCol_Plot_LegendBorder, 31, 31, 31, 181)
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
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_PopupBg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Lines, 156, 156, 156, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_LinesHovered, 255, 110, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_Histogram, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_SimplePlot_HistogramHovered, 255, 153, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBg, 48, 48, 48, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgActive, 56, 56, 56, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_TitleBgCollapsed, 43, 43, 43, 230)
        internal_dpg.set_theme_color(mvThemeCol_Window_MenuBar, 85, 85, 85, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Bg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_Scrollbar, 61, 61, 61, 135)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrab, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabHovered, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ScrollbarGrabActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeBorder, 179, 171, 153, 172)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGrip, 66, 150, 250, 64)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripHovered, 66, 150, 250, 171)
        internal_dpg.set_theme_color(mvThemeCol_Window_ResizeGripActive, 66, 150, 250, 242)
        internal_dpg.set_theme_color(mvThemeCol_Window_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_Window_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleText, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_TitleBg, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Bg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ModalDimBg, 204, 204, 204, 89)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Scrollbar, 61, 61, 61, 135)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrab, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabHovered, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_ScrollbarGrabActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_Popup_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_Selectable_BgActive, 194, 194, 194, 196)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgHovered, 120, 120, 120, 255)
        internal_dpg.set_theme_color(mvThemeCol_TreeNode_BgActive, 133, 133, 133, 150)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bar, 230, 179, 0, 255)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_ProgressBar_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_HeaderBg, 48, 48, 51, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBgAlt, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Item, 97, 97, 97, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemHovered, 133, 133, 133, 150)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderStrong, 79, 79, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderLight, 59, 59, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderHovered, 179, 171, 153, 74)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderActive, 179, 171, 153, 172)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_ImageButton_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Time, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_TimeHovered, 133, 133, 133, 150)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropSelected, 97, 97, 97, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_DropBg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Scrollbar, 61, 61, 61, 135)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrab, 105, 105, 105, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabHovered, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_ScrollbarGrabActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_TimePicker_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Date, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateBg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateDisabled, 102, 102, 102, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateHovered, 133, 133, 133, 150)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_DateActive, 194, 194, 194, 255)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_DatePicker_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Separator, 0, 0, 0, 35)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_PopupBg, 64, 64, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_ColorButton_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_TabButton_PopupBg, 64, 64, 64, 255)
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
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 179, 171, 153, 74)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 179, 171, 153, 172)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_DragFloatx_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_DragIntx_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Grab, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_GrabActive, 163, 163, 163, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_SliderFloatx_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgHovered, 107, 107, 107, 102)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BgActive, 143, 143, 143, 171)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Grab, 133, 133, 133, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_GrabActive, 163, 163, 163, 255)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_SliderIntx_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_TextHighlight, 186, 186, 186, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_InputIntx_BorderShadow, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_TextHighlight, 186, 186, 186, 89)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Bg, 107, 107, 107, 138)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_Border, 31, 31, 31, 181)
        internal_dpg.set_theme_color(mvThemeCol_InputFloatx_BorderShadow, 255, 255, 255, 15)
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
    elif("purple"):
        pass
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
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Text, 235, 235, 235, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_HeaderBg, 48, 48, 51, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBgAlt, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Item, 130, 92, 38, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderStrong, 79, 79, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderLight, 59, 59, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderActive, 199, 140, 54, 255)
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
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 232, 163, 33, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 199, 140, 54, 255)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 255, 255, 255, 15)
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
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 255, 255, 255, 200)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 66, 150, 250, 255)
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
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Text, 255, 255, 255, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_HeaderBg, 48, 48, 51, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_RowBgAlt, 255, 255, 255, 15)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_Item, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemHovered, 181, 99, 99, 166)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_ItemActive, 51, 51, 51, 128)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderStrong, 79, 79, 89, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderLight, 59, 59, 64, 255)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderHovered, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_DataGrid_BorderActive, 181, 99, 99, 138)
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
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderHovered, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Table_BorderActive, 181, 99, 99, 138)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBg, 0, 0, 0, 0)
        internal_dpg.set_theme_color(mvThemeCol_Table_RowBgAlt, 255, 255, 255, 15)
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
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_Link, 255, 255, 255, 200)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkHovered, 66, 150, 250, 255)
        internal_dpg.set_theme_color(mvThemeCol_NodeEditor_LinkSelected, 66, 150, 250, 255)

