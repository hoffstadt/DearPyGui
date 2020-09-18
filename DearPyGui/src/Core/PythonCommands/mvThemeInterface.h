#pragma once
#include "Core/mvAppItems.h"

namespace Marvel {

	void AddThemeCommands(std::map<std::string, mvPythonParser>* parsers);

	// global
	PyObject* add_additional_font(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_theme_item(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_theme(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_theme_item(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_additional_font(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_item_color(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* clear_item_color(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_theme(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_display_safe_area_padding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_display_safe_area_padding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_antialiased_fill(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_antialiased_fill(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_curve_tessellation_tolerance(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_curve_tessellation_tolerance(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_circle_segment_max_error(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_circle_segment_max_error(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_global_alpha(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_global_alpha(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_antialiased_lines(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_antialiased_lines(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_touch_extra_padding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_touch_extra_padding(PyObject* self, PyObject* args, PyObject* kwargs);

	// window
	PyObject* get_style_window_padding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_window_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_window_rounding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_window_title_align(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_window_menu_button_position(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_window_padding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_window_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_window_rounding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_window_title_align(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_window_menu_button_position(PyObject* self, PyObject* args, PyObject* kwargs);

	// frame
	PyObject* get_style_frame_padding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_frame_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_frame_rounding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_frame_padding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_frame_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_frame_rounding(PyObject* self, PyObject* args, PyObject* kwargs);

	//child
	PyObject* get_style_child_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_child_rounding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_child_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_child_rounding(PyObject* self, PyObject* args, PyObject* kwargs);

	// popup
	PyObject* get_style_popup_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_popup_rounding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_popup_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_popup_rounding(PyObject* self, PyObject* args, PyObject* kwargs);

	// scrollbar
	PyObject* get_style_scrollbar_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_scrollbar_rounding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_scrollbar_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_scrollbar_rounding(PyObject* self, PyObject* args, PyObject* kwargs);

	// grab
	PyObject* get_style_grab_min_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_grab_rounding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_grab_min_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_grab_rounding(PyObject* self, PyObject* args, PyObject* kwargs);

	// item
	PyObject* get_style_item_spacing(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_item_inner_spacing(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_item_spacing(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_item_inner_spacing(PyObject* self, PyObject* args, PyObject* kwargs);

	// tab
	PyObject* get_style_tab_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_style_tab_rounding(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_tab_border_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_tab_rounding(PyObject* self, PyObject* args, PyObject* kwargs);

	// indent
	PyObject* get_style_indent_spacing(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_indent_spacing(PyObject* self, PyObject* args, PyObject* kwargs);

	// color selector
	PyObject* get_style_color_button_position(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_color_button_position(PyObject* self, PyObject* args, PyObject* kwargs);

	// button
	PyObject* get_style_button_text_align(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_button_text_align(PyObject* self, PyObject* args, PyObject* kwargs);

	// selectable
	PyObject* get_style_selectable_text_align(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_style_selectable_text_align(PyObject* self, PyObject* args, PyObject* kwargs);




}