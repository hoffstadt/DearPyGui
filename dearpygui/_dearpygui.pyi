from typing import List, Any, Callable, Union, Tuple
from dearpygui._dearpygui import *

##########################################################
# This file is generated automatically by mvPythonParser #
##########################################################

# ~ Dear PyGui Version: master
def add_2d_histogram_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', xbins: int ='', ybins: int ='', xmin_range: float ='', xmax_range: float ='', ymin_range: float ='', ymax_range: float ='', density: bool ='', outliers: bool ='', col_major: bool ='') -> Union[int, str]:
	"""Adds a 2d histogram series."""
	...

def add_3d_slider(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float, ...]] ='', max_x: float ='', max_y: float ='', max_z: float ='', min_x: float ='', min_y: float ='', min_z: float ='', scale: float ='') -> Union[int, str]:
	"""Adds a 3D box slider."""
	...

def add_alias(alias : str, item : Union[int, str]) -> None:
	"""Adds an alias."""
	...

def add_area_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', fill: Union[List[int], Tuple[int, ...]] ='', contribute_to_bounds: bool ='') -> Union[int, str]:
	"""Adds an area series to a plot."""
	...

def add_axis_tag(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', default_value: float ='', color: Union[List[int], Tuple[int, ...]] ='', auto_rounding: bool ='') -> Union[int, str]:
	"""Adds custom labels to axes."""
	...

def add_bar_group_series(values : Union[List[float], Tuple[float, ...]], label_ids : Union[List[str], Tuple[str, ...]], group_size : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', group_width: float ='', shift: int ='', horizontal: bool ='', stacked: bool ='') -> Union[int, str]:
	"""Adds a bar groups series to a plot."""
	...

def add_bar_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', weight: float ='', horizontal: bool ='') -> Union[int, str]:
	"""Adds a bar series to a plot."""
	...

def add_bool_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a bool value."""
	...

def add_button(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', small: bool ='', arrow: bool ='', direction: int ='', repeat: bool ='') -> Union[int, str]:
	"""Adds a button."""
	...

def add_candle_series(dates : Union[List[float], Tuple[float, ...]], opens : Union[List[float], Tuple[float, ...]], closes : Union[List[float], Tuple[float, ...]], lows : Union[List[float], Tuple[float, ...]], highs : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', bull_color: Union[List[int], Tuple[int, ...]] ='', bear_color: Union[List[int], Tuple[int, ...]] ='', weight: float ='', tooltip: bool ='', time_unit: int ='') -> Union[int, str]:
	"""Adds a candle series to a plot."""
	...

def add_char_remap(source : int, target : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Remaps a character."""
	...

def add_checkbox(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: bool ='') -> Union[int, str]:
	"""Adds a checkbox."""
	...

def add_child_window(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', border: bool ='', autosize_x: bool ='', autosize_y: bool ='', no_scrollbar: bool ='', horizontal_scrollbar: bool ='', menubar: bool ='', no_scroll_with_mouse: bool ='', flattened_navigation: bool ='', always_use_window_padding: bool ='', resizable_x: bool ='', resizable_y: bool ='', always_auto_resize: bool ='', frame_style: bool ='', auto_resize_x: bool ='', auto_resize_y: bool ='') -> Union[int, str]:
	"""Adds an embedded child window. Will show scrollbars when items do not fit. About using auto_resize/resizable flags: size measurement for a given axis is only performed when the child window is within visible boundaries, or is just appearing and it won't update its auto-size while clipped. While not perfect, it is a better default behavior as the always-on performance gain is more valuable than the occasional 'resizing after becoming visible again' glitch. You may also use always_auto_resize to force an update even when child window is not in view. However doing so will degrade performance. Remember that combining both auto_resize_x and auto_resize_y defeats purpose of a scrolling region and is NOT recommended."""
	...

def add_clipper(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', delay_search: bool ='') -> Union[int, str]:
	"""Helper to manually clip large list of items. Increases performance by not searching or drawing widgets outside of the clipped region."""
	...

def add_collapsing_header(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', closable: bool ='', default_open: bool ='', open_on_double_click: bool ='', open_on_arrow: bool ='', leaf: bool ='', bullet: bool ='') -> Union[int, str]:
	"""Adds a collapsing header to add items to. Must be closed with the end command."""
	...

def add_color_button(default_value : Union[List[int], Tuple[int, ...]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', no_alpha: bool ='', no_border: bool ='', no_drag_drop: bool ='') -> Union[int, str]:
	"""Adds a color button."""
	...

def add_color_edit(default_value : Union[List[int], Tuple[int, ...]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', no_alpha: bool ='', no_picker: bool ='', no_options: bool ='', no_small_preview: bool ='', no_inputs: bool ='', no_tooltip: bool ='', no_label: bool ='', no_drag_drop: bool ='', alpha_bar: bool ='', alpha_preview: int ='', display_mode: int ='', display_type: int ='', input_mode: int ='') -> Union[int, str]:
	"""Adds an RGBA color editor. Left clicking the small color preview will provide a color picker. Click and draging the small color preview will copy the color to be applied on any other color widget."""
	...

def add_color_picker(default_value : Union[List[int], Tuple[int, ...]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', no_alpha: bool ='', no_side_preview: bool ='', no_small_preview: bool ='', no_inputs: bool ='', no_tooltip: bool ='', no_label: bool ='', alpha_bar: bool ='', display_rgb: bool ='', display_hsv: bool ='', display_hex: bool ='', picker_mode: int ='', alpha_preview: int ='', display_type: int ='', input_mode: int ='') -> Union[int, str]:
	"""Adds an RGB color picker. Right click the color picker for options. Click and drag the color preview to copy the color and drop on any other color widget to apply. Right Click allows the style of the color picker to be changed."""
	...

def add_color_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Union[List[float], Tuple[float, ...]] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a color value."""
	...

def add_colormap(colors : List[Union[List[int], Tuple[int, ...]]], qualitative : bool, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a legend that pairs colors with normalized value 0.0->1.0. Each color will be  This is typically used with a heat series. (ex. [[0, 0, 0, 255], [255, 255, 255, 255]] will be mapped to a soft transition from 0.0-1.0)"""
	...

def add_colormap_button(default_value : Union[List[int], Tuple[int, ...]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='') -> Union[int, str]:
	"""Adds a button that a color map can be bound to."""
	...

def add_colormap_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a colormap registry."""
	...

def add_colormap_scale(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', colormap: Union[int, str] ='', min_scale: float ='', max_scale: float ='', format: str ='', reverse_dir: bool ='', mirror: bool ='') -> Union[int, str]:
	"""Adds a legend that pairs values with colors. This is typically used with a heat series. """
	...

def add_colormap_slider(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='') -> Union[int, str]:
	"""Adds a color slider that a color map can be bound to."""
	...

def add_combo(items : Union[List[str], Tuple[str, ...]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: str ='', popup_align_left: bool ='', no_arrow_button: bool ='', no_preview: bool ='', fit_width: bool ='', height_mode: int ='') -> Union[int, str]:
	"""Adds a combo dropdown that allows a user to select a single option from a drop down window. All items will be shown as selectables on the dropdown."""
	...

def add_custom_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], channel_count : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', callback: Callable ='', show: bool ='', y1: Any ='', y2: Any ='', y3: Any ='', tooltip: bool ='', no_fit: bool ='') -> Union[int, str]:
	"""Adds a custom series to a plot. New in 1.6."""
	...

def add_date_picker(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: dict ='', level: int ='') -> Union[int, str]:
	"""Adds a data picker."""
	...

def add_digital_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a digital series to a plot. Digital plots do not respond to y drag or zoom, and are always referenced to the bottom of the plot."""
	...

def add_double4_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Any ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a double value."""
	...

def add_double_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: float ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a double value."""
	...

def add_drag_double(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', format: str ='', speed: float ='', min_value: float ='', max_value: float ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag for a single double value. Useful when drag float is not accurate enough. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_doublex(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Any ='', size: int ='', format: str ='', speed: float ='', min_value: float ='', max_value: float ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag input for a set of double values up to 4. Useful when drag float is not accurate enough. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_float(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', format: str ='', speed: float ='', min_value: float ='', max_value: float ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_floatx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float, ...]] ='', size: int ='', format: str ='', speed: float ='', min_value: float ='', max_value: float ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag input for a set of float values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_int(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: int ='', format: str ='', speed: float ='', min_value: int ='', max_value: int ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_intx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[int], Tuple[int, ...]] ='', size: int ='', format: str ='', speed: float ='', min_value: int ='', max_value: int ='', no_input: bool ='', clamped: bool ='') -> Union[int, str]:
	"""Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_drag_line(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', callback: Callable ='', show: bool ='', default_value: float ='', color: Union[List[int], Tuple[int, ...]] ='', thickness: float ='', show_label: bool ='', vertical: bool ='', delayed: bool ='', no_cursor: bool ='', no_fit: bool ='', no_inputs: bool ='') -> Union[int, str]:
	"""Adds a drag line to a plot."""
	...

def add_drag_payload(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', show: bool ='', drag_data: Any ='', drop_data: Any ='', payload_type: str ='') -> Union[int, str]:
	"""User data payload for drag and drop operations."""
	...

def add_drag_point(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', callback: Callable ='', show: bool ='', default_value: Any ='', color: Union[List[int], Tuple[int, ...]] ='', thickness: float ='', show_label: bool ='', offset: Union[List[float], Tuple[float, ...]] ='', clamped: bool ='', delayed: bool ='', no_cursor: bool ='', no_fit: bool ='', no_inputs: bool ='') -> Union[int, str]:
	"""Adds a drag point to a plot."""
	...

def add_drag_rect(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', callback: Callable ='', show: bool ='', default_value: Any ='', color: Union[List[int], Tuple[int, ...]] ='', delayed: bool ='', no_cursor: bool ='', no_fit: bool ='', no_inputs: bool ='') -> Union[int, str]:
	"""Adds a drag rectangle to a plot."""
	...

def add_draw_layer(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', perspective_divide: bool ='', depth_clipping: bool ='', cull_mode: int ='') -> Union[int, str]:
	"""New in 1.1. Creates a layer useful for grouping drawlist items."""
	...

def add_draw_node(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""New in 1.1. Creates a drawing node to associate a transformation matrix. Child node matricies will concatenate."""
	...

def add_drawlist(width : int, height : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='') -> Union[int, str]:
	"""Adds a drawing canvas."""
	...

def add_dynamic_texture(width : int, height : int, default_value : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a dynamic texture."""
	...

def add_error_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], negative : Union[List[float], Tuple[float, ...]], positive : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', contribute_to_bounds: bool ='', horizontal: bool ='') -> Union[int, str]:
	"""Adds an error series to a plot."""
	...

def add_file_dialog(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', callback: Callable ='', show: bool ='', default_path: str ='', default_filename: str ='', file_count: int ='', modal: bool ='', directory_selector: bool ='', min_size: Union[List[int], Tuple[int, ...]] ='', max_size: Union[List[int], Tuple[int, ...]] ='', cancel_callback: Callable ='') -> Union[int, str]:
	"""Displays a file or directory selector depending on keywords. Displays a file dialog by default. Callback will be ran when the file or directory picker is closed. The app_data arguemnt will be populated with information related to the file and directory as a dictionary."""
	...

def add_file_extension(extension : str, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', custom_text: str ='', color: Union[List[int], Tuple[int, ...]] ='') -> Union[int, str]:
	"""Creates a file extension filter option in the file dialog."""
	...

def add_filter_set(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', delay_search: bool ='') -> Union[int, str]:
	"""Helper to parse and apply text filters (e.g. aaaaa[, bbbbb][, ccccc])"""
	...

def add_float4_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Union[List[float], Tuple[float, ...]] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a float4 value."""
	...

def add_float_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: float ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a float value."""
	...

def add_float_vect_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Union[List[float], Tuple[float, ...]] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a float vect value."""
	...

def add_font(file : str, size : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', pixel_snapH: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds font to a font registry."""
	...

def add_font_chars(chars : Union[List[int], Tuple[int, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds specific font characters to a font."""
	...

def add_font_range(first_char : int, last_char : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a range of font characters to a font."""
	...

def add_font_range_hint(hint : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a range of font characters (mvFontRangeHint_ constants)."""
	...

def add_font_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a font registry."""
	...

def add_group(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', horizontal: bool ='', horizontal_spacing: float ='', xoffset: float ='') -> Union[int, str]:
	"""Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.
Enable property acts in a special way enabling/disabling everything inside the group. (Use mvStyleVar_DisabledAlpha to edit colors within the disabled group.)"""
	...

def add_handler_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a handler registry."""
	...

def add_heat_series(x : Union[List[float], Tuple[float, ...]], rows : int, cols : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', scale_min: float ='', scale_max: float ='', bounds_min: Any ='', bounds_max: Any ='', format: str ='', contribute_to_bounds: bool ='', col_major: bool ='') -> Union[int, str]:
	"""Adds a heat series to a plot."""
	...

def add_histogram_series(x : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', bins: int ='', bar_scale: float ='', min_range: float ='', max_range: float ='', cumulative: bool ='', density: bool ='', outliers: bool ='', horizontal: bool ='', contribute_to_bounds: bool ='') -> Union[int, str]:
	"""Adds a histogram series to a plot."""
	...

def add_image(texture_tag : Union[int, str], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', tint_color: Union[List[float], Tuple[float, ...]] ='', border_color: Union[List[float], Tuple[float, ...]] ='', uv_min: Union[List[float], Tuple[float, ...]] ='', uv_max: Union[List[float], Tuple[float, ...]] ='') -> Union[int, str]:
	"""Adds an image from a specified texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) for texture coordinates will generally display the entire texture."""
	...

def add_image_button(texture_tag : Union[int, str], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', tint_color: Union[List[float], Tuple[float, ...]] ='', background_color: Union[List[float], Tuple[float, ...]] ='', uv_min: Union[List[float], Tuple[float, ...]] ='', uv_max: Union[List[float], Tuple[float, ...]] ='') -> Union[int, str]:
	"""Adds an button with a texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) texture coordinates will generally display the entire texture"""
	...

def add_image_series(texture_tag : Union[int, str], bounds_min : Union[List[float], Tuple[float, ...]], bounds_max : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', uv_min: Union[List[float], Tuple[float, ...]] ='', uv_max: Union[List[float], Tuple[float, ...]] ='', tint_color: Union[List[int], Tuple[int, ...]] ='') -> Union[int, str]:
	"""Adds an image series to a plot."""
	...

def add_inf_line_series(x : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', horizontal: bool ='') -> Union[int, str]:
	"""Adds an infinite line series to a plot."""
	...

def add_input_double(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', format: str ='', min_value: float ='', max_value: float ='', step: float ='', step_fast: float ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds input for an double. Useful when input float is not accurate enough. +/- buttons can be activated by setting the value of step."""
	...

def add_input_doublex(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Any ='', format: str ='', min_value: float ='', max_value: float ='', size: int ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds multi double input for up to 4 double values. Useful when input float mulit is not accurate enough."""
	...

def add_input_float(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', format: str ='', min_value: float ='', max_value: float ='', step: float ='', step_fast: float ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds input for an float. +/- buttons can be activated by setting the value of step."""
	...

def add_input_floatx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float, ...]] ='', format: str ='', min_value: float ='', max_value: float ='', size: int ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds multi float input for up to 4 float values."""
	...

def add_input_int(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: int ='', min_value: int ='', max_value: int ='', step: int ='', step_fast: int ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds input for an int. +/- buttons can be activated by setting the value of step."""
	...

def add_input_intx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[int], Tuple[int, ...]] ='', min_value: int ='', max_value: int ='', size: int ='', min_clamped: bool ='', max_clamped: bool ='', on_enter: bool ='', readonly: bool ='') -> Union[int, str]:
	"""Adds multi int input for up to 4 integer values."""
	...

def add_input_text(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: str ='', hint: str ='', multiline: bool ='', no_spaces: bool ='', uppercase: bool ='', tab_input: bool ='', decimal: bool ='', hexadecimal: bool ='', readonly: bool ='', password: bool ='', scientific: bool ='', on_enter: bool ='', auto_select_all: bool ='', ctrl_enter_for_new_line: bool ='', no_horizontal_scroll: bool ='', always_overwrite: bool ='', no_undo_redo: bool ='', escape_clears_all: bool ='') -> Union[int, str]:
	"""Adds input for text."""
	...

def add_int4_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Union[List[int], Tuple[int, ...]] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a int4 value."""
	...

def add_int_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: int ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a int value."""
	...

def add_item_activated_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a activated handler."""
	...

def add_item_active_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a active handler."""
	...

def add_item_clicked_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a clicked handler."""
	...

def add_item_deactivated_after_edit_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a deactivated after edit handler."""
	...

def add_item_deactivated_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a deactivated handler."""
	...

def add_item_double_clicked_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a double click handler."""
	...

def add_item_edited_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds an edited handler."""
	...

def add_item_focus_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a focus handler."""
	...

def add_item_handler_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds an item handler registry."""
	...

def add_item_hover_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a hover handler."""
	...

def add_item_resize_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a resize handler."""
	...

def add_item_toggled_open_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a togged open handler."""
	...

def add_item_visible_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', callback: Callable ='', show: bool ='') -> Union[int, str]:
	"""Adds a visible handler."""
	...

def add_key_down_handler(key : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a key down handler."""
	...

def add_key_press_handler(key : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a key press handler."""
	...

def add_key_release_handler(key : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a key release handler."""
	...

def add_knob_float(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', min_value: float ='', max_value: float ='') -> Union[int, str]:
	"""Adds a knob that rotates based on change in x mouse position."""
	...

def add_line_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', segments: bool ='', loop: bool ='', skip_nan: bool ='', no_clip: bool ='', shaded: bool ='') -> Union[int, str]:
	"""Adds a line series to a plot."""
	...

def add_listbox(items : Union[List[str], Tuple[str, ...]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: str ='', num_items: int ='') -> Union[int, str]:
	"""Adds a listbox. If height is not large enough to show all items a scroll bar will appear."""
	...

def add_loading_indicator(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', style: int ='', circle_count: int ='', speed: float ='', radius: float ='', thickness: float ='', color: Union[List[int], Tuple[int, ...]] ='', secondary_color: Union[List[int], Tuple[int, ...]] ='') -> Union[int, str]:
	"""Adds a rotating animated loading symbol."""
	...

def add_menu(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='') -> Union[int, str]:
	"""Adds a menu to an existing menu bar."""
	...

def add_menu_bar(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', show: bool ='', delay_search: bool ='') -> Union[int, str]:
	"""Adds a menu bar to a window."""
	...

def add_menu_item(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: bool ='', shortcut: str ='', check: bool ='') -> Union[int, str]:
	"""Adds a menu item to an existing menu. Menu items act similar to selectables and has a bool value. When placed in a menu the checkmark will reflect its value."""
	...

def add_mouse_click_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a mouse click handler."""
	...

def add_mouse_double_click_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a mouse double click handler."""
	...

def add_mouse_down_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a mouse down handler."""
	...

def add_mouse_drag_handler(button : int ='', threshold : float ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a mouse drag handler."""
	...

def add_mouse_move_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a mouse move handler."""
	...

def add_mouse_release_handler(button : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a mouse release handler."""
	...

def add_mouse_wheel_handler(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', callback: Callable ='', show: bool ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a mouse wheel handler."""
	...

def add_node(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', draggable: bool ='') -> Union[int, str]:
	"""Adds a node to a node editor."""
	...

def add_node_attribute(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', attribute_type: int ='', shape: int ='', category: str ='') -> Union[int, str]:
	"""Adds a node attribute to a node."""
	...

def add_node_editor(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', callback: Callable ='', show: bool ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', delink_callback: Callable ='', menubar: bool ='', minimap: bool ='', minimap_location: int ='') -> Union[int, str]:
	"""Adds a node editor."""
	...

def add_node_link(attr_1 : Union[int, str], attr_2 : Union[int, str], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a node link between 2 node attributes."""
	...

def add_pie_series(x : float, y : float, radius : float, values : Union[List[float], Tuple[float, ...]], labels : Union[List[str], Tuple[str, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', format: str ='', angle: float ='', normalize: bool ='', ignore_hidden: bool ='') -> Union[int, str]:
	"""Adds an pie series to a plot."""
	...

def add_plot(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', no_title: bool ='', no_menus: bool ='', no_box_select: bool ='', no_mouse_pos: bool ='', query: bool ='', query_color: Union[List[float], Tuple[float, ...]] ='', min_query_rects: int ='', max_query_rects: int ='', crosshairs: bool ='', equal_aspects: bool ='', no_inputs: bool ='', no_frame: bool ='', use_local_time: bool ='', use_ISO8601: bool ='', use_24hour_clock: bool ='', pan_button: int ='', pan_mod: int ='', context_menu_button: int ='', fit_button: int ='', box_select_button: int ='', box_select_mod: int ='', box_select_cancel_button: int ='', query_toggle_mod: int ='', horizontal_mod: int ='', vertical_mod: int ='', override_mod: int ='', zoom_mod: int ='', zoom_rate: int ='') -> Union[int, str]:
	"""Adds a plot which is used to hold series, and can be drawn to with draw commands. For all _mod parameters use mvKey_ModX enums, or mvKey_ModDisabled to disable the modifier."""
	...

def add_plot_annotation(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', default_value: Any ='', offset: Union[List[float], Tuple[float, ...]] ='', color: Union[List[int], Tuple[int, ...]] ='', clamped: bool ='') -> Union[int, str]:
	"""Adds an annotation to a plot."""
	...

def add_plot_axis(axis : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', payload_type: str ='', drop_callback: Callable ='', show: bool ='', no_label: bool ='', no_gridlines: bool ='', no_tick_marks: bool ='', no_tick_labels: bool ='', no_initial_fit: bool ='', no_menus: bool ='', no_side_switch: bool ='', no_highlight: bool ='', opposite: bool ='', foreground_grid: bool ='', tick_format: str ='', scale: int ='', invert: bool ='', auto_fit: bool ='', range_fit: bool ='', pan_stretch: bool ='', lock_min: bool ='', lock_max: bool ='') -> Union[int, str]:
	"""Adds an axis to a plot."""
	...

def add_plot_legend(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', payload_type: str ='', drop_callback: Callable ='', show: bool ='', location: int ='', horizontal: bool ='', sort: bool ='', outside: bool ='', no_highlight_item: bool ='', no_highlight_axis: bool ='', no_menus: bool ='', no_buttons: bool ='') -> Union[int, str]:
	"""Adds a plot legend to a plot."""
	...

def add_progress_bar(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', overlay: str ='', default_value: float ='') -> Union[int, str]:
	"""Adds a progress bar."""
	...

def add_radio_button(items : Union[List[str], Tuple[str, ...]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: str ='', horizontal: bool ='') -> Union[int, str]:
	"""Adds a set of radio buttons. If items keyword is empty, nothing will be shown."""
	...

def add_raw_texture(width : int, height : int, default_value : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', format: int ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a raw texture."""
	...

def add_scatter_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', no_clip: bool ='') -> Union[int, str]:
	"""Adds a scatter series to a plot."""
	...

def add_selectable(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: bool ='', span_columns: bool ='', disable_popup_close: bool ='') -> Union[int, str]:
	"""Adds a selectable. Similar to a button but can indicate its selected state."""
	...

def add_separator(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='') -> Union[int, str]:
	"""Adds a horizontal line separator. Use 'label' parameter to add text and mvStyleVar_SeparatorText* elements to style it."""
	...

def add_series_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: Any ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a plot series value."""
	...

def add_shade_series(x : Union[List[float], Tuple[float, ...]], y1 : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', y2: Any ='') -> Union[int, str]:
	"""Adds a shade series to a plot."""
	...

def add_simple_plot(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float, ...]] ='', overlay: str ='', histogram: bool ='', autosize: bool ='', min_scale: float ='', max_scale: float ='') -> Union[int, str]:
	"""Adds a simple plot for visualization of a 1 dimensional set of values."""
	...

def add_slider_double(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', vertical: bool ='', no_input: bool ='', clamped: bool ='', min_value: float ='', max_value: float ='', format: str ='') -> Union[int, str]:
	"""Adds slider for a single double value. Useful when slider float is not accurate enough. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_doublex(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Any ='', size: int ='', no_input: bool ='', clamped: bool ='', min_value: float ='', max_value: float ='', format: str ='') -> Union[int, str]:
	"""Adds multi slider for up to 4 double values. Usueful for when multi slide float is not accurate enough. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_float(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: float ='', vertical: bool ='', no_input: bool ='', clamped: bool ='', min_value: float ='', max_value: float ='', format: str ='') -> Union[int, str]:
	"""Adds slider for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_floatx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[float], Tuple[float, ...]] ='', size: int ='', no_input: bool ='', clamped: bool ='', min_value: float ='', max_value: float ='', format: str ='') -> Union[int, str]:
	"""Adds multi slider for up to 4 float values. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_int(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: int ='', vertical: bool ='', no_input: bool ='', clamped: bool ='', min_value: int ='', max_value: int ='', format: str ='') -> Union[int, str]:
	"""Adds slider for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_slider_intx(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', enabled: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: Union[List[int], Tuple[int, ...]] ='', size: int ='', no_input: bool ='', clamped: bool ='', min_value: int ='', max_value: int ='', format: str ='') -> Union[int, str]:
	"""Adds multi slider for up to 4 int values. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes."""
	...

def add_spacer(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='') -> Union[int, str]:
	"""Adds a spacer item that can be used to help with layouts or can be used as a placeholder item."""
	...

def add_stage(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Adds a stage."""
	...

def add_stair_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', pre_step: bool ='', shaded: bool ='') -> Union[int, str]:
	"""Adds a stair series to a plot."""
	...

def add_static_texture(width : int, height : int, default_value : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a static texture."""
	...

def add_stem_series(x : Union[List[float], Tuple[float, ...]], y : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', horizontal: bool ='') -> Union[int, str]:
	"""Adds a stem series to a plot."""
	...

def add_string_value(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', source: Union[int, str] ='', default_value: str ='', parent: Union[int, str] ='') -> Union[int, str]:
	"""Adds a string value."""
	...

def add_subplots(rows : int, columns : int, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', row_ratios: Union[List[float], Tuple[float, ...]] ='', column_ratios: Union[List[float], Tuple[float, ...]] ='', no_title: bool ='', no_menus: bool ='', no_resize: bool ='', no_align: bool ='', share_series: bool ='', link_rows: bool ='', link_columns: bool ='', link_all_x: bool ='', link_all_y: bool ='', column_major: bool ='') -> Union[int, str]:
	"""Adds a collection of plots."""
	...

def add_tab(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drop_callback: Callable ='', show: bool ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', closable: bool ='', no_tooltip: bool ='', order_mode: int ='') -> Union[int, str]:
	"""Adds a tab to a tab bar."""
	...

def add_tab_bar(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', reorderable: bool ='') -> Union[int, str]:
	"""Adds a tab bar."""
	...

def add_tab_button(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', no_reorder: bool ='', leading: bool ='', trailing: bool ='', no_tooltip: bool ='') -> Union[int, str]:
	"""Adds a tab button to a tab bar."""
	...

def add_table(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', header_row: bool ='', clipper: bool ='', inner_width: int ='', policy: int ='', freeze_rows: int ='', freeze_columns: int ='', sort_multi: bool ='', sort_tristate: bool ='', resizable: bool ='', reorderable: bool ='', hideable: bool ='', sortable: bool ='', context_menu_in_body: bool ='', row_background: bool ='', borders_innerH: bool ='', borders_outerH: bool ='', borders_innerV: bool ='', borders_outerV: bool ='', no_host_extendX: bool ='', no_host_extendY: bool ='', no_keep_columns_visible: bool ='', precise_widths: bool ='', no_clip: bool ='', pad_outerX: bool ='', no_pad_outerX: bool ='', no_pad_innerX: bool ='', scrollX: bool ='', scrollY: bool ='', no_saved_settings: bool ='') -> Union[int, str]:
	"""Adds a table."""
	...

def add_table_cell(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', height: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', filter_key: str ='') -> Union[int, str]:
	"""Adds a table."""
	...

def add_table_column(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', enabled: bool ='', init_width_or_weight: float ='', default_hide: bool ='', default_sort: bool ='', width_stretch: bool ='', width_fixed: bool ='', no_resize: bool ='', no_reorder: bool ='', no_hide: bool ='', no_clip: bool ='', no_sort: bool ='', no_sort_ascending: bool ='', no_sort_descending: bool ='', no_header_width: bool ='', prefer_sort_ascending: bool ='', prefer_sort_descending: bool ='', indent_enable: bool ='', indent_disable: bool ='', angled_header: bool ='', no_header_label: bool ='') -> Union[int, str]:
	"""Adds a table column."""
	...

def add_table_row(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', height: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', filter_key: str ='') -> Union[int, str]:
	"""Adds a table row."""
	...

def add_template_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Adds a template registry."""
	...

def add_text(default_value : str ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', wrap: int ='', bullet: bool ='', color: Union[List[int], Tuple[int, ...]] ='', show_label: bool ='') -> Union[int, str]:
	"""Adds text. Text can have an optional label that will display to the right of the text."""
	...

def add_text_point(x : float, y : float, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', source: Union[int, str] ='', show: bool ='', offset: Union[List[float], Tuple[float, ...]] ='', vertical: bool ='') -> Union[int, str]:
	"""Adds a label series to a plot. x and y can only have one elements each."""
	...

def add_texture_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='') -> Union[int, str]:
	"""Adds a dynamic texture."""
	...

def add_theme(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Adds a theme."""
	...

def add_theme_color(target : int ='', value : Union[List[int], Tuple[int, ...]] ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', category: int ='') -> Union[int, str]:
	"""Adds a theme color."""
	...

def add_theme_component(item_type : int ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', enabled_state: bool ='') -> Union[int, str]:
	"""Adds a theme component."""
	...

def add_theme_style(target : int ='', x : float ='', y : float ='', *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', category: int ='') -> Union[int, str]:
	"""Adds a theme style."""
	...

def add_time_picker(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', callback: Callable ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', tracked: bool ='', track_offset: float ='', default_value: dict ='', hour24: bool ='') -> Union[int, str]:
	"""Adds a time picker."""
	...

def add_tooltip(parent : Union[int, str], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='', delay: float ='', hide_on_activity: bool ='') -> Union[int, str]:
	"""Adds a tooltip window."""
	...

def add_tree_node(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', before: Union[int, str] ='', payload_type: str ='', drag_callback: Callable ='', drop_callback: Callable ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', filter_key: str ='', delay_search: bool ='', tracked: bool ='', track_offset: float ='', default_open: bool ='', open_on_double_click: bool ='', open_on_arrow: bool ='', leaf: bool ='', bullet: bool ='', selectable: bool ='', span_text_width: bool ='', span_full_width: bool ='') -> Union[int, str]:
	"""Adds a tree node to add items to."""
	...

def add_value_registry(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='') -> Union[int, str]:
	"""Adds a value registry."""
	...

def add_viewport_drawlist(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', show: bool ='', filter_key: str ='', delay_search: bool ='', front: bool ='') -> Union[int, str]:
	"""A container that is used to present draw items or layers directly to the viewport. By default this will draw to the back of the viewport. Layers and draw items should be added to this widget as children."""
	...

def add_viewport_menu_bar(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', indent: int ='', parent: Union[int, str] ='', show: bool ='', delay_search: bool ='') -> Union[int, str]:
	"""Adds a menubar to the viewport."""
	...

def add_window(*, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', width: int ='', height: int ='', indent: int ='', show: bool ='', pos: Union[List[int], Tuple[int, ...]] ='', delay_search: bool ='', min_size: Union[List[int], Tuple[int, ...]] ='', max_size: Union[List[int], Tuple[int, ...]] ='', menubar: bool ='', collapsed: bool ='', autosize: bool ='', no_resize: bool ='', unsaved_document: bool ='', no_title_bar: bool ='', no_move: bool ='', no_scrollbar: bool ='', no_collapse: bool ='', horizontal_scrollbar: bool ='', no_focus_on_appearing: bool ='', no_bring_to_front_on_focus: bool ='', no_close: bool ='', no_background: bool ='', modal: bool ='', popup: bool ='', no_saved_settings: bool ='', no_open_over_existing_popup: bool ='', no_scroll_with_mouse: bool ='', on_close: Callable ='') -> Union[int, str]:
	"""Creates a new window for following items to be added to."""
	...

def apply_transform(item : Union[int, str], transform : Any) -> None:
	"""New in 1.1. Applies a transformation matrix to a layer."""
	...

def bind_colormap(item : Union[int, str], source : Union[int, str]) -> None:
	"""Sets the color map for widgets that accept it."""
	...

def bind_font(font : Union[int, str]) -> Union[int, str]:
	"""Binds a global font."""
	...

def bind_item_font(item : Union[int, str], font : Union[int, str]) -> None:
	"""Sets an item's font."""
	...

def bind_item_handler_registry(item : Union[int, str], handler_registry : Union[int, str]) -> None:
	"""Binds an item handler registry to an item."""
	...

def bind_item_theme(item : Union[int, str], theme : Union[int, str]) -> None:
	"""Binds a theme to an item."""
	...

def bind_theme(theme : Union[int, str]) -> None:
	"""Binds a global theme."""
	...

def capture_next_item(callback : Callable, *, user_data: Any ='') -> None:
	"""Captures the next item."""
	...

def clear_selected_links(node_editor : Union[int, str]) -> None:
	"""Clears a node editor's selected links."""
	...

def clear_selected_nodes(node_editor : Union[int, str]) -> None:
	"""Clears a node editor's selected nodes."""
	...

def configure_app(*, load_init_file: str ='', docking: bool ='', docking_space: bool ='', docking_shift_only: bool ='', init_file: str ='', auto_save_init_file: bool ='', device: int ='', auto_device: bool ='', allow_alias_overwrites: bool ='', manual_alias_management: bool ='', skip_required_args: bool ='', skip_positional_args: bool ='', skip_keyword_args: bool ='', wait_for_input: bool ='', manual_callback_management: bool ='', keyboard_navigation: bool ='', anti_aliased_lines: bool ='', anti_aliased_lines_use_tex: bool ='', anti_aliased_fill: bool ='', **kwargs) -> None:
	"""Configures app."""
	...

def configure_item(item : Union[int, str], **kwargs) -> None:
	"""Configures an item."""
	...

def configure_viewport(item : Union[int, str], **kwargs) -> None:
	"""Configures a viewport."""
	...

def create_context() -> None:
	"""Creates the Dear PyGui context."""
	...

def create_fps_matrix(eye : Union[List[float], Tuple[float, ...]], pitch : float, yaw : float) -> Any:
	"""New in 1.1. Create a 'first person shooter' matrix."""
	...

def create_lookat_matrix(eye : Union[List[float], Tuple[float, ...]], target : Union[List[float], Tuple[float, ...]], up : Union[List[float], Tuple[float, ...]]) -> Any:
	"""New in 1.1. Creates a 'Look at matrix'."""
	...

def create_orthographic_matrix(left : float, right : float, bottom : float, top : float, zNear : float, zFar : float) -> Any:
	"""New in 1.1. Creates an orthographic matrix."""
	...

def create_perspective_matrix(fov : float, aspect : float, zNear : float, zFar : float) -> Any:
	"""New in 1.1. Creates a perspective matrix."""
	...

def create_rotation_matrix(angle : float, axis : Union[List[float], Tuple[float, ...]]) -> Any:
	"""New in 1.1. Applies a transformation matrix to a layer."""
	...

def create_scale_matrix(scales : Union[List[float], Tuple[float, ...]]) -> Any:
	"""New in 1.1. Applies a transformation matrix to a layer."""
	...

def create_translation_matrix(translation : Union[List[float], Tuple[float, ...]]) -> Any:
	"""New in 1.1. Creates a translation matrix."""
	...

def create_viewport(*, title: str ='', small_icon: str ='', large_icon: str ='', width: int ='', height: int ='', x_pos: int ='', y_pos: int ='', min_width: int ='', max_width: int ='', min_height: int ='', max_height: int ='', resizable: bool ='', vsync: bool ='', always_on_top: bool ='', decorated: bool ='', clear_color: Union[List[float], Tuple[float, ...]] ='', disable_close: bool ='') -> None:
	"""Creates a viewport. Viewports are required."""
	...

def delete_item(item : Union[int, str], *, children_only: bool ='', slot: int ='') -> None:
	"""Deletes an item.."""
	...

def destroy_context() -> None:
	"""Destroys the Dear PyGui context."""
	...

def does_alias_exist(alias : str) -> bool:
	"""Checks if an alias exist."""
	...

def does_item_exist(item : Union[int, str]) -> bool:
	"""Checks if an item exist.."""
	...

def draw_arrow(p1 : Union[List[float], Tuple[float, ...]], p2 : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', thickness: float ='', size: int ='') -> Union[int, str]:
	"""Adds an arrow."""
	...

def draw_bezier_cubic(p1 : Union[List[float], Tuple[float, ...]], p2 : Union[List[float], Tuple[float, ...]], p3 : Union[List[float], Tuple[float, ...]], p4 : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', thickness: float ='', segments: int ='') -> Union[int, str]:
	"""Adds a cubic bezier curve."""
	...

def draw_bezier_quadratic(p1 : Union[List[float], Tuple[float, ...]], p2 : Union[List[float], Tuple[float, ...]], p3 : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', thickness: float ='', segments: int ='') -> Union[int, str]:
	"""Adds a quadratic bezier curve."""
	...

def draw_circle(center : Union[List[float], Tuple[float, ...]], radius : float, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', fill: Union[List[int], Tuple[int, ...]] ='', thickness: float ='', segments: int ='') -> Union[int, str]:
	"""Adds a circle"""
	...

def draw_ellipse(pmin : Union[List[float], Tuple[float, ...]], pmax : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', fill: Union[List[int], Tuple[int, ...]] ='', thickness: float ='', segments: int ='') -> Union[int, str]:
	"""Adds an ellipse."""
	...

def draw_image(texture_tag : Union[int, str], pmin : Union[List[float], Tuple[float, ...]], pmax : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', uv_min: Union[List[float], Tuple[float, ...]] ='', uv_max: Union[List[float], Tuple[float, ...]] ='', color: Union[List[int], Tuple[int, ...]] ='') -> Union[int, str]:
	"""Adds an image (for a drawing)."""
	...

def draw_image_quad(texture_tag : Union[int, str], p1 : Union[List[float], Tuple[float, ...]], p2 : Union[List[float], Tuple[float, ...]], p3 : Union[List[float], Tuple[float, ...]], p4 : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', uv1: Union[List[float], Tuple[float, ...]] ='', uv2: Union[List[float], Tuple[float, ...]] ='', uv3: Union[List[float], Tuple[float, ...]] ='', uv4: Union[List[float], Tuple[float, ...]] ='', color: Union[List[int], Tuple[int, ...]] ='') -> Union[int, str]:
	"""Adds an image (for a drawing)."""
	...

def draw_line(p1 : Union[List[float], Tuple[float, ...]], p2 : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', thickness: float ='') -> Union[int, str]:
	"""Adds a line."""
	...

def draw_polygon(points : List[List[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', fill: Union[List[int], Tuple[int, ...]] ='', thickness: float ='') -> Union[int, str]:
	"""Adds a polygon."""
	...

def draw_polyline(points : List[List[float]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', closed: bool ='', color: Union[List[int], Tuple[int, ...]] ='', thickness: float ='') -> Union[int, str]:
	"""Adds a polyline."""
	...

def draw_quad(p1 : Union[List[float], Tuple[float, ...]], p2 : Union[List[float], Tuple[float, ...]], p3 : Union[List[float], Tuple[float, ...]], p4 : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', fill: Union[List[int], Tuple[int, ...]] ='', thickness: float ='') -> Union[int, str]:
	"""Adds a quad."""
	...

def draw_rectangle(pmin : Union[List[float], Tuple[float, ...]], pmax : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', fill: Union[List[int], Tuple[int, ...]] ='', multicolor: bool ='', rounding: float ='', thickness: float ='', corner_colors: Any ='') -> Union[int, str]:
	"""Adds a rectangle."""
	...

def draw_text(pos : Union[List[float], Tuple[float, ...]], text : str, *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', size: float ='') -> Union[int, str]:
	"""Adds text (drawlist)."""
	...

def draw_triangle(p1 : Union[List[float], Tuple[float, ...]], p2 : Union[List[float], Tuple[float, ...]], p3 : Union[List[float], Tuple[float, ...]], *, label: str ='', user_data: Any ='', use_internal_label: bool ='', tag: Union[int, str] ='', parent: Union[int, str] ='', before: Union[int, str] ='', show: bool ='', color: Union[List[int], Tuple[int, ...]] ='', fill: Union[List[int], Tuple[int, ...]] ='', thickness: float ='') -> Union[int, str]:
	"""Adds a triangle."""
	...

def empty_container_stack() -> None:
	"""Emptyes the container stack."""
	...

def fit_axis_data(axis : Union[int, str]) -> None:
	"""Sets the axis boundaries max/min in the data series currently on the plot."""
	...

def focus_item(item : Union[int, str]) -> None:
	"""Focuses an item."""
	...

def generate_uuid() -> Union[int, str]:
	"""Generate a new UUID."""
	...

def get_active_window() -> Union[int, str]:
	"""Returns the active window."""
	...

def get_alias_id(alias : str) -> Union[int, str]:
	"""Returns the ID associated with an alias."""
	...

def get_aliases() -> Union[List[str], Tuple[str, ...]]:
	"""Returns all aliases."""
	...

def get_all_items() -> Union[List[int], Tuple[int, ...]]:
	"""Returns all items."""
	...

def get_app_configuration() -> dict:
	"""Returns app configuration."""
	...

def get_axis_limits(axis : Union[int, str]) -> Union[List[float], Tuple[float, ...]]:
	"""Get the specified axis limits."""
	...

def get_callback_queue() -> Any:
	"""New in 1.2. Returns and clears callback queue."""
	...

def get_clipboard_text() -> str:
	"""New in 1.3. Gets the clipboard text."""
	...

def get_colormap_color(colormap : Union[int, str], index : int) -> Union[List[int], Tuple[int, ...]]:
	"""Returns a color from a colormap given an index >= 0. (ex. 0 will be the first color in the color list of the color map) Modulo will be performed against the number of items in the color list."""
	...

def get_delta_time() -> float:
	"""Returns time since last frame."""
	...

def get_drawing_mouse_pos() -> Union[List[int], Tuple[int, ...]]:
	"""Returns mouse position in drawing."""
	...

def get_file_dialog_info(file_dialog : Union[int, str]) -> dict:
	"""Returns information related to the file dialog. Typically used while the file dialog is in use to query data about the state or info related to the file dialog."""
	...

def get_focused_item() -> Union[int, str]:
	"""Returns the item currently having focus."""
	...

def get_frame_count() -> int:
	"""Returns frame count."""
	...

def get_frame_rate() -> float:
	"""Returns the average frame rate across 120 frames."""
	...

def get_global_font_scale() -> float:
	"""Returns global font scale."""
	...

def get_item_alias(item : Union[int, str]) -> str:
	"""Returns an item's alias."""
	...

def get_item_configuration(item : Union[int, str]) -> dict:
	"""Returns an item's configuration."""
	...

def get_item_info(item : Union[int, str]) -> dict:
	"""Returns an item's information."""
	...

def get_item_state(item : Union[int, str]) -> dict:
	"""Returns an item's state."""
	...

def get_item_types() -> dict:
	"""Returns an item types."""
	...

def get_mouse_drag_delta() -> float:
	"""Returns mouse drag delta."""
	...

def get_mouse_pos(*, local: bool ='') -> Union[List[int], Tuple[int, ...]]:
	"""Returns mouse position."""
	...

def get_platform() -> int:
	"""New in 1.6. Returns platform constant."""
	...

def get_plot_mouse_pos() -> Union[List[int], Tuple[int, ...]]:
	"""Returns mouse position in plot."""
	...

def get_plot_query_rects(plot : Union[int, str]) -> List[List[float]]:
	"""Returns the query rects of the plot. Returns an array of array containing the top-left coordinates and bottom-right coordinates of the plot area."""
	...

def get_selected_links(node_editor : Union[int, str]) -> List[List[str]]:
	"""Returns a node editor's selected links."""
	...

def get_selected_nodes(node_editor : Union[int, str]) -> Union[List[int], Tuple[int, ...]]:
	"""Returns a node editor's selected nodes."""
	...

def get_text_size(text : str, *, wrap_width: float ='', font: Union[int, str] ='') -> Union[List[float], Tuple[float, ...]]:
	"""Returns width/height of text with specified font (must occur after 1st frame)."""
	...

def get_total_time() -> float:
	"""Returns total time since Dear PyGui has started."""
	...

def get_value(item : Union[int, str]) -> Any:
	"""Returns an item's value."""
	...

def get_values(items : Union[List[int], Tuple[int, ...]]) -> Any:
	"""Returns values of a list of items."""
	...

def get_viewport_configuration(item : Union[int, str]) -> dict:
	"""Returns a viewport's configuration."""
	...

def get_windows() -> Union[List[int], Tuple[int, ...]]:
	"""Returns all windows."""
	...

def get_x_scroll(item : Union[int, str]) -> float:
	"""Undocumented"""
	...

def get_x_scroll_max(item : Union[int, str]) -> float:
	"""Undocumented"""
	...

def get_y_scroll(item : Union[int, str]) -> float:
	"""Undocumented"""
	...

def get_y_scroll_max(item : Union[int, str]) -> float:
	"""Undocumented"""
	...

def highlight_table_cell(table : Union[int, str], row : int, column : int, color : Union[List[int], Tuple[int, ...]]) -> None:
	"""Highlight specified table cell."""
	...

def highlight_table_column(table : Union[int, str], column : int, color : Union[List[int], Tuple[int, ...]]) -> None:
	"""Highlight specified table column."""
	...

def highlight_table_row(table : Union[int, str], row : int, color : Union[List[int], Tuple[int, ...]]) -> None:
	"""Highlight specified table row."""
	...

def is_dearpygui_running() -> bool:
	"""Checks if Dear PyGui is running"""
	...

def is_key_down(key : int) -> bool:
	"""Checks if key is down."""
	...

def is_key_pressed(key : int) -> bool:
	"""Checks if key is pressed."""
	...

def is_key_released(key : int) -> bool:
	"""Checks if key is released."""
	...

def is_mouse_button_clicked(button : int) -> bool:
	"""Checks if mouse button is clicked."""
	...

def is_mouse_button_double_clicked(button : int) -> bool:
	"""Checks if mouse button is double clicked."""
	...

def is_mouse_button_down(button : int) -> bool:
	"""Checks if mouse button is down."""
	...

def is_mouse_button_dragging(button : int, threshold : float) -> bool:
	"""Checks if mouse button is down and dragging."""
	...

def is_mouse_button_released(button : int) -> bool:
	"""Checks if mouse button is released."""
	...

def is_table_cell_highlighted(table : Union[int, str], row : int, column : int) -> bool:
	"""Checks if a table cell is highlighted."""
	...

def is_table_column_highlighted(table : Union[int, str], column : int) -> bool:
	"""Checks if a table column is highlighted."""
	...

def is_table_row_highlighted(table : Union[int, str], row : int) -> bool:
	"""Checks if a table row is highlighted."""
	...

def is_viewport_ok() -> bool:
	"""Checks if a viewport has been created and shown."""
	...

def last_container() -> Union[int, str]:
	"""Returns the last container item added."""
	...

def last_item() -> Union[int, str]:
	"""Returns the last item added."""
	...

def last_root() -> Union[int, str]:
	"""Returns the last root added (registry or window)."""
	...

def load_image(file : str, *, gamma: float ='', gamma_scale_factor: float ='') -> Any:
	"""Loads an image. Returns width, height, channels, mvBuffer"""
	...

def lock_mutex() -> None:
	"""Locks render thread mutex."""
	...

def maximize_viewport() -> None:
	"""Maximizes the viewport."""
	...

def minimize_viewport() -> None:
	"""Minimizes a viewport."""
	...

def move_item(item : Union[int, str], *, parent: Union[int, str] ='', before: Union[int, str] ='') -> None:
	"""Moves an item to a new location."""
	...

def move_item_down(item : Union[int, str]) -> None:
	"""Moves an item down."""
	...

def move_item_up(item : Union[int, str]) -> None:
	"""Moves an item up."""
	...

def output_frame_buffer(file : str ='', *, callback: Callable ='') -> Any:
	"""Outputs frame buffer as a png if file is specified or through the second argument of a callback if specified. Render loop must have been started."""
	...

def pop_container_stack() -> Union[int, str]:
	"""Pops the top item off the parent stack and return its ID."""
	...

def push_container_stack(item : Union[int, str]) -> bool:
	"""Pushes an item onto the container stack."""
	...

def remove_alias(alias : str) -> None:
	"""Removes an alias."""
	...

def render_dearpygui_frame() -> None:
	"""Render a single Dear PyGui frame."""
	...

def reorder_items(container : Union[int, str], slot : int, new_order : Union[List[int], Tuple[int, ...]]) -> None:
	"""Reorders an item's children."""
	...

def reset_axis_limits_constraints(axis : Union[int, str]) -> None:
	"""Remove an axis' limits constraints"""
	...

def reset_axis_ticks(axis : Union[int, str]) -> None:
	"""Removes the manually set axis ticks and applies the default axis ticks"""
	...

def reset_axis_zoom_constraints(axis : Union[int, str]) -> None:
	"""Remove an axis' zoom constraints"""
	...

def reset_pos(item : Union[int, str]) -> None:
	"""Resets an item's position after using 'set_item_pos'."""
	...

def sample_colormap(colormap : Union[int, str], t : float) -> Union[List[int], Tuple[int, ...]]:
	"""Returns a color from a colormap given t between 0.0-1.0."""
	...

def save_image(file : str, width : int, height : int, data : Any, *, components: int ='', quality: int ='') -> None:
	"""Saves an image. Possible formats: png, bmp, tga, hdr, jpg."""
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

def set_axis_limits_constraints(axis : Union[int, str], vmin : float, vmax : float) -> None:
	"""Sets an axis' limits constraints so that users can't pan beyond a min or max value"""
	...

def set_axis_ticks(axis : Union[int, str], label_pairs : Any) -> None:
	"""Replaces axis ticks with 'label_pairs' argument."""
	...

def set_axis_zoom_constraints(axis : Union[int, str], vmin : float, vmax : float) -> None:
	"""Sets an axis' zoom constraints so that users can't zoom beyond a min or max value"""
	...

def set_clip_space(item : Union[int, str], top_left_x : float, top_left_y : float, width : float, height : float, min_depth : float, max_depth : float) -> None:
	"""New in 1.1. Set the clip space for depth clipping and 'viewport' transformation."""
	...

def set_clipboard_text(text : str) -> None:
	"""New in 1.3. Sets the clipboard text."""
	...

def set_exit_callback(callback : Callable, *, user_data: Any ='') -> str:
	"""Sets a callback to run on last frame."""
	...

def set_frame_callback(frame : int, callback : Callable, *, user_data: Any ='') -> str:
	"""Sets a callback to run on first frame."""
	...

def set_global_font_scale(scale : float) -> None:
	"""Sets global font scale."""
	...

def set_item_alias(item : Union[int, str], alias : str) -> None:
	"""Sets an item's alias."""
	...

def set_item_children(item : Union[int, str], source : Union[int, str], slot : int) -> None:
	"""Sets an item's children."""
	...

def set_primary_window(window : Union[int, str], value : bool) -> None:
	"""Sets the primary window."""
	...

def set_table_row_color(table : Union[int, str], row : int, color : Union[List[int], Tuple[int, ...]]) -> None:
	"""Set table row color."""
	...

def set_value(item : Union[int, str], value : Any) -> None:
	"""Set's an item's value."""
	...

def set_viewport_resize_callback(callback : Callable, *, user_data: Any ='') -> str:
	"""Sets a callback to run on viewport resize."""
	...

def set_x_scroll(item : Union[int, str], value : float) -> None:
	"""Undocumented"""
	...

def set_y_scroll(item : Union[int, str], value : float) -> None:
	"""Undocumented"""
	...

def setup_dearpygui() -> None:
	"""Sets up Dear PyGui"""
	...

def show_imgui_demo() -> None:
	"""Shows the imgui demo."""
	...

def show_implot_demo() -> None:
	"""Shows the implot demo."""
	...

def show_item_debug(item : Union[int, str]) -> None:
	"""Shows an item's debug window"""
	...

def show_tool(tool : Union[int, str]) -> str:
	"""Shows a built in tool."""
	...

def show_viewport(*, minimized: bool ='', maximized: bool ='') -> None:
	"""Shows the main viewport."""
	...

def split_frame(*, delay: int ='') -> None:
	"""Waits one frame."""
	...

def stop_dearpygui() -> None:
	"""Stops Dear PyGui"""
	...

def toggle_viewport_fullscreen() -> None:
	"""Toggle viewport fullscreen mode.."""
	...

def top_container_stack() -> Union[int, str]:
	"""Returns the item on the top of the container stack."""
	...

def unhighlight_table_cell(table : Union[int, str], row : int, column : int) -> None:
	"""Unhighlight specified table cell."""
	...

def unhighlight_table_column(table : Union[int, str], column : int) -> None:
	"""Unhighlight specified table column."""
	...

def unhighlight_table_row(table : Union[int, str], row : int) -> None:
	"""Unhighlight specified table row."""
	...

def unlock_mutex() -> None:
	"""Unlocks render thread mutex"""
	...

def unset_table_row_color(table : Union[int, str], row : int) -> None:
	"""Remove user set table row color."""
	...

def unstage(item : Union[int, str]) -> None:
	"""Unstages an item."""
	...

mvGraphicsBackend_D3D11=0
mvGraphicsBackend_D3D12=0
mvGraphicsBackend_VULKAN=0
mvGraphicsBackend_METAL=0
mvGraphicsBackend_OPENGL=0
mvMouseButton_Left=0
mvMouseButton_Right=0
mvMouseButton_Middle=0
mvMouseButton_X1=0
mvMouseButton_X2=0
mvKey_ModDisabled=0
mvKey_None=0
mvKey_0=0
mvKey_1=0
mvKey_2=0
mvKey_3=0
mvKey_4=0
mvKey_5=0
mvKey_6=0
mvKey_7=0
mvKey_8=0
mvKey_9=0
mvKey_A=0
mvKey_B=0
mvKey_C=0
mvKey_D=0
mvKey_E=0
mvKey_F=0
mvKey_G=0
mvKey_H=0
mvKey_I=0
mvKey_J=0
mvKey_K=0
mvKey_L=0
mvKey_M=0
mvKey_N=0
mvKey_O=0
mvKey_P=0
mvKey_Q=0
mvKey_R=0
mvKey_S=0
mvKey_T=0
mvKey_U=0
mvKey_V=0
mvKey_W=0
mvKey_X=0
mvKey_Y=0
mvKey_Z=0
mvKey_Back=0
mvKey_Tab=0
mvKey_Return=0
mvKey_LShift=0
mvKey_RShift=0
mvKey_LControl=0
mvKey_RControl=0
mvKey_LAlt=0
mvKey_RAlt=0
mvKey_Pause=0
mvKey_CapsLock=0
mvKey_Escape=0
mvKey_Spacebar=0
mvKey_End=0
mvKey_Home=0
mvKey_Left=0
mvKey_Up=0
mvKey_Right=0
mvKey_Down=0
mvKey_Print=0
mvKey_Insert=0
mvKey_Delete=0
mvKey_NumPad0=0
mvKey_NumPad1=0
mvKey_NumPad2=0
mvKey_NumPad3=0
mvKey_NumPad4=0
mvKey_NumPad5=0
mvKey_NumPad6=0
mvKey_NumPad7=0
mvKey_NumPad8=0
mvKey_NumPad9=0
mvKey_NumPadEnter=0
mvKey_NumPadEqual=0
mvKey_Subtract=0
mvKey_Decimal=0
mvKey_Divide=0
mvKey_Multiply=0
mvKey_Add=0
mvKey_F1=0
mvKey_F2=0
mvKey_F3=0
mvKey_F4=0
mvKey_F5=0
mvKey_F6=0
mvKey_F7=0
mvKey_F8=0
mvKey_F9=0
mvKey_F10=0
mvKey_F11=0
mvKey_F12=0
mvKey_F13=0
mvKey_F14=0
mvKey_F15=0
mvKey_F16=0
mvKey_F17=0
mvKey_F18=0
mvKey_F19=0
mvKey_F20=0
mvKey_F21=0
mvKey_F22=0
mvKey_F23=0
mvKey_F24=0
mvKey_NumLock=0
mvKey_ScrollLock=0
mvKey_Period=0
mvKey_Slash=0
mvKey_Backslash=0
mvKey_Open_Brace=0
mvKey_Close_Brace=0
mvKey_Browser_Back=0
mvKey_Browser_Forward=0
mvKey_Comma=0
mvKey_Minus=0
mvKey_Menu=0
mvKey_ModSuper=0
mvKey_ModShift=0
mvKey_ModAlt=0
mvKey_ModCtrl=0
mvKey_Clear=0
mvKey_Prior=0
mvKey_Next=0
mvKey_Select=0
mvKey_Execute=0
mvKey_LWin=0
mvKey_RWin=0
mvKey_Apps=0
mvKey_Sleep=0
mvKey_Help=0
mvKey_Browser_Refresh=0
mvKey_Browser_Stop=0
mvKey_Browser_Search=0
mvKey_Browser_Favorites=0
mvKey_Browser_Home=0
mvKey_Volume_Mute=0
mvKey_Volume_Down=0
mvKey_Volume_Up=0
mvKey_Media_Next_Track=0
mvKey_Media_Prev_Track=0
mvKey_Media_Stop=0
mvKey_Media_Play_Pause=0
mvKey_Launch_Mail=0
mvKey_Launch_Media_Select=0
mvKey_Launch_App1=0
mvKey_Launch_App2=0
mvKey_Colon=0
mvKey_Plus=0
mvKey_Tilde=0
mvKey_Quote=0
mvKey_F25=0
mvAll=0
mvTool_About=0
mvTool_Debug=0
mvTool_Doc=0
mvTool_ItemRegistry=0
mvTool_Metrics=0
mvTool_Stack=0
mvTool_Style=0
mvTool_Font=0
mvFontAtlas=0
mvAppUUID=0
mvInvalidUUID=0
mvDir_None=0
mvDir_Left=0
mvDir_Right=0
mvDir_Up=0
mvDir_Down=0
mvComboHeight_Small=0
mvComboHeight_Regular=0
mvComboHeight_Large=0
mvComboHeight_Largest=0
mvPlatform_Windows=0
mvPlatform_Apple=0
mvPlatform_Linux=0
mvColorEdit_AlphaPreviewNone=0
mvColorEdit_AlphaPreview=0
mvColorEdit_AlphaPreviewHalf=0
mvColorEdit_uint8=0
mvColorEdit_float=0
mvColorEdit_rgb=0
mvColorEdit_hsv=0
mvColorEdit_hex=0
mvColorEdit_input_rgb=0
mvColorEdit_input_hsv=0
mvPlotColormap_Default=0
mvPlotColormap_Deep=0
mvPlotColormap_Dark=0
mvPlotColormap_Pastel=0
mvPlotColormap_Paired=0
mvPlotColormap_Viridis=0
mvPlotColormap_Plasma=0
mvPlotColormap_Hot=0
mvPlotColormap_Cool=0
mvPlotColormap_Pink=0
mvPlotColormap_Jet=0
mvPlotColormap_Twilight=0
mvPlotColormap_RdBu=0
mvPlotColormap_BrBG=0
mvPlotColormap_PiYG=0
mvPlotColormap_Spectral=0
mvPlotColormap_Greys=0
mvColorPicker_bar=0
mvColorPicker_wheel=0
mvTabOrder_Reorderable=0
mvTabOrder_Fixed=0
mvTabOrder_Leading=0
mvTabOrder_Trailing=0
mvTimeUnit_Us=0
mvTimeUnit_Ms=0
mvTimeUnit_S=0
mvTimeUnit_Min=0
mvTimeUnit_Hr=0
mvTimeUnit_Day=0
mvTimeUnit_Mo=0
mvTimeUnit_Yr=0
mvDatePickerLevel_Day=0
mvDatePickerLevel_Month=0
mvDatePickerLevel_Year=0
mvCullMode_None=0
mvCullMode_Back=0
mvCullMode_Front=0
mvFontRangeHint_Default=0
mvFontRangeHint_Japanese=0
mvFontRangeHint_Korean=0
mvFontRangeHint_Chinese_Full=0
mvFontRangeHint_Chinese_Simplified_Common=0
mvFontRangeHint_Cyrillic=0
mvFontRangeHint_Thai=0
mvFontRangeHint_Vietnamese=0
mvNode_PinShape_Circle=0
mvNode_PinShape_CircleFilled=0
mvNode_PinShape_Triangle=0
mvNode_PinShape_TriangleFilled=0
mvNode_PinShape_Quad=0
mvNode_PinShape_QuadFilled=0
mvNode_Attr_Input=0
mvNode_Attr_Output=0
mvNode_Attr_Static=0
mvPlotBin_Sqrt=0
mvPlotBin_Sturges=0
mvPlotBin_Rice=0
mvPlotBin_Scott=0
mvXAxis=0
mvXAxis2=0
mvXAxis3=0
mvYAxis=0
mvYAxis2=0
mvYAxis3=0
mvPlotScale_Linear=0
mvPlotScale_Time=0
mvPlotScale_Log10=0
mvPlotScale_SymLog=0
mvPlotMarker_None=0
mvPlotMarker_Circle=0
mvPlotMarker_Square=0
mvPlotMarker_Diamond=0
mvPlotMarker_Up=0
mvPlotMarker_Down=0
mvPlotMarker_Left=0
mvPlotMarker_Right=0
mvPlotMarker_Cross=0
mvPlotMarker_Plus=0
mvPlotMarker_Asterisk=0
mvPlot_Location_Center=0
mvPlot_Location_North=0
mvPlot_Location_South=0
mvPlot_Location_West=0
mvPlot_Location_East=0
mvPlot_Location_NorthWest=0
mvPlot_Location_NorthEast=0
mvPlot_Location_SouthWest=0
mvPlot_Location_SouthEast=0
mvNodeMiniMap_Location_BottomLeft=0
mvNodeMiniMap_Location_BottomRight=0
mvNodeMiniMap_Location_TopLeft=0
mvNodeMiniMap_Location_TopRight=0
mvTable_SizingFixedFit=0
mvTable_SizingFixedSame=0
mvTable_SizingStretchProp=0
mvTable_SizingStretchSame=0
mvFormat_Float_rgba=0
mvFormat_Float_rgb=0
mvThemeCat_Core=0
mvThemeCat_Plots=0
mvThemeCat_Nodes=0
mvThemeCol_Text=0
mvThemeCol_TextDisabled=0
mvThemeCol_WindowBg=0
mvThemeCol_ChildBg=0
mvThemeCol_Border=0
mvThemeCol_PopupBg=0
mvThemeCol_BorderShadow=0
mvThemeCol_FrameBg=0
mvThemeCol_FrameBgHovered=0
mvThemeCol_FrameBgActive=0
mvThemeCol_TitleBg=0
mvThemeCol_TitleBgActive=0
mvThemeCol_TitleBgCollapsed=0
mvThemeCol_MenuBarBg=0
mvThemeCol_ScrollbarBg=0
mvThemeCol_ScrollbarGrab=0
mvThemeCol_ScrollbarGrabHovered=0
mvThemeCol_ScrollbarGrabActive=0
mvThemeCol_CheckMark=0
mvThemeCol_SliderGrab=0
mvThemeCol_SliderGrabActive=0
mvThemeCol_Button=0
mvThemeCol_ButtonHovered=0
mvThemeCol_ButtonActive=0
mvThemeCol_Header=0
mvThemeCol_HeaderHovered=0
mvThemeCol_HeaderActive=0
mvThemeCol_Separator=0
mvThemeCol_SeparatorHovered=0
mvThemeCol_SeparatorActive=0
mvThemeCol_ResizeGrip=0
mvThemeCol_ResizeGripHovered=0
mvThemeCol_ResizeGripActive=0
mvThemeCol_Tab=0
mvThemeCol_TabHovered=0
mvThemeCol_TabActive=0
mvThemeCol_TabUnfocused=0
mvThemeCol_TabUnfocusedActive=0
mvThemeCol_DockingPreview=0
mvThemeCol_DockingEmptyBg=0
mvThemeCol_PlotLines=0
mvThemeCol_PlotLinesHovered=0
mvThemeCol_PlotHistogram=0
mvThemeCol_PlotHistogramHovered=0
mvThemeCol_TableHeaderBg=0
mvThemeCol_TableBorderStrong=0
mvThemeCol_TableBorderLight=0
mvThemeCol_TableRowBg=0
mvThemeCol_TableRowBgAlt=0
mvThemeCol_TextSelectedBg=0
mvThemeCol_DragDropTarget=0
mvThemeCol_NavHighlight=0
mvThemeCol_NavWindowingHighlight=0
mvThemeCol_NavWindowingDimBg=0
mvThemeCol_ModalWindowDimBg=0
mvPlotCol_Line=0
mvPlotCol_Fill=0
mvPlotCol_MarkerOutline=0
mvPlotCol_MarkerFill=0
mvPlotCol_ErrorBar=0
mvPlotCol_FrameBg=0
mvPlotCol_PlotBg=0
mvPlotCol_PlotBorder=0
mvPlotCol_LegendBg=0
mvPlotCol_LegendBorder=0
mvPlotCol_LegendText=0
mvPlotCol_TitleText=0
mvPlotCol_InlayText=0
mvPlotCol_AxisBg=0
mvPlotCol_AxisBgActive=0
mvPlotCol_AxisBgHovered=0
mvPlotCol_AxisGrid=0
mvPlotCol_AxisText=0
mvPlotCol_Selection=0
mvPlotCol_Crosshairs=0
mvNodeCol_NodeBackground=0
mvNodeCol_NodeBackgroundHovered=0
mvNodeCol_NodeBackgroundSelected=0
mvNodeCol_NodeOutline=0
mvNodeCol_TitleBar=0
mvNodeCol_TitleBarHovered=0
mvNodeCol_TitleBarSelected=0
mvNodeCol_Link=0
mvNodeCol_LinkHovered=0
mvNodeCol_LinkSelected=0
mvNodeCol_Pin=0
mvNodeCol_PinHovered=0
mvNodeCol_BoxSelector=0
mvNodeCol_BoxSelectorOutline=0
mvNodeCol_GridBackground=0
mvNodeCol_GridLine=0
mvNodesCol_GridLinePrimary=0
mvNodesCol_MiniMapBackground=0
mvNodesCol_MiniMapBackgroundHovered=0
mvNodesCol_MiniMapOutline=0
mvNodesCol_MiniMapOutlineHovered=0
mvNodesCol_MiniMapNodeBackground=0
mvNodesCol_MiniMapNodeBackgroundHovered=0
mvNodesCol_MiniMapNodeBackgroundSelected=0
mvNodesCol_MiniMapNodeOutline=0
mvNodesCol_MiniMapLink=0
mvNodesCol_MiniMapLinkSelected=0
mvNodesCol_MiniMapCanvas=0
mvNodesCol_MiniMapCanvasOutline=0
mvStyleVar_Alpha=0
mvStyleVar_DisabledAlpha=0
mvStyleVar_WindowPadding=0
mvStyleVar_WindowRounding=0
mvStyleVar_WindowBorderSize=0
mvStyleVar_WindowMinSize=0
mvStyleVar_WindowTitleAlign=0
mvStyleVar_ChildRounding=0
mvStyleVar_ChildBorderSize=0
mvStyleVar_PopupRounding=0
mvStyleVar_PopupBorderSize=0
mvStyleVar_FramePadding=0
mvStyleVar_FrameRounding=0
mvStyleVar_FrameBorderSize=0
mvStyleVar_ItemSpacing=0
mvStyleVar_ItemInnerSpacing=0
mvStyleVar_IndentSpacing=0
mvStyleVar_CellPadding=0
mvStyleVar_ScrollbarSize=0
mvStyleVar_ScrollbarRounding=0
mvStyleVar_GrabMinSize=0
mvStyleVar_GrabRounding=0
mvStyleVar_TabRounding=0
mvStyleVar_TabBorderSize=0
mvStyleVar_TabBarBorderSize=0
mvStyleVar_TableAngledHeadersAngle=0
mvStyleVar_TableAngledHeadersTextAlign=0
mvStyleVar_ButtonTextAlign=0
mvStyleVar_SelectableTextAlign=0
mvStyleVar_SeparatorTextBorderSize=0
mvStyleVar_SeparatorTextAlign=0
mvStyleVar_SeparatorTextPadding=0
mvStyleVar_DockingSeparatorSize=0
mvPlotStyleVar_LineWeight=0
mvPlotStyleVar_Marker=0
mvPlotStyleVar_MarkerSize=0
mvPlotStyleVar_MarkerWeight=0
mvPlotStyleVar_FillAlpha=0
mvPlotStyleVar_ErrorBarSize=0
mvPlotStyleVar_ErrorBarWeight=0
mvPlotStyleVar_DigitalBitHeight=0
mvPlotStyleVar_DigitalBitGap=0
mvPlotStyleVar_PlotBorderSize=0
mvPlotStyleVar_MinorAlpha=0
mvPlotStyleVar_MajorTickLen=0
mvPlotStyleVar_MinorTickLen=0
mvPlotStyleVar_MajorTickSize=0
mvPlotStyleVar_MinorTickSize=0
mvPlotStyleVar_MajorGridSize=0
mvPlotStyleVar_MinorGridSize=0
mvPlotStyleVar_PlotPadding=0
mvPlotStyleVar_LabelPadding=0
mvPlotStyleVar_LegendPadding=0
mvPlotStyleVar_LegendInnerPadding=0
mvPlotStyleVar_LegendSpacing=0
mvPlotStyleVar_MousePosPadding=0
mvPlotStyleVar_AnnotationPadding=0
mvPlotStyleVar_FitPadding=0
mvPlotStyleVar_PlotDefaultSize=0
mvPlotStyleVar_PlotMinSize=0
mvNodeStyleVar_GridSpacing=0
mvNodeStyleVar_NodeCornerRounding=0
mvNodeStyleVar_NodePadding=0
mvNodeStyleVar_NodeBorderThickness=0
mvNodeStyleVar_LinkThickness=0
mvNodeStyleVar_LinkLineSegmentsPerLength=0
mvNodeStyleVar_LinkHoverDistance=0
mvNodeStyleVar_PinCircleRadius=0
mvNodeStyleVar_PinQuadSideLength=0
mvNodeStyleVar_PinTriangleSideLength=0
mvNodeStyleVar_PinLineThickness=0
mvNodeStyleVar_PinHoverRadius=0
mvNodeStyleVar_PinOffset=0
mvNodesStyleVar_MiniMapPadding=0
mvNodesStyleVar_MiniMapOffset=0
mvInputText=0
mvButton=0
mvRadioButton=0
mvTabBar=0
mvTab=0
mvImage=0
mvMenuBar=0
mvViewportMenuBar=0
mvMenu=0
mvMenuItem=0
mvChildWindow=0
mvGroup=0
mvSliderFloat=0
mvSliderInt=0
mvFilterSet=0
mvDragFloat=0
mvDragInt=0
mvInputFloat=0
mvInputInt=0
mvColorEdit=0
mvClipper=0
mvColorPicker=0
mvTooltip=0
mvCollapsingHeader=0
mvSeparator=0
mvCheckbox=0
mvListbox=0
mvText=0
mvCombo=0
mvPlot=0
mvSimplePlot=0
mvDrawlist=0
mvWindowAppItem=0
mvSelectable=0
mvTreeNode=0
mvProgressBar=0
mvSpacer=0
mvImageButton=0
mvTimePicker=0
mvDatePicker=0
mvColorButton=0
mvFileDialog=0
mvTabButton=0
mvDrawNode=0
mvNodeEditor=0
mvNode=0
mvNodeAttribute=0
mvTable=0
mvTableColumn=0
mvTableRow=0
mvDrawLine=0
mvDrawArrow=0
mvDrawTriangle=0
mvDrawImageQuad=0
mvDrawCircle=0
mvDrawEllipse=0
mvDrawBezierCubic=0
mvDrawBezierQuadratic=0
mvDrawQuad=0
mvDrawRect=0
mvDrawText=0
mvDrawPolygon=0
mvDrawPolyline=0
mvDrawImage=0
mvDragFloatMulti=0
mvDragIntMulti=0
mvSliderFloatMulti=0
mvSliderIntMulti=0
mvInputIntMulti=0
mvInputFloatMulti=0
mvDragPoint=0
mvDragLine=0
mvDragRect=0
mvAnnotation=0
mvAxisTag=0
mvLineSeries=0
mvScatterSeries=0
mvStemSeries=0
mvStairSeries=0
mvBarSeries=0
mvBarGroupSeries=0
mvErrorSeries=0
mvInfLineSeries=0
mvHeatSeries=0
mvImageSeries=0
mvPieSeries=0
mvShadeSeries=0
mvLabelSeries=0
mvHistogramSeries=0
mvDigitalSeries=0
mv2dHistogramSeries=0
mvCandleSeries=0
mvAreaSeries=0
mvColorMapScale=0
mvSlider3D=0
mvKnobFloat=0
mvLoadingIndicator=0
mvNodeLink=0
mvTextureRegistry=0
mvStaticTexture=0
mvDynamicTexture=0
mvStage=0
mvDrawLayer=0
mvViewportDrawlist=0
mvFileExtension=0
mvPlotLegend=0
mvPlotAxis=0
mvHandlerRegistry=0
mvKeyDownHandler=0
mvKeyPressHandler=0
mvKeyReleaseHandler=0
mvMouseMoveHandler=0
mvMouseWheelHandler=0
mvMouseClickHandler=0
mvMouseDoubleClickHandler=0
mvMouseDownHandler=0
mvMouseReleaseHandler=0
mvMouseDragHandler=0
mvHoverHandler=0
mvActiveHandler=0
mvFocusHandler=0
mvVisibleHandler=0
mvEditedHandler=0
mvActivatedHandler=0
mvDeactivatedHandler=0
mvDeactivatedAfterEditHandler=0
mvToggledOpenHandler=0
mvClickedHandler=0
mvDoubleClickedHandler=0
mvDragPayload=0
mvResizeHandler=0
mvFont=0
mvFontRegistry=0
mvTheme=0
mvThemeColor=0
mvThemeStyle=0
mvThemeComponent=0
mvFontRangeHint=0
mvFontRange=0
mvFontChars=0
mvCharRemap=0
mvValueRegistry=0
mvIntValue=0
mvFloatValue=0
mvFloat4Value=0
mvInt4Value=0
mvBoolValue=0
mvStringValue=0
mvDoubleValue=0
mvDouble4Value=0
mvColorValue=0
mvFloatVectValue=0
mvSeriesValue=0
mvRawTexture=0
mvSubPlots=0
mvColorMap=0
mvColorMapRegistry=0
mvColorMapButton=0
mvColorMapSlider=0
mvTemplateRegistry=0
mvTableCell=0
mvItemHandlerRegistry=0
mvInputDouble=0
mvInputDoubleMulti=0
mvDragDouble=0
mvDragDoubleMulti=0
mvSliderDouble=0
mvSliderDoubleMulti=0
mvCustomSeries=0
mvReservedUUID_0=0
mvReservedUUID_1=0
mvReservedUUID_2=0
mvReservedUUID_3=0
mvReservedUUID_4=0
mvReservedUUID_5=0
mvReservedUUID_6=0
mvReservedUUID_7=0
mvReservedUUID_8=0
mvReservedUUID_9=0
mvReservedUUID_10=0
