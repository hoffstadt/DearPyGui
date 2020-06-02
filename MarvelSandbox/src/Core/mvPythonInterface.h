#pragma once

#include "mvApp.h"

namespace Marvel {

	PyObject* PyInit_emb(void);

	PyObject* setItemCallback(PyObject* self, PyObject* args);
	PyObject* addInputText   (PyObject* self, PyObject* args, PyObject* kwargs);

	PyObject* addTabBar      (PyObject* self, PyObject* args);
	PyObject* addTab         (PyObject* self, PyObject* args);
	PyObject* endTab         (PyObject* self, PyObject* args);
	PyObject* endTabBar      (PyObject* self, PyObject* args);

	PyObject* addMenuBar (PyObject* self, PyObject* args);
	PyObject* addMenu    (PyObject* self, PyObject* args);
	PyObject* endMenu    (PyObject* self, PyObject* args);
	PyObject* endMenuBar (PyObject* self, PyObject* args);
	PyObject* addMenuItem(PyObject* self, PyObject* args);

	PyObject* addSpacing(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* addSameLine(PyObject* self, PyObject* args, PyObject* kwargs);
	
	PyObject* getValue(PyObject* self, PyObject* args);

}