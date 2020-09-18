#pragma once
#include "Core/mvAppItems.h"

namespace Marvel {

	void AddMenuCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_menu_item(PyObject* self, PyObject* args, PyObject* kwargs);


	void AddInputWidgets(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int4(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float4(PyObject* self, PyObject* args, PyObject* kwargs);

	void AddSliderWidgets(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_slider_float(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_float2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_float3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_float4(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_int(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_int2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_int3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_int4(PyObject* self, PyObject* args, PyObject* kwargs);

	void AddDragWidgets(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_drag_float(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_float2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_float3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_float4(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_int(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_int2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_int3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_int4(PyObject* self, PyObject* args, PyObject* kwargs);


	void AddBasicWidgets(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_simple_plot(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_progress_bar(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_image(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_image_button(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_text(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_label_text(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_listbox(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_selectable(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_indent(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_spacing(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_dummy(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_same_line(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_separator(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs);

	void AddContainterWidgets(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_group(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_child(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_popup(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* end(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_tree_node(PyObject* self, PyObject* args, PyObject* kwargs);


}
