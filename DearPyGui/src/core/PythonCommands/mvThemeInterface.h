#pragma once
#include "mvAppItems.h"

namespace Marvel {

	void AddThemeCommands(std::map<std::string, mvPythonParser>* parsers);

	// global
	PyObject* set_theme_color                       (PyObject* self, PyObject* args, PyObject* kwargs);

	// font
	PyObject* set_global_font_scale                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_global_font_scale                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_additional_font                   (PyObject* self, PyObject* args, PyObject* kwargs);
}