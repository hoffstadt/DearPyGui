#pragma once
#include "core/AppItems/mvAppItems.h"

namespace Marvel {

	void AddInputWidgets           (std::map<std::string, mvPythonParser>* parsers);
	void AddSliderWidgets          (std::map<std::string, mvPythonParser>* parsers);
	void AddDragWidgets            (std::map<std::string, mvPythonParser>* parsers);
	void AddBasicWidgets           (std::map<std::string, mvPythonParser>* parsers);

	// basics
	PyObject* add_listbox          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_combo            (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_radio_button     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_checkbox         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_button           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_selectable       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_text             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_label_text       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_progress_bar     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_simple_plot      (PyObject* self, PyObject* args, PyObject* kwargs);
	
	// images
	PyObject* add_image            (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_image_button     (PyObject* self, PyObject* args, PyObject* kwargs);

	// misc
	PyObject* add_logger           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_date_picker      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_time_picker      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_indent           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* unindent             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_spacing          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_dummy            (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_same_line        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_separator        (PyObject* self, PyObject* args, PyObject* kwargs);

	// colors
	PyObject* add_color_button     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_edit3      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_edit4      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_picker3    (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_picker4    (PyObject* self, PyObject* args, PyObject* kwargs);

}
