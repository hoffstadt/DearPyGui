#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvPythonParser.h"
#include "mvPythonTranslator.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonParser> Parsers = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbPlot");

	std::map<std::string, mvPythonParser>& BuildPlottingInterface() {

		std::map<std::string, mvPythonParser>* parsers = new std::map< std::string, mvPythonParser>{

			{"add_plot", mvPythonParser({
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
			}, "Adds a plot widget.")},

			{"clear_plot", mvPythonParser({
				{mvPythonDataType::String, "plot"},
			}, "Clears a plot.")},

			{"is_plot_queried", mvPythonParser({
				{mvPythonDataType::String, "plot"},
			}, "Clears a plot.", "boolean")},

			{"get_plot_query_area", mvPythonParser({
				{mvPythonDataType::String, "plot"},
			}, "Clears a plot.", "List(float) -> (x_min, x_max, y_min, y_max)")},

			{"set_color_map", mvPythonParser({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::Integer, "map"}
			}, "Sets the color map of the plot's series.")},

			{"set_plot_xlimits", mvPythonParser({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::Float, "xmin"},
				{mvPythonDataType::Float, "xmax"},
			}, "Sets x axis limits of a plot. (can be undone with set_plot_xlimits_auto()")},

			{"set_plot_ylimits", mvPythonParser({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::Float, "ymin"},
				{mvPythonDataType::Float, "ymax"},
			}, "Sets y axis limits of a plot. (can be undone with set_plot_ylimits_auto()")},

			{"set_plot_xlimits_auto", mvPythonParser({
				{mvPythonDataType::String, "plot"},
			}, "Sets plots x limits to be automatic.")},

			{"set_plot_ylimits_auto", mvPythonParser({
				{mvPythonDataType::String, "plot"},
			}, "Sets plots y limits to be automatic.")},

			{"add_line_series", mvPythonParser({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::FloatList, "data"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "color"},
				{mvPythonDataType::Float, "weight"}
			}, "Adds a line series to a plot.")},

			{"add_scatter_series", mvPythonParser({
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
			}, "Adds a scatter series to a plot.")},

			{"add_text_point", mvPythonParser({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Float, "x"},
				{mvPythonDataType::Float, "y"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Bool, "vertical"},
				{mvPythonDataType::Integer, "xoffset"},
				{mvPythonDataType::Integer, "yoffset"}
			}, "Adds a point with text to a plot.")}

		};

		return *parsers;
	}

	PyObject* clear_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["clear_plot"].parse(args, kwargs, __FUNCTION__, &plot))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->clear();

		Py_RETURN_NONE;
	}

	PyObject* set_plot_xlimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["set_plot_xlimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setXLimitsAuto();

		Py_RETURN_NONE;
	}

	PyObject* set_plot_ylimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["set_plot_ylimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setYLimitsAuto();

		Py_RETURN_NONE;
	}

	PyObject* set_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float xmin;
		float xmax;

		if (!Parsers["set_plot_xlimits"].parse(args, kwargs, __FUNCTION__, &plot, &xmin, &xmax))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setXLimits(xmin, xmax);

		Py_RETURN_NONE;
	}

	PyObject* set_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float ymin;
		float ymax;

		if (!Parsers["set_plot_ylimits"].parse(args, kwargs, __FUNCTION__, &plot, &ymin, &ymax))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setYLimits(ymin, ymax);

		Py_RETURN_NONE;
	}

	PyObject* is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["is_plot_queried"].parse(args, kwargs, __FUNCTION__, &plot))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		return Py_BuildValue("b", graph->isPlotQueried());
	}

	PyObject* get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["get_plot_query_area"].parse(args, kwargs, __FUNCTION__, &plot))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto area = graph->getPlotQueryArea();

		return Py_BuildValue("(ffff)", area[0], area[1], area[2], area[3]);
	}

	PyObject* set_color_map(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		int map;

		if (!Parsers["set_color_map"].parse(args, kwargs, __FUNCTION__, &plot, &map))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->SetColorMap(map);

		Py_RETURN_NONE;
	}

	PyObject* add_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* xAxisName = "";
		const char* yAxisName = "";
		int flags = 0;
		int xflags = 0;
		int yflags = 0;
		const char* parent = "";
		const char* before = "";
		int width = -1;
		int height = -1;
		const char* query_callback = "";

		if (!Parsers["add_plot"].parse(args, kwargs, __FUNCTION__, &name, &xAxisName, &yAxisName, &flags,
			&xflags, &yflags, &parent, &before, &width, &height, &query_callback))
			Py_RETURN_NONE;

		mvAppItem* item = new mvPlot("", name,xAxisName, yAxisName, width, height, flags, xflags, yflags, query_callback);
		item->setWidth(width);
		item->setHeight(height);

		auto ma = mvApp::GetApp();

		// typical run time adding
		if ((!std::string(parent).empty() || !std::string(before).empty()) && ma->isStarted())
			ma->addRuntimeItem(parent, before, item);

		// adding without specifying before or parent, instead using parent stack
		else if (std::string(parent).empty() && std::string(before).empty() && ma->isStarted() && ma->topParent() != nullptr)
			ma->addRuntimeItem(ma->topParent()->getName(), before, item);

		// adding without specifying before or parent, but with empty stack (add to main window)
		else if (std::string(parent).empty() && std::string(before).empty() && ma->isStarted())
			ma->addRuntimeItem("MainWindow", "", item);

		// adding normally but using the runtime style of adding
		else if (!std::string(parent).empty() && !ma->isStarted())
			ma->addRuntimeItem(parent, before, item);

		// typical adding before runtime
		else if (std::string(parent).empty() && !ma->isStarted() && std::string(before).empty())
			ma->addItem(item);

		Py_RETURN_NONE;
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

		if (!Parsers["add_line_series"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &color, &weight))
			Py_RETURN_NONE;

		if (!PyList_Check(data))
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " add line series requires a list of lists.");
			Py_RETURN_NONE;
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = mvPythonTranslator::getVectVec2(data);

		auto mcolor = mvPythonTranslator::getColor(color);
		if (mcolor.r > 999)
			mcolor.specified = false;

		mvSeries* series = new mvLineSeries(name, datapoints, weight, mcolor);

		graph->addSeries(series);

		Py_RETURN_NONE;
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

		if (!Parsers["add_scatter_series"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &marker,
			&size, &weight, &outline, &fill))
			Py_RETURN_NONE;

		if (!PyList_Check(data))
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " add scatter series requires a list of lists.");
			Py_RETURN_NONE;
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = mvPythonTranslator::getVectVec2(data);

		auto mmarkerOutlineColor = mvPythonTranslator::getColor(outline);
		if (mmarkerOutlineColor.r > 999)
			mmarkerOutlineColor.specified = false;

		auto mmarkerFillColor = mvPythonTranslator::getColor(fill);
		if (mmarkerFillColor.r > 999)
			mmarkerFillColor.specified = false;

		mvSeries* series = new mvScatterSeries(name, datapoints, marker, size, weight, mmarkerOutlineColor,
			mmarkerFillColor);

		graph->addSeries(series);

		Py_RETURN_NONE;
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

		if (!Parsers["add_text_point"].parse(args, kwargs, __FUNCTION__, 
			&plot, &name, &x, &y, &vertical, &xoffset, &yoffset))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			Py_RETURN_NONE;
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		mvSeries* series = new mvLabelSeries(name, { {(float)x, (float)y}}, xoffset, yoffset, vertical);

		graph->addSeries(series);

		Py_RETURN_NONE;
	}

	mvPythonModule* CreatePlotInterface()
	{

		auto pyModule = new mvPythonModule("sbPlot", {});

		pyModule->addMethodD(is_plot_queried);
		pyModule->addMethodD(get_plot_query_area);
		pyModule->addMethodD(clear_plot);
		pyModule->addMethodD(set_plot_xlimits_auto);
		pyModule->addMethodD(set_plot_ylimits_auto);
		pyModule->addMethodD(set_plot_xlimits);
		pyModule->addMethodD(set_plot_ylimits);
		pyModule->addMethodD(set_color_map);
		pyModule->addMethodD(add_plot);
		pyModule->addMethodD(add_line_series);
		pyModule->addMethodD(add_scatter_series);
		pyModule->addMethodD(add_text_point);

		return pyModule;
	}

}