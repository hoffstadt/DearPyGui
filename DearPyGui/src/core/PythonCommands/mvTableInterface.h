#pragma once
#include "mvAppItems.h"

namespace Marvel {

	void AddTableCommands         (std::map<std::string, mvPythonParser>* parsers);

	// table
	PyObject* add_table           (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* set_table_data      (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* get_table_data      (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* set_headers         (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* clear_table         (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* get_table_item      (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* set_table_item      (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* get_table_selections(PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* set_table_selection (PyObject * self, PyObject * args, PyObject * kwargs);

	// column
	PyObject* add_column          (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* insert_column       (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* delete_column       (PyObject * self, PyObject * args, PyObject * kwargs);

	// row
	PyObject* add_row             (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* insert_row          (PyObject * self, PyObject * args, PyObject * kwargs);
	PyObject* delete_row          (PyObject * self, PyObject * args, PyObject * kwargs);
}