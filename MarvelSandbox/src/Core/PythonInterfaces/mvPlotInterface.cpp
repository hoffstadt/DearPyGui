#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/AppItems/mvAppItems.h"
#include "Core/mvPythonTranslator.h"

namespace Marvel {

	PyObject* addPlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* xAxisName = "";
		const char* yAxisName = "";
		int width = -1;
		int height = 0;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::String, "xAxisName"),
			mvPythonDataElement(mvPythonDataType::String, "yAxisName"),
			mvPythonDataElement(mvPythonDataType::Integer, "width"),
			mvPythonDataElement(mvPythonDataType::Integer, "height")
			});

		pl.parse(args, kwargs,__FUNCTION__, &name, &xAxisName, &yAxisName, &width, &height);

		mvAppItem* item = new mvPlot("", name,xAxisName, yAxisName, width, height);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addLineSeries(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		PyObject* style = nullptr;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "plot"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::FloatList, "data"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::IntList, "style", true)
			});

		pl.parse(args, kwargs,__FUNCTION__, &plot, &name, &data, &style);

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = pl.getVectVec2L(data);

		mvSeries* series = new mvLineSeries(name, datapoints);

		graph->addSeries(series);

		if (style)
		{
			auto mstyle = pl.getVectInt2(style);
			for (auto& item : mstyle)
				series->setPlotStyleVariable(item.first, item.second);
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addScatterSeries(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		PyObject* style = nullptr;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "plot"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::FloatList, "data"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::IntList, "style", true)
			});

		pl.parse(args, kwargs,__FUNCTION__, &plot, &name, &data, &style);

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = pl.getVectVec2L(data);

		mvSeries* series = new mvScatterSeries(name, datapoints);

		graph->addSeries(series);

		if (style)
		{
			auto mstyle = pl.getVectInt2(style);
			for (auto& item : mstyle)
				series->setPlotStyleVariable(item.first, item.second);
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	void CreatePlotInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(addPlot, "Not Documented");
		pyModule.addMethod(addLineSeries, "Not Documented");
		pyModule.addMethod(addScatterSeries, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}

}