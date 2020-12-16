#pragma once
#include "mvAppItems.h"

namespace Marvel {

	void AddContainerWidgets       (std::map<std::string, mvPythonParser>* parsers);

	// menus

	// containers
	PyObject* end                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_about_window     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_metrics_window   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_popup            (PyObject* self, PyObject* args, PyObject* kwargs);

}
