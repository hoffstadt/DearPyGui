from contextlib import contextmanager
from typing import List, Any, Callable, Union
import dearpygui.core as internal_dpg

########################################################################################################################
# context manager container wrappers
########################################################################################################################

@contextmanager
def mutex():
   
   try:
        yield internal_dpg.lock_mutex()
   finally:
        internal_dpg.unlock_mutex()

@contextmanager
def theme(*args, id:int=0, default_theme: bool = False, label:str=None):
   
   try:
        widget = internal_dpg.add_theme(*args, id=id, default_theme=default_theme, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget
   finally:
        internal_dpg.pop_container_stack()

@contextmanager
def font_registry(*args, id:int=0, show: bool =True, label:str=None):
   
   try:
        widget = internal_dpg.add_font_registry(*args, id=id, show=show, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget
   finally:
        internal_dpg.pop_container_stack()

@contextmanager
def clipper(*args, id:int=0, show: bool =True, parent: int=0, width: int = 0, before: int=0,
           indent:int=-1, delay_search: bool = False, label:str=None):
   
   try:
        widget = internal_dpg.add_clipper(*args, id=id, show=show, parent=parent, width=width, before=before,
                                         indent=indent, delay_search=delay_search, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget
   finally:
        internal_dpg.pop_container_stack()

@contextmanager
def filter_set(*args, id:int=0, show: bool =True, parent: int=0, width: int = 0, before: int=0,
           indent:int=-1, delay_search: bool = False, label:str=None):
   
   try:
        widget = internal_dpg.add_filter_set(*args, id=id, show=show, parent=parent, width=width, before=before,
                                         indent=indent, delay_search=delay_search, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget
   finally:
        internal_dpg.pop_container_stack()

@contextmanager
def drag_payload(*args, id:int=0, show: bool =True, parent: int=0, payload_type: str='$$DPG_PAYLOAD', drag_data: Any = None, label:str=None):
   
   try:
        widget = internal_dpg.add_drag_payload(*args, id=id, show=show, parent=parent, payload_type=payload_type,
                                               drag_data=drag_data, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget
   finally:
        internal_dpg.pop_container_stack()

@contextmanager
def handler_registry(*args, id:int=0, show: bool =True, label:str=None):
   
   try:
        widget = internal_dpg.add_handler_registry(*args, id=id, show=show, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget
   finally:
        internal_dpg.pop_container_stack()

@contextmanager
def file_dialog(*args, id:int=0, width: int =0, height: int =0, label: str ='', callback: Callable =None, show: bool =True, 
					default_path: str ='', default_filename: str ='.', file_count: int =0, modal: bool =False, directory_selector: bool =False):
   
   try:
        widget = internal_dpg.add_file_dialog(*args, id=id, width=width, height=height, label=label, callback=callback, show=show,
                                              default_path=default_path, default_filename=default_filename, file_count=file_count,
                                              modal=modal, directory_selector=directory_selector)
        internal_dpg.push_container_stack(widget)
        yield widget
   finally:
        internal_dpg.pop_container_stack()


@contextmanager
def table(*args, header_row: bool = True, width: int = 0, height: int = 0, inner_width: int = 0, show: bool = True, parent: int = 0,
		before: int = 0, resizable: bool = False, reorderable: bool = False, hideable: bool = False, sortable: bool = False,   
		context_menu_in_body: bool = False, row_background: bool = False, borders_innerH: bool = False, borders_outerH: bool = False,
		borders_innerV: bool = False, borders_outerV: bool = False, policy: int = 0, no_host_extendX: bool = False,
		no_host_extendY: bool = False, no_keep_columns_visible: bool = False, precise_widths: bool = False, no_clip: bool = False,
		pad_outerX: bool = False, no_pad_outerX: bool = False, no_pad_innerX: bool = False, scrollX: bool = False, scrollY: bool = False,
        id:int=0, indent=-1, callback: Callable = None, sort_multi: bool = False, sort_tristate: bool = False, pos=[], freeze_rows:int = 0,
        freeze_columns:int = 0, delay_search: bool = False, label:str=None):

    try:
        widget = internal_dpg.add_table(*args, header_row=header_row, width = width, height = height, inner_width = inner_width,
		    show = show, parent = parent, before = before, resizable = resizable, reorderable = reorderable, hideable = hideable,
		    sortable = sortable, context_menu_in_body = context_menu_in_body, row_background = row_background,
		    borders_innerH = borders_innerH, borders_outerH = borders_outerH, borders_innerV = borders_innerV,
		    borders_outerV = borders_outerV, policy = policy, no_host_extendX = no_host_extendX,
		    no_host_extendY = no_host_extendY, no_keep_columns_visible = no_keep_columns_visible, precise_widths = precise_widths,
		    no_clip = no_clip, pad_outerX = pad_outerX, no_pad_outerX = no_pad_outerX, no_pad_innerX = no_pad_innerX,
		    scrollX = scrollX, scrollY = scrollY, id=id, indent=indent, callback=callback, sort_multi=sort_multi,
            sort_tristate=sort_tristate, pos=pos, freeze_rows=freeze_rows, freeze_columns=freeze_columns, delay_search=delay_search, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def drawlist(*args, id:int=0, width: int = 0, height: int = 0, show: bool = True, parent: int = 0, before: int = 0,
             callback: Callable = None, user_data: Any = None, pos=[], delay_search: bool = False, label:str=None):
    try:
        widget = internal_dpg.add_drawlist(*args, id=id, width = width, height = height, show=show, parent=parent, before=before,
                                           callback=callback, user_data=user_data, pos=pos, delay_search=delay_search, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget

    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def viewport_drawlist(*args, id:int=0, front: bool = True, show: bool = True, delay_search: bool = False, label:str=None):
    try:
        widget = internal_dpg.add_viewport_drawlist(*args, id=id, show=show, front=fronts, delay_search=delay_search, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget

    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def table_row(*args, id:int=0, show: bool = True, parent: int = 0, before: int = 0, height: int = 0, label:str=None):
    try:
        widget = internal_dpg.add_table_row(*args, id=id, show=show, parent=parent, before=before, height=height, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget

    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def draw_layer(*args, id:int=0, show: bool = True, parent: int = 0, before: int = 0, label:str=None):
    try:
        widget = internal_dpg.add_draw_layer(*args, id=id, show=show, parent=parent, before=before, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget

    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def window(*args, label: str = None, id: int=0, width: int = 200, height: int = 200, indent: int = -1, 
           user_data: Any = None, show: bool = True, delay_search: str = False, min_size: List[int] = [32, 32], 
           max_size: List[int] = [30000, 30000], menubar: bool = False, collapsed: bool = False, 
           autosize: bool = False, no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, 
           no_scrollbar: bool = False, no_collapse: bool = False, horizontal_scrollbar: bool = False, 
           no_focus_on_appearing: bool = False, no_bring_to_front_on_focus: bool = False, no_close: bool = False, 
           no_background: bool = False, modal: bool = False, popup: bool = False, on_close: Callable = None) -> int:
    """Wraps add_window() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **width: Width of the item.
        **height: Height of the item.
        **user_data: User data for callbacks.
        **show: Attempt to render widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **min_size: Minimum window size.
        **max_size: Maximum window size.
        **menubar: Decides if the menubar is shown or not.
        **collapsed: Collapse the window.
        **autosize: Autosized the window to fit it's items.
        **no_resize: Allows for the window size to be changed or fixed.
        **no_title_bar: Title name for the title bar of the window.
        **no_move: Allows for the window's position to be changed or fixed.
        **no_scrollbar: Disable scrollbars. (window can still scroll with mouse or programmatically)
        **no_collapse: Disable user collapsing window by double-clicking on it.
        **horizontal_scrollbar: Allow horizontal scrollbar to appear. (off by default)
        **no_focus_on_appearing: Disable taking focus when transitioning from hidden to visible state.
        **no_bring_to_front_on_focus: Disable bringing window to front when taking focus. (e.g. clicking on it or
            programmatically giving it focus)
        **no_close: Disable user closing the window by removing the close button.
        **no_background: Sets Background and border alpha to transparent.
        **modal: Fills area behind window according to the theme and disables user ability to interact with 
            anything except the window.
        **popup: Fills area behind window according to the theme, removes title bar, collapse and close. Window can 
            be closed by selecting area in the background behind the window.
        **on_close: Callback ran when window is closed.

    Returns:
        UUID as int
    """
    try:

        widget = internal_dpg.add_window(*args, width=width, height=height, autosize=autosize,
                                        no_resize=no_resize, no_title_bar=no_title_bar, no_move=no_move,
                                        no_scrollbar=no_scrollbar, no_collapse=no_collapse,
                                        horizontal_scrollbar=horizontal_scrollbar,
                                        no_focus_on_appearing=no_focus_on_appearing,
                                        no_bring_to_front_on_focus=no_bring_to_front_on_focus,
                                        menubar=menubar, no_close=no_close, user_data=user_data,
                                        no_background=no_background, label=label, show=show, 
                                        collapsed=collapsed, on_close=on_close,
                                        min_size=min_size, max_size=max_size, id=id, modal=modal,
                                        popup=popup, delay_search=delay_search)
        internal_dpg.push_container_stack(widget)
        yield widget

    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def menu_bar(*args, label: str=None, id: int=0, indent: int = -1, parent: int = 0, show: bool = True, delay_search: bool = False) -> int:
    """Wraps add_menu_bar() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **show: Attempt to render widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
    Returns:
        UUID as int
    """
    try:
        widget = internal_dpg.add_menu_bar(*args, show=show, parent=parent, id=id, indent=indent, delay_search=delay_search, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def menu(*args, label: str = None, id: int=0, indent: int = -1, parent: int = 0, before: int = 0,
        payload_type: str = '$$DPG_PAYLOAD', drag_callback: Callable = None, drop_callback: Callable = None, show: bool = True, 
        enabled: bool = True, filter_key: str = '', delay_search: bool = False, tracked: bool = False, track_offset: float = 0.5) -> int:
    """Wraps add_menu() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **show: Attempt to render widget.
        **enabled: Turns off functionality of widget and applies the disabled theme.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.

    Returns:
        UUID as int
    """
    try: 
        widget = internal_dpg.add_menu(*args, label=label, id=id, indent=indent, parent=parent, before=before, 
                                       payload_type=payload_type, drag_callback=drag_callback, drop_callback=drop_callback, 
                                       show=show, enabled=enabled, filter_key=filter_key, delay_search=delay_search,
                                       tracked=tracked, track_offset=track_offset)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def child(*args, label: str = None, id: int=0, width: int = 0, height: int = 0, indent: int = -1, parent: int = 0, before: int = 0,
          payload_type: str = '$$DPG_PAYLOAD', drag_callback: Callable = None, drop_callback: Callable = None, show: bool = True, pos: List[int] = [],
          filter_key: str = '', delay_search: bool = False, tracked: bool = False, track_offset: float = 0.5, border: bool = True, 
          autosize_x: bool = False, autosize_y: bool = False, no_scrollbar: bool = False, horizontal_scrollbar: bool = False, 
          menubar: bool = False, ) -> int:
    """Wraps add_child() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **width: Width of the item.
        **height: Height of the item.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **show: Attempt to render widget.
        **pos: Places the item relative to window coordinates, [0,0] is top left.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **border: Shows/Hides the border around the sides.
        **autosize_x: Autosize the window to fit its items in the x.
        **autosize_y: Autosize the window to fit its items in the y.
        **no_scrollbar: Disable scrollbars (window can still scroll with mouse or programmatically)
        **horizontal_scrollbar: Allow horizontal scrollbar to appear (off by default)
        **menubar: Shows/Hides the menubar at the top.

    Returns:
        UUID as int
    """

    try: 
        widget = internal_dpg.add_child(*args, label=label, id=id, width=width, height=height, indent=indent, 
                                        parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, 
                                        drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search,
                                        tracked=tracked, track_offset=track_offset, border=border, autosize_x=autosize_x, 
                                        autosize_y=autosize_y, no_scrollbar=no_scrollbar, horizontal_scrollbar=horizontal_scrollbar,
                                        menubar=menubar)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def collapsing_header(*args, label: str = None, id: int=0, indent: int = -1, parent: int = 0, before: int = 0,
                      payload_type: str = '$$DPG_PAYLOAD', drag_callback: Callable = None, drop_callback: Callable = None, show: bool = True, pos: List[int] = [],
                      filter_key: str = '', delay_search: bool = False, tracked: bool = False, track_offset: float = 0.5,
                      closable: bool = False, default_open: bool = False, open_on_double_click: bool = False, open_on_arrow: bool = False, 
                      leaf: bool = False, bullet: bool = False, ) -> int:
    """Wraps add_collapsing_header() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **show: Attempt to render widget.
        **pos: Places the item relative to window coordinates, [0,0] is top left.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **closable: Decides if the header can be collapsed.
        **default_open: Decides if item is open by default.
        **open_on_double_click: Need double-click to open node.
        **open_on_arrow: Only open when clicking on the arrow part.
        **leaf: No collapsing, no arrow (use as a convenience for leaf nodes).
        **bullet: Display a bullet instead of arrow.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_collapsing_header(*args, label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, 
                                                    drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, 
                                                    filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, 
                                                    closable=closable, default_open=default_open, open_on_double_click=open_on_double_click,
                                                    open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def group(*args, label: str = None, id: int=0, width: int = 0, indent: int = -1, parent: int = 0, before: int = 0,
          payload_type: str = '$$DPG_PAYLOAD', drag_callback: Callable = None, drop_callback: Callable = None, show: bool = True, pos: List[int] = [],
          filter_key: str = '', delay_search: bool = False, tracked: bool = False, track_offset: float = 0.5,
          horizontal: bool = False, horizontal_spacing: float = -1.0) -> int:
    """Wraps add_group() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **width: Width of the item.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **show: Attempt to render widget.
        **pos: Places the item relative to window coordinates, [0,0] is top left.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **horizontal: Adds the items on the same row by default.
        **horizontal_spacing: Decides the spacing for the items.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_group(*args, label=label, id=id, width=width, indent=indent, 
                                        parent=parent, before=before, payload_type=payload_type, drag_callback=drag_callback, 
                                        drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search,
                                        tracked=tracked, track_offset=track_offset, 
                                        horizontal=horizontal, horizontal_spacing=horizontal_spacing)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def node(*args, label: str = None, id: int=0, parent: int = 0, before: int = 0,
         payload_type: str = '$$DPG_PAYLOAD', drag_callback: Callable = None, drop_callback: Callable = None, show: bool = True, pos: List[int] = [],
         filter_key: str = '', delay_search: bool = False, tracked: bool = False, track_offset: float = 0.5,
         draggable: bool = True) -> int:
    """Wraps add_node() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **show: Attempt to render widget.
        **pos: Places the item relative to window coordinates, [0,0] is top left.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **draggable: Allow node to be draggable.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_node(*args, label=label, id=id, parent=parent, before=before, payload_type=payload_type, 
                                       drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, 
                                       delay_search=delay_search, tracked=tracked, track_offset=track_offset,
                                       draggable=draggable)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def node_attribute(*args, label: str = None, id: int=0, indent: int = -1, parent: int = 0, before: int = 0,
                   payload_type: str = '$$DPG_PAYLOAD', drag_callback: Callable = None, drop_callback: Callable = None, show: bool = True,
                   filter_key: str = '', tracked: bool = False, track_offset: float = 0.5, 
                   output: bool = False, static: bool = False, shape: int = 0,) -> int:
    """Wraps add_node_attribute() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **show: Attempt to render widget.
        **filter_key: Used by filter widget.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **output: Set as output attribute.
        **static: Set as static attribute.
        **shape : Pin shape.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_node_attribute(*args, label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, 
                                       drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, 
                                       tracked=tracked, track_offset=track_offset, 
                                       output=output, static=static, shape=shape)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def node_editor(*args, label: str = None, id: int=0, parent: int = 0, before: int = 0, payload_type: str = '$$DPG_PAYLOAD', 
                callback: Callable = None, drag_callback: Callable = None, drop_callback: Callable = None, show: bool = True,
                filter_key: str = '', delay_search: bool = False, tracked: bool = False, track_offset: float = 0.5, 
                delink_callback: Callable = None) -> int:
    """Wraps add_node_editor() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **callback: Registers a callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **show: Attempt to render widget.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **delink_callback: Callback ran when a link is detached.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_node_editor(*args, label=label, id=id, parent=parent, before=before, payload_type=payload_type, callback=callback,
                                              drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, 
                                              delay_search=delay_search, tracked=tracked, track_offset=track_offset,
                                              delink_callback=delink_callback)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def staging_container(*args, id:int=0, label:str=None):

    try:
        widget = internal_dpg.add_staging_container(id=id)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def tab_bar(*args, label: str = None, id: int=0, indent: int = -1, parent: int = 0, before: int = 0, payload_type: str = '$$DPG_PAYLOAD', 
            callback: Callable = None, drag_callback: Callable = None, drop_callback: Callable = None, user_data: Any = None, show: bool = True,
            pos: List[int] = [], filter_key: str = '', delay_search: bool = False, tracked: bool = False, track_offset: float = 0.5, 
            reorderable: bool = False) -> int:
    """Wraps add_tab_bar() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **callback: Registers a callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **user_data: User data for callbacks.
        **show: Attempt to render widget.
        **pos: Places the item relative to window coordinates, [0,0] is top left.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **reorderable: Allows for moveable tabs.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_tab_bar(*args, label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type, callback=callback,
                                              drag_callback=drag_callback, drop_callback=drop_callback, user_data=user_data, show=show, pos=pos, 
                                              filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset, 
                                              reorderable=reorderable)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def tab(*args, label: str = None, id: int=0, indent: int = -1, parent: int = 0, before: int = 0, payload_type: str = '$$DPG_PAYLOAD', 
        drag_callback: Callable = None, drop_callback: Callable = None, show: bool = True, filter_key: str = '', delay_search: bool = False, 
        tracked: bool = False, track_offset: float = 0.5, 
        closable: bool = False, no_reorder: bool = False, leading: bool = False, trailing: bool = False, no_tooltip: bool = False) -> int:
    """Wraps add_tab() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **show: Attempt to render widget.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **closable: Creates a button on the tab that can hide the tab.
        **no_reorder: Disable reordering this tab or having another tab cross over this tab.
        **leading: Enforce the tab position to the left of the tab bar (after the tab list popup button).
        **trailing: Enforce the tab position to the right of the tab bar (before the scrolling buttons).
        **no_tooltip: Disable tooltip for the given tab.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_tab(*args, label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type,
                                      drag_callback=drag_callback, drop_callback=drop_callback, show=show, filter_key=filter_key, 
                                      delay_search=delay_search, tracked=tracked, track_offset=track_offset, 
                                      closable=closable, no_reorder=no_reorder, leading=leading, trailing=trailing, no_tooltip=no_tooltip)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def tree_node(*args, label: str = None, id: int=0, indent: int = -1, parent: int = 0, before: int = 0, payload_type: str = '$$DPG_PAYLOAD', 
              drag_callback: Callable = None, drop_callback: Callable = None, show: bool = True, pos: List[int] = [], filter_key: str = '', 
              delay_search: bool = False, tracked: bool = False, track_offset: float = 0.5, 
              default_open: bool = False, open_on_double_click: bool = False, open_on_arrow: bool = False, leaf: bool = False, 
              bullet: bool = False, selectable: bool = False) -> int:
    """Wraps add_tree_node() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **show: Attempt to render widget.
        **pos: Places the item relative to window coordinates, [0,0] is top left.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **default_open: Decides if item is open by default.
        **open_on_double_click: Need double-click to open node.
        **open_on_arrow: Only open when clicking on the arrow part.
        **leaf: No collapsing, no arrow (use as a convenience for leaf nodes).
        **bullet: Display a bullet instead of arrow.
        **selectable: Display a selectable instead of arrow.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_tree_node(*args, label=label, id=id, indent=indent, parent=parent, before=before, payload_type=payload_type,
                                            drag_callback=drag_callback, drop_callback=drop_callback, show=show, pos=pos, filter_key=filter_key, 
                                            delay_search=delay_search, tracked=tracked, track_offset=track_offset, 
                                            default_open=default_open, open_on_double_click=open_on_double_click, 
                                            open_on_arrow=open_on_arrow, leaf=leaf, bullet=bullet, selectable=selectable)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def tooltip(*args, label: str=None, id: int=0, show: bool = True, ) -> int:
    """Wraps add_menu_bar() and automates calling end().

    Args:
        parent: widget that will show the tooltip when hovered.
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **show: Attempt to render widget.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_tooltip(*args, label=label, id=id, show=show)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def texture_container(*args, show: bool = False, id:int=0, label:str=None):

    try:
        widget = internal_dpg.add_texture_container(*args, show=show, id=id, label=label)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()


@contextmanager
def popup(*args, label: str = None, id: int=0, width: int = 200, height: int = 200, parent: int = 0,  show: bool = True, pos: List[int] = [],
         mousebutton: int = 1, modal: bool = False) -> int:
    """Wraps add_group() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **width: Width of the item.
        **height: Height of the item.
        **parent: Parent this item will be added to. (runtime adding)
        **show: Attempt to render widget.
        **pos: Places the item relative to window coordinates, [0,0] is top left.
        **mousebutton: The mouse code that will trigger the popup.
        **modal: Fills area behind window according to the theme and disables user ability to interact with 
            anything except the window.

    Returns:
        UUID as int
    """

    try:
        widget = internal_dpg.add_popup(*args, label=label, id=id, width=width, height=height, parent=parent, show=show,
                                     mousebutton=mousebutton, modal=modal)
        internal_dpg.push_container_stack(widget)
        yield widget
    finally:
        internal_dpg.pop_container_stack()

@contextmanager
def plot(*args, label: str = None, id: int=0,  width: int = -1, height: int = 400, indent: int = -1, parent: int = 0, before: int = 0, 
         payload_type: str = '$$DPG_PAYLOAD', callback: Callable = None, drag_callback: Callable = None, drop_callback: Callable = None, 
         user_data: Any = None, show: bool = True, pos: List[int] = [], filter_key: str = '', delay_search: bool = False, tracked: bool = False, 
         track_offset: float = 0.5,
         no_title: bool = False, no_menus: bool = False, no_box_select: bool = False, no_mouse_pos: bool = False,
         no_highlight: bool = False, no_child: bool = False, query: bool = False, crosshairs: bool = False,
         anti_aliased: bool = False, equal_aspects: bool = False) -> int:
    """Wraps add_window() and automates calling end().

    Args:
        **label: Overrides 'id' as label.
        **id: Unique id used to programmatically refer to the item. If label is unused this will be the label.
        **width: Width of the item.
        **height: Height of the item.
        **indent: Offsets the widget to the right the specified number multiplied by the indent style.
        **parent: Parent this item will be added to. (runtime adding)
        **before: Parent this item will be added to. (runtime adding)
        **payload_type: Sender string type must be the same as the target for the target to run the payload_callback.
        **callback: Registers a callback.
        **drag_callback: Registers a drag callback for drag and drop.
        **drop_callback: Registers a drop callback for drag and drop.
        **user_data: User data for callbacks.
        **show: Attempt to render widget.
        **pos: Places the item relative to window coordinates, [0,0] is top left.
        **filter_key: Used by filter widget.
        **delay_search: Delays searching container for specified items until the end of the app. 
            Possible optimization when a container has many children that are not accessed often.
        **tracked: Scroll tracking.
        **track_offset : 0.0f:top, 0.5f:center, 1.0f:bottom.
        **no_title : ...
        **no_menus : ...
        **no_box_select : ...
        **no_mouse_pos : ...
        **no_highlight : ...
        **no_child : ...
        **query : ...
        **crosshairs : ...
        **anti_aliased : ...
        **equal_aspects : ...

    Returns:
        UUID as int
    """

    try:

        widget = internal_dpg.add_plot(*args, label=label, id=id, width=width, height=height, indent=indent, parent=parent, before=before, 
                                       payload_type=payload_type, callback=callback, drag_callback=drag_callback, drop_callback=drop_callback,
                                       user_data=user_data, show=show, pos=pos, filter_key=filter_key, delay_search=delay_search, tracked=tracked, track_offset=track_offset,
                                       no_title=no_title, no_menus=no_menus, no_box_select=no_box_select, no_mouse_pos=no_mouse_pos,
                                       no_highlight=no_highlight, no_child=no_child, query=query, crosshairs=crosshairs,
                                       anti_aliased=anti_aliased, equal_aspects=equal_aspects)
        internal_dpg.push_container_stack(widget)
        yield widget

    finally:
        internal_dpg.pop_container_stack()
