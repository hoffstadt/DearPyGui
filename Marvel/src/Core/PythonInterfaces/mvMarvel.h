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

	void AddDrawingCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_drawing", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
		}, "Adds a drawing widget.", "None", "Drawing")});

		parsers->insert({ "set_drawing_size", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"}
		}, "Sets the size of a drawing widget.", "None", "Drawing") });

		parsers->insert({ "set_drawing_origin", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"}
		}, "Sets the drawing origin (default is 0,0).", "None", "Drawing") });

		parsers->insert({ "set_drawing_scale", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"}
		}, "Sets the drawing scale (default is (1,1)).", "None", "Drawing") });

		parsers->insert({ "get_drawing_origin", mvPythonParser({
			{mvPythonDataType::String, "name"},
		}, "Returns the drawing origin.", "(float, float)", "Drawing") });

		parsers->insert({ "get_drawing_scale", mvPythonParser({
			{mvPythonDataType::String, "name"},
		}, "Returns the drawing scale.", "(float, float)", "Drawing") });

		parsers->insert({ "get_drawing_size", mvPythonParser({
			{mvPythonDataType::String, "name"},
		}, "Returns the size of a drawing widget.", "(float, float)", "Drawing") });

		parsers->insert({ "draw_image", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "file"},
			{mvPythonDataType::FloatList, "pmin"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "pmax"},
			{mvPythonDataType::FloatList, "uv_min"},
			{mvPythonDataType::FloatList, "uv_max"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::String, "tag"},
		}, "Draws an image on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_line", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "thickness"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tag"},
		}, "Draws a line on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_arrow", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "thickness"},
			{mvPythonDataType::Integer, "size"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tag"},
		}, "Draws an arrow on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_triangle", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::FloatList, "p3"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a triangle on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_rectangle", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "pmin"},
			{mvPythonDataType::FloatList, "pmax"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::Float, "rounding"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a rectangle on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_quad", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::FloatList, "p3"},
			{mvPythonDataType::FloatList, "p4"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a quad on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_text", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "pos"},
			{mvPythonDataType::String, "text"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "size"},
			{mvPythonDataType::String, "tag"},
		}, "Draws text on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_circle", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "center"},
			{mvPythonDataType::Float, "radius"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "segments"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a circle on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_polyline", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "points"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "closed"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::String, "tag"},
		}, "Draws lines on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_polygon", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "points"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a polygon on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_bezier_curve", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::FloatList, "p3"},
			{mvPythonDataType::FloatList, "p4"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::Integer, "segments"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a bezier curve on a drawing.", "None", "Drawing") });

		parsers->insert({ "clear_drawing", mvPythonParser({
			{mvPythonDataType::String, "draw"}
		}, "Clears a drawing.", "None", "Drawing") });
	}

	void AddPlotCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_plot", mvPythonParser({
		{mvPythonDataType::String, "name"},
		{mvPythonDataType::Optional},
		{mvPythonDataType::String, "xAxisName"},
		{mvPythonDataType::String, "yAxisName"},
		{mvPythonDataType::KeywordOnly},
		{mvPythonDataType::Integer, "flags"},
		{mvPythonDataType::Integer, "xflags"},
		{mvPythonDataType::Integer, "yflags"},
		{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
		{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		{mvPythonDataType::Integer, "width",""},
		{mvPythonDataType::Integer, "height", ""},
		{mvPythonDataType::String, "query_callback", "Callback ran when plot is queried. Should be of the form 'def Callback(sender, data)'\n Data is (x_min, x_max, y_min, y_max)."},
	}, "Adds a plot widget.", "None", "Plotting") });

		parsers->insert({ "clear_plot", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Clears a plot.", "None", "Plotting") });

		parsers->insert({ "is_plot_queried", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Clears a plot.", "boolean", "Plotting") });

		parsers->insert({ "get_plot_query_area", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Clears a plot.", "List(float) -> (x_min, x_max, y_min, y_max)", "Plotting") });

		parsers->insert({ "set_color_map", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Integer, "map"}
		}, "Sets the color map of the plot's series.", "None", "Plotting") });

		parsers->insert({ "set_plot_xlimits", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Float, "xmin"},
			{mvPythonDataType::Float, "xmax"},
		}, "Sets x axis limits of a plot. (can be undone with set_plot_xlimits_auto()", "None", "Plotting") });

		parsers->insert({ "set_plot_ylimits", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Float, "ymin"},
			{mvPythonDataType::Float, "ymax"},
		}, "Sets y axis limits of a plot. (can be undone with set_plot_ylimits_auto()", "None", "Plotting") });

		parsers->insert({ "set_plot_xlimits_auto", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Sets plots x limits to be automatic.", "None", "Plotting") });

		parsers->insert({ "set_plot_ylimits_auto", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Sets plots y limits to be automatic.", "None", "Plotting") });

		parsers->insert({ "add_line_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "data"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::Float, "weight"}
		}, "Adds a line series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_scatter_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "data"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "marker"},
			{mvPythonDataType::Float, "size"},
			{mvPythonDataType::Float, "weight"},
			{mvPythonDataType::FloatList, "outline"},
			{mvPythonDataType::FloatList, "fill"},
		}, "Adds a scatter series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_text_point", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "vertical"},
			{mvPythonDataType::Integer, "xoffset"},
			{mvPythonDataType::Integer, "yoffset"}
		}, "Adds a point with text to a plot.", "None", "Plotting") });
	}

	void AddLogCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "get_log_level", mvPythonParser({
		}, "Returns the log level.", "int", "Logging") });

		parsers->insert({ "clear_log", mvPythonParser({
		}, "Clears the logger.", "None", "Logging") });

		parsers->insert({ "set_log_level", mvPythonParser({
			{mvPythonDataType::Integer, "level"}
		}, "Sets the log level.", "None", "Logging") });

		parsers->insert({ "log", mvPythonParser({
			{mvPythonDataType::String, "message"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "level"}
		}, "Logs a trace level log.", "None", "Logging") });

		parsers->insert({ "log_debug", mvPythonParser({
			{mvPythonDataType::String, "message"}
		}, "Logs a debug level log.", "None", "Logging") });

		parsers->insert({ "log_info", mvPythonParser({
			{mvPythonDataType::String, "message"}
		}, "Logs a info level log.", "None", "Logging") });

		parsers->insert({ "log_warning", mvPythonParser({
			{mvPythonDataType::String, "message"}
		}, "Logs a warning level log.", "None", "Logging") });

		parsers->insert({ "log_error", mvPythonParser({
			{mvPythonDataType::String, "message"}
		}, "Logs a error level log.", "None", "Logging") });
	}

	void AddInputCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "set_render_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window/child/popup/menu is active (default is main window)"},
		}, "Sets the callback to be ran every frame.", "None", "Input Polling") });

		parsers->insert({ "get_mouse_pos", mvPythonParser({
		}, "Returns the current mouse position in relation to the active window (minus titlebar).", 
		"(int, int)", "Input Polling") });

		parsers->insert({ "get_mouse_drag_delta", mvPythonParser({
		}, "Returns the current mouse drag delta", "(float, float)", "Input Polling") });

		parsers->insert({ "is_mouse_button_dragging", mvPythonParser({
			{mvPythonDataType::Integer, "button"},
			{mvPythonDataType::Float, "threshold"},
		}, "Checks if the mouse is dragging.", "Boolean", "Input Polling") });

		parsers->insert({ "is_mouse_button_down", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is pressed.", "Boolean", "Input Polling") });

		parsers->insert({ "is_mouse_button_clicked", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is clicked.", "Boolean", "Input Polling") });

		parsers->insert({ "is_mouse_button_released", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is released.", "Boolean", "Input Polling") });

		parsers->insert({ "is_mouse_button_double_clicked", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is double clicked.", "Boolean", "Input Polling") });

		parsers->insert({ "is_key_pressed", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is pressed.", "Boolean", "Input Polling") });

		parsers->insert({ "is_key_released", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is released.", "Boolean", "Input Polling") });

		parsers->insert({ "is_key_down", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is down.", "Boolean", "Input Polling") });

		parsers->insert({ "set_resize_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a window resizes.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_down_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse down event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_drag_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Float, "threshold"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse drag event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_wheel_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse wheel event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_double_click_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse double click event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_click_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse click event.", "None", "Input Polling") });

		parsers->insert({ "set_key_down_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a key down event.", "None", "Input Polling") }),

		parsers->insert({ "set_key_press_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
			}, "Sets a callback for a key press event.", "None", "Input Polling") });

		parsers->insert({ "set_key_release_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a key release event.", "None", "Input Polling") });
	}

	void AddInputWidgets(std::map<std::string, mvPythonParser>* parsers)
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
			{mvPythonDataType::Bool, "password"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for text values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 2 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 3 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 4 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 2 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 3 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 4 float values.", "None", "Adding Widgets") });
	}

	void AddSliderWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_slider_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::Bool, "vertical"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height","Height of a vertical slider"},
		}, "Adds slider for a single float value", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 2 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 3 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 4 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Bool, "vertical"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height","Height of a vertical slider"},
		}, "Adds slider for a single int value", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 2 int values.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 3 int values.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 4 int values.", "None", "Adding Widgets") });
	}

	void AddDragWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_drag_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a single float value", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 2 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 3 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 4 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a single int value", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 2 int values.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 3 int values.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 4 int values.", "None", "Adding Widgets") });
	}

	void AddTableCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_table", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "headers"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before","Item to add this item before. (runtime adding)"},
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

		parsers->insert({ "add_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::StringList, "row"},
		}, "Adds a row to a table.", "None", "Tables") });

		parsers->insert({ "add_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "column"},
		}, "Adds a column to a table.", "None", "Tables") });

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
		}, "Retrieves data from storage.", "List of integer pairs.", "Tables") });
	}

	void AddItemCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "delete_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Bool, "children_only"}
		}, "Deletes an item if it exists.", "None", "Widget Commands") });

		parsers->insert({ "move_item_up", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves item up if possible and if it exists.", "None", "Widget Commands") });

		parsers->insert({ "move_item_down", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves down up if possible and if it exists.", "None", "Widget Commands") });

		parsers->insert({ "get_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item' callback", "str", "Widget Commands") });

		parsers->insert({ "get_item_height", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's height.", "float", "Widget Commands") });

		parsers->insert({ "get_item_width", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's width.", "float", "Widget Commands") });

		parsers->insert({ "get_item_popup", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's popup.", "str", "Widget Commands") });

		parsers->insert({ "get_item_tip", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's tip.", "str", "Widget Commands") });

		parsers->insert({ "is_item_hovered", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is hovered.", "Boolean", "Widget Commands") });

		parsers->insert({ "is_item_active", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is active.", "Boolean", "Widget Commands") });

		parsers->insert({ "is_item_focused", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is focused.", "Boolean", "Widget Commands") });

		parsers->insert({ "is_item_clicked", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is clicked.", "Boolean", "Widget Commands") });

		parsers->insert({ "is_item_visible", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is visible.", "Boolean", "Widget Commands") });

		parsers->insert({ "is_item_edited", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been edited.", "Boolean", "Widget Commands") });

		parsers->insert({ "is_item_activated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been activated.", "Boolean", "Widget Commands") });

		parsers->insert({ "is_item_deactivated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated.", "Boolean", "Widget Commands") });

		parsers->insert({ "is_item_deactivated_after_edit", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated after editing.", "Boolean", "Widget Commands") });

		parsers->insert({ "is_item_toggled_open", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is toggled.", "Boolean", "Widget Commands") });

		parsers->insert({ "get_item_rect_min", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's minimum allowable size.", "(float, float)", "Widget Commands") });

		parsers->insert({ "get_item_rect_max", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's maximum allowable size.", "(float, float)", "Widget Commands") });

		parsers->insert({ "get_item_rect_size", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's current size.", "(float, float)", "Widget Commands") });

		parsers->insert({ "get_value", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Returns an item's value or None if there is none.", "Depends", "Widget Commands") });

		parsers->insert({ "set_value", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "value"}
		}, "Sets an item's value if applicable.", "None", "Widget Commands") });

		parsers->insert({ "show_item", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Shows an item if it was hidden.", "None", "Widget Commands") });

		parsers->insert({ "hide_item", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Hides an item.", "None", "Widget Commands") });

		parsers->insert({ "set_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "callback"}
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

	void AddStdWindowCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "show_logger", mvPythonParser({
		}, "Shows the logging window.", "None", "Standard Windows") });

		parsers->insert({ "show_documentation", mvPythonParser({
		}, "Shows the documentation window.", "None", "Standard Windows") });

		parsers->insert({ "show_about", mvPythonParser({
		}, "Shows the about window.", "None", "Standard Windows") });

		parsers->insert({ "show_metrics", mvPythonParser({
		}, "Shows the metrics window.", "None", "Standard Windows") });

		parsers->insert({ "show_debug", mvPythonParser({
		}, "Shows the debug window.", "None", "Standard Windows") });

		parsers->insert({ "close_popup", mvPythonParser({
		}, "Needs documentation") });

		parsers->insert({ "show_source", mvPythonParser({
		}, "Shows the source code for the current app.", "None", "Standard Windows") });
	}

	void AddThemeCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "set_theme", mvPythonParser({
			{mvPythonDataType::String, "theme"}
		}, "Set the application's theme to a built-in theme.", "None", "Themes and Styles") });

		parsers->insert({ "get_style_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"}
		}, "Returns a style item's value", "(float, float)", "Themes and Styles") });

		parsers->insert({ "get_theme_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"}
		}, "Returns a theme item's color", "(float, float, float, float)", "Themes and Styles") });

		parsers->insert({ "get_theme", mvPythonParser({
		}, "Returns the current theme.", "str", "Themes and Styles") });

		parsers->insert({ "add_item_color_style", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "style"},
			{mvPythonDataType::FloatList, "color"}
		}, "Needs documentation", "None", "Themes and Styles") });

		parsers->insert({ "change_style_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "y"}
		}, "Needs documentation", "None", "Themes and Styles") });

		parsers->insert({ "change_theme_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"},
			{mvPythonDataType::Float, "r"},
			{mvPythonDataType::Float, "g"},
			{mvPythonDataType::Float, "b"},
			{mvPythonDataType::Float, "a"}
		}, "Needs documentation", "None", "Themes and Styles") });
	}

	void AddMenuCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_menu_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a menu bar to a window. Must be followed by a call to end_menu_bar.", "None", "Containers") });

		parsers->insert({ "add_menu", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a menu to an existing menu bar. Must be followed by a call to end_menu.", "None", "Containers") });

		parsers->insert({ "add_menu_item", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a menu item to an existing menu.", "None", "Containers") });

		parsers->insert({ "end_menu", mvPythonParser({
		}, "Ends the menu created by a call to add_menu.", "None", "Containers") });

		parsers->insert({ "end_menu_bar", mvPythonParser({
		}, "Ends the menu bar created by a call to add_menu_bar.", "None", "Containers") });
	}

	void AddBasicWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_seperator", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before","Item to add this item before. (runtime adding)"},
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
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before","Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
		}, "A simple plot for visualization of a set of values", "None", "Adding Widgets") });

		parsers->insert({ "add_progress_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "overlay"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
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
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
			{mvPythonDataType::FloatList, "uv_min"},
			{mvPythonDataType::FloatList, "uv_max"},
			{mvPythonDataType::String, "secondary_data_source", "Float list for uv_min and uv_max (i.e. float list (minx, miny, maxx, maxy))"},
		}, "Adds an image.", "None", "Adding Widgets") });

		parsers->insert({ "add_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "wrap", "number of characters until wraping"},
			{mvPythonDataType::FloatList, "color", "color of the text (rgba)"},
			{mvPythonDataType::Bool, "bullet", "makes the text bulleted"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds text", "None", "Adding Widgets") });

		parsers->insert({ "add_label_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds text with a label. Useful for output values.", "None", "Adding Widgets") });

		parsers->insert({ "add_listbox", mvPythonParser({
			{mvPythonDataType::String, "name", "Name of the listbox"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
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
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::String, "secondary_data_source", "Data source for the combo items."},
		}, "Adds a combo.", "None", "Adding Widgets") });

		parsers->insert({ "add_selectable", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds a selectable.", "None", "Adding Widgets") });

		parsers->insert({ "add_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text."},
			{mvPythonDataType::Bool, "arrow", "Arrow button."},
			{mvPythonDataType::Integer, "direction", "A cardinal direction"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds a button.", "None", "Adding Widgets") });

		parsers->insert({ "add_indent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds an indent to following items.", "None", "Adding Widgets") });

		parsers->insert({ "unindent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Unindents following items.", "None", "Adding Widgets") });

		parsers->insert({ "add_spacing", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "count"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds vertical spacing.", "None", "Adding Widgets") });

		parsers->insert({ "add_same_line", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "xoffset"},
			{mvPythonDataType::Float, "spacing"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Places a widget on the same line as the previous widget. Can also be used for horizontal spacing.", 
		"None", "Adding Widgets") });

		parsers->insert({ "add_radio_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::String, "secondary_data_source", "data source for radio button items"},
		}, "Adds a set of radio buttons.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_edit3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value (0, 0, 0)"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgb color editing widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_edit4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value (0, 0, 0, 255)"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgba color editing widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_picker3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value (0, 0, 0)"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgb color picking widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_picker4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value (0, 0, 0, 255)"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgba color picking widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_checkbox", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"}
		}, "Adds a checkbox widget.", "None", "Adding Widgets") });
	}

	void AddContainterWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_tab_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "reorderable"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds a tab bar.", "None", "Containers") });

		parsers->insert({ "add_tab", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "closable"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a tab to a tab bar.", "None", "Containers") });

		parsers->insert({ "add_collapsing_header", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"}
		}, "Adds a collapsing header to add items to. Must be closed with the end_collapsing_header command.", 
			"None", "Containers") });

		parsers->insert({ "add_tree_node", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a tree node to add items to. Must be closed with the end_tree_node command.", 
		"None", "Containers") });

		parsers->insert({ "add_group", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""}
		}, "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.\
				Must be closed with the end_group command unless added at runtime."
		, "None", "Containers") });

		parsers->insert({ "add_child", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end_child.", 
		"None", "Containers") });

		parsers->insert({ "add_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "start_x"},
			{mvPythonDataType::Integer, "start_y"},
			{mvPythonDataType::Bool, "autosize"},
			{mvPythonDataType::Bool, "hide"}
		}, "Creates a new window for following items to be added to. Must call end_main_window command before.", 
			"None", "Containers") });

		parsers->insert({ "add_tooltip", mvPythonParser({
			{mvPythonDataType::String, "tipparent"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"}
		}, "Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for.", 
			"None", "Containers") });

		parsers->insert({ "add_popup", mvPythonParser({
			{mvPythonDataType::String, "popupparent"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "mousebutton"},
			{mvPythonDataType::Bool, "modal"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds a popup window for an item. This command must come immediately after the item the popup is for.", 
		"None", "Containers") });

		parsers->insert({ "end_tree_node", mvPythonParser({
		}, "Ends the tree node created by a call to add_tree_node.", "None", "Containers") });

		parsers->insert({ "end_popup", mvPythonParser({
		}, "Ends the popup created by a call to add_popup.", "None", "Containers") });

		parsers->insert({ "end_window", mvPythonParser({
		}, "Ends the window created by a call to add_window.", "None", "Containers") });

		parsers->insert({ "end_group", mvPythonParser({
		}, "Ends the group created by a call to add_group.", "None", "Containers") });

		parsers->insert({ "end_child", mvPythonParser({
		}, "Ends the child created by a call to add_child.", "None", "Containers") });

		parsers->insert({ "end_tab", mvPythonParser({
		}, "Ends the tab created by a call to add_tab.", "None", "Containers") });

		parsers->insert({ "end_tab_bar", mvPythonParser({
		}, "Ends the tab bar created by a call to add_tab_bar", "None", "Containers") });

		parsers->insert({ "end_tooltip", mvPythonParser({
		}, "Ends the tooltip created by a call to add_tooltip.", "None", "Containers") });

		parsers->insert({ "end_collapsing_header", mvPythonParser({
		}, "Ends the collapsing header created by a call to add_collapsing_header.", "None", "Containers") });
	}

	void AddAppCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "set_global_font_scale", mvPythonParser({
			{mvPythonDataType::Float, "scale", "default is 1.0"}
		}, "Changes the global font scale.") });

		parsers->insert({ "get_global_font_scale", mvPythonParser({
		}, "Returns the global font scale.", "float") });

		parsers->insert({ "run_file", mvPythonParser({
			{mvPythonDataType::String, "name", "Executable name"},
			{mvPythonDataType::String, "file", "Path to python file."},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "flags", "Executable flags."},
		}, "Runs a seperate Marvel app.") });

		parsers->insert({ "run_async_function", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "data"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "return_handler"},
		}, "Runs a function asyncronously.") });

		parsers->insert({ "open_file_dialog", mvPythonParser({
			{mvPythonDataType::StringList, "extensions", "i.e [['Python', '*.py']]"},
		}, "Opens an 'open file' dialog.", "str") });

		parsers->insert({ "select_directory_dialog", mvPythonParser({
		}, "Opens a select directory dialog.", "str") });

		parsers->insert({ "save_file_dialog", mvPythonParser({
			{mvPythonDataType::StringList, "extensions", "i.e [['Python', '*.py']]"},
		}, "Opens an 'save file' dialog.", "str") });

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
		}, "Returns the size of the main window.", "(float, float)") });

		parsers->insert({ "get_thread_count", mvPythonParser({
		}, "Returns the allowable thread count.", "int") });

		parsers->insert({ "is_threadpool_high_performance", mvPythonParser({
		}, "Checks if the threadpool is allowed to use the maximum number of threads.", "Boolean") });

		parsers->insert({ "get_threadpool_timeout", mvPythonParser({
		}, "Returns the threadpool timeout in seconds.", "float") });

		parsers->insert({ "get_active_window", mvPythonParser({
		}, "Returns the active window name.", "str") });

		parsers->insert({ "get_marvel_version", mvPythonParser({
		}, "Returns the current version of Marvel.", "str") });

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

}