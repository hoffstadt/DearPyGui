#pragma once
#include "Core/mvAppItems.h"

namespace Marvel {

	void AddDrawingCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_drawing        (PyObject* self, PyObject* args, PyObject* kwargs);

	// data removal
	PyObject* delete_drawing_item(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* clear_drawing      (PyObject* self, PyObject* args, PyObject* kwargs);

	// settings
	//PyObject* set_drawing_size   (PyObject* self, PyObject* args, PyObject* kwargs);
	//PyObject* set_drawing_origin (PyObject* self, PyObject* args, PyObject* kwargs);
	//PyObject* set_drawing_scale  (PyObject* self, PyObject* args, PyObject* kwargs);
	//PyObject* get_drawing_origin (PyObject* self, PyObject* args, PyObject* kwargs);
	//PyObject* get_drawing_scale  (PyObject* self, PyObject* args, PyObject* kwargs);
	//PyObject* get_drawing_size   (PyObject* self, PyObject* args, PyObject* kwargs);
								 
	// drawing primitives		 
	PyObject* draw_image         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_line          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_arrow         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_triangle      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_rectangle     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_quad          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_text          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_circle        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_polyline      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_polygon       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_bezier_curve  (PyObject* self, PyObject* args, PyObject* kwargs);
}