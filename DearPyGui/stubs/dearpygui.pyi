from typing import List, Any

def add_button(name: str, small: bool = False, arrow: bool = False, direction: int = -1, callback: str = "", tip: str = "", parent: str = "", before: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds a button."""
	...

def add_checkbox(name: str, default_value: int = 0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "") -> None:
	"""Adds a checkbox widget."""
	...

def add_child(name: str, tip: str = "", parent: str = "", before: str = "", width: int = 0, height: int = 0, border: bool = True) -> None:
	"""Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end_child."""
	...

def add_collapsing_header(name: str, default_open: bool = False, closable: bool = False, tip: str = "", parent: str = "", before: str = "") -> None:
	"""Adds a collapsing header to add items to. Must be closed with the end_collapsing_header command."""
	...

def add_color_edit3(name: str, default_value: List[int] = [0, 0, 0], callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds an rgb color editing widget."""
	...

def add_color_edit4(name: str, default_value: List[int] = [0,0,0,0], callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds an rgba color editing widget."""
	...

def add_color_picker3(name: str, default_value: List[int] = [0, 0, 0], callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds an rgb color picking widget."""
	...

def add_color_picker4(name: str, default_value: List[int] = [0, 0, 0, 0], callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds an rgba color picking widget."""
	...

def add_column(table: str, name: str, column: List[str]) -> None:
	"""Adds a column to a table."""
	...

def add_combo(name: str, items: List[str], default_value: str = "", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0, secondary_data_source: str = "") -> None:
	"""Adds a combo."""
	...

def add_data(name: str, data: object) -> None:
	"""Adds data for later retrieval."""
	...

def add_drag_float(name: str, default_value: float = 0.0, speed: float = 1.0, min_value: float = 0.0, max_value: float = 1.0, format: str = "%.3f", power: float = 1.0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds drag for a single float value"""
	...

def add_drag_float2(name: str, default_value: List[float] = [0.0,0.0], speed: float = 1.0, min_value: float = 0.0, max_value: float = 1.0, format: str = "%.3f", power: float = 1.0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds drag for a 2 float values."""
	...

def add_drag_float3(name: str, default_value: List[float] = [0.0, 0.0, 0.0], speed: float = 1.0, min_value: float = 0.0, max_value: float = 1.0, format: str = "%.3f", power: float = 1.0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds drag for a 3 float values."""
	...

def add_drag_float4(name: str, default_value: List[float] = [0.0, 0.0, 0.0, 0.0], speed: float = 1.0, min_value: float = 0.0, max_value: float = 1.0, format: str = "%.3f", power: float = 1.0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds drag for a 4 float values."""
	...

def add_drag_int(name: str, default_value: int = 0, speed: float = 1.0, min_value: int = 0, max_value: int = 100, format: str = "%d", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds drag for a single int value"""
	...

def add_drag_int2(name: str, default_value: List[int] = [0, 0], speed: float = 1.0, min_value: int = 0, max_value: int = 100, format: str = "%d", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds drag for a 2 int values."""
	...

def add_drag_int3(name: str, default_value: List[int] = [0, 0, 0], speed: float = 1.0, min_value: int = 0, max_value: int = 100, format: str = "%d", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds drag for a 3 int values."""
	...

def add_drag_int4(name: str, default_value: List[int] = [0, 0, 0, 0], speed: float = 1.0, min_value: int = 0, max_value: int = 100, format: str = "%d", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds drag for a 4 int values."""
	...

def add_drawing(name: str, tip: str = "", parent: str = "", before: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds a drawing widget."""
	...

def add_group(name: str, tip: str = "", parent: str = "", before: str = "", width: int = 0, hide: bool = False, horizontal: bool = False, horizontal_spacing: float = -1.0) -> None:
	"""Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.				Must be closed with the end_group command unless added at runtime."""
	...

def add_image(name: str, value: str, tint_color: List[float] = [1.0, 1.0, 1.0, 1.0], border_color: List[float] = [0.0, 0.0, 0.0, 0.0], tip: str = "", parent: str = "", before: str = "", data_source: str = "", 
			  width: int = 0, height: int = 0, uv_min: List[float] = [0.0, 0.0], uv_max: List[float] = [1.0, 1.0], secondary_data_source: str = "") -> None:
	"""Adds an image."""
	...

def add_indent(name: str = "", offset: float = 0.0, parent: str = "", before: str = "") -> None:
	"""Adds an indent to following items."""
	...

def add_input_float(name: str, default_value: float = 0.0, format: str = "%.3f", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds input for float values."""
	...

def add_input_float2(name: str, default_value: List[float] = [0.0, 0.0], format: str = "%.3f", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds input for 2 float values."""
	...

def add_input_float3(name: str, default_value: List[float] = [0.0, 0.0, 0.0], format: str = "%.3f", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds input for 3 float values."""
	...

def add_input_float4(name: str, default_value: List[float] = [0.0, 0.0, 0.0, 0.0], format: str = "%.3f", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds input for 4 float values."""
	...

def add_input_int(name: str, default_value: int = 0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds input for integer values."""
	...

def add_input_int2(name: str, default_value: List[int] = [0, 0], callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds input for 2 integer values."""
	...

def add_input_int3(name: str, default_value: List[int] = [0, 0, 0], callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds input for 3 integer values."""
	...

def add_input_int4(name: str, default_value: List[int] = [0, 0, 0, 0], callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds input for 4 integer values."""
	...

def add_input_text(name: str, default_value: str = "", hint: str = "", multiline: bool = False, no_spaces: bool = False, uppercase: bool = False, decimal: bool = False, hexadecimal: bool = False, 
				   readonly: bool = False, password: bool = False, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds input for text values."""
	...

def add_item_color_style(item: str, style: int, color: List[float]) -> None:
	"""Needs documentation"""
	...

def add_label_text(name: str, value: str, color: List[float] = [0.0, 0.0, 0.0, 255], tip: str = "", parent: str = "", before: str = "", data_source: str = "") -> None:
	"""Adds text with a label. Useful for output values."""
	...

def add_line_series(plot: str, name: str, data: List[float], color: List[float] = ..., fill: List[float] = ..., weight: float = 1.0) -> None:
	"""Adds a line series to a plot."""
	...

def add_listbox(name: str, items: List[str], default_value: int = 0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0, height: int = 0, secondary_data_source: str = "") -> None:
	"""Adds a listbox."""
	...

def add_menu(name: str, tip: str = "", parent: str = "", before: str = "") -> None:
	"""Adds a menu to an existing menu bar. Must be followed by a call to end_menu."""
	...

def add_menu_bar(name: str, parent: str = "", before: str = "") -> None:
	"""Adds a menu bar to a window. Must be followed by a call to end_menu_bar."""
	...

def add_menu_item(name: str, callback: str = "", tip: str = "", parent: str = "", before: str = "") -> None:
	"""Adds a menu item to an existing menu."""
	...

def add_plot(name: str, xAxisName: str = "", yAxisName: str = "", flags: int = 0, xflags: int = 0, yflags: int = 0, parent: str = "", before: str = "", width: int = -1, height: int = -1, query_callback: str = "") -> None:
	"""Adds a plot widget."""
	...

def add_popup(popupparent: str, name: str, mousebutton: int = 1, modal: bool = False, parent: str = "", before: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds a popup window for an item. This command must come immediately after the item the popup is for."""
	...

def add_progress_bar(name: str, value: float = 0.0, overlay: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds a progress bar."""
	...

def add_radio_button(name: str, items: List[str], default_value: int = 0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", secondary_data_source: str = "") -> None:
	"""Adds a set of radio buttons."""
	...

def add_row(table: str, row: List[str]) -> None:
	"""Adds a row to a table."""
	...

def add_same_line(name: str = "", xoffset: float = 0.0, spacing: float = -1.0, parent: str = "", before: str = "") -> None:
	"""Places a widget on the same line as the previous widget. Can also be used for horizontal spacing."""
	...

def add_scatter_series(plot: str, name: str, data: List[float], marker: int = 2, size: float = 4.0, weight: float = 1.0, outline: List[float] = ..., fill: List[float] = ...) -> None:
	"""Adds a scatter series to a plot."""
	...

def add_selectable(name: str, default_value: bool = False, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "") -> None:
	"""Adds a selectable."""
	...

def add_seperator(name: str = "", tip: str = "", parent: str = "", before: str = "") -> None:
	"""Adds a horizontal line."""
	...

def add_simple_plot(name: str, value: List[float], autoscale: bool = True, overlay: str = "", minscale: float = 0.0, maxscale: float = 0.0, histogram: bool = False, tip: str = "", parent: str = "", before: str = "", width: int = 0, height: int = 0) -> None:
	"""A simple plot for visualization of a set of values"""
	...

def add_slider_float(name: str, default_value: float = 0.0, min_value: float = 0.0, max_value: float = 1.0, format: str = "%.3f", power: float = 1.0, vertical: bool = ..., callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds slider for a single float value"""
	...

def add_slider_float2(name: str, default_value: List[float] = [0.0, 0.0], min_value: float = 0.0, max_value: float = 1.0, format: str = "%.3f", power: float = 1.0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds slider for a 2 float values."""
	...

def add_slider_float3(name: str, default_value: List[float] = [0.0, 0.0, 0.0], min_value: float = 0.0, max_value: float = 1.0, format: str = "%.3f", power: float = 1.0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds slider for a 3 float values."""
	...

def add_slider_float4(name: str, default_value: List[float] = [0.0, 0.0, 0.0, 0.0], min_value: float = 0.0, max_value: float = 1.0, format: str = "%.3f", power: float = 1.0, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds slider for a 4 float values."""
	...

def add_slider_int(name: str, default_value: int = 0, min_value: int = 0, max_value: int = 100, format: str = "%d", vertical: bool = False, callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0, height: int = 0) -> None:
	"""Adds slider for a single int value"""
	...

def add_slider_int2(name: str, default_value: List[int] = [0, 0], min_value: int = 0, max_value: int = 100, format: str = "%d", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds slider for a 2 int values."""
	...

def add_slider_int3(name: str, default_value: List[int] = [0, 0, 0], min_value: int = 0, max_value: int = 100, format: str = "%d", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds slider for a 3 int values."""
	...

def add_slider_int4(name: str, default_value: List[int] = [0, 0, 0, 0], min_value: int = 0, max_value: int = 100, format: str = "%d", callback: str = "", tip: str = "", parent: str = "", before: str = "", data_source: str = "", width: int = 0) -> None:
	"""Adds slider for a 4 int values."""
	...

def add_spacing(name: str = "", count: int = 1, parent: str = "", before: str = "") -> None:
	"""Adds vertical spacing."""
	...

def add_tab(name: str, closable: bool = False, tip: str = "", parent: str = "", before: str = "") -> None:
	"""Adds a tab to a tab bar."""
	...

def add_tab_bar(name: str, reorderable: bool = False, callback: str = "", parent: str = "", before: str = "", data_source: str = "") -> None:
	"""Adds a tab bar."""
	...

def add_table(name: str, headers: List[str], callback: str = "", parent: str = "", before: str = "") -> None:
	"""Adds table."""
	...

def add_text(name: str, wrap: int = 0, color: List[float] = ..., bullet: bool = False, tip: str = "", parent: str = "", before: str = "") -> None:
	"""Adds text"""
	...

def add_text_point(plot: str, name: str, x: float, y: float, vertical: bool = False, xoffset: int = 0, yoffset: int = 0) -> None:
	"""Adds a point with text to a plot."""
	...

def add_tooltip(tipparent: str, name: str, parent: str = "", before: str = "") -> None:
	"""Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for."""
	...

def add_tree_node(name: str, default_open: bool = False, tip: str = "", parent: str = "", before: str = "") -> None:
	"""Adds a tree node to add items to. Must be closed with the end_tree_node command."""
	...

def add_window(name: str, width: int = -1, height: int = -1, start_x: int = 200, start_y: int = 200, autosize: bool = False, resizable: bool = True, title_bar: bool = True, 
			   movable: bool = True, hide: bool = False, on_close: str = "") -> None:
	"""Creates a new window for following items to be added to. Must call end_main_window command before."""
	...

def change_style_item(item: int, x: float, y: float = ...) -> None:
	"""Needs documentation"""
	...

def change_theme_item(item: int, r: float, g: float, b: float, a: float) -> None:
	"""Needs documentation"""
	...

def clear_drawing(draw: str) -> None:
	"""Clears a drawing."""
	...

def clear_log() -> None:
	"""Clears the logger."""
	...

def clear_plot(plot: str) -> None:
	"""Clears a plot."""
	...

def clear_table(table: str) -> None:
	"""Clears data in a table"""
	...

def close_popup() -> None:
	"""Needs documentation"""
	...

def delete_column(table: str, column: int) -> None:
	"""Delete a column in a table."""
	...

def delete_data(name: str) -> None:
	"""Deletes data from storage."""
	...

def delete_item(item: str, children_only: bool = False) -> None:
	"""Deletes an item if it exists."""
	...

def delete_row(table: str, row: int) -> None:
	"""Delete a row in a table."""
	...

def draw_arrow(drawing: str, p1: List[float], p2: List[float], color: List[int], thickness: int, size: int, tag: str = "") -> None:
	"""Draws an arrow on a drawing."""
	...

def draw_bezier_curve(drawing: str, p1: List[float], p2: List[float], p3: List[float], p4: List[float], color: List[int], thickness: float = 1.0, segments: int = 0, tag: str = "") -> None:
	"""Draws a bezier curve on a drawing."""
	...

def draw_circle(drawing: str, center: List[float], radius: float, color: List[int], segments: int = 12, thickness: float = 1, fill: List[float] = ..., tag: str = "") -> None:
	"""Draws a circle on a drawing."""
	...

def draw_image(drawing: str, file: str, pmin: List[float], pmax: List[float] = [-100.0, -100.0], uv_min: List[float] = [0.0, 0.0], uv_max: List[float] = [1.0, 1.0], color: List[int] = ..., tag: str = "") -> None:
	"""Draws an image on a drawing."""
	...

def draw_line(drawing: str, p1: List[float], p2: List[float], color: List[int], thickness: int, tag: str = "") -> None:
	"""Draws a line on a drawing."""
	...

def draw_polygon(drawing: str, points: List[float], color: List[int], fill: List[float] = ..., thickness: float = 1.0, tag: str = "") -> None:
	"""Draws a polygon on a drawing."""
	...

def draw_polyline(drawing: str, points: List[float], color: List[int], closed: int = False, thickness: float = 1.0, tag: str = "") -> None:
	"""Draws lines on a drawing."""
	...

def draw_quad(drawing: str, p1: List[float], p2: List[float], p3: List[float], p4: List[float], color: List[int], fill: List[float] = ..., thickness: float = 1.0, tag: str = "") -> None:
	"""Draws a quad on a drawing."""
	...

def draw_rectangle(drawing: str, pmin: List[float], pmax: List[float], color: List[int], fill: List[float] = ..., rounding: float = 0.0, thickness: float = 1.0, tag: str = "") -> None:
	"""Draws a rectangle on a drawing."""
	...

def draw_text(drawing: str, pos: List[float], text: str, color: List[int] = ..., size: int = 10, tag: str = "") -> None:
	"""Draws text on a drawing."""
	...

def draw_triangle(drawing: str, p1: List[float], p2: List[float], p3: List[float], color: List[int], fill: List[float] = ..., thickness: float = 1.0, tag: str = "") -> None:
	"""Draws a triangle on a drawing."""
	...

def end_child() -> None:
	"""Ends the child created by a call to add_child."""
	...

def end_collapsing_header() -> None:
	"""Ends the collapsing header created by a call to add_collapsing_header."""
	...

def end_group() -> None:
	"""Ends the group created by a call to add_group."""
	...

def end_menu() -> None:
	"""Ends the menu created by a call to add_menu."""
	...

def end_menu_bar() -> None:
	"""Ends the menu bar created by a call to add_menu_bar."""
	...

def end_popup() -> None:
	"""Ends the popup created by a call to add_popup."""
	...

def end_tab() -> None:
	"""Ends the tab created by a call to add_tab."""
	...

def end_tab_bar() -> None:
	"""Ends the tab bar created by a call to add_tab_bar"""
	...

def end_tooltip() -> None:
	"""Ends the tooltip created by a call to add_tooltip."""
	...

def end_tree_node() -> None:
	"""Ends the tree node created by a call to add_tree_node."""
	...

def end_window() -> None:
	"""Ends the window created by a call to add_window."""
	...

def get_active_window() -> str:
	"""Returns the active window name."""
	...

def get_data(name: str) -> object:
	"""Retrieves data from storage."""
	...

def get_delta_time() -> float:
	"""Returns time since last frame."""
	...

def get_drawing_origin(name: str) -> (float, float):
	"""Returns the drawing origin."""
	...

def get_drawing_scale(name: str) -> (float, float):
	"""Returns the drawing scale."""
	...

def get_drawing_size(name: str) -> (float, float):
	"""Returns the size of a drawing widget."""
	...

def get_global_font_scale() -> float:
	"""Returns the global font scale."""
	...

def get_item_callback(item: str) -> str:
	"""Returns an item' callback"""
	...

def get_item_height(item: str) -> float:
	"""Returns an item's height."""
	...

def get_item_popup(item: str) -> str:
	"""Returns an item's popup."""
	...

def get_item_rect_max(item: str) -> [float, float]:
	"""Returns an item's maximum allowable size."""
	...

def get_item_rect_min(item: str) -> [float, float]:
	"""Returns an item's minimum allowable size."""
	...

def get_item_rect_size(item: str) -> [float, float]:
	"""Returns an item's current size."""
	...

def get_item_tip(item: str) -> str:
	"""Returns an item's tip."""
	...

def get_item_width(item: str) -> float:
	"""Returns an item's width."""
	...

def get_log_level() -> int:
	"""Returns the log level."""
	...

def get_main_window_size() -> [float, float]:
	"""Returns the size of the main window."""
	...

def get_marvel_version() -> str:
	"""Returns the current version of Marvel."""
	...

def get_mouse_drag_delta() -> (float, float):
	"""Returns the current mouse drag delta"""
	...

def get_mouse_pos(local: bool = True) -> (int, int):
	"""Returns the current mouse position in relation to the active window (minus titlebar) unless local flag is unset."""
	...

def get_plot_query_area(plot: str) -> List[float]:
	"""Clears a plot."""
	...

def get_style_item(item: int) -> (float, float):
	"""Returns a style item's value"""
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

def get_thread_count() -> int:
	"""Returns the allowable thread count."""
	...

def get_threadpool_timeout() -> float:
	"""Returns the threadpool timeout in seconds."""
	...

def get_total_time() -> float:
	"""Returns total time since app started."""
	...

def get_value(name: str) -> Any:
	"""Returns an item's value or None if there is none."""
	...

def get_window_pos(window: str) -> List[float]:
	"""Gets a windows position"""
	...

def hide_item(name: str, children_only: bool = False) -> None:
	"""Hides an item."""
	...

def insert_column(table: str, column_index: int, name: str, column: List[str]) -> None:
	"""Inserts a column into a table."""
	...

def insert_row(table: str, row_index: int, row: List[str]) -> None:
	"""Inserts a row into a table."""
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

def is_item_toggled_open(item: str) -> bool:
	"""Checks if an item is toggled."""
	...

def is_item_visible(item: str) -> bool:
	"""Checks if an item is visible."""
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
	"""Clears a plot."""
	...

def is_threadpool_high_performance() -> bool:
	"""Checks if the threadpool is allowed to use the maximum number of threads."""
	...

def log(message: Any, level: str = "") -> None:
	"""Logs a trace level log."""
	...

def log_debug(message: Any) -> None:
	"""Logs a debug level log."""
	...

def log_error(message: Any) -> None:
	"""Logs a error level log."""
	...

def log_info(message: Any) -> None:
	"""Logs a info level log."""
	...

def log_warning(message: Any) -> None:
	"""Logs a warning level log."""
	...

def move_item_down(item: str) -> None:
	"""Moves down up if possible and if it exists."""
	...

def move_item_up(item: str) -> None:
	"""Moves item up if possible and if it exists."""
	...

def open_file_dialog(callback: str = "", extensions: str = ".*") -> str:
	"""Opens an 'open file' dialog."""
	...

def run_async_function(name: str, data: object, return_handler: str = "") -> None:
	"""Runs a function asyncronously."""
	...

def select_directory_dialog(callback: str = "") -> None:
	"""Opens a select directory dialog."""
	...

def set_color_map(plot: str, map: int) -> None:
	"""Sets the color map of the plot's series."""
	...

def set_drawing_origin(name: str, x: float, y: float) -> None:
	"""Sets the drawing origin (default is 0,0)."""
	...

def set_drawing_scale(name: str, x: float, y: float) -> None:
	"""Sets the drawing scale (default is (1,1))."""
	...

def set_drawing_size(name: str, width: int, height: int) -> None:
	"""Sets the size of a drawing widget."""
	...

def set_global_font_scale(scale: float) -> None:
	"""Changes the global font scale."""
	...

def set_item_callback(item: str, callback: str) -> None:
	"""Sets an item's callback if applicable."""
	...

def set_item_height(item: str, height: int) -> None:
	"""Sets an item's height if applicable."""
	...

def set_item_popup(item: str, popup: str) -> None:
	"""Sets an item's popup if applicable."""
	...

def set_item_tip(item: str, tip: str) -> None:
	"""Sets a simple tooltip for an item."""
	...

def set_item_width(item: str, width: int) -> None:
	"""Sets an item's width."""
	...

def set_key_down_callback(callback: str, handler: str = "") -> None:
	"""Sets a callback for a key down event."""
	...

def set_key_press_callback(callback: str, handler: str = "") -> None:
	"""Sets a callback for a key press event."""
	...

def set_key_release_callback(callback: str, handler: str = "") -> None:
	"""Sets a callback for a key release event."""
	...

def set_log_level(level: int) -> None:
	"""Sets the log level."""
	...

def set_main_window_size(width: int, height: int) -> None:
	"""Sets the main window size."""
	...

def set_mouse_click_callback(callback: str, handler: str = "") -> None:
	"""Sets a callback for a mouse click event."""
	...

def set_mouse_double_click_callback(callback: str, handler: str = "") -> None:
	"""Sets a callback for a mouse double click event."""
	...

def set_mouse_down_callback(callback: str, handler: str = "") -> None:
	"""Sets a callback for a mouse down event."""
	...

def set_mouse_drag_callback(callback: str, threshold: float, handler: str = "") -> None:
	"""Sets a callback for a mouse drag event."""
	...

def set_mouse_wheel_callback(callback: str, handler: str = "") -> None:
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

def set_render_callback(callback: str, handler: str = "") -> None:
	"""Sets the callback to be ran every frame."""
	...

def set_resize_callback(callback: str, handler: str = "") -> None:
	"""Sets a callback for a window resizes."""
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

def set_thread_count(threads: int) -> None:
	"""Sets number of threads to use if the threadpool is active."""
	...

def set_threadpool_high_performance() -> None:
	"""Set the thread count to the maximum number of threads on your computer."""
	...

def set_threadpool_timeout(time: float) -> None:
	"""Sets the threadpool timeout."""
	...

def set_value(name: str, value: object) -> None:
	"""Sets an item's value if applicable."""
	...

def set_window_pos(window: str, x: float, y: float) -> None:
	"""Sets a windows position"""
	...

def show_about() -> None:
	"""Shows the about window."""
	...

def show_debug() -> None:
	"""Shows the debug window."""
	...

def show_documentation() -> None:
	"""Shows the documentation window."""
	...

def show_item(name: str) -> None:
	"""Shows an item if it was hidden."""
	...

def show_logger() -> None:
	"""Shows the logging window."""
	...

def show_metrics() -> None:
	"""Shows the metrics window."""
	...

def show_source(file: str) -> None:
	"""Shows the source code for a file."""
	...

def setup_dearpygui() -> None:
	"""Sets up DearPyGui for user controlled rendering. Only call once and you must call cleanup_deapygui when finished."""
	...

def render_dearpygui_frame() -> None:
	"""Renders a DearPyGui frame. Should be called within a user's event loop. Must first call setup_dearpygui outside of event loop."""
	...

def cleanup_dearpygui() -> None:
	"""Cleans up DearPyGui after calling setup_dearpygui."""
	...

def start_dearpygui() -> None:
	"""Starts DearPyGui"""
	...

def start_dearpygui_docs() -> None:
	"""Starts DearPyGui documentation"""
	...

def start_dearpygui_editor() -> None:
	"""Starts DearPyGui editor"""
	...

def unindent(name: str = "", offset: float = 0.0, parent: str = "", before: str = "") -> None:
	"""Unindents following items."""
	...

import sys

mvPlotFlags_MousePos = 1
mvPlotFlags_Legend = 2
mvPlotFlags_Highlight = 4
mvPlotFlags_BoxSelect = 8
mvPlotFlags_Query = 16
mvPlotFlags_ContextMenu = 32
mvPlotFlags_Crosshairs = 64
mvPlotFlags_CullData = 128
mvPlotFlags_AntiAliased = 256
mvPlotFlags_NoChild = 512
mvPlotFlags_YAxis2 = 1024
mvPlotFlags_YAxis3 = 2048
mvPlotFlags_Default = 175
mvPlotAxisFlags_GridLines = 1
mvPlotAxisFlags_TickMarks = 2
mvPlotAxisFlags_TickLabels = 4
mvPlotAxisFlags_Invert = 8
mvPlotAxisFlags_LockMin = 16
mvPlotAxisFlags_LockMax = 32
mvPlotAxisFlags_Adaptive = 64
mvPlotAxisFlags_LogScale = 128
mvPlotAxisFlags_Scientific = 256
mvPlotAxisFlags_Default = 71
mvPlotAxisFlags_Auxiliary = 70
mvPlotCol_Line = 0
mvPlotCol_Fill = 1
mvPlotCol_MarkerOutline = 2
mvPlotCol_MarkerFill = 3
mvPlotCol_ErrorBar = 4
mvPlotCol_FrameBg = 5
mvPlotCol_PlotBg = 6
mvPlotCol_PlotBorder = 7
mvPlotCol_XAxis = 8
mvPlotCol_YAxis = 9
mvPlotCol_YAxis2 = 10
mvPlotCol_YAxis3 = 11
mvPlotCol_Selection = 12
mvPlotCol_Query = 13
mvPlotCol_COUNT = 14
mvPlotMarker_None = 1
mvPlotMarker_Circle = 2
mvPlotMarker_Square = 4
mvPlotMarker_Diamond = 8
mvPlotMarker_Up = 16
mvPlotMarker_Down = 32
mvPlotMarker_Left = 64
mvPlotMarker_Right = 128
mvPlotMarker_Cross = 256
mvPlotMarker_Plus = 512
mvPlotMarker_Asterisk = 1024
mvPlotColormap_Default = 0
mvPlotColormap_Dark = 1
mvPlotColormap_Pastel = 2
mvPlotColormap_Paired = 3
mvPlotColormap_Viridis = 4
mvPlotColormap_Plasma = 5
mvPlotColormap_Hot = 6
mvPlotColormap_Cool = 7
mvPlotColormap_Pink = 8
mvPlotColormap_Jet = 9
mvGuiCol_Text = 0
mvGuiCol_TextDisabled = 1
mvGuiCol_WindowBg = 2
mvGuiCol_ChildBg = 3
mvGuiCol_PopupBg = 4
mvGuiCol_Border = 5
mvGuiCol_BorderShadow = 6
mvGuiCol_FrameBg = 7
mvGuiCol_FrameBgHovered = 8
mvGuiCol_FrameBgActive = 9
mvGuiCol_TitleBg = 10
mvGuiCol_TitleBgActive = 11
mvGuiCol_TitleBgCollapsed = 12
mvGuiCol_MenuBarBg = 13
mvGuiCol_ScrollbarBg = 14
mvGuiCol_ScrollbarGrab = 15
mvGuiCol_ScrollbarGrabHovered = 16
mvGuiCol_ScrollbarGrabActive = 17
mvGuiCol_CheckMark = 18
mvGuiCol_SliderGrab = 19
mvGuiCol_SliderGrabActive = 20
mvGuiCol_Button = 21
mvGuiCol_ButtonHovered = 22
mvGuiCol_ButtonActive = 23
mvGuiCol_Header = 24
mvGuiCol_HeaderHovered = 25
mvGuiCol_HeaderActive = 26
mvGuiCol_Separator = 27
mvGuiCol_SeparatorHovered = 28
mvGuiCol_SeparatorActive = 29
mvGuiCol_ResizeGrip = 30
mvGuiCol_ResizeGripHovered = 31
mvGuiCol_ResizeGripActive = 32
mvGuiCol_Tab = 33
mvGuiCol_TabHovered = 34
mvGuiCol_TabActive = 35
mvGuiCol_TabUnfocused = 36
mvGuiCol_TabUnfocusedActive = 37
mvGuiCol_PlotLines = 38
mvGuiCol_PlotLinesHovered = 39
mvGuiCol_PlotHistogram = 40
mvGuiCol_PlotHistogramHovered = 41
mvGuiCol_TextSelectedBg = 42
mvGuiCol_DragDropTarget = 43
mvGuiCol_NavHighlight = 44
mvGuiCol_NavWindowingHighlight = 45
mvGuiCol_NavWindowingDimBg = 46
mvGuiCol_ModalWindowDimBg = 47
mvGuiStyleVar_Alpha = 0
mvGuiStyleVar_WindowPadding = 1
mvGuiStyleVar_WindowRounding = 2
mvGuiStyleVar_WindowBorderSize = 3
mvGuiStyleVar_WindowMinSize = 4
mvGuiStyleVar_WindowTitleAlign = 5
mvGuiStyleVar_ChildRounding = 6
mvGuiStyleVar_ChildBorderSize = 7
mvGuiStyleVar_PopupRounding = 8
mvGuiStyleVar_PopupBorderSize = 9
mvGuiStyleVar_FramePadding = 10
mvGuiStyleVar_FrameRounding = 11
mvGuiStyleVar_FrameBorderSize = 12
mvGuiStyleVar_ItemSpacing = 13
mvGuiStyleVar_ItemInnerSpacing = 14
mvGuiStyleVar_IndentSpacing = 15
mvGuiStyleVar_ScrollbarSize = 16
mvGuiStyleVar_ScrollbarRounding = 17
mvGuiStyleVar_GrabMinSize = 18
mvGuiStyleVar_GrabRounding = 19
mvGuiStyleVar_TabRounding = 20
mvGuiStyleVar_ButtonTextAlign = 21
mvGuiStyleVar_SelectableTextAlign = 22
ImGuiStyleVar_TouchExtraPadding = 23
ImGuiStyleVar_TabBorderSize = 24
ImGuiStyleVar_DisplaySafeAreaPadding = 25
ImGuiStyleVar_AntiAliasedLines = 26
ImGuiStyleVar_AntiAliasedFill = 27
ImGuiStyleVar_CurveTessellationTol = 28
ImGuiStyleVar_CircleSegmentMaxError = 29
mvMouseButton_Left = 0
mvMouseButton_Right = 1
mvMouseButton_Middle = 2
mvMouseButton_X1 = 3
mvMouseButton_X2 = 4
mvDir_None = -1
mvDir_Left = 0
mvDir_Right = 1
mvDir_Up = 2
mvDir_Down = 3
mvTRACE = 0
mvDEBUG = 1
mvINFO = 2
mvWARNING = 3
mvERROR = 4
mvOFF = 5

if sys.platform.startswith('win32'):
    mvKey_Back = 8
    mvKey_Tab = 9
    mvKey_Clear = 12
    mvKey_Return = 13
    mvKey_Shift = 16
    mvKey_Control = 17
    mvKey_Alt = 18
    mvKey_Pause = 19
    mvKey_Capital = 20
    mvKey_Escape = 27
    mvKey_Spacebar = 32
    mvKey_Prior = 33
    mvKey_Next = 34
    mvKey_End = 35
    mvKey_Home = 36
    mvKey_Left = 37
    mvKey_Up = 38
    mvKey_Right = 39
    mvKey_Down = 40
    mvKey_Select = 41
    mvKey_Print = 42
    mvKey_Execute = 43
    mvKey_PrintScreen = 44
    mvKey_Insert = 45
    mvKey_Delete = 46
    mvKey_Help = 47
    mvKey_0 = 48
    mvKey_1 = 49
    mvKey_2 = 50
    mvKey_3 = 51
    mvKey_4 = 52
    mvKey_5 = 53
    mvKey_6 = 54
    mvKey_7 = 55
    mvKey_8 = 56
    mvKey_9 = 57
    mvKey_A = 65
    mvKey_B = 66
    mvKey_C = 67
    mvKey_D = 68
    mvKey_E = 69
    mvKey_F = 70
    mvKey_G = 71
    mvKey_H = 72
    mvKey_I = 73
    mvKey_J = 74
    mvKey_K = 75
    mvKey_L = 76
    mvKey_M = 77
    mvKey_N = 78
    mvKey_O = 79
    mvKey_P = 80
    mvKey_Q = 81
    mvKey_R = 82
    mvKey_S = 83
    mvKey_T = 84
    mvKey_U = 85
    mvKey_V = 86
    mvKey_W = 87
    mvKey_X = 88
    mvKey_Y = 89
    mvKey_Z = 90
    mvKey_LWin = 91
    mvKey_RWin = 92
    mvKey_Apps = 93
    mvKey_Sleep = 95
    mvKey_NumPad0 = 96
    mvKey_NumPad1 = 97
    mvKey_NumPad2 = 98
    mvKey_NumPad3 = 99
    mvKey_NumPad4 = 100
    mvKey_NumPad5 = 101
    mvKey_NumPad6 = 102
    mvKey_NumPad7 = 103
    mvKey_NumPad8 = 104
    mvKey_NumPad9 = 105
    mvKey_Multiply = 106
    mvKey_Add = 107
    mvKey_Separator = 108
    mvKey_Subtract = 109
    mvKey_Decimal = 110
    mvKey_Divide = 111
    mvKey_F1 = 112
    mvKey_F2 = 113
    mvKey_F3 = 114
    mvKey_F4 = 115
    mvKey_F5 = 116
    mvKey_F6 = 117
    mvKey_F7 = 118
    mvKey_F8 = 119
    mvKey_F9 = 120
    mvKey_F10 = 121
    mvKey_F11 = 122
    mvKey_F12 = 123
    mvKey_F13 = 124
    mvKey_F14 = 125
    mvKey_F15 = 126
    mvKey_F16 = 127
    mvKey_F17 = 128
    mvKey_F18 = 129
    mvKey_F19 = 130
    mvKey_F20 = 131
    mvKey_F21 = 132
    mvKey_F22 = 133
    mvKey_F23 = 134
    mvKey_F24 = 135
    mvKey_F25 = 136
    mvKey_NumLock = 144
    mvKey_ScrollLock = 145
    mvKey_LShift = 160
    mvKey_RShift = 161
    mvKey_LControl = 162
    mvKey_RControl = 163
    mvKey_LMenu = 164
    mvKey_RMenu = 165
    mvKey_Browser_Back = 166
    mvKey_Browser_Forward = 167
    mvKey_Browser_Refresh = 168
    mvKey_Browser_Stop = 169
    mvKey_Browser_Search = 170
    mvKey_Browser_Favorites = 171
    mvKey_Browser_Home = 172
    mvKey_Volume_Mute = 173
    mvKey_Volume_Down = 174
    mvKey_Volume_Up = 175
    mvKey_Media_Next_Track = 176
    mvKey_Media_Prev_Track = 177
    mvKey_Media_Stop = 178
    mvKey_Media_Play_Pause = 179
    mvKey_Launch_Mail = 180
    mvKey_Launch_Media_Select = 181
    mvKey_Launch_App1 = 182
    mvKey_Launch_App2 = 183
    mvKey_Colon = 186
    mvKey_Plus = 187
    mvKey_Comma = 188
    mvKey_Minus = 189
    mvKey_Period = 190
    mvKey_Slash = 191
    mvKey_Tilde = 192
    mvKey_Open_Brace = 219
    mvKey_Backslash = 220
    mvKey_Close_Brace = 221
    mvKey_Quote = 222

else:
    mvKey_Back = 259
    mvKey_Tab = 258
    mvKey_Clear = 259
    mvKey_Return = 257
    mvKey_Shift = 340
    mvKey_Control = 241
    mvKey_Alt = 342
    mvKey_Pause = 284
    mvKey_Capital = 280
    mvKey_Escape = 256
    mvKey_Spacebar = 32
    mvKey_Prior = -1
    mvKey_Next = -1
    mvKey_End = 269
    mvKey_Home = 268
    mvKey_Left = 263
    mvKey_Up = 265
    mvKey_Right = 262
    mvKey_Down = 264
    mvKey_Select = -1
    mvKey_Print = -1
    mvKey_Execute = -1
    mvKey_PrintScreen = 286
    mvKey_Insert = 260
    mvKey_Delete = 261
    mvKey_Help = -1
    mvKey_0 = 48
    mvKey_1 = 49
    mvKey_2 = 50
    mvKey_3 = 51
    mvKey_4 = 52
    mvKey_5 = 53
    mvKey_6 = 54
    mvKey_7 = 55
    mvKey_8 = 56
    mvKey_9 = 57
    mvKey_A = 65
    mvKey_B = 66
    mvKey_C = 67
    mvKey_D = 68
    mvKey_E = 69
    mvKey_F = 70
    mvKey_G = 71
    mvKey_H = 72
    mvKey_I = 73
    mvKey_J = 74
    mvKey_K = 75
    mvKey_L = 76
    mvKey_M = 77
    mvKey_N = 78
    mvKey_O = 79
    mvKey_P = 80
    mvKey_Q = 81
    mvKey_R = 82
    mvKey_S = 83
    mvKey_T = 84
    mvKey_U = 85
    mvKey_V = 86
    mvKey_W = 87
    mvKey_X = 88
    mvKey_Y = 89
    mvKey_Z = 90
    mvKey_LWin = 343
    mvKey_RWin = 347
    mvKey_Apps = -1
    mvKey_Sleep = -1
    mvKey_NumPad0 = 320
    mvKey_NumPad1 = 321
    mvKey_NumPad2 = 322
    mvKey_NumPad3 = 323
    mvKey_NumPad4 = 324
    mvKey_NumPad5 = 325
    mvKey_NumPad6 = 326
    mvKey_NumPad7 = 327
    mvKey_NumPad8 = 328
    mvKey_NumPad9 = 329
    mvKey_Multiply = 332
    mvKey_Add = 334
    mvKey_Separator = -1
    mvKey_Subtract = 333
    mvKey_Decimal = 330
    mvKey_Divide = 331
    mvKey_F1 = 290
    mvKey_F2 = 291
    mvKey_F3 = 292
    mvKey_F4 = 293
    mvKey_F5 = 294
    mvKey_F6 = 295
    mvKey_F7 = 296
    mvKey_F8 = 297
    mvKey_F9 = 298
    mvKey_F10 = 299
    mvKey_F11 = 300
    mvKey_F12 = 301
    mvKey_F13 = 302
    mvKey_F14 = 303
    mvKey_F15 = 304
    mvKey_F16 = 305
    mvKey_F17 = 306
    mvKey_F18 = 307
    mvKey_F19 = 308
    mvKey_F20 = 309
    mvKey_F21 = 310
    mvKey_F22 = 311
    mvKey_F23 = 312
    mvKey_F24 = 313
    mvKey_F24 = 314
    mvKey_NumLock = 282
    mvKey_ScrollLock = 281
    mvKey_LShift = 340
    mvKey_RShift = 344
    mvKey_LControl = 341
    mvKey_RControl = 345
    mvKey_LMenu = -1
    mvKey_RMenu = -1
    mvKey_Browser_Back = -1
    mvKey_Browser_Forward = -1
    mvKey_Browser_Refresh = -1
    mvKey_Browser_Stop = -1
    mvKey_Browser_Search = -1
    mvKey_Browser_Favorites = -1
    mvKey_Browser_Home = -1
    mvKey_Volume_Mute = -1
    mvKey_Volume_Down = -1
    mvKey_Volume_Up = -1
    mvKey_Media_Next_Track = -1
    mvKey_Media_Prev_Track = -1
    mvKey_Media_Stop = -1
    mvKey_Media_Play_Pause = -1
    mvKey_Launch_Mail = -1
    mvKey_Launch_Media_Select = -1
    mvKey_Launch_App1 = -1
    mvKey_Launch_App2 = -1
    mvKey_Colon = 59
    mvKey_Plus = 61
    mvKey_Comma = 44
    mvKey_Minus = 45
    mvKey_Period = 46
    mvKey_Slash = 47
    mvKey_Tilde = 96
    mvKey_Open_Brace = 91
    mvKey_Backslash = 92
    mvKey_Close_Brace = 93
    mvKey_Quote = 39
