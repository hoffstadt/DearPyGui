#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/AppItems/mvAppItems.h"
#include "Core/mvPythonTranslator.h"
#include "mvPlotInterface.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = BuildTranslations();

	PyObject* addPlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* xAxisName = "";
		const char* yAxisName = "";
		int width = -1;
		int height = 0;

		if (!Translators["addPlot"].parse(args, kwargs, __FUNCTION__, &name, &xAxisName, &yAxisName, &width, &height))
			return Py_None;

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

		if (!Translators["addPlot"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &style))
			return Py_None;

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

		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addScatterSeries(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		PyObject* data;
		PyObject* style = nullptr;

		if (!Translators["addPlot"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &style))
			return Py_None;

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

		Py_INCREF(Py_None);
		return Py_None;
	}

	void CreatePlotInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethodD(addPlot);
		pyModule.addMethodD(addLineSeries);
		pyModule.addMethodD(addScatterSeries);

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}

}