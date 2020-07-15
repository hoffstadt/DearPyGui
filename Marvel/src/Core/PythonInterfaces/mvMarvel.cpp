#include "mvMarvel.h"
#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonParser> Parsers = mvInterfaceRegistry::GetRegistry()->getPythonInterface("marvel");

	static void AddItemWithRuntimeChecks(mvAppItem* item, const char* parent, const char* before)
	{
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
	}

	std::map<std::string, mvPythonParser>& BuildMarvelInterface() {
		
		std::map<std::string, mvPythonParser>* parsers = new std::map< std::string, mvPythonParser>;

		AddDrawingCommands(parsers);
		AddPlotCommands(parsers);
		AddLogCommands(parsers);
		AddInputCommands(parsers);
		AddInputWidgets(parsers);
		AddSliderWidgets(parsers);
		AddDragWidgets(parsers);
		AddTableCommands(parsers);
		AddItemCommands(parsers);
		AddStdWindowCommands(parsers);
		AddMenuCommands(parsers);
		AddThemeCommands(parsers);
		AddBasicWidgets(parsers);
		AddContainterWidgets(parsers);
		AddAppCommands(parsers);
		return *parsers;
	}

	PyObject* run_file(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* file;
		const char* flags = "";

		if (!Parsers["run_file"].parse(args, kwargs, __FUNCTION__, &name, &file, &flags))
			Py_RETURN_NONE;

		RunFile(name, file, flags);

		Py_RETURN_NONE;
	}

	PyObject* add_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_drawing"].parse(args, kwargs, __FUNCTION__, &name, &tip, &parent, &before, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDrawing("", name, width, height);
		item->setTip(tip);
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

	PyObject* set_drawing_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width;
		int height;

		if (!Parsers["set_drawing_size"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
			Py_RETURN_NONE;

		auto drawing = mvApp::GetApp()->getItem(name);

		if (drawing)
		{
			drawing->setWidth(width);
			drawing->setHeight(height);
		}

		Py_RETURN_NONE;
	}

	PyObject* set_drawing_origin(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float x;
		float y;

		if (!Parsers["set_drawing_origin"].parse(args, kwargs, __FUNCTION__, &name, &x, &y))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(name);
		if (item == nullptr)
		{
			std::string message = name;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->setOrigin(x, y);

		Py_RETURN_NONE;
	}

	PyObject* set_drawing_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float x;
		float y;

		if (!Parsers["set_drawing_scale"].parse(args, kwargs, __FUNCTION__, &name, &x, &y))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(name);
		if (item == nullptr)
		{
			std::string message = name;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->setScale(x, y);

		Py_RETURN_NONE;
	}

	PyObject* get_drawing_origin(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["get_drawing_origin"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(name);
		if (item == nullptr)
		{
			std::string message = name;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		return Py_BuildValue("(ff)", dwg->getOrigin().x, dwg->getOrigin().y);
	}

	PyObject* get_drawing_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["get_drawing_scale"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(name);
		if (item == nullptr)
		{
			std::string message = name;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		return Py_BuildValue("(ff)", dwg->getScale().x, dwg->getScale().y);
	}

	PyObject* get_drawing_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["get_drawing_size"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		auto drawing = mvApp::GetApp()->getItem(name);

		if (drawing)
			return Py_BuildValue("(ff)", drawing->getWidth(), drawing->getHeight());

		Py_RETURN_NONE;
	}

	PyObject* draw_image(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* file;
		PyObject* pmin;
		PyObject* pmax = PyTuple_New(2);
		PyTuple_SetItem(pmax, 0, PyFloat_FromDouble(-100));
		PyTuple_SetItem(pmax, 1, PyFloat_FromDouble(-100));
		PyObject* uv_min = PyTuple_New(2);
		PyTuple_SetItem(uv_min, 0, PyFloat_FromDouble(0));
		PyTuple_SetItem(uv_min, 1, PyFloat_FromDouble(0));
		PyObject* uv_max = PyTuple_New(2);
		PyTuple_SetItem(uv_max, 0, PyFloat_FromDouble(1));
		PyTuple_SetItem(uv_max, 1, PyFloat_FromDouble(1));
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyFloat_FromDouble(255));
		PyTuple_SetItem(color, 1, PyFloat_FromDouble(255));
		PyTuple_SetItem(color, 2, PyFloat_FromDouble(255));
		PyTuple_SetItem(color, 3, PyFloat_FromDouble(255));
		const char* tag = "";

		if (!Parsers["draw_image"].parse(args, kwargs, __FUNCTION__, &drawing, &file,
			&pmin, &pmax, &uv_min, &uv_max, &color, &tag))
			Py_RETURN_NONE;

		mvVec2 mpmin = mvPythonTranslator::getVec2(pmin);
		mvVec2 mpmax = mvPythonTranslator::getVec2(pmax);
		mvVec2 muv_min = mvPythonTranslator::getVec2(uv_min);
		mvVec2 muv_max = mvPythonTranslator::getVec2(uv_max);
		mvColor mcolor = mvPythonTranslator::getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawImage(file, mpmin, mpmax, muv_min, muv_max, mcolor, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		int thickness;
		PyObject* p1, * p2;
		PyObject* color;
		const char* tag = "";

		if (!Parsers["draw_line"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &color, &thickness, &tag))
			Py_RETURN_NONE;

		mvVec2 mp1 = mvPythonTranslator::getVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::getVec2(p2);
		mvColor mcolor = mvPythonTranslator::getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawLine(mp1, mp2, mcolor, thickness, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_arrow(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		int thickness;
		int size;
		PyObject* p1, * p2;
		PyObject* color;
		const char* tag = "";

		if (!Parsers["draw_arrow"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &color, &thickness, &size, &tag))
			Py_RETURN_NONE;

		mvVec2 mp1 = mvPythonTranslator::getVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::getVec2(p2);
		mvColor mcolor = mvPythonTranslator::getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawArrow(mp1, mp2, mcolor, thickness, size, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_triangle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, * p3;
		PyObject* color;
		PyObject* fill = nullptr;
		const char* tag = "";

		if (!Parsers["draw_triangle"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &p3, &color, &fill, &thickness, &tag))
			Py_RETURN_NONE;


		mvVec2 mp1 = mvPythonTranslator::getVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::getVec2(p2);
		mvVec2 mp3 = mvPythonTranslator::getVec2(p3);
		mvColor mcolor = mvPythonTranslator::getColor(color);
		mvColor mfill = mvPythonTranslator::getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawTriangle(mp1, mp2, mp3, mcolor, mfill, thickness, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_rectangle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f, rounding = 0.0f;
		PyObject* pmin, * pmax;
		PyObject* color;
		PyObject* fill = nullptr;
		const char* tag = "";

		if (!Parsers["draw_rectangle"].parse(args, kwargs, __FUNCTION__, &drawing, &pmin, &pmax, &color, &fill, &rounding, &thickness, &tag))
			Py_RETURN_NONE;


		mvVec2 mpmax = mvPythonTranslator::getVec2(pmax);
		mvVec2 mpmin = mvPythonTranslator::getVec2(pmin);
		mvColor mcolor = mvPythonTranslator::getColor(color);
		mvColor mfill = mvPythonTranslator::getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawRectangle(mpmin, mpmax, mcolor, mfill, rounding, thickness, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_quad(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, * p3, * p4;
		PyObject* color;
		PyObject* fill = nullptr;
		const char* tag = "";

		if (!Parsers["draw_quad"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &fill, &thickness, &tag))
			Py_RETURN_NONE;


		mvVec2 mp1 = mvPythonTranslator::getVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::getVec2(p2);
		mvVec2 mp3 = mvPythonTranslator::getVec2(p3);
		mvVec2 mp4 = mvPythonTranslator::getVec2(p4);
		mvColor mcolor = mvPythonTranslator::getColor(color);
		mvColor mfill = mvPythonTranslator::getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawQuad(mp1, mp2, mp3, mp4, mcolor, mfill, thickness, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* text;
		PyObject* pos;
		int size = 10;
		PyObject* color = nullptr;
		const char* tag = "";

		if (!Parsers["draw_text"].parse(args, kwargs, __FUNCTION__, &drawing, &pos, &text, &color, &size, &tag))
			Py_RETURN_NONE;

		mvVec2 mpos = mvPythonTranslator::getVec2(pos);
		mvColor mcolor = mvPythonTranslator::getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawText(mpos, text, mcolor, size, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_circle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* center;
		float radius;
		PyObject* color;
		int segments = 12;
		float thickness = 1.0f;
		PyObject* fill = nullptr;
		const char* tag = "";

		if (!Parsers["draw_circle"].parse(args, kwargs, __FUNCTION__, &drawing, &center, &radius, &color, &segments, &thickness, &fill, &tag))
			Py_RETURN_NONE;

		mvVec2 mcenter = mvPythonTranslator::getVec2(center);
		mvColor mcolor = mvPythonTranslator::getColor(color);
		mvColor mfill = mvPythonTranslator::getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawCircle(mcenter, radius, mcolor, segments, thickness, mfill, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_polyline(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* points;
		PyObject* color;
		int closed = false;
		float thickness = 1.0f;
		const char* tag = "";

		if (!Parsers["draw_polyline"].parse(args, kwargs, __FUNCTION__, &drawing, &points, &color, &closed, &thickness, &tag))
			Py_RETURN_NONE;

		auto mpoints = mvPythonTranslator::getVectVec2(points);
		mvColor mcolor = mvPythonTranslator::getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawPolyline(mpoints, mcolor, closed, thickness, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_polygon(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* points;
		PyObject* color;
		PyObject* fill = nullptr;
		float thickness = 1.0f;
		const char* tag = "";

		if (!Parsers["draw_polygon"].parse(args, kwargs, __FUNCTION__, &drawing, &points, &color, &fill, &thickness, &tag))
			Py_RETURN_NONE;

		auto mpoints = mvPythonTranslator::getVectVec2(points);
		mvColor mcolor = mvPythonTranslator::getColor(color);
		mvColor mfill = mvPythonTranslator::getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawPolygon(mpoints, mcolor, mfill, thickness, tag);

		Py_RETURN_NONE;
	}

	PyObject* draw_bezier_curve(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, * p3, * p4;
		PyObject* color;
		int segments = 0;
		const char* tag = "";

		if (!Parsers["draw_bezier_curve"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &thickness, &segments, &tag))
			Py_RETURN_NONE;

		mvVec2 mp1 = mvPythonTranslator::getVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::getVec2(p2);
		mvVec2 mp3 = mvPythonTranslator::getVec2(p3);
		mvVec2 mp4 = mvPythonTranslator::getVec2(p4);
		mvColor mcolor = mvPythonTranslator::getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawBezierCurve(mp1, mp2, mp3, mp4, mcolor, thickness, segments, tag);

		Py_RETURN_NONE;
	}

	PyObject* clear_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		if (!Parsers["clear_drawing"].parse(args, kwargs, __FUNCTION__, &drawing))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			Py_RETURN_NONE;
		}

		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->clear();

		Py_RETURN_NONE;
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

		mvAppItem* item = new mvPlot("", name, xAxisName, yAxisName, width, height, flags, xflags, yflags, query_callback);
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

		mvSeries* series = new mvLabelSeries(name, { {(float)x, (float)y} }, xoffset, yoffset, vertical);

		graph->addSeries(series);

		Py_RETURN_NONE;
	}

	PyObject* show_logger(PyObject* self, PyObject* args)
	{
		mvApp::GetApp()->showStandardWindow("logger");
		Py_INCREF(Py_None);
		Py_RETURN_NONE;
	}

	PyObject* get_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("i", mvAppLog::getLogger()->getLogLevel());
	}

	PyObject* set_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int level;
		if (!Parsers["set_log_level"].parse(args, kwargs, __FUNCTION__, &level))
			Py_RETURN_NONE;
		mvAppLog::getLogger()->setLogLevel(level);
		Py_RETURN_NONE;
	}

	PyObject* log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;
		const char* level = "TRACE";
		if (!Parsers["log"].parse(args, kwargs, __FUNCTION__, &message, &level))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->Log(std::string(message), std::string(level));
		Py_RETURN_NONE;
	}

	PyObject* log_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Parsers["log_debug"].parse(args, kwargs, __FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogDebug(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* log_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Parsers["log_info"].parse(args, kwargs, __FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogInfo(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* log_warning(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Parsers["log_warning"].parse(args, kwargs, __FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogWarning(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* log_error(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Parsers["log_error"].parse(args, kwargs, __FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogError(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* clear_log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppLog::getLogger()->ClearLog();
		Py_RETURN_NONE;
	}

	PyObject* get_key_down_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseDragCallback());
	}

	PyObject* get_mouse_drag_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getKeyDownCallback());
	}

	PyObject* get_key_press_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getKeyPressCallback());
	}

	PyObject* get_key_release_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getKeyReleaseCallback());
	}

	PyObject* get_mouse_click_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseClickCallback());
	}

	PyObject* get_mouse_wheel_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseWheelCallback());
	}

	PyObject* get_mouse_double_click_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseDoubleClickCallback());
	}

	PyObject* get_mouse_down_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseDownCallback());
	}

	PyObject* get_mouse_pos(PyObject* self, PyObject* args)
	{
		mvMousePos pos = mvApp::GetApp()->getMousePosition();
		PyObject* pvalue = Py_BuildValue("(ff)", pos.x, pos.y);
		return pvalue;
	}

	PyObject* get_mouse_drag_delta(PyObject* self, PyObject* args)
	{
		mvVec2 pos = mvApp::GetApp()->getMouseDragDelta();
		PyObject* pvalue = Py_BuildValue("(ff)", pos.x, pos.y);
		return pvalue;
	}

	PyObject* is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Parsers["is_key_pressed"].parse(args, kwargs, __FUNCTION__, &key))
			Py_RETURN_NONE;

		bool pressed = mvApp::GetApp()->isKeyPressed(key);

		PyObject* pvalue = Py_BuildValue("i", pressed);

		return pvalue;
	}

	PyObject* is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Parsers["is_key_released"].parse(args, kwargs, __FUNCTION__, &key))
			Py_RETURN_NONE;

		bool pressed = mvApp::GetApp()->isKeyReleased(key);

		PyObject* pvalue = Py_BuildValue("i", pressed);

		return pvalue;
	}

	PyObject* is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Parsers["is_key_down"].parse(args, kwargs, __FUNCTION__, &key))
			Py_RETURN_NONE;

		bool pressed = mvApp::GetApp()->isKeyDown(key);

		PyObject* pvalue = Py_BuildValue("i", pressed);

		return pvalue;
	}

	PyObject* is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;
		float threshold;

		if (!Parsers["is_mouse_button_dragging"].parse(args, kwargs, __FUNCTION__, &button, &threshold))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseDragging(button, threshold));
	}

	PyObject* is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Parsers["is_mouse_button_down"].parse(args, kwargs, __FUNCTION__, &button))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseButtonDown(button));
	}

	PyObject* is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Parsers["is_mouse_button_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseButtonClicked(button));
	}

	PyObject* is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Parsers["is_mouse_button_double_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseButtonDoubleClicked(button));
	}

	PyObject* is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Parsers["is_mouse_button_released"].parse(args, kwargs, __FUNCTION__, &button))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseButtonReleased(button));
	}

	PyObject* set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Parsers["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseDownCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_mouse_drag_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		float threshold;

		if (!Parsers["set_mouse_drag_callback"].parse(args, kwargs, __FUNCTION__, &callback, &threshold))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseDragCallback(std::string(callback));
		mvApp::GetApp()->setMouseDragThreshold(threshold);

		Py_RETURN_NONE;
	}

	PyObject* set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Parsers["set_mouse_double_click_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseDoubleClickCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_mouse_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Parsers["set_mouse_click_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseClickCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_key_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Parsers["set_key_down_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setKeyDownCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_key_press_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Parsers["set_key_press_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setKeyPressCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_key_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Parsers["set_key_release_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setKeyReleaseCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_mouse_wheel_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Parsers["set_mouse_wheel_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseWheelCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* add_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* headers;
		const char* callback = "";
		const char* parent = "";
		const char* before = "";

		if (!Parsers["add_table"].parse(args, kwargs, __FUNCTION__, &name, &headers, &callback, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTable("", name, mvPythonTranslator::getStringVec(headers));
		item->setCallback(callback);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_simple_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* overlay = "";
		float minscale = 0.0f;
		float maxscale = 0.0f;
		int autoscale = true;
		int histogram = false;
		PyObject* value;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_simple_plot"].parse(args, kwargs,__FUNCTION__, &name, &value, &autoscale, &overlay, 
			&minscale, &maxscale, &histogram, &tip, &parent, &before, &width, &height))
			Py_RETURN_NONE;

		std::vector<float> values = mvPythonTranslator::getFloatVec(value);

		if (autoscale)
		{
			maxscale = values[0];
			minscale = values[0];

			for (auto& item : values)
			{
				if (item > maxscale)
					maxscale = item;
				if (item < minscale)
					minscale = item;
			}
		}

		mvAppItem* item = new mvSimplePlot("", name, values, overlay, minscale, maxscale, height, histogram);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_progress_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		const char* overlay = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_progress_bar"].parse(args, kwargs, __FUNCTION__, &name, &default_value, 
			&overlay, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvProgressBar("", name, default_value, overlay);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		item->setDataSource(data_source);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_image(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* value;
		PyObject* tintcolor = PyTuple_New(4);
		PyTuple_SetItem(tintcolor, 0, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 1, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 2, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 3, PyFloat_FromDouble(1.0));
		PyObject* bordercolor = PyTuple_New(4);
		PyTuple_SetItem(bordercolor, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 3, PyFloat_FromDouble(0.0));
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_image"].parse(args, kwargs, __FUNCTION__, &name, 
			&value, &tintcolor, &bordercolor, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto mtintcolor = mvPythonTranslator::getColor(tintcolor);
		auto mbordercolor = mvPythonTranslator::getColor(bordercolor);

		mvAppItem* item = new mvImage("", name, value, mtintcolor, mbordercolor);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		item->setDataSource(data_source);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_drag_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char*tip = ""; 
		int width = 0; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		if (!Parsers["add_drag_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDragFloat("", name, default_value, speed, min_value, max_value, format, power);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat2("", name, vec.data(), speed, min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat3("", name, vec.data(), speed, min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat4("", name, vec.data(), speed, min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed, 
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDragInt("", name, default_value, speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt2("", name, vec.data(), speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt3("", name, vec.data(), speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt4("", name, vec.data(), speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_slider_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		int vertical = false;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_slider_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, 
			&min_value, &max_value, &format, &power, &vertical, &callback, &tip, &parent, &before, 
			&data_source, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSliderFloat("", name, default_value, min_value, max_value, format, power, vertical);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat2("", name, vec.data(), min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat3("", name, vec.data(), min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat4("", name, vec.data(), min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		int vertical = false;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_slider_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &vertical, &callback, &tip, &parent, &before, &data_source,
			&width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSliderInt("", name, default_value, min_value, max_value, format, vertical);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt2("", name, vec.data(), min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt3("", name, vec.data(), min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt4("", name, vec.data(), min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int wrap = 0;
		int bullet = false;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = ""; 


		if (!Parsers["add_text"].parse(args, kwargs,__FUNCTION__, &name, &wrap, 
			&color, &bullet, &tip, &parent, &before))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);

		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvText("", name, wrap, mcolor, bullet);
		item->setTip(tip);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_label_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		const char* value;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";


		if (!Parsers["add_label_text"].parse(args, kwargs,__FUNCTION__, &name, &value, 
			&color, &tip, &parent, &before, &data_source))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);
		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvLabelText("", std::string(name), value, mcolor);
		item->setTip(tip); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_listbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* items;
		int default_value = 0;
		const char* callback = "";
		const char* tip = ""; 
		int width = 0; 
		int height = 3; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		if (!Parsers["add_listbox"].parse(args, kwargs,__FUNCTION__, &name, &items, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvListbox("", name, mvPythonTranslator::getStringVec(items), default_value, height);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		PyObject* items;
		const char* callback = "";
		const char* tip = ""; 
		int width = 0;
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		if (!Parsers["add_combo"].parse(args, kwargs,__FUNCTION__, &name, &items, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCombo("", name, mvPythonTranslator::getStringVec(items), default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_selectable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = false;
		const char* callback = "";
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		if (!Parsers["add_selectable"].parse(args, kwargs,__FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSelectable("", name, default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int small = false;
		int arrow = false;
		int direction = -1;
		const char* callback = "";
		const char* tip = ""; 
		int width = 0; 
		int height = 0; 
		const char* before = ""; 
		const char* parent = ""; 

		if (!Parsers["add_button"].parse(args, kwargs,__FUNCTION__, &name, &small,
			&arrow, &direction, &callback, &tip, &parent, &before, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvButton("", name, small, arrow, direction);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		const char* hint = "";
		int multiline = 0;
		int no_spaces = false;
		int uppercase = false;
		int decimal = false;
		int hexadecimal = false;
		int readonly = false;
		int password = false;
		const char* callback = "";
		const char* tip = ""; 
		int width = 0; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		int flags = 0;

		if (!Parsers["add_input_text"].parse(args, kwargs,__FUNCTION__, &name, &default_value, &hint, &multiline, &no_spaces, 
			&uppercase, &decimal, &hexadecimal, &readonly, &password, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		if (no_spaces) flags |= ImGuiInputTextFlags_CharsNoBlank;
		if (uppercase) flags |= ImGuiInputTextFlags_CharsUppercase;
		if (decimal) flags |= ImGuiInputTextFlags_CharsDecimal;
		if (hexadecimal) flags |= ImGuiInputTextFlags_CharsHexadecimal;
		if (readonly) flags |= ImGuiInputTextFlags_ReadOnly;
		if (password) flags |= ImGuiInputTextFlags_Password;

		mvAppItem* item = new mvInputText("", name, default_value, hint, multiline, flags);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width);
				
		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_input_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_int"].parse(args, kwargs,__FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputInt("", name, default_value);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_int2"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt2("", name, vec.data());
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_int3"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt3("", name, vec.data());
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
			
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_int4"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt4("", name, vec.data());
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
				
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		const char* format = "%.3f";
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_float"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputFloat("", name, default_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		const char* format = "%.3f";
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_float2"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat2("", name, vec.data(), format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* format = "%.3f";
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_float3"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat3("", name, vec.data(), format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* format = "%.3f";
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_float4"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat4("", name, vec.data(), format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("indent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_indent"].parse(args, kwargs,__FUNCTION__, &name, &offset, 
			&parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvIndent("", name, offset);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("unindent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = "";
		const char* parent = "";

		if (!Parsers["unindent"].parse(args, kwargs,__FUNCTION__, &name, &offset, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvUnindent("", name, offset);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int reorderable = false;
		const char* callback = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";

		if (!Parsers["add_tab_bar"].parse(args, kwargs, __FUNCTION__, &name, &reorderable, 
			&callback, &parent, &before, &data_source))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTabBar("", name, reorderable);
		item->setCallback(callback); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int closeable = false;
		const char* tip = "";
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_tab"].parse(args, kwargs, __FUNCTION__, &name, &closeable,
			&tip, &parent, &before))
			Py_RETURN_NONE;
		
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("add_tab must follow a call to add_tabbar.");

		else if (parentItem->getType() == mvAppItemType::TabBar)
		{
			mvAppItem* item = new mvTab("", name, closeable);
			item->setTip(tip);
			AddItemWithRuntimeChecks(item, parent, before);
			mvApp::GetApp()->pushParent(item);
		}

		else
			mvAppLog::getLogger()->LogError("add_tab was called incorrectly. Did you forget to call end_tab?");

		Py_RETURN_NONE;
	}

	PyObject* end_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_tab must follow a call to add_tab.");

		else if (parentItem->getType() == mvAppItemType::TabItem)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_tab was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* end_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_tabbar must follow a call to add_menubar.");

		else if (parentItem->getType() == mvAppItemType::TabBar)
			mvApp::GetApp()->popParent();

		else
		{
			mvAppLog::getLogger()->LogError("add_menubar was called incorrectly. Did you forget to call end_menu?");
			mvAppLog::getLogger()->LogWarning("Taking corrective action. Clearing parent stack.");

			auto item = mvApp::GetApp()->popParent();
			while (item->getType() != mvAppItemType::TabBar)
			{
				mvApp::GetApp()->popParent();
				item = mvApp::GetApp()->topParent();

				if (item == nullptr)
					break;
			}

		}

		Py_RETURN_NONE;
	}

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_menu_bar"].parse(args, kwargs, __FUNCTION__, &name,
			&parent, &before))
			Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();
		
		if (parentItem->getType() == mvAppItemType::Window)
		{
			auto window = static_cast<mvWindowAppitem*>(parentItem);
			window->addFlag(ImGuiWindowFlags_MenuBar);
			mvAppItem* item = new mvMenuBar(name);
			AddItemWithRuntimeChecks(item, parent, before);
			mvApp::GetApp()->pushParent(item);
		}
		

		Py_RETURN_NONE;
	}

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_menu"].parse(args, kwargs, __FUNCTION__, &name, 
			&tip, &parent, &before))
			Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("add_menu must follow a call to add_menubar.");

		else if (parentItem->getType() == mvAppItemType::MenuBar || parentItem->getType() == mvAppItemType::Menu)
		{
			mvAppItem* item = new mvMenu("", name);
			item->setTip(tip);
			AddItemWithRuntimeChecks(item, parent, before);
			mvApp::GetApp()->pushParent(item);
		}

		else
			mvAppLog::getLogger()->LogError("add_menu was called incorrectly. Did you forget to call end_menu?");

		Py_RETURN_NONE;
	}

	PyObject* end_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();
		
		if(parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_menu must follow a call to add_menu.");

		else if (parentItem->getType() == mvAppItemType::Menu)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_menu was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* end_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_menu_bar must follow a call to add_menu_bar.");

		else if (parentItem->getType() == mvAppItemType::MenuBar)
			mvApp::GetApp()->popParent();

		else
		{
			mvAppLog::getLogger()->LogError("end_menu_bar was called incorrectly. Did you forget to call end_menu?");
			mvAppLog::getLogger()->LogWarning("Taking corrective action. Clearing parent stack.");

			auto item = mvApp::GetApp()->popParent();
			while (item != nullptr)
			{
				mvApp::GetApp()->popParent();
				item = mvApp::GetApp()->topParent();
			}

		}

		Py_RETURN_NONE;
	}

	PyObject* add_menu_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* callback = "";
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_menu_item"].parse(args, kwargs,__FUNCTION__, &name, 
			&callback, &tip, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvMenuItem("", name);
		item->setCallback(callback); 
		item->setTip(tip); 
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("spacing" + std::to_string(i));
		const char* name = sname.c_str();
		int count = 1;
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_spacing"].parse(args, kwargs,__FUNCTION__, &name, &count,
			&parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSpacing("", name, count);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_same_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("sameline" + std::to_string(i));
		const char* name = sname.c_str();
		float xoffset = 0.0f;
		float spacing = 0.0f;
		const char* before = "";
		const char* parent = "";

		if (!Parsers["add_same_line"].parse(args, kwargs,__FUNCTION__, &name,
			&xoffset, &spacing, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSameLine("", name, xoffset, spacing);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* items;
		int default_value = 0;
		const char* callback = "";
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_radio_button"].parse(args, kwargs,__FUNCTION__, &name, &items, 
			&default_value, &callback, &tip, &parent, &before, &data_source))
			Py_RETURN_NONE;

		mvAppItem* item = new mvRadioButton("", name, mvPythonTranslator::getStringVec(items), default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_group(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = ""; 
		int width = 0; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_group"].parse(args, kwargs, __FUNCTION__, &name, 
			&tip, &parent, &before, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvGroup("", name);
		item->setTip(tip);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_group(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_group must follow a call to add_group.");

		else if (parentItem->getType() == mvAppItemType::Group)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_group was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";

		if (!Parsers["add_child"].parse(args, kwargs, __FUNCTION__, &name, 
			&tip, &parent, &before, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvChild("", name);
		item->setTip(tip); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);

		Py_RETURN_NONE;
	}

	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width = -1;
		int height = -1;
		int startx = 0;
		int starty = 0;
		int autosize = false;
		int hide = false;

		if (!Parsers["add_window"].parse(args, kwargs, __FUNCTION__, &name, &width, 
			&height, &startx, &starty, &autosize, &hide))
			Py_RETURN_NONE;

		if (width == -1 && height == -1)
		{
			width = 500;
			height = 500;
		}

		mvAppItem* item = new mvWindowAppitem("", name, width, height, startx, starty, false, autosize);
		mvApp::GetApp()->addWindow(item);
		mvApp::GetApp()->pushParent(item);

		if (hide)
			item->hide();

		Py_RETURN_NONE;
	}

	PyObject* end_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_window must follow a call to add_window.");

		else if (parentItem->getType() == mvAppItemType::Window)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_window was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* end_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_child must follow a call to add_child.");

		else if (parentItem->getType() == mvAppItemType::Child)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_child was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tipparent;
		const char*name;
		const char* parent = ""; 
		const char* before = ""; 

		if (!Parsers["add_tooltip"].parse(args, kwargs, __FUNCTION__, &tipparent, 
			&name, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTooltip(tipparent, name);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_tooltip must follow a call to add_tooltip.");

		else if (parentItem->getType() == mvAppItemType::Tooltip)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_tooltip was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popupparent;
		const char* name;
		int mousebutton = 1;
		int modal = false;
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";

		if (!Parsers["add_popup"].parse(args, kwargs, __FUNCTION__, &popupparent, 
			&name, &mousebutton, &modal, &parent, &before, &width, &height))
			Py_RETURN_NONE;

		auto PopupParent = mvApp::GetApp()->getItem(popupparent);
		if (PopupParent)
			PopupParent->setPopup(name);

		if (std::string(popupparent) == "")
			mvApp::GetApp()->getItem("MainWindow")->setPopup(name);

		mvAppItem* item = new mvPopup(popupparent, name, mousebutton, modal);
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_popup must follow a call to add_group.");

		else if (parentItem->getType() == mvAppItemType::Popup)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_popup was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_open = false;
		int flags = 0;
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_open, &tip, &parent, &before))
			Py_RETURN_NONE;

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvCollapsingHeader("", name, flags);
		item->setTip(tip);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_collapsing_header must follow a call to add_collapsing_header.");

		else if (parentItem->getType() == mvAppItemType::CollapsingHeader)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_collapsing_header was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_tree_node(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_open = false;
		int flags = 0;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!Parsers["add_tree_node"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_open, &tip, &parent, &before))
			Py_RETURN_NONE;

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvTreeNode("", name, flags);
		item->setTip(tip);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_tree_node(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_tree_node must follow a call to add_tree_node.");

		else if (parentItem->getType() == mvAppItemType::TreeNode)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_tree_node was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_seperator(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("seperator" + std::to_string(i));
		const char* name = sname.c_str();
		const char* tip = "";
		const char* parent = "";
		const char* before = "";

		if (!Parsers["add_seperator"].parse(args, kwargs, __FUNCTION__, &name, &tip, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSeparator("", name);
		item->setTip(tip);

		AddItemWithRuntimeChecks(item, parent, before);
		
		Py_RETURN_NONE;
	}
	
	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_color_edit3"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorEdit3("", name, color);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_color_edit4"].parse(args, kwargs,__FUNCTION__, &name, &default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorEdit4("", name, color);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_color_picker3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorPicker3("", name, color);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(1));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_color_picker4"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorPicker4("", name, color);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		const char* callback = "";
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";


		if (!Parsers["add_checkbox"].parse(args, kwargs,__FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCheckbox("", name, default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* set_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		const char* value;

		if (!Parsers["set_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setTableItem(row, column, value);

		Py_RETURN_NONE;
	}

	PyObject* delete_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;

		if (!Parsers["delete_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteRow(row);

		Py_RETURN_NONE;
	}

	PyObject* delete_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column;

		if (!Parsers["delete_column"].parse(args, kwargs, __FUNCTION__, &table, &column))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteColumn(column);

		Py_RETURN_NONE;
	}

	PyObject* add_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		PyObject* row;

		if (!Parsers["add_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		auto prow = mvPythonTranslator::getStringVec(row);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->addRow(prow);

		Py_RETURN_NONE;
	}

	PyObject* add_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		const char* name;
		PyObject* column;

		if (!Parsers["add_column"].parse(args, kwargs, __FUNCTION__, &table, &name, &column))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		auto pcolumn = mvPythonTranslator::getStringVec(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->addColumn(name, pcolumn);

		Py_RETURN_NONE;
	}

	PyObject* insert_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row_index;
		PyObject* row;

		if (!Parsers["insert_row"].parse(args, kwargs, __FUNCTION__, &table, &row_index, &row))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		auto prow = mvPythonTranslator::getStringVec(row);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->insertRow(row_index, prow);

		Py_RETURN_NONE;
	}

	PyObject* insert_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column_index;
		const char* name;
		PyObject* column;

		if (!Parsers["insert_column"].parse(args, kwargs, __FUNCTION__, &table, &column_index, &name, &column))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		auto prow = mvPythonTranslator::getStringVec(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->insertColumn(column_index, name, prow);

		Py_RETURN_NONE;
	}

	PyObject* set_table_selection(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		int value;

		if (!Parsers["set_table_selection"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setSelection(row, column, value);

		Py_RETURN_NONE;
	}

	PyObject* get_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;

		if (!Parsers["get_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return Py_BuildValue("s", atable->getTableItem(row, column).c_str());

	}

	PyObject* get_table_selections(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!Parsers["get_table_selections"].parse(args, kwargs, __FUNCTION__, &table))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return atable->getSelections();
	}

	PyObject* run_async_function(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* return_handler = "";
		PyObject* data;

		if (!Parsers["run_async_function"].parse(args, kwargs, __FUNCTION__, &name, &data, &return_handler))
			Py_RETURN_NONE;

		mvApp::GetApp()->addMTCallback(name, data, return_handler);

		Py_RETURN_NONE;

	}

	PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			Py_RETURN_NONE;
		}

		const char* item;
		int childrenOnly = false;

		if (!Parsers["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			Py_RETURN_NONE;

		if (childrenOnly)
			mvApp::GetApp()->deleteItemChildren(item);
		else
			mvApp::GetApp()->deleteItem(item);

		Py_RETURN_NONE;

	}

	PyObject* select_directory_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::string file = PickDirectory("");
		return Py_BuildValue("s", file.c_str());
	}

	PyObject* open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* extensions;

		if (!Parsers["open_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			Py_RETURN_NONE;

		std::string file = OpenFile(mvPythonTranslator::getStringPairVec(extensions));

		return Py_BuildValue("s", file.c_str());
	}

	PyObject* save_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* extensions;

		if (!Parsers["save_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			Py_RETURN_NONE;

		std::string file = SaveFile(mvPythonTranslator::getStringPairVec(extensions));

		return Py_BuildValue("s", SaveFile(mvPythonTranslator::getStringPairVec(extensions)).c_str());
	}

	PyObject* move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			Py_RETURN_NONE;
		}

		const char* item;

		if (!Parsers["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		mvApp::GetApp()->moveItemUp(item);

		Py_RETURN_NONE;

	}

	PyObject* move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			Py_RETURN_NONE;
		}

		const char* item;

		if (!Parsers["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		mvApp::GetApp()->moveItemDown(item);

		Py_RETURN_NONE;

	}

	PyObject* get_style_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!Parsers["get_style_item"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto values = mvApp::GetApp()->getStyleItem(item);

		return Py_BuildValue("(ff)", values.first, values.second);
	}

	PyObject* get_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!Parsers["get_theme_item"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto color = mvApp::GetApp()->getThemeItem(item);

		return Py_BuildValue("(iiii)", color.r, color.g, color.b, color.a);
	}

	PyObject* get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("f", mvApp::GetApp()->getTotalTime());
	}

	PyObject* get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("f", mvApp::GetApp()->getDeltaTime());
	}

	PyObject* get_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("s", appitem->getCallback());

		Py_RETURN_NONE;
	}

	PyObject* get_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_height"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->getHeight());

		Py_RETURN_NONE;
	}

	PyObject* get_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_width"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->getWidth());

		Py_RETURN_NONE;
	}

	PyObject* get_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_popup"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("s", appitem->getPopup());

		Py_RETURN_NONE;
	}

	PyObject* get_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_tip"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("s", appitem->getTip());

		Py_RETURN_NONE;
	}

	PyObject* get_main_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMainCallback());
	}

	PyObject* get_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("(i,i)", mvApp::GetApp()->getWindowWidth(), mvApp::GetApp()->getWindowWidth());
	}

	PyObject* get_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getAppTheme());
	}

	PyObject* get_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("i", mvApp::GetApp()->getThreadCount());
	}

	PyObject* is_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("b", mvApp::GetApp()->usingThreadPoolHighPerformance());
	}

	PyObject* get_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("f", mvApp::GetApp()->getThreadPoolTimeout());
	}

	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getActiveWindow());
	}

	PyObject* get_marvel_version(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->GetVersion());
	}

	PyObject* set_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float time;

		if (!Parsers["set_threadpool_timeout"].parse(args, kwargs, __FUNCTION__, &time))
			Py_RETURN_NONE;

		mvApp::GetApp()->setThreadPoolTimeout(time);

		Py_RETURN_NONE;
	}

	PyObject* set_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		int threads;

		if (!Parsers["set_thread_count"].parse(args, kwargs, __FUNCTION__, &threads))
			Py_RETURN_NONE;

		mvApp::GetApp()->setThreadCount(threads);

		Py_RETURN_NONE;
	}

	PyObject* add_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		PyObject* data;

		if (!Parsers["add_data"].parse(args, kwargs, __FUNCTION__, &name, &data))
			Py_RETURN_NONE;

		mvApp::GetApp()->addData(name, data);
		Py_XINCREF(data);

		Py_RETURN_NONE;
	}

	PyObject* get_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!Parsers["get_data"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		auto result = mvApp::GetApp()->getData(name);

		if (result)
			return result;

		Py_RETURN_NONE;
	}

	PyObject* delete_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!Parsers["delete_data"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		mvApp::GetApp()->deleteData(name);

		Py_RETURN_NONE;
	}

	PyObject* set_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->setThreadPoolHighPerformance();
		Py_RETURN_NONE;
	}

	PyObject* add_item_color_style(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int style;
		PyObject* color;

		if (!Parsers["add_item_color_style"].parse(args, kwargs, __FUNCTION__, &item, &style, &color))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);

		mvApp::GetApp()->addItemColorStyle(item, style, mcolor);

		Py_RETURN_NONE;
	}

	PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_hovered"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemHovered());

		Py_RETURN_NONE;
	}

	PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_active"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemActive());

		Py_RETURN_NONE;
	}

	PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_focused"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemFocused());

		Py_RETURN_NONE;
	}

	PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_clicked"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemClicked());

		Py_RETURN_NONE;
	}

	PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_visible"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemVisible());

		Py_RETURN_NONE;
	}

	PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_edited"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemEdited());

		Py_RETURN_NONE;
	}

	PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_activated"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemActivated());

		Py_RETURN_NONE;
	}

	PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_deactivated"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemDeactivated());

		Py_RETURN_NONE;
	}

	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_deactivated_after_edit"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemDeactivatedAfterEdit());

		Py_RETURN_NONE;
	}

	PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_toggled_open"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemToogledOpen());

		Py_RETURN_NONE;
	}

	PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_rect_min"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMin();
			PyObject* pvalue = PyTuple_New(2);
			PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
			PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));
			return pvalue;
		}

		Py_RETURN_NONE;
	}

	PyObject* get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_rect_max"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMax();
			PyObject* pvalue = PyTuple_New(2);
			PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
			PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));
			return pvalue;
		}

		Py_RETURN_NONE;
	}

	PyObject* get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_rect_size"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectSize();
			PyObject* pvalue = PyTuple_New(2);
			PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
			PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));
			return pvalue;
		}

		Py_RETURN_NONE;
	}

	PyObject* change_style_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		float x = 0.0f;
		float y = 0.0f;

		if (!Parsers["change_style_item"].parse(args, kwargs, __FUNCTION__, &item, &x, &y))
			Py_RETURN_NONE;

		mvApp::GetApp()->changeStyleItem(item, x, y);

		Py_RETURN_NONE;
	}

	PyObject* change_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		int r, g, b, a;

		if (!Parsers["change_theme_item"].parse(args, kwargs, __FUNCTION__, &item, &r, &g, &b, &a))
			Py_RETURN_NONE;

		mvApp::GetApp()->changeThemeItem(item, { r, g, b, a });



		Py_RETURN_NONE;
	}

	PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["get_value"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			Py_RETURN_NONE;

		return item->getPyValue();
	}

	PyObject* set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if (!Parsers["set_value"].parse(args, kwargs, __FUNCTION__, &name, &value))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			Py_RETURN_NONE;

		item->setPyValue(value);
		Py_RETURN_NONE;
	}

	PyObject* show_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["show_item"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->show();

		Py_RETURN_NONE;
	}

	PyObject* hide_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["hide_item"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->hide();

		Py_RETURN_NONE;
	}

	PyObject* set_main_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;


		if (!Parsers["set_main_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMainCallback(std::string(callback));



		Py_RETURN_NONE;
	}

	PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* item;

		if (!Parsers["set_item_callback"].parse(args, kwargs, __FUNCTION__, &item, &callback))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setCallback(callback);

		Py_RETURN_NONE;
	}

	PyObject* set_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popup;
		const char* item;

		if (!Parsers["set_item_popup"].parse(args, kwargs, __FUNCTION__, &item, &popup))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setPopup(popup);

		Py_RETURN_NONE;
	}

	PyObject* set_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tip, * item;

		if (!Parsers["set_item_tip"].parse(args, kwargs, __FUNCTION__, &item, &tip))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setTip(tip);

		Py_RETURN_NONE;
	}

	PyObject* set_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int width;

		if (!Parsers["set_item_width"].parse(args, kwargs, __FUNCTION__, &item, &width))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setWidth(width);


		Py_RETURN_NONE;
	}

	PyObject* set_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int height;

		if (!Parsers["set_item_height"].parse(args, kwargs, __FUNCTION__, &item, &height))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setHeight(height);


		Py_RETURN_NONE;
	}

	PyObject* set_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int width;
		int height;

		if (!Parsers["set_main_window_size"].parse(args, kwargs, __FUNCTION__, &width, &height))
			Py_RETURN_NONE;

		mvApp::GetApp()->setWindowSize(width, height);

		Py_RETURN_NONE;
	}

	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::GetApp()->isStarted())
			Py_RETURN_NONE;

		ImGui::CloseCurrentPopup();

		Py_RETURN_NONE;
	}

	PyObject* set_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* theme;

		if (!Parsers["set_theme"].parse(args, kwargs, __FUNCTION__, &theme))
			Py_RETURN_NONE;

		mvApp::GetApp()->setAppTheme(std::string(theme));

		Py_RETURN_NONE;
	}

	PyObject* show_metrics(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("metrics");
		Py_RETURN_NONE;
	}

	PyObject* show_about(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("about");
		Py_RETURN_NONE;
	}

	PyObject* show_source(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("source");
		Py_RETURN_NONE;
	}

	PyObject* show_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("debug");
		Py_RETURN_NONE;
	}

	PyObject* show_documentation(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("documentation");
		Py_RETURN_NONE;
	}

	mvPythonModule* CreateMarvelInterface()
	{

		auto pyModule = new mvPythonModule("marvel", {});

		pyModule->addMethodD(select_directory_dialog);
		pyModule->addMethodD(run_file);
		pyModule->addMethodD(add_table);
		pyModule->addMethodD(end_tree_node);
		pyModule->addMethodD(end_popup);
		pyModule->addMethodD(end_window);
		pyModule->addMethodD(end_group);
		pyModule->addMethodD(end_child);
		pyModule->addMethodD(end_tab);
		pyModule->addMethodD(end_tab_bar);
		pyModule->addMethodD(end_menu);
		pyModule->addMethodD(end_menu_bar);
		pyModule->addMethodD(end_tooltip);
		pyModule->addMethodD(end_collapsing_header);
		pyModule->addMethodD(add_image);
		pyModule->addMethodD(add_progress_bar);
		pyModule->addMethodD(add_drag_float);
		pyModule->addMethodD(add_drag_int);
		pyModule->addMethodD(add_drag_float2);
		pyModule->addMethodD(add_drag_float3);
		pyModule->addMethodD(add_drag_float4);
		pyModule->addMethodD(add_drag_int2);
		pyModule->addMethodD(add_drag_int3);
		pyModule->addMethodD(add_drag_int4);
		pyModule->addMethodD(add_slider_float);
		pyModule->addMethodD(add_slider_int);
		pyModule->addMethodD(add_slider_float2);
		pyModule->addMethodD(add_slider_float3);
		pyModule->addMethodD(add_slider_float4);
		pyModule->addMethodD(add_slider_int2);
		pyModule->addMethodD(add_slider_int3);
		pyModule->addMethodD(add_slider_int4);
		pyModule->addMethodD(add_tree_node);
		pyModule->addMethodD(add_selectable);
		pyModule->addMethodD(add_popup);
		pyModule->addMethodD(add_window);
		pyModule->addMethodD(add_indent);
		pyModule->addMethodD(unindent);
		pyModule->addMethodD(add_simple_plot);
		pyModule->addMethodD(add_combo);
		pyModule->addMethodD(add_text);
		pyModule->addMethodD(add_label_text);
		pyModule->addMethodD(add_listbox);
		pyModule->addMethodD(add_color_edit3);
		pyModule->addMethodD(add_color_edit4);
		pyModule->addMethodD(add_color_picker3);
		pyModule->addMethodD(add_color_picker4);
		pyModule->addMethodD(add_seperator);
		pyModule->addMethodD(add_button);
		pyModule->addMethodD(add_input_text);
		pyModule->addMethodD(add_input_int);
		pyModule->addMethodD(add_input_int2);
		pyModule->addMethodD(add_input_int3);
		pyModule->addMethodD(add_input_int4);
		pyModule->addMethodD(add_input_float);
		pyModule->addMethodD(add_input_float2);
		pyModule->addMethodD(add_input_float3);
		pyModule->addMethodD(add_input_float4);
		pyModule->addMethodD(add_radio_button);
		pyModule->addMethodD(add_checkbox);
		pyModule->addMethodD(add_group);
		pyModule->addMethodD(add_child);
		pyModule->addMethodD(add_tab_bar);
		pyModule->addMethodD(add_tab);
		pyModule->addMethodD(add_menu_bar);
		pyModule->addMethodD(add_menu);
		pyModule->addMethodD(add_menu_item);
		pyModule->addMethodD(add_spacing);
		pyModule->addMethodD(add_same_line);
		pyModule->addMethodD(add_tooltip);
		pyModule->addMethodD(add_collapsing_header);

		// drawing commands
		pyModule->addMethodD(draw_arrow);
		pyModule->addMethodD(get_drawing_origin);
		pyModule->addMethodD(get_drawing_scale);
		pyModule->addMethodD(set_drawing_origin);
		pyModule->addMethodD(set_drawing_scale);
		pyModule->addMethodD(get_drawing_size);
		pyModule->addMethodD(set_drawing_size);
		pyModule->addMethodD(add_drawing);
		pyModule->addMethodD(draw_image);
		pyModule->addMethodD(draw_line);
		pyModule->addMethodD(draw_triangle);
		pyModule->addMethodD(draw_rectangle);
		pyModule->addMethodD(draw_quad);
		pyModule->addMethodD(draw_text);
		pyModule->addMethodD(draw_circle);
		pyModule->addMethodD(draw_polyline);
		pyModule->addMethodD(draw_polygon);
		pyModule->addMethodD(draw_bezier_curve);
		pyModule->addMethodD(clear_drawing);

		// old app
		pyModule->addMethodD(add_column);
		pyModule->addMethodD(insert_column);
		pyModule->addMethodD(delete_column);
		pyModule->addMethodD(add_row);
		pyModule->addMethodD(insert_row);
		pyModule->addMethodD(delete_row);
		pyModule->addMethodD(get_table_item);
		pyModule->addMethodD(set_table_item);
		pyModule->addMethodD(set_table_selection);
		pyModule->addMethodD(get_table_selections);
		pyModule->addMethodD(get_delta_time);
		pyModule->addMethodD(get_total_time);
		pyModule->addMethodD(get_data);
		pyModule->addMethodD(delete_data);
		pyModule->addMethodD(add_data);
		pyModule->addMethodD(run_async_function);
		pyModule->addMethodD(save_file_dialog);
		pyModule->addMethodD(open_file_dialog);
		pyModule->addMethodD(delete_item);
		pyModule->addMethodD(move_item_down);
		pyModule->addMethodD(move_item_up);
		pyModule->addMethodD(get_style_item);
		pyModule->addMethodD(get_theme_item);
		pyModule->addMethodD(get_item_callback);
		pyModule->addMethodD(get_item_height);
		pyModule->addMethodD(get_item_popup);
		pyModule->addMethodD(get_item_tip);
		pyModule->addMethodD(get_main_callback);
		pyModule->addMethodD(get_main_window_size);
		pyModule->addMethodD(get_theme);
		pyModule->addMethodD(get_thread_count);
		pyModule->addMethodD(is_threadpool_high_performance);
		pyModule->addMethodD(get_threadpool_timeout);
		pyModule->addMethodD(get_active_window);
		pyModule->addMethodD(get_marvel_version);
		pyModule->addMethodD(show_source);
		pyModule->addMethodD(show_about);
		pyModule->addMethodD(show_debug);
		pyModule->addMethodD(show_metrics);
		pyModule->addMethodD(close_popup);
		pyModule->addMethodD(show_documentation);
		pyModule->addMethodD(set_threadpool_timeout);
		pyModule->addMethodD(set_thread_count);
		pyModule->addMethodD(set_threadpool_high_performance);
		pyModule->addMethodD(set_main_window_size);
		pyModule->addMethodD(add_item_color_style);
		pyModule->addMethodD(set_item_popup);
		pyModule->addMethodD(is_item_hovered);
		pyModule->addMethodD(is_item_active);
		pyModule->addMethodD(is_item_focused);
		pyModule->addMethodD(is_item_clicked);
		pyModule->addMethodD(is_item_visible);
		pyModule->addMethodD(is_item_edited);
		pyModule->addMethodD(is_item_activated);
		pyModule->addMethodD(is_item_deactivated);
		pyModule->addMethodD(is_item_deactivated_after_edit);
		pyModule->addMethodD(is_item_toggled_open);
		pyModule->addMethodD(get_item_rect_min);
		pyModule->addMethodD(get_item_rect_max);
		pyModule->addMethodD(get_item_rect_size);
		pyModule->addMethodD(change_style_item);
		pyModule->addMethodD(show_item);
		pyModule->addMethodD(hide_item);
		pyModule->addMethodD(change_theme_item);
		pyModule->addMethodD(set_theme);
		pyModule->addMethodD(set_main_callback);
		pyModule->addMethodD(set_item_callback);
		pyModule->addMethodD(set_item_tip);
		pyModule->addMethodD(set_item_width);
		pyModule->addMethodD(set_item_height);
		pyModule->addMethodD(get_value);
		pyModule->addMethodD(set_value);

		// old input
		pyModule->addMethodD(get_mouse_drag_callback);
		pyModule->addMethodD(set_mouse_drag_callback);
		pyModule->addMethodD(is_mouse_button_dragging);
		pyModule->addMethodD(is_mouse_button_down);
		pyModule->addMethodD(is_mouse_button_clicked);
		pyModule->addMethodD(is_mouse_button_double_clicked);
		pyModule->addMethodD(is_mouse_button_released);
		pyModule->addMethodD(get_mouse_drag_delta);
		pyModule->addMethodD(get_mouse_wheel_callback);
		pyModule->addMethodD(set_mouse_wheel_callback);
		pyModule->addMethodD(get_key_down_callback);
		pyModule->addMethodD(get_key_press_callback);
		pyModule->addMethodD(get_key_release_callback);
		pyModule->addMethodD(get_mouse_click_callback);
		pyModule->addMethodD(get_mouse_double_click_callback);
		pyModule->addMethodD(get_mouse_down_callback);
		pyModule->addMethodD(get_mouse_pos);
		pyModule->addMethodD(is_key_pressed);
		pyModule->addMethodD(is_key_released);
		pyModule->addMethodD(is_key_down);
		pyModule->addMethodD(set_mouse_click_callback);
		pyModule->addMethodD(set_mouse_down_callback);
		pyModule->addMethodD(set_mouse_double_click_callback);
		pyModule->addMethodD(set_key_down_callback);
		pyModule->addMethodD(set_key_press_callback);
		pyModule->addMethodD(set_key_release_callback);

		// old logging
		pyModule->addMethodD(get_log_level);
		pyModule->addMethodD(clear_log);
		pyModule->addMethodD(show_logger);
		pyModule->addMethodD(set_log_level);
		pyModule->addMethodD(log);
		pyModule->addMethodD(log_debug);
		pyModule->addMethodD(log_info);
		pyModule->addMethodD(log_warning);
		pyModule->addMethodD(log_error);

		// old plotting
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