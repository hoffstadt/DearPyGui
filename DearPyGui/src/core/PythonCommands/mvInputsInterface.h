#pragma once
#include "mvAppItems.h"

namespace Marvel {

	void AddInputWidgets           (std::map<std::string, mvPythonParser>* parsers);

	// inputs
	PyObject* add_input_text  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int2  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int3  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_int4  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_input_float4(PyObject* self, PyObject* args, PyObject* kwargs);

}
