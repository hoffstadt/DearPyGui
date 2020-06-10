#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/AppItems/mvAppItems.h"
#include "Core/mvPythonTranslator.h"

namespace Marvel {

	PyObject* addPlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width, height;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Integer, "width"),
			mvPythonDataElement(mvPythonDataType::Integer, "height")
			});

		pl.parse(__FUNCTION__, &name, &width, &height);

		mvAppItem* item = new mvPlot("", name, width, height);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* addSeries(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		const char* name;
		const char* seriestype;
		PyObject* data;
		PyObject* color;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "plot"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::String, "seriestype"),
			mvPythonDataElement(mvPythonDataType::FloatList, "data"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color")
			});

		pl.parse(__FUNCTION__, &plot, &name, &seriestype, &data, &color);

		auto pcolor = pl.getColor(color);

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		marvel::mvImGuiGraph* graph = static_cast<mvPlot*>(aplot)->getGraph();

		auto datapoints = pl.getVectVec2L(data);
		std::vector<marvel::mvPoint> points;
		for (const auto& point : datapoints)
			points.emplace_back(point.x, point.y);

		marvel::mvSeries* lineseries1 = new marvel::mvLineSeries(graph, name, points, {(int)pcolor.r*255, (int)pcolor.g*255, (int)pcolor.b*255, (int)pcolor.a*255});

		graph->addSeries(lineseries1);

		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* changePlotSize(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		int width;
		int height;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "plot"),
			mvPythonDataElement(mvPythonDataType::Integer, "width"),
			mvPythonDataElement(mvPythonDataType::Integer, "height")
			});

		pl.parse(__FUNCTION__, &plot, &width, &height);

		mvPlot* aplot = static_cast<mvPlot*>(mvApp::GetApp()->getItem(plot));

		aplot->getGraph()->setSize(width, height);


		Py_INCREF(Py_None);
		return Py_None;
	}

	void CreatePlotInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(addPlot, "Not Documented");
		pyModule.addMethod(addSeries, "Not Documented");
		pyModule.addMethod(changePlotSize, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}