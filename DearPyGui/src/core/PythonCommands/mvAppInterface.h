#pragma once
#include "mvAppItems.h"

namespace Marvel {

	void AddAppCommands(std::map<std::string, mvPythonParser>* parsers);
	void AddLogCommands(std::map<std::string, mvPythonParser>* parsers);
	void AddStdWindowCommands(std::map<std::string, mvPythonParser>* parsers);

	// blah
	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs);

	// app
	PyObject* is_dearpygui_running           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* start_dearpygui                (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* stop_dearpygui                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_start_callback             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_exit_callback              (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_accelerator_callback       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* setup_dearpygui                (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* render_dearpygui_frame         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* cleanup_dearpygui              (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_vsync                      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_dearpygui_version          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_active_window              (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_hovered_item				 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_character_remap            (PyObject* self, PyObject* args, PyObject* kwargs);

	// docking
	PyObject* enable_docking                 (PyObject* self, PyObject* args, PyObject* kwargs);
	
	// main viewport
	PyObject* set_main_window_pos            (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_main_window_title          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_main_window_resizable      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_main_window_size           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_main_window_size           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_primary_window             (PyObject* self, PyObject* args, PyObject* kwargs);

	// timing
	PyObject* get_total_time                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_delta_time                 (PyObject* self, PyObject* args, PyObject* kwargs);
	
	// data storage
	PyObject* add_data                       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_data                       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_data                    (PyObject* self, PyObject* args, PyObject* kwargs);

	// texture storage
	PyObject* add_texture                    (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* decrement_texture              (PyObject* self, PyObject* args, PyObject* kwargs);
	
	// concurrency
	PyObject* set_threadpool_timeout         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_thread_count               (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_thread_count               (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_threadpool_high_performance (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_threadpool_timeout         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* run_async_function             (PyObject* self, PyObject* args, PyObject* kwargs);
	
	// logging
	PyObject* get_log_level                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_log_level                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log                            (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log_debug                      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log_info                       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log_warning                    (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log_error                      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* clear_log                      (PyObject* self, PyObject* args, PyObject* kwargs);

	// standard windows
	PyObject* show_logger                    (PyObject* self, PyObject* args);
	PyObject* select_directory_dialog        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* open_file_dialog               (PyObject* self, PyObject* args, PyObject* kwargs);
}
