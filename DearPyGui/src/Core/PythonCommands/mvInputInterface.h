#pragma once
#include "Core/mvAppItems.h"

namespace Marvel {

	void AddInputCommands(std::map<std::string, mvPythonParser>* parsers);

	// mouse
	PyObject* is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_mouse_drag_delta(PyObject* self, PyObject* args);
	PyObject* set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_drag_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_click_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_release_callback(PyObject* self, PyObject* args, PyObject* kwargs);

	// key
	PyObject* is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_key_released(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_key_down(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_key_down_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_key_press_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_key_release_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_wheel_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_move_callback(PyObject* self, PyObject* args, PyObject* kwargs);

	PyObject* set_render_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs);

}