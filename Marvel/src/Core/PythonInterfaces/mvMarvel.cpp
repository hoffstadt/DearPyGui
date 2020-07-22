#include "mvMarvel.h"
#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvInput.h"
#include "Core/mvDataStorage.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaceRegistry.h"
#include "Platform/Windows/mvWindowsWindow.h"

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define ADD_PYTHON_FUNCTION(Function) { #Function, (PyCFunction)Function, METH_VARARGS | METH_KEYWORDS, Parsers[#Function].getDocumentation() },

namespace Marvel {

	static std::map<std::string, mvPythonParser> Parsers = mvInterfaceRegistry::GetRegistry()->getPythonInterface("marvel");

	static void AddItemWithRuntimeChecks(mvAppItem* item, const char* parent, const char* before)
	{

		if (item == nullptr)
			return;

		auto ma = mvApp::GetApp();

		// typical run time adding
		if ((!std::string(parent).empty() || !std::string(before).empty()) && mvApp::IsAppStarted())
			ma->addRuntimeItem(parent, before, item);

		// adding without specifying before or parent, instead using parent stack
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted() && ma->topParent() != nullptr)
			ma->addRuntimeItem(ma->topParent()->getName(), before, item);

		// adding without specifying before or parent, but with empty stack (add to main window)
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted())
			ma->addRuntimeItem("MainWindow", "", item);

		// adding normally but using the runtime style of adding
		else if (!std::string(parent).empty() && !mvApp::IsAppStarted())
			ma->addRuntimeItem(parent, before, item);

		else if (std::string(parent).empty() && !mvApp::IsAppStarted() && std::string(before).empty() && item->getType()==mvAppItemType::Window)
			ma->addWindow(item);

		// typical adding before runtime
		else if (std::string(parent).empty() && !mvApp::IsAppStarted() && std::string(before).empty())
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

	PyObject* start_marvel(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvApp::GetApp()->precheck();
		mvApp::SetAppStarted();
		//mvApp::GetApp()->setModuleDict(PyModule_GetDict(self));
		//mvApp::GetApp()->setModuleDict(PyModule_GetDict(PyImport_AddModule("Demo"))); // borrowed reference
		mvApp::GetApp()->setModuleDict(PyModule_GetDict(PyImport_AddModule("__main__"))); // borrowed reference
		PyEval_InitThreads();

		// create window
		mvWindow* window = new mvWindowsWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight());
		window->show();
		window->run();
		delete window;
		delete mvApp::GetApp();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* generate_stub_file(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;

		if (!Parsers["generate_stub_file"].parse(args, kwargs, __FUNCTION__, &file))
			return mvPythonTranslator::GetPyNone();

		mvInterfaceRegistry::GetRegistry()->generateStubFile(file);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float scale;

		if (!Parsers["set_global_font_scale"].parse(args, kwargs, __FUNCTION__, &scale))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->setGlobalFontScale(scale);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyFloat(mvApp::GetApp()->getGlobalFontScale());
	}

	PyObject* run_file(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* file;
		const char* flags = "";

		if (!Parsers["run_file"].parse(args, kwargs, __FUNCTION__, &name, &file, &flags))
			return mvPythonTranslator::GetPyNone();

		RunFile(name, file, flags);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvDrawing("", name, width, height);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);

		if(!item)
			return mvPythonTranslator::GetPyNone();

		AddItemWithRuntimeChecks(item, parent, before);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_drawing_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width;
		int height;

		if (!Parsers["set_drawing_size"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
			return mvPythonTranslator::GetPyNone();

		auto drawing = mvApp::GetApp()->getItem(name);

		if (drawing)
		{
			drawing->setWidth(width);
			drawing->setHeight(height);
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_drawing_origin(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float x;
		float y;

		if (!Parsers["set_drawing_origin"].parse(args, kwargs, __FUNCTION__, &name, &x, &y))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(name);

		if (item == nullptr)
		{
			std::string message = name;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if(item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(name) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->setOrigin(x, y);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_drawing_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float x;
		float y;

		if (!Parsers["set_drawing_scale"].parse(args, kwargs, __FUNCTION__, &name, &x, &y))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(name);

		if (item == nullptr)
		{
			std::string message = name;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(name) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->setScale(x, y);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_drawing_origin(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["get_drawing_origin"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(name);

		if (item == nullptr)
		{
			std::string message = name;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(name) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}

		return mvPythonTranslator::ToPyPair(dwg->getOrigin().x, dwg->getOrigin().y);
	}

	PyObject* get_drawing_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["get_drawing_scale"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(name);

		if (item == nullptr)
		{
			std::string message = name;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(name) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}

		return mvPythonTranslator::ToPyPair(dwg->getScale().x, dwg->getScale().y);
	}

	PyObject* get_drawing_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["get_drawing_size"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		auto drawing = mvApp::GetApp()->getItem(name);

		if (drawing)
			return mvPythonTranslator::ToPyPair(drawing->getWidth(), drawing->getHeight());

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvVec2 mpmin = mvPythonTranslator::ToVec2(pmin);
		mvVec2 mpmax = mvPythonTranslator::ToVec2(pmax);
		mvVec2 muv_min = mvPythonTranslator::ToVec2(uv_min);
		mvVec2 muv_max = mvPythonTranslator::ToVec2(uv_max);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawImage(file, mpmin, mpmax, muv_min, muv_max, mcolor, tag);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* draw_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		int thickness;
		PyObject* p1, * p2;
		PyObject* color;
		const char* tag = "";

		if (!Parsers["draw_line"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &color, &thickness, &tag))
			return mvPythonTranslator::GetPyNone();

		mvVec2 mp1 = mvPythonTranslator::ToVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::ToVec2(p2);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawLine(mp1, mp2, mcolor, thickness, tag);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvVec2 mp1 = mvPythonTranslator::ToVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::ToVec2(p2);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawArrow(mp1, mp2, mcolor, thickness, size, tag);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();


		mvVec2 mp1 = mvPythonTranslator::ToVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::ToVec2(p2);
		mvVec2 mp3 = mvPythonTranslator::ToVec2(p3);
		mvColor mcolor = mvPythonTranslator::ToColor(color);
		mvColor mfill = mvPythonTranslator::ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawTriangle(mp1, mp2, mp3, mcolor, mfill, thickness, tag);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();


		mvVec2 mpmax = mvPythonTranslator::ToVec2(pmax);
		mvVec2 mpmin = mvPythonTranslator::ToVec2(pmin);
		mvColor mcolor = mvPythonTranslator::ToColor(color);
		mvColor mfill = mvPythonTranslator::ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawRectangle(mpmin, mpmax, mcolor, mfill, rounding, thickness, tag);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();


		mvVec2 mp1 = mvPythonTranslator::ToVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::ToVec2(p2);
		mvVec2 mp3 = mvPythonTranslator::ToVec2(p3);
		mvVec2 mp4 = mvPythonTranslator::ToVec2(p4);
		mvColor mcolor = mvPythonTranslator::ToColor(color);
		mvColor mfill = mvPythonTranslator::ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawQuad(mp1, mp2, mp3, mp4, mcolor, mfill, thickness, tag);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvVec2 mpos = mvPythonTranslator::ToVec2(pos);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawText(mpos, text, mcolor, size, tag);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvVec2 mcenter = mvPythonTranslator::ToVec2(center);
		mvColor mcolor = mvPythonTranslator::ToColor(color);
		mvColor mfill = mvPythonTranslator::ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawCircle(mcenter, radius, mcolor, segments, thickness, mfill, tag);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto mpoints = mvPythonTranslator::ToVectVec2(points);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawPolyline(mpoints, mcolor, closed, thickness, tag);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto mpoints = mvPythonTranslator::ToVectVec2(points);
		mvColor mcolor = mvPythonTranslator::ToColor(color);
		mvColor mfill = mvPythonTranslator::ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawPolygon(mpoints, mcolor, mfill, thickness, tag);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvVec2 mp1 = mvPythonTranslator::ToVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::ToVec2(p2);
		mvVec2 mp3 = mvPythonTranslator::ToVec2(p3);
		mvVec2 mp4 = mvPythonTranslator::ToVec2(p4);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawBezierCurve(mp1, mp2, mp3, mp4, mcolor, thickness, segments, tag);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* clear_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		if (!Parsers["clear_drawing"].parse(args, kwargs, __FUNCTION__, &drawing))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			mvAppLog::getLogger()->LogWarning(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			mvAppLog::getLogger()->LogWarning(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->clear();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* clear_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["clear_plot"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->clear();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_plot_xlimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["set_plot_xlimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setXLimitsAuto();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_plot_ylimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["set_plot_ylimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setYLimitsAuto();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_plot_xlimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float xmin;
		float xmax;

		if (!Parsers["set_plot_xlimits"].parse(args, kwargs, __FUNCTION__, &plot, &xmin, &xmax))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setXLimits(xmin, xmax);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_plot_ylimits(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;
		float ymin;
		float ymax;

		if (!Parsers["set_plot_ylimits"].parse(args, kwargs, __FUNCTION__, &plot, &ymin, &ymax))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setYLimits(ymin, ymax);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["is_plot_queried"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		return Py_BuildValue("b", graph->isPlotQueried());
	}

	PyObject* get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!Parsers["get_plot_query_area"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->SetColorMap(map);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvPlot("", name, xAxisName, yAxisName, width, height, flags, xflags, yflags, query_callback);
		item->setWidth(width);
		item->setHeight(height);

		AddItemWithRuntimeChecks(item, parent, before);

		return mvPythonTranslator::GetPyNone();
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

		PyObject* fill = PyTuple_New(4);
		PyTuple_SetItem(fill, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(fill, 1, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 2, PyLong_FromLong(0));
		PyTuple_SetItem(fill, 3, PyLong_FromLong(255));

		if (!Parsers["add_line_series"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &color, &fill, &weight))
			return mvPythonTranslator::GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " add line series requires a list of lists.");
			return mvPythonTranslator::GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = mvPythonTranslator::ToVectVec2(data);

		auto mcolor = mvPythonTranslator::ToColor(color);
		if (mcolor.r > 999)
			mcolor.specified = false;

		auto mfill = mvPythonTranslator::ToColor(fill);
		if (mfill.r > 999)
			mfill.specified = false;

		mvSeries* series = new mvLineSeries(name, datapoints, weight, mcolor, mfill);

		graph->addSeries(series);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " add scatter series requires a list of lists.");
			return mvPythonTranslator::GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvPlot* graph = static_cast<mvPlot*>(aplot);

		auto datapoints = mvPythonTranslator::ToVectVec2(data);

		auto mmarkerOutlineColor = mvPythonTranslator::ToColor(outline);
		if (mmarkerOutlineColor.r > 999)
			mmarkerOutlineColor.specified = false;

		auto mmarkerFillColor = mvPythonTranslator::ToColor(fill);
		if (mmarkerFillColor.r > 999)
			mmarkerFillColor.specified = false;

		mvSeries* series = new mvScatterSeries(name, datapoints, marker, size, weight, mmarkerOutlineColor,
			mmarkerFillColor);

		graph->addSeries(series);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			mvAppLog::getLogger()->LogWarning(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		mvSeries* series = new mvLabelSeries(name, { {(float)x, (float)y} }, xoffset, yoffset, vertical);

		graph->addSeries(series);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* show_logger(PyObject* self, PyObject* args)
	{
		mvApp::GetApp()->showStandardWindow("logger");
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyInt(mvAppLog::getLogger()->getLogLevel());
	}

	PyObject* set_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int level;
		if (!Parsers["set_log_level"].parse(args, kwargs, __FUNCTION__, &level))
			return mvPythonTranslator::GetPyNone();
		mvAppLog::getLogger()->setLogLevel(level);
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;
		const char* level = "TRACE";
		if (!Parsers["log"].parse(args, kwargs, __FUNCTION__, &message, &level))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::getLogger()->Log(std::string(message), std::string(level));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Parsers["log_debug"].parse(args, kwargs, __FUNCTION__, &message))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::getLogger()->LogDebug(std::string(message));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Parsers["log_info"].parse(args, kwargs, __FUNCTION__, &message))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::getLogger()->LogInfo(std::string(message));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log_warning(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Parsers["log_warning"].parse(args, kwargs, __FUNCTION__, &message))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::getLogger()->LogWarning(std::string(message));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log_error(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Parsers["log_error"].parse(args, kwargs, __FUNCTION__, &message))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::getLogger()->LogError(std::string(message));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* clear_log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppLog::getLogger()->ClearLog();
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int local = true;

		if (!Parsers["get_mouse_pos"].parse(args, kwargs, __FUNCTION__, &local))
			return mvPythonTranslator::GetPyNone();

		mvVec2 pos = mvInput::getMousePosition();
		if (!local)
			pos = mvInput::getGlobalMousePosition();
		PyObject* pvalue = mvPythonTranslator::ToPyPair(pos.x, pos.y);
		return pvalue;
	}

	PyObject* get_mouse_drag_delta(PyObject* self, PyObject* args)
	{
		mvVec2 pos = mvInput::getMouseDragDelta();
		PyObject* pvalue = mvPythonTranslator::ToPyPair(pos.x, pos.y);
		return pvalue;
	}

	PyObject* is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Parsers["is_key_pressed"].parse(args, kwargs, __FUNCTION__, &key))
			return mvPythonTranslator::GetPyNone();

		bool pressed = mvInput::isKeyPressed(key);

		PyObject* pvalue = mvPythonTranslator::ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Parsers["is_key_released"].parse(args, kwargs, __FUNCTION__, &key))
			return mvPythonTranslator::GetPyNone();

		bool pressed = mvInput::isKeyReleased(key);

		PyObject* pvalue = mvPythonTranslator::ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Parsers["is_key_down"].parse(args, kwargs, __FUNCTION__, &key))
			return mvPythonTranslator::GetPyNone();

		bool pressed = mvInput::isKeyDown(key);

		PyObject* pvalue = mvPythonTranslator::ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;
		float threshold;

		if (!Parsers["is_mouse_button_dragging"].parse(args, kwargs, __FUNCTION__, &button, &threshold))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseDragging(button, threshold));
	}

	PyObject* is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Parsers["is_mouse_button_down"].parse(args, kwargs, __FUNCTION__, &button))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseButtonDown(button));
	}

	PyObject* is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Parsers["is_mouse_button_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseButtonClicked(button));
	}

	PyObject* is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Parsers["is_mouse_button_double_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseButtonDoubleClicked(button));
	}

	PyObject* is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Parsers["is_mouse_button_released"].parse(args, kwargs, __FUNCTION__, &button))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseButtonReleased(button));
	}

	PyObject* set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!Parsers["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseDownCallback(std::string(callback));
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				auto windowtype = static_cast<mvWindowAppitem*>(item);
				static_cast<mvEventHandler*>(windowtype)->setMouseDownCallback(callback);
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_mouse_drag_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		float threshold;
		const char* handler = "MainWindow";

		if (!Parsers["set_mouse_drag_callback"].parse(args, kwargs, __FUNCTION__, &callback, &threshold, &handler))
			return mvPythonTranslator::GetPyNone();

		mvInput::setMouseDragThreshold(threshold);

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseDragCallback(std::string(callback));
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				auto windowtype = static_cast<mvWindowAppitem*>(item);
				static_cast<mvEventHandler*>(windowtype)->setMouseDragCallback(callback);
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler;

		if (!Parsers["set_mouse_double_click_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseDoubleClickCallback(std::string(callback));
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				auto windowtype = static_cast<mvWindowAppitem*>(item);
				static_cast<mvEventHandler*>(windowtype)->setMouseDoubleClickCallback(callback);
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_mouse_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!Parsers["set_mouse_click_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseClickCallback(std::string(callback));
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				auto windowtype = static_cast<mvWindowAppitem*>(item);
				static_cast<mvEventHandler*>(windowtype)->setMouseClickCallback(callback);
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_key_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!Parsers["set_key_down_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setKeyDownCallback(std::string(callback));
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				auto windowtype = static_cast<mvWindowAppitem*>(item);
				static_cast<mvEventHandler*>(windowtype)->setKeyDownCallback(callback);
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_key_press_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!Parsers["set_key_press_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setKeyPressCallback(std::string(callback));
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				auto windowtype = static_cast<mvWindowAppitem*>(item);
				static_cast<mvEventHandler*>(windowtype)->setKeyPressCallback(callback);
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_key_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!Parsers["set_key_release_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setKeyReleaseCallback(std::string(callback));
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				auto windowtype = static_cast<mvWindowAppitem*>(item);
				static_cast<mvEventHandler*>(windowtype)->setKeyReleaseCallback(callback);
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_mouse_wheel_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!Parsers["set_mouse_wheel_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseWheelCallback(std::string(callback));
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				auto windowtype = static_cast<mvWindowAppitem*>(item);
				static_cast<mvEventHandler*>(windowtype)->setMouseWheelCallback(callback);
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* headers;
		const char* callback = "";
		const char* parent = "";
		const char* before = "";

		if (!Parsers["add_table"].parse(args, kwargs, __FUNCTION__, &name, &headers, &callback, &parent, &before))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvTable("", name, mvPythonTranslator::ToStringVect(headers));
		item->setCallback(callback);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		std::vector<float> values = mvPythonTranslator::ToFloatVect(value);

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

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvProgressBar("", name, default_value, overlay);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		item->setDataSource(data_source);

		AddItemWithRuntimeChecks(item, parent, before);

		return mvPythonTranslator::GetPyNone();
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
		PyObject* uv_min = PyTuple_New(2);
		PyTuple_SetItem(uv_min, 0, PyFloat_FromDouble(0));
		PyTuple_SetItem(uv_min, 1, PyFloat_FromDouble(0));
		PyObject* uv_max = PyTuple_New(2);
		PyTuple_SetItem(uv_max, 0, PyFloat_FromDouble(1));
		PyTuple_SetItem(uv_max, 1, PyFloat_FromDouble(1));
		const char* secondary_data_source = "";

		if (!Parsers["add_image"].parse(args, kwargs, __FUNCTION__, &name, 
			&value, &tintcolor, &bordercolor, &tip, &parent, &before, &data_source, &width, 
			&height, &uv_min, &uv_max, &secondary_data_source))
			return mvPythonTranslator::GetPyNone();

		auto mtintcolor = mvPythonTranslator::ToColor(tintcolor);
		auto mbordercolor = mvPythonTranslator::ToColor(bordercolor);
		mvVec2 muv_min = mvPythonTranslator::ToVec2(uv_min);
		mvVec2 muv_max = mvPythonTranslator::ToVec2(uv_max);

		mvAppItem* item = new mvImage("", name, value, mtintcolor, mbordercolor, muv_min, muv_max,
			secondary_data_source);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		item->setDataSource(data_source);

		AddItemWithRuntimeChecks(item, parent, before);

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvDragFloat("", name, default_value, speed, min_value, max_value, format, power);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToFloatVect(default_value);

		mvAppItem* item = new mvDragFloat2("", name, vec.data(), speed, min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToFloatVect(default_value);

		mvAppItem* item = new mvDragFloat3("", name, vec.data(), speed, min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToFloatVect(default_value);

		mvAppItem* item = new mvDragFloat4("", name, vec.data(), speed, min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvDragInt("", name, default_value, speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToIntVect(default_value);

		mvAppItem* item = new mvDragInt2("", name, vec.data(), speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToIntVect(default_value);

		mvAppItem* item = new mvDragInt3("", name, vec.data(), speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToIntVect(default_value);

		mvAppItem* item = new mvDragInt4("", name, vec.data(), speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvSliderFloat("", name, default_value, min_value, max_value, format, power, vertical);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToFloatVect(default_value);

		mvAppItem* item = new mvSliderFloat2("", name, vec.data(), min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToFloatVect(default_value);

		mvAppItem* item = new mvSliderFloat3("", name, vec.data(), min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToFloatVect(default_value);

		mvAppItem* item = new mvSliderFloat4("", name, vec.data(), min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvSliderInt("", name, default_value, min_value, max_value, format, vertical);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToIntVect(default_value);

		mvAppItem* item = new mvSliderInt2("", name, vec.data(), min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToIntVect(default_value);

		mvAppItem* item = new mvSliderInt3("", name, vec.data(), min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToIntVect(default_value);

		mvAppItem* item = new mvSliderInt4("", name, vec.data(), min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto mcolor = mvPythonTranslator::ToColor(color);

		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvText("", name, wrap, mcolor, bullet);
		item->setTip(tip);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto mcolor = mvPythonTranslator::ToColor(color);
		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvLabelText("", std::string(name), value, mcolor);
		item->setTip(tip); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
		const char* secondary_data_source = "";

		if (!Parsers["add_listbox"].parse(args, kwargs,__FUNCTION__, &name, &items, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width, 
			&height, &secondary_data_source))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvListbox("", name, mvPythonTranslator::ToStringVect(items), 
			default_value, height, secondary_data_source);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width); 
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
		const char* secondary_data_source = "";

		if (!Parsers["add_combo"].parse(args, kwargs,__FUNCTION__, &name, &items, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width
			,&secondary_data_source))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvCombo("", name, mvPythonTranslator::ToStringVect(items), default_value, secondary_data_source);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvSelectable("", name, default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int smallb = false;
		int arrow = false;
		int direction = -1;
		const char* callback = "";
		const char* tip = ""; 
		int width = 0; 
		int height = 0; 
		const char* before = ""; 
		const char* parent = ""; 

		if (!Parsers["add_button"].parse(args, kwargs,__FUNCTION__, &name, &smallb,
			&arrow, &direction, &callback, &tip, &parent, &before, &width, &height))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvButton("", name, smallb, arrow, direction);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

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
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvInputInt("", name, default_value);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToIntVect(default_value);

		mvAppItem* item = new mvInputInt2("", name, vec.data());
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToIntVect(default_value);

		mvAppItem* item = new mvInputInt3("", name, vec.data());
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
			
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToIntVect(default_value);

		mvAppItem* item = new mvInputInt4("", name, vec.data());
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
				
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvInputFloat("", name, default_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToFloatVect(default_value);

		mvAppItem* item = new mvInputFloat2("", name, vec.data(), format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToFloatVect(default_value);

		mvAppItem* item = new mvInputFloat3("", name, vec.data(), format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto vec = mvPythonTranslator::ToFloatVect(default_value);

		mvAppItem* item = new mvInputFloat4("", name, vec.data(), format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvIndent("", name, offset);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvUnindent("", name, offset);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvTabBar("", name, reorderable);
		item->setCallback(callback); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();
		
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

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* end_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		return mvPythonTranslator::GetPyNone();

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_tab must follow a call to add_tab.");

		else if (parentItem->getType() == mvAppItemType::TabItem)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_tab was called incorrectly and will be ignored");

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* end_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		return mvPythonTranslator::GetPyNone();

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

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_menu_bar"].parse(args, kwargs, __FUNCTION__, &name,
			&parent, &before))
			return mvPythonTranslator::GetPyNone();

		auto parentItem = mvApp::GetApp()->topParent();
		
		if (parentItem->getType() == mvAppItemType::Window)
		{
			auto window = static_cast<mvWindowAppitem*>(parentItem);
			window->addFlag(ImGuiWindowFlags_MenuBar);
			mvAppItem* item = new mvMenuBar(name);
			AddItemWithRuntimeChecks(item, parent, before);
			mvApp::GetApp()->pushParent(item);
		}
		
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_menu"].parse(args, kwargs, __FUNCTION__, &name, 
			&tip, &parent, &before))
			return mvPythonTranslator::GetPyNone();

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

		return mvPythonTranslator::GetPyNone();
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

		return mvPythonTranslator::GetPyNone();
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

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvMenuItem("", name);
		item->setCallback(callback); 
		item->setTip(tip); 
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvSpacing("", name, count);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvSameLine("", name, xoffset, spacing);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
		const char* secondary_data_source = "";

		if (!Parsers["add_radio_button"].parse(args, kwargs,__FUNCTION__, &name, &items, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &secondary_data_source))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvRadioButton("", name, mvPythonTranslator::ToStringVect(items), default_value,
			secondary_data_source);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvGroup("", name);
		item->setTip(tip);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		return mvPythonTranslator::GetPyNone();
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

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		int border = true;

		if (!Parsers["add_child"].parse(args, kwargs, __FUNCTION__, &name, 
			&tip, &parent, &before, &width, &height, &border))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvChild("", name, border);
		item->setTip(tip); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width = -1;
		int height = -1;
		int startx = 200;
		int starty = 200;
		int autosize = false;
		int hide = false;
		int resizable = true;
		int title_bar = true;
		int movable = true;
		const char* closing_callback = "";

		if (!Parsers["add_window"].parse(args, kwargs, __FUNCTION__, &name, &width, 
			&height, &startx, &starty, &autosize, &resizable, &title_bar, &movable,
			&hide, &closing_callback))
			return mvPythonTranslator::GetPyNone();

		if (width == -1 && height == -1)
		{
			width = 500;
			height = 500;
		}

		mvAppItem* item = new mvWindowAppitem("", name, width, height, startx, starty, 
			false, autosize, resizable, title_bar, movable, closing_callback);
		AddItemWithRuntimeChecks(item, "", "");
		mvApp::GetApp()->pushParent(item);

		if (hide)
			item->hide();

		return mvPythonTranslator::GetPyNone();
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

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_window_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* window;
		float x;
		float y;

		if (!Parsers["set_window_pos"].parse(args, kwargs, __FUNCTION__, &window,&x, &y))
			return mvPythonTranslator::GetPyNone();

		auto awindow = mvApp::GetApp()->getWindow(window);

		if (awindow == nullptr)
		{
			mvAppLog::getLogger()->LogError(window + std::string(" window was not found"));
			return mvPythonTranslator::GetPyNone();
		}

		awindow->setWindowPos(x, y);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_window_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* window;

		if (!Parsers["get_window_pos"].parse(args, kwargs, __FUNCTION__, &window))
			return mvPythonTranslator::GetPyNone();

		auto awindow = mvApp::GetApp()->getWindow(window);

		if (awindow == nullptr)
		{
			mvAppLog::getLogger()->LogError(window + std::string(" window was not found"));
			return mvPythonTranslator::GetPyNone();
		}

		mvVec2 pos = awindow->getWindowPos();

		return mvPythonTranslator::ToPyPair(pos.x, pos.y);
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

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tipparent;
		const char*name;
		const char* parent = ""; 
		const char* before = ""; 

		if (!Parsers["add_tooltip"].parse(args, kwargs, __FUNCTION__, &tipparent, 
			&name, &parent, &before))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvTooltip(tipparent, name);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		return mvPythonTranslator::GetPyNone();
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

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

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
		return mvPythonTranslator::GetPyNone();
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

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_open = false;
		int closable = false;
		int flags = 0;
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_open, &closable, &tip, &parent, &before))
			return mvPythonTranslator::GetPyNone();

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvCollapsingHeader("", name, flags, closable);
		item->setTip(tip);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		return mvPythonTranslator::GetPyNone();
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

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvTreeNode("", name, flags);
		item->setTip(tip);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		return mvPythonTranslator::GetPyNone();
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

		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvSeparator("", name);
		item->setTip(tip);

		AddItemWithRuntimeChecks(item, parent, before);
		
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto color = mvPythonTranslator::ToColor(default_value);

		mvAppItem* item = new mvColorEdit3("", name, color);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto color = mvPythonTranslator::ToColor(default_value);

		mvAppItem* item = new mvColorEdit4("", name, color);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto color = mvPythonTranslator::ToColor(default_value);

		mvAppItem* item = new mvColorPicker3("", name, color);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		auto color = mvPythonTranslator::ToColor(default_value);

		mvAppItem* item = new mvColorPicker4("", name, color);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvCheckbox("", name, default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		const char* value;

		if (!Parsers["set_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setTableItem(row, column, value);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* delete_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;

		if (!Parsers["delete_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteRow(row);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* delete_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column;

		if (!Parsers["delete_column"].parse(args, kwargs, __FUNCTION__, &table, &column))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteColumn(column);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		PyObject* row;

		if (!Parsers["add_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		auto prow = mvPythonTranslator::ToStringVect(row);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->addRow(prow);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		const char* name;
		PyObject* column;

		if (!Parsers["add_column"].parse(args, kwargs, __FUNCTION__, &table, &name, &column))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		auto pcolumn = mvPythonTranslator::ToStringVect(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->addColumn(name, pcolumn);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* insert_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row_index;
		PyObject* row;

		if (!Parsers["insert_row"].parse(args, kwargs, __FUNCTION__, &table, &row_index, &row))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		auto prow = mvPythonTranslator::ToStringVect(row);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->insertRow(row_index, prow);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* insert_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column_index;
		const char* name;
		PyObject* column;

		if (!Parsers["insert_column"].parse(args, kwargs, __FUNCTION__, &table, &column_index, &name, &column))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		auto prow = mvPythonTranslator::ToStringVect(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->insertColumn(column_index, name, prow);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_table_selection(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		int value;

		if (!Parsers["set_table_selection"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setSelection(row, column, value);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* clear_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!Parsers["clear_table"].parse(args, kwargs, __FUNCTION__, &table))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->clearTable();
		Py_RETURN_NONE;
	}

	PyObject* get_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;

		if (!Parsers["get_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return Py_BuildValue("s", atable->getTableItem(row, column).c_str());

	}

	PyObject* get_table_selections(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!Parsers["get_table_selections"].parse(args, kwargs, __FUNCTION__, &table))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
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
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->addMTCallback(name, data, return_handler);

		return mvPythonTranslator::GetPyNone();

	}

	PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			return mvPythonTranslator::GetPyNone();
		}

		const char* item;
		int childrenOnly = false;

		if (!Parsers["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			return mvPythonTranslator::GetPyNone();

		if (childrenOnly)
			mvApp::GetApp()->deleteItemChildren(item);
		else
			mvApp::GetApp()->deleteItem(item);

		return mvPythonTranslator::GetPyNone();

	}

	PyObject* select_directory_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::string file = PickDirectory("");
		return mvPythonTranslator::ToPyString(file);
	}

	PyObject* open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* extensions;

		if (!Parsers["open_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			return mvPythonTranslator::GetPyNone();

		std::string file = OpenFile(mvPythonTranslator::ToVectPairString(extensions));

		return mvPythonTranslator::ToPyString(file.c_str());
	}

	PyObject* save_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* extensions;

		if (!Parsers["save_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			return mvPythonTranslator::GetPyNone();

		std::string file = SaveFile(mvPythonTranslator::ToVectPairString(extensions));

		return mvPythonTranslator::ToPyString(SaveFile(mvPythonTranslator::ToVectPairString(extensions)).c_str());
	}

	PyObject* move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			return mvPythonTranslator::GetPyNone();
		}

		const char* item;

		if (!Parsers["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->moveItemUp(item);

		return mvPythonTranslator::GetPyNone();

	}

	PyObject* move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			return mvPythonTranslator::GetPyNone();
		}

		const char* item;

		if (!Parsers["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->moveItemDown(item);

		return mvPythonTranslator::GetPyNone();

	}

	PyObject* get_style_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!Parsers["get_style_item"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto values = mvApp::GetApp()->getStyleItem(item);

		return mvPythonTranslator::ToPyPair(values.first, values.second);
	}

	PyObject* get_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!Parsers["get_theme_item"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto color = mvApp::GetApp()->getThemeItem(item);

		return mvPythonTranslator::ToPyList(std::vector<int>{ color.r, color.g, color.b, color.a });
	}

	PyObject* get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyFloat(mvApp::GetApp()->getTotalTime());
	}

	PyObject* get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyFloat(mvApp::GetApp()->getDeltaTime());
	}

	PyObject* get_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyString(appitem->getCallback());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_height"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyInt(appitem->getHeight());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_width"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyInt(appitem->getWidth());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_popup"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyString(appitem->getPopup());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_tip"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyString(appitem->getTip());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyPair(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight());
	}

	PyObject* get_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyString(mvApp::GetApp()->getAppTheme());
	}

	PyObject* get_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyInt(mvApp::GetApp()->getThreadCount());
	}

	PyObject* is_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyBool(mvApp::GetApp()->usingThreadPoolHighPerformance());
	}

	PyObject* get_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyFloat(mvApp::GetApp()->getThreadPoolTimeout());
	}

	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyString(mvApp::GetApp()->getActiveWindow());
	}

	PyObject* get_marvel_version(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyString(mvApp::GetApp()->GetVersion());
	}

	PyObject* set_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float time;

		if (!Parsers["set_threadpool_timeout"].parse(args, kwargs, __FUNCTION__, &time))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->setThreadPoolTimeout(time);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		int threads;

		if (!Parsers["set_thread_count"].parse(args, kwargs, __FUNCTION__, &threads))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->setThreadCount(threads);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		PyObject* data;

		if (!Parsers["add_data"].parse(args, kwargs, __FUNCTION__, &name, &data))
			return mvPythonTranslator::GetPyNone();

		mvDataStorage::AddData(name, data);
		Py_XINCREF(data);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!Parsers["get_data"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		auto result = mvDataStorage::GetData(name);

		if (result)
			return result;

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* delete_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!Parsers["delete_data"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		mvDataStorage::DeleteData(name);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->setThreadPoolHighPerformance();
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_item_color_style(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int style;
		PyObject* color;

		if (!Parsers["add_item_color_style"].parse(args, kwargs, __FUNCTION__, &item, &style, &color))
			return mvPythonTranslator::GetPyNone();

		auto mcolor = mvPythonTranslator::ToColor(color);

		mvApp::GetApp()->addItemColorStyle(item, style, mcolor);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_hovered"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemHovered());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_active"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemActive());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_focused"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemFocused());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_clicked"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemClicked());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_visible"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemVisible());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_edited"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemEdited());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_activated"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemActivated());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_deactivated"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemDeactivated());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_deactivated_after_edit"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemDeactivatedAfterEdit());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_toggled_open"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemToogledOpen());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_rect_min"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMin();
			return mvPythonTranslator::ToPyPair(value.x, value.y);
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_rect_max"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMax();
			return mvPythonTranslator::ToPyPair(value.x, value.y);
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_rect_size"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectSize();
			return mvPythonTranslator::ToPyPair(value.x, value.y);
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* change_style_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		float x = 0.0f;
		float y = 0.0f;

		if (!Parsers["change_style_item"].parse(args, kwargs, __FUNCTION__, &item, &x, &y))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->changeStyleItem(item, x, y);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* change_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		int r, g, b, a;

		if (!Parsers["change_theme_item"].parse(args, kwargs, __FUNCTION__, &item, &r, &g, &b, &a))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->changeThemeItem(item, { r, g, b, a });



		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["get_value"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			return mvPythonTranslator::GetPyNone();

		return item->getPyValue();
	}

	PyObject* set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if (!Parsers["set_value"].parse(args, kwargs, __FUNCTION__, &name, &value))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			return mvPythonTranslator::GetPyNone();

		item->setPyValue(value);
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* show_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["show_item"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->show();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* hide_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Parsers["hide_item"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->hide();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_render_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!Parsers["set_render_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if(std::string(handler) == "MainWindow")
			mvApp::GetApp()->setRenderCallback(std::string(callback));
		else
		{

			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(windowtype);
					eventhandler->setRenderCallback(callback);
				}
				else if (item->getType() == mvAppItemType::Child)
				{
					auto childType = static_cast<mvChild*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(childType);
					eventhandler->setRenderCallback(callback);
				}
				else if (item->getType() == mvAppItemType::Popup)
				{
					auto childType = static_cast<mvPopup*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(childType);
					eventhandler->setRenderCallback(callback);
				}
				else if (item->getType() == mvAppItemType::Menu)
				{
					auto childType = static_cast<mvMenu*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(childType);
					eventhandler->setRenderCallback(callback);
				}
				else
					mvAppLog::getLogger()->LogWarning("Render callback can only be set for window/child items");
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!Parsers["set_resize_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setResizeCallback(std::string(callback));
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);

			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(windowtype);
					eventhandler->setResizeCallback(callback);
				}
				else if (item->getType() == mvAppItemType::Child)
				{
					auto childType = static_cast<mvChild*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(childType);
					eventhandler->setResizeCallback(callback);
				}
				else if (item->getType() == mvAppItemType::Popup)
				{
					auto childType = static_cast<mvPopup*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(childType);
					eventhandler->setResizeCallback(callback);
				}
				else if (item->getType() == mvAppItemType::Menu)
				{
					auto childType = static_cast<mvMenu*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(childType);
					eventhandler->setResizeCallback(callback);
				}
				else
					mvAppLog::getLogger()->LogWarning("Resize callback can only be set for window/child items");
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* item;

		if (!Parsers["set_item_callback"].parse(args, kwargs, __FUNCTION__, &item, &callback))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* appitem;
		appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setCallback(callback);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popup;
		const char* item;

		if (!Parsers["set_item_popup"].parse(args, kwargs, __FUNCTION__, &item, &popup))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setPopup(popup);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tip, * item;

		if (!Parsers["set_item_tip"].parse(args, kwargs, __FUNCTION__, &item, &tip))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setTip(tip);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int width;

		if (!Parsers["set_item_width"].parse(args, kwargs, __FUNCTION__, &item, &width))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setWidth(width);


		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int height;

		if (!Parsers["set_item_height"].parse(args, kwargs, __FUNCTION__, &item, &height))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setHeight(height);


		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int width;
		int height;

		if (!Parsers["set_main_window_size"].parse(args, kwargs, __FUNCTION__, &width, &height))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->setActualSize(width, height);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::IsAppStarted())
			return mvPythonTranslator::GetPyNone();

		ImGui::CloseCurrentPopup();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* theme;

		if (!Parsers["set_theme"].parse(args, kwargs, __FUNCTION__, &theme))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->setAppTheme(std::string(theme));

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* show_metrics(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("metrics");
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* show_about(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("about");
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* show_source(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("source");
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* show_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("debug");
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* show_documentation(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("documentation");
		return mvPythonTranslator::GetPyNone();
	}

	static PyMethodDef marvelmethods[]
	{
		ADD_PYTHON_FUNCTION(start_marvel)
		ADD_PYTHON_FUNCTION(clear_table)
		ADD_PYTHON_FUNCTION(generate_stub_file)
		ADD_PYTHON_FUNCTION(get_window_pos)
		ADD_PYTHON_FUNCTION(set_window_pos)
		ADD_PYTHON_FUNCTION(get_global_font_scale)
		ADD_PYTHON_FUNCTION(set_global_font_scale)
		ADD_PYTHON_FUNCTION(select_directory_dialog)
		ADD_PYTHON_FUNCTION(run_file)
		ADD_PYTHON_FUNCTION(add_table)
		ADD_PYTHON_FUNCTION(end_tree_node)
		ADD_PYTHON_FUNCTION(end_popup)
		ADD_PYTHON_FUNCTION(end_window)
		ADD_PYTHON_FUNCTION(end_group)
		ADD_PYTHON_FUNCTION(end_child)
		ADD_PYTHON_FUNCTION(end_tab)
		ADD_PYTHON_FUNCTION(end_tab_bar)
		ADD_PYTHON_FUNCTION(end_menu)
		ADD_PYTHON_FUNCTION(end_menu_bar)
		ADD_PYTHON_FUNCTION(end_tooltip)
		ADD_PYTHON_FUNCTION(end_collapsing_header)
		ADD_PYTHON_FUNCTION(add_image)
		ADD_PYTHON_FUNCTION(add_progress_bar)
		ADD_PYTHON_FUNCTION(add_drag_float)
		ADD_PYTHON_FUNCTION(add_drag_int)
		ADD_PYTHON_FUNCTION(add_drag_float2)
		ADD_PYTHON_FUNCTION(add_drag_float3)
		ADD_PYTHON_FUNCTION(add_drag_float4)
		ADD_PYTHON_FUNCTION(add_drag_int2)
		ADD_PYTHON_FUNCTION(add_drag_int3)
		ADD_PYTHON_FUNCTION(add_drag_int4)
		ADD_PYTHON_FUNCTION(add_slider_float)
		ADD_PYTHON_FUNCTION(add_slider_int)
		ADD_PYTHON_FUNCTION(add_slider_float2)
		ADD_PYTHON_FUNCTION(add_slider_float3)
		ADD_PYTHON_FUNCTION(add_slider_float4)
		ADD_PYTHON_FUNCTION(add_slider_int2)
		ADD_PYTHON_FUNCTION(add_slider_int3)
		ADD_PYTHON_FUNCTION(add_slider_int4)
		ADD_PYTHON_FUNCTION(add_tree_node)
		ADD_PYTHON_FUNCTION(add_selectable)
		ADD_PYTHON_FUNCTION(add_popup)
		ADD_PYTHON_FUNCTION(add_window)
		ADD_PYTHON_FUNCTION(add_indent)
		ADD_PYTHON_FUNCTION(unindent)
		ADD_PYTHON_FUNCTION(add_simple_plot)
		ADD_PYTHON_FUNCTION(add_combo)
		ADD_PYTHON_FUNCTION(add_text)
		ADD_PYTHON_FUNCTION(add_label_text)
		ADD_PYTHON_FUNCTION(add_listbox)
		ADD_PYTHON_FUNCTION(add_color_edit3)
		ADD_PYTHON_FUNCTION(add_color_edit4)
		ADD_PYTHON_FUNCTION(add_color_picker3)
		ADD_PYTHON_FUNCTION(add_color_picker4)
		ADD_PYTHON_FUNCTION(add_seperator)
		ADD_PYTHON_FUNCTION(add_button)
		ADD_PYTHON_FUNCTION(add_input_text)
		ADD_PYTHON_FUNCTION(add_input_int)
		ADD_PYTHON_FUNCTION(add_input_int2)
		ADD_PYTHON_FUNCTION(add_input_int3)
		ADD_PYTHON_FUNCTION(add_input_int4)
		ADD_PYTHON_FUNCTION(add_input_float)
		ADD_PYTHON_FUNCTION(add_input_float2)
		ADD_PYTHON_FUNCTION(add_input_float3)
		ADD_PYTHON_FUNCTION(add_input_float4)
		ADD_PYTHON_FUNCTION(add_radio_button)
		ADD_PYTHON_FUNCTION(add_checkbox)
		ADD_PYTHON_FUNCTION(add_group)
		ADD_PYTHON_FUNCTION(add_child)
		ADD_PYTHON_FUNCTION(add_tab_bar)
		ADD_PYTHON_FUNCTION(add_tab)
		ADD_PYTHON_FUNCTION(add_menu_bar)
		ADD_PYTHON_FUNCTION(add_menu)
		ADD_PYTHON_FUNCTION(add_menu_item)
		ADD_PYTHON_FUNCTION(add_spacing)
		ADD_PYTHON_FUNCTION(add_same_line)
		ADD_PYTHON_FUNCTION(add_tooltip)
		ADD_PYTHON_FUNCTION(add_collapsing_header)
		ADD_PYTHON_FUNCTION(draw_arrow)
		ADD_PYTHON_FUNCTION(get_drawing_origin)
		ADD_PYTHON_FUNCTION(get_drawing_scale)
		ADD_PYTHON_FUNCTION(set_drawing_origin)
		ADD_PYTHON_FUNCTION(set_drawing_scale)
		ADD_PYTHON_FUNCTION(get_drawing_size)
		ADD_PYTHON_FUNCTION(set_drawing_size)
		ADD_PYTHON_FUNCTION(add_drawing)
		ADD_PYTHON_FUNCTION(draw_image)
		ADD_PYTHON_FUNCTION(draw_line)
		ADD_PYTHON_FUNCTION(draw_triangle)
		ADD_PYTHON_FUNCTION(draw_rectangle)
		ADD_PYTHON_FUNCTION(draw_quad)
		ADD_PYTHON_FUNCTION(draw_text)
		ADD_PYTHON_FUNCTION(draw_circle)
		ADD_PYTHON_FUNCTION(draw_polyline)
		ADD_PYTHON_FUNCTION(draw_polygon)
		ADD_PYTHON_FUNCTION(draw_bezier_curve)
		ADD_PYTHON_FUNCTION(clear_drawing)
		ADD_PYTHON_FUNCTION(add_column)
		ADD_PYTHON_FUNCTION(insert_column)
		ADD_PYTHON_FUNCTION(delete_column)
		ADD_PYTHON_FUNCTION(add_row)
		ADD_PYTHON_FUNCTION(insert_row)
		ADD_PYTHON_FUNCTION(delete_row)
		ADD_PYTHON_FUNCTION(get_table_item)
		ADD_PYTHON_FUNCTION(set_table_item)
		ADD_PYTHON_FUNCTION(set_table_selection)
		ADD_PYTHON_FUNCTION(get_table_selections)
		ADD_PYTHON_FUNCTION(get_delta_time)
		ADD_PYTHON_FUNCTION(get_total_time)
		ADD_PYTHON_FUNCTION(get_data)
		ADD_PYTHON_FUNCTION(delete_data)
		ADD_PYTHON_FUNCTION(add_data)
		ADD_PYTHON_FUNCTION(run_async_function)
		ADD_PYTHON_FUNCTION(save_file_dialog)
		ADD_PYTHON_FUNCTION(open_file_dialog)
		ADD_PYTHON_FUNCTION(delete_item)
		ADD_PYTHON_FUNCTION(move_item_down)
		ADD_PYTHON_FUNCTION(move_item_up)
		ADD_PYTHON_FUNCTION(get_style_item)
		ADD_PYTHON_FUNCTION(get_theme_item)
		ADD_PYTHON_FUNCTION(get_item_callback)
		ADD_PYTHON_FUNCTION(get_item_width)
		ADD_PYTHON_FUNCTION(get_item_height)
		ADD_PYTHON_FUNCTION(get_item_popup)
		ADD_PYTHON_FUNCTION(get_item_tip)
		ADD_PYTHON_FUNCTION(get_main_window_size)
		ADD_PYTHON_FUNCTION(get_theme)
		ADD_PYTHON_FUNCTION(get_thread_count)
		ADD_PYTHON_FUNCTION(is_threadpool_high_performance)
		ADD_PYTHON_FUNCTION(get_threadpool_timeout)
		ADD_PYTHON_FUNCTION(get_active_window)
		ADD_PYTHON_FUNCTION(get_marvel_version)
		ADD_PYTHON_FUNCTION(show_source)
		ADD_PYTHON_FUNCTION(show_about)
		ADD_PYTHON_FUNCTION(show_debug)
		ADD_PYTHON_FUNCTION(show_metrics)
		ADD_PYTHON_FUNCTION(close_popup)
		ADD_PYTHON_FUNCTION(show_documentation)
		ADD_PYTHON_FUNCTION(set_threadpool_timeout)
		ADD_PYTHON_FUNCTION(set_thread_count)
		ADD_PYTHON_FUNCTION(set_threadpool_high_performance)
		ADD_PYTHON_FUNCTION(set_main_window_size)
		ADD_PYTHON_FUNCTION(add_item_color_style)
		ADD_PYTHON_FUNCTION(set_item_popup)
		ADD_PYTHON_FUNCTION(is_item_hovered)
		ADD_PYTHON_FUNCTION(is_item_active)
		ADD_PYTHON_FUNCTION(is_item_focused)
		ADD_PYTHON_FUNCTION(is_item_clicked)
		ADD_PYTHON_FUNCTION(is_item_visible)
		ADD_PYTHON_FUNCTION(is_item_edited)
		ADD_PYTHON_FUNCTION(is_item_activated)
		ADD_PYTHON_FUNCTION(is_item_deactivated)
		ADD_PYTHON_FUNCTION(is_item_deactivated_after_edit)
		ADD_PYTHON_FUNCTION(is_item_toggled_open)
		ADD_PYTHON_FUNCTION(get_item_rect_min)
		ADD_PYTHON_FUNCTION(get_item_rect_max)
		ADD_PYTHON_FUNCTION(get_item_rect_size)
		ADD_PYTHON_FUNCTION(change_style_item)
		ADD_PYTHON_FUNCTION(show_item)
		ADD_PYTHON_FUNCTION(hide_item)
		ADD_PYTHON_FUNCTION(change_theme_item)
		ADD_PYTHON_FUNCTION(set_theme)
		ADD_PYTHON_FUNCTION(set_render_callback)
		ADD_PYTHON_FUNCTION(set_item_callback)
		ADD_PYTHON_FUNCTION(set_item_tip)
		ADD_PYTHON_FUNCTION(set_item_width)
		ADD_PYTHON_FUNCTION(set_item_height)
		ADD_PYTHON_FUNCTION(get_value)
		ADD_PYTHON_FUNCTION(set_value)
		ADD_PYTHON_FUNCTION(set_mouse_drag_callback)
		ADD_PYTHON_FUNCTION(is_mouse_button_dragging)
		ADD_PYTHON_FUNCTION(is_mouse_button_down)
		ADD_PYTHON_FUNCTION(is_mouse_button_clicked)
		ADD_PYTHON_FUNCTION(is_mouse_button_double_clicked)
		ADD_PYTHON_FUNCTION(is_mouse_button_released)
		ADD_PYTHON_FUNCTION(get_mouse_drag_delta)
		ADD_PYTHON_FUNCTION(set_mouse_wheel_callback)
		ADD_PYTHON_FUNCTION(get_mouse_pos)
		ADD_PYTHON_FUNCTION(is_key_pressed)
		ADD_PYTHON_FUNCTION(is_key_released)
		ADD_PYTHON_FUNCTION(is_key_down)
		ADD_PYTHON_FUNCTION(set_mouse_click_callback)
		ADD_PYTHON_FUNCTION(set_mouse_down_callback)
		ADD_PYTHON_FUNCTION(set_mouse_double_click_callback)
		ADD_PYTHON_FUNCTION(set_key_down_callback)
		ADD_PYTHON_FUNCTION(set_key_press_callback)
		ADD_PYTHON_FUNCTION(set_key_release_callback)
		ADD_PYTHON_FUNCTION(set_resize_callback)
		ADD_PYTHON_FUNCTION(get_log_level)
		ADD_PYTHON_FUNCTION(clear_log)
		ADD_PYTHON_FUNCTION(show_logger)
		ADD_PYTHON_FUNCTION(set_log_level)
		ADD_PYTHON_FUNCTION(log)
		ADD_PYTHON_FUNCTION(log_debug)
		ADD_PYTHON_FUNCTION(log_info)
		ADD_PYTHON_FUNCTION(log_warning)
		ADD_PYTHON_FUNCTION(log_error)
		ADD_PYTHON_FUNCTION(is_plot_queried)
		ADD_PYTHON_FUNCTION(get_plot_query_area)
		ADD_PYTHON_FUNCTION(clear_plot)
		ADD_PYTHON_FUNCTION(set_plot_xlimits_auto)
		ADD_PYTHON_FUNCTION(set_plot_ylimits_auto)
		ADD_PYTHON_FUNCTION(set_plot_xlimits)
		ADD_PYTHON_FUNCTION(set_plot_ylimits)
		ADD_PYTHON_FUNCTION(set_color_map)
		ADD_PYTHON_FUNCTION(add_plot)
		ADD_PYTHON_FUNCTION(add_line_series)
		ADD_PYTHON_FUNCTION(add_scatter_series)
		ADD_PYTHON_FUNCTION(add_text_point)
		{NULL, NULL, 0, NULL}
	};

	static PyModuleDef marvelModule = {
		PyModuleDef_HEAD_INIT, "marvel", NULL, -1, marvelmethods,
		NULL, NULL, NULL, NULL
	};

	PyMODINIT_FUNC PyInit_marvel(void)
	{
		PyObject* m;

		m = PyModule_Create(&marvelModule);
		if (m == NULL)
			return NULL;

		auto MarvelError = PyErr_NewException("marvel.error", NULL, NULL);
		Py_XINCREF(MarvelError);
		if (PyModule_AddObject(m, "error", MarvelError) < 0) {
			Py_XDECREF(MarvelError);
			Py_CLEAR(MarvelError);
			Py_DECREF(m);
			return NULL;
		}

		return m;
	}

}