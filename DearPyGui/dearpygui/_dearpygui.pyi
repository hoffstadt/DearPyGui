from typing import List, Any, Callable, Union, Tuple
from dearpygui._dearpygui import *

##########################################################
# This file is generated automatically by mvPythonParser #
##########################################################

# ~ Dear PyGui Version: master
def add_2d_histogram_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, xbins: int =-1, ybins: int =-1, xmin_range: float =0.0, xmax_range: float =1.0, ymin_range: float =0.0, ymax_range: float =1.0, density: bool =False, outliers: bool =True) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_3d_slider(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: Union[List[float], Tuple[float]] =(0.0, 0.0, 0.0, 0.0), max_x: float =100.0, max_y: float =100.0, max_z: float =100.0, min_x: float =0.0, min_y: float =0.0, min_z: float =0.0, scale: float =1.0) -> Union[int, str]:
	"""Adds a 3D box slider that allows a 3d point to be show in 2d represented cube space."""
	...

def add_activated_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is activated."""
	...

def add_active_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is active."""
	...

def add_alias(alias : str, item : Union[int, str]) -> None:
	"""Undocumented"""
	...

def add_area_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, fill: Union[List[int], Tuple[int]] =(0, 0, 0, -255), contribute_to_bounds: bool =True) -> Union[int, str]:
	"""Adds an area series to a plot."""
	...

def add_bar_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, weight: float =1.0, horizontal: bool =False) -> Union[int, str]:
	"""Adds a bar series to a plot."""
	...

def add_bool_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: bool =False, parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_button(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, small: bool =False, arrow: bool =False, direction: int =0) -> Union[int, str]:
	"""Adds a button."""
	...

def add_candle_series(dates : Union[List[float], Tuple[float]], opens : Union[List[float], Tuple[float]], closes : Union[List[float], Tuple[float]], lows : Union[List[float], Tuple[float]], highs : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, bull_color: Union[List[int], Tuple[int]] =(0, 255, 113, 255), bear_color: Union[List[int], Tuple[int]] =(218, 13, 79, 255), weight: int =0.25, tooltip: bool =True) -> Union[int, str]:
	"""Adds a candle series to a plot."""
	...

def add_char_remap(source : int, target : int, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_checkbox(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: bool =False) -> Union[int, str]:
	"""Adds a checkbox."""
	...

def add_child(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, border: bool =True, autosize_x: bool =False, autosize_y: bool =False, no_scrollbar: bool =False, horizontal_scrollbar: bool =False, menubar: bool =False) -> Union[int, str]:
	"""Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end."""
	...

def add_clicked_handler(parent : Union[int, str], button : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is clicked."""
	...

def add_clipper(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, delay_search: bool =False) -> Union[int, str]:
	"""Helper to manually clip large list of items. Increases performance by not searching or drawing widgets outside of the clipped region."""
	...

def add_collapsing_header(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, closable: bool =False, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False) -> Union[int, str]:
	"""Adds a collapsing header to add items to. Must be closed with the end command."""
	...

def add_color_button(default_value : Union[List[int], Tuple[int]] =(0, 0, 0, 255), *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_border: bool =False, no_drag_drop: bool =False) -> Union[int, str]:
	"""Adds a color button."""
	...

def add_color_edit(default_value : Union[List[int], Tuple[int]] =(0, 0, 0, 255), *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_picker: bool =False, no_options: bool =False, no_small_preview: bool =False, no_inputs: bool =False, no_tooltip: bool =False, no_label: bool =False, no_drag_drop: bool =False, alpha_bar: bool =False, alpha_preview: int =0, display_mode: int =1048576, display_type: int =8388608, input_mode: int =134217728) -> Union[int, str]:
	"""Adds an RGBA color editor. Click the small color preview will provide a color picker. Click and draging the small color preview will copy the color to be applied on any other color widget."""
	...

def add_color_picker(default_value : Union[List[int], Tuple[int]] =(0, 0, 0, 255), *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_side_preview: bool =False, no_small_preview: bool =False, no_inputs: bool =False, no_tooltip: bool =False, no_label: bool =False, alpha_bar: bool =False, display_rgb: bool =False, display_hsv: bool =False, display_hex: bool =False, picker_mode: int =33554432, alpha_preview: int =0, display_type: int =8388608, input_mode: int =134217728) -> Union[int, str]:
	"""Adds an RGB color picker. Right click the color picker for options. Click and drag the color preview to copy the color and drop on any other color widget to apply. Right Click allows the style of the color picker to be changed."""
	...

def add_color_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: Union[List[float], Tuple[float]] =(0.0, 0.0, 0.0, 0.0), parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_colormap(colors : List[List[int]], qualitative : bool, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_4) -> Union[int, str]:
	"""Adds a legend that pairs values with colors. This is typically used with a heat series. """
	...

def add_colormap_button(default_value : Union[List[int], Tuple[int]] =(0, 0, 0, 255), *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_border: bool =False, no_drag_drop: bool =False) -> Union[int, str]:
	"""Adds a color button."""
	...

def add_colormap_registry(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, show: bool =False) -> Union[int, str]:
	"""Adds a colormap registry."""
	...

def add_colormap_scale(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, pos: Union[List[int], Tuple[int]] =[], colormap: Union[int, str] =0, min_scale: float =0.0, max_scale: float =1.0) -> Union[int, str]:
	"""Adds a legend that pairs values with colors. This is typically used with a heat series. """
	...

def add_colormap_slider(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0) -> Union[int, str]:
	"""Adds a color button."""
	...

def add_combo(items : Union[List[str], Tuple[str]] =(), *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', popup_align_left: bool =False, no_arrow_button: bool =False, no_preview: bool =False, height_mode: int =1) -> Union[int, str]:
	"""Adds a combo dropdown that allows a user to select a single option from a drop down window."""
	...

def add_date_picker(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: dict ={'month_day': 14, 'year':20, 'month':5}, level: int =0) -> Union[int, str]:
	"""Creates a date picker."""
	...

def add_deactivated_after_edit_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is deactivated after edit."""
	...

def add_deactivated_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is deactivated."""
	...

def add_double4_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: Any =(0.0, 0.0, 0.0, 0.0), parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_double_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: float =0.0, parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_drag_float(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, format: str ='%0.3f', speed: float =1.0, min_value: float =0.0, max_value: float =100.0, no_input: bool =False, clamped: bool =False) -> Union[int, str]:
	"""Adds drag for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_floatx(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: Union[List[float], Tuple[float]] =(0.0, 0.0, 0.0, 0.0), size: int =4, format: str ='%0.3f', speed: float =1.0, min_value: float =0.0, max_value: float =100.0, no_input: bool =False, clamped: bool =False) -> Union[int, str]:
	"""Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_int(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: int =0, format: str ='%d', speed: float =1.0, min_value: int =0, max_value: int =100, no_input: bool =False, clamped: bool =False) -> Union[int, str]:
	"""Adds drag for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_intx(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: Union[List[int], Tuple[int]] =(0, 0, 0, 0), size: int =4, format: str ='%d', speed: float =1.0, min_value: int =0, max_value: int =100, no_input: bool =False, clamped: bool =False) -> Union[int, str]:
	"""Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_line(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, callback: Callable =None, show: bool =True, default_value: Any =0.0, color: Union[List[int], Tuple[int]] =(0, 0, 0, -255), thickness: float =1.0, show_label: bool =True, vertical: bool =True) -> Union[int, str]:
	"""Adds a drag line to a plot."""
	...

def add_drag_payload(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, show: bool =True, drag_data: Any =None, payload_type: str ='$$DPG_PAYLOAD') -> Union[int, str]:
	"""User data payload for drag and drop operations."""
	...

def add_drag_point(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, callback: Callable =None, show: bool =True, default_value: Any =(0.0, 0.0), color: Union[List[int], Tuple[int]] =(0, 0, 0, -255), thickness: float =1.0, show_label: bool =True) -> Union[int, str]:
	"""Adds a drag point to a plot."""
	...

def add_draw_layer(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Creates a layer that can be drawn to. Useful for grouping drawing items."""
	...

def add_drawlist(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5) -> Union[int, str]:
	"""A container widget that is used to present draw items or layers. Layers and draw items should be added to this widget as children."""
	...

def add_dummy(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, pos: Union[List[int], Tuple[int]] =[]) -> Union[int, str]:
	"""Adds a spacer or 'dummy' object."""
	...

def add_dynamic_texture(width : int, height : int, default_value : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =internal_dpg.mvReservedUUID_2) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_edited_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is edited."""
	...

def add_error_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], negative : Union[List[float], Tuple[float]], positive : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, contribute_to_bounds: bool =True, horizontal: bool =False) -> Union[int, str]:
	"""Adds an error series to a plot."""
	...

def add_file_dialog(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, callback: Callable =None, show: bool =True, default_path: str ='', default_filename: str ='.', file_count: int =0, modal: bool =False, directory_selector: bool =False) -> Union[int, str]:
	"""Displays a file or directory selector depending on keywords. Displays a file dialog by default."""
	...

def add_file_extension(extension : str, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, parent: Union[int, str] =0, before: Union[int, str] =0, custom_text: str ='', color: Union[List[float], Tuple[float]] =(-255, 0, 0, 255)) -> Union[int, str]:
	"""Creates a file extension filter option in the file dialog. Only works when the parent is a file dialog."""
	...

def add_filter_set(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, delay_search: bool =False) -> Union[int, str]:
	"""Helper to parse and apply text filters (e.g. aaaaa[, bbbbb][, ccccc])"""
	...

def add_float4_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: Union[List[float], Tuple[float]] =(0.0, 0.0, 0.0, 0.0), parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_float_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: float =0.0, parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_float_vect_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: Union[List[float], Tuple[float]] =(), parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_focus_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is focused."""
	...

def add_font(file : str, size : int, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, default_font: bool =False, parent: Union[int, str] =internal_dpg.mvReservedUUID_0) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_font_chars(chars : Union[List[int], Tuple[int]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_font_range(first_char : int, last_char : int, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_font_range_hint(hint : int, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_font_registry(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_group(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, horizontal: bool =False, horizontal_spacing: float =-1) -> Union[int, str]:
	"""Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members. Must be closed with the end command."""
	...

def add_handler_registry(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Adds a handler registry."""
	...

def add_heat_series(x : Union[List[float], Tuple[float]], rows : int, cols : int, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, scale_min: float =0.0, scale_max: float =1.0, bounds_min: Any =(0.0, 0.0), bounds_max: Any =(1.0, 1.0), format: str ='%0.1f', contribute_to_bounds: bool =True) -> Union[int, str]:
	"""Adds a heat series to a plot. Typically a color scale widget is also added to show the legend."""
	...

def add_histogram_series(x : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, bins: int =-1, bar_scale: float =1.0, min_range: float =0.0, max_range: float =1.0, cumlative: bool =False, density: bool =False, outliers: bool =True, contribute_to_bounds: bool =True) -> Union[int, str]:
	"""Adds a histogram series to a plot."""
	...

def add_hline_series(x : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, contribute_to_bounds: bool =True) -> Union[int, str]:
	"""Adds a infinite horizontal line series to a plot."""
	...

def add_hover_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is hovered."""
	...

def add_image(texture_id : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, tint_color: Union[List[float], Tuple[float]] =(255, 255, 255, 255), border_color: Union[List[float], Tuple[float]] =(0, 0, 0, 0), uv_min: Union[List[float], Tuple[float]] =(0.0, 0.0), uv_max: Union[List[float], Tuple[float]] =(1.0, 1.0)) -> Union[int, str]:
	"""Adds an image from a specified texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) for texture coordinates will generally display the entire texture."""
	...

def add_image_button(texture_id : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, frame_padding: int =-1, tint_color: Union[List[float], Tuple[float]] =(255, 255, 255, 255), background_color: Union[List[float], Tuple[float]] =(0, 0, 0, 0), uv_min: Union[List[float], Tuple[float]] =(0.0, 0.0), uv_max: Union[List[float], Tuple[float]] =(1.0, 1.0)) -> Union[int, str]:
	"""Adds an button with a texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) texture coordinates will generally display the entire texture"""
	...

def add_image_series(texture_id : Union[int, str], bounds_min : Union[List[float], Tuple[float]], bounds_max : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, uv_min: Union[List[float], Tuple[float]] =(0.0, 0.0), uv_max: Union[List[float], Tuple[float]] =(1.0, 1.0), tint_color: Union[List[int], Tuple[int]] =(255, 255, 255, 255)) -> Union[int, str]:
	"""Adds a image series to a plot."""
	...

def add_input_float(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, format: str ='%.3f', min_value: float =0.0, max_value: float =100.0, step: float =0.1, step_fast: float =1.0, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> Union[int, str]:
	"""Adds input for floats. Step buttons can be turned on or off."""
	...

def add_input_floatx(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: Union[List[float], Tuple[float]] =(0.0, 0.0, 0.0, 0.0), format: str ='%.3f', min_value: float =0.0, max_value: float =100.0, size: int =4, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> Union[int, str]:
	"""Adds multi float input for up to 4 float values."""
	...

def add_input_int(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: int =0, min_value: int =0, max_value: int =100, step: int =1, step_fast: int =100, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> Union[int, str]:
	"""Adds input for an int. Step buttons can be turned on or off."""
	...

def add_input_intx(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: Union[List[int], Tuple[int]] =(0, 0, 0, 0), min_value: int =0, max_value: int =100, size: int =4, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> Union[int, str]:
	"""Adds multi int input for up to 4 integer values."""
	...

def add_input_text(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', hint: str ='', multiline: bool =False, no_spaces: bool =False, uppercase: bool =False, tab_input: bool =False, decimal: bool =False, hexadecimal: bool =False, readonly: bool =False, password: bool =False, scientific: bool =False, on_enter: bool =False) -> Union[int, str]:
	"""Adds input for text."""
	...

def add_int4_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: Union[List[int], Tuple[int]] =(0, 0, 0, 0), parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_int_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: int =0, parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_key_down_handler(key : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified key is down. Parent must be a handler registry."""
	...

def add_key_press_handler(key : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified key is pressed. Parent must be a handler registry."""
	...

def add_key_release_handler(key : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified key is released. Parent must be a handler registry."""
	...

def add_knob_float(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, min_value: float =0.0, max_value: float =100.0) -> Union[int, str]:
	"""Adds a knob that rotates based of change in x mouse position."""
	...

def add_line_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Adds a line series to a plot."""
	...

def add_listbox(items : Union[List[str], Tuple[str]] =(), *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', num_items: int =3) -> Union[int, str]:
	"""Adds a listbox. If height is not large enought to show all items a scroll bar will appear."""
	...

def add_loading_indicator(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, pos: Union[List[int], Tuple[int]] =[], style: int =0, circle_count: int =8, speed: float =1.0, radius: float =3.0, thickness: float =1.0, color: Union[List[int], Tuple[int]] =(51, 51, 55, 255), secondary_color: Union[List[int], Tuple[int]] =(29, 151, 236, 103)) -> Union[int, str]:
	"""Adds a rotating anamated loding symbol."""
	...

def add_menu(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5) -> Union[int, str]:
	"""Adds a menu to an existing menu bar. Must be followed by a call to end."""
	...

def add_menu_bar(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, show: bool =True, delay_search: bool =False) -> Union[int, str]:
	"""Adds a menu bar to a window."""
	...

def add_menu_item(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: bool =False, shortcut: str ='', check: bool =False) -> Union[int, str]:
	"""Adds a menu item to an existing menu. Menu items act similar to selectables."""
	...

def add_mouse_click_handler(button : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is clicked. Parent must be a handler registry."""
	...

def add_mouse_double_click_handler(button : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is double clicked. Parent must be a handler registry."""
	...

def add_mouse_down_handler(button : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is down. Parent must be a handler registry."""
	...

def add_mouse_drag_handler(button : int =-1, threshold : float =10.0, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is clicked and dragged a set threshold. Parent must be a handler registry."""
	...

def add_mouse_move_handler(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the mouse is moved. Parent must be a handler registry."""
	...

def add_mouse_release_handler(button : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is released. Parent must be a handler registry."""
	...

def add_mouse_wheel_handler(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True, parent: Union[int, str] =internal_dpg.mvReservedUUID_1) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the vertical mouse wheel is scrolled. Parent must be a handler registry."""
	...

def add_node(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, draggable: bool =True) -> Union[int, str]:
	"""Adds a node to a node editor."""
	...

def add_node_attribute(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, attribute_type: int =0, shape: int =1, category: str ='general') -> Union[int, str]:
	"""Adds a node attribute."""
	...

def add_node_editor(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, delink_callback: Callable =None, menubar: bool =False) -> Union[int, str]:
	"""Adds a node editor."""
	...

def add_node_link(attr_1 : Union[int, str], attr_2 : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Adds a node link between nodes."""
	...

def add_pie_series(x : float, y : float, radius : float, values : Union[List[float], Tuple[float]], labels : Union[List[str], Tuple[str]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, format: str ='%0.2f', angle: float =90.0, normalize: bool =False) -> Union[int, str]:
	"""Adds a pie series to a plot."""
	...

def add_plot(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, no_title: bool =False, no_menus: bool =False, no_box_select: bool =False, no_mouse_pos: bool =False, no_highlight: bool =False, no_child: bool =False, query: bool =False, crosshairs: bool =False, anti_aliased: bool =False, equal_aspects: bool =False, pan_button: int =internal_dpg.mvMouseButton_Left, pan_mod: int =-1, fit_button: int =internal_dpg.mvMouseButton_Left, context_menu_button: int =internal_dpg.mvMouseButton_Right, box_select_button: int =internal_dpg.mvMouseButton_Right, box_select_mod: int =-1, box_select_cancel_button: int =internal_dpg.mvMouseButton_Left, query_button: int =internal_dpg.mvMouseButton_Middle, query_mod: int =-1, query_toggle_mod: int =internal_dpg.mvKey_Control, horizontal_mod: int =internal_dpg.mvKey_Alt, vertical_mod: int =internal_dpg.mvKey_Shift) -> Union[int, str]:
	"""Adds a plot which is used to hold series, and can be drawn to with draw commands."""
	...

def add_plot_annotation(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, default_value: Any =(0.0, 0.0), offset: Union[List[float], Tuple[float]] =(0.0, 0.0), color: Union[List[int], Tuple[int]] =(0, 0, 0, -255), clamped: bool =True) -> Union[int, str]:
	"""Adds an annotation to a plot."""
	...

def add_plot_axis(axis : int, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, no_gridlines: bool =False, no_tick_marks: bool =False, no_tick_labels: bool =False, log_scale: bool =False, invert: bool =False, lock_min: bool =False, lock_max: bool =False, time: bool =False) -> Union[int, str]:
	"""Adds a plot legend to a plot."""
	...

def add_plot_legend(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, location: int =5, horizontal: bool =False, outside: bool =False) -> Union[int, str]:
	"""Adds a plot legend to a plot."""
	...

def add_progress_bar(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, overlay: str ='', default_value: float =0.0) -> Union[int, str]:
	"""Adds a progress bar."""
	...

def add_radio_button(items : Union[List[str], Tuple[str]] =(), *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', horizontal: bool =False) -> Union[int, str]:
	"""Adds a set of radio buttons. If items keyword is empty, nothing will be shown."""
	...

def add_raw_texture(width : int, height : int, default_value : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, format: int =internal_dpg.mvFormat_Float_rgba, parent: Union[int, str] =internal_dpg.mvReservedUUID_2) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_resize_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is resized."""
	...

def add_same_line(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, xoffset: float =0.0, spacing: float =-1.0) -> Union[int, str]:
	"""Places a widget on the same line as the previous widget. Can also be used for horizontal spacing."""
	...

def add_scatter_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Adds a scatter series to a plot."""
	...

def add_selectable(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: bool =False, span_columns: bool =False) -> Union[int, str]:
	"""Adds a selectable."""
	...

def add_separator(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, pos: Union[List[int], Tuple[int]] =[]) -> Union[int, str]:
	"""Adds a horizontal line."""
	...

def add_series_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: Any =(), parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_shade_series(x : Union[List[float], Tuple[float]], y1 : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, y2: Any =[]) -> Union[int, str]:
	"""Adds a shade series to a plot."""
	...

def add_simple_plot(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: Union[List[float], Tuple[float]] =(), overlay: str ='', histogram: bool =False, autosize: bool =True, min_scale: float =0.0, max_scale: float =0.0) -> Union[int, str]:
	"""A simple plot for visualization of a 1 dimensional set of values."""
	...

def add_slider_float(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, vertical: bool =False, no_input: bool =False, clamped: bool =False, min_value: float =0.0, max_value: float =100.0, format: str ='%.3f') -> Union[int, str]:
	"""Adds slider for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_floatx(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: Union[List[float], Tuple[float]] =(0.0, 0.0, 0.0, 0.0), size: int =4, no_input: bool =False, clamped: bool =False, min_value: float =0.0, max_value: float =100.0, format: str ='%.3f') -> Union[int, str]:
	"""Adds multi slider for up to 4 float values. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_int(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: int =0, vertical: bool =False, no_input: bool =False, clamped: bool =False, min_value: int =0, max_value: int =100, format: str ='%d') -> Union[int, str]:
	"""Adds slider for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_intx(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: Union[List[int], Tuple[int]] =(0, 0, 0, 0), size: int =4, no_input: bool =False, clamped: bool =False, min_value: int =0, max_value: int =100, format: str ='%d') -> Union[int, str]:
	"""Adds multi slider for up to 4 int values. CTRL+Click to directly modify the value."""
	...

def add_spacing(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, pos: Union[List[int], Tuple[int]] =[], count: int =1) -> Union[int, str]:
	"""Adds vertical spacing."""
	...

def add_staging_container(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_stair_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Adds a stair series to a plot."""
	...

def add_static_texture(width : int, height : int, default_value : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =internal_dpg.mvReservedUUID_2) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_stem_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Adds a stem series to a plot."""
	...

def add_string_value(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, source: Union[int, str] =0, default_value: str ='', parent: Union[int, str] =internal_dpg.mvReservedUUID_3) -> Union[int, str]:
	"""Undocumented"""
	...

def add_subplots(rows : int, columns : int, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, row_ratios: Union[List[float], Tuple[float]] =[], column_ratios: Union[List[float], Tuple[float]] =[], no_title: bool =False, no_menus: bool =False, no_resize: bool =False, no_align: bool =False, link_rows: bool =False, link_columns: bool =False, link_all_x: bool =False, link_all_y: bool =False, column_major: bool =False) -> Union[int, str]:
	"""Adds a plot which is used to hold series, and can be drawn to with draw commands."""
	...

def add_tab(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, closable: bool =False, no_tooltip: bool =False, order_mode: bool =0) -> Union[int, str]:
	"""Adds a tab to a tab bar. Must be closed with thes end command."""
	...

def add_tab_bar(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, reorderable: bool =False) -> Union[int, str]:
	"""Adds a tab bar."""
	...

def add_tab_button(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_reorder: bool =False, leading: bool =False, trailing: bool =False, no_tooltip: bool =False) -> Union[int, str]:
	"""Adds a tab button to a tab bar."""
	...

def add_table(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, header_row: bool =True, inner_width: int =0, policy: int =0, freeze_rows: int =0, freeze_columns: int =0, sort_multi: bool =False, sort_tristate: bool =False, resizable: bool =False, reorderable: bool =False, hideable: bool =False, sortable: bool =False, context_menu_in_body: bool =False, row_background: bool =False, borders_innerH: bool =False, borders_outerH: bool =False, borders_innerV: bool =False, borders_outerV: bool =False, no_host_extendX: bool =False, no_host_extendY: bool =False, no_keep_columns_visible: bool =False, precise_widths: bool =False, no_clip: bool =False, pad_outerX: bool =False, no_pad_outerX: bool =False, no_pad_innerX: bool =False, scrollX: bool =False, scrollY: bool =False, no_saved_settings: bool =False) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_table_column(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, init_width_or_weight: float =0.0, default_hide: bool =False, default_sort: bool =False, width_stretch: bool =False, width_fixed: bool =False, no_resize: bool =False, no_reorder: bool =False, no_hide: bool =False, no_clip: bool =False, no_sort: bool =False, no_sort_ascending: bool =False, no_sort_descending: bool =False, no_header_width: bool =False, prefer_sort_ascending: bool =True, prefer_sort_descending: bool =False, indent_enable: bool =False, indent_disable: bool =False) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_table_next_column(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_table_row(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, height: int =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, filter_key: str ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_text(default_value : str ='', *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, wrap: int =-1, bullet: bool =False, color: Union[List[float], Tuple[float]] =(-1, -1, -1, -1), show_label: bool =False) -> Union[int, str]:
	"""Adds text. Text can have an optional label that will display to the right of the text."""
	...

def add_text_point(x : float, y : float, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True, x_offset: int =..., y_offset: int =..., vertical: bool =False) -> Union[int, str]:
	"""Adds a labels series to a plot."""
	...

def add_texture_registry(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, show: bool =False) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_theme(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, default_theme: bool =False) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_theme_color(target : int =0, value : Union[List[int], Tuple[int]] =(0, 0, 0, 255), *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, category: int =0) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_theme_style(target : int =0, x : float =1.0, y : float =-1.0, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, category: int =0) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_time_picker(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: dict ={'hour': 14, 'min': 32, 'sec': 23}, hour24: bool =False) -> Union[int, str]:
	"""Adds a time picker."""
	...

def add_toggled_open_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is toggled open."""
	...

def add_tooltip(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for."""
	...

def add_tree_node(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, before: Union[int, str] =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: Union[List[int], Tuple[int]] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False, selectable: bool =False) -> Union[int, str]:
	"""Adds a tree node to add items to. Must be closed with the end command."""
	...

def add_value_registry(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0) -> Union[int, str]:
	"""Undocumented function"""
	...

def add_viewport_drawlist(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, show: bool =True, filter_key: str ='', delay_search: bool =False, front: bool =True) -> Union[int, str]:
	"""A container that is used to present draw items or layers directly to the viewport. By default this will draw to the back of teh viewport. Layers and draw items should be added to this widget as children."""
	...

def add_viewport_menu_bar(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, indent: int =-1, parent: Union[int, str] =0, show: bool =True, delay_search: bool =False) -> Union[int, str]:
	"""Adds a menu bar to the viewport."""
	...

def add_visible_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is visible."""
	...

def add_vline_series(x : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, source: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	"""Adds a infinite vertical line series to a plot."""
	...

def add_window(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, width: int =0, height: int =0, indent: int =-1, show: bool =True, pos: Union[List[int], Tuple[int]] =[], delay_search: bool =False, min_size: Union[List[int], Tuple[int]] =[100, 100], max_size: Union[List[int], Tuple[int]] =[30000, 30000], menubar: bool =False, collapsed: bool =False, autosize: bool =False, no_resize: bool =False, no_title_bar: bool =False, no_move: bool =False, no_scrollbar: bool =False, no_collapse: bool =False, horizontal_scrollbar: bool =False, no_focus_on_appearing: bool =False, no_bring_to_front_on_focus: bool =False, no_close: bool =False, no_background: bool =False, modal: bool =False, popup: bool =False, no_saved_settings: bool =False, on_close: Callable =None) -> Union[int, str]:
	"""Creates a new window for following items to be added to."""
	...

def cleanup_dearpygui() -> None:
	"""Cleans up dearpygui."""
	...

def clear_selected_links(node_editor : Union[int, str]) -> None:
	"""Undocumented"""
	...

def clear_selected_nodes(node_editor : Union[int, str]) -> None:
	"""Undocumented"""
	...

def configure_item(item : Union[int, str], **kwargs) -> None:
	"""Undocumented"""
	...

def configure_item_registry(*, allow_alias_overwrites: bool =False, manual_alias_management: bool =False) -> None:
	"""Undocumented"""
	...

def configure_viewport(item : Union[int, str], **kwargs) -> None:
	"""Configures the viewport."""
	...

def create_viewport(*, title: str ='Dear PyGui', small_icon: str ='', large_icon: str ='', width: int =1280, height: int =800, x_pos: int =100, y_pos: int =100, min_width: int =250, max_width: int =10000, min_height: int =250, max_height: int =10000, resizable: bool =True, vsync: bool =True, always_on_top: bool =False, decorated: bool =True, clear_color: Union[List[float], Tuple[float]] =(0, 0, 0, 255)) -> str:
	"""Creates a viewport."""
	...

def delete_item(item : Union[int, str], *, children_only: bool =False, slot: int =-1) -> None:
	"""Undocumented"""
	...

def does_alias_exist(alias : str) -> bool:
	"""Undocumented"""
	...

def does_item_exist(item : Union[int, str]) -> bool:
	"""Undocumented"""
	...

def draw_arrow(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), thickness: float =1.0, size: int =4) -> Union[int, str]:
	"""Draws an arrow on a drawing."""
	...

def draw_bezier_cubic(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], p3 : Union[List[float], Tuple[float]], p4 : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), thickness: float =1.0, segments: int =0) -> Union[int, str]:
	"""Draws a cubic bezier curve on a drawing."""
	...

def draw_bezier_quadratic(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], p3 : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), thickness: float =1.0, segments: int =0) -> Union[int, str]:
	"""Draws a quadratic bezier curve on a drawing."""
	...

def draw_circle(center : Union[List[float], Tuple[float]], radius : float, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), fill: Union[List[int], Tuple[int]] =(0, 0, 0, -255), thickness: float =1.0, segments: int =0) -> Union[int, str]:
	"""Draws a circle on a drawing."""
	...

def draw_ellipse(pmin : Union[List[float], Tuple[float]], pmax : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), fill: Union[List[int], Tuple[int]] =(0, 0, 0, -255), thickness: float =1.0, segments: int =32) -> Union[int, str]:
	"""Draws an ellipse on a drawing."""
	...

def draw_image(texture_id : Union[int, str], pmin : Union[List[float], Tuple[float]], pmax : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, uv_min: Union[List[float], Tuple[float]] =(0.0, 0.0), uv_max: Union[List[float], Tuple[float]] =(1.0, 1.0), color: Union[List[int], Tuple[int]] =(255, 255, 255, 255)) -> Union[int, str]:
	"""Draws an image on a drawing. p_min (top-left) and p_max (bottom-right) represent corners of the rectangle the image will be drawn to.Setting the p_min equal to the p_max will sraw the image to with 1:1 scale.uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using (0.0,0.0)->(1.0,1.0) texturecoordinates will generally display the entire texture."""
	...

def draw_line(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), thickness: float =1.0) -> Union[int, str]:
	"""Draws a line on a drawing."""
	...

def draw_polygon(points : List[List[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), fill: Union[List[int], Tuple[int]] =(0, 0, 0, -255), thickness: float =1.0) -> Union[int, str]:
	"""Draws a polygon on a drawing. First and and last point should be the same to close teh polygone."""
	...

def draw_polyline(points : List[List[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, closed: bool =False, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), thickness: float =1.0) -> Union[int, str]:
	"""Draws connected lines on a drawing from points."""
	...

def draw_quad(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], p3 : Union[List[float], Tuple[float]], p4 : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), fill: Union[List[int], Tuple[int]] =(0, 0, 0, -255), thickness: float =1.0) -> Union[int, str]:
	"""Draws a quad on a drawing."""
	...

def draw_rectangle(pmin : Union[List[float], Tuple[float]], pmax : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), color_upper_left: Union[List[int], Tuple[int]] =(255, 255, 255, 255), color_upper_right: Union[List[int], Tuple[int]] =(255, 255, 255, 255), color_bottom_right: Union[List[int], Tuple[int]] =(255, 255, 255, 255), color_bottom_left: Union[List[int], Tuple[int]] =(255, 255, 255, 255), fill: Union[List[int], Tuple[int]] =(0, 0, 0, -255), multicolor: bool =False, rounding: float =0.0, thickness: float =1.0) -> Union[int, str]:
	"""Draws a rectangle on a drawing."""
	...

def draw_text(pos : Union[List[float], Tuple[float]], text : str, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), size: float =10.0) -> Union[int, str]:
	"""Draws a text on a drawing."""
	...

def draw_triangle(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], p3 : Union[List[float], Tuple[float]], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True, color: Union[List[int], Tuple[int]] =(255, 255, 255, 255), fill: Union[List[int], Tuple[int]] =(0, 0, 0, -255), thickness: float =1.0) -> Union[int, str]:
	"""Draws a triangle on a drawing."""
	...

def empty_container_stack() -> None:
	"""Undocumented"""
	...

def enable_docking(*, dock_space: bool =False) -> None:
	"""Undocumented"""
	...

def fit_axis_data(axis : Union[int, str]) -> None:
	"""Sets the axis boundries max and min in the data series currently on the plot."""
	...

def focus_item(item : Union[int, str]) -> None:
	"""Undocumented"""
	...

def generate_uuid() -> Union[int, str]:
	"""Generate a new UUID"""
	...

def get_active_window() -> Union[int, str]:
	"""Undocumented"""
	...

def get_alias_id(alias : str) -> Union[int, str]:
	"""Undocumented"""
	...

def get_aliases() -> Union[List[str], Tuple[str]]:
	"""Undocumented"""
	...

def get_all_items() -> Union[List[int], Tuple[int]]:
	"""Undocumented"""
	...

def get_axis_limits(axis : Union[int, str]) -> Union[List[float], Tuple[float]]:
	"""Gets the specified axis limits."""
	...

def get_colormap_color(colormap : Union[int, str], index : int) -> Union[List[int], Tuple[int]]:
	"""Returns a color from a colormap given an index >= 0 (modulo will be performed). This command can only be ran once the app is started."""
	...

def get_dearpygui_version() -> str:
	"""Returns the dearpygui version."""
	...

def get_delta_time() -> float:
	"""Returns time since last frame."""
	...

def get_drawing_mouse_pos() -> Union[List[int], Tuple[int]]:
	"""Undocumented"""
	...

def get_file_dialog_info(file_dialog : Union[int, str]) -> dict:
	"""Undocumented function"""
	...

def get_frame_count() -> int:
	"""Get frame count."""
	...

def get_global_font_scale() -> float:
	"""Undocumented function"""
	...

def get_item_alias(item : Union[int, str]) -> str:
	"""Gets the item's alias."""
	...

def get_item_configuration(item : Union[int, str]) -> dict:
	"""Undocumented"""
	...

def get_item_info(item : Union[int, str]) -> dict:
	"""Undocumented"""
	...

def get_item_registry_configuration() -> dict:
	"""Undocumented"""
	...

def get_item_state(item : Union[int, str]) -> dict:
	"""Undocumented"""
	...

def get_item_types() -> dict:
	"""Undocumented"""
	...

def get_mouse_drag_delta() -> float:
	"""Undocumented"""
	...

def get_mouse_pos(*, local: bool =True) -> Union[List[int], Tuple[int]]:
	"""Undocumented"""
	...

def get_plot_mouse_pos() -> Union[List[int], Tuple[int]]:
	"""Undocumented"""
	...

def get_plot_query_area(plot : Union[int, str]) -> Union[List[float], Tuple[float]]:
	"""Returns the last/current query area of the plot. (Requires plot 'query' kwarg to be enabled)"""
	...

def get_selected_links(node_editor : Union[int, str]) -> List[List[str]]:
	"""Undocumented"""
	...

def get_selected_nodes(node_editor : Union[int, str]) -> Union[List[int], Tuple[int]]:
	"""Undocumented"""
	...

def get_total_time() -> float:
	"""Returns total time since Dear PyGui has started."""
	...

def get_value(item : Union[int, str]) -> Any:
	"""Undocumented"""
	...

def get_values(items : Union[List[int], Tuple[int]]) -> Any:
	"""Undocumented"""
	...

def get_viewport_configuration(item : Union[int, str]) -> dict:
	"""Returns viewport configuration."""
	...

def get_windows() -> Union[List[int], Tuple[int]]:
	"""Undocumented"""
	...

def get_x_scroll(item : Union[int, str]) -> float:
	"""Undocumented function"""
	...

def get_x_scroll_max(item : Union[int, str]) -> float:
	"""Undocumented function"""
	...

def get_y_scroll(item : Union[int, str]) -> float:
	"""Undocumented function"""
	...

def get_y_scroll_max(item : Union[int, str]) -> float:
	"""Undocumented function"""
	...

def is_dearpygui_running() -> bool:
	"""Checks if dearpygui is running."""
	...

def is_key_down(key : int) -> bool:
	"""Undocumented"""
	...

def is_key_pressed(key : int) -> bool:
	"""Undocumented"""
	...

def is_key_released(key : int) -> bool:
	"""Undocumented"""
	...

def is_mouse_button_clicked(button : int) -> bool:
	"""Undocumented"""
	...

def is_mouse_button_double_clicked(button : int) -> bool:
	"""Undocumented"""
	...

def is_mouse_button_down(button : int) -> bool:
	"""Undocumented"""
	...

def is_mouse_button_dragging(button : int, threshold : float) -> bool:
	"""Undocumented"""
	...

def is_mouse_button_released(button : int) -> bool:
	"""Undocumented"""
	...

def is_plot_queried(plot : Union[int, str]) -> bool:
	"""Returns true if the plot is currently being queried. (Requires plot 'query' kwarg to be enabled)"""
	...

def is_viewport_created() -> bool:
	"""Checks if a viewport has been created."""
	...

def last_container() -> Union[int, str]:
	"""Undocumented"""
	...

def last_item() -> Union[int, str]:
	"""Undocumented"""
	...

def last_root() -> Union[int, str]:
	"""Undocumented"""
	...

def load_image(file : str, *, gamma: float =1.0, gamma_scale_factor: float =1.0) -> Any:
	"""Loads an image. Returns width, height, channels, mvBuffer"""
	...

def load_init_file(file : str) -> None:
	"""Load dpg.ini file."""
	...

def lock_mutex() -> None:
	"""Locks mutex"""
	...

def maximize_viewport() -> None:
	"""Maximizes the viewport."""
	...

def minimize_viewport() -> None:
	"""Minimizes the viewport."""
	...

def move_item(item : Union[int, str], *, parent: Union[int, str] =0, before: Union[int, str] =0) -> None:
	"""Undocumented"""
	...

def move_item_down(item : Union[int, str]) -> None:
	"""Undocumented"""
	...

def move_item_up(item : Union[int, str]) -> None:
	"""Undocumented"""
	...

def pop_container_stack() -> Union[int, str]:
	"""Undocumented"""
	...

def push_container_stack(item : Union[int, str]) -> bool:
	"""Undocumented"""
	...

def remove_alias(alias : str) -> None:
	"""Undocumented"""
	...

def render_dearpygui_frame() -> None:
	"""Renders a dearpygui frame."""
	...

def reorder_items(container : Union[int, str], slot : int, new_order : Union[List[int], Tuple[int]]) -> None:
	"""Undocumented"""
	...

def reset_axis_ticks(axis : Union[int, str]) -> None:
	"""Removes the and manually set axis ticks and applys the default auto axis ticks."""
	...

def reset_default_theme() -> None:
	"""Resets to default theme."""
	...

def reset_pos(item : Union[int, str]) -> None:
	"""Undocumented"""
	...

def sample_colormap(colormap : Union[int, str], t : float) -> Union[List[int], Tuple[int]]:
	"""Returns a color from a colormap given t between 0 and 1. This command can only be ran once the app is started."""
	...

def save_init_file(file : str) -> None:
	"""Save dpg.ini file."""
	...

def set_axis_limits(axis : Union[int, str], ymin : float, ymax : float) -> None:
	"""Sets limits on the axis for pan and zoom."""
	...

def set_axis_limits_auto(axis : Union[int, str]) -> None:
	"""Removes all limits on specified axis."""
	...

def set_axis_ticks(axis : Union[int, str], label_pairs : Any) -> None:
	"""Replaces axis ticks with 'label_pairs' argument"""
	...

def set_colormap(item : Union[int, str], source : Union[int, str]) -> None:
	"""Sets the color map for widgets that accept it."""
	...

def set_exit_callback(callback : Callable) -> str:
	"""Undocumented function"""
	...

def set_global_font_scale(scale : float) -> None:
	"""Undocumented function"""
	...

def set_init_file(*, file: str ='dpg.ini') -> None:
	"""set dpg.ini file."""
	...

def set_item_alias(item : Union[int, str], alias : str) -> None:
	"""Sets the item's alias."""
	...

def set_item_children(item : Union[int, str], source : Union[int, str], slot : int) -> dict:
	"""Undocumented"""
	...

def set_item_disabled_theme(item : Union[int, str], theme : Union[int, str]) -> None:
	"""Undocumented"""
	...

def set_item_font(item : Union[int, str], font : Union[int, str]) -> None:
	"""Undocumented"""
	...

def set_item_theme(item : Union[int, str], theme : Union[int, str]) -> None:
	"""Undocumented"""
	...

def set_item_type_disabled_theme(item : int, theme : Union[int, str]) -> None:
	"""Undocumented"""
	...

def set_item_type_theme(item : int, theme : Union[int, str]) -> None:
	"""Undocumented"""
	...

def set_primary_window(window : Union[int, str], value : bool) -> None:
	"""Undocumented"""
	...

def set_staging_mode(mode : bool) -> None:
	"""Undocumented"""
	...

def set_start_callback(callback : Callable) -> str:
	"""Undocumented function"""
	...

def set_value(item : Union[int, str], value : Any) -> None:
	"""Undocumented"""
	...

def set_viewport_resize_callback(callback : Callable) -> str:
	"""Undocumented function"""
	...

def set_x_scroll(item : Union[int, str], value : float) -> None:
	"""Undocumented function"""
	...

def set_y_scroll(item : Union[int, str], value : float) -> None:
	"""Undocumented function"""
	...

def setup_dearpygui(*, viewport: str ='') -> None:
	"""Sets up dearpygui"""
	...

def show_imgui_demo() -> None:
	"""Undocumented"""
	...

def show_implot_demo() -> None:
	"""Undocumented"""
	...

def show_tool(tool : Union[int, str]) -> str:
	"""Undocumented function"""
	...

def show_viewport(viewport : str, *, minimized: bool =False, maximized: bool =False) -> None:
	"""Shows the viewport"""
	...

def split_frame(*, delay: int =32) -> None:
	"""Waits one frame."""
	...

def stage_items(items : Union[List[int], Tuple[int]]) -> None:
	"""Undocumented"""
	...

def stop_dearpygui() -> None:
	"""Stops dearpygui."""
	...

def top_container_stack() -> Union[int, str]:
	"""Undocumented"""
	...

def unlock_mutex() -> None:
	"""Unlocks mutex"""
	...

def unstage_items(items : Union[List[int], Tuple[int]]) -> None:
	"""Undocumented"""
	...

