#pragma once

#include "core/mvAppItems.h"

namespace Marvel {
void AddNodeEditorCommands(std::map<std::string, mvPythonParser> *parsers);

PyObject *add_link(PyObject *self, PyObject *args, PyObject *kwargs);

PyObject *delete_link(PyObject *self, PyObject *args, PyObject *kwargs);

PyObject *add_node_title_bar(PyObject *self, PyObject *args, PyObject *kwargs);

PyObject *add_node_attribute(PyObject *self, PyObject *args, PyObject *kwargs);

PyObject *add_node_editor(PyObject *self, PyObject *args, PyObject *kwargs);

PyObject *add_node(PyObject *self, PyObject *args, PyObject *kwargs);

PyObject *get_links(PyObject *self, PyObject *args, PyObject *kwargs);

PyObject *get_selected_links(PyObject *self, PyObject *args, PyObject *kwargs);

PyObject *get_selected_nodes(PyObject *self, PyObject *args, PyObject *kwargs);

}