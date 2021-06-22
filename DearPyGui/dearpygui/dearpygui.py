from typing import List, Any, Callable, Union
from contextlib import contextmanager
import dearpygui.core as internal_dpg

########################################################################################################################
# User API
#
#    * Sections
#      - Helper Commands
#      - Container Context Managers
#      - Core Wrappings
#      - Constants
#
########################################################################################################################

########################################################################################################################
# Helper Commands
########################################################################################################################

def configure_item(item : int, **kwargs) -> None:
	"""Undocumented"""
	internal_dpg.configure_item(item, **kwargs)

def start_dearpygui():
    vp = internal_dpg.create_viewport()
    internal_dpg.setup_dearpygui(viewport=vp)
    internal_dpg.show_viewport(vp)
    while(internal_dpg.is_dearpygui_running()):
        internal_dpg.render_dearpygui_frame()   
    internal_dpg.cleanup_dearpygui()

@contextmanager
def mutex():
   
   try:
        yield internal_dpg.lock_mutex()
   finally:
        internal_dpg.unlock_mutex()

@contextmanager
def popup(parent: int, mousebutton: int = internal_dpg.mvMouseButton_Right, modal: bool = False) -> int:
    
    try:
        _internal_popup_id = internal_dpg.generate_uuid()
        internal_dpg.add_clicked_handler(parent, mousebutton, callback=lambda: internal_dpg.configure_item(_internal_popup_id, show=True))
        if modal:
            internal_dpg.add_window(modal=True, show=False, id=_internal_popup_id, autosize=True)
        else:
            internal_dpg.add_window(popup=True, show=False, id=_internal_popup_id, autosize=True)
        internal_dpg.push_container_stack(internal_dpg.last_container())
        yield _internal_popup_id

    finally:
        internal_dpg.pop_container_stack()

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


def set_item_user_data(item: str, user_data: Any):
    """Sets the item's callack_data to any python object.

    Args:
        item: Item the callback will be applied to.
        user_data: Callback_data to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, user_data=user_data)


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

def get_item_user_data(item: str) -> Union[Any, None]:
    """Gets the item's callback data.

    Returns:
        callback data as a python object or None
    """
    return internal_dpg.get_item_configuration(item)["user_data"]

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


def show_style_editor(sender: str="", data: Any=None) -> None:
    """Shows the standard style editor window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Style)

def show_metrics(sender: str="", data: Any=None) -> None:
    """Shows the standard metrics window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Metrics)

def show_about(sender: str="", data: Any=None) -> None:
    """Shows the standard about window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_About)

def show_debug(sender: str="", data: Any=None) -> None:
    """Shows the standard debug window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Debug)

def show_documentation(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Doc)

def show_font_manager(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Font)

def show_item_registry(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_ItemRegistry)

##########################################################
# Container Context Managers
##########################################################

@contextmanager
def child(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, border: bool =True, autosize_x: bool =False, autosize_y: bool =False, no_scrollbar: bool =False, horizontal_scrollbar: bool =False, menubar: bool =False) -> int:
	"""Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end."""
	try:
		widget = internal_dpg.add_child(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, border=border, autosize_x=autosize_x, autosize_y=autosize_y, no_scrollbar=no_scrollbar, horizontal_scrollbar=horizontal_scrollbar, menubar=menubar)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def clipper(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, delay_search: str =False) -> int:
	"""Helper to manually clip large list of items."""
	try:
		widget = internal_dpg.add_clipper(label=label, id=id, width=width, indent=indent, parent=parent, before=before, show=show, delay_search=delay_search)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def collapsing_header(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, closable: bool =False, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False) -> int:
	"""Adds a collapsing header to add items to. Must be closed with the end command."""
	try:
		widget = internal_dpg.add_collapsing_header(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, closable=closable, default_open=default_open, open_on_double_click=open_on_double_click, open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def drag_payload(*, label: str =None, id: int =0, parent: int =0, show: bool =True, drag_data: Any =None, payload_type: str ='$$DPG_PAYLOAD') -> int:
	"""User data payload for drag and drop operations."""
	try:
		widget = internal_dpg.add_drag_payload(label=label, id=id, parent=parent, show=show, drag_data=drag_data, payload_type=payload_type)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def draw_layer(*, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True) -> int:
	"""Creates a layer that can be drawn to."""
	try:
		widget = internal_dpg.add_draw_layer(label=label, id=id, parent=parent, before=before, show=show)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def drawlist(*, label: str =None, id: int =0, width: int =0, height: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5) -> int:
	"""A container that can hold drawings commands."""
	try:
		widget = internal_dpg.add_drawlist(label=label, id=id, width=width, height=height, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def file_dialog(*, label: str =None, id: int =0, width: int =0, height: int =0, callback: Callable =None, user_data: Any =None, show: bool =True, default_path: str ='', default_filename: str ='.', file_count: int =0, modal: bool =False, directory_selector: bool =False) -> int:
	"""Undocumented Function"""
	try:
		widget = internal_dpg.add_file_dialog(label=label, id=id, width=width, height=height, callback=callback, user_data=user_data, show=show, default_path=default_path, default_filename=default_filename, file_count=file_count, modal=modal, directory_selector=directory_selector)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def filter_set(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, delay_search: str =False) -> int:
	"""Helper to parse and apply text filters (e.g. aaaaa[, bbbbb][, ccccc])"""
	try:
		widget = internal_dpg.add_filter_set(label=label, id=id, width=width, indent=indent, parent=parent, before=before, show=show, delay_search=delay_search)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def font(file : str, size : int, *, label: str =None, id: int =0, parent: int =0, default_font: bool =False) -> int:
	"""Undocumented function"""
	try:
		widget = internal_dpg.add_font(file, size, label=label, id=id, parent=parent, default_font=default_font)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def font_registry(*, label: str =None, id: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	try:
		widget = internal_dpg.add_font_registry(label=label, id=id, show=show)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def group(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, horizontal: bool =False, horizontal_spacing: float =-1) -> int:
	"""Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members. Must be closed with the end command."""
	try:
		widget = internal_dpg.add_group(label=label, id=id, width=width, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, horizontal=horizontal, horizontal_spacing=horizontal_spacing)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def handler_registry(*, label: str =None, id: int =0, show: bool =True) -> int:
	"""Adds a handler registry."""
	try:
		widget = internal_dpg.add_handler_registry(label=label, id=id, show=show)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def menu(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5) -> int:
	"""Adds a menu to an existing menu bar. Must be followed by a call to end."""
	try:
		widget = internal_dpg.add_menu(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, enabled=enabled, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def menu_bar(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, show: bool =True, delay_search: str =False) -> int:
	"""Adds a menu bar to a window. Must be followed by a call to end."""
	try:
		widget = internal_dpg.add_menu_bar(label=label, id=id, indent=indent, parent=parent, show=show, delay_search=delay_search)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def node(*, label: str =None, id: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, draggable: bool =True) -> int:
	"""Adds a node to a node editor."""
	try:
		widget = internal_dpg.add_node(label=label, id=id, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, draggable=draggable)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def node_attribute(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, attribute_type: int =0, shape: int =0) -> int:
	"""Adds a node attribute."""
	try:
		widget = internal_dpg.add_node_attribute(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, tracked=tracked, track_offset=track_offset, attribute_type=attribute_type, shape=shape)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def node_editor(*, label: str =None, id: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, delink_callback: Callable =None) -> int:
	"""Adds a node editor."""
	try:
		widget = internal_dpg.add_node_editor(label=label, id=id, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, delink_callback=delink_callback)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def plot(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, no_title: bool =False, no_menus: bool =False, no_box_select: bool =False, no_mouse_pos: bool =False, no_highlight: bool =False, no_child: bool =False, query: bool =False, crosshairs: bool =False, anti_aliased: bool =False, equal_aspects: bool =False) -> int:
	"""Adds a plot which is used to hold series, and can be drawn to with draw commands."""
	try:
		widget = internal_dpg.add_plot(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, no_title=no_title, no_menus=no_menus, no_box_select=no_box_select, no_mouse_pos=no_mouse_pos, no_highlight=no_highlight, no_child=no_child, query=query, crosshairs=crosshairs, anti_aliased=anti_aliased, equal_aspects=equal_aspects)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def staging_container(*, label: str =None, id: int =0) -> int:
	"""Undocumented function"""
	try:
		widget = internal_dpg.add_staging_container(label=label, id=id)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def tab(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, closable: bool =False, no_tooltip: bool =False, order_mode: bool =0) -> int:
	"""Adds a tab to a tab bar. Must be closed with thes end command."""
	try:
		widget = internal_dpg.add_tab(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, closable=closable, no_tooltip=no_tooltip, order_mode=order_mode)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def tab_bar(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, reorderable: bool =False) -> int:
	"""Adds a tab bar."""
	try:
		widget = internal_dpg.add_tab_bar(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, reorderable=reorderable)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def table(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, header_row: bool =True, inner_width: int =0, policy: int =0, freeze_rows: int =0, freeze_columns: int =0, sort_multi: bool =False, sort_tristate: bool =False, resizable: bool =False, reorderable: bool =False, hideable: bool =False, sortable: bool =False, context_menu_in_body: bool =False, row_background: bool =False, borders_innerH: bool =False, borders_outerH: bool =False, borders_innerV: bool =False, borders_outerV: bool =False, no_host_extendX: bool =False, no_host_extendY: bool =False, no_keep_columns_visible: bool =False, precise_widths: bool =False, no_clip: bool =False, pad_outerX: bool =False, no_pad_outerX: bool =False, no_pad_innerX: bool =False, scrollX: bool =False, scrollY: bool =False) -> int:
	"""Undocumented function"""
	try:
		widget = internal_dpg.add_table(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, callback=callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, header_row=header_row, inner_width=inner_width, policy=policy, freeze_rows=freeze_rows, freeze_columns=freeze_columns, sort_multi=sort_multi, sort_tristate=sort_tristate, resizable=resizable, reorderable=reorderable, hideable=hideable, sortable=sortable, context_menu_in_body=context_menu_in_body, row_background=row_background, borders_innerH=borders_innerH, borders_outerH=borders_outerH, borders_innerV=borders_innerV, borders_outerV=borders_outerV, no_host_extendX=no_host_extendX, no_host_extendY=no_host_extendY, no_keep_columns_visible=no_keep_columns_visible, precise_widths=precise_widths, no_clip=no_clip, pad_outerX=pad_outerX, no_pad_outerX=no_pad_outerX, no_pad_innerX=no_pad_innerX, scrollX=scrollX, scrollY=scrollY)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def table_row(*, label: str =None, id: int =0, height: int =0, parent: int =0, before: int =0, show: bool =True, filter_key: str ='') -> int:
	"""Undocumented function"""
	try:
		widget = internal_dpg.add_table_row(label=label, id=id, height=height, parent=parent, before=before, show=show, filter_key=filter_key)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def texture_registry(*, label: str =None, id: int =0, show: bool =False) -> int:
	"""Undocumented function"""
	try:
		widget = internal_dpg.add_texture_registry(label=label, id=id, show=show)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def theme(*, label: str =None, id: int =0, default_theme: bool =False) -> int:
	"""Undocumented function"""
	try:
		widget = internal_dpg.add_theme(label=label, id=id, default_theme=default_theme)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def tooltip(parent : str, *, label: str =None, id: int =0, show: bool =True) -> int:
	"""Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for."""
	try:
		widget = internal_dpg.add_tooltip(parent, label=label, id=id, show=show)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def tree_node(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False, selectable: bool =False) -> int:
	"""Adds a tree node to add items to. Must be closed with the end command."""
	try:
		widget = internal_dpg.add_tree_node(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, default_open=default_open, open_on_double_click=open_on_double_click, open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet, selectable=selectable)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def viewport_drawlist(*, label: str =None, id: int =0, show: bool =True, filter_key: str ='', delay_search: str =False, front: bool =True) -> int:
	"""Draws a quad on a drawing."""
	try:
		widget = internal_dpg.add_viewport_drawlist(label=label, id=id, show=show, filter_key=filter_key, delay_search=delay_search, front=front)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()
@contextmanager
def window(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, user_data: Any =None, show: bool =True, delay_search: str =False, min_size: List[int] =[32, 32], max_size: List[int] =[30000, 30000], menubar: bool =False, collapsed: bool =False, autosize: bool =False, no_resize: bool =False, no_title_bar: bool =False, no_move: bool =False, no_scrollbar: bool =False, no_collapse: bool =False, horizontal_scrollbar: bool =False, no_focus_on_appearing: bool =False, no_bring_to_front_on_focus: bool =False, no_close: bool =False, no_background: bool =False, modal: bool =False, popup: bool =False, on_close: Callable =None) -> int:
	"""Creates a new window for following items to be added to."""
	try:
		widget = internal_dpg.add_window(label=label, id=id, width=width, height=height, indent=indent, user_data=user_data, show=show, delay_search=delay_search, min_size=min_size, max_size=max_size, menubar=menubar, collapsed=collapsed, autosize=autosize, no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move, no_scrollbar=no_scrollbar, no_collapse=no_collapse, horizontal_scrollbar=horizontal_scrollbar, no_focus_on_appearing=no_focus_on_appearing, no_bring_to_front_on_focus=no_bring_to_front_on_focus, no_close=no_close, no_background=no_background, modal=modal, popup=popup, on_close=on_close)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()

##########################################################
# Core Wrappings
##########################################################

def add_2d_histogram_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, xbins: int =-1, ybins: int =-1, xmin_range: float =0.0, xmax_range: float =1.0, ymin_range: float =0.0, ymax_range: float =1.0, density: bool =False, outliers: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	return internal_dpg.add_2d_histogram_series(x, y, label=label, id=id, parent=parent, before=before, source=source, show=show, xbins=xbins, ybins=ybins, xmin_range=xmin_range, xmax_range=xmax_range, ymin_range=ymin_range, ymax_range=ymax_range, density=density, outliers=outliers, contribute_to_bounds=contribute_to_bounds)

def add_3d_slider(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), max_x: float =100.0, max_y: float =100.0, max_z: float =100.0, min_x: float =0.0, min_y: float =0.0, min_z: float =0.0, scale: float =1.0) -> int:
	"""Undocumented"""
	return internal_dpg.add_3d_slider(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, max_x=max_x, max_y=max_y, max_z=max_z, min_x=min_x, min_y=min_y, min_z=min_z, scale=scale)

def add_activated_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is activated."""
	return internal_dpg.add_activated_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_active_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is active."""
	return internal_dpg.add_active_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_area_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, fill: List[int] =(0, 0, 0, -255), contribute_to_bounds: bool =True) -> int:
	"""Adds an area series to a plot."""
	return internal_dpg.add_area_series(x, y, label=label, id=id, parent=parent, before=before, source=source, show=show, fill=fill, contribute_to_bounds=contribute_to_bounds)

def add_bar_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, weight: float =1.0, horizontal: bool =False, contribute_to_bounds: bool =True) -> int:
	"""Adds a bar series to a plot."""
	return internal_dpg.add_bar_series(x, y, label=label, id=id, parent=parent, before=before, source=source, show=show, weight=weight, horizontal=horizontal, contribute_to_bounds=contribute_to_bounds)

def add_button(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, small: bool =False, arrow: bool =False, direction: int =0) -> int:
	"""Adds a button."""
	return internal_dpg.add_button(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, small=small, arrow=arrow, direction=direction)

def add_candle_series(dates : List[float], opens : List[float], closes : List[float], lows : List[float], highs : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, bull_color: List[int] =(0, 255, 113, 255), bear_color: List[int] =(218, 13, 79, 255), weight: int =0.25, contribute_to_bounds: bool =True, tooltip: bool =True) -> int:
	"""Adds a candle series to a plot."""
	return internal_dpg.add_candle_series(dates, opens, closes, lows, highs, label=label, id=id, parent=parent, before=before, source=source, show=show, bull_color=bull_color, bear_color=bear_color, weight=weight, contribute_to_bounds=contribute_to_bounds, tooltip=tooltip)

def add_char_remap(source : int, target : int, *, label: str =None, id: int =0, parent: int =0) -> int:
	"""Undocumented function"""
	return internal_dpg.add_char_remap(source, target, label=label, id=id, parent=parent)

def add_checkbox(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: bool =False) -> int:
	"""Adds a checkbox."""
	return internal_dpg.add_checkbox(label=label, id=id, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value)

def add_child(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, border: bool =True, autosize_x: bool =False, autosize_y: bool =False, no_scrollbar: bool =False, horizontal_scrollbar: bool =False, menubar: bool =False) -> int:
	"""Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end."""
	return internal_dpg.add_child(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, border=border, autosize_x=autosize_x, autosize_y=autosize_y, no_scrollbar=no_scrollbar, horizontal_scrollbar=horizontal_scrollbar, menubar=menubar)

def add_clicked_handler(parent : int, button : int =-1, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is clicked."""
	return internal_dpg.add_clicked_handler(parent, button=button, label=label, id=id, callback=callback, user_data=user_data)

def add_clipper(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, delay_search: str =False) -> int:
	"""Helper to manually clip large list of items."""
	return internal_dpg.add_clipper(label=label, id=id, width=width, indent=indent, parent=parent, before=before, show=show, delay_search=delay_search)

def add_collapsing_header(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, closable: bool =False, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False) -> int:
	"""Adds a collapsing header to add items to. Must be closed with the end command."""
	return internal_dpg.add_collapsing_header(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, closable=closable, default_open=default_open, open_on_double_click=open_on_double_click, open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet)

def add_color_button(default_value : List[int] =(0, 0, 0, 255), *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_border: bool =False, no_drag_drop: bool =False) -> int:
	"""Adds a color button."""
	return internal_dpg.add_color_button(default_value=default_value, label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, no_alpha=no_alpha, no_border=no_border, no_drag_drop=no_drag_drop)

def add_color_edit(default_value : List[int] =(0, 0, 0, 255), *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_picker: bool =False, no_options: bool =False, no_small_preview: bool =False, no_inputs: bool =False, no_tooltip: bool =False, no_label: bool =False, no_drag_drop: bool =False, alpha_bar: bool =False, alpha_preview: int =0, display_mode: int =1048576, display_type: int =8388608, input_mode: int =134217728) -> int:
	"""Adds an RGBA color editor. Click and draging the color square will copy the color to be applied on any other color widget."""
	return internal_dpg.add_color_edit(default_value=default_value, label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, no_alpha=no_alpha, no_picker=no_picker, no_options=no_options, no_small_preview=no_small_preview, no_inputs=no_inputs, no_tooltip=no_tooltip, no_label=no_label, no_drag_drop=no_drag_drop, alpha_bar=alpha_bar, alpha_preview=alpha_preview, display_mode=display_mode, display_type=display_type, input_mode=input_mode)

def add_color_picker(default_value : List[int] =(0, 0, 0, 255), *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_side_preview: bool =False, no_small_preview: bool =False, no_inputs: bool =False, no_tooltip: bool =False, no_label: bool =False, alpha_bar: bool =False, display_rgb: bool =False, display_hsv: bool =False, display_hex: bool =False, picker_mode: int =33554432, alpha_preview: int =0, display_type: int =8388608, input_mode: int =134217728) -> int:
	"""Adds an RGB color picker. Click and drag the color square to copy the color and drop on any other color widget to apply. Right Click allows the style of the color picker to be changed."""
	return internal_dpg.add_color_picker(default_value=default_value, label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, no_alpha=no_alpha, no_side_preview=no_side_preview, no_small_preview=no_small_preview, no_inputs=no_inputs, no_tooltip=no_tooltip, no_label=no_label, alpha_bar=alpha_bar, display_rgb=display_rgb, display_hsv=display_hsv, display_hex=display_hex, picker_mode=picker_mode, alpha_preview=alpha_preview, display_type=display_type, input_mode=input_mode)

def add_colormap_scale(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, show: bool =True, pos: List[int] =[], default_value: int =0, min_scale: float =0.0, max_scale: float =1.0) -> int:
	"""Undocumented"""
	return internal_dpg.add_colormap_scale(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, show=show, pos=pos, default_value=default_value, min_scale=min_scale, max_scale=max_scale)

def add_combo(items : List[str] =(), *, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', popup_align_left: bool =False, no_arrow_button: bool =False, no_preview: bool =False, height_mode: int =1) -> int:
	"""Adds a combo dropdown."""
	return internal_dpg.add_combo(items=items, label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, popup_align_left=popup_align_left, no_arrow_button=no_arrow_button, no_preview=no_preview, height_mode=height_mode)

def add_date_picker(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: dict ={'month_day': 14, 'year':20, 'month':5}, level: int =0) -> int:
	"""Undocumented"""
	return internal_dpg.add_date_picker(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, level=level)

def add_deactivated_after_edit_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is deactivated after edit."""
	return internal_dpg.add_deactivated_after_edit_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_deactivated_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is deactivated."""
	return internal_dpg.add_deactivated_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_drag_float(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, format: str ='%0.3f', speed: float =1.0, min_value: float =0.0, max_value: float =100.0, no_input: bool =False, clamped: bool =False) -> int:
	"""Adds drag for a single float value. CTRL+Click to directly modify the value."""
	return internal_dpg.add_drag_float(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, format=format, speed=speed, min_value=min_value, max_value=max_value, no_input=no_input, clamped=clamped)

def add_drag_floatx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), size: int =4, format: str ='%0.3f', speed: float =1.0, min_value: float =0.0, max_value: float =100.0, no_input: bool =False, clamped: bool =False) -> int:
	"""Adds drag input for a set of int values up to 4. CTRL+Click to directly modify the value."""
	return internal_dpg.add_drag_floatx(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, size=size, format=format, speed=speed, min_value=min_value, max_value=max_value, no_input=no_input, clamped=clamped)

def add_drag_int(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: int =0, format: str ='%d', speed: float =1.0, min_value: int =0, max_value: int =100, no_input: bool =False, clamped: bool =False) -> int:
	"""Adds drag for a single int value. CTRL+Click to directly modify the value."""
	return internal_dpg.add_drag_int(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, format=format, speed=speed, min_value=min_value, max_value=max_value, no_input=no_input, clamped=clamped)

def add_drag_intx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[int] =(0, 0, 0, 0), size: int =4, format: str ='%d', speed: float =1.0, min_value: int =0, max_value: int =100, no_input: bool =False, clamped: bool =False) -> int:
	"""Adds drag input for a set of int values up to 4. CTRL+Click to directly modify the value."""
	return internal_dpg.add_drag_intx(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, size=size, format=format, speed=speed, min_value=min_value, max_value=max_value, no_input=no_input, clamped=clamped)

def add_drag_line(*, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, callback: Callable =None, show: bool =True, default_value: Any =0.0, color: List[int] =(0, 0, 0, -255), thickness: float =1.0, show_label: bool =True, vertical: bool =True) -> int:
	"""Adds a drag line to a plot."""
	return internal_dpg.add_drag_line(label=label, id=id, parent=parent, before=before, source=source, callback=callback, show=show, default_value=default_value, color=color, thickness=thickness, show_label=show_label, vertical=vertical)

def add_drag_payload(*, label: str =None, id: int =0, parent: int =0, show: bool =True, drag_data: Any =None, payload_type: str ='$$DPG_PAYLOAD') -> int:
	"""User data payload for drag and drop operations."""
	return internal_dpg.add_drag_payload(label=label, id=id, parent=parent, show=show, drag_data=drag_data, payload_type=payload_type)

def add_drag_point(*, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, callback: Callable =None, show: bool =True, default_value: Any =(0.0, 0.0), color: List[int] =(0, 0, 0, -255), thickness: float =1.0, show_label: bool =True) -> int:
	"""Adds a drag point to a plot."""
	return internal_dpg.add_drag_point(label=label, id=id, parent=parent, before=before, source=source, callback=callback, show=show, default_value=default_value, color=color, thickness=thickness, show_label=show_label)

def add_draw_layer(*, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True) -> int:
	"""Creates a layer that can be drawn to."""
	return internal_dpg.add_draw_layer(label=label, id=id, parent=parent, before=before, show=show)

def add_drawlist(*, label: str =None, id: int =0, width: int =0, height: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5) -> int:
	"""A container that can hold drawings commands."""
	return internal_dpg.add_drawlist(label=label, id=id, width=width, height=height, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset)

def add_dummy(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[]) -> int:
	"""Adds a spacer or 'dummy' object."""
	return internal_dpg.add_dummy(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, show=show, pos=pos)

def add_dynamic_texture(width : int, height : int, default_value : List[float], *, label: str =None, id: int =0, parent: int =0) -> int:
	"""Undocumented function"""
	return internal_dpg.add_dynamic_texture(width, height, default_value, label=label, id=id, parent=parent)

def add_edited_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is edited."""
	return internal_dpg.add_edited_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_error_series(x : List[float], y : List[float], negative : List[float], positive : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, contribute_to_bounds: bool =True, horizontal: bool =False) -> int:
	"""Adds an error series to a plot."""
	return internal_dpg.add_error_series(x, y, negative, positive, label=label, id=id, parent=parent, before=before, source=source, show=show, contribute_to_bounds=contribute_to_bounds, horizontal=horizontal)

def add_file_dialog(*, label: str =None, id: int =0, width: int =0, height: int =0, callback: Callable =None, user_data: Any =None, show: bool =True, default_path: str ='', default_filename: str ='.', file_count: int =0, modal: bool =False, directory_selector: bool =False) -> int:
	"""Undocumented Function"""
	return internal_dpg.add_file_dialog(label=label, id=id, width=width, height=height, callback=callback, user_data=user_data, show=show, default_path=default_path, default_filename=default_filename, file_count=file_count, modal=modal, directory_selector=directory_selector)

def add_file_extension(extension : str, *, label: str =None, id: int =0, width: int =0, height: int =0, parent: int =0, before: int =0, custom_text: str ='', color: List[float] =(-255, 0, 0, 255)) -> int:
	"""Undocumented Function"""
	return internal_dpg.add_file_extension(extension, label=label, id=id, width=width, height=height, parent=parent, before=before, custom_text=custom_text, color=color)

def add_filter_set(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, delay_search: str =False) -> int:
	"""Helper to parse and apply text filters (e.g. aaaaa[, bbbbb][, ccccc])"""
	return internal_dpg.add_filter_set(label=label, id=id, width=width, indent=indent, parent=parent, before=before, show=show, delay_search=delay_search)

def add_focus_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is focused."""
	return internal_dpg.add_focus_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_font(file : str, size : int, *, label: str =None, id: int =0, parent: int =0, default_font: bool =False) -> int:
	"""Undocumented function"""
	return internal_dpg.add_font(file, size, label=label, id=id, parent=parent, default_font=default_font)

def add_font_chars(chars : List[int], *, label: str =None, id: int =0, parent: int =0) -> int:
	"""Undocumented function"""
	return internal_dpg.add_font_chars(chars, label=label, id=id, parent=parent)

def add_font_range(first_char : int, last_char : int, *, label: str =None, id: int =0, parent: int =0) -> int:
	"""Undocumented function"""
	return internal_dpg.add_font_range(first_char, last_char, label=label, id=id, parent=parent)

def add_font_range_hint(hint : int, *, label: str =None, id: int =0, parent: int =0) -> int:
	"""Undocumented function"""
	return internal_dpg.add_font_range_hint(hint, label=label, id=id, parent=parent)

def add_font_registry(*, label: str =None, id: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	return internal_dpg.add_font_registry(label=label, id=id, show=show)

def add_group(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, horizontal: bool =False, horizontal_spacing: float =-1) -> int:
	"""Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members. Must be closed with the end command."""
	return internal_dpg.add_group(label=label, id=id, width=width, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, horizontal=horizontal, horizontal_spacing=horizontal_spacing)

def add_handler_registry(*, label: str =None, id: int =0, show: bool =True) -> int:
	"""Adds a handler registry."""
	return internal_dpg.add_handler_registry(label=label, id=id, show=show)

def add_heat_series(x : List[float], rows : int, cols : int, *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, scale_min: float =0.0, scale_max: float =1.0, bounds_min: Any =(0.0, 0.0), bounds_max: Any =(1.0, 1.0), format: str ='%0.1f', contribute_to_bounds: bool =True) -> int:
	"""Adds a heat series to a plot."""
	return internal_dpg.add_heat_series(x, rows, cols, label=label, id=id, parent=parent, before=before, source=source, show=show, scale_min=scale_min, scale_max=scale_max, bounds_min=bounds_min, bounds_max=bounds_max, format=format, contribute_to_bounds=contribute_to_bounds)

def add_histogram_series(x : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, bins: int =-1, bar_scale: float =1.0, min_range: float =0.0, max_range: float =1.0, cumlative: bool =False, density: bool =False, outliers: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Adds a histogram series to a plot."""
	return internal_dpg.add_histogram_series(x, label=label, id=id, parent=parent, before=before, source=source, show=show, bins=bins, bar_scale=bar_scale, min_range=min_range, max_range=max_range, cumlative=cumlative, density=density, outliers=outliers, contribute_to_bounds=contribute_to_bounds)

def add_hline_series(x : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Adds a infinite horizontal line series to a plot."""
	return internal_dpg.add_hline_series(x, label=label, id=id, parent=parent, before=before, source=source, show=show, contribute_to_bounds=contribute_to_bounds)

def add_hover_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is hovered."""
	return internal_dpg.add_hover_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_image(texture_id : int, *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, tint_color: List[float] =(255, 255, 255, 255), border_color: List[float] =(0, 0, 0, 0), uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0)) -> int:
	"""Adds an image. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) for texture coordinates will generally display the entire texture."""
	return internal_dpg.add_image(texture_id, label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, tint_color=tint_color, border_color=border_color, uv_min=uv_min, uv_max=uv_max)

def add_image_button(texture_id : int, *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, frame_padding: int =-1, tint_color: List[float] =(255, 255, 255, 255), background_color: List[float] =(0, 0, 0, 0), uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0)) -> int:
	"""Adds an image button. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) texture coordinates will generally display the entire texture"""
	return internal_dpg.add_image_button(texture_id, label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, frame_padding=frame_padding, tint_color=tint_color, background_color=background_color, uv_min=uv_min, uv_max=uv_max)

def add_image_series(texture_id : int, bounds_min : List[float], bounds_max : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0), tint_color: List[int] =(255, 255, 255, 255), contribute_to_bounds: bool =True) -> int:
	"""Adds a image series to a plot."""
	return internal_dpg.add_image_series(texture_id, bounds_min, bounds_max, label=label, id=id, parent=parent, before=before, source=source, show=show, uv_min=uv_min, uv_max=uv_max, tint_color=tint_color, contribute_to_bounds=contribute_to_bounds)

def add_input_float(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, format: str ='%.3f', min_value: float =0.0, max_value: float =100.0, step: float =0.1, step_fast: float =1.0, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Adds input for int."""
	return internal_dpg.add_input_float(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, format=format, min_value=min_value, max_value=max_value, step=step, step_fast=step_fast, min_clamped=min_clamped, max_clamped=max_clamped, on_enter=on_enter, readonly=readonly)

def add_input_floatx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), format: str ='%.3f', min_value: float =0.0, max_value: float =100.0, size: int =4, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Adds multi float input for up to 4 float values."""
	return internal_dpg.add_input_floatx(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, format=format, min_value=min_value, max_value=max_value, size=size, min_clamped=min_clamped, max_clamped=max_clamped, on_enter=on_enter, readonly=readonly)

def add_input_int(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[int] =0, min_value: int =0, max_value: int =100, step: int =1, step_fast: int =100, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Adds input for float."""
	return internal_dpg.add_input_int(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, min_value=min_value, max_value=max_value, step=step, step_fast=step_fast, min_clamped=min_clamped, max_clamped=max_clamped, on_enter=on_enter, readonly=readonly)

def add_input_intx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[int] =(0, 0, 0, 0), min_value: int =0, max_value: int =100, size: int =4, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Adds multi int input for up to 4 integer values."""
	return internal_dpg.add_input_intx(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, min_value=min_value, max_value=max_value, size=size, min_clamped=min_clamped, max_clamped=max_clamped, on_enter=on_enter, readonly=readonly)

def add_input_text(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', hint: str ='', multiline: bool =False, no_spaces: bool =False, uppercase: bool =False, tab_input: bool =False, decimal: bool =False, hexadecimal: bool =False, readonly: bool =False, password: bool =False, scientific: bool =False, on_enter: bool =False) -> int:
	"""Adds input for text."""
	return internal_dpg.add_input_text(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, hint=hint, multiline=multiline, no_spaces=no_spaces, uppercase=uppercase, tab_input=tab_input, decimal=decimal, hexadecimal=hexadecimal, readonly=readonly, password=password, scientific=scientific, on_enter=on_enter)

def add_key_down_handler(key : int =-1, *, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified key is down. Parent must be a handler registry."""
	return internal_dpg.add_key_down_handler(key=key, label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_key_press_handler(key : int =-1, *, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified key is pressed. Parent must be a handler registry."""
	return internal_dpg.add_key_press_handler(key=key, label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_key_release_handler(key : int =-1, *, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified key is released. Parent must be a handler registry."""
	return internal_dpg.add_key_release_handler(key=key, label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_knob_float(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, min_value: float =0.0, max_value: float =100.0) -> int:
	"""Undocumented"""
	return internal_dpg.add_knob_float(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, min_value=min_value, max_value=max_value)

def add_line_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Adds a line series to a plot."""
	return internal_dpg.add_line_series(x, y, label=label, id=id, parent=parent, before=before, source=source, show=show, contribute_to_bounds=contribute_to_bounds)

def add_listbox(items : List[str] =(), *, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', num_items: int =3) -> int:
	"""Adds a listbox."""
	return internal_dpg.add_listbox(items=items, label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, num_items=num_items)

def add_loading_indicator(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[], style: int =0, circle_count: int =8, speed: float =1.0, radius: float =3.0, thickness: float =1.0, color: List[int] =(51, 51, 55, 255), secondary_color: List[int] =(29, 151, 236, 103)) -> int:
	"""Undocumented"""
	return internal_dpg.add_loading_indicator(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, show=show, pos=pos, style=style, circle_count=circle_count, speed=speed, radius=radius, thickness=thickness, color=color, secondary_color=secondary_color)

def add_menu(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5) -> int:
	"""Adds a menu to an existing menu bar. Must be followed by a call to end."""
	return internal_dpg.add_menu(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, enabled=enabled, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset)

def add_menu_bar(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, show: bool =True, delay_search: str =False) -> int:
	"""Adds a menu bar to a window. Must be followed by a call to end."""
	return internal_dpg.add_menu_bar(label=label, id=id, indent=indent, parent=parent, show=show, delay_search=delay_search)

def add_menu_item(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: bool =False, shortcut: str ='', check: bool =False) -> int:
	"""Adds a menu item to an existing menu."""
	return internal_dpg.add_menu_item(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, shortcut=shortcut, check=check)

def add_mouse_click_handler(button : int =-1, *, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is clicked. Parent must be a handler registry."""
	return internal_dpg.add_mouse_click_handler(button=button, label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_mouse_double_click_handler(button : int =-1, *, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is double clicked. Parent must be a handler registry."""
	return internal_dpg.add_mouse_double_click_handler(button=button, label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_mouse_down_handler(button : int =-1, *, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is down. Parent must be a handler registry."""
	return internal_dpg.add_mouse_down_handler(button=button, label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_mouse_drag_handler(button : int =-1, threshold : float =10.0, *, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is clicked and dragged a set threshold. Parent must be a handler registry."""
	return internal_dpg.add_mouse_drag_handler(button=button, threshold=threshold, label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_mouse_move_handler(*, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the mouse is moved. Parent must be a handler registry."""
	return internal_dpg.add_mouse_move_handler(label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_mouse_release_handler(button : int =-1, *, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is released. Parent must be a handler registry."""
	return internal_dpg.add_mouse_release_handler(button=button, label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_mouse_wheel_handler(*, label: str =None, id: int =0, parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Adds a handler which runs a given callback when the vertical mouse wheel is scrolled. Parent must be a handler registry."""
	return internal_dpg.add_mouse_wheel_handler(label=label, id=id, parent=parent, callback=callback, user_data=user_data, show=show)

def add_node(*, label: str =None, id: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, draggable: bool =True) -> int:
	"""Adds a node to a node editor."""
	return internal_dpg.add_node(label=label, id=id, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, draggable=draggable)

def add_node_attribute(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, attribute_type: int =0, shape: int =0) -> int:
	"""Adds a node attribute."""
	return internal_dpg.add_node_attribute(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, tracked=tracked, track_offset=track_offset, attribute_type=attribute_type, shape=shape)

def add_node_editor(*, label: str =None, id: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, delink_callback: Callable =None) -> int:
	"""Adds a node editor."""
	return internal_dpg.add_node_editor(label=label, id=id, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, delink_callback=delink_callback)

def add_node_link(node_1 : int, node_2 : int, *, label: str =None, id: int =0, parent: int =0, show: bool =True) -> int:
	"""Adds a node link between nodes."""
	return internal_dpg.add_node_link(node_1, node_2, label=label, id=id, parent=parent, show=show)

def add_pie_series(x : float, y : float, radius : float, values : List[float], labels : List[str], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, format: str ='%0.2f', angle: float =90.0, normalize: bool =False, contribute_to_bounds: bool =True) -> int:
	"""Adds a pie series to a plot."""
	return internal_dpg.add_pie_series(x, y, radius, values, labels, label=label, id=id, parent=parent, before=before, source=source, show=show, format=format, angle=angle, normalize=normalize, contribute_to_bounds=contribute_to_bounds)

def add_plot(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, no_title: bool =False, no_menus: bool =False, no_box_select: bool =False, no_mouse_pos: bool =False, no_highlight: bool =False, no_child: bool =False, query: bool =False, crosshairs: bool =False, anti_aliased: bool =False, equal_aspects: bool =False) -> int:
	"""Adds a plot which is used to hold series, and can be drawn to with draw commands."""
	return internal_dpg.add_plot(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, no_title=no_title, no_menus=no_menus, no_box_select=no_box_select, no_mouse_pos=no_mouse_pos, no_highlight=no_highlight, no_child=no_child, query=query, crosshairs=crosshairs, anti_aliased=anti_aliased, equal_aspects=equal_aspects)

def add_plot_annotation(*, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, default_value: Any =(0.0, 0.0), offset: List[float] =(0.0, 0.0), color: List[int] =(0, 0, 0, -255), clamped: bool =True) -> int:
	"""Adds an annotation to a plot."""
	return internal_dpg.add_plot_annotation(label=label, id=id, parent=parent, before=before, source=source, show=show, default_value=default_value, offset=offset, color=color, clamped=clamped)

def add_plot_axis(axis : int, *, label: str =None, id: int =0, parent: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, no_gridlines: bool =False, no_tick_marks: bool =False, no_tick_labels: bool =False, log_scale: bool =False, invert: bool =False, lock_min: bool =False, lock_max: bool =False, time: bool =False) -> int:
	"""Adds a plot legend to a plot."""
	return internal_dpg.add_plot_axis(axis, label=label, id=id, parent=parent, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, no_gridlines=no_gridlines, no_tick_marks=no_tick_marks, no_tick_labels=no_tick_labels, log_scale=log_scale, invert=invert, lock_min=lock_min, lock_max=lock_max, time=time)

def add_plot_legend(*, label: str =None, id: int =0, parent: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, location: int =5, horizontal: bool =False, outside: bool =False) -> int:
	"""Adds a plot legend to a plot."""
	return internal_dpg.add_plot_legend(label=label, id=id, parent=parent, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, location=location, horizontal=horizontal, outside=outside)

def add_progress_bar(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, overlay: str ='', default_value: float =0.0) -> int:
	"""Adds a progress bar."""
	return internal_dpg.add_progress_bar(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, overlay=overlay, default_value=default_value)

def add_radio_button(items : int =(), *, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', horizontal: bool =False) -> int:
	"""Adds a set of radio buttons. If items is empty, nothing will be shown."""
	return internal_dpg.add_radio_button(items=items, label=label, id=id, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, horizontal=horizontal)

def add_resize_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is resized."""
	return internal_dpg.add_resize_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_same_line(*, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, xoffset: float =0.0, spacing: float =-1.0) -> int:
	"""Places a widget on the same line as the previous widget. Can also be used for horizontal spacing."""
	return internal_dpg.add_same_line(label=label, id=id, parent=parent, before=before, show=show, xoffset=xoffset, spacing=spacing)

def add_scatter_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Adds a scatter series to a plot."""
	return internal_dpg.add_scatter_series(x, y, label=label, id=id, parent=parent, before=before, source=source, show=show, contribute_to_bounds=contribute_to_bounds)

def add_selectable(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: bool =False, span_columns: bool =False) -> int:
	"""Adds a selectable."""
	return internal_dpg.add_selectable(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, span_columns=span_columns)

def add_separator(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[]) -> int:
	"""Adds a horizontal line."""
	return internal_dpg.add_separator(label=label, id=id, indent=indent, parent=parent, before=before, show=show, pos=pos)

def add_shade_series(x : List[float], y1 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, y2: Any =[], contribute_to_bounds: bool =True) -> int:
	"""Adds a shade series to a plot."""
	return internal_dpg.add_shade_series(x, y1, label=label, id=id, parent=parent, before=before, source=source, show=show, y2=y2, contribute_to_bounds=contribute_to_bounds)

def add_simple_plot(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(), overlay: str ='', histogram: bool =False, autosize: bool =True, min_scale: float =0.0, max_scale: float =0.0) -> int:
	"""A simple plot for visualization of a 1 dimensional set of values."""
	return internal_dpg.add_simple_plot(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, overlay=overlay, histogram=histogram, autosize=autosize, min_scale=min_scale, max_scale=max_scale)

def add_slider_float(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, vertical: bool =False, no_input: bool =False, clamped: bool =False, min_value: float =0.0, max_value: float =100.0, format: str ='%.3f') -> int:
	"""Adds slider for a single float value. CTRL+Click to directly modify the value."""
	return internal_dpg.add_slider_float(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, vertical=vertical, no_input=no_input, clamped=clamped, min_value=min_value, max_value=max_value, format=format)

def add_slider_floatx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), size: int =4, no_input: bool =False, clamped: bool =False, min_value: float =0.0, max_value: float =100.0, format: str ='%.3f') -> int:
	"""Adds multi slider for up to 4 float values. CTRL+Click to directly modify the value."""
	return internal_dpg.add_slider_floatx(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, size=size, no_input=no_input, clamped=clamped, min_value=min_value, max_value=max_value, format=format)

def add_slider_int(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: int =0, vertical: bool =False, no_input: bool =False, clamped: bool =False, min_value: int =0, max_value: int =100, format: str ='%d') -> int:
	"""Adds slider for a single int value. CTRL+Click to directly modify the value."""
	return internal_dpg.add_slider_int(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, vertical=vertical, no_input=no_input, clamped=clamped, min_value=min_value, max_value=max_value, format=format)

def add_slider_intx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[int] =(0, 0, 0, 0), size: int =4, no_input: bool =False, clamped: bool =False, min_value: int =0, max_value: int =100, format: str ='%d') -> int:
	"""Adds multi slider for up to 4 int values. CTRL+Click to directly modify the value."""
	return internal_dpg.add_slider_intx(label=label, id=id, width=width, indent=indent, parent=parent, before=before, source=source, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, enabled=enabled, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, size=size, no_input=no_input, clamped=clamped, min_value=min_value, max_value=max_value, format=format)

def add_spacing(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[], count: int =1) -> int:
	"""Adds vertical spacing."""
	return internal_dpg.add_spacing(label=label, id=id, indent=indent, parent=parent, before=before, show=show, pos=pos, count=count)

def add_staging_container(*, label: str =None, id: int =0) -> int:
	"""Undocumented function"""
	return internal_dpg.add_staging_container(label=label, id=id)

def add_stair_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Adds a stair series to a plot."""
	return internal_dpg.add_stair_series(x, y, label=label, id=id, parent=parent, before=before, source=source, show=show, contribute_to_bounds=contribute_to_bounds)

def add_static_texture(width : int, height : int, default_value : List[float], *, label: str =None, id: int =0, parent: int =0, file: str ='') -> int:
	"""Undocumented function"""
	return internal_dpg.add_static_texture(width, height, default_value, label=label, id=id, parent=parent, file=file)

def add_stem_series(x : List[float], y : List[float], *, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Adds a stem series to a plot."""
	return internal_dpg.add_stem_series(x, y, label=label, id=id, indent=indent, parent=parent, before=before, source=source, show=show, contribute_to_bounds=contribute_to_bounds)

def add_tab(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, closable: bool =False, no_tooltip: bool =False, order_mode: bool =0) -> int:
	"""Adds a tab to a tab bar. Must be closed with thes end command."""
	return internal_dpg.add_tab(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, closable=closable, no_tooltip=no_tooltip, order_mode=order_mode)

def add_tab_bar(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, reorderable: bool =False) -> int:
	"""Adds a tab bar."""
	return internal_dpg.add_tab_bar(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, reorderable=reorderable)

def add_tab_button(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_reorder: bool =False, leading: bool =False, trailing: bool =False, no_tooltip: bool =False) -> int:
	"""Adds a tab button to a tab bar."""
	return internal_dpg.add_tab_button(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, filter_key=filter_key, tracked=tracked, track_offset=track_offset, no_reorder=no_reorder, leading=leading, trailing=trailing, no_tooltip=no_tooltip)

def add_table(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, header_row: bool =True, inner_width: int =0, policy: int =0, freeze_rows: int =0, freeze_columns: int =0, sort_multi: bool =False, sort_tristate: bool =False, resizable: bool =False, reorderable: bool =False, hideable: bool =False, sortable: bool =False, context_menu_in_body: bool =False, row_background: bool =False, borders_innerH: bool =False, borders_outerH: bool =False, borders_innerV: bool =False, borders_outerV: bool =False, no_host_extendX: bool =False, no_host_extendY: bool =False, no_keep_columns_visible: bool =False, precise_widths: bool =False, no_clip: bool =False, pad_outerX: bool =False, no_pad_outerX: bool =False, no_pad_innerX: bool =False, scrollX: bool =False, scrollY: bool =False) -> int:
	"""Undocumented function"""
	return internal_dpg.add_table(label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, source=source, callback=callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, header_row=header_row, inner_width=inner_width, policy=policy, freeze_rows=freeze_rows, freeze_columns=freeze_columns, sort_multi=sort_multi, sort_tristate=sort_tristate, resizable=resizable, reorderable=reorderable, hideable=hideable, sortable=sortable, context_menu_in_body=context_menu_in_body, row_background=row_background, borders_innerH=borders_innerH, borders_outerH=borders_outerH, borders_innerV=borders_innerV, borders_outerV=borders_outerV, no_host_extendX=no_host_extendX, no_host_extendY=no_host_extendY, no_keep_columns_visible=no_keep_columns_visible, precise_widths=precise_widths, no_clip=no_clip, pad_outerX=pad_outerX, no_pad_outerX=no_pad_outerX, no_pad_innerX=no_pad_innerX, scrollX=scrollX, scrollY=scrollY)

def add_table_column(*, label: str =None, id: int =0, width: int =0, parent: int =0, before: int =0, show: bool =True, init_width_or_weight: bool =0.0, default_hide: bool =False, default_sort: bool =False, width_stretch: bool =False, width_fixed: bool =False, no_resize: bool =False, no_reorder: bool =False, no_hide: bool =False, no_clip: bool =False, no_sort: bool =False, no_sort_ascending: bool =False, no_sort_descending: bool =False, no_header_width: bool =False, prefer_sort_ascending: bool =False, prefer_sort_descending: bool =False, indent_enable: bool =False, indent_disable: bool =False) -> int:
	"""Undocumented function"""
	return internal_dpg.add_table_column(label=label, id=id, width=width, parent=parent, before=before, show=show, init_width_or_weight=init_width_or_weight, default_hide=default_hide, default_sort=default_sort, width_stretch=width_stretch, width_fixed=width_fixed, no_resize=no_resize, no_reorder=no_reorder, no_hide=no_hide, no_clip=no_clip, no_sort=no_sort, no_sort_ascending=no_sort_ascending, no_sort_descending=no_sort_descending, no_header_width=no_header_width, prefer_sort_ascending=prefer_sort_ascending, prefer_sort_descending=prefer_sort_descending, indent_enable=indent_enable, indent_disable=indent_disable)

def add_table_next_column(*, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	return internal_dpg.add_table_next_column(label=label, id=id, parent=parent, before=before, show=show)

def add_table_row(*, label: str =None, id: int =0, height: int =0, parent: int =0, before: int =0, show: bool =True, filter_key: str ='') -> int:
	"""Undocumented function"""
	return internal_dpg.add_table_row(label=label, id=id, height=height, parent=parent, before=before, show=show, filter_key=filter_key)

def add_text(default_value : str ='', *, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, wrap: int =-1, bullet: bool =False, color: List[float] =(-1, -1, -1, -1), show_label: bool =False) -> int:
	"""Adds text."""
	return internal_dpg.add_text(default_value=default_value, label=label, id=id, indent=indent, parent=parent, before=before, source=source, show=show, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, wrap=wrap, bullet=bullet, color=color, show_label=show_label)

def add_text_point(x : float, y : float, *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, x_offset: int =..., y_offset: int =..., contribute_to_bounds: bool =True, vertical: bool =False) -> int:
	"""Adds a labels series to a plot."""
	return internal_dpg.add_text_point(x, y, label=label, id=id, parent=parent, before=before, source=source, show=show, x_offset=x_offset, y_offset=y_offset, contribute_to_bounds=contribute_to_bounds, vertical=vertical)

def add_texture_registry(*, label: str =None, id: int =0, show: bool =False) -> int:
	"""Undocumented function"""
	return internal_dpg.add_texture_registry(label=label, id=id, show=show)

def add_theme(*, label: str =None, id: int =0, default_theme: bool =False) -> int:
	"""Undocumented function"""
	return internal_dpg.add_theme(label=label, id=id, default_theme=default_theme)

def add_theme_color(target : int =0, value : List[int] =(0, 0, 0, 255), *, label: str =None, id: int =0, parent: int =0, category: int =0) -> int:
	"""Undocumented function"""
	return internal_dpg.add_theme_color(target=target, value=value, label=label, id=id, parent=parent, category=category)

def add_theme_style(target : int =0, x : float =1.0, y : float =-1.0, *, label: str =None, id: int =0, parent: int =0, category: int =0) -> int:
	"""Undocumented function"""
	return internal_dpg.add_theme_style(target=target, x=x, y=y, label=label, id=id, parent=parent, category=category)

def add_time_picker(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: dict ={'hour': 14, 'min': 32, 'sec': 23}, hour24: bool =False) -> int:
	"""Undocumented"""
	return internal_dpg.add_time_picker(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, filter_key=filter_key, tracked=tracked, track_offset=track_offset, default_value=default_value, hour24=hour24)

def add_toggled_open_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is toggled open."""
	return internal_dpg.add_toggled_open_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_tooltip(parent : str, *, label: str =None, id: int =0, show: bool =True) -> int:
	"""Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for."""
	return internal_dpg.add_tooltip(parent, label=label, id=id, show=show)

def add_tree_node(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: str =False, tracked: bool =False, track_offset: float =0.5, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False, selectable: bool =False) -> int:
	"""Adds a tree node to add items to. Must be closed with the end command."""
	return internal_dpg.add_tree_node(label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, default_open=default_open, open_on_double_click=open_on_double_click, open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet, selectable=selectable)

def add_viewport_drawlist(*, label: str =None, id: int =0, show: bool =True, filter_key: str ='', delay_search: str =False, front: bool =True) -> int:
	"""Draws a quad on a drawing."""
	return internal_dpg.add_viewport_drawlist(label=label, id=id, show=show, filter_key=filter_key, delay_search=delay_search, front=front)

def add_visible_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, user_data: Any =None) -> int:
	"""Adds a handler which runs a given callback when the specified item is visible."""
	return internal_dpg.add_visible_handler(parent, label=label, id=id, callback=callback, user_data=user_data)

def add_vline_series(x : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Adds a infinite vertical line series to a plot."""
	return internal_dpg.add_vline_series(x, label=label, id=id, parent=parent, before=before, source=source, show=show, contribute_to_bounds=contribute_to_bounds)

def add_window(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, user_data: Any =None, show: bool =True, delay_search: str =False, min_size: List[int] =[32, 32], max_size: List[int] =[30000, 30000], menubar: bool =False, collapsed: bool =False, autosize: bool =False, no_resize: bool =False, no_title_bar: bool =False, no_move: bool =False, no_scrollbar: bool =False, no_collapse: bool =False, horizontal_scrollbar: bool =False, no_focus_on_appearing: bool =False, no_bring_to_front_on_focus: bool =False, no_close: bool =False, no_background: bool =False, modal: bool =False, popup: bool =False, on_close: Callable =None) -> int:
	"""Creates a new window for following items to be added to."""
	return internal_dpg.add_window(label=label, id=id, width=width, height=height, indent=indent, user_data=user_data, show=show, delay_search=delay_search, min_size=min_size, max_size=max_size, menubar=menubar, collapsed=collapsed, autosize=autosize, no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move, no_scrollbar=no_scrollbar, no_collapse=no_collapse, horizontal_scrollbar=horizontal_scrollbar, no_focus_on_appearing=no_focus_on_appearing, no_bring_to_front_on_focus=no_bring_to_front_on_focus, no_close=no_close, no_background=no_background, modal=modal, popup=popup, on_close=on_close)

def cleanup_dearpygui() -> None:
	"""Undocumented"""
	return internal_dpg.cleanup_dearpygui()

def clear_selected_links(node_editor : int) -> None:
	"""Undocumented"""
	return internal_dpg.clear_selected_links(node_editor)

def clear_selected_nodes(node_editor : int) -> None:
	"""Undocumented"""
	return internal_dpg.clear_selected_nodes(node_editor)

def configure_viewport() -> None:
	"""Undocumented"""
	return internal_dpg.configure_viewport()

def create_viewport(*, title: str ='Dear PyGui', small_icon: str ='', large_icon: str ='', width: int =1280, height: int =800, x_pos: int =100, y_pos: int =100, min_width: int =250, max_width: int =10000, min_height: int =250, max_height: int =10000, resizable: bool =True, vsync: bool =True, always_on_top: bool =False, maximized_box: bool =True, minimized_box: bool =True, border: bool =True, caption: bool =True, overlapped: bool =True, clear_color: List[float] =(0, 0, 0, 255)) -> str:
	"""Undocumented"""
	return internal_dpg.create_viewport(title=title, small_icon=small_icon, large_icon=large_icon, width=width, height=height, x_pos=x_pos, y_pos=y_pos, min_width=min_width, max_width=max_width, min_height=min_height, max_height=max_height, resizable=resizable, vsync=vsync, always_on_top=always_on_top, maximized_box=maximized_box, minimized_box=minimized_box, border=border, caption=caption, overlapped=overlapped, clear_color=clear_color)

def delete_item(item : int, *, children_only: bool =False) -> None:
	"""Undocumented"""
	return internal_dpg.delete_item(item, children_only=children_only)

def does_item_exist(item : int) -> bool:
	"""Undocumented"""
	return internal_dpg.does_item_exist(item)

def draw_arrow(p1 : List[float], p2 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0, size: int =4) -> int:
	"""Draws an arrow on a drawing."""
	return internal_dpg.draw_arrow(p1, p2, label=label, id=id, parent=parent, before=before, show=show, color=color, thickness=thickness, size=size)

def draw_bezier_cubic(p1 : List[float], p2 : List[float], p3 : List[float], p4 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0, size: int =4, segments: int =0) -> int:
	"""Draws a cubic bezier curve on a drawing."""
	return internal_dpg.draw_bezier_cubic(p1, p2, p3, p4, label=label, id=id, parent=parent, before=before, show=show, color=color, thickness=thickness, size=size, segments=segments)

def draw_bezier_quadratic(p1 : List[float], p2 : List[float], p3 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0, size: int =4, segments: int =0) -> int:
	"""Draws a quadratic bezier curve on a drawing."""
	return internal_dpg.draw_bezier_quadratic(p1, p2, p3, label=label, id=id, parent=parent, before=before, show=show, color=color, thickness=thickness, size=size, segments=segments)

def draw_circle(center : List[float], radius : float, *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0, segments: int =0) -> int:
	"""Draws a circle on a drawing."""
	return internal_dpg.draw_circle(center, radius, label=label, id=id, parent=parent, before=before, show=show, color=color, fill=fill, thickness=thickness, segments=segments)

def draw_ellipse(pmin : List[float], pmax : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0, segments: int =32) -> int:
	"""Draws an ellipse on a drawing."""
	return internal_dpg.draw_ellipse(pmin, pmax, label=label, id=id, parent=parent, before=before, show=show, color=color, fill=fill, thickness=thickness, segments=segments)

def draw_image(texture_id : int, pmin : List[float], pmax : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0), color: List[int] =(255, 255, 255, 255)) -> int:
	"""Draws an image on a drawing. p_min (top-left) and p_max (bottom-right) represent corners of the rectangle the image will be drawn to.Setting the p_min equal to the p_max will sraw the image to with 1:1 scale.uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using (0.0,0.0)->(1.0,1.0) texturecoordinates will generally display the entire texture."""
	return internal_dpg.draw_image(texture_id, pmin, pmax, label=label, id=id, parent=parent, before=before, show=show, uv_min=uv_min, uv_max=uv_max, color=color)

def draw_line(p1 : List[float], p2 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0) -> int:
	"""Draws a line on a drawing."""
	return internal_dpg.draw_line(p1, p2, label=label, id=id, parent=parent, before=before, show=show, color=color, thickness=thickness)

def draw_polygon(points : List[List[float]], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0) -> int:
	"""Draws a polygon on a drawing."""
	return internal_dpg.draw_polygon(points, label=label, id=id, parent=parent, before=before, show=show, color=color, fill=fill, thickness=thickness)

def draw_polyline(points : List[List[float]], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, closed: bool =False, color: List[int] =(255, 255, 255, 255), thickness: float =1.0) -> int:
	"""Draws lines on a drawing."""
	return internal_dpg.draw_polyline(points, label=label, id=id, parent=parent, before=before, show=show, closed=closed, color=color, thickness=thickness)

def draw_quad(p1 : List[float], p2 : List[float], p3 : List[float], p4 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0) -> int:
	"""Draws a quad on a drawing."""
	return internal_dpg.draw_quad(p1, p2, p3, p4, label=label, id=id, parent=parent, before=before, show=show, color=color, fill=fill, thickness=thickness)

def draw_rectangle(pmin : List[float], pmax : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), rounding: float =0.0, thickness: float =1.0) -> int:
	"""Draws a rectangle on a drawing."""
	return internal_dpg.draw_rectangle(pmin, pmax, label=label, id=id, parent=parent, before=before, show=show, color=color, fill=fill, rounding=rounding, thickness=thickness)

def draw_text(pos : List[float], text : str, *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), size: int =10) -> int:
	"""Draws a text on a drawing."""
	return internal_dpg.draw_text(pos, text, label=label, id=id, parent=parent, before=before, show=show, color=color, size=size)

def draw_triangle(p1 : List[float], p2 : List[float], p3 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0) -> int:
	"""Draws a triangle on a drawing."""
	return internal_dpg.draw_triangle(p1, p2, p3, label=label, id=id, parent=parent, before=before, show=show, color=color, fill=fill, thickness=thickness)

def empty_container_stack() -> None:
	"""Undocumented"""
	return internal_dpg.empty_container_stack()

def enable_docking(*, dock_space: bool =False) -> None:
	"""Undocumented"""
	return internal_dpg.enable_docking(dock_space=dock_space)

def focus_item(item : int) -> None:
	"""Undocumented"""
	return internal_dpg.focus_item(item)

def generate_uuid() -> int:
	"""Undocumented"""
	return internal_dpg.generate_uuid()

def get_active_window() -> int:
	"""Undocumented"""
	return internal_dpg.get_active_window()

def get_all_items() -> List[int]:
	"""Undocumented"""
	return internal_dpg.get_all_items()

def get_axis_limits(axis : int) -> List[float]:
	"""Undocumented function"""
	return internal_dpg.get_axis_limits(axis)

def get_dearpygui_version() -> str:
	"""Undocumented"""
	return internal_dpg.get_dearpygui_version()

def get_delta_time() -> float:
	"""Undocumented"""
	return internal_dpg.get_delta_time()

def get_drawing_mouse_pos() -> List[int]:
	"""Undocumented"""
	return internal_dpg.get_drawing_mouse_pos()

def get_file_dialog_info(file_dialog : int) -> dict:
	"""Undocumented function"""
	return internal_dpg.get_file_dialog_info(file_dialog)

def get_global_font_scale() -> float:
	"""Undocumented function"""
	return internal_dpg.get_global_font_scale()

def get_item_configuration(item : int) -> dict:
	"""Undocumented"""
	return internal_dpg.get_item_configuration(item)

def get_item_info(item : int) -> dict:
	"""Undocumented"""
	return internal_dpg.get_item_info(item)

def get_item_state(item : int) -> dict:
	"""Undocumented"""
	return internal_dpg.get_item_state(item)

def get_item_types() -> dict:
	"""Undocumented"""
	return internal_dpg.get_item_types()

def get_links(node_editor : int) -> List[List[str]]:
	"""Undocumented"""
	return internal_dpg.get_links(node_editor)

def get_mouse_drag_delta() -> float:
	"""Undocumented"""
	return internal_dpg.get_mouse_drag_delta()

def get_mouse_pos(*, local: bool =True) -> List[int]:
	"""Undocumented"""
	return internal_dpg.get_mouse_pos(local=local)

def get_plot_mouse_pos() -> List[int]:
	"""Undocumented"""
	return internal_dpg.get_plot_mouse_pos()

def get_plot_query_area(plot : int) -> List[float]:
	"""Undocumented function"""
	return internal_dpg.get_plot_query_area(plot)

def get_selected_links(node_editor : int) -> List[List[str]]:
	"""Undocumented"""
	return internal_dpg.get_selected_links(node_editor)

def get_selected_nodes(node_editor : int) -> List[int]:
	"""Undocumented"""
	return internal_dpg.get_selected_nodes(node_editor)

def get_total_time() -> float:
	"""Undocumented"""
	return internal_dpg.get_total_time()

def get_value(item : int) -> Any:
	"""Undocumented"""
	return internal_dpg.get_value(item)

def get_values(items : List[int]) -> Any:
	"""Undocumented"""
	return internal_dpg.get_values(items)

def get_viewport_configuration(item : int) -> dict:
	"""Undocumented"""
	return internal_dpg.get_viewport_configuration(item)

def get_windows() -> List[int]:
	"""Undocumented"""
	return internal_dpg.get_windows()

def get_x_scroll(item : int) -> float:
	"""Undocumented function"""
	return internal_dpg.get_x_scroll(item)

def get_x_scroll_max(item : int) -> float:
	"""Undocumented function"""
	return internal_dpg.get_x_scroll_max(item)

def get_y_scroll(item : int) -> float:
	"""Undocumented function"""
	return internal_dpg.get_y_scroll(item)

def get_y_scroll_max(item : int) -> float:
	"""Undocumented function"""
	return internal_dpg.get_y_scroll_max(item)

def is_dearpygui_running() -> bool:
	"""Undocumented"""
	return internal_dpg.is_dearpygui_running()

def is_key_down(key : int) -> bool:
	"""Undocumented"""
	return internal_dpg.is_key_down(key)

def is_key_pressed(key : int) -> bool:
	"""Undocumented"""
	return internal_dpg.is_key_pressed(key)

def is_key_released(key : int) -> bool:
	"""Undocumented"""
	return internal_dpg.is_key_released(key)

def is_mouse_button_clicked(button : int) -> bool:
	"""Undocumented"""
	return internal_dpg.is_mouse_button_clicked(button)

def is_mouse_button_double_clicked(button : int) -> bool:
	"""Undocumented"""
	return internal_dpg.is_mouse_button_double_clicked(button)

def is_mouse_button_down(button : int) -> bool:
	"""Undocumented"""
	return internal_dpg.is_mouse_button_down(button)

def is_mouse_button_dragging(button : int, threshold : float) -> bool:
	"""Undocumented"""
	return internal_dpg.is_mouse_button_dragging(button, threshold)

def is_mouse_button_released(button : int) -> bool:
	"""Undocumented"""
	return internal_dpg.is_mouse_button_released(button)

def is_plot_queried(plot : int) -> bool:
	"""Undocumented function"""
	return internal_dpg.is_plot_queried(plot)

def last_container() -> int:
	"""Undocumented"""
	return internal_dpg.last_container()

def last_item() -> int:
	"""Undocumented"""
	return internal_dpg.last_item()

def last_root() -> int:
	"""Undocumented"""
	return internal_dpg.last_root()

def maximize_viewport() -> None:
	"""Undocumented"""
	return internal_dpg.maximize_viewport()

def minimize_viewport() -> None:
	"""Undocumented"""
	return internal_dpg.minimize_viewport()

def move_item(item : int, *, parent: int =0, before: int =0) -> None:
	"""Undocumented"""
	return internal_dpg.move_item(item, parent=parent, before=before)

def move_item_down(item : int) -> None:
	"""Undocumented"""
	return internal_dpg.move_item_down(item)

def move_item_up(item : int) -> None:
	"""Undocumented"""
	return internal_dpg.move_item_up(item)

def pop_container_stack() -> int:
	"""Undocumented"""
	return internal_dpg.pop_container_stack()

def push_container_stack(item : int) -> bool:
	"""Undocumented"""
	return internal_dpg.push_container_stack(item)

def render_dearpygui_frame() -> None:
	"""Undocumented"""
	return internal_dpg.render_dearpygui_frame()

def reorder_items(container : int, slot : int, new_order : List[int]) -> None:
	"""Undocumented"""
	return internal_dpg.reorder_items(container, slot, new_order)

def reset_axis_ticks(axis : int) -> None:
	"""Undocumented function"""
	return internal_dpg.reset_axis_ticks(axis)

def reset_pos(item : int) -> None:
	"""Undocumented"""
	return internal_dpg.reset_pos(item)

def set_axis_limits(axis : int, ymin : float, ymax : float) -> None:
	"""Undocumented function"""
	return internal_dpg.set_axis_limits(axis, ymin, ymax)

def set_axis_ticks(axis : int, label_pairs : Any) -> None:
	"""Undocumented function"""
	return internal_dpg.set_axis_ticks(axis, label_pairs)

def set_exit_callback(callback : Callable) -> str:
	"""Undocumented function"""
	return internal_dpg.set_exit_callback(callback)

def set_global_font_scale(scale : float) -> None:
	"""Undocumented function"""
	return internal_dpg.set_global_font_scale(scale)

def set_item_children(item : int, source : int, slot : int) -> dict:
	"""Undocumented"""
	return internal_dpg.set_item_children(item, source, slot)

def set_item_disabled_theme(item : int, theme : int) -> None:
	"""Undocumented"""
	return internal_dpg.set_item_disabled_theme(item, theme)

def set_item_font(item : int, font : int) -> None:
	"""Undocumented"""
	return internal_dpg.set_item_font(item, font)

def set_item_pos(item : int, x : float, y : float) -> None:
	"""Undocumented"""
	return internal_dpg.set_item_pos(item, x, y)

def set_item_theme(item : int, theme : int) -> None:
	"""Undocumented"""
	return internal_dpg.set_item_theme(item, theme)

def set_item_type_disabled_theme(item : int, theme : int) -> None:
	"""Undocumented"""
	return internal_dpg.set_item_type_disabled_theme(item, theme)

def set_item_type_theme(item : int, theme : int) -> None:
	"""Undocumented"""
	return internal_dpg.set_item_type_theme(item, theme)

def set_primary_window(window : int, value : bool) -> None:
	"""Undocumented"""
	return internal_dpg.set_primary_window(window, value)

def set_staging_mode(mode : bool) -> None:
	"""Undocumented"""
	return internal_dpg.set_staging_mode(mode)

def set_start_callback(callback : Callable) -> str:
	"""Undocumented function"""
	return internal_dpg.set_start_callback(callback)

def set_value(item : int, value : Any) -> None:
	"""Undocumented"""
	return internal_dpg.set_value(item, value)

def set_viewport_resize_callback(callback : Callable) -> str:
	"""Undocumented function"""
	return internal_dpg.set_viewport_resize_callback(callback)

def set_x_scroll(item : int, value : float) -> None:
	"""Undocumented function"""
	return internal_dpg.set_x_scroll(item, value)

def set_y_scroll(item : int, value : float) -> None:
	"""Undocumented function"""
	return internal_dpg.set_y_scroll(item, value)

def setup_dearpygui(*, viewport: str ='') -> None:
	"""Undocumented"""
	return internal_dpg.setup_dearpygui(viewport=viewport)

def show_imgui_demo() -> None:
	"""Undocumented"""
	return internal_dpg.show_imgui_demo()

def show_implot_demo() -> None:
	"""Undocumented"""
	return internal_dpg.show_implot_demo()

def show_tool(tool : int) -> str:
	"""Undocumented function"""
	return internal_dpg.show_tool(tool)

def show_viewport(viewport : str, *, minimized: bool =False, maximized: bool =False) -> None:
	"""Undocumented"""
	return internal_dpg.show_viewport(viewport, minimized=minimized, maximized=maximized)

def stage_items(items : List[int]) -> None:
	"""Undocumented"""
	return internal_dpg.stage_items(items)

def stop_dearpygui() -> None:
	"""Undocumented"""
	return internal_dpg.stop_dearpygui()

def top_container_stack() -> int:
	"""Undocumented"""
	return internal_dpg.top_container_stack()

def unstage_items(items : List[int]) -> None:
	"""Undocumented"""
	return internal_dpg.unstage_items(items)


##########################################################
# Constants #
##########################################################

mvMouseButton_Left=0
mvMouseButton_Right=1
mvMouseButton_Middle=2
mvMouseButton_X1=3
mvMouseButton_X2=4
mvKey_0=48
mvKey_1=49
mvKey_2=50
mvKey_3=51
mvKey_4=52
mvKey_5=53
mvKey_6=54
mvKey_7=55
mvKey_8=56
mvKey_9=57
mvKey_A=65
mvKey_B=66
mvKey_C=67
mvKey_D=68
mvKey_E=69
mvKey_F=70
mvKey_G=71
mvKey_H=72
mvKey_I=73
mvKey_J=74
mvKey_K=75
mvKey_L=76
mvKey_M=77
mvKey_N=78
mvKey_O=79
mvKey_P=80
mvKey_Q=81
mvKey_R=82
mvKey_S=83
mvKey_T=84
mvKey_U=85
mvKey_V=86
mvKey_W=87
mvKey_X=88
mvKey_Y=89
mvKey_Z=90
mvKey_Back=8
mvKey_Tab=9
mvKey_Clear=12
mvKey_Return=13
mvKey_Shift=16
mvKey_Control=17
mvKey_Alt=18
mvKey_Pause=19
mvKey_Capital=20
mvKey_Escape=27
mvKey_Spacebar=32
mvKey_Prior=33
mvKey_Next=34
mvKey_End=35
mvKey_Home=36
mvKey_Left=37
mvKey_Up=38
mvKey_Right=39
mvKey_Down=40
mvKey_Select=41
mvKey_Print=42
mvKey_Execute=43
mvKey_PrintScreen=44
mvKey_Insert=45
mvKey_Delete=46
mvKey_Help=47
mvKey_LWin=91
mvKey_RWin=92
mvKey_Apps=93
mvKey_Sleep=95
mvKey_NumPad0=96
mvKey_NumPad1=97
mvKey_NumPad2=98
mvKey_NumPad3=99
mvKey_NumPad4=100
mvKey_NumPad5=101
mvKey_NumPad6=102
mvKey_NumPad7=103
mvKey_NumPad8=104
mvKey_NumPad9=105
mvKey_Multiply=106
mvKey_Add=107
mvKey_Separator=108
mvKey_Subtract=109
mvKey_Decimal=110
mvKey_Divide=111
mvKey_F1=112
mvKey_F2=113
mvKey_F3=114
mvKey_F4=115
mvKey_F5=116
mvKey_F6=117
mvKey_F7=118
mvKey_F8=119
mvKey_F9=120
mvKey_F10=121
mvKey_F11=122
mvKey_F12=123
mvKey_F13=124
mvKey_F14=125
mvKey_F15=126
mvKey_F16=127
mvKey_F17=128
mvKey_F18=129
mvKey_F19=130
mvKey_F20=131
mvKey_F21=132
mvKey_F22=133
mvKey_F23=134
mvKey_F24=135
mvKey_NumLock=144
mvKey_ScrollLock=145
mvKey_LShift=160
mvKey_RShift=161
mvKey_LControl=162
mvKey_RControl=163
mvKey_LMenu=164
mvKey_RMenu=165
mvKey_Browser_Back=166
mvKey_Browser_Forward=167
mvKey_Browser_Refresh=168
mvKey_Browser_Stop=169
mvKey_Browser_Search=170
mvKey_Browser_Favorites=171
mvKey_Browser_Home=172
mvKey_Volume_Mute=173
mvKey_Volume_Down=174
mvKey_Volume_Up=175
mvKey_Media_Next_Track=176
mvKey_Media_Prev_Track=177
mvKey_Media_Stop=178
mvKey_Media_Play_Pause=179
mvKey_Launch_Mail=180
mvKey_Launch_Media_Select=181
mvKey_Launch_App1=182
mvKey_Launch_App2=183
mvKey_Colon=186
mvKey_Plus=187
mvKey_Comma=188
mvKey_Minus=189
mvKey_Period=190
mvKey_Slash=191
mvKey_Tilde=192
mvKey_Open_Brace=219
mvKey_Backslash=220
mvKey_Close_Brace=221
mvKey_Quote=222
mvSpacing=1
mvSameLine=2
mvInputText=3
mvButton=4
mvTool_About=3
mvTool_Debug=4
mvTool_Doc=5
mvTool_ItemRegistry=6
mvTool_Metrics=7
mvTool_Style=8
mvTool_Font=9
mvFontAtlas=2
mvAppUUID=1
mvInvalidUUID=0
mvDir_None=-1
mvDir_Left=0
mvDir_Right=1
mvDir_Up=2
mvDir_Down=3
mvRadioButton=5
mvTabBar=6
mvTab=7
mvTabOrder_Reorderable=0
mvTabOrder_Fixed=1
mvTabOrder_Leading=2
mvTabOrder_Trailing=3
mvImage=8
mvMenuBar=9
mvMenu=10
mvMenuItem=11
mvGroup=12
mvChild=13
mvSliderFloat=14
mvSliderInt=15
mvFilterSet=16
mvDragFloat=17
mvDragInt=18
mvInputFloat=19
mvInputInt=20
mvColorEdit=21
mvColorEdit_AlphaPreviewNone=0
mvColorEdit_AlphaPreview=131072
mvColorEdit_AlphaPreviewHalf=262144
mvColorEdit_uint8=8388608
mvColorEdit_float=16777216
mvColorEdit_rgb=1048576
mvColorEdit_hsv=2097152
mvColorEdit_hex=4194304
mvColorEdit_input_rgb=134217728
mvColorEdit_input_hsv=268435456
mvClipper=22
mvColorPicker=23
mvColorPicker_bar=33554432
mvColorPicker_wheel=67108864
mvTooltip=24
mvCollapsingHeader=25
mvSeparator=26
mvCheckbox=27
mvListbox=28
mvText=29
mvCombo=30
mvComboHeight_Small=0
mvComboHeight_Regular=1
mvComboHeight_Large=2
mvComboHeight_Largest=3
mvPlot=31
mvPlotMarker_None=-1
mvPlotMarker_Circle=0
mvPlotMarker_Square=1
mvPlotMarker_Diamond=2
mvPlotMarker_Up=3
mvPlotMarker_Down=4
mvPlotMarker_Left=5
mvPlotMarker_Right=6
mvPlotMarker_Cross=7
mvPlotMarker_Plus=8
mvPlotMarker_Asterisk=9
mvPlotColormap_Default=0
mvPlotColormap_Deep=1
mvPlotColormap_Dark=2
mvPlotColormap_Pastel=3
mvPlotColormap_Paired=4
mvPlotColormap_Viridis=5
mvPlotColormap_Plasma=6
mvPlotColormap_Hot=7
mvPlotColormap_Cool=8
mvPlotColormap_Pink=9
mvPlotColormap_Jet=10
mvSimplePlot=32
mvDrawlist=33
mvWindowAppItem=34
mvSelectable=35
mvTreeNode=36
mvProgressBar=37
mvDummy=38
mvImageButton=39
mvTimePicker=40
mvDatePicker=41
mvColorButton=42
mvFileDialog=43
mvTabButton=44
mvNodeEditor=45
mvNode=46
mvNodeAttribute=47
mvNode_PinShape_Circle=0
mvNode_PinShape_CircleFilled=1
mvNode_PinShape_Triangle=2
mvNode_PinShape_TriangleFilled=3
mvNode_PinShape_Quad=4
mvNode_PinShape_QuadFilled=5
mvNode_Attr_Input=0
mvNode_Attr_Output=1
mvNode_Attr_Static=2
mvTable=48
mvTable_SizingFixedFit=8192
mvTable_SizingFixedSame=16384
mvTable_SizingStretchProp=24576
mvTable_SizingStretchSame=32768
mvTableColumn=49
mvTableNextColumn=50
mvTableRow=51
mvDrawLine=52
mvDrawArrow=53
mvDrawTriangle=54
mvDrawCircle=55
mvDrawEllipse=56
mvDrawBezierCubic=57
mvDrawBezierQuadratic=58
mvDrawQuad=59
mvDrawRect=60
mvDrawText=61
mvDrawPolygon=62
mvDrawPolyline=63
mvDrawImage=64
mvDragFloatMulti=65
mvDragIntMulti=66
mvSliderFloatMulti=67
mvSliderIntMulti=68
mvInputIntMulti=69
mvInputFloatMulti=70
mvDragPoint=71
mvDragLine=72
mvAnnotation=73
mvLineSeries=74
mvScatterSeries=75
mvStemSeries=76
mvStairSeries=77
mvBarSeries=78
mvErrorSeries=79
mvVLineSeries=80
mvHLineSeries=81
mvHeatSeries=82
mvImageSeries=83
mvPieSeries=84
mvShadeSeries=85
mvLabelSeries=86
mvHistogramSeries=87
mvPlotBin_Sqrt=-1
mvPlotBin_Sturges=-2
mvPlotBin_Rice=-3
mvPlotBin_Scott=-4
mv2dHistogramSeries=88
mvCandleSeries=89
mvAreaSeries=90
mvColorMapScale=91
mvSlider3D=92
mvKnobFloat=93
mvLoadingIndicator=94
mvNodeLink=95
mvTextureRegistry=96
mvStaticTexture=97
mvDynamicTexture=98
mvStagingContainer=99
mvDrawLayer=100
mvViewportDrawlist=101
mvFileExtension=102
mvPlotLegend=103
mvPlot_Location_Center=0
mvPlot_Location_North=1
mvPlot_Location_South=2
mvPlot_Location_West=4
mvPlot_Location_East=8
mvPlot_Location_NorthWest=5
mvPlot_Location_NorthEast=9
mvPlot_Location_SouthWest=6
mvPlot_Location_SouthEast=10
mvPlotAxis=104
mvXAxis=0
mvYAxis=1
mvHandlerRegistry=105
mvKeyDownHandler=106
mvKeyPressHandler=107
mvKeyReleaseHandler=108
mvMouseMoveHandler=109
mvMouseWheelHandler=110
mvMouseClickHandler=111
mvMouseDoubleClickHandler=112
mvMouseDownHandler=113
mvMouseReleaseHandler=114
mvMouseDragHandler=115
mvHoverHandler=116
mvActiveHandler=117
mvFocusHandler=118
mvVisibleHandler=119
mvEditedHandler=120
mvActivatedHandler=121
mvDeactivatedHandler=122
mvDeactivatedAfterEditHandler=123
mvToggledOpenHandler=124
mvClickedHandler=125
mvDragPayload=126
mvResizeHandler=127
mvFont=128
mvFontRegistry=129
mvTheme=130
mvThemeColor=131
mvThemeCol_Text=0
mvThemeCol_TextDisabled=1
mvThemeCol_WindowBg=2
mvThemeCol_ChildBg=3
mvThemeCol_Border=5
mvThemeCol_PopupBg=4
mvThemeCol_BorderShadow=6
mvThemeCol_FrameBg=7
mvThemeCol_FrameBgHovered=8
mvThemeCol_FrameBgActive=9
mvThemeCol_TitleBg=10
mvThemeCol_TitleBgActive=11
mvThemeCol_TitleBgCollapsed=12
mvThemeCol_MenuBarBg=13
mvThemeCol_ScrollbarBg=14
mvThemeCol_ScrollbarGrab=15
mvThemeCol_ScrollbarGrabHovered=16
mvThemeCol_ScrollbarGrabActive=17
mvThemeCol_CheckMark=18
mvThemeCol_SliderGrab=19
mvThemeCol_SliderGrabActive=20
mvThemeCol_Button=21
mvThemeCol_ButtonHovered=22
mvThemeCol_ButtonActive=23
mvThemeCol_Header=24
mvThemeCol_HeaderHovered=25
mvThemeCol_HeaderActive=26
mvThemeCol_Separator=27
mvThemeCol_SeparatorHovered=28
mvThemeCol_SeparatorActive=29
mvThemeCol_ResizeGrip=30
mvThemeCol_ResizeGripHovered=31
mvThemeCol_ResizeGripActive=32
mvThemeCol_Tab=33
mvThemeCol_TabHovered=34
mvThemeCol_TabActive=35
mvThemeCol_TabUnfocused=36
mvThemeCol_TabUnfocusedActive=37
mvThemeCol_DockingPreview=38
mvThemeCol_DockingEmptyBg=39
mvThemeCol_PlotLines=40
mvThemeCol_PlotLinesHovered=41
mvThemeCol_PlotHistogram=42
mvThemeCol_PlotHistogramHovered=43
mvThemeCol_TableHeaderBg=44
mvThemeCol_TableBorderStrong=45
mvThemeCol_TableBorderLight=46
mvThemeCol_TableRowBg=47
mvThemeCol_TableRowBgAlt=48
mvThemeCol_TextSelectedBg=49
mvThemeCol_DragDropTarget=50
mvThemeCol_NavHighlight=51
mvThemeCol_NavWindowingHighlight=52
mvThemeCol_NavWindowingDimBg=53
mvThemeCol_ModalWindowDimBg=54
mvPlotCol_Line=0
mvPlotCol_Fill=1
mvPlotCol_MarkerOutline=2
mvPlotCol_MarkerFill=3
mvPlotCol_ErrorBar=4
mvPlotCol_FrameBg=5
mvPlotCol_PlotBg=6
mvPlotCol_PlotBorder=7
mvPlotCol_LegendBg=8
mvPlotCol_LegendBorder=9
mvPlotCol_LegendText=10
mvPlotCol_TitleText=11
mvPlotCol_InlayText=12
mvPlotCol_XAxis=13
mvPlotCol_XAxisGrid=14
mvPlotCol_YAxis=15
mvPlotCol_YAxisGrid=16
mvPlotCol_YAxis2=17
mvPlotCol_YAxisGrid2=18
mvPlotCol_YAxis3=19
mvPlotCol_YAxisGrid3=20
mvPlotCol_Selection=21
mvPlotCol_Query=22
mvPlotCol_Crosshairs=23
mvNodeCol_NodeBackground=0
mvNodeCol_NodeBackgroundHovered=1
mvNodeCol_NodeBackgroundSelected=2
mvNodeCol_NodeOutline=3
mvNodeCol_TitleBar=4
mvNodeCol_TitleBarHovered=5
mvNodeCol_TitleBarSelected=6
mvNodeCol_Link=7
mvNodeCol_LinkHovered=8
mvNodeCol_LinkSelected=9
mvNodeCol_Pin=10
mvNodeCol_PinHovered=11
mvNodeCol_BoxSelector=12
mvNodeCol_BoxSelectorOutline=13
mvNodeCol_GridBackground=14
mvNodeCol_GridLine=15
mvThemeStyle=132
mvStyleVar_Alpha=0
mvStyleVar_WindowPadding=1
mvStyleVar_WindowRounding=2
mvStyleVar_WindowBorderSize=3
mvStyleVar_WindowMinSize=4
mvStyleVar_WindowTitleAlign=5
mvStyleVar_ChildRounding=6
mvStyleVar_ChildBorderSize=7
mvStyleVar_PopupRounding=8
mvStyleVar_PopupBorderSize=9
mvStyleVar_FramePadding=10
mvStyleVar_FrameRounding=11
mvStyleVar_FrameBorderSize=12
mvStyleVar_ItemSpacing=13
mvStyleVar_ItemInnerSpacing=14
mvStyleVar_IndentSpacing=15
mvStyleVar_CellPadding=16
mvStyleVar_ScrollbarSize=17
mvStyleVar_ScrollbarRounding=18
mvStyleVar_GrabMinSize=19
mvStyleVar_GrabRounding=20
mvStyleVar_TabRounding=21
mvStyleVar_ButtonTextAlign=22
mvStyleVar_SelectableTextAlign=23
mvPlotStyleVar_LineWeight=0
mvPlotStyleVar_Marker=1
mvPlotStyleVar_MarkerSize=2
mvPlotStyleVar_MarkerWeight=3
mvPlotStyleVar_FillAlpha=4
mvPlotStyleVar_ErrorBarSize=5
mvPlotStyleVar_ErrorBarWeight=6
mvPlotStyleVar_DigitalBitHeight=7
mvPlotStyleVar_DigitalBitGap=8
mvPlotStyleVar_PlotBorderSize=9
mvPlotStyleVar_MinorAlpha=10
mvPlotStyleVar_MajorTickLen=11
mvPlotStyleVar_MinorTickLen=12
mvPlotStyleVar_MajorTickSize=13
mvPlotStyleVar_MinorTickSize=14
mvPlotStyleVar_MajorGridSize=15
mvPlotStyleVar_MinorGridSize=16
mvPlotStyleVar_PlotPadding=17
mvPlotStyleVar_LabelPadding=18
mvPlotStyleVar_LegendPadding=19
mvPlotStyleVar_LegendInnerPadding=20
mvPlotStyleVar_LegendSpacing=21
mvPlotStyleVar_MousePosPadding=22
mvPlotStyleVar_AnnotationPadding=23
mvPlotStyleVar_FitPadding=24
mvPlotStyleVar_PlotDefaultSize=25
mvPlotStyleVar_PlotMinSize=26
mvNodeStyleVar_GridSpacing=0
mvNodeStyleVar_NodeCornerRounding=1
mvNodeStyleVar_NodePaddingHorizontal=2
mvNodeStyleVar_NodePaddingVertical=3
mvNodeStyleVar_NodeBorderThickness=4
mvNodeStyleVar_LinkThickness=5
mvNodeStyleVar_LinkLineSegmentsPerLength=6
mvNodeStyleVar_LinkHoverDistance=7
mvNodeStyleVar_PinCircleRadius=8
mvNodeStyleVar_PinQuadSideLength=9
mvNodeStyleVar_PinTriangleSideLength=10
mvNodeStyleVar_PinLineThickness=11
mvNodeStyleVar_PinHoverRadius=12
mvNodeStyleVar_PinOffset=13
mvFontRangeHint=133
mvFontRangeHint_Default=0
mvFontRangeHint_Japanese=1
mvFontRangeHint_Korean=2
mvFontRangeHint_Chinese_Full=3
mvFontRangeHint_Chinese_Simplified_Common=4
mvFontRangeHint_Cyrillic=5
mvFontRangeHint_Thai=6
mvFontRangeHint_Vietnamese=7
mvFontRange=134
mvFontChars=135
mvCharRemap=136
