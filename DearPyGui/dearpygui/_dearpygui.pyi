from typing import List, Any, Callable
from dearpygui._dearpygui import *

##########################################################
# This file is generated automatically by mvPythonParser #
##########################################################

# ~ Dear PyGui Version: master
def add_2d_histogram_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, xbins: int =-1, ybins: int =-1, xmin_range: float =0.0, xmax_range: float =1.0, ymin_range: float =0.0, ymax_range: float =1.0, density: bool =False, outliers: bool =True) -> int:
	"""Undocumented function"""
	...

def add_3d_slider(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), max_x: float =100.0, max_y: float =100.0, max_z: float =100.0, min_x: float =0.0, min_y: float =0.0, min_z: float =0.0, scale: float =1.0) -> int:
	"""Adds a 3D box slider that allows a 3d point to be show in 2d represented cube space."""
	...

def add_activated_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is activated."""
	...

def add_active_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is active."""
	...

def add_area_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, fill: List[int] =(0, 0, 0, -255), contribute_to_bounds: bool =True) -> int:
	"""Adds an area series to a plot."""
	...

def add_bar_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, weight: float =1.0, horizontal: bool =False) -> int:
	"""Adds a bar series to a plot."""
	...

def add_bool_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: bool =False, parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_button(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, small: bool =False, arrow: bool =False, direction: int =0) -> int:
	"""Adds a button."""
	...

def add_candle_series(dates : List[float], opens : List[float], closes : List[float], lows : List[float], highs : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, bull_color: List[int] =(0, 255, 113, 255), bear_color: List[int] =(218, 13, 79, 255), weight: int =0.25, tooltip: bool =True) -> int:
	"""Adds a candle series to a plot."""
	...

def add_char_remap(source : int, target : int, *, label: str =None, id: int =0, parent: int =0, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_checkbox(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: bool =False) -> int:
	"""Adds a checkbox."""
	...

def add_child(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, border: bool =True, autosize_x: bool =False, autosize_y: bool =False, no_scrollbar: bool =False, horizontal_scrollbar: bool =False, menubar: bool =False) -> int:
	"""Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end."""
	...

def add_clicked_handler(parent : int, button : int =-1, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is clicked."""
	...

def add_clipper(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, delay_search: bool =False, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Helper to manually clip large list of items. Increases performance by not searching or drawing widgets outside of the clipped region."""
	...

def add_collapsing_header(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, closable: bool =False, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False) -> int:
	"""Adds a collapsing header to add items to. Must be closed with the end command."""
	...

def add_color_button(default_value : List[int] =(0, 0, 0, 255), *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, no_alpha: bool =False, no_border: bool =False, no_drag_drop: bool =False) -> int:
	"""Adds a color button."""
	...

def add_color_edit(default_value : List[int] =(0, 0, 0, 255), *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, no_alpha: bool =False, no_picker: bool =False, no_options: bool =False, no_small_preview: bool =False, no_inputs: bool =False, no_tooltip: bool =False, no_label: bool =False, no_drag_drop: bool =False, alpha_bar: bool =False, alpha_preview: int =0, display_mode: int =1048576, display_type: int =8388608, input_mode: int =134217728) -> int:
	"""Adds an RGBA color editor. Click the small color preview will provide a color picker. Click and draging the small color preview will copy the color to be applied on any other color widget."""
	...

def add_color_picker(default_value : List[int] =(0, 0, 0, 255), *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, no_alpha: bool =False, no_side_preview: bool =False, no_small_preview: bool =False, no_inputs: bool =False, no_tooltip: bool =False, no_label: bool =False, alpha_bar: bool =False, display_rgb: bool =False, display_hsv: bool =False, display_hex: bool =False, picker_mode: int =33554432, alpha_preview: int =0, display_type: int =8388608, input_mode: int =134217728) -> int:
	"""Adds an RGB color picker. Right click the color picker for options. Click and drag the color preview to copy the color and drop on any other color widget to apply. Right Click allows the style of the color picker to be changed."""
	...

def add_color_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_colormap_scale(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, show: bool =True, pos: List[int] =[], user_data: Any =None, use_internal_label: bool =True, default_value: int =0, min_scale: float =0.0, max_scale: float =1.0) -> int:
	"""Adds a legend that pairs values with colors. This is typically used with a heat series. """
	...

def add_combo(items : List[str] =(), *, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: str ='', popup_align_left: bool =False, no_arrow_button: bool =False, no_preview: bool =False, height_mode: int =1) -> int:
	"""Adds a combo dropdown that allows a user to select a single option from a drop down window."""
	...

def add_date_picker(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: dict ={'month_day': 14, 'year':20, 'month':5}, level: int =0) -> int:
	"""Creates a date picker."""
	...

def add_deactivated_after_edit_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is deactivated after edit."""
	...

def add_deactivated_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is deactivated."""
	...

def add_double4_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: Any =(0.0, 0.0, 0.0, 0.0), parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_double_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: float =0.0, parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_drag_float(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: float =0.0, format: str ='%0.3f', speed: float =1.0, min_value: float =0.0, max_value: float =100.0, no_input: bool =False, clamped: bool =False) -> int:
	"""Adds drag for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_floatx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), size: int =4, format: str ='%0.3f', speed: float =1.0, min_value: float =0.0, max_value: float =100.0, no_input: bool =False, clamped: bool =False) -> int:
	"""Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_int(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: int =0, format: str ='%d', speed: float =1.0, min_value: int =0, max_value: int =100, no_input: bool =False, clamped: bool =False) -> int:
	"""Adds drag for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_intx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: List[int] =(0, 0, 0, 0), size: int =4, format: str ='%d', speed: float =1.0, min_value: int =0, max_value: int =100, no_input: bool =False, clamped: bool =False) -> int:
	"""Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_line(*, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, default_value: Any =0.0, color: List[int] =(0, 0, 0, -255), thickness: float =1.0, show_label: bool =True, vertical: bool =True) -> int:
	"""Adds a drag line to a plot."""
	...

def add_drag_payload(*, label: str =None, id: int =0, parent: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, drag_data: Any =None, payload_type: str ='$$DPG_PAYLOAD') -> int:
	"""User data payload for drag and drop operations."""
	...

def add_drag_point(*, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, default_value: Any =(0.0, 0.0), color: List[int] =(0, 0, 0, -255), thickness: float =1.0, show_label: bool =True) -> int:
	"""Adds a drag point to a plot."""
	...

def add_draw_layer(*, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Creates a layer that can be drawn to. Useful for grouping drawing items."""
	...

def add_drawlist(*, label: str =None, id: int =0, width: int =0, height: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""A container widget that is used to present draw items or layers. Layers and draw items should be added to this widget as children."""
	...

def add_dummy(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[], user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a spacer or 'dummy' object."""
	...

def add_dynamic_texture(width : int, height : int, default_value : List[float], *, label: str =None, id: int =0, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_2) -> int:
	"""Undocumented function"""
	...

def add_edited_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is edited."""
	...

def add_error_series(x : List[float], y : List[float], negative : List[float], positive : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, contribute_to_bounds: bool =True, horizontal: bool =False) -> int:
	"""Adds an error series to a plot."""
	...

def add_file_dialog(*, label: str =None, id: int =0, width: int =0, height: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, default_path: str ='', default_filename: str ='.', file_count: int =0, modal: bool =False, directory_selector: bool =False) -> int:
	"""Displays a file or directory selector depending on keywords. Displays a file dialog by default."""
	...

def add_file_extension(extension : str, *, label: str =None, id: int =0, width: int =0, height: int =0, parent: int =0, before: int =0, user_data: Any =None, use_internal_label: bool =True, custom_text: str ='', color: List[float] =(-255, 0, 0, 255)) -> int:
	"""Creates a file extension filter option in the file dialog. Only works when the parent is a file dialog."""
	...

def add_filter_set(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, delay_search: bool =False, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Helper to parse and apply text filters (e.g. aaaaa[, bbbbb][, ccccc])"""
	...

def add_float4_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_float_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: float =0.0, parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_float_vect_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: List[float] =(), parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_focus_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is focused."""
	...

def add_font(file : str, size : int, *, label: str =None, id: int =0, user_data: Any =None, use_internal_label: bool =True, default_font: bool =False, parent: int =internal_dpg.mvReservedUUID_0) -> int:
	"""Undocumented function"""
	...

def add_font_chars(chars : List[int], *, label: str =None, id: int =0, parent: int =0, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_font_range(first_char : int, last_char : int, *, label: str =None, id: int =0, parent: int =0, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_font_range_hint(hint : int, *, label: str =None, id: int =0, parent: int =0, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_font_registry(*, label: str =None, id: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_group(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, horizontal: bool =False, horizontal_spacing: float =-1) -> int:
	"""Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members. Must be closed with the end command."""
	...

def add_handler_registry(*, label: str =None, id: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler registry."""
	...

def add_heat_series(x : List[float], rows : int, cols : int, *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, scale_min: float =0.0, scale_max: float =1.0, bounds_min: Any =(0.0, 0.0), bounds_max: Any =(1.0, 1.0), format: str ='%0.1f', contribute_to_bounds: bool =True) -> int:
	"""Adds a heat series to a plot. Typically a color scale widget is also added to show the legend."""
	...

def add_histogram_series(x : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, bins: int =-1, bar_scale: float =1.0, min_range: float =0.0, max_range: float =1.0, cumlative: bool =False, density: bool =False, outliers: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Adds a histogram series to a plot."""
	...

def add_hline_series(x : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Adds a infinite horizontal line series to a plot."""
	...

def add_hover_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is hovered."""
	...

def add_image(texture_id : int, *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, tint_color: List[float] =(255, 255, 255, 255), border_color: List[float] =(0, 0, 0, 0), uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0)) -> int:
	"""Adds an image from a specified texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) for texture coordinates will generally display the entire texture."""
	...

def add_image_button(texture_id : int, *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, frame_padding: int =-1, tint_color: List[float] =(255, 255, 255, 255), background_color: List[float] =(0, 0, 0, 0), uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0)) -> int:
	"""Adds an button with a texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) texture coordinates will generally display the entire texture"""
	...

def add_image_series(texture_id : int, bounds_min : List[float], bounds_max : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0), tint_color: List[int] =(255, 255, 255, 255)) -> int:
	"""Adds a image series to a plot."""
	...

def add_input_float(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: float =0.0, format: str ='%.3f', min_value: float =0.0, max_value: float =100.0, step: float =0.1, step_fast: float =1.0, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Adds input for floats. Step buttons can be turned on or off."""
	...

def add_input_floatx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), format: str ='%.3f', min_value: float =0.0, max_value: float =100.0, size: int =4, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Adds multi float input for up to 4 float values."""
	...

def add_input_int(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: int =0, min_value: int =0, max_value: int =100, step: int =1, step_fast: int =100, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Adds input for an int. Step buttons can be turned on or off."""
	...

def add_input_intx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: List[int] =(0, 0, 0, 0), min_value: int =0, max_value: int =100, size: int =4, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Adds multi int input for up to 4 integer values."""
	...

def add_input_text(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: str ='', hint: str ='', multiline: bool =False, no_spaces: bool =False, uppercase: bool =False, tab_input: bool =False, decimal: bool =False, hexadecimal: bool =False, readonly: bool =False, password: bool =False, scientific: bool =False, on_enter: bool =False) -> int:
	"""Adds input for text."""
	...

def add_int4_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: List[int] =(0, 0, 0, 0), parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_int_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: int =0, parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_key_down_handler(key : int =-1, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the specified key is down. Parent must be a handler registry."""
	...

def add_key_press_handler(key : int =-1, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the specified key is pressed. Parent must be a handler registry."""
	...

def add_key_release_handler(key : int =-1, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the specified key is released. Parent must be a handler registry."""
	...

def add_knob_float(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: float =0.0, min_value: float =0.0, max_value: float =100.0) -> int:
	"""Adds a knob that rotates based of change in x mouse position."""
	...

def add_line_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a line series to a plot."""
	...

def add_listbox(items : List[str] =(), *, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: str ='', num_items: int =3) -> int:
	"""Adds a listbox. If height is not large enought to show all items a scroll bar will appear."""
	...

def add_loading_indicator(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[], user_data: Any =None, use_internal_label: bool =True, style: int =0, circle_count: int =8, speed: float =1.0, radius: float =3.0, thickness: float =1.0, color: List[int] =(51, 51, 55, 255), secondary_color: List[int] =(29, 151, 236, 103)) -> int:
	"""Adds a rotating anamated loding symbol."""
	...

def add_menu(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a menu to an existing menu bar. Must be followed by a call to end."""
	...

def add_menu_bar(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, show: bool =True, delay_search: bool =False, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a menu bar to a window."""
	...

def add_menu_item(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: bool =False, shortcut: str ='', check: bool =False) -> int:
	"""Adds a menu item to an existing menu. Menu items act similar to selectables."""
	...

def add_mouse_click_handler(button : int =-1, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is clicked. Parent must be a handler registry."""
	...

def add_mouse_double_click_handler(button : int =-1, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is double clicked. Parent must be a handler registry."""
	...

def add_mouse_down_handler(button : int =-1, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is down. Parent must be a handler registry."""
	...

def add_mouse_drag_handler(button : int =-1, threshold : float =10.0, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is clicked and dragged a set threshold. Parent must be a handler registry."""
	...

def add_mouse_move_handler(*, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the mouse is moved. Parent must be a handler registry."""
	...

def add_mouse_release_handler(button : int =-1, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the specified mouse button is released. Parent must be a handler registry."""
	...

def add_mouse_wheel_handler(*, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_1) -> int:
	"""Adds a handler which runs a given callback when the vertical mouse wheel is scrolled. Parent must be a handler registry."""
	...

def add_node(*, label: str =None, id: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, draggable: bool =True) -> int:
	"""Adds a node to a node editor."""
	...

def add_node_attribute(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, attribute_type: int =0, shape: int =1, category: str ='general') -> int:
	"""Adds a node attribute."""
	...

def add_node_editor(*, label: str =None, id: int =0, width: int =0, height: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, delink_callback: Callable =None, menubar: bool =False) -> int:
	"""Adds a node editor."""
	...

def add_node_link(attr_1 : int, attr_2 : int, *, label: str =None, id: int =0, parent: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a node link between nodes."""
	...

def add_pie_series(x : float, y : float, radius : float, values : List[float], labels : List[str], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, format: str ='%0.2f', angle: float =90.0, normalize: bool =False) -> int:
	"""Adds a pie series to a plot."""
	...

def add_plot(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, no_title: bool =False, no_menus: bool =False, no_box_select: bool =False, no_mouse_pos: bool =False, no_highlight: bool =False, no_child: bool =False, query: bool =False, crosshairs: bool =False, anti_aliased: bool =False, equal_aspects: bool =False, pan_button: int =internal_dpg.mvMouseButton_Left, pan_mod: int =-1, fit_button: int =internal_dpg.mvMouseButton_Left, context_menu_button: int =internal_dpg.mvMouseButton_Right, box_select_button: int =internal_dpg.mvMouseButton_Right, box_select_mod: int =-1, box_select_cancel_button: int =internal_dpg.mvMouseButton_Left, query_button: int =internal_dpg.mvMouseButton_Middle, query_mod: int =-1, query_toggle_mod: int =internal_dpg.mvKey_Control, horizontal_mod: int =internal_dpg.mvKey_Alt, vertical_mod: int =internal_dpg.mvKey_Shift) -> int:
	"""Adds a plot which is used to hold series, and can be drawn to with draw commands."""
	...

def add_plot_annotation(*, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, default_value: Any =(0.0, 0.0), offset: List[float] =(0.0, 0.0), color: List[int] =(0, 0, 0, -255), clamped: bool =True) -> int:
	"""Adds an annotation to a plot."""
	...

def add_plot_axis(axis : int, *, label: str =None, id: int =0, parent: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, no_gridlines: bool =False, no_tick_marks: bool =False, no_tick_labels: bool =False, log_scale: bool =False, invert: bool =False, lock_min: bool =False, lock_max: bool =False, time: bool =False) -> int:
	"""Adds a plot legend to a plot."""
	...

def add_plot_legend(*, label: str =None, id: int =0, parent: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True, location: int =5, horizontal: bool =False, outside: bool =False) -> int:
	"""Adds a plot legend to a plot."""
	...

def add_progress_bar(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, overlay: str ='', default_value: float =0.0) -> int:
	"""Adds a progress bar."""
	...

def add_radio_button(items : List[str] =(), *, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: str ='', horizontal: bool =False) -> int:
	"""Adds a set of radio buttons. If items keyword is empty, nothing will be shown."""
	...

def add_raw_texture(width : int, height : int, default_value : List[float], *, label: str =None, id: int =0, user_data: Any =None, use_internal_label: bool =True, format: int =internal_dpg.mvFormat_Float_rgba, parent: int =internal_dpg.mvReservedUUID_2) -> int:
	"""Undocumented function"""
	...

def add_resize_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is resized."""
	...

def add_same_line(*, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, xoffset: float =0.0, spacing: float =-1.0) -> int:
	"""Places a widget on the same line as the previous widget. Can also be used for horizontal spacing."""
	...

def add_scatter_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a scatter series to a plot."""
	...

def add_selectable(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: bool =False, span_columns: bool =False) -> int:
	"""Adds a selectable."""
	...

def add_separator(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[], user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a horizontal line."""
	...

def add_series_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: Any =(), parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_shade_series(x : List[float], y1 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, y2: Any =[]) -> int:
	"""Adds a shade series to a plot."""
	...

def add_simple_plot(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: List[float] =(), overlay: str ='', histogram: bool =False, autosize: bool =True, min_scale: float =0.0, max_scale: float =0.0) -> int:
	"""A simple plot for visualization of a 1 dimensional set of values."""
	...

def add_slider_float(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: float =0.0, vertical: bool =False, no_input: bool =False, clamped: bool =False, min_value: float =0.0, max_value: float =100.0, format: str ='%.3f') -> int:
	"""Adds slider for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_floatx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), size: int =4, no_input: bool =False, clamped: bool =False, min_value: float =0.0, max_value: float =100.0, format: str ='%.3f') -> int:
	"""Adds multi slider for up to 4 float values. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_int(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: int =0, vertical: bool =False, no_input: bool =False, clamped: bool =False, min_value: int =0, max_value: int =100, format: str ='%d') -> int:
	"""Adds slider for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_intx(*, label: str =None, id: int =0, width: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: List[int] =(0, 0, 0, 0), size: int =4, no_input: bool =False, clamped: bool =False, min_value: int =0, max_value: int =100, format: str ='%d') -> int:
	"""Adds multi slider for up to 4 int values. CTRL+Click to directly modify the value."""
	...

def add_spacing(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[], user_data: Any =None, use_internal_label: bool =True, count: int =1) -> int:
	"""Adds vertical spacing."""
	...

def add_staging_container(*, label: str =None, id: int =0, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_stair_series(x : List[float], y : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a stair series to a plot."""
	...

def add_static_texture(width : int, height : int, default_value : List[float], *, label: str =None, id: int =0, user_data: Any =None, use_internal_label: bool =True, parent: int =internal_dpg.mvReservedUUID_2) -> int:
	"""Undocumented function"""
	...

def add_stem_series(x : List[float], y : List[float], *, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a stem series to a plot."""
	...

def add_string_value(*, label: str =None, id: int =0, source: int =0, user_data: Any =None, use_internal_label: bool =True, default_value: str ='', parent: int =internal_dpg.mvReservedUUID_3) -> int:
	"""Undocumented"""
	...

def add_subplots(rows : int, columns : int, *, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, row_ratios: List[float] =[], column_ratios: List[float] =[], no_title: bool =False, no_menus: bool =False, no_resize: bool =False, no_align: bool =False, link_rows: bool =False, link_columns: bool =False, link_all_x: bool =False, link_all_y: bool =False, column_major: bool =False) -> int:
	"""Adds a plot which is used to hold series, and can be drawn to with draw commands."""
	...

def add_tab(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, closable: bool =False, no_tooltip: bool =False, order_mode: bool =0) -> int:
	"""Adds a tab to a tab bar. Must be closed with thes end command."""
	...

def add_tab_bar(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, reorderable: bool =False) -> int:
	"""Adds a tab bar."""
	...

def add_tab_button(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, no_reorder: bool =False, leading: bool =False, trailing: bool =False, no_tooltip: bool =False) -> int:
	"""Adds a tab button to a tab bar."""
	...

def add_table(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, user_data: Any =None, use_internal_label: bool =True, header_row: bool =True, inner_width: int =0, policy: int =0, freeze_rows: int =0, freeze_columns: int =0, sort_multi: bool =False, sort_tristate: bool =False, resizable: bool =False, reorderable: bool =False, hideable: bool =False, sortable: bool =False, context_menu_in_body: bool =False, row_background: bool =False, borders_innerH: bool =False, borders_outerH: bool =False, borders_innerV: bool =False, borders_outerV: bool =False, no_host_extendX: bool =False, no_host_extendY: bool =False, no_keep_columns_visible: bool =False, precise_widths: bool =False, no_clip: bool =False, pad_outerX: bool =False, no_pad_outerX: bool =False, no_pad_innerX: bool =False, scrollX: bool =False, scrollY: bool =False, no_saved_settings: bool =False) -> int:
	"""Undocumented function"""
	...

def add_table_column(*, label: str =None, id: int =0, width: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, init_width_or_weight: float =0.0, default_hide: bool =False, default_sort: bool =False, width_stretch: bool =False, width_fixed: bool =False, no_resize: bool =False, no_reorder: bool =False, no_hide: bool =False, no_clip: bool =False, no_sort: bool =False, no_sort_ascending: bool =False, no_sort_descending: bool =False, no_header_width: bool =False, prefer_sort_ascending: bool =True, prefer_sort_descending: bool =False, indent_enable: bool =False, indent_disable: bool =False) -> int:
	"""Undocumented function"""
	...

def add_table_next_column(*, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_table_row(*, label: str =None, id: int =0, height: int =0, parent: int =0, before: int =0, show: bool =True, filter_key: str ='', user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_text(default_value : str ='', *, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, wrap: int =-1, bullet: bool =False, color: List[float] =(-1, -1, -1, -1), show_label: bool =False) -> int:
	"""Adds text. Text can have an optional label that will display to the right of the text."""
	...

def add_text_point(x : float, y : float, *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, x_offset: int =..., y_offset: int =..., vertical: bool =False) -> int:
	"""Adds a labels series to a plot."""
	...

def add_texture_registry(*, label: str =None, id: int =0, user_data: Any =None, use_internal_label: bool =True, show: bool =False) -> int:
	"""Undocumented function"""
	...

def add_theme(*, label: str =None, id: int =0, user_data: Any =None, use_internal_label: bool =True, default_theme: bool =False) -> int:
	"""Undocumented function"""
	...

def add_theme_color(target : int =0, value : List[int] =(0, 0, 0, 255), *, label: str =None, id: int =0, parent: int =0, user_data: Any =None, use_internal_label: bool =True, category: int =0) -> int:
	"""Undocumented function"""
	...

def add_theme_style(target : int =0, x : float =1.0, y : float =-1.0, *, label: str =None, id: int =0, parent: int =0, user_data: Any =None, use_internal_label: bool =True, category: int =0) -> int:
	"""Undocumented function"""
	...

def add_time_picker(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_value: dict ={'hour': 14, 'min': 32, 'sec': 23}, hour24: bool =False) -> int:
	"""Adds a time picker."""
	...

def add_toggled_open_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is toggled open."""
	...

def add_tooltip(parent : int, *, label: str =None, id: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for."""
	...

def add_tree_node(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', delay_search: bool =False, tracked: bool =False, track_offset: float =0.5, user_data: Any =None, use_internal_label: bool =True, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False, selectable: bool =False) -> int:
	"""Adds a tree node to add items to. Must be closed with the end command."""
	...

def add_value_registry(*, label: str =None, id: int =0, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_viewport_drawlist(*, label: str =None, id: int =0, show: bool =True, filter_key: str ='', delay_search: bool =False, user_data: Any =None, use_internal_label: bool =True, front: bool =True) -> int:
	"""A container that is used to present draw items or layers directly to the viewport. By default this will draw to the back of teh viewport. Layers and draw items should be added to this widget as children."""
	...

def add_viewport_menu_bar(*, label: str =None, id: int =0, indent: int =-1, parent: int =0, show: bool =True, delay_search: bool =False, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a menu bar to the viewport."""
	...

def add_visible_handler(parent : int, *, label: str =None, id: int =0, callback: Callable =None, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a handler which runs a given callback when the specified item is visible."""
	...

def add_vline_series(x : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, source: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True) -> int:
	"""Adds a infinite vertical line series to a plot."""
	...

def add_window(*, label: str =None, id: int =0, width: int =0, height: int =0, indent: int =-1, show: bool =True, pos: List[int] =[], delay_search: bool =False, user_data: Any =None, use_internal_label: bool =True, min_size: List[int] =[100, 100], max_size: List[int] =[30000, 30000], menubar: bool =False, collapsed: bool =False, autosize: bool =False, no_resize: bool =False, no_title_bar: bool =False, no_move: bool =False, no_scrollbar: bool =False, no_collapse: bool =False, horizontal_scrollbar: bool =False, no_focus_on_appearing: bool =False, no_bring_to_front_on_focus: bool =False, no_close: bool =False, no_background: bool =False, modal: bool =False, popup: bool =False, no_saved_settings: bool =False, on_close: Callable =None) -> int:
	"""Creates a new window for following items to be added to."""
	...

def cleanup_dearpygui() -> None:
	"""Cleans up dearpygui."""
	...

def clear_selected_links(node_editor : int) -> None:
	"""Undocumented"""
	...

def clear_selected_nodes(node_editor : int) -> None:
	"""Undocumented"""
	...

def configure_item(item : int, **kwargs) -> None:
	"""Undocumented"""
	...

def configure_viewport(item : int, **kwargs) -> None:
	"""Configures the viewport."""
	...

def create_viewport(*, title: str ='Dear PyGui', small_icon: str ='', large_icon: str ='', width: int =1280, height: int =800, x_pos: int =100, y_pos: int =100, min_width: int =250, max_width: int =10000, min_height: int =250, max_height: int =10000, resizable: bool =True, vsync: bool =True, always_on_top: bool =False, decorated: bool =True, clear_color: List[float] =(0, 0, 0, 255)) -> str:
	"""Creates a viewport."""
	...

def delete_item(item : int, *, children_only: bool =False, slot: int =-1) -> None:
	"""Undocumented"""
	...

def does_item_exist(item : int) -> bool:
	"""Undocumented"""
	...

def draw_arrow(p1 : List[float], p2 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0, size: int =4) -> int:
	"""Draws an arrow on a drawing."""
	...

def draw_bezier_cubic(p1 : List[float], p2 : List[float], p3 : List[float], p4 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0, segments: int =0) -> int:
	"""Draws a cubic bezier curve on a drawing."""
	...

def draw_bezier_quadratic(p1 : List[float], p2 : List[float], p3 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0, segments: int =0) -> int:
	"""Draws a quadratic bezier curve on a drawing."""
	...

def draw_circle(center : List[float], radius : float, *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0, segments: int =0) -> int:
	"""Draws a circle on a drawing."""
	...

def draw_ellipse(pmin : List[float], pmax : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0, segments: int =32) -> int:
	"""Draws an ellipse on a drawing."""
	...

def draw_image(texture_id : int, pmin : List[float], pmax : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0), color: List[int] =(255, 255, 255, 255)) -> int:
	"""Draws an image on a drawing. p_min (top-left) and p_max (bottom-right) represent corners of the rectangle the image will be drawn to.Setting the p_min equal to the p_max will sraw the image to with 1:1 scale.uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using (0.0,0.0)->(1.0,1.0) texturecoordinates will generally display the entire texture."""
	...

def draw_line(p1 : List[float], p2 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0) -> int:
	"""Draws a line on a drawing."""
	...

def draw_polygon(points : List[List[float]], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0) -> int:
	"""Draws a polygon on a drawing. First and and last point should be the same to close teh polygone."""
	...

def draw_polyline(points : List[List[float]], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, closed: bool =False, color: List[int] =(255, 255, 255, 255), thickness: float =1.0) -> int:
	"""Draws connected lines on a drawing from points."""
	...

def draw_quad(p1 : List[float], p2 : List[float], p3 : List[float], p4 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0) -> int:
	"""Draws a quad on a drawing."""
	...

def draw_rectangle(pmin : List[float], pmax : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), rounding: float =0.0, thickness: float =1.0) -> int:
	"""Draws a rectangle on a drawing."""
	...

def draw_text(pos : List[float], text : str, *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), size: float =10.0) -> int:
	"""Draws a text on a drawing."""
	...

def draw_triangle(p1 : List[float], p2 : List[float], p3 : List[float], *, label: str =None, id: int =0, parent: int =0, before: int =0, show: bool =True, user_data: Any =None, use_internal_label: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0) -> int:
	"""Draws a triangle on a drawing."""
	...

def empty_container_stack() -> None:
	"""Undocumented"""
	...

def enable_docking(*, dock_space: bool =False) -> None:
	"""Undocumented"""
	...

def fit_axis_data(axis : int) -> None:
	"""Sets the axis boundries max and min in the data series currently on the plot."""
	...

def focus_item(item : int) -> None:
	"""Undocumented"""
	...

def generate_uuid() -> int:
	"""Generate a new UUID"""
	...

def get_active_window() -> int:
	"""Undocumented"""
	...

def get_all_items() -> List[int]:
	"""Undocumented"""
	...

def get_axis_limits(axis : int) -> List[float]:
	"""Gets the specified axis limits."""
	...

def get_dearpygui_version() -> str:
	"""Returns the dearpygui version."""
	...

def get_delta_time() -> float:
	"""Returns time since last frame."""
	...

def get_drawing_mouse_pos() -> List[int]:
	"""Undocumented"""
	...

def get_file_dialog_info(file_dialog : int) -> dict:
	"""Undocumented function"""
	...

def get_frame_count() -> int:
	"""Get frame count."""
	...

def get_global_font_scale() -> float:
	"""Undocumented function"""
	...

def get_item_configuration(item : int) -> dict:
	"""Undocumented"""
	...

def get_item_info(item : int) -> dict:
	"""Undocumented"""
	...

def get_item_state(item : int) -> dict:
	"""Undocumented"""
	...

def get_item_types() -> dict:
	"""Undocumented"""
	...

def get_mouse_drag_delta() -> float:
	"""Undocumented"""
	...

def get_mouse_pos(*, local: bool =True) -> List[int]:
	"""Undocumented"""
	...

def get_plot_mouse_pos() -> List[int]:
	"""Undocumented"""
	...

def get_plot_query_area(plot : int) -> List[float]:
	"""Returns the last/current query area of the plot. (Requires plot 'query' kwarg to be enabled)"""
	...

def get_selected_links(node_editor : int) -> List[List[str]]:
	"""Undocumented"""
	...

def get_selected_nodes(node_editor : int) -> List[int]:
	"""Undocumented"""
	...

def get_total_time() -> float:
	"""Returns total time since Dear PyGui has started."""
	...

def get_value(item : int) -> Any:
	"""Undocumented"""
	...

def get_values(items : List[int]) -> Any:
	"""Undocumented"""
	...

def get_viewport_configuration(item : int) -> dict:
	"""Returns viewport configuration."""
	...

def get_windows() -> List[int]:
	"""Undocumented"""
	...

def get_x_scroll(item : int) -> float:
	"""Undocumented function"""
	...

def get_x_scroll_max(item : int) -> float:
	"""Undocumented function"""
	...

def get_y_scroll(item : int) -> float:
	"""Undocumented function"""
	...

def get_y_scroll_max(item : int) -> float:
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

def is_plot_queried(plot : int) -> bool:
	"""Returns true if the plot is currently being queried. (Requires plot 'query' kwarg to be enabled)"""
	...

def is_viewport_created() -> bool:
	"""Checks if a viewport has been created."""
	...

def last_container() -> int:
	"""Undocumented"""
	...

def last_item() -> int:
	"""Undocumented"""
	...

def last_root() -> int:
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

def move_item(item : int, *, parent: int =0, before: int =0) -> None:
	"""Undocumented"""
	...

def move_item_down(item : int) -> None:
	"""Undocumented"""
	...

def move_item_up(item : int) -> None:
	"""Undocumented"""
	...

def pop_container_stack() -> int:
	"""Undocumented"""
	...

def push_container_stack(item : int) -> bool:
	"""Undocumented"""
	...

def render_dearpygui_frame() -> None:
	"""Renders a dearpygui frame."""
	...

def reorder_items(container : int, slot : int, new_order : List[int]) -> None:
	"""Undocumented"""
	...

def reset_axis_ticks(axis : int) -> None:
	"""Removes the and manually set axis ticks and applys the default auto axis ticks."""
	...

def reset_default_theme() -> None:
	"""Resets to default theme."""
	...

def reset_pos(item : int) -> None:
	"""Undocumented"""
	...

def save_init_file(file : str) -> None:
	"""Save dpg.ini file."""
	...

def set_axis_limits(axis : int, ymin : float, ymax : float) -> None:
	"""Sets limits on the axis for pan and zoom."""
	...

def set_axis_limits_auto(axis : int) -> None:
	"""Removes all limits on specified axis."""
	...

def set_axis_ticks(axis : int, label_pairs : Any) -> None:
	"""Replaces axis ticks with 'label_pairs' argument"""
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

def set_item_children(item : int, source : int, slot : int) -> dict:
	"""Undocumented"""
	...

def set_item_disabled_theme(item : int, theme : int) -> None:
	"""Undocumented"""
	...

def set_item_font(item : int, font : int) -> None:
	"""Undocumented"""
	...

def set_item_theme(item : int, theme : int) -> None:
	"""Undocumented"""
	...

def set_item_type_disabled_theme(item : int, theme : int) -> None:
	"""Undocumented"""
	...

def set_item_type_theme(item : int, theme : int) -> None:
	"""Undocumented"""
	...

def set_primary_window(window : int, value : bool) -> None:
	"""Undocumented"""
	...

def set_staging_mode(mode : bool) -> None:
	"""Undocumented"""
	...

def set_start_callback(callback : Callable) -> str:
	"""Undocumented function"""
	...

def set_value(item : int, value : Any) -> None:
	"""Undocumented"""
	...

def set_viewport_resize_callback(callback : Callable) -> str:
	"""Undocumented function"""
	...

def set_x_scroll(item : int, value : float) -> None:
	"""Undocumented function"""
	...

def set_y_scroll(item : int, value : float) -> None:
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

def show_tool(tool : int) -> str:
	"""Undocumented function"""
	...

def show_viewport(viewport : str, *, minimized: bool =False, maximized: bool =False) -> None:
	"""Shows the viewport"""
	...

def split_frame(*, delay: int =32) -> None:
	"""Waits one frame."""
	...

def stage_items(items : List[int]) -> None:
	"""Undocumented"""
	...

def stop_dearpygui() -> None:
	"""Stops dearpygui."""
	...

def top_container_stack() -> int:
	"""Undocumented"""
	...

def unlock_mutex() -> None:
	"""Unlocks mutex"""
	...

def unstage_items(items : List[int]) -> None:
	"""Undocumented"""
	...

