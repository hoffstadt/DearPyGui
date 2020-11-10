#pragma once
#include "Core/mvAppItems.h"

namespace Marvel {

	void AddPlotCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_plot             (PyObject* self, PyObject* args, PyObject* kwargs);

	// data removal
	PyObject* clear_plot           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_series        (PyObject* self, PyObject* args, PyObject* kwargs);
	
	// styles
	PyObject* set_color_map        (PyObject* self, PyObject* args, PyObject* kwargs);

	// ticks
	PyObject* reset_xticks         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* reset_yticks         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_xticks           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_yticks           (PyObject* self, PyObject* args, PyObject* kwargs);

	// query
	PyObject* is_plot_queried      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_plot_query_area  (PyObject* self, PyObject* args, PyObject* kwargs);
	
	// limits
	PyObject* set_plot_xlimits_auto(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_plot_ylimits_auto(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_plot_xlimits     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_plot_ylimits     (PyObject* self, PyObject* args, PyObject* kwargs);

	// drag points
	PyObject* add_drag_point       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_drag_point    (PyObject* self, PyObject* args, PyObject* kwargs);

	// drag lines
	PyObject* add_drag_line        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_drag_line     (PyObject* self, PyObject* args, PyObject* kwargs);

	// annotations
	PyObject* add_annotation       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_annotation    (PyObject* self, PyObject* args, PyObject* kwargs);

	// series
	PyObject* add_image_series     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_pie_series       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_line_series      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_bar_series       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_shade_series     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_scatter_series   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_stem_series      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_text_point       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_area_series      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_error_series     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_heat_series      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_stair_series     (PyObject* self, PyObject* args, PyObject* kwargs);
}