#pragma once
#include "core/mvAppItems.h"

namespace Marvel {

	void AddContainerWidgets       (std::map<std::string, mvPythonParser>* parsers);

	// menus
	PyObject* add_menu_bar         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_menu             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_menu_item        (PyObject* self, PyObject* args, PyObject* kwargs);

	// containers
	PyObject* end                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_tab_bar          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_tab              (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_tab_button       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_group            (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_child            (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_window           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_about_window     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_doc_window       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_debug_window     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_style_window     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_metrics_window   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_tooltip          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_popup            (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_tree_node        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_managed_columns  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_columns          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_next_column      (PyObject* self, PyObject* args, PyObject* kwargs);

}
