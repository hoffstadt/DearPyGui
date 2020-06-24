#include "Core/PythonUtilities/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/AppItems/mvAppItems.h"
#include "Core/PythonUtilities/mvPythonTranslator.h"
#include "mvInterfaces.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("PlottingInterface");

	std::map<std::string, mvPythonTranslator> BuildPlottingInterface() {

		std::map<std::string, mvPythonTranslator> translators = {

			{"addPlot", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::String, "xAxisName"},
				{mvPythonDataType::String, "yAxisName"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "flags"},
				{mvPythonDataType::Integer, "xflags"},
				{mvPythonDataType::Integer, "yflags"}
			}, false, "Needs documentation")},

			{"clearPlot", mvPythonTranslator({
				{mvPythonDataType::String, "plot"},
			}, false, "Needs documentation")},

			{"setColorMap", mvPythonTranslator({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::Integer, "map"}
			}, false, "Needs documentation")},

			{"addLineSeries", mvPythonTranslator({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::FloatList, "data"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "color"},
				{mvPythonDataType::Float, "weight"}
			}, false, "Needs documentation")},

			{"addScatterSeries", mvPythonTranslator({
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
			}, false, "Needs documentation")},

			{"addTextPoint", mvPythonTranslator({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Float, "x"},
				{mvPythonDataType::Float, "y"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Bool, "vertical"},
				{mvPythonDataType::Integer, "xoffset"},
				{mvPythonDataType::Integer, "yoffset"}
			}, false, "Needs documentation")}

		};

		return translators;
	}

	PyObject* clearPlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Translators["clearPlot"].parse(args, kwargs, __FUNCTION__, &plot))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->clear();

		Py_RETURN_NONE;
	}

	PyObject* setColorMap(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		int map;

		if (!Translators["setColorMap"].parse(args, kwargs, __FUNCTION__, &plot, &map))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->SetColorMap(map);

		Py_RETURN_NONE;
	}

	PyObject* addPlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* xAxisName = "";
		const char* yAxisName = "";
		int width = -1;
		int height = 0;
		int flags = 0;
		int xflags = 0;
		int yflags = 0;

		if (!Translators["addPlot"].parse(args, kwargs, __FUNCTION__, &name, &xAxisName, &yAxisName, &width, &height, &flags,
			&xflags, &yflags))
			Py_RETURN_NONE;

		mvAppItem* item = new mvPlot("", name,xAxisName, yAxisName, width, height, flags, xflags, yflags);
		mvApp::GetApp()->addItem(item);

		Py_RETURN_NONE;
	}

	PyObject* addLineSeries(PyObject* self, PyObject* args, PyObject* kwargs)
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

		if (!Translators["addLineSeries"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &color, &weight))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = mvPythonTranslator::getVectVec2(data);

		auto mcolor = mvPythonTranslator::getColor(color);
		if (mcolor.r > 1000)
			mcolor.specified = false;

		mvSeries* series = new mvLineSeries(name, datapoints, weight, mcolor);

		graph->addSeries(series);

		Py_RETURN_NONE;
	}

	PyObject* addScatterSeries(PyObject* self, PyObject* args, PyObject* kwargs)
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

		if (!Translators["addScatterSeries"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &marker,
			&size, &weight, &outline, &fill))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = mvPythonTranslator::getVectVec2(data);

		auto mmarkerOutlineColor = mvPythonTranslator::getColor(outline);
		if (mmarkerOutlineColor.r > 1000)
			mmarkerOutlineColor.specified = false;

		auto mmarkerFillColor = mvPythonTranslator::getColor(fill);
		if (mmarkerFillColor.r > 1000)
			mmarkerFillColor.specified = false;

		mvSeries* series = new mvScatterSeries(name, datapoints, marker, size, weight, mmarkerOutlineColor,
			mmarkerFillColor);

		graph->addSeries(series);

		Py_RETURN_NONE;
	}

	PyObject* addTextPoint(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		float x;
		float y;
		int vertical = false;
		int xoffset = 0;
		int yoffset = 0;

		if (!Translators["addTextPoint"].parse(args, kwargs, __FUNCTION__, 
			&plot, &name, &x, &y, &vertical, &xoffset, &yoffset))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		mvSeries* series = new mvLabelSeries(name, { {(float)x, (float)y}}, xoffset, yoffset, vertical);

		graph->addSeries(series);

		Py_RETURN_NONE;
	}

	void CreatePlotInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethodD(clearPlot);
		pyModule.addMethodD(setColorMap);
		pyModule.addMethodD(addPlot);
		pyModule.addMethodD(addLineSeries);
		pyModule.addMethodD(addScatterSeries);
		pyModule.addMethodD(addTextPoint);

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}

}