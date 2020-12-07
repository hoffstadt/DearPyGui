#pragma once
#include "core/AppItems/mvAppItems.h"

namespace Marvel {

	void AddDrawingCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_drawing        (PyObject* self, PyObject* args, PyObject* kwargs);

	// commands
	PyObject* modify_draw_command        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_draw_command           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* bring_draw_command_forward (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* bring_draw_command_to_front(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* send_draw_command_back     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* send_draw_command_to_back  (PyObject* self, PyObject* args, PyObject* kwargs);

	// data removal
	PyObject* delete_draw_command        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* clear_drawing              (PyObject* self, PyObject* args, PyObject* kwargs);
								         
	// drawing primitives		         
	PyObject* draw_image                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_line                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_arrow                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_triangle              (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_rectangle             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_quad                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_text                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_circle                (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_polyline              (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_polygon               (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* draw_bezier_curve          (PyObject* self, PyObject* args, PyObject* kwargs);
}