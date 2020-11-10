#pragma once
#include "Core/mvAppItems.h"

namespace Marvel {

	void AddDragWidgets      (std::map<std::string, mvPythonParser>* parsers);

	// drags
	PyObject* add_drag_float (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_float2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_float3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_float4(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_int   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_int2  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_int3  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_drag_int4  (PyObject* self, PyObject* args, PyObject* kwargs);

}
