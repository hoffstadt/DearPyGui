#pragma once
#include "Core/mvAppItems.h"

namespace Marvel {

	void AddSliderWidgets      (std::map<std::string, mvPythonParser>* parsers);

	// sliders
	PyObject* add_slider_float (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_float2(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_float3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_float4(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_int   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_int2  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_int3  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_slider_int4  (PyObject* self, PyObject* args, PyObject* kwargs);
}
