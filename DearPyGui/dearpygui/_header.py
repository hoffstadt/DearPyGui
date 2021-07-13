from typing import List, Any, Callable, Union
from contextlib import contextmanager
import dearpygui._dearpygui as internal_dpg
from dearpygui._dearpygui import mvBuffer

########################################################################################################################
# User API Index
#
#    * Sections
#      - Helper Commands
#      - Tool Commands
#      - Information Commands
#      - Configuration Getter Commands
#      - Configuration Setter Commands
#      - State Commands
#      - Viewport Setter Commands
#      - Viewport Getter Commands
#      - Container Context Managers
#      - Core Wrappings
#      - Constants
#
########################################################################################################################

########################################################################################################################
# Helper Commands
########################################################################################################################

def configure_item(item : int, **kwargs) -> None:
	"""Configures an item after creation."""
	internal_dpg.configure_item(item, **kwargs)


def configure_viewport(item : int, **kwargs) -> None:
	"""Configures a viewport after creation."""
	internal_dpg.configure_viewport(item, **kwargs)


def setup_registries() -> None:
    """Adds default registries for fonts, handlers, textures, and values."""
    internal_dpg.add_font_registry(id=internal_dpg.mvReservedUUID_0)
    internal_dpg.add_handler_registry(id=internal_dpg.mvReservedUUID_1)
    internal_dpg.add_texture_registry(id=internal_dpg.mvReservedUUID_2)
    internal_dpg.add_value_registry(id=internal_dpg.mvReservedUUID_3)


def setup_viewport():
    """Prepares viewport. If not called, start_dearpygui will automatically call this.

    Returns:
        None
    """
    vp = internal_dpg.create_viewport()
    internal_dpg.setup_dearpygui(viewport=vp)
    internal_dpg.show_viewport(vp)


def start_dearpygui():
    """Prepares viewport (if not done already). sets up, cleans up, and runs main event loop.

    Returns:
        None
    """

    if not internal_dpg.is_viewport_created():
        setup_viewport()

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


########################################################################################################################
# Tool Commands
########################################################################################################################

def show_style_editor() -> None:
    """Shows the standard style editor window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Style)


def show_metrics() -> None:
    """Shows the standard metrics window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Metrics)


def show_about() -> None:
    """Shows the standard about window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_About)


def show_debug() -> None:
    """Shows the standard debug window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Debug)


def show_documentation() -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Doc)


def show_font_manager() -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Font)


def show_item_registry() -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_ItemRegistry)


########################################################################################################################
# Information Commands
########################################################################################################################

def get_item_slot(item: int) -> Union[int, None]:
    """Returns an items target slot.

    Returns:
        slot as a int
    """
    return internal_dpg.get_item_info(item)["target"]

def is_item_container(item: int) -> Union[bool, None]:
    """Checks if item is a container.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_info(item)["container"]


def get_item_parent(item: int) -> Union[int, None]:
    """Gets the item's parent.

    Returns:
        parent as a int or None
    """
    return internal_dpg.get_item_info(item)["parent"]


def get_item_children(item: int , slot: int = -1) -> Union[List[int], None]:
    """Provides access to the item's children slots.

    Returns:
        A 2-D tuple of children slots ex. ((child_slot_1),(child_slot_2),(child_slot_3),...) or a single slot if slot is used.
    """
    if slot < 0 or slot > 4:
        return internal_dpg.get_item_info(item)["children"]
    return internal_dpg.get_item_info(item)["children"][slot]


def get_item_type(item: int) -> Union[str]:
    """Gets the item's type.

    Returns:
        type as a string or None
    """
    return internal_dpg.get_item_info(item)["type"]


def get_item_theme(item: int) -> int:
    """Gets the item's theme.

    Returns:
        theme's uuid
    """
    return internal_dpg.get_item_info(item)["theme"]


def get_item_font(item: int) -> int:
    """Gets the item's font.

    Returns:
        font's uuid
    """
    return internal_dpg.get_item_info(item)["font"]


def get_item_disabled_theme(item: int) -> int:
    """Gets the item's disabled theme.

    Returns:
        theme's uuid
    """
    return internal_dpg.get_item_info(item)["disabled_theme"]


########################################################################################################################
# Configuration Setter Commands
########################################################################################################################

def enable_item(item: int):
    """Enables the item.

    Args:
        **item: Item to enable.

    Returns:
        None
    """
    internal_dpg.configure_item(item, enabled=True)


def disable_item(item: int):
    """Disables the item.

    Args:
        **item: Item to disable.

    Returns:
        None
    """
    internal_dpg.configure_item(item, enabled=False)


def set_item_label(item: int, label: str):
    """Sets the item's displayed label, anything after the characters "##" in the name will not be shown.

    Args:
        item: Item label will be applied to.
        label: Displayed name to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, label=label)


def set_item_source(item: int, source: int):
    """Sets the item's value, to the source's value. Widget's value will now be "linked" to source's value.

    Args:
        item: Item to me linked.
        source: Source to link to.

    Returns:
        None
    """
    internal_dpg.configure_item(item, source=source)


def set_item_pos(item: int, pos: List[float]):
    """Sets the item's position.

    Args:
        item: Item the absolute position will be applied to.
        pos: X and Y positions relative to parent of the item.

    Returns:
        None
    """
    internal_dpg.configure_item(item, pos=pos)


def set_item_width(item: int, width: int):
    """Sets the item's width.

    Args:
        item: Item the Width will be applied to.
        width: Width to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, width=width)


def set_item_height(item: int, height: int):
    """Sets the item's height.

    Args:
        item: Item the Height will be applied to.
        height: Height to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, height=height)


def set_item_indent(item: int, indent: int):
    """Sets the item's indent.

    Args:
        item: Item the Height will be applied to.
        height: Height to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, indent=indent)


def set_item_track_offset(item: int, offset: float):
    """Sets the item's track offset.

    Args:
        item: Item the Height will be applied to.
        height: Height to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, track_offset=offset)


def set_item_payload_type(item: int, payload_type: str):
    """Sets the item's payload type.

    Args:
        item: Item the Height will be applied to.
        height: Height to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, payload_type=str)


def set_item_callback(item: int, callback: Callable):
    """Sets the item's callack.

    Args:
        item: Item the callback will be applied to.
        callback: Callback to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, callback=callback)


def set_item_drag_callback(item: int, callback: Callable):
    """Sets the item's drag callack.

    Args:
        item: Item the callback will be applied to.
        callback: Callback to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, drag_callback=callback)


def set_item_drop_callback(item: int, callback: Callable):
    """Sets the item's drop callack.

    Args:
        item: Item the callback will be applied to.
        callback: Callback to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, drop_callback=callback)


def track_item(item: int):
    """Track item in scroll region.

    Args:
        item: Item the callback will be applied to.
        callback: Callback to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, tracked=True)


def untrack_item(item: int):
    """Track item in scroll region.

    Args:
        item: Item the callback will be applied to.
        callback: Callback to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, tracked=False)


def set_item_user_data(item: int, user_data: Any):
    """Sets the item's callack_data to any python object.

    Args:
        item: Item the callback will be applied to.
        user_data: Callback_data to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, user_data=user_data)


def show_item(item: int):
    """Shows the item.

    Args:
        item: Item to show.

    Returns:
        None
    """
    internal_dpg.configure_item(item, show=True)


def hide_item(item: int, *, children_only: bool = False):
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


########################################################################################################################
# Configuration Getter Commands
########################################################################################################################

def get_item_label(item: int) -> Union[str, None]:
    """Gets the item's label.

    Returns:
        label as a string or None
    """
    return internal_dpg.get_item_configuration(item)["label"]


def get_item_filter_key(item: int) -> Union[str, None]:
    """Gets the item's filter key.

    Returns:
        filter key as a string or None
    """
    return internal_dpg.get_item_configuration(item)["filter_key"]


def is_item_tracked(item: int) -> Union[bool, None]:
    """Checks if item is tracked.

    Returns:
        tracked as a bool or None
    """
    return internal_dpg.get_item_configuration(item)["tracked"]


def is_item_search_delayed(item: int) -> Union[bool, None]:
    """Checks if item is search delayed.

    Returns:
        tracked as a bool or None
    """
    return internal_dpg.get_item_configuration(item)["delay_search"]


def get_item_indent(item: int) -> Union[int, None]:
    """Gets the item's indent.

    Returns:
        indent as a int or None
    """
    return internal_dpg.get_item_configuration(item)["indent"]


def get_item_track_offset(item: int) -> Union[float, None]:
    """Gets the item's track offset.

    Returns:
        track offset as a int or None
    """
    return internal_dpg.get_item_configuration(item)["track_offset"]


def get_item_width(item: int) -> Union[int, None]:
    """Gets the item's width.

    Returns:
        width as a int or None
    """
    return internal_dpg.get_item_configuration(item)["width"]


def get_item_height(item: int) -> Union[int, None]:
    """Gets the item's height.

    Returns:
        height as a int or None
    """
    return internal_dpg.get_item_configuration(item)["height"]


def get_item_callback(item: int) -> Union[str, None]:
    """Gets the item's callback.

    Returns:
        callback as a string or None
    """
    return internal_dpg.get_item_configuration(item)["callback"]


def get_item_drag_callback(item: int) -> Union[str, None]:
    """Gets the item's drag callback.

    Returns:
        callback as a string or None
    """
    return internal_dpg.get_item_configuration(item)["drag_callback"]


def get_item_drop_callback(item: int) -> Union[str, None]:
    """Gets the item's drop callback.

    Returns:
        callback as a string or None
    """
    return internal_dpg.get_item_configuration(item)["drop_callback"]


def get_item_user_data(item: int) -> Union[Any, None]:
    """Gets the item's callback data.

    Returns:
        callback data as a python object or None
    """
    return internal_dpg.get_item_configuration(item)["user_data"]


def get_item_source(item: int) -> Union[str, None]:
    """Gets the item's source.

    Returns:
        source as a string or None
    """
    return internal_dpg.get_item_configuration(item)["source"]


########################################################################################################################
# State Commands
########################################################################################################################

def is_item_hovered(item: int) -> Union[bool, None]:
    """Checks if item is hovered.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["hovered"]


def is_item_active(item: int) -> Union[bool, None]:
    """Checks if item is active.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["active"]


def is_item_focused(item: int) -> Union[bool, None]:
    """Checks if item is focused.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["focused"]


def is_item_clicked(item: int) -> Union[bool, None]:
    """Checks if item is clicked.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["clicked"]


def is_item_visible(item: int) -> Union[bool, None]:
    """Checks if item is visible.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["visible"]


def is_item_edited(item: int) -> Union[bool, None]:
    """Checks if item is edited.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["edited"]


def is_item_activated(item: int) -> Union[bool, None]:
    """Checks if item is activated.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["activated"]


def is_item_deactivated(item: int) -> Union[bool, None]:
    """Checks if item is deactivated.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["deactivated"]


def is_item_deactivated_after_edit(item: int) -> Union[bool, None]:
    """Checks if item is deactivated_after_edit.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["deactivated_after_edit"]


def is_item_toggled_open(item: int) -> Union[bool, None]:
    """Checks if item is toggled_open.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["toggled_open"]


def is_item_ok(item: int) -> Union[bool, None]:
    """Checks if item is ok and can be used.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["ok"]


def is_item_shown(item: int) -> Union[bool, None]:
    """Checks if item is shown.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_configuration(item)["show"]


def is_item_enabled(item: int) -> Union[bool, None]:
    """Checks if item is enabled.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_configuration(item)["enabled"]


def get_item_pos(item: int) -> List[int]:
    """Returns item's position.

    Returns:
        position
    """
    return internal_dpg.get_item_state(item)["pos"]


def get_available_content_region(item: int) -> List[int]:
    """Returns item's available content region.

    Returns:
        position
    """
    return internal_dpg.get_item_state(item)["content_region_avail"]


def get_item_rect_size(item: int) -> List[int]:
    """Returns item's available content region.

    Returns:
        position
    """
    return internal_dpg.get_item_state(item)["rect_size"]


def get_item_rect_min(item: int) -> List[int]:
    """Returns item's minimum content region.

    Returns:
        position
    """
    return internal_dpg.get_item_state(item)["rect_min"]


def get_item_rect_max(item: int) -> List[int]:
    """Returns item's maximum content region.

    Returns:
        position
    """
    return internal_dpg.get_item_state(item)["rect_max"]


########################################################################################################################
# Viewport Setter Commands
########################################################################################################################

def set_viewport_clear_color(color: List[int]):
    """Sets the viewport's clear color.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, clear_color=color)


def set_viewport_small_icon(icon: str):
    """Sets the viewport's small icon. Must be ico for windows.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, small_icon=icon)


def set_viewport_large_icon(icon: str):
    """Sets the viewport's small icon. Must be ico for windows.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, large_icon=icon)


def set_viewport_pos(pos: List[float]):
    """Sets the viewport's position.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, pos=pos)


def set_viewport_width(width: int):
    """Sets the viewport's width.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, width=width)


def set_viewport_height(height: int):
    """Sets the viewport's height.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, height=height)


def set_viewport_min_width(width: int):
    """Sets the viewport's minimum width.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, min_width=width)


def set_viewport_max_width(width: int):
    """Sets the viewport's max width.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, max_width=width)


def set_viewport_min_height(height: int):
    """Sets the viewport's minimum height.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, min_height=height)


def set_viewport_max_height(height: int):
    """Sets the viewport's max width.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, max_height=height)


def set_viewport_title(title: str):
    """Sets the viewport's title.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, title=title)


def set_viewport_always_top(value: bool):
    """Sets the viewport always on top.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, always_on_top=value)


def set_viewport_resizable(value: bool):
    """Sets the viewport resizable.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, resizable=value)

def set_viewport_vsync(value: bool):
    """Sets the viewport vsync.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, vsync=value)


def set_viewport_border(value: bool):
    """Sets the viewport border.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, border=value)


def set_viewport_caption(value: bool):
    """Sets the viewport caption.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, caption=value)


def set_viewport_overlapped(value: bool):
    """Sets the viewport overlapped.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, overlapped=value)


def set_viewport_maximized_box(value: bool):
    """Sets the viewport maximized box.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, maximized_box=value)


def set_viewport_minimized_box(value: bool):
    """Sets the viewport minimized box.

    Returns:
        None
    """
    internal_dpg.configure_viewport(0, minimized_box=value)


########################################################################################################################
# Viewport Getter Commands
########################################################################################################################

def get_viewport_clear_color() ->List[int]:
    """Gets the viewport's clear color.

    Returns:
        List[int]
    """
    return internal_dpg.internal_dpg.get_viewport_configuration()["clear_color"]


def get_viewport_pos() ->List[float]:
    """Gets the viewport's position.

    Returns:
        viewport position.
    """
    config = internal_dpg.get_viewport_configuration()
    x_pos = config["x_pos"]
    y_pos = config["y_pos"]
    return [x_pos, y_pos]


def get_viewport_width() -> int:
    """Gets the viewport's width.

    Returns:
        viewport width
    """
    return internal_dpg.get_viewport_configuration()["width"]


def get_viewport_client_width() -> int:
    """Gets the viewport's client width.

    Returns:
        viewport width
    """
    return internal_dpg.get_viewport_configuration()["client_width"]


def get_viewport_client_height() -> int:
    """Gets the viewport's client height.

    Returns:
        viewport width
    """
    return internal_dpg.get_viewport_configuration()["client_height"]


def get_viewport_height() -> int:
    """Gets the viewport's height.

    Returns:
        int
    """
    return internal_dpg.get_viewport_configuration()["height"]


def get_viewport_min_width() -> int:
    """Gets the viewport's minimum width.

    Returns:
        int
    """
    return internal_dpg.get_viewport_configuration()["min_width"]


def get_viewport_max_width() -> int:
    """Gets the viewport's max width.

    Returns:
        int
    """
    return internal_dpg.get_viewport_configuration()["max_width"]


def get_viewport_min_height() -> int:
    """Gets the viewport's minimum height.

    Returns:
        int
    """
    return internal_dpg.get_viewport_configuration()["min_height"]


def get_viewport_max_height() -> int:
    """Gets the viewport's max width.

    Returns:
        int
    """
    return internal_dpg.get_viewport_configuration()["max_height"]


def get_viewport_title() -> str:
    """Gets the viewport's title.

    Returns:
        str
    """
    return internal_dpg.get_viewport_configuration()["title"]


def is_viewport_always_top() -> bool:
    """Checks the viewport always on top flag.

    Returns:
        bool
    """
    return internal_dpg.get_viewport_configuration()["always_on_top"]


def is_viewport_resizable() -> bool:
    """Checks the viewport resizable flag.

    Returns:
        bool
    """
    return internal_dpg.get_viewport_configuration()["resizable"]

def is_viewport_vsync_on() -> bool:
    """Checks the viewport vsync flag.

    Returns:
        bool
    """
    return internal_dpg.get_viewport_configuration()["vsync"]


def get_viewport_border() -> bool:
    """Checks the viewport border flag.

    Returns:
        bool
    """
    return internal_dpg.get_viewport_configuration()["border"]


def is_viewport_caption_on() -> bool:
    """Checks the viewport caption flag.

    Returns:
        bool
    """
    return internal_dpg.get_viewport_configuration()["caption"]


def is_viewport_overlapped_on() -> bool:
    """Checks the viewport overlapped flag.

    Returns:
        bool
    """
    return internal_dpg.get_viewport_configuration()["overlapped"]


def is_viewport_maximized_box_on() -> bool:
    """Checks the viewport maximized box flag.

    Returns:
        bool
    """
    return internal_dpg.get_viewport_configuration()["maximized_box"]


def is_viewport_minimized_box_on() -> bool:
    """Checks the viewport minimized box flag.

    Returns:
        bool
    """
    return internal_dpg.get_viewport_configuration()["minimized_box"]
