#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/AppItems/mvAppItems.h"
#include "Core/mvPythonTranslator.h"
#include "mvPlotInterface.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = BuildTranslations();

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

		if (!Translators["addPlot"].parse(args, kwargs, __FUNCTION__, &name, &xAxisName, &yAxisName, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvPlot("", name,xAxisName, yAxisName, width, height);
		mvApp::GetApp()->addItem(item);

		Py_RETURN_NONE;
	}

	PyObject* addLineSeries(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		PyObject* style = nullptr;

		if (!Translators["addLineSeries"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &style))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = mvPythonTranslator::getVectVec2L(data);

		mvSeries* series = new mvLineSeries(name, datapoints);

		graph->addSeries(series);

		if (style)
		{
			auto mstyle = mvPythonTranslator::getVectInt2(style);
			for (auto& item : mstyle)
				series->setPlotStyleVariable(item.first, item.second);
		}

		Py_RETURN_NONE;
	}

	PyObject* addScatterSeries(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		PyObject* style = nullptr;

		if (!Translators["addScatterSeries"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &style))
			Py_RETURN_NONE;

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = mvPythonTranslator::getVectVec2L(data);

		mvSeries* series = new mvScatterSeries(name, datapoints);

		graph->addSeries(series);

		if (style)
		{
			auto mstyle = mvPythonTranslator::getVectInt2(style);
			for (auto& item : mstyle)
				series->setPlotStyleVariable(item.first, item.second);
		}

		Py_RETURN_NONE;
	}

	void CreatePlotInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethodD(clearPlot);
		pyModule.addMethodD(setColorMap);
		pyModule.addMethodD(addPlot);
		pyModule.addMethodD(addLineSeries);
		pyModule.addMethodD(addScatterSeries);

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}

}