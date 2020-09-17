#pragma once

//-----------------------------------------------------------------------------
// mvMarvel
//
//     - This file contains marvel python module command information.
//
//     - These parsers are used to check python parsing and build documentation
//     
//-----------------------------------------------------------------------------

#include "mvPythonParser.h"

namespace Marvel {

	static void AddLogCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "get_log_level", mvPythonParser({
		}, "Returns the log level.", "int", "Logging") });

		parsers->insert({ "clear_log", mvPythonParser({
		}, "Clears the logger.", "None", "Logging") });

		parsers->insert({ "set_log_level", mvPythonParser({
			{mvPythonDataType::Integer, "level"}
		}, "Sets the log level.", "None", "Logging") });

		parsers->insert({ "log", mvPythonParser({
			{mvPythonDataType::Object, "message"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "level"}
		}, "Logs a trace level log.", "None", "Logging") });

		parsers->insert({ "log_debug", mvPythonParser({
			{mvPythonDataType::Object, "message"}
		}, "Logs a debug level log.", "None", "Logging") });

		parsers->insert({ "log_info", mvPythonParser({
			{mvPythonDataType::Object, "message"}
		}, "Logs a info level log.", "None", "Logging") });

		parsers->insert({ "log_warning", mvPythonParser({
			{mvPythonDataType::Object, "message"}
		}, "Logs a warning level log.", "None", "Logging") });

		parsers->insert({ "log_error", mvPythonParser({
			{mvPythonDataType::Object, "message"}
		}, "Logs a error level log.", "None", "Logging") });
	}

	static void AddInputCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "set_mouse_move_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse move event. Data is the mouse position in local coordinates.", "None", "Input Polling") });

		parsers->insert({ "set_render_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window/child/popup/menu is active (default is main window)"},
		}, "Sets the callback to be ran every frame.", "None", "Input Polling") });

		parsers->insert({ "get_mouse_pos", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "local"}
		}, "Returns the current mouse position in relation to the active window (minus titlebar) unless local flag is unset.", 
		"(int, int)", "Input Polling") });

		parsers->insert({ "get_mouse_drag_delta", mvPythonParser({
		}, "Returns the current mouse drag delta in pixels", "(float, float)", "Input Polling") });

		parsers->insert({ "is_mouse_button_dragging", mvPythonParser({
			{mvPythonDataType::Integer, "button"},
			{mvPythonDataType::Float, "threshold"},
		}, "Checks if the mouse is dragging.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_down", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is pressed.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_clicked", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is clicked.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_released", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is released.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_double_clicked", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is double clicked.", "bool", "Input Polling") });

		parsers->insert({ "is_key_pressed", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is pressed.", "bool", "Input Polling") });

		parsers->insert({ "is_key_released", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is released.", "bool", "Input Polling") });

		parsers->insert({ "is_key_down", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is down.", "bool", "Input Polling") });

		parsers->insert({ "set_resize_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a window resize event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_release_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse release event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_down_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse down event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_drag_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Float, "threshold"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse drag event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_wheel_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse wheel event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_double_click_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse double click event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_click_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse click event.", "None", "Input Polling") });

		parsers->insert({ "set_key_down_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a key down event.", "None", "Input Polling") }),

		parsers->insert({ "set_key_press_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
			}, "Sets a callback for a key press event.", "None", "Input Polling") });

		parsers->insert({ "set_key_release_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a key release event.", "None", "Input Polling") });
	}

	static void AddInputWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_input_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "default_value"},
			{mvPythonDataType::String, "hint"},
			{mvPythonDataType::Bool, "multiline"},
			{mvPythonDataType::Bool, "no_spaces"},
			{mvPythonDataType::Bool, "uppercase"},
			{mvPythonDataType::Bool, "decimal"},
			{mvPythonDataType::Bool, "hexadecimal"},
			{mvPythonDataType::Bool, "readonly"},
			{mvPythonDataType::Bool, "password", "hides text values"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds input for text values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds input for integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds input for 2 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds input for 3 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds input for 4 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds input for float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds input for 2 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds input for 3 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds input for 4 float values.", "None", "Adding Widgets") });
	}

	static void AddSliderWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_slider_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Bool, "vertical", "sets orientation to vertical"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height","Height of a vertical slider"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds slider for a single float value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds slider for a 2 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds slider for a 3 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds slider for a 4 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Bool, "vertical", "sets orientation to vertical"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height","Height of a vertical slider"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds slider for a single int value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds slider for a 2 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds slider for a 3 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds slider for a 4 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
	}

	static void AddDragWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_drag_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a single float value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 2 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 3 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 4 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a single int value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 2 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 3 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 4 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
	}

	static void AddTableCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_table", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "headers"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)"}
		}, "Adds table.", "None", "Tables") });

		parsers->insert({ "set_table_item", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"},
			{mvPythonDataType::Integer, "column"},
			{mvPythonDataType::String, "value"},
		}, "Sets a table's cell value.", "None", "Tables") });

		parsers->insert({ "delete_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"}
		}, "Delete a row in a table.", "None", "Tables") });

		parsers->insert({ "delete_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "column"}
		}, "Delete a column in a table.", "None", "Tables") });

		parsers->insert({ "set_headers", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::StringList, "headers"},
		}, "Sets a tables headers.", "None", "Tables") });

		parsers->insert({ "add_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::StringList, "row"},
		}, "Adds a row to the end of a table.", "None", "Tables") });

		parsers->insert({ "clear_table", mvPythonParser({
			{mvPythonDataType::String, "table"}
		}, "Clears data in a table", "None", "Tables") });

		parsers->insert({ "add_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "column"},
		}, "Adds a column to the end of a table.", "None", "Tables") });

		parsers->insert({ "insert_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "column_index"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "column"},
		}, "Inserts a column into a table.", "None", "Tables") });

		parsers->insert({ "insert_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row_index"},
			{mvPythonDataType::StringList, "row"},
		}, "Inserts a row into a table.", "None", "Tables") });

		parsers->insert({ "set_table_selection", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"},
			{mvPythonDataType::Integer, "column"},
			{mvPythonDataType::Bool, "value"},
		}, "Sets a table's cell selection value.", "None", "Tables") });

		parsers->insert({ "get_table_item", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"},
			{mvPythonDataType::Integer, "column"}
		}, "Gets a table's cell value.", "str", "Tables") });

		parsers->insert({ "get_table_selections", mvPythonParser({
			{mvPythonDataType::String, "table"}
		}, "Retrieves data from storage.", "List[List[int]]", "Tables") });
	}

	static void AddItemCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "get_windows", mvPythonParser({
		}, "Returns a list of windows.", "List[str]", "Widget Commands") });

		parsers->insert({ "get_all_items", mvPythonParser({
		}, "Returns a list of all items.", "List[str]", "Widget Commands") });

		parsers->insert({ "get_item_children", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns a list of an item's children.", "List[str]", "Widget Commands") });

		parsers->insert({ "set_item_label", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "label"},
		}, "Sets an item's label.", "None", "Widget Commands") });

		parsers->insert({ "get_item_label", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Gets an item's label.", "str", "Widget Commands") });

		parsers->insert({ "set_window_pos", mvPythonParser({
			{mvPythonDataType::String, "window"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"}
		}, "Sets a windows position", "None", "Widget Commands") });

		parsers->insert({ "get_window_pos", mvPythonParser({
			{mvPythonDataType::String, "window"}
		}, "Gets a windows position", "List[float]", "Widget Commands") });

		parsers->insert({ "delete_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Bool, "children_only"}
		}, "Deletes an item if it exists.", "None", "Widget Commands") });

		parsers->insert({ "does_item_exist", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if item exists.", "Bool", "Widget Commands") });

		parsers->insert({ "move_item_up", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves item up if possible and if it exists.", "None", "Widget Commands") });

		parsers->insert({ "move_item_down", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves item down if possible and if it exists.", "None", "Widget Commands") });

		parsers->insert({ "get_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item' callback", "Callable", "Widget Commands") });

		parsers->insert({ "get_item_height", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's height.", "float", "Widget Commands") });

		parsers->insert({ "get_item_width", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's width.", "float", "Widget Commands") });

		parsers->insert({ "get_item_parent", mvPythonParser({
		{mvPythonDataType::String, "item"}
		}, "Returns an item's parent.", "str", "Widget Commands") });

		parsers->insert({ "get_item_popup", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's popup.", "str", "Widget Commands") });

		parsers->insert({ "get_item_tip", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's tip.", "str", "Widget Commands") });

		parsers->insert({ "is_item_hovered", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is hovered.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_shown", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if an item is shown.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_active", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is active.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_focused", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is focused.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_clicked", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is clicked.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_container", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if an item is container.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_visible", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is visible on screen.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_edited", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been edited.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_activated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been activated.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_deactivated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_deactivated_after_edit", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated after editing.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_toggled_open", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is toggled.", "bool", "Widget Commands") });

		parsers->insert({ "get_item_rect_min", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's minimum allowable size. [width, height]", "[float, float]", "Widget Commands") });

		parsers->insert({ "get_item_rect_max", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's maximum allowable size. [width, height]", "[float, float]", "Widget Commands") });

		parsers->insert({ "get_item_rect_size", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's current size. [width, height]", "[float, float]", "Widget Commands") });

		parsers->insert({ "get_value", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Returns an item's value or None if there is none.", "Any", "Widget Commands") });

		parsers->insert({ "set_value", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "value"}
		}, "Sets an item's value if applicable.", "None", "Widget Commands") });

		parsers->insert({ "show_item", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Shows an item if it was hidden.", "None", "Widget Commands") });

		parsers->insert({ "hide_item", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Bool, "children_only"}
		}, "Hides an item.", "None", "Widget Commands") });

		parsers->insert({ "set_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
		}, "Sets an item's callback if applicable.", "None", "Widget Commands") });

		parsers->insert({ "set_item_popup", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "popup"}
		}, "Sets an item's popup if applicable.", "None", "Widget Commands") });

		parsers->insert({ "set_item_tip", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "tip"}
		}, "Sets a simple tooltip for an item.", "None", "Widget Commands") });

		parsers->insert({ "set_item_width", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "width"}
		}, "Sets an item's width.", "None", "Widget Commands") });

		parsers->insert({ "set_item_height", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "height"}
		}, "Sets an item's height if applicable.", "None", "Widget Commands") });
	}

	static void AddStdWindowCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "show_logger", mvPythonParser({
		}, "Shows the logging window. The Default log level is Trace", "None", "Standard Windows") });

		parsers->insert({ "show_documentation", mvPythonParser({
		}, "Shows the documentation window.", "None", "Standard Windows") });

		parsers->insert({ "show_about", mvPythonParser({
		}, "Shows the about window.", "None", "Standard Windows") });

		parsers->insert({ "show_metrics", mvPythonParser({
		}, "Shows the metrics window.", "None", "Standard Windows") });

		parsers->insert({ "show_debug", mvPythonParser({
		}, "Shows the debug window.", "None", "Standard Windows") });

		parsers->insert({ "show_style_editor", mvPythonParser({
		}, "Shows the font window.") });

		parsers->insert({ "close_popup", mvPythonParser({
		}, "Closes the current popup") });

		parsers->insert({ "show_source", mvPythonParser({
			{mvPythonDataType::String, "file"},
		}, "Shows the source code for a file.", "None", "Standard Windows") });
	}

	static void AddThemeCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_additional_font", mvPythonParser({
			{mvPythonDataType::String, "file", "ttf or otf file"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "size"},
			{mvPythonDataType::String, "glyph_ranges", "options: korean, japanese, chinese_full, chinese_simplified_common, cryillic, thai, vietnamese"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "custom_glyph_chars", ""},
			{mvPythonDataType::Object, "custom_glyph_ranges", ""},
		}, "Adds additional font.", "None", "Themes and Styles") });

		parsers->insert({ "set_theme", mvPythonParser({
			{mvPythonDataType::String, "theme"}
		}, "Set the application's theme to a built-in theme.", "None", "Themes and Styles") });

		parsers->insert({ "get_theme_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"}
		}, "Returns a theme item's color", "(float, float, float, float)", "Themes and Styles") });

		parsers->insert({ "get_theme", mvPythonParser({
		}, "Returns the current theme.", "str", "Themes and Styles") });

		parsers->insert({ "set_item_color", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "style"},
			{mvPythonDataType::FloatList, "color"}
		}, "Sets an color style for a single item.", "None", "Themes and Styles") });

		parsers->insert({ "clear_item_color", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Clears individual color styles for an item.", "None", "Themes and Styles") });

		parsers->insert({ "set_theme_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"},
			{mvPythonDataType::Integer, "r"},
			{mvPythonDataType::Integer, "g"},
			{mvPythonDataType::Integer, "b"},
			{mvPythonDataType::Integer, "a"}
		}, "Sets a theme item.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_padding", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets padding within a window.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_frame_padding", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets padding within a framed rectangle (used by most widgets).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_item_spacing", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets horizontal and vertical spacing between widgets/lines.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_item_inner_spacing", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_touch_extra_padding", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!", "None", "Themes and Styles") });

		parsers->insert({ "set_style_indent_spacing", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_scrollbar_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets width of the vertical scrollbar, Height of the horizontal scrollbar.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_grab_min_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets minimum width/height of a grab box for slider/scrollbar.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_child_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around child windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_popup_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around popup/tooltip windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_frame_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around frames. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_tab_border_size", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets thickness of border around tabs.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets Radius of window corners rounding. Set to 0.0fto have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_child_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of child window corners rounding. Set to 0.0 to have rectangular windows.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_frame_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of frame corners rounding. Set to 0.0 to have rectangular frame (used by most widgets).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_popup_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of popup window corners rounding. (Note that tooltip windows use WindowRounding).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_scrollbar_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of grab corners for scrollbar.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_grab_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of grabs corners rounding. Set to 0.0 to have rectangular slider grabs.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_tab_rounding", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets radius of upper corners of a tab. Set to 0.0 to have rectangular tabs.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_title_align", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets alignment for title bar text. Defaults to (0.0,0.5) for left-aligned,vertically centered.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_window_menu_button_position", mvPythonParser({
			{mvPythonDataType::Integer, "value"}
		}, "Sets side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to mvGuiDir_Left.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_color_button_position", mvPythonParser({
			{mvPythonDataType::Integer, "value"}
		}, "Sets side of the color button in the ColorEdit4 widget (left/right). Defaults to mvGuiDir_Right.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_button_text_align", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets alignment of button text when button is larger than text. Defaults to (0.5, 0.5) (centered).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_selectable_text_align", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets alignment of selectable text. Defaults to (0.0, 0.0) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_display_safe_area_padding", mvPythonParser({
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
		}, "Sets if you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!", "None", "Themes and Styles") });

		parsers->insert({ "set_style_global_alpha", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets global alpha applies to everything in Dear PyGui.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_antialiased_lines", mvPythonParser({
			{mvPythonDataType::Bool, "value"}
		}, "Sets anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_antialiased_fill", mvPythonParser({
			{mvPythonDataType::Bool, "value"}
		}, "Sets anti-aliasing on filled shapes (rounded rectangles, circles, etc.).", "None", "Themes and Styles") });

		parsers->insert({ "set_style_curve_tessellation_tolerance", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets Tessellation tolerance.", "None", "Themes and Styles") });

		parsers->insert({ "set_style_circle_segment_max_error", mvPythonParser({
			{mvPythonDataType::Float, "value"}
		}, "Sets maximum error (in pixels) allowed when using draw_circle()or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.", "None", "Themes and Styles") });

		parsers->insert({ "get_style_window_padding", mvPythonParser({
		}, "Gets padding within a window.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_frame_padding", mvPythonParser({
		}, "Gets padding within a framed rectangle (used by most widgets).", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_item_spacing", mvPythonParser({
		}, "Gets horizontal and vertical spacing between widgets/lines.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_item_inner_spacing", mvPythonParser({
		}, "Gets horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_touch_extra_padding", mvPythonParser({
		}, "Get touch extra padding.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_indent_spacing", mvPythonParser({
		}, "Gets horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).", "float", "Themes and Styles") });

		parsers->insert({ "get_style_scrollbar_size", mvPythonParser({
		}, "Gets width of the vertical scrollbar, Height of the horizontal scrollbar.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_grab_min_size", mvPythonParser({
		}, "Gets minimum width/height of a grab box for slider/scrollbar.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_window_border_size", mvPythonParser({
		}, "Gets thickness of border around windows.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_child_border_size", mvPythonParser({
		}, "Gets thickness of border around child windows.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_popup_border_size", mvPythonParser({
		}, "Gets thickness of border around popup/tooltip windows.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_frame_border_size", mvPythonParser({
		}, "Gets thickness of border around frames.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_tab_border_size", mvPythonParser({
		}, "Gets thickness of border around tabs.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_window_rounding", mvPythonParser({
		}, "Gets radius of window corners rounding.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_child_rounding", mvPythonParser({
		}, "Gets radius of child window corners rounding.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_frame_rounding", mvPythonParser({
		}, "Gets radius of frame corners rounding.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_popup_rounding", mvPythonParser({
		}, "Gets radius of popup window corners rounding. (Note that tooltip windows use WindowRounding).", "float", "Themes and Styles") });

		parsers->insert({ "get_style_scrollbar_rounding", mvPythonParser({
		}, "Gets radius of grab corners for scrollbar.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_grab_rounding", mvPythonParser({
		}, "Gets radius of grabs corners rounding. Set to 0.0 to have rectangular slider grabs.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_tab_rounding", mvPythonParser({
		}, "Gets radius of upper corners of a tab. Set to 0.0 to have rectangular tabs.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_window_title_align", mvPythonParser({
		}, "Gets alignment for title bar text. Defaults to (0.0,0.5) for left-aligned,vertically centered.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_window_menu_button_position", mvPythonParser({
		}, "Gets side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to mvGuiDir_Left.", "int", "Themes and Styles") });

		parsers->insert({ "get_style_color_button_position", mvPythonParser({
		}, "Gets side of the color button in the ColorEdit4 widget (left/right). Defaults to mvGuiDir_Right.", "int", "Themes and Styles") });

		parsers->insert({ "get_style_button_text_align", mvPythonParser({
		}, "Gets alignment of button text when button is larger than text. Defaults to (0.5, 0.5) (centered).", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_selectable_text_align", mvPythonParser({
		}, "Gets alignment of selectable text. Defaults to (0.0, 0.0) (top-left aligned).", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_display_safe_area_padding", mvPythonParser({
		}, "Gets safe area padding. Applies to popups/tooltips as well regular windows.", "List[float]", "Themes and Styles") });

		parsers->insert({ "get_style_global_alpha", mvPythonParser({
		}, "Gets global alpha applies to everything in Dear PyGui.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_antialiased_lines", mvPythonParser({
		}, "Gets anti-aliasing on lines/borders.", "bool", "Themes and Styles") });

		parsers->insert({ "get_style_antialiased_fill", mvPythonParser({
		}, "Gets anti-aliasing on filled shapes (rounded rectangles, circles, etc.).", "bool", "Themes and Styles") });

		parsers->insert({ "get_style_curve_tessellation_tolerance", mvPythonParser({
		}, "Gets Tessellation tolerance.", "float", "Themes and Styles") });

		parsers->insert({ "get_style_circle_segment_max_error", mvPythonParser({
		}, "Gets maximum error (in pixels) allowed when using draw_circle()or drawing rounded corner rectangles with no explicit segment count specified.", "float", "Themes and Styles") });
	}

	static void AddMenuCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_menu_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a menu bar to a window. Must be followed by a call to end_menu_bar.", "None", "Containers") });

		parsers->insert({ "add_menu", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a menu to an existing menu bar. Must be followed by a call to end_menu.", "None", "Containers") });

		parsers->insert({ "add_menu_item", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "shortcut", "Adds a shortcut"},
			{mvPythonDataType::Bool, "check", "Makes menu with checkmarks."},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a menu item to an existing menu.", "None", "Containers") });
	}

	static void AddBasicWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_separator", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a horizontal line.", "None", "Adding Widgets") });

		parsers->insert({ "add_simple_plot", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "value", "Tuple of float values"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "autoscale", "autoscales range based on data (default is True)"},
			{mvPythonDataType::String, "overlay", "overlays text (similar to a plot title)"},
			{mvPythonDataType::Float, "minscale", "used if autoscale is false"},
			{mvPythonDataType::Float, "maxscale", "used if autoscale is false"},
			{mvPythonDataType::Bool, "histogram", "create a histogram"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
			{mvPythonDataType::String, "data_source",""},
		}, "A simple plot for visualization of a set of values", "None", "Adding Widgets") });

		parsers->insert({ "add_progress_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "overlay"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
		}, "Adds a progress bar.", "None", "Adding Widgets") });

		parsers->insert({ "add_image", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "tint_color"},
			{mvPythonDataType::FloatList, "border_color"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
			{mvPythonDataType::FloatList, "uv_min"},
			{mvPythonDataType::FloatList, "uv_max"},
			{mvPythonDataType::String, "secondary_data_source", "Float list for uv_min and uv_max (i.e. float list (minx, miny, maxx, maxy))"},
		}, "Adds an image."
		"uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown."
		"Using(0,0)->(1,1) texture coordinates will generally display the entire texture", "None", "Adding Widgets") });

		parsers->insert({ "add_image_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::FloatList, "tint_color"},
			{mvPythonDataType::FloatList, "background_color"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
			{mvPythonDataType::Integer, "frame_padding",""},
			{mvPythonDataType::FloatList, "uv_min"},
			{mvPythonDataType::FloatList, "uv_max"},
		}, "Adds an image button."
		"uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown."
		"Using(0,0)->(1,1) texture coordinates will generally display the entire texture", "None", "Adding Widgets") });

		parsers->insert({ "add_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "wrap", "number of characters until wraping"},
			{mvPythonDataType::FloatList, "color", "color of the text (rgba)"},
			{mvPythonDataType::Bool, "bullet", "makes the text bulleted"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds text", "None", "Adding Widgets") });

		parsers->insert({ "add_label_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds text with a label. Useful for output values.", "None", "Adding Widgets") });

		parsers->insert({ "add_listbox", mvPythonParser({
			{mvPythonDataType::String, "name", "Name of the listbox"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", "number of items to show"},
			{mvPythonDataType::String, "secondary_data_source", "Data source for the list items."},
		}, "Adds a listbox.", "None", "Adding Widgets") });

		parsers->insert({ "add_combo", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::String, "secondary_data_source", "Data source for the combo items."},
		}, "Adds a combo.", "None", "Adding Widgets") });

		parsers->insert({ "add_selectable", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds a selectable.", "None", "Adding Widgets") });

		parsers->insert({ "add_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text."},
			{mvPythonDataType::Bool, "arrow", "Arrow button."},
			{mvPythonDataType::Integer, "direction", "A cardinal direction"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds a button.", "None", "Adding Widgets") });

		parsers->insert({ "add_indent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds an indent to following items. Must be closed with the unindent command.", "None", "Adding Widgets") });

		parsers->insert({ "unindent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Unindents following items.", "None", "Adding Widgets") });

		parsers->insert({ "add_spacing", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "count"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds vertical spacing.", "None", "Adding Widgets") });

		parsers->insert({ "add_same_line", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "xoffset"},
			{mvPythonDataType::Float, "spacing"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Places a widget on the same line as the previous widget. Can also be used for horizontal spacing.", 
		"None", "Adding Widgets") });

		parsers->insert({ "add_radio_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::String, "secondary_data_source", "data source for radio button items"},
		}, "Adds a set of radio buttons.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_edit3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgb color editing widget. Click and draging the color square will copy the color to be applied on any other color widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_edit4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgba color editing widget. Click and draging the color square will copy the color to be applied on any other color widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_picker3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgb color picking widget. Click and draging the color square will copy the color to be applied on any other color widget. Right Click allows the style of the color picker to be changed.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_picker4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgba color picking widget. Click and draging the color square will copy the color to be applied on any other color widget. Right Click allows the style of the color picker to be changed", "None", "Adding Widgets") });

		parsers->insert({ "add_checkbox", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"}
		}, "Adds a checkbox widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_dummy", mvPythonParser({
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a spacer or 'dummy' object.", "None", "Adding Widgets") });
	}

	static void AddContainterWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_tab_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "reorderable", "allows for moveable tabs"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds a tab bar.", "None", "Containers") });

		parsers->insert({ "add_tab", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "closable"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a tab to a tab bar. Must be closed with the end_tab command.", "None", "Containers") });

		parsers->insert({ "add_collapsing_header", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::Bool, "closable"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"}
		}, "Adds a collapsing header to add items to. Must be closed with the end_collapsing_header command.", 
			"None", "Containers") });

		parsers->insert({ "add_tree_node", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a tree node to add items to. Must be closed with the end_tree_node command.", 
		"None", "Containers") });

		parsers->insert({ "add_group", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "hide"},
			{mvPythonDataType::Bool, "horizontal"},
			{mvPythonDataType::Float, "horizontal_spacing",""},
		}, "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.\
				Must be closed with the end_group command."
		, "None", "Containers") });

		parsers->insert({ "add_child", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::Bool, "border", ""},
		}, "Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end_child.", 
		"None", "Containers") });

		parsers->insert({ "add_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "start_x", "x position the window will start at"},
			{mvPythonDataType::Integer, "start_y", "y position the window will start at"},
			{mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items."},
			{mvPythonDataType::Bool, "resizable", "Allows for the window size to be changed or fixed"},
			{mvPythonDataType::Bool, "title_bar", "Title name for the title bar of the window"},
			{mvPythonDataType::Bool, "movable", "Allows for the window's position to be changed or fixed"},
			{mvPythonDataType::Bool, "hide", "Hides window."},
			{mvPythonDataType::Object, "on_close", "Callback ran when window is closed"},
		}, "Creates a new window for following items to be added to.", 
			"None", "Containers") });

		parsers->insert({ "add_tooltip", mvPythonParser({
			{mvPythonDataType::String, "tipparent", "Sets the item's tool tip to be the same as the named item's tool tip"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"}
		}, "Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for.", 
			"None", "Containers") });

		parsers->insert({ "add_popup", mvPythonParser({
			{mvPythonDataType::String, "popupparent", "Parent that the popup will be assigned to."},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "mousebutton", "The mouse code that will trigger the popup. Default is 1 or mvMouseButton_Right. (mvMouseButton_Left, mvMouseButton_Right, mvMouseButton_Middle, mvMouseButton_X1, mvMouseButton_X2"},
			{mvPythonDataType::Bool, "modal"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds a popup window for an item. This command must come immediately after the item the popup is for. Must be followed by a call to end_popup.", 
		"None", "Containers") });

		parsers->insert({ "end", mvPythonParser({
		}, "Ends a container.", "None", "Containers") });

	}

	static void AddAppCommands(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ "set_start_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback"},
		}, "Callback to run when starting main window.") });

		parsers->insert({ "set_exit_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback"},
		}, "Callback to run when exiting main window.") });

		parsers->insert({ "set_vsync", mvPythonParser({
			{mvPythonDataType::Bool, "value"},
		}, "Sets vsync on or off.") });

		parsers->insert({ "is_dearpygui_running", mvPythonParser({
		}, "Checks if dearpygui is still running", "bool") });

		parsers->insert({ "set_main_window_title", mvPythonParser({
			{mvPythonDataType::String, "title"}
		}, "Sets the title of the main window.") });

		parsers->insert({ "set_main_window_resizable", mvPythonParser({
			{mvPythonDataType::Bool, "resizable"}
		}, "Sets the main window to be resizable.") });

		parsers->insert({ "setup_dearpygui", mvPythonParser({
		}, "Sets up DearPyGui for user controlled rendering. Only call once and you must call cleanup_deapygui when finished.") });

		parsers->insert({ "render_dearpygui_frame", mvPythonParser({
		}, "Renders a DearPyGui frame. Should be called within a user's event loop. Must first call setup_dearpygui outside of event loop.") });

		parsers->insert({ "cleanup_dearpygui", mvPythonParser({
		}, "Cleans up DearPyGui after calling setup_dearpygui.") });

		parsers->insert({ "start_dearpygui", mvPythonParser({
		}, "Starts DearPyGui.") });

		parsers->insert({ "stop_dearpygui", mvPythonParser({
		}, "Stops DearPyGui.") });

		parsers->insert({ "set_global_font_scale", mvPythonParser({
			{mvPythonDataType::Float, "scale", "default is 1.0"}
		}, "Changes the global font scale.") });

		parsers->insert({ "get_global_font_scale", mvPythonParser({
		}, "Returns the global font scale.", "float") });

		parsers->insert({ "run_async_function", mvPythonParser({
			{mvPythonDataType::Object, "name"},
			{mvPythonDataType::Object, "data", "Data that will be sent into the async funstion"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Object, "return_handler"},
		}, "Runs a function asyncronously.") });

		parsers->insert({ "open_file_dialog", mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::Object, "callback"},
			{mvPythonDataType::String, "extensions", "filters items with extensions i.e '.*, .py'"},
		}, "Opens an 'open file' dialog.") });

		parsers->insert({ "select_directory_dialog", mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::Object, "callback"},
		}, "Opens a select directory dialog.") });

		parsers->insert({ "add_data", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "data"}
		}, "Adds data for later retrieval.") });

		parsers->insert({ "get_data", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Retrieves data from storage.", "object") });

		parsers->insert({ "delete_data", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Deletes data from storage.") });

		parsers->insert({ "get_total_time", mvPythonParser({
		}, "Returns total time since app started.", "float") });

		parsers->insert({ "get_delta_time", mvPythonParser({
		}, "Returns time since last frame.", "float") });

		parsers->insert({ "get_main_window_size", mvPythonParser({
		}, "Returns the size of the main window.", "[int, int]") });

		parsers->insert({ "get_thread_count", mvPythonParser({
		}, "Returns the allowable thread count.", "int") });

		parsers->insert({ "is_threadpool_high_performance", mvPythonParser({
		}, "Checks if the threadpool is allowed to use the maximum number of threads.", "bool") });
		
		parsers->insert({ "get_threadpool_timeout", mvPythonParser({
		}, "Returns the threadpool timeout in seconds.", "float") });

		parsers->insert({ "get_active_window", mvPythonParser({
		}, "Returns the active window name.", "str") });

		parsers->insert({ "get_dearpygui_version", mvPythonParser({
		}, "Returns the current version of Dear PyGui.", "str") });

		parsers->insert({ "set_threadpool_timeout", mvPythonParser({
			{mvPythonDataType::Float, "time"}
		}, "Sets the threadpool timeout.") });

		parsers->insert({ "set_thread_count", mvPythonParser({
			{mvPythonDataType::Integer, "threads"}
		}, "Sets number of threads to use if the threadpool is active.") });

		parsers->insert({ "set_threadpool_high_performance", mvPythonParser({
		}, "Set the thread count to the maximum number of threads on your computer.") });

		parsers->insert({ "set_main_window_size", mvPythonParser({
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"}
		}, "Sets the main window size.") });
	}

	PyMODINIT_FUNC PyInit_dearpygui(void);

	std::map<std::string, mvPythonParser>* BuildDearPyGuiInterface();

	std::vector<std::pair<std::string, std::string>> GetAllCommands();

	std::vector<std::pair<std::string, long>> GetModuleConstants();

	void start_dearpygui_error();
}
