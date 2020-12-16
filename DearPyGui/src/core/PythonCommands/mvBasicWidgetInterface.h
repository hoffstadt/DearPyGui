#pragma once
#include "mvAppItems.h"

namespace Marvel {

	void AddInputWidgets           (std::map<std::string, mvPythonParser>* parsers);
	void AddSliderWidgets          (std::map<std::string, mvPythonParser>* parsers);
	void AddDragWidgets            (std::map<std::string, mvPythonParser>* parsers);
	void AddBasicWidgets           (std::map<std::string, mvPythonParser>* parsers);

	// basics
	PyObject* add_simple_plot      (PyObject* self, PyObject* args, PyObject* kwargs);
	
	// images

	// misc
	PyObject* add_indent           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* unindent             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_spacing          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_same_line        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_separator        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_dummy            (PyObject* self, PyObject* args, PyObject* kwargs);

	// colors

}
