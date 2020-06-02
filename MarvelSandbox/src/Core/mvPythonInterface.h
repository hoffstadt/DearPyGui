#pragma once

#include "mvApp.h"

namespace Marvel {

	PyObject* PyInit_emb(void);

	PyObject* setItemCallback(PyObject* self, PyObject* args);
	PyObject* addInputText   (PyObject* self, PyObject* args);
	PyObject* addTabBar      (PyObject* self, PyObject* args);
	PyObject* addTab         (PyObject* self, PyObject* args);
	PyObject* endTab         (PyObject* self, PyObject* args);
	PyObject* endTabBar      (PyObject* self, PyObject* args);

}