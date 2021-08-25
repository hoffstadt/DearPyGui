from typing import List, Any, Callable, Union, Tuple
from dearpygui._dearpygui import *

##########################################################
# This file is generated automatically by mvPythonParser #
##########################################################

# ~ Dear PyGui Version: master
def add_2d_histogram_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', xbins: int ='', ybins: int ='', xmin_range: float ='', xmax_range: float ='', ymin_range: float ='', ymax_range: float ='', density: bool ='', outliers: bool ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_3d_slider(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float]] ='', max_x: float ='', max_y: float ='', max_z: float ='', min_x: float ='', min_y: float ='', min_z: float ='', scale: float ='') -> Union[int, str]:
	"""Adds a 3D box slider that allows a 3d point to be show in 2d represented cube space."""
	...

def add_alias(alias : str, item : Union[int, str]) -> None:
	"""Undocumented"""
	...

def add_area_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', fill: Union[List[int], Tuple[int]] ='', contribute_to_bounds: bool ='') -> Union[int, str]:
	"""Adds an area series to a plot."""
	...

def add_bar_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', weight: float ='', horizontal: bool ='') -> Union[int, str]:
	"""Adds a bar series to a plot."""
	...

def add_bool_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_button(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', small: bool ='', arrow: bool ='', direction: int ='') -> Union[int, str]:
	"""Adds a button."""
	...

def add_candle_series(dates : Union[List[float], Tuple[float]], opens : Union[List[float], Tuple[float]], closes : Union[List[float], Tuple[float]], lows : Union[List[float], Tuple[float]], highs : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', bull_color: Union[List[int], Tuple[int]] ='', bear_color: Union[List[int], Tuple[int]] ='', weight: int ='', tooltip: bool ='') -> Union[int, str]:
	"""Adds a candle series to a plot."""
	...

def add_char_remap(source : int, target : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_checkbox(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: bool ='') -> Union[int, str]:
	"""Adds a checkbox."""
	...

def add_child(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', border: bool ='', autosize_x: bool ='', autosize_y: bool ='', no_scrollbar: bool ='', horizontal_scrollbar: bool ='', menubar: bool ='') -> Union[int, str]:
	"""Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end."""
	...

def add_clipper(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', delay_search: bool ='') -> Union[int, str]:
	"""Helper to manually clip large list of items. Increases performance by not searching or drawing widgets outside of the clipped region."""
	...

def add_collapsing_header(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', closable: bool ='', default_open: bool ='', open_on_double_click: bool ='', open_on_arrow: bool ='', leaf: bool ='', bullet: bool ='') -> Union[int, str]:
	"""Adds a collapsing header to add items to. Must be closed with the end command."""
	...

def add_color_button(default_value : Union[List[int], Tuple[int]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', no_alpha: bool ='', no_border: bool ='', no_drag_drop: bool ='') -> Union[int, str]:
	"""Adds a color button."""
	...

def add_color_edit(default_value : Union[List[int], Tuple[int]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', no_alpha: bool ='', no_picker: bool ='', no_options: bool ='', no_small_preview: bool ='', no_inputs: bool ='', no_tooltip: bool ='', no_label: bool ='', no_drag_drop: bool ='', alpha_bar: bool ='', alpha_preview: int ='', display_mode: int ='', display_type: int ='', input_mode: int ='') -> Union[int, str]:
	"""Adds an RGBA color editor. Click the small color preview will provide a color picker. Click and draging the small color preview will copy the color to be applied on any other color widget."""
	...

def add_color_picker(default_value : Union[List[int], Tuple[int]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', no_alpha: bool ='', no_side_preview: bool ='', no_small_preview: bool ='', no_inputs: bool ='', no_tooltip: bool ='', no_label: bool ='', alpha_bar: bool ='', display_rgb: bool ='', display_hsv: bool ='', display_hex: bool ='', picker_mode: int ='', alpha_preview: int ='', display_type: int ='', input_mode: int ='') -> Union[int, str]:
	"""Adds an RGB color picker. Right click the color picker for options. Click and drag the color preview to copy the color and drop on any other color widget to apply. Right Click allows the style of the color picker to be changed."""
	...

def add_color_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Union[List[float], Tuple[float]] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_colormap(colors : List[List[int]], qualitative : bool, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a legend that pairs values with colors. This is typically used with a heat series. """
	...

def add_colormap_button(default_value : Union[List[int], Tuple[int]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', no_alpha: bool ='', no_border: bool ='', no_drag_drop: bool ='') -> Union[int, str]:
	"""Adds a color button."""
	...

def add_colormap_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a colormap registry."""
	...

def add_colormap_scale(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', colormap: Union[int, str] ='', min_scale: float ='', max_scale: float ='') -> Union[int, str]:
	"""Adds a legend that pairs values with colors. This is typically used with a heat series. """
	...

def add_colormap_slider(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='') -> Union[int, str]:
	"""Adds a color button."""
	...

def add_combo(items : Union[List[str], Tuple[str]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: str ='', popup_align_left: bool ='', no_arrow_button: bool ='', no_preview: bool ='', height_mode: int ='') -> Union[int, str]:
	"""Adds a combo dropdown that allows a user to select a single option from a drop down window."""
	...

def add_date_picker(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: dict ='', level: int ='') -> Union[int, str]:
	"""Creates a date picker."""
	...

def add_double4_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Any ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_double_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: float ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_drag_float(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', format: str ='', speed: float ='', min_value: float ='', max_value: float ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_floatx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float]] ='', size: int ='', format: str ='', speed: float ='', min_value: float ='', max_value: float ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_int(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: int ='', format: str ='', speed: float ='', min_value: int ='', max_value: int ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_intx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[int], Tuple[int]] ='', size: int ='', format: str ='', speed: float ='', min_value: int ='', max_value: int ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_line(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', callback: Callable ='', show: bool ='', default_value: Any ='', color: Union[List[int], Tuple[int]] ='', thickness: float ='', show_label: bool ='', vertical: bool ='') -> Union[int, str]:
	"""Adds a drag line to a plot."""
	...

def add_drag_payload(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', show: bool ='', drag_data: Any ='', payload_type: str ='') -> Union[int, str]:
	"""User data payload for drag and drop operations."""
	...

def add_drag_point(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', callback: Callable ='', show: bool ='', default_value: Any ='', color: Union[List[int], Tuple[int]] ='', thickness: float ='', show_label: bool ='') -> Union[int, str]:
	"""Adds a drag point to a plot."""
	...

def add_draw_layer(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Creates a layer that can be drawn to. Useful for grouping drawing items."""
	...

def add_drawlist(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='') -> Union[int, str]:
	"""A container widget that is used to present draw items or layers. Layers and draw items should be added to this widget as children."""
	...

def add_dummy(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='') -> Union[int, str]:
	"""Adds a spacer or 'dummy' object."""
	...

def add_dynamic_texture(width : int, height : int, default_value : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_error_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], negative : Union[List[float], Tuple[float]], positive : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', contribute_to_bounds: bool ='', horizontal: bool ='') -> Union[int, str]:
	"""Adds an error series to a plot."""
	...

def add_file_dialog(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', callback: Callable ='', show: bool ='', default_path: str ='', default_filename: str ='', file_count: int ='', modal: bool ='', directory_selector: bool ='') -> Union[int, str]:
	"""Displays a file or directory selector depending on keywords. Displays a file dialog by default."""
	...

def add_file_extension(extension : str, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', custom_text: str ='', color: Union[List[float], Tuple[float]] ='') -> Union[int, str]:
	"""Creates a file extension filter option in the file dialog. Only works when the parent is a file dialog."""
	...

def add_filter_set(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', delay_search: bool ='') -> Union[int, str]:
	"""Helper to parse and apply text filters (e.g. aaaaa[, bbbbb][, ccccc])"""
	...

def add_float4_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Union[List[float], Tuple[float]] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_float_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: float ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_float_vect_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Union[List[float], Tuple[float]] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_font(file : str, size : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_font_chars(chars : Union[List[int], Tuple[int]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_font_range(first_char : int, last_char : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_font_range_hint(hint : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_font_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_group(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', horizontal: bool ='', horizontal_spacing: float ='') -> Union[int, str]:
	"""Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members. Must be closed with the end command."""
	...

def add_handler_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler registry."""
	...

def add_heat_series(x : Union[List[float], Tuple[float]], rows : int, cols : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', scale_min: float ='', scale_max: float ='', bounds_min: Any ='', bounds_max: Any ='', format: str ='', contribute_to_bounds: bool ='') -> Union[int, str]:
	"""Adds a heat series to a plot. Typically a color scale widget is also added to show the legend."""
	...

def add_histogram_series(x : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', bins: int ='', bar_scale: float ='', min_range: float ='', max_range: float ='', cumlative: bool ='', density: bool ='', outliers: bool ='', contribute_to_bounds: bool ='') -> Union[int, str]:
	"""Adds a histogram series to a plot."""
	...

def add_hline_series(x : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', contribute_to_bounds: bool ='') -> Union[int, str]:
	"""Adds a infinite horizontal line series to a plot."""
	...

def add_image(texture_id : Union[int, str], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', tint_color: Union[List[float], Tuple[float]] ='', border_color: Union[List[float], Tuple[float]] ='', uv_min: Union[List[float], Tuple[float]] ='', uv_max: Union[List[float], Tuple[float]] ='') -> Union[int, str]:
	"""Adds an image from a specified texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) for texture coordinates will generally display the entire texture."""
	...

def add_image_button(texture_id : Union[int, str], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', frame_padding: int ='', tint_color: Union[List[float], Tuple[float]] ='', background_color: Union[List[float], Tuple[float]] ='', uv_min: Union[List[float], Tuple[float]] ='', uv_max: Union[List[float], Tuple[float]] ='') -> Union[int, str]:
	"""Adds an button with a texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) texture coordinates will generally display the entire texture"""
	...

def add_image_series(texture_id : Union[int, str], bounds_min : Union[List[float], Tuple[float]], bounds_max : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', uv_min: Union[List[float], Tuple[float]] ='', uv_max: Union[List[float], Tuple[float]] ='', tint_color: Union[List[int], Tuple[int]] ='') -> Union[int, str]:
	"""Adds a image series to a plot."""
	...

def add_input_float(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', format: str ='', min_value: float ='', max_value: float ='', step: float ='', step_fast: float ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds input for floats. Step buttons can be turned on or off."""
	...

def add_input_floatx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float]] ='', format: str ='', min_value: float ='', max_value: float ='', size: int ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds multi float input for up to 4 float values."""
	...

def add_input_int(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: int ='', min_value: int ='', max_value: int ='', step: int ='', step_fast: int ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds input for an int. Step buttons can be turned on or off."""
	...

def add_input_intx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[int], Tuple[int]] ='', min_value: int ='', max_value: int ='', size: int ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds multi int input for up to 4 integer values."""
	...

def add_input_text(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: str ='', hint: str ='', multiline: bool ='', no_spaces: bool ='', uppercase: bool ='', tab_input: bool ='', decimal: bool ='', hexadecimal: bool ='', readonly: bool ='', password: bool ='', scientific: bool ='', on_enter: bool ='') -> Union[int, str]:
	"""Adds input for text."""
	...

def add_int4_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Union[List[int], Tuple[int]] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_int_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: int ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_item_activated_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is activated."""
	...

def add_item_active_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is active."""
	...

def add_item_clicked_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is clicked."""
	...

def add_item_deactivated_after_edit_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is deactivated after edit."""
	...

def add_item_deactivated_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is deactivated."""
	...

def add_item_edited_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is edited."""
	...

def add_item_focus_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is focused."""
	...

def add_item_handler_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler registry."""
	...

def add_item_hover_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is hovered."""
	...

def add_item_pool(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_item_resize_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is resized."""
	...

def add_item_set(type : int, count : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_item_toggled_open_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is toggled open."""
	...

def add_item_visible_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified item is visible."""
	...

def add_key_down_handler(key : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified key is down. Parent must be a handler registry."""
	...

def add_key_press_handler(key : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified key is pressed. Parent must be a handler registry."""
	...

def add_key_release_handler(key : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified key is released. Parent must be a handler registry."""
	...

def add_knob_float(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', min_value: float ='', max_value: float ='') -> Union[int, str]:
	"""Adds a knob that rotates based of change in x mouse position."""
	...

def add_line_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a line series to a plot."""
	...

def add_listbox(items : Union[List[str], Tuple[str]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: str ='', num_items: int ='') -> Union[int, str]:
	"""Adds a listbox. If height is not large enought to show all items a scroll bar will appear."""
	...

def add_loading_indicator(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', style: int ='', circle_count: int ='', speed: float ='', radius: float ='', thickness: float ='', color: Union[List[int], Tuple[int]] ='', secondary_color: Union[List[int], Tuple[int]] ='') -> Union[int, str]:
	"""Adds a rotating anamated loding symbol."""
	...

def add_menu(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='') -> Union[int, str]:
	"""Adds a menu to an existing menu bar. Must be followed by a call to end."""
	...

def add_menu_bar(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', show: bool ='', delay_search: bool ='') -> Union[int, str]:
	"""Adds a menu bar to a window."""
	...

def add_menu_item(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: bool ='', shortcut: str ='', check: bool ='') -> Union[int, str]:
	"""Adds a menu item to an existing menu. Menu items act similar to selectables."""
	...

def add_mouse_click_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is clicked. Parent must be a handler registry."""
	...

def add_mouse_double_click_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is double clicked. Parent must be a handler registry."""
	...

def add_mouse_down_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is down. Parent must be a handler registry."""
	...

def add_mouse_drag_handler(button : int ='', threshold : float ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is clicked and dragged a set threshold. Parent must be a handler registry."""
	...

def add_mouse_move_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the mouse is moved. Parent must be a handler registry."""
	...

def add_mouse_release_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the specified mouse button is released. Parent must be a handler registry."""
	...

def add_mouse_wheel_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a handler which runs a given callback when the vertical mouse wheel is scrolled. Parent must be a handler registry."""
	...

def add_node(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', draggable: bool ='') -> Union[int, str]:
	"""Adds a node to a node editor."""
	...

def add_node_attribute(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', attribute_type: int ='', shape: int ='', category: str ='') -> Union[int, str]:
	"""Adds a node attribute."""
	...

def add_node_editor(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', delink_callback: Callable ='', menubar: bool ='') -> Union[int, str]:
	"""Adds a node editor."""
	...

def add_node_link(attr_1 : Union[int, str], attr_2 : Union[int, str], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a node link between nodes."""
	...

def add_pie_series(x : float, y : float, radius : float, values : Union[List[float], Tuple[float]], labels : Union[List[str], Tuple[str]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', format: str ='', angle: float ='', normalize: bool ='') -> Union[int, str]:
	"""Adds a pie series to a plot."""
	...

def add_plot(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', no_title: bool ='', no_menus: bool ='', no_box_select: bool ='', no_mouse_pos: bool ='', no_highlight: bool ='', no_child: bool ='', query: bool ='', crosshairs: bool ='', anti_aliased: bool ='', equal_aspects: bool ='', pan_button: int ='', pan_mod: int ='', fit_button: int ='', context_menu_button: int ='', box_select_button: int ='', box_select_mod: int ='', box_select_cancel_button: int ='', query_button: int ='', query_mod: int ='', query_toggle_mod: int ='', horizontal_mod: int ='', vertical_mod: int ='') -> Union[int, str]:
	"""Adds a plot which is used to hold series, and can be drawn to with draw commands."""
	...

def add_plot_annotation(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', default_value: Any ='', offset: Union[List[float], Tuple[float]] ='', color: Union[List[int], Tuple[int]] ='', clamped: bool ='') -> Union[int, str]:
	"""Adds an annotation to a plot."""
	...

def add_plot_axis(axis : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', no_gridlines: bool ='', no_tick_marks: bool ='', no_tick_labels: bool ='', log_scale: bool ='', invert: bool ='', lock_min: bool ='', lock_max: bool ='', time: bool ='') -> Union[int, str]:
	"""Adds a plot legend to a plot."""
	...

def add_plot_legend(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', location: int ='', horizontal: bool ='', outside: bool ='') -> Union[int, str]:
	"""Adds a plot legend to a plot."""
	...

def add_progress_bar(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', overlay: str ='', default_value: float ='') -> Union[int, str]:
	"""Adds a progress bar."""
	...

def add_radio_button(items : Union[List[str], Tuple[str]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: str ='', horizontal: bool ='') -> Union[int, str]:
	"""Adds a set of radio buttons. If items keyword is empty, nothing will be shown."""
	...

def add_raw_texture(width : int, height : int, default_value : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', format: int ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_same_line(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', xoffset: float ='', spacing: float ='') -> Union[int, str]:
	"""Places a widget on the same line as the previous widget. Can also be used for horizontal spacing."""
	...

def add_scatter_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a scatter series to a plot."""
	...

def add_selectable(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: bool ='', span_columns: bool ='') -> Union[int, str]:
	"""Adds a selectable."""
	...

def add_separator(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='') -> Union[int, str]:
	"""Adds a horizontal line."""
	...

def add_series_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Any ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_shade_series(x : Union[List[float], Tuple[float]], y1 : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', y2: Any ='') -> Union[int, str]:
	"""Adds a shade series to a plot."""
	...

def add_simple_plot(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float]] ='', overlay: str ='', histogram: bool ='', autosize: bool ='', min_scale: float ='', max_scale: float ='') -> Union[int, str]:
	"""A simple plot for visualization of a 1 dimensional set of values."""
	...

def add_slider_float(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', vertical: bool ='', no_input: bool ='', clamped: bool ='', min_value: float ='', max_value: float ='', format: str ='') -> Union[int, str]:
	"""Adds slider for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_floatx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float]] ='', size: int ='', no_input: bool ='', clamped: bool ='', min_value: float ='', max_value: float ='', format: str ='') -> Union[int, str]:
	"""Adds multi slider for up to 4 float values. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_int(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: int ='', vertical: bool ='', no_input: bool ='', clamped: bool ='', min_value: int ='', max_value: int ='', format: str ='') -> Union[int, str]:
	"""Adds slider for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_intx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[int], Tuple[int]] ='', size: int ='', no_input: bool ='', clamped: bool ='', min_value: int ='', max_value: int ='', format: str ='') -> Union[int, str]:
	"""Adds multi slider for up to 4 int values. CTRL+Click to directly modify the value."""
	...

def add_spacing(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', count: int ='') -> Union[int, str]:
	"""Adds vertical spacing."""
	...

def add_stage(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_stair_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a stair series to a plot."""
	...

def add_static_texture(width : int, height : int, default_value : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_stem_series(x : Union[List[float], Tuple[float]], y : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a stem series to a plot."""
	...

def add_string_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: str ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented"""
	...

def add_subplots(rows : int, columns : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', row_ratios: Union[List[float], Tuple[float]] ='', column_ratios: Union[List[float], Tuple[float]] ='', no_title: bool ='', no_menus: bool ='', no_resize: bool ='', no_align: bool ='', link_rows: bool ='', link_columns: bool ='', link_all_x: bool ='', link_all_y: bool ='', column_major: bool ='') -> Union[int, str]:
	"""Adds a plot which is used to hold series, and can be drawn to with draw commands."""
	...

def add_tab(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', closable: bool ='', no_tooltip: bool ='', order_mode: bool ='') -> Union[int, str]:
	"""Adds a tab to a tab bar. Must be closed with thes end command."""
	...

def add_tab_bar(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', reorderable: bool ='') -> Union[int, str]:
	"""Adds a tab bar."""
	...

def add_tab_button(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', no_reorder: bool ='', leading: bool ='', trailing: bool ='', no_tooltip: bool ='') -> Union[int, str]:
	"""Adds a tab button to a tab bar."""
	...

def add_table(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', header_row: bool ='', clipper: bool ='', inner_width: int ='', policy: int ='', freeze_rows: int ='', freeze_columns: int ='', sort_multi: bool ='', sort_tristate: bool ='', resizable: bool ='', reorderable: bool ='', hideable: bool ='', sortable: bool ='', context_menu_in_body: bool ='', row_background: bool ='', borders_innerH: bool ='', borders_outerH: bool ='', borders_innerV: bool ='', borders_outerV: bool ='', no_host_extendX: bool ='', no_host_extendY: bool ='', no_keep_columns_visible: bool ='', precise_widths: bool ='', no_clip: bool ='', pad_outerX: bool ='', no_pad_outerX: bool ='', no_pad_innerX: bool ='', scrollX: bool ='', scrollY: bool ='', no_saved_settings: bool ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_table_cell(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', height: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', filter_key: str ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_table_column(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', enabled: bool ='', init_width_or_weight: float ='', default_hide: bool ='', default_sort: bool ='', width_stretch: bool ='', width_fixed: bool ='', no_resize: bool ='', no_reorder: bool ='', no_hide: bool ='', no_clip: bool ='', no_sort: bool ='', no_sort_ascending: bool ='', no_sort_descending: bool ='', no_header_width: bool ='', prefer_sort_ascending: bool ='', prefer_sort_descending: bool ='', indent_enable: bool ='', indent_disable: bool ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_table_row(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', height: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', filter_key: str ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_template_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_text(default_value : str ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', wrap: int ='', bullet: bool ='', color: Union[List[float], Tuple[float]] ='', show_label: bool ='') -> Union[int, str]:
	"""Adds text. Text can have an optional label that will display to the right of the text."""
	...

def add_text_point(x : float, y : float, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', x_offset: int ='', y_offset: int ='', vertical: bool ='') -> Union[int, str]:
	"""Adds a labels series to a plot."""
	...

def add_texture_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_theme(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_theme_color(target : int ='', value : Union[List[int], Tuple[int]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', category: int ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_theme_style(target : int ='', x : float ='', y : float ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', category: int ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_time_picker(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: dict ='', hour24: bool ='') -> Union[int, str]:
	"""Adds a time picker."""
	...

def add_tooltip(parent : Union[int, str], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for."""
	...

def add_tree_node(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', default_open: bool ='', open_on_double_click: bool ='', open_on_arrow: bool ='', leaf: bool ='', bullet: bool ='', selectable: bool ='') -> Union[int, str]:
	"""Adds a tree node to add items to. Must be closed with the end command."""
	...

def add_value_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Undocumented function"""
	...

def add_viewport_drawlist(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='', filter_key: str ='', delay_search: bool ='', front: bool ='') -> Union[int, str]:
	"""A container that is used to present draw items or layers directly to the viewport. By default this will draw to the back of teh viewport. Layers and draw items should be added to this widget as children."""
	...

def add_viewport_menu_bar(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', show: bool ='', delay_search: bool ='') -> Union[int, str]:
	"""Adds a menu bar to the viewport."""
	...

def add_vline_series(x : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a infinite vertical line series to a plot."""
	...

def add_window(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', show: bool ='', pos: Union[List[int], Tuple[int]] ='', delay_search: bool ='', min_size: Union[List[int], Tuple[int]] ='', max_size: Union[List[int], Tuple[int]] ='', menubar: bool ='', collapsed: bool ='', autosize: bool ='', no_resize: bool ='', no_title_bar: bool ='', no_move: bool ='', no_scrollbar: bool ='', no_collapse: bool ='', horizontal_scrollbar: bool ='', no_focus_on_appearing: bool ='', no_bring_to_front_on_focus: bool ='', no_close: bool ='', no_background: bool ='', modal: bool ='', popup: bool ='', no_saved_settings: bool ='', on_close: Callable ='') -> Union[int, str]:
	"""Creates a new window for following items to be added to."""
	...

def bind_colormap(item : Union[int, str], source : Union[int, str]) -> None:
	"""Sets the color map for widgets that accept it."""
	...

def bind_font(font : Union[int, str]) -> None:
	"""Undocumented"""
	...

def bind_item_disabled_theme(item : Union[int, str], theme : Union[int, str]) -> None:
	"""Undocumented"""
	...

def bind_item_handler_registry(item : Union[int, str], handler_registry : Union[int, str]) -> None:
	"""Undocumented"""
	...

def bind_item_theme(item : Union[int, str], theme : Union[int, str]) -> None:
	"""Undocumented"""
	...

def bind_item_type_disabled_theme(item : int, theme : Union[int, str]) -> None:
	"""Undocumented"""
	...

def bind_item_type_theme(item : int, theme : Union[int, str]) -> None:
	"""Undocumented"""
	...

def bind_template_registry(template_registry : Union[int, str]) -> None:
	"""Undocumented"""
	...

def bind_theme(theme : Union[int, str]) -> None:
	"""Undocumented"""
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

def configure_app(*, docking: bool ='', docking_space: bool ='', load_init_file: str ='', init_file: str ='', device: int ='', auto_device: bool ='', allow_alias_overwrites: bool ='', manual_alias_management: bool ='', skip_required_args: bool ='', skip_positional_args: bool ='', skip_keyword_args: bool ='') -> None:
	"""Undocumented"""
	...

def configure_item(item : Union[int, str], **kwargs) -> None:
	"""Undocumented"""
	...

def configure_viewport(item : Union[int, str], **kwargs) -> None:
	"""Configures the viewport."""
	...

def create_viewport(*, title: str ='', small_icon: str ='', large_icon: str ='', width: int ='', height: int ='', x_pos: int ='', y_pos: int ='', min_width: int ='', max_width: int ='', min_height: int ='', max_height: int ='', resizable: bool ='', vsync: bool ='', always_on_top: bool ='', decorated: bool ='', clear_color: Union[List[float], Tuple[float]] ='') -> None:
	"""Creates a viewport."""
	...

def delete_item(item : Union[int, str], *, children_only: bool ='', slot: int ='') -> None:
	"""Undocumented"""
	...

def does_alias_exist(alias : str) -> bool:
	"""Undocumented"""
	...

def does_item_exist(item : Union[int, str]) -> bool:
	"""Undocumented"""
	...

def draw_arrow(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', thickness: float ='', size: int ='') -> Union[int, str]:
	"""Draws an arrow on a drawing."""
	...

def draw_bezier_cubic(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], p3 : Union[List[float], Tuple[float]], p4 : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', thickness: float ='', segments: int ='') -> Union[int, str]:
	"""Draws a cubic bezier curve on a drawing."""
	...

def draw_bezier_quadratic(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], p3 : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', thickness: float ='', segments: int ='') -> Union[int, str]:
	"""Draws a quadratic bezier curve on a drawing."""
	...

def draw_circle(center : Union[List[float], Tuple[float]], radius : float, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', fill: Union[List[int], Tuple[int]] ='', thickness: float ='', segments: int ='') -> Union[int, str]:
	"""Draws a circle on a drawing."""
	...

def draw_ellipse(pmin : Union[List[float], Tuple[float]], pmax : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', fill: Union[List[int], Tuple[int]] ='', thickness: float ='', segments: int ='') -> Union[int, str]:
	"""Draws an ellipse on a drawing."""
	...

def draw_image(texture_id : Union[int, str], pmin : Union[List[float], Tuple[float]], pmax : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', uv_min: Union[List[float], Tuple[float]] ='', uv_max: Union[List[float], Tuple[float]] ='', color: Union[List[int], Tuple[int]] ='') -> Union[int, str]:
	"""Draws an image on a drawing. p_min (top-left) and p_max (bottom-right) represent corners of the rectangle the image will be drawn to.Setting the p_min equal to the p_max will sraw the image to with 1:1 scale.uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using (0.0,0.0)->(1.0,1.0) texturecoordinates will generally display the entire texture."""
	...

def draw_line(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', thickness: float ='') -> Union[int, str]:
	"""Draws a line on a drawing."""
	...

def draw_polygon(points : List[List[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', fill: Union[List[int], Tuple[int]] ='', thickness: float ='') -> Union[int, str]:
	"""Draws a polygon on a drawing. First and and last point should be the same to close teh polygone."""
	...

def draw_polyline(points : List[List[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', closed: bool ='', color: Union[List[int], Tuple[int]] ='', thickness: float ='') -> Union[int, str]:
	"""Draws connected lines on a drawing from points."""
	...

def draw_quad(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], p3 : Union[List[float], Tuple[float]], p4 : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', fill: Union[List[int], Tuple[int]] ='', thickness: float ='') -> Union[int, str]:
	"""Draws a quad on a drawing."""
	...

def draw_rectangle(pmin : Union[List[float], Tuple[float]], pmax : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', color_upper_left: Union[List[int], Tuple[int]] ='', color_upper_right: Union[List[int], Tuple[int]] ='', color_bottom_right: Union[List[int], Tuple[int]] ='', color_bottom_left: Union[List[int], Tuple[int]] ='', fill: Union[List[int], Tuple[int]] ='', multicolor: bool ='', rounding: float ='', thickness: float ='') -> Union[int, str]:
	"""Draws a rectangle on a drawing."""
	...

def draw_text(pos : Union[List[float], Tuple[float]], text : str, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', size: float ='') -> Union[int, str]:
	"""Draws a text on a drawing."""
	...

def draw_triangle(p1 : Union[List[float], Tuple[float]], p2 : Union[List[float], Tuple[float]], p3 : Union[List[float], Tuple[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int]] ='', fill: Union[List[int], Tuple[int]] ='', thickness: float ='') -> Union[int, str]:
	"""Draws a triangle on a drawing."""
	...

def empty_container_stack() -> None:
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

def get_app_configuration() -> dict:
	"""Undocumented"""
	...

def get_axis_limits(axis : Union[int, str]) -> Union[List[float], Tuple[float]]:
	"""Gets the specified axis limits."""
	...

def get_colormap_color(colormap : Union[int, str], index : int) -> Union[List[int], Tuple[int]]:
	"""Returns a color from a colormap given an index >= 0 (modulo will be performed). This command can only be ran once the app is started."""
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

def get_frame_rate() -> float:
	"""Returns the average frame rate across 120 frames."""
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

def get_item_state(item : Union[int, str]) -> dict:
	"""Undocumented"""
	...

def get_item_types() -> dict:
	"""Undocumented"""
	...

def get_mouse_drag_delta() -> float:
	"""Undocumented"""
	...

def get_mouse_pos(*, local: bool ='') -> Union[List[int], Tuple[int]]:
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

def highlight_table_cell(table : Union[int, str], row : int, column : int, color : Union[List[int], Tuple[int]]) -> None:
	""""""
	...

def highlight_table_column(table : Union[int, str], column : int, color : Union[List[int], Tuple[int]]) -> None:
	""""""
	...

def highlight_table_row(table : Union[int, str], row : int, color : Union[List[int], Tuple[int]]) -> None:
	""""""
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

def is_table_cell_highlight(table : Union[int, str], row : int, column : int) -> bool:
	""""""
	...

def is_table_column_highlight(table : Union[int, str], column : int) -> bool:
	""""""
	...

def is_table_row_highlight(table : Union[int, str], row : int) -> bool:
	""""""
	...

def is_viewport_ok() -> bool:
	"""Checks if a viewport has been created and shown."""
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

def load_image(file : str, *, gamma: float ='', gamma_scale_factor: float ='') -> Any:
	"""Loads an image. Returns width, height, channels, mvBuffer"""
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

def move_item(item : Union[int, str], *, parent: Union[int, str] ='', before: Union[int, str] ='') -> None:
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

def set_exit_callback(callback : Callable) -> str:
	"""Undocumented function"""
	...

def set_global_font_scale(scale : float) -> None:
	"""Undocumented function"""
	...

def set_item_alias(item : Union[int, str], alias : str) -> None:
	"""Sets the item's alias."""
	...

def set_item_children(item : Union[int, str], source : Union[int, str], slot : int) -> dict:
	"""Undocumented"""
	...

def set_item_font(item : Union[int, str], font : Union[int, str]) -> None:
	"""Undocumented"""
	...

def set_primary_window(window : Union[int, str], value : bool) -> None:
	"""Undocumented"""
	...

def set_start_callback(callback : Callable) -> str:
	"""Undocumented function"""
	...

def set_table_row_color(table : Union[int, str], row : int, color : Union[List[int], Tuple[int]]) -> None:
	""""""
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

def setup_dearpygui() -> None:
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

def show_viewport(*, minimized: bool ='', maximized: bool ='') -> None:
	"""Shows the viewport"""
	...

def split_frame(*, delay: int ='') -> None:
	"""Waits one frame."""
	...

def stop_dearpygui() -> None:
	"""Stops dearpygui."""
	...

def top_container_stack() -> Union[int, str]:
	"""Undocumented"""
	...

def unhighlight_table_cell(table : Union[int, str], row : int, column : int) -> None:
	""""""
	...

def unhighlight_table_column(table : Union[int, str], column : int) -> None:
	""""""
	...

def unhighlight_table_row(table : Union[int, str], row : int) -> None:
	""""""
	...

def unlock_mutex() -> None:
	"""Unlocks mutex"""
	...

def unset_table_row_color(table : Union[int, str], row : int) -> None:
	""""""
	...

def unstage(item : Union[int, str]) -> None:
	"""Undocumented"""
	...

