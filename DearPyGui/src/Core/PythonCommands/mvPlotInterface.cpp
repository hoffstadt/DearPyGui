#include "mvPlotInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddPlotCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_plot", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "xAxisName"},
			{mvPythonDataType::String, "yAxisName"},
			{mvPythonDataType::KeywordOnly},

			// plot flags
			{mvPythonDataType::Bool, "no_legend"},
			{mvPythonDataType::Bool, "no_menus"},
			{mvPythonDataType::Bool, "no_box_select"},
			{mvPythonDataType::Bool, "no_mouse_pos"},
			{mvPythonDataType::Bool, "no_highlight"},
			{mvPythonDataType::Bool, "no_child"},
			{mvPythonDataType::Bool, "query"},
			{mvPythonDataType::Bool, "crosshairs"},
			{mvPythonDataType::Bool, "anti_aliased"},

			// x axis flags
			{mvPythonDataType::Bool, "xaxis_no_gridlines"},
			{mvPythonDataType::Bool, "xaxis_no_tick_marks"},
			{mvPythonDataType::Bool, "xaxis_no_tick_labels"},
			{mvPythonDataType::Bool, "xaxis_log_scale"},
			{mvPythonDataType::Bool, "xaxis_time"},
			{mvPythonDataType::Bool, "xaxis_invert"},
			{mvPythonDataType::Bool, "xaxis_lock_min"},
			{mvPythonDataType::Bool, "xaxis_lock_max"},

			// y axis flags
			{mvPythonDataType::Bool, "yaxis_no_gridlines"},
			{mvPythonDataType::Bool, "yaxis_no_tick_marks"},
			{mvPythonDataType::Bool, "yaxis_no_tick_labels"},
			{mvPythonDataType::Bool, "yaxis_log_scale"},
			{mvPythonDataType::Bool, "yaxis_invert"},
			{mvPythonDataType::Bool, "yaxis_lock_min"},
			{mvPythonDataType::Bool, "yaxis_lock_max"},

			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::String, "query_callback", "Callback ran when plot is queried. Should be of the form 'def Callback(sender, data)'\n Data is (x_min, x_max, y_min, y_max)."},
			
			{mvPythonDataType::Bool, "show_color_scale"},
			{mvPythonDataType::Float, "scale_min"},
			{mvPythonDataType::Float, "scale_max"},
			{mvPythonDataType::Integer, "scale_height"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::Bool, "show"},

		}, "Adds a plot widget.", "None", "Plotting") });

		parsers->insert({ "clear_plot", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Clears a plot.", "None", "Plotting") });

		parsers->insert({ "is_plot_queried", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Returns true if plot was queried", "bool", "Plotting") });

		parsers->insert({ "get_plot_query_area", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Returns the bounding axis limits for the query area [x_min, x_max, y_min, y_max]", "List[float]", "Plotting") });

		parsers->insert({ "set_color_map", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Integer, "map"}
		}, "Sets the color map of the plot's series.", "None", "Plotting") });

		parsers->insert({ "delete_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "series"}
		}, "Deletes a series if it exists.", "None", "Plotting") });

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

		parsers->insert({ "add_image_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::FloatList, "bounds_min"},
			{mvPythonDataType::FloatList, "bounds_max"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "uv_min"},
			{mvPythonDataType::FloatList, "uv_max"},
			{mvPythonDataType::IntList, "tint_color"},
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds a image series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_pie_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "data", "list of [str,float]"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
			{mvPythonDataType::Float, "radius"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "normalize"},
			{mvPythonDataType::Float, "angle"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds a pie series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_line_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "data"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::Float, "weight"},
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds a line series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_error_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "data"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "horizontal"},
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds an error series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_bar_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "data"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "weight"},
			{mvPythonDataType::Bool, "horizontal"},
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds a bar series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_shade_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "data"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::Float, "weight"},
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds a shade series to a plot.", "None", "Plotting") });

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
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds a scatter series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_stem_series", mvPythonParser({
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
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds a stem series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_text_point", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "vertical"},
			{mvPythonDataType::Integer, "xoffset"},
			{mvPythonDataType::Integer, "yoffset"},
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds a point with text to a plot.", "None", "Plotting") });

		parsers->insert({ "add_area_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "data"},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "weight"},
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds an area series to a plot.", "None", "Plotting") });

		parsers->insert({ "add_heat_series", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "data"},
			{mvPythonDataType::Integer, "rows"},
			{mvPythonDataType::Integer, "columns"},
			{mvPythonDataType::Double, "scale_min"},
			{mvPythonDataType::Double, "scale_max"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::FloatList, "bounds_min"},
			{mvPythonDataType::FloatList, "bounds_max"},
			{mvPythonDataType::Bool, "update_bounds"},
		}, "Adds a heat series to a plot.", "None", "Plotting") });

		parsers->insert({ "set_xticks", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Object, "label_pairs", "list of [str,float]"},
		}, "Sets plots x ticks and labels", "None", "Plotting") });

		parsers->insert({ "set_yticks", mvPythonParser({
			{mvPythonDataType::String, "plot"},
			{mvPythonDataType::Object, "label_pairs", "list of [str,float]"},
		}, "Sets plots y ticks and labels", "None", "Plotting") });

		parsers->insert({ "reset_xticks", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Sets plots x ticks and labels back to automatic", "None", "Plotting") });

		parsers->insert({ "reset_yticks", mvPythonParser({
			{mvPythonDataType::String, "plot"},
		}, "Sets plots y ticks and labels back to automatic", "None", "Plotting") });
	}

	PyObject* clear_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["clear_plot"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->clear();

		return GetPyNone();
	}

	PyObject* reset_xticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["reset_xticks"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->resetXTicks();

		return GetPyNone();
	}

	PyObject* reset_yticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["reset_yticks"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->resetXTicks();

		return GetPyNone();
	}

	PyObject* set_xticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		PyObject* label_pairs;

		if (!(*mvApp::GetApp()->getParsers())["set_xticks"].parse(args, kwargs, __FUNCTION__, &plot, &label_pairs))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto mlabel_pairs = ToVectPairStringFloat(label_pairs);

		std::vector<std::string> labels;
		std::vector<double> locations;

		for (const auto& item : mlabel_pairs)
		{
			labels.emplace_back(item.first.c_str());
			locations.emplace_back((double)item.second);
		}

		graph->setXTicks(labels, locations);

		return GetPyNone();
	}

	PyObject* set_yticks(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		PyObject* label_pairs;

		if (!(*mvApp::GetApp()->getParsers())["set_yticks"].parse(args, kwargs, __FUNCTION__, &plot, &label_pairs))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto mlabel_pairs = ToVectPairStringFloat(label_pairs);

		std::vector<std::string> labels;
		std::vector<double> locations;

		for (const auto& item : mlabel_pairs)
		{
			labels.emplace_back(item.first.c_str());
			locations.emplace_back((double)item.second);
		}

		graph->setYTicks(labels, locations);

		return GetPyNone();
	}

	PyObject* set_plot_xlimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["set_plot_xlimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setXLimitsAuto();

		return GetPyNone();
	}

	PyObject* set_plot_ylimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["set_plot_ylimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setYLimitsAuto();

		return GetPyNone();
	}

	PyObject* set_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float xmin;
		float xmax;

		if (!(*mvApp::GetApp()->getParsers())["set_plot_xlimits"].parse(args, kwargs, __FUNCTION__, &plot, &xmin, &xmax))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setXLimits(xmin, xmax);

		return GetPyNone();
	}

	PyObject* set_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float ymin;
		float ymax;

		if (!(*mvApp::GetApp()->getParsers())["set_plot_ylimits"].parse(args, kwargs, __FUNCTION__, &plot, &ymin, &ymax))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setYLimits(ymin, ymax);

		return GetPyNone();
	}

	PyObject* is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["is_plot_queried"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		return Py_BuildValue("b", graph->isPlotQueried());
	}

	PyObject* get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["get_plot_query_area"].parse(args, kwargs, __FUNCTION__, &plot))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto area = graph->getPlotQueryArea();

		return Py_BuildValue("(ffff)", area[0], area[1], area[2], area[3]);
	}

	PyObject* set_color_map(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		int map;

		if (!(*mvApp::GetApp()->getParsers())["set_color_map"].parse(args, kwargs, __FUNCTION__, &plot, &map))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->SetColorMap(map);

		return GetPyNone();
	}

	PyObject* add_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* xAxisName = "";
		const char* yAxisName = "";

		// plot flags
		int no_legend = false;
		int no_menus = false;
		int no_box_select = false;
		int no_mouse_pos = false;
		int no_highlight = false;
		int no_child = false;
		int query = false;
		int crosshairs = false;
		int antialiased = false;

		// x axis flags
		int xaxis_no_gridlines = false;
		int xaxis_no_tick_marks = false;
		int xaxis_no_tick_labels = false;
		int xaxis_log_scale = false;
		int xaxis_time = false;
		int xaxis_invert = false;
		int xaxis_lock_min = false;
		int xaxis_lock_max = false;

		// y axis flags
		int yaxis_no_gridlines = false;
		int yaxis_no_tick_marks = false;
		int yaxis_no_tick_labels = false;
		int yaxis_log_scale = false;
		int yaxis_invert = false;
		int yaxis_lock_min = false;
		int yaxis_lock_max = false;

		const char* parent = "";
		const char* before = "";
		int width = -1;
		int height = -1;
		PyObject* query_callback = nullptr;

		int show_color_scale = false;
		float scale_min = 0.0f;
		float scale_max = 1.0f;
		int scale_height = 100;

		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_plot"].parse(args, kwargs, __FUNCTION__, &name, &xAxisName, &yAxisName,
			&no_legend, &no_menus, &no_box_select, &no_mouse_pos, &no_highlight, &no_child, &query, &crosshairs, &antialiased,
			&xaxis_no_gridlines,
			&xaxis_no_tick_marks,
			&xaxis_no_tick_labels,
			&xaxis_log_scale,
			&xaxis_time,
			&xaxis_invert,
			&xaxis_lock_min,
			&xaxis_lock_max,
			&yaxis_no_gridlines,
			&yaxis_no_tick_marks,
			&yaxis_no_tick_labels,
			&yaxis_log_scale,
			&yaxis_invert,
			&yaxis_lock_min,
			&yaxis_lock_max,
			&parent, &before, &width, &height, &query_callback, &show_color_scale, &scale_min, &scale_max,
			&scale_height, &label, &show))
			return ToPyBool(false);


		mvAppItem* item = new mvPlot(name, query_callback);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* delete_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* series;

		if (!(*mvApp::GetApp()->getParsers())["delete_series"].parse(args, kwargs, __FUNCTION__, &plot, &series))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);
		graph->deleteSeries(series);
		return GetPyNone();
	}

	PyObject* add_image_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		const char* value;
		PyObject* bounds_min;
		PyObject* bounds_max;

		PyObject* uv_min = PyTuple_New(2);
		PyTuple_SetItem(uv_min, 0, PyFloat_FromDouble(0));
		PyTuple_SetItem(uv_min, 1, PyFloat_FromDouble(0));

		PyObject* uv_max = PyTuple_New(2);
		PyTuple_SetItem(uv_max, 0, PyFloat_FromDouble(1));
		PyTuple_SetItem(uv_max, 1, PyFloat_FromDouble(1));

		PyObject* tintcolor = PyTuple_New(4);
		PyTuple_SetItem(tintcolor, 0, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 1, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 2, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 3, PyFloat_FromDouble(255.0));
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_image_series"].parse(args, kwargs, __FUNCTION__, 
			&plot, &name, &value, &bounds_min, &bounds_max, &uv_min, &uv_max, &tintcolor, &update_bounds))
			return GetPyNone();


		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto mbounds_min = ToFloatVect(bounds_min);
		auto mbounds_max = ToFloatVect(bounds_max);
		auto muv_min = ToVec2(uv_min);
		auto muv_max = ToVec2(uv_max);
		auto mcolor = ToColor(tintcolor);

		auto series = new mvImageSeries(name, value, ImPlotPoint(mbounds_min[0], mbounds_min[1]), ImPlotPoint(mbounds_max[0], mbounds_max[1]),
			muv_min, muv_max, mcolor);

		graph->updateSeries(series, update_bounds);


		return GetPyNone();
	}

	PyObject* add_pie_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		float x;
		float y;
		float radius;
		int normalize = false;
		double angle = 90.0;
		const char* format = "%0.2f";
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_pie_series"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &x,
			&y, &radius, &normalize, &angle, &format, &update_bounds))
			return GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add line series requires a list of lists.");
			return GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto mlabel_pairs = ToVectPairStringFloat(data);

		std::vector<mvVec2> points;
		std::vector<std::string> labels;
		for (const auto& item : mlabel_pairs)
		{
			points.push_back({ item.second, 0.0f });
			labels.push_back(item.first);
		}

		auto series = new mvPieSeries(name, points, x, y, radius, normalize, angle, format, labels);

		graph->updateSeries(series, update_bounds);


		return GetPyNone();
	}

	PyObject* add_line_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		float weight = 1.0f;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_line_series"].parse(args, kwargs, __FUNCTION__, 
			&plot, &name, &data, &color, &weight, &update_bounds))
			return GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add line series requires a list of lists.");
			return GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = ToVectVec2(data);

		auto mcolor = ToColor(color);
		if (mcolor.r > 999)
			mcolor.specified = false;

		auto series = new mvLineSeries(name, datapoints, mcolor);
		series->setWeight(weight);
		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_bar_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		float weight = 1.0f;
		int horizontal = false;
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_bar_series"].parse(args, kwargs, __FUNCTION__, 
			&plot, &name, &data, &weight, &horizontal, &update_bounds))
			return GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add bar series requires a list of lists.");
			return GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = ToVectVec2(data);

		auto series = new mvBarSeries(name, datapoints, horizontal);
		series->setWeight(weight);
		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_shade_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		float weight = 1.0f;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));

		PyObject* fill = PyTuple_New(4);
		PyTuple_SetItem(fill, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(fill, 1, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 2, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 3, PyLong_FromLong(255));
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_shade_series"].parse(args, kwargs, __FUNCTION__, 
			&plot, &name, &data, &color, &fill, &weight, &update_bounds))
			return GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add shade series requires a list of lists.");
			return GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = ToVectVec2(data);

		auto mcolor = ToColor(color);
		if (mcolor.r > 999)
			mcolor.specified = false;

		auto mfill = ToColor(fill);
		if (mfill.r > 999)
			mfill.specified = false;

		auto series = new mvShadeSeries(name, datapoints, mcolor, mfill);
		series->setWeight(weight);
		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_scatter_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		int marker = 2;
		float size = 4.0f;
		float weight = 1.0f;
		PyObject* outline = PyTuple_New(4);
		PyTuple_SetItem(outline, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(outline, 1, PyLong_FromLong(0));
		PyTuple_SetItem(outline, 2, PyLong_FromLong(0));
		PyTuple_SetItem(outline, 3, PyLong_FromLong(255));
		PyObject* fill = PyTuple_New(4);
		PyTuple_SetItem(fill, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(fill, 1, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 2, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 3, PyLong_FromLong(255));
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_scatter_series"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &marker,
			&size, &weight, &outline, &fill, &update_bounds))
			return GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add scatter series requires a list of lists.");
			return GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = ToVectVec2(data);

		auto mmarkerOutlineColor = ToColor(outline);
		if (mmarkerOutlineColor.r > 999)
			mmarkerOutlineColor.specified = false;

		auto mmarkerFillColor = ToColor(fill);
		if (mmarkerFillColor.r > 999)
			mmarkerFillColor.specified = false;

		graph->updateSeries(new mvScatterSeries(name, datapoints, marker, size, weight, mmarkerOutlineColor,
			mmarkerFillColor), update_bounds);

		return GetPyNone();
	}

	PyObject* add_stem_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		int marker = 2;
		float size = 4.0f;
		float weight = 1.0f;
		PyObject* outline = PyTuple_New(4);
		PyTuple_SetItem(outline, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(outline, 1, PyLong_FromLong(0));
		PyTuple_SetItem(outline, 2, PyLong_FromLong(0));
		PyTuple_SetItem(outline, 3, PyLong_FromLong(255));
		PyObject* fill = PyTuple_New(4);
		PyTuple_SetItem(fill, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(fill, 1, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 2, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 3, PyLong_FromLong(255));
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_stem_series"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &marker,
			&size, &weight, &outline, &fill, &update_bounds))
			return GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add stem series requires a list of lists.");
			return GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = ToVectVec2(data);

		auto mmarkerOutlineColor = ToColor(outline);
		if (mmarkerOutlineColor.r > 999)
			mmarkerOutlineColor.specified = false;

		auto mmarkerFillColor = ToColor(fill);
		if (mmarkerFillColor.r > 999)
			mmarkerFillColor.specified = false;

		graph->updateSeries(new mvStemSeries(name, datapoints, marker, size, weight, mmarkerOutlineColor,
			mmarkerFillColor), update_bounds);

		return GetPyNone();
	}

	PyObject* add_text_point(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		float x;
		float y;
		int vertical = false;
		int xoffset = 0;
		int yoffset = 0;
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_text_point"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &y, &vertical, &xoffset, &yoffset, &update_bounds))
			return GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->updateSeries(new mvLabelSeries(name, { {(float)x, (float)y} }, xoffset, yoffset, vertical), update_bounds);

		return GetPyNone();
	}

	PyObject* add_area_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		PyObject* color;
		PyObject* fill;
		float weight = 1.0f;
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_area_series"].parse(args, kwargs, __FUNCTION__, 
			&plot, &name, &data, &color, &fill, &weight, &update_bounds))
			return GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add area series requires a list of lists.");
			return GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = ToVectVec2(data);

		auto mcolor = ToColor(color);
		if (mcolor.r > 999)
			mcolor.specified = false;

		auto mfill = ToColor(fill);
		if (mfill.r > 999)
			mfill.specified = false;

		graph->deleteSeries(name);
		auto aseries = new mvAreaSeries(name, datapoints, mcolor, mfill);
		auto lseries = new mvLineSeries(name, datapoints, mcolor);
		aseries->setWeight(weight);
		lseries->setWeight(weight);
		graph->addSeries(aseries, update_bounds);
		graph->addSeries(lseries, update_bounds); // this allows our custom render to work


		return GetPyNone();
	}

	PyObject* add_error_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		int horizontal = false;
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_error_series"].parse(args, kwargs, __FUNCTION__, 
			&plot, &name, &data, &horizontal, &update_bounds))
			return GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add error series requires a list of lists.");
			return GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = ToVectVec4(data);
		auto series = new mvErrorSeries(name, datapoints, horizontal);
		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

	PyObject* add_heat_series(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		int rows;
		int columns;
		double scale_min;
		double scale_max;
		const char* format = "%0.1f";
		PyObject* bounds_min = PyTuple_New(2);
		PyTuple_SetItem(bounds_min, 0, PyLong_FromLong(0));
		PyTuple_SetItem(bounds_min, 1, PyLong_FromLong(0));
		PyObject* bounds_max = PyTuple_New(2);
		PyTuple_SetItem(bounds_max, 0, PyLong_FromLong(1));
		PyTuple_SetItem(bounds_max, 1, PyLong_FromLong(1));
		int update_bounds = true;

		if (!(*mvApp::GetApp()->getParsers())["add_heat_series"].parse(args, kwargs, __FUNCTION__, 
			&plot, &name, &data, &rows, &columns, &scale_min, &scale_max, &format, &bounds_min, &bounds_max, &update_bounds))
			return GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add heat series requires a list of lists.");
			return GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::Plot)
		{
			std::string message = plot;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = ToVectVectFloat(data);
		auto mbounds_min = ToVec2(bounds_min);
		auto mbounds_max = ToVec2(bounds_max);

		auto series = new mvHeatSeries(name, datapoints, rows, columns, scale_min,
			scale_max, format, mbounds_min, mbounds_max);
		graph->updateSeries(series, update_bounds);

		return GetPyNone();
	}

}