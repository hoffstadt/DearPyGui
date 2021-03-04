from typing import List, Any, Callable
from dearpygui.core import *

##########################################################
# This file is generated automatically by mvPythonParser #
##########################################################

# ~ Dear PyGui Version: master
def add_about_window(name: str, *, width: int = -1, height: int = -1, x_pos: int = 200, y_pos: int = 200, autosize: bool = False, no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False, no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False, no_bring_to_front_on_focus: bool = False, no_close: bool = False, no_background: bool = False, label: str = '', show: bool = True) -> None:
	"""Creates an about window."""
	...

def add_additional_font(file: str, size: float = 13.0, glyph_ranges: str = '', *, custom_glyph_chars: List[int] = (), custom_glyph_ranges: Any = List[List[int]]) -> None:
	"""Adds additional font."""
	...

def add_annotation(plot: str, text: str, x: float, y: float, xoffset: float, yoffset: float, *, color: List[float] = (0, 0, 0, -1), clamped: bool = True, tag: str = '') -> None:
	"""Adds an annotation to a plot."""
	...

def add_area_series(plot: str, name: str, x: List[float], y: List[float], color: List[float], fill: List[float], *, weight: float = 1.0, update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds an area series to a plot."""
	...

def add_bar_series(plot: str, name: str, x: List[float], y: List[float], *, weight: float = 1.0, horizontal: bool = False, update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a bar series to a plot."""
	...

def add_button(name: str, *, small: bool = False, arrow: bool = False, direction: int = 0, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', width: int = 0, height: int = 0, label: str = '', show: bool = True, enabled: bool = True) -> None:
	"""Adds a button."""
	...

def add_candle_series(plot: str, name: str, date: List[float], opens: List[float], highs: List[float], lows: List[float], closes: List[float], *, tooltip: bool = True, bull_color: List[float] = (0, 255, 113, 255), bear_color: List[float] = (218, 13, 79, 255), weight: float = 0.25, update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a candle series to a plot."""
	...

def add_character_remap(destination: int, source: int) -> None:
	"""Remaps characters."""
	...

def add_checkbox(name: str, *, default_value: int = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', label: str = '', show: bool = True, enabled: bool = True) -> None:
	"""Adds a checkbox widget."""
	...

def add_child(name: str, *, show: bool = True, tip: str = '', parent: str = '', before: str = '', width: int = 0, height: int = 0, border: bool = True, autosize_x: bool = False, autosize_y: bool = False, no_scrollbar: bool = False, horizontal_scrollbar: bool = False, menubar: bool = False) -> None:
	"""Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end."""
	...

def add_collapsing_header(name: str, *, label: str = '', show: bool = True, tip: str = '', parent: str = '', before: str = '', closable: bool = False, default_open: bool = False, open_on_double_click: bool = False, open_on_arrow: bool = False, leaf: bool = False, bullet: bool = False) -> None:
	"""Adds a collapsing header to add items to. Must be closed with the end command."""
	...

def add_color_button(name: str, color: List[float], *, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', width: int = 0, height: int = 0, show: bool = True, no_alpha: bool = False, no_border: bool = False, no_drag_drop: bool = False) -> None:
	"""Adds a color button."""
	...

def add_color_edit3(name: str, *, default_value: List[int] = (0, 0, 0, 255), callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', width: int = 0, height: int = 0, label: str = '', show: bool = True, no_alpha: bool = False, no_picker: bool = False, no_options: bool = False, no_small_preview: bool = False, no_inputs: bool = False, no_tooltip: bool = False, no_label: bool = False, no_drag_drop: bool = False, alpha_bar: bool = False, alpha_preview: bool = False, alpha_preview_half: bool = False, display_rgb: bool = False, display_hsv: bool = False, display_hex: bool = False, uint8: bool = False, floats: bool = False, input_rgb: bool = False, input_hsv: bool = False) -> None:
	"""Adds an rgb color editing widget. Click and draging the color square will copy the color to be applied on any other color widget."""
	...

def add_color_edit4(name: str, *, default_value: List[int] = (0, 0, 0, 255), callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', width: int = 0, height: int = 0, label: str = '', show: bool = True, no_alpha: bool = False, no_picker: bool = False, no_options: bool = False, no_small_preview: bool = False, no_inputs: bool = False, no_tooltip: bool = False, no_label: bool = False, no_drag_drop: bool = False, alpha_bar: bool = False, alpha_preview: bool = False, alpha_preview_half: bool = False, display_rgb: bool = False, display_hsv: bool = False, display_hex: bool = False, uint8: bool = False, floats: bool = False, input_rgb: bool = False, input_hsv: bool = False) -> None:
	"""Adds an rgba color editing widget. Click and draging the color square will copy the color to be applied on any other color widget."""
	...

def add_color_picker3(name: str, *, default_value: List[int] = (0, 0, 0, 255), callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', width: int = 0, height: int = 0, label: str = '', show: bool = True, no_alpha: bool = False, no_small_preview: bool = False, no_inputs: bool = False, no_tooltip: bool = False, no_label: bool = False, no_side_preview: bool = False, alpha_bar: bool = False, alpha_preview: bool = False, alpha_preview_half: bool = False, display_rgb: bool = False, display_hsv: bool = False, display_hex: bool = False, uint8: bool = False, floats: bool = False, picker_hue_bar: bool = False, picker_hue_wheel: bool = False, input_rgb: bool = False, input_hsv: bool = False) -> None:
	"""Adds an rgb color picking widget. Click and draging the color square will copy the color to be applied on any other color widget. Right Click allows the style of the color picker to be changed."""
	...

def add_color_picker4(name: str, *, default_value: List[int] = (0, 0, 0, 255), callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', width: int = 0, height: int = 0, label: str = '', show: bool = True, no_alpha: bool = False, no_small_preview: bool = False, no_inputs: bool = False, no_tooltip: bool = False, no_label: bool = False, no_side_preview: bool = False, alpha_bar: bool = False, alpha_preview: bool = False, alpha_preview_half: bool = False, display_rgb: bool = False, display_hsv: bool = False, display_hex: bool = False, uint8: bool = False, floats: bool = False, picker_hue_bar: bool = False, picker_hue_wheel: bool = False, input_rgb: bool = False, input_hsv: bool = False) -> None:
	"""Adds an rgba color picking widget. Click and draging the color square will copy the color to be applied on any other color widget. Right Click allows the style of the color picker to be changed"""
	...

def add_column(table: str, name: str, column: List[str]) -> None:
	"""Adds a column to the end of a table."""
	...

def add_columns(name: str, columns: int, *, border: bool = True, show: bool = True, parent: str = '', before: str = '') -> None:
	"""Sets columns."""
	...

def add_combo(name: str, *, items: List[str] = (), default_value: str = '', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, label: str = '', show: bool = True, popup_align_left: bool = False, height_small: bool = False, height_regular: bool = False, height_large: bool = False, height_largest: bool = False, no_arrow_button: bool = False, no_preview: bool = False) -> None:
	"""Adds a combo."""
	...

def add_data(name: str, data: Any) -> None:
	"""Adds data for later retrieval."""
	...

def add_date_picker(name: str, *, default_value: dict = {'month_day': 14, 'year':20, 'month':5}, level: int = 0, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', show: bool = True) -> None:
	"""Adds a data selector widget."""
	...

def add_debug_window(name: str, *, width: int = 700, height: int = 500, x_pos: int = 200, y_pos: int = 200, autosize: bool = False, no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False, no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False, no_bring_to_front_on_focus: bool = False, no_close: bool = False, no_background: bool = False, label: str = '', show: bool = True) -> None:
	"""Creates a debug window."""
	...

def add_doc_window(name: str, *, width: int = -1, height: int = -1, x_pos: int = 200, y_pos: int = 200, autosize: bool = True, no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False, no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False, no_bring_to_front_on_focus: bool = False, no_close: bool = False, no_background: bool = False, label: str = '', show: bool = True) -> None:
	"""Creates a documentation window."""
	...

def add_drag_float(name: str, *, default_value: float = 0.0, speed: float = 1.0, min_value: float = 0.0, max_value: float = 100.0, format: str = '%0.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds drag for a single float value. CTRL+Click to directly modify the value."""
	...

def add_drag_float2(name: str, *, default_value: List[float] = (0.0, 0.0), speed: float = 1.0, min_value: float = 0.0, max_value: float = 100.0, format: str = '%0.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds drag for a 2 float values. CTRL+Click to directly modify the value."""
	...

def add_drag_float3(name: str, *, default_value: List[float] = (0.0, 0.0, 0.0), speed: float = 1.0, min_value: float = 0.0, max_value: float = 100.0, format: str = '%0.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds drag for a 3 float values. CTRL+Click to directly modify the value."""
	...

def add_drag_float4(name: str, *, default_value: List[float] = (0.0, 0.0, 0.0, 0.0), speed: float = 1.0, min_value: float = 0.0, max_value: float = 100.0, format: str = '%0.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds drag for a 4 float values. CTRL+Click to directly modify the value."""
	...

def add_drag_int(name: str, *, default_value: int = 0, speed: float = 1.0, min_value: int = 0, max_value: int = 100, format: str = '%d', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds drag for a single int value. CTRL+Click to directly modify the value."""
	...

def add_drag_int2(name: str, *, default_value: List[int] = (0, 0), speed: float = 1.0, min_value: int = 0, max_value: int = 100, format: str = '%d', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds drag for a 2 int values. CTRL+Click to directly modify the value."""
	...

def add_drag_int3(name: str, *, default_value: List[int] = (0, 0, 0), speed: float = 1.0, min_value: int = 0, max_value: int = 100, format: str = '%d', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds drag for a 3 int values. CTRL+Click to directly modify the value."""
	...

def add_drag_int4(name: str, *, default_value: List[int] = (0, 0, 0, 0), speed: float = 1.0, min_value: int = 0, max_value: int = 100, format: str = '%d', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds drag for a 4 int values. CTRL+Click to directly modify the value."""
	...

def add_drag_line(plot: str, name: str, *, source: str = '', color: List[float] = (0, 0, 0, -1), thickness: float = -1, y_line: bool = False, show_label: bool = True, callback: Callable = None, default_value: float = 0.0) -> None:
	"""Adds a drag line to a plot."""
	...

def add_drag_point(plot: str, name: str, *, source: str = '', color: List[float] = (0, 0, 0, -1), radius: float = 4.0, show_label: bool = True, callback: Callable = None, default_x: float = 0.0, default_y: float = 0.0) -> None:
	"""Adds a drag point to a plot."""
	...

def add_drawing(name: str, *, tip: str = '', parent: str = '', before: str = '', width: int = 0, height: int = 0, show: bool = True) -> None:
	"""Adds a drawing widget."""
	...

def add_dummy(*, width: int = 0, height: int = 0, name: str = 'dummy', parent: str = '', before: str = '', show: bool = True) -> None:
	"""Adds a spacer or 'dummy' object."""
	...

def add_error_series(plot: str, name: str, x: List[float], y: List[float], negative: List[float], positive: List[float], *, horizontal: bool = False, update_bounds: bool = True, color: List[float] = (0, 0, 0, -1), axis: int = 0) -> None:
	"""Adds an error series to a plot."""
	...

def add_group(name: str, *, show: bool = True, tip: str = '', parent: str = '', before: str = '', width: int = 0, horizontal: bool = False, horizontal_spacing: float = -1) -> None:
	"""Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.				Must be closed with the end command."""
	...

def add_heat_series(plot: str, name: str, values: List[float], rows: int, columns: int, scale_min: float, scale_max: float, *, format: str = '%0.1f', bounds_min: List[float] = (0.0, 0.0), bounds_max: List[float] = (1.0, 1.0), update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a heat series to a plot."""
	...

def add_hline_series(plot: str, name: str, x: List[float], *, color: List[float] = (0, 0, 0, -1), weight: float = 1.0, update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a infinite horizontal line series to a plot."""
	...

def add_image(name: str, value: str, *, tint_color: List[float] = (255, 255, 255, 255), border_color: List[float] = (0, 0, 0, 0), tip: str = '', parent: str = '', before: str = '', source: str = '', width: int = 0, height: int = 0, uv_min: List[float] = (0.0, 0.0), uv_max: List[float] = (1.0, 1.0), show: bool = True) -> None:
	"""Adds an image.uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown.Using(0,0)->(1,1) texture coordinates will generally display the entire texture"""
	...

def add_image_button(name: str, value: str, *, callback: Callable = None, callback_data: Any = None, tint_color: List[float] = (255, 255, 255, 255), background_color: List[float] = (0, 0, 0, 0), tip: str = '', parent: str = '', before: str = '', width: int = 0, height: int = 0, frame_padding: int = -1, uv_min: List[float] = (0.0, 0.0), uv_max: List[float] = (1.0, 1.0), show: bool = True) -> None:
	"""Adds an image button.uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown.Using(0,0)->(1,1) texture coordinates will generally display the entire texture"""
	...

def add_image_series(plot: str, name: str, value: str, bounds_min: List[float], bounds_max: List[float], *, uv_min: List[float] = (0.0, 0.0), uv_max: List[float] = (1.0, 1.0), tint_color: List[int] = (255, 255, 255, 255), update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a image series to a plot."""
	...

def add_indent(*, name: str = 'indent', offset: float = 0.0, parent: str = '', before: str = '', show: bool = True) -> None:
	"""Adds an indent to following items. Must be closed with the unindent command."""
	...

def add_input_float(name: str, *, default_value: float = 0.0, min_value: float = 0, max_value: float = 100, min_clamped: bool = False, max_clamped: bool = False, format: str = '%.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, on_enter: bool = False, label: str = '', show: bool = True, step: float = 0.1, step_fast: float = 1.0, readonly: bool = False) -> None:
	"""Adds input for float values."""
	...

def add_input_float2(name: str, *, default_value: List[float] = (0.0, 0.0), min_value: float = 0, max_value: float = 100, min_clamped: bool = False, max_clamped: bool = False, format: str = '%.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, on_enter: bool = False, label: str = '', show: bool = True, readonly: bool = False) -> None:
	"""Adds input for 2 float values."""
	...

def add_input_float3(name: str, *, default_value: List[float] = (0.0, 0.0, 0.0), min_value: float = 0, max_value: float = 100, min_clamped: bool = False, max_clamped: bool = False, format: str = '%.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, on_enter: bool = False, label: str = '', show: bool = True, readonly: bool = False) -> None:
	"""Adds input for 3 float values."""
	...

def add_input_float4(name: str, *, default_value: List[float] = (0.0, 0.0, 0.0, 0.0), min_value: float = 0, max_value: float = 100, min_clamped: bool = False, max_clamped: bool = False, format: str = '%.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, on_enter: bool = False, label: str = '', show: bool = True, readonly: bool = False) -> None:
	"""Adds input for 4 float values."""
	...

def add_input_int(name: str, *, default_value: int = 0, min_value: int = 0, max_value: int = 100, min_clamped: bool = False, max_clamped: bool = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, on_enter: bool = False, label: str = '', show: bool = True, step: int = 1, step_fast: int = 100, readonly: bool = False) -> None:
	"""Adds input for integer values."""
	...

def add_input_int2(name: str, *, default_value: List[int] = (0, 0), min_value: int = 0, max_value: int = 100, min_clamped: bool = False, max_clamped: bool = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, on_enter: bool = False, label: str = '', show: bool = True, readonly: bool = False) -> None:
	"""Adds input for 2 integer values."""
	...

def add_input_int3(name: str, *, default_value: List[int] = (0, 0, 0), min_value: int = 0, max_value: int = 100, min_clamped: bool = False, max_clamped: bool = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, on_enter: bool = False, label: str = '', show: bool = True, readonly: bool = False) -> None:
	"""Adds input for 3 integer values."""
	...

def add_input_int4(name: str, *, default_value: List[int] = (0, 0, 0, 0), min_value: int = 0, max_value: int = 100, min_clamped: bool = False, max_clamped: bool = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, on_enter: bool = False, label: str = '', show: bool = True, readonly: bool = False) -> None:
	"""Adds input for 4 integer values."""
	...

def add_input_text(name: str, *, default_value: str = '', hint: str = '', multiline: bool = False, no_spaces: bool = False, uppercase: bool = False, tab_input: bool = False, decimal: bool = False, hexadecimal: bool = False, readonly: bool = False, password: bool = False, scientific: bool = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, height: int = 0, on_enter: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds input for text values."""
	...

def add_label_text(name: str, *, default_value: str = '', color: List[float] = (0, 0, 0, -1), tip: str = '', parent: str = '', before: str = '', source: str = '', label: str = '', show: bool = True) -> None:
	"""Adds text with a label. Useful for output values."""
	...

def add_line_series(plot: str, name: str, x: List[float], y: List[float], *, color: List[float] = (0, 0, 0, -1), weight: float = 1.0, update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a line series to a plot."""
	...

def add_listbox(name: str, *, items: List[str] = (), default_value: int = 0, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, num_items: int = 3, label: str = '', show: bool = True) -> None:
	"""Adds a listbox."""
	...

def add_logger(name: str, *, log_level: int = 1, auto_scroll: bool = True, auto_scroll_button: bool = True, clear_button: bool = True, copy_button: bool = True, filter: bool = True, width: int = 0, height: int = 0, parent: str = '', before: str = '', show: bool = True, autosize_x: bool = False, autosize_y: bool = False) -> None:
	"""Adds a logging widget."""
	...

def add_managed_columns(name: str, columns: int, *, border: bool = True, show: bool = True, parent: str = '', before: str = '') -> None:
	"""Adds managed columns."""
	...

def add_menu(name: str, *, label: str = '', show: bool = True, parent: str = '', before: str = '', enabled: bool = True) -> None:
	"""Adds a menu to an existing menu bar. Must be followed by a call to end."""
	...

def add_menu_bar(name: str, *, show: bool = True, parent: str = '', before: str = '') -> None:
	"""Adds a menu bar to a window. Must be followed by a call to end."""
	...

def add_menu_item(name: str, *, shortcut: str = '', check: bool = False, callback: Callable = None, callback_data: Any = None, label: str = '', show: bool = True, enabled: bool = True, parent: str = '', before: str = '') -> None:
	"""Adds a menu item to an existing menu."""
	...

def add_metrics_window(name: str, *, width: int = 700, height: int = 500, x_pos: int = 200, y_pos: int = 200, autosize: bool = False, no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False, no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False, no_bring_to_front_on_focus: bool = False, no_close: bool = False, no_background: bool = False, label: str = '', show: bool = True) -> None:
	"""Creates a metrics window."""
	...

def add_next_column(*, name: str = 'next_collumn', show: bool = True, parent: str = '', before: str = '') -> None:
	"""Changes to next column."""
	...

def add_node(name: str, *, show: bool = True, label: str = '', draggable: bool = True, parent: str = '', before: str = '', x_pos: int = 100, y_pos: int = 100) -> None:
	"""Adds a node to a node editor."""
	...

def add_node_attribute(name: str, *, output: bool = False, static: bool = False, show: bool = True, parent: str = '', before: str = '') -> None:
	"""Adds a node attribute."""
	...

def add_node_editor(name: str, *, show: bool = True, parent: str = '', before: str = '', link_callback: Callable = None, delink_callback: Callable = None) -> None:
	"""Adds a node editor."""
	...

def add_node_link(node_editor: str, node_1: str, node_2: str) -> None:
	"""Adds a node link between nodes."""
	...

def add_pie_series(plot: str, name: str, values: List[float], labels: List[str], x: float, y: float, radius: float, *, normalize: bool = False, angle: float = 90.0, format: str = '%0.2f', update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a pie series to a plot."""
	...

def add_plot(name: str, *, x_axis_name: str = '', y_axis_name: str = '', no_legend: bool = False, no_menus: bool = False, no_box_select: bool = False, no_mouse_pos: bool = False, no_highlight: bool = False, no_child: bool = False, query: bool = False, crosshairs: bool = False, anti_aliased: bool = False, equal_aspects: bool = False, yaxis2: bool = False, yaxis3: bool = False, xaxis_no_gridlines: bool = False, xaxis_no_tick_marks: bool = False, xaxis_no_tick_labels: bool = False, xaxis_log_scale: bool = False, xaxis_time: bool = False, xaxis_invert: bool = False, xaxis_lock_min: bool = False, xaxis_lock_max: bool = False, yaxis_no_gridlines: bool = False, yaxis_no_tick_marks: bool = False, yaxis_no_tick_labels: bool = False, yaxis_log_scale: bool = False, yaxis_invert: bool = False, yaxis_lock_min: bool = False, yaxis_lock_max: bool = False, y2axis_no_gridlines: bool = False, y2axis_no_tick_marks: bool = False, y2axis_no_tick_labels: bool = False, y2axis_log_scale: bool = False, y2axis_invert: bool = False, y2axis_lock_min: bool = False, y2axis_lock_max: bool = False, y3axis_no_gridlines: bool = False, y3axis_no_tick_marks: bool = False, y3axis_no_tick_labels: bool = False, y3axis_log_scale: bool = False, y3axis_invert: bool = False, y3axis_lock_min: bool = False, y3axis_lock_max: bool = False, parent: str = '', before: str = '', width: int = -1, height: int = -1, query_callback: Callable = None, show_color_scale: bool = False, scale_min: float = 0.0, scale_max: float = 1.0, scale_height: int = 100, label: str = '', show: bool = True, show_annotations: bool = True, show_drag_lines: bool = True, show_drag_points: bool = True) -> None:
	"""Adds a plot widget."""
	...

def add_popup(popupparent: str, name: str, *, mousebutton: int = 1, modal: bool = False, parent: str = '', before: str = '', width: int = 0, height: int = 0, show: bool = True) -> None:
	"""Adds a popup window for an item. This command must come immediately after the item the popup is for. Must be followed by a call to end."""
	...

def add_progress_bar(name: str, *, default_value: float = 0.0, overlay: str = '', tip: str = '', parent: str = '', before: str = '', source: str = '', width: int = 0, height: int = 0, show: bool = True) -> None:
	"""Adds a progress bar."""
	...

def add_radio_button(name: str, *, items: List[str] = (), default_value: int = 0, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, horizontal: bool = False, show: bool = True) -> None:
	"""Adds a set of radio buttons. If items is empty, nothing will be shown."""
	...

def add_row(table: str, row: List[str]) -> None:
	"""Adds a row to the end of a table."""
	...

def add_same_line(*, name: str = 'sameline', xoffset: float = 0.0, spacing: float = -1.0, parent: str = '', before: str = '', show: bool = True) -> None:
	"""Places a widget on the same line as the previous widget. Can also be used for horizontal spacing."""
	...

def add_scatter_series(plot: str, name: str, x: List[float], y: List[float], *, marker: int = 0, size: float = 4.0, weight: float = 1.0, outline: List[float] = (0, 0, 0, -1), fill: List[float] = (0, 0, 0, -1), update_bounds: bool = True, xy_data_format: bool = False, axis: int = 0) -> None:
	"""Adds a scatter series to a plot."""
	...

def add_selectable(name: str, *, default_value: bool = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, label: str = '', show: bool = True, span_columns: bool = False) -> None:
	"""Adds a selectable."""
	...

def add_separator(*, name: str = 'separator', tip: str = '', parent: str = '', before: str = '') -> None:
	"""Adds a horizontal line."""
	...

def add_shade_series(plot: str, name: str, x: List[float], y1: List[float], *, y2: List[float] = ..., color: List[float] = (0, 0, 0, -1), fill: List[float] = (0, 0, 0, -1), weight: float = 1.0, update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a shade series to a plot."""
	...

def add_simple_plot(name: str, *, value: List[float] = (), overlay: str = '', minscale: float = 0.0, maxscale: float = 0.0, histogram: bool = False, tip: str = '', parent: str = '', before: str = '', width: int = 0, height: int = 0, source: str = '', label: str = '', show: bool = True) -> None:
	"""A simple plot for visualization of a set of values"""
	...

def add_slider_float(name: str, *, default_value: float = 0.0, min_value: float = 0.0, max_value: float = 100.0, format: str = '%.3f', vertical: bool = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, height: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds slider for a single float value. CTRL+Click to directly modify the value."""
	...

def add_slider_float2(name: str, *, default_value: List[float] = (0.0, 0.0), min_value: float = 0.0, max_value: float = 100.0, format: str = '%.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds slider for a 2 float values. CTRL+Click to directly modify the value."""
	...

def add_slider_float3(name: str, *, default_value: List[float] = (0.0, 0.0, 0.0), min_value: float = 0.0, max_value: float = 100.0, format: str = '%.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds slider for a 3 float values. CTRL+Click to directly modify the value."""
	...

def add_slider_float4(name: str, *, default_value: List[float] = (0.0, 0.0, 0.0, 0.0), min_value: float = 0.0, max_value: float = 100.0, format: str = '%.3f', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds slider for a 4 float values. CTRL+Click to directly modify the value."""
	...

def add_slider_int(name: str, *, default_value: int = 0, min_value: int = 0, max_value: int = 100, format: str = '%d', vertical: bool = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, height: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds slider for a single int value. CTRL+Click to directly modify the value."""
	...

def add_slider_int2(name: str, *, default_value: List[int] = (0, 0), min_value: int = 0, max_value: int = 100, format: str = '%d', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds slider for a 2 int values. CTRL+Click to directly modify the value."""
	...

def add_slider_int3(name: str, *, default_value: List[int] = (0, 0, 0), min_value: int = 0, max_value: int = 100, format: str = '%d', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds slider for a 3 int values. CTRL+Click to directly modify the value."""
	...

def add_slider_int4(name: str, *, default_value: List[int] = (0, 0, 0, 0), min_value: int = 0, max_value: int = 100, format: str = '%d', callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', source: str = '', enabled: bool = True, width: int = 0, no_input: bool = False, clamped: bool = False, label: str = '', show: bool = True) -> None:
	"""Adds slider for a 4 int values. CTRL+Click to directly modify the value."""
	...

def add_spacing(*, name: str = 'spacing', count: int = 1, parent: str = '', before: str = '', show: bool = True) -> None:
	"""Adds vertical spacing."""
	...

def add_stair_series(plot: str, name: str, x: List[float], y: List[float], *, color: List[float] = (0, 0, 0, -1), weight: float = 1.0, update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a stair series to a plot."""
	...

def add_stem_series(plot: str, name: str, x: List[float], y: List[float], *, marker: int = 0, size: float = 4.0, weight: float = 1.0, outline: List[float] = (0, 0, 0, -1), fill: List[float] = (0, 0, 0, -1), update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a stem series to a plot."""
	...

def add_style_window(name: str, *, width: int = 700, height: int = 500, x_pos: int = 200, y_pos: int = 200, autosize: bool = False, no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False, no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False, no_bring_to_front_on_focus: bool = False, no_close: bool = False, no_background: bool = False, label: str = '', show: bool = True) -> None:
	"""Creates a style window."""
	...

def add_tab(name: str, *, closable: bool = False, label: str = '', show: bool = True, no_reorder: bool = False, leading: bool = False, trailing: bool = False, no_tooltip: bool = False, tip: str = '', parent: str = '', before: str = '') -> None:
	"""Adds a tab to a tab bar. Must be closed with the end command."""
	...

def add_tab_bar(name: str, *, reorderable: bool = False, callback: Callable = None, callback_data: Any = None, show: bool = True, parent: str = '', before: str = '') -> None:
	"""Adds a tab bar."""
	...

def add_tab_button(name: str, *, label: str = '', show: bool = True, no_reorder: bool = False, leading: bool = False, trailing: bool = False, no_tooltip: bool = False, tip: str = '', callback: Callable = None, callback_data: Any = None, parent: str = '', before: str = '') -> None:
	"""Adds a tab button to a tab bar"""
	...

def add_table(name: str, headers: List[str], *, callback: Callable = None, callback_data: Any = None, parent: str = '', before: str = '', width: int = 0, height: int = 200, hide_headers: bool = False, show: bool = True) -> None:
	"""Adds table."""
	...

def add_text(name: str, *, wrap: int = -1, color: List[float] = (0, 0, 0, -1), bullet: bool = False, tip: str = '', parent: str = '', before: str = '', source: str = '', default_value: str = '', show: bool = True) -> None:
	"""Adds text"""
	...

def add_text_point(plot: str, name: str, x: float, y: float, *, vertical: bool = False, xoffset: int = 0.0, yoffset: int = 0.0, update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a point with text to a plot."""
	...

def add_texture(name: str, data: List[int], width: int, height: int, *, format: int = 0) -> None:
	"""Adds a texture."""
	...

def add_time_picker(name: str, *, default_value: dict = {'hour': 14, 'min': 32, 'sec': 23}, hour24: bool = False, callback: Callable = None, callback_data: Any = None, tip: str = '', parent: str = '', before: str = '', show: bool = True) -> None:
	"""Adds a time selector widget."""
	...

def add_tooltip(tipparent: str, name: str, *, parent: str = '', before: str = '', show: bool = True) -> None:
	"""Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for."""
	...

def add_tree_node(name: str, *, label: str = '', show: bool = True, tip: str = '', parent: str = '', before: str = '', default_open: bool = False, open_on_double_click: bool = False, open_on_arrow: bool = False, leaf: bool = False, bullet: bool = False) -> None:
	"""Adds a tree node to add items to. Must be closed with the end command."""
	...

def add_value(name: str, value: Any) -> None:
	"""Adds a value to the value storage."""
	...

def add_vline_series(plot: str, name: str, x: List[float], *, color: List[float] = (0, 0, 0, -1), weight: float = 1.0, update_bounds: bool = True, axis: int = 0) -> None:
	"""Adds a infinite vertical line series to a plot."""
	...

def add_window(name: str, *, width: int = -1, height: int = -1, x_pos: int = 200, y_pos: int = 200, autosize: bool = False, no_resize: bool = False, no_title_bar: bool = False, no_move: bool = False, no_scrollbar: bool = False, no_collapse: bool = False, horizontal_scrollbar: bool = False, no_focus_on_appearing: bool = False, no_bring_to_front_on_focus: bool = False, menubar: bool = False, no_close: bool = False, no_background: bool = False, label: str = '', show: bool = True, collapsed: bool = False, on_close: Callable = None) -> None:
	"""Creates a new window for following items to be added to."""
	...

def bring_draw_command_forward(drawing: str, tag: str) -> None:
	"""Brings draw command forward."""
	...

def bring_draw_command_to_front(drawing: str, tag: str) -> None:
	"""Brings draw command to front."""
	...

def cleanup_dearpygui() -> None:
	"""Cleans up DearPyGui after calling setup_dearpygui."""
	...

def clear_drawing(draw: str) -> None:
	"""Clears a drawing."""
	...

def clear_item_color(item: str) -> None:
	"""Clears individual color styles for an item."""
	...

def clear_item_style_vars(item: str) -> None:
	"""Clears individual styles variables for an item."""
	...

def clear_log(*, logger: str = '') -> None:
	"""Clears the logger."""
	...

def clear_plot(plot: str) -> None:
	"""Clears a plot."""
	...

def clear_selected_links(node_editor: str) -> None:
	"""Clears selected links."""
	...

def clear_selected_nodes(node_editor: str) -> None:
	"""Clears selected nodes."""
	...

def clear_table(table: str) -> None:
	"""Clears data in a table"""
	...

def close_popup(item: str) -> None:
	"""Closes a popup."""
	...

def configure_item(item: str, **Kwargs) -> None:
	"""Configures an item"""
	...

def decref_value(name: str) -> None:
	"""Decreases the reference count of a value."""
	...

def decrement_texture(name: str) -> None:
	"""Decrements a texture."""
	...

def delete_annotation(plot: str, name: str) -> None:
	"""Deletes an annotation"""
	...

def delete_column(table: str, column: int) -> None:
	"""Delete a column in a table."""
	...

def delete_data(name: str) -> None:
	"""Deletes data from storage."""
	...

def delete_drag_line(plot: str, name: str) -> None:
	"""Deletes a drag line if it exists."""
	...

def delete_drag_point(plot: str, name: str) -> None:
	"""Deletes a drag point if it exists."""
	...

def delete_draw_command(drawing: str, tag: str) -> None:
	"""Deletes a drawing item."""
	...

def delete_item(item: str, *, children_only: bool = False) -> None:
	"""Deletes an item if it exists."""
	...

def delete_node_link(node_editor: str, node_1: str, node_2: str) -> None:
	"""Deletes a node link if it exist."""
	...

def delete_row(table: str, row: int) -> None:
	"""Delete a row in a table."""
	...

def delete_series(plot: str, series: str) -> None:
	"""Deletes a series if it exists."""
	...

def does_item_exist(item: str) -> bool:
	"""Checks if item exists."""
	...

def draw_arrow(drawing: str, p1: List[float], p2: List[float], color: List[int], thickness: int, size: int, *, tag: str = '') -> None:
	"""Draws an arrow on a drawing."""
	...

def draw_bezier_curve(drawing: str, p1: List[float], p2: List[float], p3: List[float], p4: List[float], color: List[int], *, thickness: float = 1.0, segments: int = 0, tag: str = '') -> None:
	"""Draws a bezier curve on a drawing."""
	...

def draw_circle(drawing: str, center: List[float], radius: float, color: List[int], *, segments: int = 0, thickness: float = 1.0, fill: List[float] = (0, 0, 0, -1), tag: str = '') -> None:
	"""Draws a circle on a drawing."""
	...

def draw_image(drawing: str, file: str, pmin: List[float], pmax: List[float], *, uv_min: List[float] = (0.0, 0.0), uv_max: List[float] = (1.0, 1.0), color: List[int] = (255, 255, 255, 255), tag: str = '') -> None:
	"""Draws an image on a drawing. p_min (top-left) and p_max (bottom-right) represent corners of the rectangle the image will be drawn to.Setting the p_min equal to the p_max will sraw the image to with 1:1 scale.uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using (0,0)->(1,1) texturecoordinates will generally display the entire texture."""
	...

def draw_line(drawing: str, p1: List[float], p2: List[float], color: List[int], thickness: int, *, tag: str = '') -> None:
	"""Draws a line on a drawing."""
	...

def draw_polygon(drawing: str, points: List[List[float]], color: List[int], *, fill: List[float] = (0, 0, 0, -1), thickness: float = 1.0, tag: str = '') -> None:
	"""Draws a polygon on a drawing."""
	...

def draw_polyline(drawing: str, points: List[List[float]], color: List[int], *, closed: bool = False, thickness: float = 1.0, tag: str = '') -> None:
	"""Draws lines on a drawing."""
	...

def draw_quad(drawing: str, p1: List[float], p2: List[float], p3: List[float], p4: List[float], color: List[int], *, fill: List[float] = (0, 0, 0, -1), thickness: float = 1.0, tag: str = '') -> None:
	"""Draws a quad on a drawing."""
	...

def draw_rectangle(drawing: str, pmin: List[float], pmax: List[float], color: List[int], *, fill: List[float] = (0, 0, 0, -1), rounding: float = 0.0, thickness: float = 1.0, tag: str = '') -> None:
	"""Draws a rectangle on a drawing."""
	...

def draw_text(drawing: str, pos: List[float], text: str, *, color: List[int] = (0, 0, 0, -1), size: int = 10, tag: str = '') -> None:
	"""Draws text on a drawing."""
	...

def draw_triangle(drawing: str, p1: List[float], p2: List[float], p3: List[float], color: List[int], *, fill: List[float] = (0, 0, 0, -1), thickness: float = 1.0, tag: str = '') -> None:
	"""Draws a triangle on a drawing."""
	...

def enable_docking(*, shift_only: bool = True, dock_space: bool = False) -> None:
	"""Decrements a texture."""
	...

def end() -> None:
	"""Ends a container."""
	...

def get_active_window() -> str:
	"""Returns the active window name."""
	...

def get_all_items() -> List[str]:
	"""Returns a list of all items."""
	...

def get_data(name: str) -> object:
	"""Retrieves data from storage."""
	...

def get_dearpygui_version() -> str:
	"""Returns the current version of Dear PyGui."""
	...

def get_delta_time() -> float:
	"""Returns time since last frame."""
	...

def get_draw_command(drawing: str, tag: str) -> dict:
	"""Returns an draw commands information"""
	...

def get_drawing_mouse_pos() -> (int, int):
	"""Returns the current mouse position in the currently hovered drawing."""
	...

def get_global_font_scale() -> float:
	"""Returns the global font scale."""
	...

def get_item_callback(item: str) -> Callable:
	"""Returns an item' callback"""
	...

def get_item_callback_data(item: str) -> Any:
	"""Returns an item' callback data"""
	...

def get_item_children(item: str) -> List[str]:
	"""Returns a list of an item's children."""
	...

def get_item_configuration(item: str) -> dict:
	"""Returns an items configuration"""
	...

def get_item_height(item: str) -> float:
	"""Returns an item's height."""
	...

def get_item_label(item: str) -> str:
	"""Gets an item's label."""
	...

def get_item_parent(item: str) -> str:
	"""Returns an item's parent."""
	...

def get_item_rect_max(item: str) -> [float, float]:
	"""Returns an item's maximum allowable size. [width, height]"""
	...

def get_item_rect_min(item: str) -> [float, float]:
	"""Returns an item's minimum allowable size. [width, height]"""
	...

def get_item_rect_size(item: str) -> [float, float]:
	"""Returns an item's current size. [width, height]"""
	...

def get_item_tip(item: str) -> str:
	"""Returns an item's tip."""
	...

def get_item_type(item: str) -> str:
	"""Returns an item's type"""
	...

def get_item_width(item: str) -> float:
	"""Returns an item's width."""
	...

def get_links(node_editor: str) -> List[List[str]]:
	"""Returns all links. """
	...

def get_log_level(*, logger: str = '') -> int:
	"""Returns the log level."""
	...

def get_main_window_size() -> [int, int]:
	"""Returns the size of the main window."""
	...

def get_managed_column_width(item: str, column: int) -> Float:
	"""Returns the width of the ith column."""
	...

def get_mouse_drag_delta() -> (float, float):
	"""Returns the current mouse drag delta in pixels"""
	...

def get_mouse_pos(*, local: bool = True) -> (int, int):
	"""Returns the current mouse position in relation to the active window (minus titlebar) unless local flag is unset."""
	...

def get_plot_mouse_pos() -> (int, int):
	"""Returns the current mouse position in the currently hovered plot."""
	...

def get_plot_query_area(plot: str) -> List[float]:
	"""Returns the bounding axis limits for the query area [x_min, x_max, y_min, y_max]"""
	...

def get_plot_xlimits(plot: str) -> List[float]:
	"""Returns the plots x limits"""
	...

def get_plot_ylimits(plot: str) -> List[float]:
	"""Returns the plots x limits"""
	...

def get_selected_links(node_editor: str) -> List[List[str]]:
	"""Returns selected links."""
	...

def get_selected_nodes(node_editor: str) -> List[str]:
	"""Returns selected nodes."""
	...

def get_style_antialiased_fill() -> bool:
	"""Gets anti-aliasing on filled shapes (rounded rectangles, circles, etc.)."""
	...

def get_style_antialiased_lines() -> bool:
	"""Gets anti-aliasing on lines/borders."""
	...

def get_style_button_text_align() -> List[float]:
	"""Gets alignment of button text when button is larger than text. Defaults to (0.5, 0.5) (centered)."""
	...

def get_style_child_border_size() -> float:
	"""Gets thickness of border around child windows."""
	...

def get_style_child_rounding() -> float:
	"""Gets radius of child window corners rounding."""
	...

def get_style_circle_segment_max_error() -> float:
	"""Gets maximum error (in pixels) allowed when using draw_circle()or drawing rounded corner rectangles with no explicit segment count specified."""
	...

def get_style_color_button_position() -> int:
	"""Gets side of the color button in the ColorEdit4 widget (left/right). Defaults to mvGuiDir_Right."""
	...

def get_style_curve_tessellation_tolerance() -> float:
	"""Gets Tessellation tolerance."""
	...

def get_style_display_safe_area_padding() -> List[float]:
	"""Gets safe area padding. Applies to popups/tooltips as well regular windows."""
	...

def get_style_frame_border_size() -> float:
	"""Gets thickness of border around frames."""
	...

def get_style_frame_padding() -> List[float]:
	"""Gets padding within a framed rectangle (used by most widgets)."""
	...

def get_style_frame_rounding() -> float:
	"""Gets radius of frame corners rounding."""
	...

def get_style_global_alpha() -> float:
	"""Gets global alpha applies to everything in Dear PyGui."""
	...

def get_style_grab_min_size() -> float:
	"""Gets minimum width/height of a grab box for slider/scrollbar."""
	...

def get_style_grab_rounding() -> float:
	"""Gets radius of grabs corners rounding. Set to 0.0 to have rectangular slider grabs."""
	...

def get_style_indent_spacing() -> float:
	"""Gets horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2)."""
	...

def get_style_item_inner_spacing() -> List[float]:
	"""Gets horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)."""
	...

def get_style_item_spacing() -> List[float]:
	"""Gets horizontal and vertical spacing between widgets/lines."""
	...

def get_style_popup_border_size() -> float:
	"""Gets thickness of border around popup/tooltip windows."""
	...

def get_style_popup_rounding() -> float:
	"""Gets radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)."""
	...

def get_style_scrollbar_rounding() -> float:
	"""Gets radius of grab corners for scrollbar."""
	...

def get_style_scrollbar_size() -> float:
	"""Gets width of the vertical scrollbar, Height of the horizontal scrollbar."""
	...

def get_style_selectable_text_align() -> List[float]:
	"""Gets alignment of selectable text. Defaults to (0.0, 0.0) (top-left aligned)."""
	...

def get_style_tab_border_size() -> float:
	"""Gets thickness of border around tabs."""
	...

def get_style_tab_rounding() -> float:
	"""Gets radius of upper corners of a tab. Set to 0.0 to have rectangular tabs."""
	...

def get_style_touch_extra_padding() -> List[float]:
	"""Get touch extra padding."""
	...

def get_style_window_border_size() -> float:
	"""Gets thickness of border around windows."""
	...

def get_style_window_menu_button_position() -> int:
	"""Gets side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to mvGuiDir_Left."""
	...

def get_style_window_padding() -> List[float]:
	"""Gets padding within a window."""
	...

def get_style_window_rounding() -> float:
	"""Gets radius of window corners rounding."""
	...

def get_style_window_title_align() -> List[float]:
	"""Gets alignment for title bar text. Defaults to (0.0,0.5) for left-aligned,vertically centered."""
	...

def get_table_data(name: str) -> List[List[str]]:
	"""Gets table data."""
	...

def get_table_item(table: str, row: int, column: int) -> str:
	"""Gets a table's cell value."""
	...

def get_table_selections(table: str) -> List[List[int]]:
	"""Retrieves data from storage."""
	...

def get_theme() -> str:
	"""Returns the current theme."""
	...

def get_theme_item(item: int) -> (float, float, float, float):
	"""Returns a theme item's color"""
	...

def get_total_time() -> float:
	"""Returns total time since app started."""
	...

def get_value(name: str) -> Any:
	"""Returns an item's value or None if there is none."""
	...

def get_windows() -> List[str]:
	"""Returns a list of windows."""
	...

def incref_value(name: str) -> None:
	"""Increases the reference count of a value."""
	...

def insert_column(table: str, column_index: int, name: str, column: List[str]) -> None:
	"""Inserts a column into a table."""
	...

def insert_row(table: str, row_index: int, row: List[str]) -> None:
	"""Inserts a row into a table."""
	...

def is_dearpygui_running() -> bool:
	"""Checks if dearpygui is still running"""
	...

def is_item_activated(item: str) -> bool:
	"""Checks if an item has been activated."""
	...

def is_item_active(item: str) -> bool:
	"""Checks if an item is active."""
	...

def is_item_clicked(item: str) -> bool:
	"""Checks if an item is clicked."""
	...

def is_item_container(item: str) -> bool:
	"""Checks if an item is container."""
	...

def is_item_deactivated(item: str) -> bool:
	"""Checks if an item has been deactivated."""
	...

def is_item_deactivated_after_edit(item: str) -> bool:
	"""Checks if an item has been deactivated after editing."""
	...

def is_item_edited(item: str) -> bool:
	"""Checks if an item has been edited."""
	...

def is_item_focused(item: str) -> bool:
	"""Checks if an item is focused."""
	...

def is_item_hovered(item: str) -> bool:
	"""Checks if an item is hovered."""
	...

def is_item_shown(item: str) -> bool:
	"""Checks if an item is shown."""
	...

def is_item_toggled_open(item: str) -> bool:
	"""Checks if an item is toggled."""
	...

def is_item_visible(item: str) -> bool:
	"""Checks if an item is visible on screen."""
	...

def is_key_down(key: int) -> bool:
	"""Checks if the key is down."""
	...

def is_key_pressed(key: int) -> bool:
	"""Checks if the key is pressed."""
	...

def is_key_released(key: int) -> bool:
	"""Checks if the key is released."""
	...

def is_mouse_button_clicked(button: int) -> bool:
	"""Checks if the mouse button is clicked."""
	...

def is_mouse_button_double_clicked(button: int) -> bool:
	"""Checks if the mouse button is double clicked."""
	...

def is_mouse_button_down(button: int) -> bool:
	"""Checks if the mouse button is pressed."""
	...

def is_mouse_button_dragging(button: int, threshold: float) -> bool:
	"""Checks if the mouse is dragging."""
	...

def is_mouse_button_released(button: int) -> bool:
	"""Checks if the mouse button is released."""
	...

def is_plot_queried(plot: str) -> bool:
	"""Returns true if plot was queried"""
	...

def log(message: Any, *, level: str = 'TRACE', logger: str = '') -> None:
	"""Logs a trace level log."""
	...

def log_debug(message: Any, *, logger: str = '') -> None:
	"""Logs a debug level log."""
	...

def log_error(message: Any, *, logger: str = '') -> None:
	"""Logs a error level log."""
	...

def log_info(message: Any, *, logger: str = '') -> None:
	"""Logs a info level log."""
	...

def log_warning(message: Any, *, logger: str = '') -> None:
	"""Logs a warning level log."""
	...

def modify_draw_command(drawing: str, tag: str, **Kwargs) -> None:
	"""Configures an drawing command."""
	...

def move_item(item: str, *, parent: str = '', before: str = '') -> None:
	"""Moves an existing item."""
	...

def move_item_down(item: str) -> None:
	"""Moves item down if possible and if it exists."""
	...

def move_item_up(item: str) -> None:
	"""Moves item up if possible and if it exists."""
	...

def open_file_dialog(callback: Callable = None, extensions: str = '') -> None:
	"""Opens an 'open file' dialog."""
	...

def render_dearpygui_frame() -> None:
	"""Renders a DearPyGui frame. Should be called within a user's event loop. Must first call setup_dearpygui outside of event loop."""
	...

def reset_xticks(plot: str) -> None:
	"""Sets plots x ticks and labels back to automatic"""
	...

def reset_yticks(plot: str) -> None:
	"""Sets plots y ticks and labels back to automatic"""
	...

def select_directory_dialog(callback: Callable = None) -> None:
	"""Opens a select directory dialog."""
	...

def send_draw_command_back(drawing: str, tag: str) -> None:
	"""Sends draw command back."""
	...

def send_draw_command_to_back(drawing: str, tag: str) -> None:
	"""Sends draw command to the back."""
	...

def set_accelerator_callback(callback: Any) -> None:
	"""Callback similar to keypress but used for accelerator keys."""
	...

def set_color_map(plot: str, map: int) -> None:
	"""Sets the color map of the plot's series."""
	...

def set_exit_callback(callback: Any) -> None:
	"""Callback to run when exiting main window."""
	...

def set_global_color(id: int, color: List[float]) -> None:
	"""Sets an color of a theme item."""
	...

def set_global_font_scale(scale: float) -> None:
	"""Changes the global font scale."""
	...

def set_headers(table: str, headers: List[str]) -> None:
	"""Sets a tables headers."""
	...

def set_individual_color(item: str, id: int, color: List[float]) -> None:
	"""Sets an color of a theme item."""
	...

def set_item_callback(item: str, callback: Callable, *, callback_data: Any = None) -> None:
	"""Sets an item's callback if applicable."""
	...

def set_item_callback_data(item: str, callback_data: Any) -> None:
	"""Sets an item's callback data if applicable."""
	...

def set_item_color(item: str, style: int, color: List[float]) -> None:
	"""Sets an color style for a single item."""
	...

def set_item_height(item: str, height: int) -> None:
	"""Sets an item's height if applicable."""
	...

def set_item_label(item: str, label: str) -> None:
	"""Sets an item's label."""
	...

def set_item_style_var(item: str, style: int, value: List[float]) -> None:
	"""Sets an style variable for a single item."""
	...

def set_item_tip(item: str, tip: str) -> None:
	"""Sets a simple tooltip for an item."""
	...

def set_item_width(item: str, width: int) -> None:
	"""Sets an item's width."""
	...

def set_key_down_callback(callback: Callable) -> None:
	"""Sets a callback for a key down event."""
	...

def set_key_press_callback(callback: Callable) -> None:
	"""Sets a callback for a key press event."""
	...

def set_key_release_callback(callback: Callable) -> None:
	"""Sets a callback for a key release event."""
	...

def set_log_level(level: int, *, logger: str = '') -> None:
	"""Sets the log level."""
	...

def set_logger_window_title(title: str) -> None:
	"""Sets the title of the logger window."""
	...

def set_main_window_pos(x: int, y: int) -> None:
	"""Sets the main window position."""
	...

def set_main_window_resizable(resizable: bool) -> None:
	"""Sets the main window to be resizable."""
	...

def set_main_window_size(width: int, height: int) -> None:
	"""Sets the main window size."""
	...

def set_main_window_title(title: str) -> None:
	"""Sets the title of the main window."""
	...

def set_managed_column_width(item: str, column: int, width: float) -> None:
	"""Sets the width of the ith column."""
	...

def set_mouse_click_callback(callback: Callable) -> None:
	"""Sets a callback for a mouse click event."""
	...

def set_mouse_double_click_callback(callback: Callable) -> None:
	"""Sets a callback for a mouse double click event."""
	...

def set_mouse_down_callback(callback: Callable) -> None:
	"""Sets a callback for a mouse down event."""
	...

def set_mouse_drag_callback(callback: Callable, threshold: float) -> None:
	"""Sets a callback for a mouse drag event."""
	...

def set_mouse_move_callback(callback: Callable) -> None:
	"""Sets a callback for a mouse move event. Data is the mouse position in local coordinates."""
	...

def set_mouse_release_callback(callback: Callable) -> None:
	"""Sets a callback for a mouse release event."""
	...

def set_mouse_wheel_callback(callback: Callable) -> None:
	"""Sets a callback for a mouse wheel event."""
	...

def set_plot_xlimits(plot: str, xmin: float, xmax: float) -> None:
	"""Sets x axis limits of a plot. (can be undone with set_plot_xlimits_auto()"""
	...

def set_plot_xlimits_auto(plot: str) -> None:
	"""Sets plots x limits to be automatic."""
	...

def set_plot_ylimits(plot: str, ymin: float, ymax: float) -> None:
	"""Sets y axis limits of a plot. (can be undone with set_plot_ylimits_auto()"""
	...

def set_plot_ylimits_auto(plot: str) -> None:
	"""Sets plots y limits to be automatic."""
	...

def set_primary_window(window: str, value: bool) -> None:
	"""Sets the primary window to fill the viewport."""
	...

def set_render_callback(callback: Callable) -> None:
	"""Sets the callback to be ran every frame."""
	...

def set_resize_callback(callback: Callable, *, handler: str = '') -> None:
	"""Sets a callback for a window resize event."""
	...

def set_start_callback(callback: Any) -> None:
	"""Callback to run when starting main window."""
	...

def set_style_antialiased_fill(value: bool) -> None:
	"""Sets anti-aliasing on filled shapes (rounded rectangles, circles, etc.)."""
	...

def set_style_antialiased_lines(value: bool) -> None:
	"""Sets anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU."""
	...

def set_style_button_text_align(x: float, y: float) -> None:
	"""Sets alignment of button text when button is larger than text. Defaults to (0.5, 0.5) (centered)."""
	...

def set_style_child_border_size(value: float) -> None:
	"""Sets thickness of border around child windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly)."""
	...

def set_style_child_rounding(value: float) -> None:
	"""Sets radius of child window corners rounding. Set to 0.0 to have rectangular windows."""
	...

def set_style_circle_segment_max_error(value: float) -> None:
	"""Sets maximum error (in pixels) allowed when using draw_circle()or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry."""
	...

def set_style_color_button_position(value: int) -> None:
	"""Sets side of the color button in the ColorEdit4 widget (left/right). Defaults to mvGuiDir_Right."""
	...

def set_style_curve_tessellation_tolerance(value: float) -> None:
	"""Sets Tessellation tolerance."""
	...

def set_style_display_safe_area_padding(x: float, y: float) -> None:
	"""Sets if you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!"""
	...

def set_style_frame_border_size(value: float) -> None:
	"""Sets thickness of border around frames. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly)."""
	...

def set_style_frame_padding(x: float, y: float) -> None:
	"""Sets padding within a framed rectangle (used by most widgets)."""
	...

def set_style_frame_rounding(value: float) -> None:
	"""Sets radius of frame corners rounding. Set to 0.0 to have rectangular frame (used by most widgets)."""
	...

def set_style_global_alpha(value: float) -> None:
	"""Sets global alpha applies to everything in Dear PyGui."""
	...

def set_style_grab_min_size(value: float) -> None:
	"""Sets minimum width/height of a grab box for slider/scrollbar."""
	...

def set_style_grab_rounding(value: float) -> None:
	"""Sets radius of grabs corners rounding. Set to 0.0 to have rectangular slider grabs."""
	...

def set_style_indent_spacing(value: float) -> None:
	"""Sets horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2)."""
	...

def set_style_item_inner_spacing(x: float, y: float) -> None:
	"""Sets horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)."""
	...

def set_style_item_spacing(x: float, y: float) -> None:
	"""Sets horizontal and vertical spacing between widgets/lines."""
	...

def set_style_popup_border_size(value: float) -> None:
	"""Sets thickness of border around popup/tooltip windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly)."""
	...

def set_style_popup_rounding(value: float) -> None:
	"""Sets radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)."""
	...

def set_style_scrollbar_rounding(value: float) -> None:
	"""Sets radius of grab corners for scrollbar."""
	...

def set_style_scrollbar_size(value: float) -> None:
	"""Sets width of the vertical scrollbar, Height of the horizontal scrollbar."""
	...

def set_style_selectable_text_align(x: float, y: float) -> None:
	"""Sets alignment of selectable text. Defaults to (0.0, 0.0) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line."""
	...

def set_style_tab_border_size(value: float) -> None:
	"""Sets thickness of border around tabs."""
	...

def set_style_tab_rounding(value: float) -> None:
	"""Sets radius of upper corners of a tab. Set to 0.0 to have rectangular tabs."""
	...

def set_style_touch_extra_padding(x: float, y: float) -> None:
	"""Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!"""
	...

def set_style_window_border_size(value: float) -> None:
	"""Sets thickness of border around windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly)."""
	...

def set_style_window_menu_button_position(value: int) -> None:
	"""Sets side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to mvGuiDir_Left."""
	...

def set_style_window_padding(x: float, y: float) -> None:
	"""Sets padding within a window."""
	...

def set_style_window_rounding(value: float) -> None:
	"""Sets Radius of window corners rounding. Set to 0.0fto have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended."""
	...

def set_style_window_title_align(x: float, y: float) -> None:
	"""Sets alignment for title bar text. Defaults to (0.0,0.5) for left-aligned,vertically centered."""
	...

def set_table_data(name: str, data: List[List[str]]) -> None:
	"""Overwrites table data."""
	...

def set_table_item(table: str, row: int, column: int, value: str) -> None:
	"""Sets a table's cell value."""
	...

def set_table_selection(table: str, row: int, column: int, value: bool) -> None:
	"""Sets a table's cell selection value."""
	...

def set_theme(theme: str) -> None:
	"""Set the application's theme to a built-in theme."""
	...

def set_theme_item(item: int, r: int, g: int, b: int, a: int) -> None:
	"""Sets a theme item."""
	...

def set_value(name: str, value: Any) -> bool:
	"""Sets an item's value if applicable."""
	...

def set_vsync(value: bool) -> None:
	"""Sets vsync on or off."""
	...

def set_xticks(plot: str, label_pairs: Any) -> None:
	"""Sets plots x ticks and labels"""
	...

def set_yticks(plot: str, label_pairs: Any) -> None:
	"""Sets plots y ticks and labels"""
	...

def setup_dearpygui() -> None:
	"""Sets up DearPyGui for user controlled rendering. Only call once and you must call cleanup_deapygui when finished."""
	...

def show_item(name: str) -> None:
	"""Shows an item if it was hidden."""
	...

def show_logger() -> None:
	"""Shows the logging window. The Default log level is Trace"""
	...

def start_dearpygui(*, primary_window: str = '') -> None:
	"""Starts DearPyGui."""
	...

def stop_dearpygui() -> None:
	"""Stops DearPyGui."""
	...

def unindent(*, name: str = 'unindent', offset: float = 0.0, parent: str = '', before: str = '', show: bool = True) -> None:
	"""Unindents following items."""
	...

