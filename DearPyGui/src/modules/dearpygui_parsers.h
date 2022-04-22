#include "mvAppItemCommons.h"
#include "mvFontManager.h"
#include "mvItemRegistry.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>
#include "mvToolManager.h"
#include "mvBuffer.h"
#include "mvVec4Type.h"
#include "mvMat4Type.h"
#include "mvPythonExceptions.h"
#include "mvViewport.h"
#include <stb_image.h>
#include "mvLog.h"
#include "mvProfiler.h"

mv_internal void
InsertParser_Block0(std::map<std::string, mvPythonParser>& parsers)
{
	//-----------------------------------------------------------------------------
	// callback registry
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "frame" });
		args.push_back({ mvPyDataType::Callable, "callback" });
		args.push_back({ mvPyDataType::Object, "user_data", mvArgType::KEYWORD_ARG, "None", "New in 1.3. Optional user data to send to the callback"});

		mvPythonParserSetup setup;
		setup.about = "Sets a callback to run on first frame.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::String;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_frame_callback", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Callable, "callback" });
		args.push_back({ mvPyDataType::Object, "user_data", mvArgType::KEYWORD_ARG, "None", "New in 1.3. Optional user data to send to the callback" });

		mvPythonParserSetup setup;
		setup.about = "Sets a callback to run on last frame.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::String;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_exit_callback", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Callable, "callback" });
		args.push_back({ mvPyDataType::Object, "user_data", mvArgType::KEYWORD_ARG, "None", "New in 1.3. Optional user data to send to the callback" });

		mvPythonParserSetup setup;
		setup.about = "Sets a callback to run on viewport resize.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::String;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_viewport_resize_callback", parser });
	}

	//-----------------------------------------------------------------------------
	// themes
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "theme" });

		mvPythonParserSetup setup;
		setup.about = "Binds a global theme.";
		setup.category = { "Themes" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "bind_theme", parser });
	}

	//-----------------------------------------------------------------------------
	// tables
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "column" });
		args.push_back({ mvPyDataType::IntList, "color" });

		mvPythonParserSetup setup;
		setup.about = "Highlight specified table column.";
		setup.category = { "Tables", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "highlight_table_column", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "column" });

		mvPythonParserSetup setup;
		setup.about = "Unhighlight specified table column.";
		setup.category = { "Tables", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "unhighlight_table_column", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "row" });
		args.push_back({ mvPyDataType::IntList, "color" });

		mvPythonParserSetup setup;
		setup.about = "Set table row color.";
		setup.category = { "Tables", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_table_row_color", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "row" });

		mvPythonParserSetup setup;
		setup.about = "Remove user set table row color.";
		setup.category = { "Tables", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "unset_table_row_color", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(4);
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "row" });
		args.push_back({ mvPyDataType::Integer, "column" });
		args.push_back({ mvPyDataType::IntList, "color" });

		mvPythonParserSetup setup;
		setup.about = "Highlight specified table cell.";
		setup.category = { "Tables", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "highlight_table_cell", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "row" });
		args.push_back({ mvPyDataType::Integer, "column" });

		mvPythonParserSetup setup;
		setup.about = "Unhighlight specified table cell.";
		setup.category = { "Tables", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "unhighlight_table_cell", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "row" });
		args.push_back({ mvPyDataType::IntList, "color" });

		mvPythonParserSetup setup;
		setup.about = "Highlight specified table row.";
		setup.category = { "Tables", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "highlight_table_row", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "row" });

		mvPythonParserSetup setup;
		setup.about = "Unhighlight specified table row.";
		setup.category = { "Tables", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "unhighlight_table_row", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "column" });

		mvPythonParserSetup setup;
		setup.about = "Checks if a table column is highlighted.";
		setup.category = { "Tables", "App Item Operations" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_table_column_highlighted", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "row" });

		mvPythonParserSetup setup;
		setup.about = "Checks if a table row is highlighted.";
		setup.category = { "Tables", "App Item Operations" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_table_row_highlighted", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "table" });
		args.push_back({ mvPyDataType::Integer, "row" });
		args.push_back({ mvPyDataType::Integer, "column" });

		mvPythonParserSetup setup;
		setup.about = "Checks if a table cell is highlighted.";
		setup.category = { "Tables", "App Item Operations" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_table_cell_highlighted", parser });
	}

	//-----------------------------------------------------------------------------
	// plots
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "plot" });

		mvPythonParserSetup setup;
		setup.about = "Returns true if the plot is currently being queried. (Requires plot 'query' kwarg to be enabled)";
		setup.category = { "Plotting", "App Item Operations" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_plot_queried", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "plot" });

		mvPythonParserSetup setup;
		setup.about = "Returns the last/current query area of the plot. (Requires plot 'query' kwarg to be enabled)";
		setup.category = { "Plotting", "App Item Operations" };
		setup.returnType = mvPyDataType::FloatList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_plot_query_area", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "axis" });

		mvPythonParserSetup setup;
		setup.about = "Get the specified axis limits.";
		setup.category = { "Plotting", "App Item Operations" };
		setup.returnType = mvPyDataType::FloatList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_axis_limits", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "axis" });
		args.push_back({ mvPyDataType::Float, "ymin" });
		args.push_back({ mvPyDataType::Float, "ymax" });

		mvPythonParserSetup setup;
		setup.about = "Sets limits on the axis for pan and zoom.";
		setup.category = { "Plotting", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_axis_limits", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "axis" });

		mvPythonParserSetup setup;
		setup.about = "Removes all limits on specified axis.";
		setup.category = { "Plotting", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_axis_limits_auto", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "axis" });

		mvPythonParserSetup setup;
		setup.about = "Sets the axis boundaries max/min in the data series currently on the plot.";
		setup.category = { "Plotting", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "fit_axis_data", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "axis" });

		mvPythonParserSetup setup;
		setup.about = "Removes the manually set axis ticks and applies the default axis ticks";
		setup.category = { "Plotting", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "reset_axis_ticks", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "axis" });
		args.push_back({ mvPyDataType::Object, "label_pairs", mvArgType::REQUIRED_ARG, "...", "Tuples of label and value in the form '((label, axis_value), (label, axis_value), ...)'" });

		mvPythonParserSetup setup;
		setup.about = "Replaces axis ticks with 'label_pairs' argument.";
		setup.category = { "Plotting", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_axis_ticks", parser });
	}

	//-----------------------------------------------------------------------------
	// viewport
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;
		args.reserve(16);
		args.push_back({ mvPyDataType::String, "title", mvArgType::KEYWORD_ARG, "'Dear PyGui'", "Sets the title of the viewport." });
		args.push_back({ mvPyDataType::String, "small_icon", mvArgType::KEYWORD_ARG, "''", "Sets the small icon that is found in the viewport's decorator bar. Must be ***.ico on windows and either ***.ico or ***.png on mac." });
		args.push_back({ mvPyDataType::String, "large_icon", mvArgType::KEYWORD_ARG, "''", "Sets the large icon that is found in the task bar while the app is running. Must be ***.ico on windows and either ***.ico or ***.png on mac." });
		args.push_back({ mvPyDataType::Integer, "width", mvArgType::KEYWORD_ARG, "1280", "Sets the width of the drawable space on the viewport. Does not inclue the border." });
		args.push_back({ mvPyDataType::Integer, "height", mvArgType::KEYWORD_ARG, "800", "Sets the height of the drawable space on the viewport. Does not inclue the border or decorator bar." });
		args.push_back({ mvPyDataType::Integer, "x_pos", mvArgType::KEYWORD_ARG, "100", "Sets x position the viewport will be drawn in screen coordinates." });
		args.push_back({ mvPyDataType::Integer, "y_pos", mvArgType::KEYWORD_ARG, "100", "Sets y position the viewport will be drawn in screen coordinates." });
		args.push_back({ mvPyDataType::Integer, "min_width", mvArgType::KEYWORD_ARG, "250", "Applies a minimuim limit to the width of the viewport." });
		args.push_back({ mvPyDataType::Integer, "max_width", mvArgType::KEYWORD_ARG, "10000", "Applies a maximum limit to the width of the viewport." });
		args.push_back({ mvPyDataType::Integer, "min_height", mvArgType::KEYWORD_ARG, "250", "Applies a minimuim limit to the height of the viewport." });
		args.push_back({ mvPyDataType::Integer, "max_height", mvArgType::KEYWORD_ARG, "10000", "Applies a maximum limit to the height of the viewport." });
		args.push_back({ mvPyDataType::Bool, "resizable", mvArgType::KEYWORD_ARG, "True", "Enables and Disables user ability to resize the viewport." });
		args.push_back({ mvPyDataType::Bool, "vsync", mvArgType::KEYWORD_ARG, "True", "Enables and Disables the renderloop vsync limit. vsync frame value is set by refresh rate of display." });
		args.push_back({ mvPyDataType::Bool, "always_on_top", mvArgType::KEYWORD_ARG, "False", "Forces the viewport to always be drawn ontop of all other viewports." });
		args.push_back({ mvPyDataType::Bool, "decorated", mvArgType::KEYWORD_ARG, "True", "Enabled and disabled the decorator bar at the top of the viewport." });
		args.push_back({ mvPyDataType::FloatList, "clear_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 255)", "Sets the color of the back of the viewport." });

		mvPythonParserSetup setup;
		setup.about = "Creates a viewport. Viewports are required.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "create_viewport", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Bool, "minimized", mvArgType::KEYWORD_ARG, "False", "Sets the state of the viewport to minimized" });
		args.push_back({ mvPyDataType::Bool, "maximized", mvArgType::KEYWORD_ARG, "False", "Sets the state of the viewport to maximized" });
		args.push_back({ mvPyDataType::UUID, "viewport", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });

		mvPythonParserSetup setup;
		setup.about = "Shows the main viewport.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "show_viewport", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Configures a viewport.";
		setup.category = { "General" };
		setup.unspecifiedKwargs = true;
		setup.internal = true;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "configure_viewport", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Returns a viewport's configuration.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::Dict;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_viewport_configuration", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Checks if a viewport has been created and shown.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_viewport_ok", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Maximizes the viewport.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "maximize_viewport", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Minimizes a viewport.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "minimize_viewport", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Toggle viewport fullscreen mode..";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "toggle_viewport_fullscreen", parser });
	}
}

mv_internal void
InsertParser_Block1(std::map<std::string, mvPythonParser>& parsers)
{
	//-----------------------------------------------------------------------------
	// context
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns app configuration.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::Dict;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_app_configuration", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(11);
		args.push_back({ mvPyDataType::Bool, "docking", mvArgType::KEYWORD_ARG, "False", "Enables docking support." });
		args.push_back({ mvPyDataType::Bool, "docking_space", mvArgType::KEYWORD_ARG, "False", "add explicit dockspace over viewport" });
		args.push_back({ mvPyDataType::String, "load_init_file", mvArgType::KEYWORD_ARG, "''", "Load .ini file." });
		args.push_back({ mvPyDataType::String, "init_file", mvArgType::KEYWORD_ARG, "''" });
		args.push_back({ mvPyDataType::Bool, "auto_save_init_file", mvArgType::KEYWORD_ARG, "False" });
		args.push_back({ mvPyDataType::Integer, "device", mvArgType::KEYWORD_ARG, "-1", "Which display adapter to use. (-1 will use default)" });
		args.push_back({ mvPyDataType::Bool, "auto_device", mvArgType::KEYWORD_ARG, "False", "Let us pick the display adapter." });
		args.push_back({ mvPyDataType::Bool, "allow_alias_overwrites", mvArgType::KEYWORD_ARG, "False" });
		args.push_back({ mvPyDataType::Bool, "manual_alias_management", mvArgType::KEYWORD_ARG, "False" });
		args.push_back({ mvPyDataType::Bool, "skip_required_args", mvArgType::KEYWORD_ARG, "False" });
		args.push_back({ mvPyDataType::Bool, "skip_positional_args", mvArgType::KEYWORD_ARG, "False" });
		args.push_back({ mvPyDataType::Bool, "skip_keyword_args", mvArgType::KEYWORD_ARG, "False" });
		args.push_back({ mvPyDataType::Bool, "wait_for_input", mvArgType::KEYWORD_ARG, "False", "New in 1.1. Only update when user input occurs" });
		args.push_back({ mvPyDataType::Bool, "manual_callback_management", mvArgType::KEYWORD_ARG, "False", "New in 1.2"});

		mvPythonParserSetup setup;
		setup.about = "Configures app.";
		setup.category = { "General" };
		setup.unspecifiedKwargs = true;
		setup.internal = true;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "configure_app", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::String, "file" });

		mvPythonParserSetup setup;
		setup.about = "Save dpg.ini file.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "save_init_file", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "delay", mvArgType::KEYWORD_ARG, "32", "Minimal delay in in milliseconds" });

		mvPythonParserSetup setup;
		setup.about = "Waits one frame.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "split_frame", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns frame count.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::Integer;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_frame_count", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::String, "file" });
		args.push_back({ mvPyDataType::Float, "gamma", mvArgType::KEYWORD_ARG, "1.0", "Gamma correction factor. (default is 1.0 to avoid automatic gamma correction on loading." });
		args.push_back({ mvPyDataType::Float, "gamma_scale_factor", mvArgType::KEYWORD_ARG, "1.0", "Gamma scale factor." });

		mvPythonParserSetup setup;
		setup.about = "Loads an image. Returns width, height, channels, mvBuffer";
		setup.category = { "Textures", "Utilities" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "load_image", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(5);
		args.push_back({ mvPyDataType::String, "file" });
		args.push_back({ mvPyDataType::Integer, "width" });
		args.push_back({ mvPyDataType::Integer, "height" });
		args.push_back({ mvPyDataType::Object, "data" });
		args.push_back({ mvPyDataType::Integer, "components", mvArgType::KEYWORD_ARG, "4", "Number of components (1-4). Default of 4."});
		//args.push_back({ mvPyDataType::Integer, "stride_in_bytes", mvArgType::KEYWORD_ARG, "1.0", "Stride in bytes (only used for png)." });
		args.push_back({ mvPyDataType::Integer, "quality", mvArgType::KEYWORD_ARG, "50", "Stride in bytes (only used for jpg)." });

		mvPythonParserSetup setup;
		setup.about = "Saves an image. Possible formats: png, bmp, tga, hdr, jpg.";
		setup.category = { "Textures", "Utilities" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "save_image", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(1);
		args.push_back({ mvPyDataType::String, "file", mvArgType::POSITIONAL_ARG, "''"});
		args.push_back({ mvPyDataType::Callable, "callback", mvArgType::KEYWORD_ARG, "None", "Callback will return framebuffer as an array through the second arg."});

		mvPythonParserSetup setup;
		setup.about = "Outputs frame buffer as a png if file is specified or through the second argument of a callback if specified. Render loop must have been started.";
		setup.category = { "Textures", "Utilities" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "output_frame_buffer", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Generate a new UUID.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "generate_uuid", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Locks render thread mutex.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "lock_mutex", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Unlocks render thread mutex";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "unlock_mutex", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Checks if Dear PyGui is running";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_dearpygui_running", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Sets up Dear PyGui";
		setup.category = { "General" };

		args.push_back({ mvPyDataType::UUID, "viewport", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });
		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "setup_dearpygui", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Render a single Dear PyGui frame.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "render_dearpygui_frame", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Destroys the Dear PyGui context.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "destroy_context", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Creates the Dear PyGui context.";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "create_context", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Stops Dear PyGui";
		setup.category = { "General" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "stop_dearpygui", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns total time since Dear PyGui has started.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::Float;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_total_time", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns time since last frame.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::Float;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_delta_time", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns the average frame rate across 120 frames.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::Float;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_frame_rate", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Bool, "local", mvArgType::KEYWORD_ARG, "True" });

		mvPythonParserSetup setup;
		setup.about = "Returns mouse position.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::IntList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_mouse_pos", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns mouse position in plot.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::IntList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_plot_mouse_pos", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns mouse position in drawing.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::IntList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_drawing_mouse_pos", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns mouse drag delta.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::Float;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_mouse_drag_delta", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "button" });
		args.push_back({ mvPyDataType::Float, "threshold" });

		mvPythonParserSetup setup;
		setup.about = "Checks if mouse button is down and dragging.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_mouse_button_dragging", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "button" });

		mvPythonParserSetup setup;
		setup.about = "Checks if mouse button is down.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_mouse_button_down", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "button" });

		mvPythonParserSetup setup;
		setup.about = "Checks if mouse button is clicked.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_mouse_button_clicked", parser });
	}


	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "button" });

		mvPythonParserSetup setup;
		setup.about = "Checks if mouse button is released.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_mouse_button_released", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "button" });

		mvPythonParserSetup setup;
		setup.about = "Checks if mouse button is double clicked.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_mouse_button_double_clicked", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "key" });

		mvPythonParserSetup setup;
		setup.about = "Checks if key is pressed.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_key_pressed", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "key" });

		mvPythonParserSetup setup;
		setup.about = "Checks if key is released.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_key_released", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Integer, "key" });

		mvPythonParserSetup setup;
		setup.about = "Checks if key is down.";
		setup.category = { "Input Polling" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "is_key_down", parser });
	}
}

mv_internal void
InsertParser_Block2(std::map<std::string, mvPythonParser>& parsers)
{
	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::String, "alias" });
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Adds an alias.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "add_alias", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::Callable, "callback" });
		args.push_back({ mvPyDataType::Object, "user_data", mvArgType::KEYWORD_ARG, "None", "New in 1.3. Optional user data to send to the callback" });

		mvPythonParserSetup setup;
		setup.about = "Captures the next item.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "capture_next_item", parser });
	}


	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::String, "alias" });

		mvPythonParserSetup setup;
		setup.about = "Removes an alias.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "remove_alias", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::String, "alias" });

		mvPythonParserSetup setup;
		setup.about = "Checks if an alias exist.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "does_alias_exist", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::String, "alias" });

		mvPythonParserSetup setup;
		setup.about = "Returns the ID associated with an alias.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_alias_id", parser });
	}

	{

		mvPythonParserSetup setup;
		setup.about = "Pops the top item off the parent stack and return its ID.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, {});
		parsers.insert({ "pop_container_stack", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Shows the imgui demo.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "show_imgui_demo", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Shows the implot demo.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "show_implot_demo", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "container" });
		args.push_back({ mvPyDataType::Integer, "slot" });
		args.push_back({ mvPyDataType::UUIDList, "new_order" });

		mvPythonParserSetup setup;
		setup.about = "Reorders an item's children.";
		setup.category = { "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "reorder_items", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "template_registry" });

		mvPythonParserSetup setup;
		setup.about = "Binds a global template registry.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "bind_template_registry", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Unstages an item.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "unstage", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Shows an item's debug window";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "show_item_debug", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Pushes an item onto the container stack.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "push_container_stack", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns the item on the top of the container stack.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "top_container_stack", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns the last item added.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "last_item", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns the last container item added.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "last_container", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns the last root added (registry or window).";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "last_root", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Emptyes the container stack.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "empty_container_stack", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "0" });
		args.push_back({ mvPyDataType::UUID, "before", mvArgType::KEYWORD_ARG, "0" });

		mvPythonParserSetup setup;
		setup.about = "Moves an item to a new location.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "move_item", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns all windows.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::UUIDList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_windows", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns all items.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::UUIDList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_all_items", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns all aliases.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::StringList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_aliases", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::Bool, "children_only", mvArgType::KEYWORD_ARG, "False" });
		args.push_back({ mvPyDataType::Integer, "slot", mvArgType::KEYWORD_ARG, "-1" });

		mvPythonParserSetup setup;
		setup.about = "Deletes an item..";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "delete_item", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Checks if an item exist..";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::Bool;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "does_item_exist", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Moves an item up.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "move_item_up", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Moves an item down.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "move_item_down", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "Returns the active window.";
		setup.category = { "Item Registry" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_active_window", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "window" });
		args.push_back({ mvPyDataType::Bool, "value" });

		mvPythonParserSetup setup;
		setup.about = "Sets the primary window.";
		setup.category = { "Item Registry" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_primary_window", parser });
	}
}

mv_internal void
InsertParser_Block3(std::map<std::string, mvPythonParser>& parsers)
{

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Focuses an item.";
		setup.category = { "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "focus_item", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Returns an item's information.";
		setup.category = { "App Item Operations" };
		setup.returnType = mvPyDataType::Dict;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_item_info", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Returns an item's configuration.";
		setup.category = { "App Item Operations" };
		setup.returnType = mvPyDataType::Dict;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_item_configuration", parser });
	}

	{

		mvPythonParserSetup setup;
		setup.about = "Returns an item types.";
		setup.category = { "App Item Operations" };
		setup.returnType = mvPyDataType::Dict;

		mvPythonParser parser = FinalizeParser(setup, {});
		parsers.insert({ "get_item_types", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.reserve(3);
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::UUID, "source" });
		args.push_back({ mvPyDataType::Integer, "slot" });

		mvPythonParserSetup setup;
		setup.about = "Sets an item's children.";
		setup.category = { "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_item_children", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::UUID, "font" });

		mvPythonParserSetup setup;
		setup.about = "Sets an item's font.";
		setup.category = { "Fonts", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "bind_item_font", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::String, "alias" });

		mvPythonParserSetup setup;
		setup.about = "Sets an item's alias.";
		setup.category = { "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_item_alias", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Returns an item's alias.";
		setup.category = { "App Item Operations" };
		setup.returnType = mvPyDataType::String;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_item_alias", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::UUID, "handler_registry" });

		mvPythonParserSetup setup;
		setup.about = "Binds an item handler registry to an item.";
		setup.category = { "App Item Operations", "Events" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "bind_item_handler_registry", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::UUID, "theme" });

		mvPythonParserSetup setup;
		setup.about = "Binds a theme to an item.";
		setup.category = { "App Item Operations", "Themes" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "bind_item_theme", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Returns an item's state.";
		setup.category = { "App Item Operations" };
		setup.returnType = mvPyDataType::Dict;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_item_state", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Configures an item.";
		setup.category = { "App Item Operations" };
		setup.unspecifiedKwargs = true;
		setup.internal = true;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "configure_item", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Returns an item's value.";
		setup.category = { "App Item Operations" };
		setup.returnType = mvPyDataType::Any;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_value", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUIDList, "items" });

		mvPythonParserSetup setup;
		setup.about = "Returns values of a list of items.";
		setup.category = { "App Item Operations" };
		setup.returnType = mvPyDataType::Any;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_values", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::Object, "value" });

		mvPythonParserSetup setup;
		setup.about = "Set's an item's value.";
		setup.category = { "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_value", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });

		mvPythonParserSetup setup;
		setup.about = "Resets an item's position after using 'set_item_pos'.";
		setup.category = { "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "reset_pos", parser });
	}
}

mv_internal void
InsertParser_Block4(std::map<std::string, mvPythonParser>& parsers)
{
	//-----------------------------------------------------------------------------
	// node editor
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "node_editor" });

		mvPythonParserSetup setup;
		setup.about = "Returns a node editor's selected nodes.";
		setup.category = { "Node Editor", "App Item Operations" };
		setup.returnType = mvPyDataType::UUIDList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_selected_nodes", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "node_editor" });

		mvPythonParserSetup setup;
		setup.about = "Returns a node editor's selected links.";
		setup.category = { "Node Editor", "App Item Operations" };
		setup.returnType = mvPyDataType::ListStrList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_selected_links", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "node_editor" });

		mvPythonParserSetup setup;
		setup.about = "Clears a node editor's selected links.";
		setup.category = { "Node Editor", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "clear_selected_links", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "node_editor" });

		mvPythonParserSetup setup;
		setup.about = "Clears a node editor's selected nodes.";
		setup.category = { "Node Editor", "App Item Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "clear_selected_nodes", parser });
	}

	//-----------------------------------------------------------------------------
	// fonts
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::Float, "scale" });

		mvPythonParserSetup setup;
		setup.about = "Sets global font scale.";
		setup.category = { "Fonts" };

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_global_font_scale", parser });
	}

	{

		mvPythonParserSetup setup;
		setup.about = "Returns global font scale.";
		setup.category = { "Fonts" };
		setup.returnType = mvPyDataType::Float;
		mvPythonParser parser = FinalizeParser(setup, {});
		parsers.insert({ "get_global_font_scale", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "font" });

		mvPythonParserSetup setup;
		setup.about = "Binds a global font.";
		setup.category = { "Fonts" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "bind_font", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::String, "text" });
		args.push_back({ mvPyDataType::Float, "wrap_width", mvArgType::KEYWORD_ARG, "-1.0", "Wrap width to use (-1.0 turns wrap off)." });
		args.push_back({ mvPyDataType::UUID, "font", mvArgType::KEYWORD_ARG, "0", "Font to use." });

		mvPythonParserSetup setup;
		setup.about = "Returns width/height of text with specified font (must occur after 1st frame).";
		setup.category = { "Fonts" };
		setup.returnType = mvPyDataType::FloatList;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_text_size", parser });
	}

	//-----------------------------------------------------------------------------
	// drawings
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "draw layer to set clip space" });
		args.push_back({ mvPyDataType::Float, "top_left_x", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
		args.push_back({ mvPyDataType::Float, "top_left_y", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
		args.push_back({ mvPyDataType::Float, "width", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
		args.push_back({ mvPyDataType::Float, "height", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
		args.push_back({ mvPyDataType::Float, "min_depth", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
		args.push_back({ mvPyDataType::Float, "max_depth", mvArgType::REQUIRED_ARG, "", "angle to rotate" });

		mvPythonParserSetup setup;
		setup.about = "New in 1.1. Set the clip space for depth clipping and 'viewport' transformation.";
		setup.category = { "Drawlist", "Widgets" };

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "set_clip_space", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "Drawing node to apply transform to." });
		args.push_back({ mvPyDataType::Object, "transform", mvArgType::REQUIRED_ARG, "", "Transformation matrix." });

		mvPythonParserSetup setup;
		setup.about = "New in 1.1. Applies a transformation matrix to a layer.";
		setup.category = { "Drawlist", "Matrix Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "apply_transform", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::Float, "angle", mvArgType::REQUIRED_ARG, "", "angle to rotate" });
		args.push_back({ mvPyDataType::FloatList, "axis", mvArgType::REQUIRED_ARG, "", "axis to rotate around" });

		mvPythonParserSetup setup;
		setup.about = "New in 1.1. Applies a transformation matrix to a layer.";
		setup.category = { "Drawlist", "Matrix Operations" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "create_rotation_matrix", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::FloatList, "scales", mvArgType::REQUIRED_ARG, "", "scale values per axis" });

		mvPythonParserSetup setup;
		setup.about = "New in 1.1. Applies a transformation matrix to a layer.";
		setup.category = { "Drawlist", "Matrix Operations" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "create_scale_matrix", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::FloatList, "translation", mvArgType::REQUIRED_ARG, "", "translation vector" });

		mvPythonParserSetup setup;
		setup.about = "New in 1.1. Creates a translation matrix.";
		setup.category = { "Drawlist", "Matrix Operations" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "create_translation_matrix", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::FloatList, "eye", mvArgType::REQUIRED_ARG, "", "eye position" });
		args.push_back({ mvPyDataType::FloatList, "target", mvArgType::REQUIRED_ARG, "", "target position" });
		args.push_back({ mvPyDataType::FloatList, "up", mvArgType::REQUIRED_ARG, "", "up vector" });

		mvPythonParserSetup setup;
		setup.about = "New in 1.1. Creates a 'Look at matrix'.";
		setup.category = { "Drawlist", "Matrix Operations" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "create_lookat_matrix", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::Float, "fov", mvArgType::REQUIRED_ARG, "", "Field of view (in radians)" });
		args.push_back({ mvPyDataType::Float, "aspect", mvArgType::REQUIRED_ARG, "", "Aspect ratio (width/height)" });
		args.push_back({ mvPyDataType::Float, "zNear", mvArgType::REQUIRED_ARG, "", "Near clipping plane." });
		args.push_back({ mvPyDataType::Float, "zFar", mvArgType::REQUIRED_ARG, "", "Far clipping plane." });

		mvPythonParserSetup setup;
		setup.about = "New in 1.1. Creates a perspective matrix.";
		setup.category = { "Drawlist", "Matrix Operations" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "create_perspective_matrix", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::Float, "left", mvArgType::REQUIRED_ARG, "", "left plane" });
		args.push_back({ mvPyDataType::Float, "right", mvArgType::REQUIRED_ARG, "", "right plane" });
		args.push_back({ mvPyDataType::Float, "bottom", mvArgType::REQUIRED_ARG, "", "bottom plane" });
		args.push_back({ mvPyDataType::Float, "top", mvArgType::REQUIRED_ARG, "", "top plane" });
		args.push_back({ mvPyDataType::Float, "zNear", mvArgType::REQUIRED_ARG, "", "Near clipping plane." });
		args.push_back({ mvPyDataType::Float, "zFar", mvArgType::REQUIRED_ARG, "", "Far clipping plane." });

		mvPythonParserSetup setup;
		setup.about = "New in 1.1. Creates an orthographic matrix.";
		setup.category = { "Drawlist", "Matrix Operations" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "create_orthographic_matrix", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::FloatList, "eye", mvArgType::REQUIRED_ARG, "", "eye position" });
		args.push_back({ mvPyDataType::Float, "pitch", mvArgType::REQUIRED_ARG, "", "pitch (in radians)" });
		args.push_back({ mvPyDataType::Float, "yaw", mvArgType::REQUIRED_ARG, "", "yaw (in radians)" });

		mvPythonParserSetup setup;
		setup.about = "New in 1.1. Create a 'first person shooter' matrix.";
		setup.category = { "Drawlist", "Matrix Operations" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "create_fps_matrix", parser });
	}

	//-----------------------------------------------------------------------------
	// windows
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::Float, "value" });

		mvPythonParserSetup setup;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_x_scroll", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });
		args.push_back({ mvPyDataType::Float, "value" });
		mvPythonParserSetup setup;
		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_y_scroll", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID , "item" });
		mvPythonParserSetup setup;
		setup.returnType = mvPyDataType::Float;
		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_x_scroll", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });
		mvPythonParserSetup setup;
		setup.returnType = mvPyDataType::Float;
		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_y_scroll", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID , "item" });
		mvPythonParserSetup setup;
		setup.returnType = mvPyDataType::Float;
		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_x_scroll_max", parser });
	}

	{
		std::vector<mvPythonDataElement> args;
		args.push_back({ mvPyDataType::UUID, "item" });
		mvPythonParserSetup setup;
		setup.returnType = mvPyDataType::Float;
		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_y_scroll_max", parser });
	}

	//-----------------------------------------------------------------------------
	// file dialogs
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::UUID, "file_dialog" });

		mvPythonParserSetup setup;
		setup.about = "Returns information related to the file dialog. Typically used while the file dialog is in use to query data about the state or info related to the file dialog.";
		setup.category = { "Widgets", "File Dialog" };
		setup.returnType = mvPyDataType::Dict;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_file_dialog_info", parser });
	}

	//-----------------------------------------------------------------------------
	// color maps
	//-----------------------------------------------------------------------------
	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::UUID, "item", mvArgType::REQUIRED_ARG, "", "item that the color map will be applied to" });
		args.push_back({ mvPyDataType::UUID, "source", mvArgType::REQUIRED_ARG, "", "The colormap tag. This should come from a colormap that was added to a colormap registry.  Built in color maps are accessible through their corresponding constants mvPlotColormap_Twilight, mvPlotColormap_***" });

		mvPythonParserSetup setup;
		setup.about = "Sets the color map for widgets that accept it.";
		setup.category = { "Widget Operations" };

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "bind_colormap", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::UUID, "colormap", mvArgType::REQUIRED_ARG, "", "The colormap tag. This should come from a colormap that was added to a colormap registry. Built in color maps are accessible through their corresponding constants mvPlotColormap_Twilight, mvPlotColormap_***" });
		args.push_back({ mvPyDataType::Float, "t", mvArgType::REQUIRED_ARG, "", "Value of the colormap to sample between 0.0-1.0" });

		mvPythonParserSetup setup;
		setup.about = "Returns a color from a colormap given t between 0.0-1.0.";
		setup.category = { "Widget Operations" };
		setup.returnType = mvPyDataType::IntList;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "sample_colormap", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::UUID, "colormap", mvArgType::REQUIRED_ARG, "", "The colormap tag. This should come from a colormap that was added to a colormap registry. Built in color maps are accessible through their corresponding constants mvPlotColormap_Twilight, mvPlotColormap_***" });
		args.push_back({ mvPyDataType::Integer, "index", mvArgType::REQUIRED_ARG, "", "Desired position of the color in the colors list value of the colormap being quiered " });

		mvPythonParserSetup setup;
		setup.about = "Returns a color from a colormap given an index >= 0. (ex. 0 will be the first color in the color list of the color map) Modulo will be performed against the number of items in the color list.";
		setup.category = { "Widget Operations" };
		setup.returnType = mvPyDataType::IntList;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "get_colormap_color", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		args.push_back({ mvPyDataType::UUID, "tool" });

		mvPythonParserSetup setup;
		setup.about = "Shows a built in tool.";
		setup.category = { "Widgets" };
		setup.returnType = mvPyDataType::String;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers.insert({ "show_tool", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "New in 1.2. Returns and clears callback queue.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::Object;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_callback_queue", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "New in 1.3. Sets the clipboard text.";
		setup.category = { "General" };

		args.push_back({ mvPyDataType::String, "text" });

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "set_clipboard_text", parser });
	}

	{
		std::vector<mvPythonDataElement> args;

		mvPythonParserSetup setup;
		setup.about = "New in 1.3. Gets the clipboard text.";
		setup.category = { "General" };
		setup.returnType = mvPyDataType::String;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers.insert({ "get_clipboard_text", parser });
	}
}