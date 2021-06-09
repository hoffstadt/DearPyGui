from typing import List, Any, Callable
from dearpygui.core import *

##########################################################
# This file is generated automatically by mvPythonParser #
##########################################################

# ~ Dear PyGui Version: master
def add_2d_histogram_series(x : List[float], y : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, xbins: int =-1, ybins: int =-1, xmin_range: float =0.0, xmax_range: float =1.0, ymin_range: float =0.0, ymax_range: float =1.0, density: bool =False, outliers: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_3d_slider(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), max_x: float =100.0, max_y: float =100.0, max_z: float =100.0, min_x: float =0.0, min_y: float =0.0, min_z: float =0.0, scale: float =1.0) -> int:
	"""Undocumented function"""
	...

def add_activated_handler(parent : str, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_active_handler(parent : str, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_area_series(x : List[float], y : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, fill: List[int] =(0, 0, 0, -255), contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_bar_series(x : List[float], y : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, weight: float =1.0, horizontal: bool =False, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_button(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, small: bool =False, arrow: bool =False, direction: int =0) -> int:
	"""Undocumented function"""
	...

def add_candle_series(dates : List[float], opens : List[float], closes : List[float], lows : List[float], highs : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, bull_color: List[int] =(0, 255, 113, 255), bear_color: List[int] =(218, 13, 79, 255), weight: int =0.25, contribute_to_bounds: bool =True, tooltip: bool =True) -> int:
	"""Undocumented function"""
	...

def add_checkbox(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: bool =False) -> int:
	"""Undocumented function"""
	...

def add_child(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, border: bool =True, autosize_x: bool =False, autosize_y: bool =False, no_scrollbar: bool =False, horizontal_scrollbar: bool =False, menubar: bool =False) -> int:
	"""Undocumented function"""
	...

def add_clicked_handler(parent : str, button : int, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_clipper(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_collapsing_header(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, closable: bool =False, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False) -> int:
	"""Undocumented function"""
	...

def add_color_button(default_value : List[int] =(0, 0, 0, 255), *, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_border: bool =False, no_drag_drop: bool =False) -> int:
	"""Undocumented function"""
	...

def add_color_edit(default_value : List[int] =(0, 0, 0, 255), *, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_picker: bool =False, no_options: bool =False, no_small_preview: bool =False, no_inputs: bool =False, no_tooltip: bool =False, no_label: bool =False, no_drag_drop: bool =False, alpha_bar: bool =False, alpha_preview: bool =False, alpha_preview_half: bool =False, display_rgb: bool =False, display_hsv: bool =False, display_hex: bool =False, uint8: bool =False, floats: bool =False, input_rgb: bool =False, input_hsv: bool =False, m_3component: bool =False) -> int:
	"""Undocumented function"""
	...

def add_color_picker(default_value : List[int] =(0, 0, 0, 255), *, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_alpha: bool =False, no_picker: bool =False, no_side_preview: bool =False, no_small_preview: bool =False, no_inputs: bool =False, no_tooltip: bool =False, no_label: bool =False, alpha_bar: bool =False, alpha_preview: bool =False, alpha_preview_half: bool =False, display_rgb: bool =False, display_hsv: bool =False, display_hex: bool =False, uint8: bool =False, floats: bool =False, input_rgb: bool =False, input_hsv: bool =False, picker_hue_bar: bool =False, picker_hue_wheel: bool =False) -> int:
	"""Undocumented function"""
	...

def add_colormap_scale(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, pos: List[int] =[], default_value: int =0, min_scale: float =0.0, max_scale: float =1.0) -> int:
	"""Undocumented function"""
	...

def add_combo(items : List[str] =(), *, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', popup_align_left: bool =False, height_small: bool =False, height_regular: bool =False, height_large: bool =False, height_largest: bool =False, no_arrow_button: bool =False, no_preview: bool =False) -> int:
	"""Undocumented function"""
	...

def add_date_picker(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: dict ={'month_day': 14, 'year':20, 'month':5}, level: int =0) -> int:
	"""Undocumented function"""
	...

def add_deactivated_after_edit_handler(parent : str, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_deactivated_handler(parent : str, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_drag_float(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, size: int =4, format: str ='%0.3f', speed: float =1.0, min_value: float =0.0, max_value: float =100.0, no_input: bool =False, clamped: bool =False) -> int:
	"""Undocumented function"""
	...

def add_drag_floatx(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), size: int =4, format: str ='%0.3f', speed: float =1.0, min_value: float =0.0, max_value: float =100.0, no_input: bool =False, clamped: bool =False) -> int:
	"""Undocumented function"""
	...

def add_drag_int(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: int =0, size: int =4, format: str ='%d', speed: float =1.0, min_value: int =0, max_value: int =100, no_input: bool =False, clamped: bool =False) -> int:
	"""Undocumented function"""
	...

def add_drag_intx(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[int] =(0, 0, 0, 0), size: int =4, format: str ='%d', speed: float =1.0, min_value: int =0, max_value: int =100, no_input: bool =False, clamped: bool =False) -> int:
	"""Undocumented function"""
	...

def add_drag_line(*, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, callback: Callable =None, show: bool =True, default_value: Any =(0.0, 0.0, 0.0, 0.0), color: List[int] =(0, 0, 0, -255), thickness: float =1.0, show_label: bool =True, vertical: bool =True) -> int:
	"""Undocumented function"""
	...

def add_drag_payload(*, id: int =..., parent: int =0, show: bool =True, drag_data: Any =None, payload_type: str ='$$DPG_PAYLOAD') -> int:
	"""Undocumented function"""
	...

def add_drag_point(*, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, callback: Callable =None, show: bool =True, default_value: Any =(0.0, 0.0, 0.0, 0.0), color: List[int] =(0, 0, 0, -255), thickness: float =1.0, show_label: bool =True) -> int:
	"""Undocumented function"""
	...

def add_draw_layer(*, id: int =..., parent: int =0, before: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_drawlist(*, id: int =..., width: int =0, height: int =0, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5) -> int:
	"""Undocumented function"""
	...

def add_dummy(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[]) -> int:
	"""Undocumented function"""
	...

def add_dynamic_texture(width : int, height : int, default_value : List[float], *, id: int =..., parent: int =0) -> int:
	"""Undocumented function"""
	...

def add_edited_handler(parent : str, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_error_series(x : List[float], y : List[float], negative : List[float], positive : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, contribute_to_bounds: bool =True, horizontal: bool =False) -> int:
	"""Undocumented function"""
	...

def add_file_dialog(*, id: int =..., width: int =0, height: int =0, label: str =None, callback: Callable =None, show: bool =True, default_path: str ='', default_filename: str ='.', file_count: int =0, modal: bool =False, directory_selector: bool =False) -> int:
	"""Undocumented function"""
	...

def add_file_extension(extension : str, *, id: int =..., width: int =0, height: int =0, parent: int =0, before: int =0, label: str =None, custom_text: str ='', color: List[float] =(-255, 0, 0, 255)) -> int:
	"""Undocumented function"""
	...

def add_filter_set(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_focus_handler(parent : str, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_font(font : str, file : str, size : float, glyph_ranges : str ='', *, custom_glyph_chars: List[int] =[], custom_glyph_ranges: Any =[[]], char_remaps: Any =[[]]) -> None:
	"""Undocumented function"""
	...

def add_group(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, horizontal: bool =False, horizontal_spacing: float =-1) -> int:
	"""Undocumented function"""
	...

def add_handler_registry(*, id: int =..., show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_heat_series(x : List[float], rows : int, cols : int, *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, scale_min: float =0.0, scale_max: float =1.0, bounds_min: Any =(0.0, 0.0), bounds_max: Any =(1.0, 1.0), format: str ='%0.1f', contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_histogram_series(x : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, bins: int =-1, bar_scale: float =1.0, min_range: float =0.0, max_range: float =1.0, cumlative: bool =False, density: bool =False, outliers: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_hline_series(x : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_hover_handler(parent : str, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_image(default_value : str, *, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, tint_color: List[float] =(255, 255, 255, 255), border_color: List[float] =(0, 0, 0, 0), uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0)) -> int:
	"""Undocumented function"""
	...

def add_image_button(default_value : int, *, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, frame_padding: int =-1, tint_color: List[float] =(255, 255, 255, 255), background_color: List[float] =(0, 0, 0, 0), uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0)) -> int:
	"""Undocumented function"""
	...

def add_image_series(value : int, bounds_min : List[float], bounds_max : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0), tint_color: List[int] =(255, 255, 255, 255), contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_input_float(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, format: str ='%.3f', min_value: float =0.0, max_value: float =100.0, step: float =0.1, step_fast: float =1.0, size: int =4, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Undocumented function"""
	...

def add_input_floatx(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), format: str ='%.3f', min_value: float =0.0, max_value: float =100.0, size: int =4, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Undocumented function"""
	...

def add_input_int(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[int] =(0, 0, 0, 0), min_value: int =0, max_value: int =100, size: int =4, step: int =1, step_fast: int =100, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Undocumented function"""
	...

def add_input_intx(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[int] =(0, 0, 0, 0), min_value: int =0, max_value: int =100, size: int =4, min_clamped: bool =False, max_clamped: bool =False, on_enter: bool =False, readonly: bool =False) -> int:
	"""Undocumented function"""
	...

def add_input_text(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', hint: str ='', multiline: bool =False, no_spaces: bool =False, uppercase: bool =False, tab_input: bool =False, decimal: bool =False, hexadecimal: bool =False, readonly: bool =False, password: bool =False, scientific: bool =False, on_enter: bool =False) -> int:
	"""Undocumented function"""
	...

def add_key_down_handler(key : int =-1, *, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_key_press_handler(key : int =-1, *, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_key_release_handler(key : int =-1, *, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_knob_float(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, min_value: float =0.0, max_value: float =100.0) -> int:
	"""Undocumented function"""
	...

def add_line_series(x : List[float], y : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_listbox(items : List[str] =(), *, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', num_items: int =3) -> int:
	"""Undocumented function"""
	...

def add_loading_indicator(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, show: bool =True, pos: List[int] =[], style: int =0) -> int:
	"""Undocumented function"""
	...

def add_menu(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, enabled: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5) -> int:
	"""Undocumented function"""
	...

def add_menu_bar(*, id: int =..., indent: int =-1, parent: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_menu_item(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: bool =False, shortcut: str ='', check: bool =False) -> int:
	"""Undocumented function"""
	...

def add_mouse_click_handler(button : int =-1, *, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_mouse_double_click_handler(button : int =-1, *, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_mouse_down_handler(button : int =-1, *, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_mouse_drag_handler(button : int =-1, threshold : float =10.0, *, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_mouse_move_handler(*, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_mouse_release_handler(button : int =-1, *, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_mouse_wheel_handler(*, id: int =..., parent: int =0, callback: Callable =None, user_data: Any =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_node(*, id: int =..., parent: int =0, before: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, draggable: bool =True) -> int:
	"""Undocumented function"""
	...

def add_node_attribute(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, output: bool =False, static: bool =False, shape: int =54010) -> int:
	"""Undocumented function"""
	...

def add_node_editor(*, id: int =..., parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, delink_callback: Callable =None) -> int:
	"""Undocumented function"""
	...

def add_node_link(node_1 : int, node_2 : int, *, id: int =..., parent: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_pie_series(x : float, y : float, radius : float, values : List[float], labels : List[str], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, format: str ='%0.2f', angle: float =90.0, normalize: bool =False, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_plot(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_title: bool =False, no_menus: bool =False, no_box_select: bool =False, no_mouse_pos: bool =False, no_highlight: bool =False, no_child: bool =False, query: bool =False, crosshairs: bool =False, anti_aliased: bool =False, equal_aspects: bool =False) -> int:
	"""Undocumented function"""
	...

def add_plot_annotation(*, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, default_value: Any =(0.0, 0.0), offset: List[float] =(0.0, 0.0), color: List[int] =(0, 0, 0, -255), clamped: bool =True) -> int:
	"""Undocumented function"""
	...

def add_plot_axis(axis : int, *, id: int =..., parent: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, no_gridlines: bool =False, no_tick_marks: bool =False, no_tick_labels: bool =False, log_scale: bool =False, invert: bool =False, lock_min: bool =False, lock_max: bool =False, time: bool =False) -> int:
	"""Undocumented function"""
	...

def add_plot_legend(*, id: int =..., parent: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_popup(*, id: int =..., width: int =0, height: int =0, parent: int =0, show: bool =True, pos: List[int] =[], mousebutton: int =1, modal: bool =False) -> int:
	"""Undocumented function"""
	...

def add_progress_bar(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, overlay: str ='', default_value: float =0.0) -> int:
	"""Undocumented function"""
	...

def add_radio_button(items : int =(), *, id: int =..., indent: int =-1, parent: int =0, before: int =0, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: str ='', horizontal: bool =False) -> int:
	"""Undocumented function"""
	...

def add_same_line(*, id: int =..., parent: int =0, before: int =0, show: bool =True, xoffset: float =0.0, spacing: float =-1.0) -> int:
	"""Undocumented function"""
	...

def add_scatter_series(x : List[float], y : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_selectable(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: bool =False, span_columns: bool =False) -> int:
	"""Undocumented function"""
	...

def add_separator(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[]) -> int:
	"""Undocumented function"""
	...

def add_shade_series(x : List[float], y1 : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, y2: Any =[], contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_simple_plot(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(), overlay: str ='', histogram: bool =False, autosize: bool =True, min_scale: float =0.0, max_scale: float =0.0) -> int:
	"""Undocumented function"""
	...

def add_slider_float(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: float =0.0, size: int =4, vertical: bool =False, no_input: bool =False, clamped: bool =False, min_value: float =0.0, max_value: float =100.0, format: str ='%.3f') -> int:
	"""Undocumented function"""
	...

def add_slider_floatx(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[float] =(0.0, 0.0, 0.0, 0.0), size: int =4, no_input: bool =False, clamped: bool =False, min_value: float =0.0, max_value: float =100.0, format: str ='%.3f') -> int:
	"""Undocumented function"""
	...

def add_slider_int(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: int =0, size: int =4, vertical: bool =False, no_input: bool =False, clamped: bool =False, min_value: int =0, max_value: int =100, format: str ='%d') -> int:
	"""Undocumented function"""
	...

def add_slider_intx(*, id: int =..., width: int =0, indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, enabled: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: List[int] =(0, 0, 0, 0), size: int =4, no_input: bool =False, clamped: bool =False, min_value: int =0, max_value: int =100, format: str ='%d') -> int:
	"""Undocumented function"""
	...

def add_spacing(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, show: bool =True, pos: List[int] =[], count: int =1) -> int:
	"""Undocumented function"""
	...

def add_staging_container(*, id: int =...) -> int:
	"""Undocumented function"""
	...

def add_stair_series(x : List[float], y : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_static_texture(width : int, height : int, default_value : List[float], *, id: int =..., parent: int =0, file: str ='') -> int:
	"""Undocumented function"""
	...

def add_stem_series(x : List[float], y : List[float], *, id: int =..., indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_tab(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, closable: bool =False, no_reorder: bool =False, leading: bool =False, trailing: bool =False, no_tooltip: bool =False) -> int:
	"""Undocumented function"""
	...

def add_tab_bar(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, reorderable: bool =False) -> int:
	"""Undocumented function"""
	...

def add_tab_button(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, filter_key: str ='', tracked: bool =False, track_offset: float =0.5, no_reorder: bool =False, leading: bool =False, trailing: bool =False, no_tooltip: bool =False) -> int:
	"""Undocumented function"""
	...

def add_table(*, id: int =..., width: int =0, height: int =0, indent: int =-1, parent: int =0, before: int =0, source: int =0, callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', header_row: bool =True, inner_width: int =0, policy: int =0, freeze_rows: int =0, freeze_columns: int =0, sort_multi: bool =False, sort_tristate: bool =False, resizable: bool =False, reorderable: bool =False, hideable: bool =False, sortable: bool =False, context_menu_in_body: bool =False, row_background: bool =False, borders_innerH: bool =False, borders_outerH: bool =False, borders_innerV: bool =False, borders_outerV: bool =False, no_host_extendX: bool =False, no_host_extendY: bool =False, no_keep_columns_visible: bool =False, precise_widths: bool =False, no_clip: bool =False, pad_outerX: bool =False, no_pad_outerX: bool =False, no_pad_innerX: bool =False, scrollX: bool =False, scrollY: bool =False) -> int:
	"""Undocumented function"""
	...

def add_table_column(*, id: int =..., width: int =0, parent: int =0, before: int =0, label: str =None, show: bool =True, init_width_or_weight: bool =0.0, default_hide: bool =False, default_sort: bool =False, width_stretch: bool =False, width_fixed: bool =False, no_resize: bool =False, no_reorder: bool =False, no_hide: bool =False, no_clip: bool =False, no_sort: bool =False, no_sort_ascending: bool =False, no_sort_descending: bool =False, no_header_width: bool =False, prefer_sort_ascending: bool =False, prefer_sort_descending: bool =False, indent_enable: bool =False, indent_disable: bool =False) -> int:
	"""Undocumented function"""
	...

def add_table_next_column(*, id: int =..., parent: int =0, before: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_table_row(*, id: int =..., height: int =0, parent: int =0, before: int =0, show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_text(default_value : str ='', *, id: int =..., indent: int =-1, parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, wrap: int =-1, bullet: bool =False, color: List[float] =(-1, -1, -1, -1), show_label: bool =False) -> int:
	"""Undocumented function"""
	...

def add_text_point(x : float, y : float, *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, x_offset: int =..., y_offset: int =..., contribute_to_bounds: bool =True, vertical: bool =False) -> int:
	"""Undocumented function"""
	...

def add_texture_container(*, id: int =..., show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_time_picker(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, payload_type: str ='$$DPG_PAYLOAD', callback: Callable =None, drag_callback: Callable =None, drop_callback: Callable =None, user_data: Any =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_value: dict ={'hour': 14, 'min': 32, 'sec': 23}, hour24: bool =False) -> int:
	"""Undocumented function"""
	...

def add_toggled_open_handler(parent : str, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_tooltip(*, id: int =..., show: bool =True) -> int:
	"""Undocumented function"""
	...

def add_tree_node(*, id: int =..., indent: int =-1, parent: int =0, before: int =0, label: str =None, payload_type: str ='$$DPG_PAYLOAD', drag_callback: Callable =None, drop_callback: Callable =None, show: bool =True, pos: List[int] =[], filter_key: str ='', tracked: bool =False, track_offset: float =0.5, default_open: bool =False, open_on_double_click: bool =False, open_on_arrow: bool =False, leaf: bool =False, bullet: bool =False, selectable: bool =False) -> int:
	"""Undocumented function"""
	...

def add_viewport_drawlist(*, id: int =..., show: bool =True, filter_key: str ='', front: bool =True) -> int:
	"""Undocumented function"""
	...

def add_visible_handler(parent : str, *, id: int =..., callback: Callable =None, user_data: Any =None) -> int:
	"""Undocumented function"""
	...

def add_vline_series(x : List[float], *, id: int =..., parent: int =0, before: int =0, label: str =None, source: int =0, show: bool =True, contribute_to_bounds: bool =True) -> int:
	"""Undocumented function"""
	...

def add_window(*, id: int =..., width: int =0, height: int =0, indent: int =-1, label: str =None, user_data: Any =None, show: bool =True, min_size: List[int] =[32, 32], max_size: List[int] =[30000, 30000], menubar: bool =False, collapsed: bool =False, autosize: bool =False, no_resize: bool =False, no_title_bar: bool =False, no_move: bool =False, no_scrollbar: bool =False, no_collapse: bool =False, horizontal_scrollbar: bool =False, no_focus_on_appearing: bool =False, no_bring_to_front_on_focus: bool =False, no_close: bool =False, no_background: bool =False, modal: bool =False, popup: bool =False, on_close: Callable =None) -> int:
	"""Undocumented function"""
	...

def cleanup_dearpygui() -> None:
	"""Undocumented function"""
	...

def clear_selected_links(node_editor : int) -> None:
	"""Undocumented function"""
	...

def clear_selected_nodes(node_editor : int) -> None:
	"""Undocumented function"""
	...

def close_popup(item : int) -> None:
	"""Undocumented function"""
	...

def configure_item(item : int, **kwargs) -> None:
	"""Undocumented function"""
	...

def configure_viewport() -> None:
	"""Undocumented function"""
	...

def create_viewport(*, title: str ='Dear PyGui', small_icon: str ='', large_icon: str ='', width: str =1280, height: str =800, x_pos: str =100, y_pos: str =100, min_width: str =250, max_width: str =10000, min_height: str =250, max_height: str =10000, resizable: bool =True, vsync: bool =True, always_on_top: bool =False, maximized_box: bool =True, minimized_box: bool =True, border: bool =True, caption: bool =True, overlapped: bool =True, clear_color: List[float] =(0, 0, 0, 255)) -> str:
	"""Undocumented function"""
	...

def delete_item(item : int, *, children_only: bool =False) -> None:
	"""Undocumented function"""
	...

def does_item_exist(item : int) -> bool:
	"""Undocumented function"""
	...

def draw_arrow(p1 : List[float], p2 : List[float], *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0, size: int =4) -> int:
	"""Undocumented function"""
	...

def draw_bezier_cubic(p1 : List[float], p2 : List[float], p3 : List[float], p4 : List[float], *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0, size: int =4, segments: int =0) -> int:
	"""Undocumented function"""
	...

def draw_bezier_quadratic(p1 : List[float], p2 : List[float], p3 : List[float], *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0, size: int =4, segments: int =0) -> int:
	"""Undocumented function"""
	...

def draw_circle(center : List[float], radius : float, *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0, segments: int =0) -> int:
	"""Undocumented function"""
	...

def draw_ellipse(pmin : List[float], pmax : List[float], *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0, segments: int =32) -> int:
	"""Undocumented function"""
	...

def draw_image(file : str, pmin : List[float], pmax : List[float], *, id: int =..., parent: int =0, before: int =0, show: bool =True, uv_min: List[float] =(0.0, 0.0), uv_max: List[float] =(1.0, 1.0), color: List[int] =(255, 255, 255, 255)) -> int:
	"""Undocumented function"""
	...

def draw_line(p1 : List[float], p2 : List[float], *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), thickness: float =1.0) -> int:
	"""Undocumented function"""
	...

def draw_polygon(points : List[List[float]], *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0) -> int:
	"""Undocumented function"""
	...

def draw_polyline(points : List[List[float]], *, id: int =..., parent: int =0, before: int =0, show: bool =True, closed: bool =False, color: List[int] =(255, 255, 255, 255), thickness: float =1.0) -> int:
	"""Undocumented function"""
	...

def draw_quad(p1 : List[float], p2 : List[float], p3 : List[float], p4 : List[float], *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0) -> int:
	"""Undocumented function"""
	...

def draw_rectangle(pmin : List[float], pmax : List[float], *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), rounding: float =0.0, thickness: float =1.0) -> int:
	"""Undocumented function"""
	...

def draw_text(pos : List[float], text : str, *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), size: int =10) -> int:
	"""Undocumented function"""
	...

def draw_triangle(p1 : List[float], p2 : List[float], p3 : List[float], *, id: int =..., parent: int =0, before: int =0, show: bool =True, color: List[int] =(255, 255, 255, 255), fill: List[int] =(0, 0, 0, -255), thickness: float =1.0) -> int:
	"""Undocumented function"""
	...

def empty_container_stack() -> None:
	"""Undocumented function"""
	...

def enable_docking(*, dock_space: bool =False) -> None:
	"""Undocumented function"""
	...

def focus_item(item : int) -> None:
	"""Undocumented function"""
	...

def generate_uuid() -> int:
	"""Undocumented function"""
	...

def get_active_window() -> int:
	"""Undocumented function"""
	...

def get_all_items() -> List[int]:
	"""Undocumented function"""
	...

def get_axis_limits(axis : int) -> List[float]:
	"""Undocumented function"""
	...

def get_dearpygui_version() -> str:
	"""Undocumented function"""
	...

def get_delta_time() -> float:
	"""Undocumented function"""
	...

def get_drawing_mouse_pos() -> List[int]:
	"""Undocumented function"""
	...

def get_file_dialog_info(file_dialog : int) -> dict:
	"""Undocumented function"""
	...

def get_global_font_scale() -> float:
	"""Undocumented function"""
	...

def get_item_configuration(item : int) -> dict:
	"""Undocumented function"""
	...

def get_item_info(item : int) -> dict:
	"""Undocumented function"""
	...

def get_item_state(item : int) -> dict:
	"""Undocumented function"""
	...

def get_links(node_editor : int) -> List[List[str]]:
	"""Undocumented function"""
	...

def get_mouse_drag_delta() -> float:
	"""Undocumented function"""
	...

def get_mouse_pos(*, local: bool =True) -> List[int]:
	"""Undocumented function"""
	...

def get_plot_mouse_pos() -> List[int]:
	"""Undocumented function"""
	...

def get_plot_query_area(plot : int) -> List[float]:
	"""Undocumented function"""
	...

def get_selected_links(node_editor : int) -> List[List[str]]:
	"""Undocumented function"""
	...

def get_selected_nodes(node_editor : int) -> List[int]:
	"""Undocumented function"""
	...

def get_total_time() -> float:
	"""Undocumented function"""
	...

def get_value(item : int) -> Any:
	"""Undocumented function"""
	...

def get_values(items : unknown) -> Any:
	"""Undocumented function"""
	...

def get_windows() -> List[int]:
	"""Undocumented function"""
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
	"""Undocumented function"""
	...

def is_key_down(key : int) -> bool:
	"""Undocumented function"""
	...

def is_key_pressed(key : int) -> bool:
	"""Undocumented function"""
	...

def is_key_released(key : int) -> bool:
	"""Undocumented function"""
	...

def is_mouse_button_clicked(button : int) -> bool:
	"""Undocumented function"""
	...

def is_mouse_button_double_clicked(button : int) -> bool:
	"""Undocumented function"""
	...

def is_mouse_button_down(button : int) -> bool:
	"""Undocumented function"""
	...

def is_mouse_button_dragging(button : int, threshold : float) -> bool:
	"""Undocumented function"""
	...

def is_mouse_button_released(button : int) -> bool:
	"""Undocumented function"""
	...

def is_plot_queried(plot : int) -> bool:
	"""Undocumented function"""
	...

def last_container() -> int:
	"""Undocumented function"""
	...

def last_item() -> int:
	"""Undocumented function"""
	...

def last_root() -> int:
	"""Undocumented function"""
	...

def maximize_viewport() -> None:
	"""Undocumented function"""
	...

def minimize_viewport() -> None:
	"""Undocumented function"""
	...

def move_item(item : int, *, parent: int =0, before: int =0) -> None:
	"""Undocumented function"""
	...

def move_item_down(item : int) -> None:
	"""Undocumented function"""
	...

def move_item_up(item : int) -> None:
	"""Undocumented function"""
	...

def pop_container_stack() -> int:
	"""Undocumented function"""
	...

def push_container_stack(item : int) -> bool:
	"""Undocumented function"""
	...

def render_dearpygui_frame() -> None:
	"""Undocumented function"""
	...

def reorder_items(container : int, slot : int, new_order : unknown) -> None:
	"""Undocumented function"""
	...

def reset_axis_ticks(axis : int) -> None:
	"""Undocumented function"""
	...

def reset_pos(item : int) -> None:
	"""Undocumented function"""
	...

def set_axis_limits(axis : int, ymin : float, ymax : float) -> None:
	"""Undocumented function"""
	...

def set_axis_ticks(axis : int, label_pairs : Any) -> None:
	"""Undocumented function"""
	...

def set_exit_callback(callback : Callable) -> str:
	"""Undocumented function"""
	...

def set_font(font : str, size : float, *, item: int =0) -> None:
	"""Undocumented function"""
	...

def set_global_font_scale(scale : float) -> None:
	"""Undocumented function"""
	...

def set_item_pos(item : int, x : float, y : float) -> None:
	"""Undocumented function"""
	...

def set_primary_window(window : int, value : bool) -> None:
	"""Undocumented function"""
	...

def set_resize_callback(callback : Callable, *, handler: int ='') -> str:
	"""Undocumented function"""
	...

def set_staging_mode(mode : bool) -> None:
	"""Undocumented function"""
	...

def set_start_callback(callback : Callable) -> str:
	"""Undocumented function"""
	...

def set_theme_color(constant : int, color : List[float], *, item: int =0) -> str:
	"""Undocumented function"""
	...

def set_theme_color_disabled(constant : int, color : List[float], *, item: int =0) -> str:
	"""Undocumented function"""
	...

def set_theme_style(constant : int, style : float, *, item: int =0) -> str:
	"""Undocumented function"""
	...

def set_value(item : int, value : Any) -> None:
	"""Undocumented function"""
	...

def set_x_scroll(item : int, value : float) -> None:
	"""Undocumented function"""
	...

def set_y_scroll(item : int, value : float) -> None:
	"""Undocumented function"""
	...

def setup_dearpygui(*, viewport: str ='') -> None:
	"""Undocumented function"""
	...

def show_tool(tool : int) -> str:
	"""Undocumented function"""
	...

def show_viewport(viewport : str, *, minimized: bool =False, maximized: bool =False) -> None:
	"""Undocumented function"""
	...

def stage_items(items : unknown) -> None:
	"""Undocumented function"""
	...

def stop_dearpygui() -> None:
	"""Undocumented function"""
	...

def top_container_stack() -> int:
	"""Undocumented function"""
	...

def unstage_items(items : unknown) -> None:
	"""Undocumented function"""
	...

