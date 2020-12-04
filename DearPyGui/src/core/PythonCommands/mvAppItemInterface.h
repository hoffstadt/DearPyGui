#pragma once
#include "core/mvAppItems.h"

namespace Marvel {

	void AddItemCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* get_item_type                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_item_configuration        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* configure_item                (PyObject* self, PyObject* args, PyObject* kwargs);

	// replacing
	PyObject* set_item_callback             (PyObject* self, PyObject* args, PyObject* kwargs);

	PyObject* move_item                     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_item                   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* does_item_exist               (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* move_item_up                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* move_item_down                (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_item_callback             (PyObject* self, PyObject* args, PyObject* kwargs);
	
	PyObject* get_item_children             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_all_items                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_windows                   (PyObject* self, PyObject* args, PyObject* kwargs);
							                
	PyObject* get_item_parent               (PyObject* self, PyObject* args, PyObject* kwargs);

	PyObject* set_managed_column_width      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_managed_column_width      (PyObject* self, PyObject* args, PyObject* kwargs);

	PyObject* is_item_hovered               (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_shown                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_active                (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_focused               (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_clicked               (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_container             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_visible               (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_edited                (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_activated             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_deactivated           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_item_toggled_open          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_item_rect_min             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_item_rect_max             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_item_rect_size            (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_value                     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_value                     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_value                     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* incref_value                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* decref_value                  (PyObject* self, PyObject* args, PyObject* kwargs);


}
