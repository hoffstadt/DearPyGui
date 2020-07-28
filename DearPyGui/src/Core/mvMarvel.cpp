#include "mvMarvel.h"
#include "mvApp.h"
#include "Core/mvInput.h"
#include "Core/mvDataStorage.h"
#include "mvAppLog.h"
#include "Core/StandardWindows/mvSourceWindow.h"
#include "mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#if defined (_WIN32)
	#include "Platform/Windows/mvWindowsWindow.h"
#define mvWindowClass mvWindowsWindow
#endif
#include "Core/mvPythonExceptions.h"

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define ADD_PYTHON_FUNCTION(Function) { #Function, (PyCFunction)Function, METH_VARARGS | METH_KEYWORDS, (*mvApp::GetApp()->getParsers())[#Function].getDocumentation() },

namespace Marvel {

	static void AddItemWithRuntimeChecks(mvAppItem* item, const char* parent, const char* before)
	{

		if (item == nullptr)
			return;

		auto ma = mvApp::GetApp();

		// remove bad parent stack item
		if (item->getType() == mvAppItemType::Window && ma->topParent() != nullptr)
		{
			if (ma->topParent()->getName() != "MainWindow")
			{
				ma->popParent();
				ThrowPythonException("Adding window will remove last item in parent stack. Don't forget to end container types.");
			}
		}

		// window runtime adding
		if (item->getType() == mvAppItemType::Window && mvApp::IsAppStarted())
			ma->addRuntimeItem("", "", item);

		// window compile adding
		else if (item->getType() == mvAppItemType::Window)
			ma->addWindow(item);

		// typical run time adding
		else if ((!std::string(parent).empty() || !std::string(before).empty()) && mvApp::IsAppStarted())
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

		// typical adding before runtime
		else if (std::string(parent).empty() && !mvApp::IsAppStarted() && std::string(before).empty())
			ma->addItem(item);
	}

	std::map<std::string, mvPythonParser>* BuildDearPyGuiInterface()
	{

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
		return parsers;
	}

	std::vector<std::pair<std::string, std::string>> GetAllCommands()
	{
		auto mapping = BuildDearPyGuiInterface();

		std::vector<std::pair<std::string, std::string>> result;

		for (const auto& item : *mapping)
			result.emplace_back(item.first, item.second.getDocumentation());

		delete mapping;

		return std::move(result);
	}

	PyObject* start_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvApp::GetApp()->precheck();
		mvApp::SetAppStarted();
		//PyEval_SaveThread();

		// create window
		mvWindow* window = new mvWindowClass(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight());
		window->show();
		window->run();
		delete window;
		mvApp::DeleteApp();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* start_dearpygui_editor(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvApp::SetAppStarted();

		// create window
		mvWindow* window = new mvWindowClass(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), true);
		window->show();
		window->run();
		delete window;
		mvApp::DeleteApp();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* start_dearpygui_docs(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		// create window
		mvWindow* window = new mvWindowClass(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), false, false, true);
		window->show();
		window->run();
		delete window;
		delete mvApp::GetApp();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* generate_stub_file(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;

		if (!(*mvApp::GetApp()->getParsers())["generate_stub_file"].parse(args, kwargs, __FUNCTION__, &file))
			return mvPythonTranslator::GetPyNone();

		GenerateStubFile(file);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float scale;

		if (!(*mvApp::GetApp()->getParsers())["set_global_font_scale"].parse(args, kwargs, __FUNCTION__, &scale))
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
		const char* file;
		const char* flags;

		if (!(*mvApp::GetApp()->getParsers())["run_file"].parse(args, kwargs, __FUNCTION__, &file, &flags))
			return mvPythonTranslator::GetPyNone();

		RunFile(file, flags);

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

		if (!(*mvApp::GetApp()->getParsers())["add_drawing"].parse(args, kwargs, __FUNCTION__, &name, &tip, &parent, &before, &width, &height))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = new mvDrawing("", name, width, height);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);

		if (!item)
			return mvPythonTranslator::GetPyNone();

		AddItemWithRuntimeChecks(item, parent, before);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_drawing_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width;
		int height;

		if (!(*mvApp::GetApp()->getParsers())["set_drawing_size"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
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

		if (!(*mvApp::GetApp()->getParsers())["set_drawing_origin"].parse(args, kwargs, __FUNCTION__, &name, &x, &y))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(name);

		if (item == nullptr)
		{
			std::string message = name;
			ThrowPythonException("Drawing does not exist");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(name) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["set_drawing_scale"].parse(args, kwargs, __FUNCTION__, &name, &x, &y))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(name);

		if (item == nullptr)
		{
			std::string message = name;
			ThrowPythonException("Drawing does not exist");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(name) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->setScale(x, y);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_drawing_origin(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_drawing_origin"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(name);

		if (item == nullptr)
		{
			std::string message = name;
			ThrowPythonException("Drawing does not exist");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(name) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}

		return mvPythonTranslator::ToPyPair(dwg->getOrigin().x, dwg->getOrigin().y);
	}

	PyObject* get_drawing_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_drawing_scale"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(name);

		if (item == nullptr)
		{
			std::string message = name;
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(name) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}

		return mvPythonTranslator::ToPyPair(dwg->getScale().x, dwg->getScale().y);
	}

	PyObject* get_drawing_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_drawing_size"].parse(args, kwargs, __FUNCTION__, &name))
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

		if (!(*mvApp::GetApp()->getParsers())["draw_image"].parse(args, kwargs, __FUNCTION__, &drawing, &file,
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
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_line"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &color, &thickness, &tag))
			return mvPythonTranslator::GetPyNone();

		mvVec2 mp1 = mvPythonTranslator::ToVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::ToVec2(p2);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_arrow"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &color, &thickness, &size, &tag))
			return mvPythonTranslator::GetPyNone();

		mvVec2 mp1 = mvPythonTranslator::ToVec2(p1);
		mvVec2 mp2 = mvPythonTranslator::ToVec2(p2);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_triangle"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &p3, &color, &fill, &thickness, &tag))
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
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_rectangle"].parse(args, kwargs, __FUNCTION__, &drawing, &pmin, &pmax, &color, &fill, &rounding, &thickness, &tag))
			return mvPythonTranslator::GetPyNone();


		mvVec2 mpmax = mvPythonTranslator::ToVec2(pmax);
		mvVec2 mpmin = mvPythonTranslator::ToVec2(pmin);
		mvColor mcolor = mvPythonTranslator::ToColor(color);
		mvColor mfill = mvPythonTranslator::ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_quad"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &fill, &thickness, &tag))
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
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_text"].parse(args, kwargs, __FUNCTION__, &drawing, &pos, &text, &color, &size, &tag))
			return mvPythonTranslator::GetPyNone();

		mvVec2 mpos = mvPythonTranslator::ToVec2(pos);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_circle"].parse(args, kwargs, __FUNCTION__, &drawing, &center, &radius, &color, &segments, &thickness, &fill, &tag))
			return mvPythonTranslator::GetPyNone();

		mvVec2 mcenter = mvPythonTranslator::ToVec2(center);
		mvColor mcolor = mvPythonTranslator::ToColor(color);
		mvColor mfill = mvPythonTranslator::ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_polyline"].parse(args, kwargs, __FUNCTION__, &drawing, &points, &color, &closed, &thickness, &tag))
			return mvPythonTranslator::GetPyNone();

		auto mpoints = mvPythonTranslator::ToVectVec2(points);
		mvColor mcolor = mvPythonTranslator::ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_polygon"].parse(args, kwargs, __FUNCTION__, &drawing, &points, &color, &fill, &thickness, &tag))
			return mvPythonTranslator::GetPyNone();

		auto mpoints = mvPythonTranslator::ToVectVec2(points);
		mvColor mcolor = mvPythonTranslator::ToColor(color);
		mvColor mfill = mvPythonTranslator::ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
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

		if (!(*mvApp::GetApp()->getParsers())["draw_bezier_curve"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &thickness, &segments, &tag))
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
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->drawBezierCurve(mp1, mp2, mp3, mp4, mcolor, thickness, segments, tag);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* clear_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		if (!(*mvApp::GetApp()->getParsers())["clear_drawing"].parse(args, kwargs, __FUNCTION__, &drawing))
			return mvPythonTranslator::GetPyNone();

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return mvPythonTranslator::GetPyNone();
		}
		dwg->clear();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* clear_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["clear_plot"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->clear();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_plot_xlimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["set_plot_xlimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setXLimitsAuto();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_plot_ylimits_auto(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["set_plot_ylimits_auto"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["set_plot_xlimits"].parse(args, kwargs, __FUNCTION__, &plot, &xmin, &xmax))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["set_plot_ylimits"].parse(args, kwargs, __FUNCTION__, &plot, &ymin, &ymax))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		graph->setYLimits(ymin, ymax);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["is_plot_queried"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		return Py_BuildValue("b", graph->isPlotQueried());
	}

	PyObject* get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* plot;

		if (!(*mvApp::GetApp()->getParsers())["get_plot_query_area"].parse(args, kwargs, __FUNCTION__, &plot))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["set_color_map"].parse(args, kwargs, __FUNCTION__, &plot, &map))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["add_plot"].parse(args, kwargs, __FUNCTION__, &name, &xAxisName, &yAxisName, &flags,
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

		if (!(*mvApp::GetApp()->getParsers())["add_line_series"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &color, &fill, &weight))
			return mvPythonTranslator::GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add line series requires a list of lists.");
			return mvPythonTranslator::GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["add_scatter_series"].parse(args, kwargs, __FUNCTION__, &plot, &name, &data, &marker,
			&size, &weight, &outline, &fill))
			return mvPythonTranslator::GetPyNone();

		if (!PyList_Check(data))
		{
			std::string message = plot;
			ThrowPythonException(message + " add scatter series requires a list of lists.");
			return mvPythonTranslator::GetPyNone();
		}

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);

		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["add_text_point"].parse(args, kwargs, __FUNCTION__,
			&plot, &name, &x, &y, &vertical, &xoffset, &yoffset))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* aplot = mvApp::GetApp()->getItem(plot);
		if (aplot == nullptr)
		{
			std::string message = plot;
			ThrowPythonException(message + " plot does not exist.");
			return mvPythonTranslator::GetPyNone();
		}
		mvPlot* graph = static_cast<mvPlot*>(aplot);

		mvSeries* series = new mvLabelSeries(name, { {(float)x, (float)y} }, xoffset, yoffset, vertical);

		graph->addSeries(series);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* show_logger(PyObject* self, PyObject* args)
	{
		mvAppLog::Show();
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return mvPythonTranslator::ToPyInt(mvAppLog::getLogLevel());
	}

	PyObject* set_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int level;
		if (!(*mvApp::GetApp()->getParsers())["set_log_level"].parse(args, kwargs, __FUNCTION__, &level))
			return mvPythonTranslator::GetPyNone();
		mvAppLog::setLogLevel(level);
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;
		const char* level = "TRACE";
		if (!(*mvApp::GetApp()->getParsers())["log"].parse(args, kwargs, __FUNCTION__, &message, &level))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::Log(std::string(message), std::string(level));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!(*mvApp::GetApp()->getParsers())["log_debug"].parse(args, kwargs, __FUNCTION__, &message))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::LogDebug(std::string(message));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!(*mvApp::GetApp()->getParsers())["log_info"].parse(args, kwargs, __FUNCTION__, &message))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::LogInfo(std::string(message));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log_warning(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!(*mvApp::GetApp()->getParsers())["log_warning"].parse(args, kwargs, __FUNCTION__, &message))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::LogWarning(std::string(message));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* log_error(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!(*mvApp::GetApp()->getParsers())["log_error"].parse(args, kwargs, __FUNCTION__, &message))
			return mvPythonTranslator::GetPyNone();

		mvAppLog::LogError(std::string(message));
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* clear_log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppLog::ClearLog();
		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int local = true;

		if (!(*mvApp::GetApp()->getParsers())["get_mouse_pos"].parse(args, kwargs, __FUNCTION__, &local))
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

		if (!(*mvApp::GetApp()->getParsers())["is_key_pressed"].parse(args, kwargs, __FUNCTION__, &key))
			return mvPythonTranslator::GetPyNone();

		bool pressed = mvInput::isKeyPressed(key);

		PyObject* pvalue = mvPythonTranslator::ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_released"].parse(args, kwargs, __FUNCTION__, &key))
			return mvPythonTranslator::GetPyNone();

		bool pressed = mvInput::isKeyReleased(key);

		PyObject* pvalue = mvPythonTranslator::ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_down"].parse(args, kwargs, __FUNCTION__, &key))
			return mvPythonTranslator::GetPyNone();

		bool pressed = mvInput::isKeyDown(key);

		PyObject* pvalue = mvPythonTranslator::ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;
		float threshold;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_dragging"].parse(args, kwargs, __FUNCTION__, &button, &threshold))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseDragging(button, threshold));
	}

	PyObject* is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_down"].parse(args, kwargs, __FUNCTION__, &button))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseButtonDown(button));
	}

	PyObject* is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseButtonClicked(button));
	}

	PyObject* is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_double_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseButtonDoubleClicked(button));
	}

	PyObject* is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_released"].parse(args, kwargs, __FUNCTION__, &button))
			return mvPythonTranslator::GetPyNone();

		return mvPythonTranslator::ToPyBool(mvInput::isMouseButtonReleased(button));
	}

	PyObject* set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
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

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_drag_callback"].parse(args, kwargs, __FUNCTION__, &callback, &threshold, &handler))
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

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_double_click_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
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

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_click_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
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

		if (!(*mvApp::GetApp()->getParsers())["set_key_down_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
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

		if (!(*mvApp::GetApp()->getParsers())["set_key_press_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
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

		if (!(*mvApp::GetApp()->getParsers())["set_key_release_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
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

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_wheel_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
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

		if (!(*mvApp::GetApp()->getParsers())["add_table"].parse(args, kwargs, __FUNCTION__, &name, &headers, &callback, &parent, &before))
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

		if (!(*mvApp::GetApp()->getParsers())["add_simple_plot"].parse(args, kwargs, __FUNCTION__, &name, &value, &autoscale, &overlay,
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

		if (!(*mvApp::GetApp()->getParsers())["add_progress_bar"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
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

		if (!(*mvApp::GetApp()->getParsers())["add_image"].parse(args, kwargs, __FUNCTION__, &name,
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
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
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

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
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

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
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

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
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

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
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

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
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

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
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

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
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

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
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

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
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

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
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

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
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

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
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

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
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

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
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

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
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


		if (!(*mvApp::GetApp()->getParsers())["add_text"].parse(args, kwargs, __FUNCTION__, &name, &wrap,
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


		if (!(*mvApp::GetApp()->getParsers())["add_label_text"].parse(args, kwargs, __FUNCTION__, &name, &value,
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

		if (!(*mvApp::GetApp()->getParsers())["add_listbox"].parse(args, kwargs, __FUNCTION__, &name, &items,
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

		if (!(*mvApp::GetApp()->getParsers())["add_combo"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &tip, &parent, &before, &data_source, &width
			, &secondary_data_source))
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

		if (!(*mvApp::GetApp()->getParsers())["add_selectable"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_button"].parse(args, kwargs, __FUNCTION__, &name, &smallb,
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

		if (!(*mvApp::GetApp()->getParsers())["add_input_text"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &hint, &multiline, &no_spaces,
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

		if (!(*mvApp::GetApp()->getParsers())["add_input_int"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_input_int2"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_input_int3"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_input_int4"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_input_float"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_input_float2"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_input_float3"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_input_float4"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_indent"].parse(args, kwargs, __FUNCTION__, &name, &offset,
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

		if (!(*mvApp::GetApp()->getParsers())["unindent"].parse(args, kwargs, __FUNCTION__, &name, &offset, &parent, &before))
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

		if (!(*mvApp::GetApp()->getParsers())["add_tab_bar"].parse(args, kwargs, __FUNCTION__, &name, &reorderable,
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

		if (!(*mvApp::GetApp()->getParsers())["add_tab"].parse(args, kwargs, __FUNCTION__, &name, &closeable,
			&tip, &parent, &before))
			return mvPythonTranslator::GetPyNone();

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			ThrowPythonException("add_tab must follow a call to add_tabbar.");

		else if (parentItem->getType() == mvAppItemType::TabBar)
		{
			mvAppItem* item = new mvTab("", name, closeable);
			item->setTip(tip);
			AddItemWithRuntimeChecks(item, parent, before);
			mvApp::GetApp()->pushParent(item);
		}

		else
			ThrowPythonException("add_tab was called incorrectly. Did you forget to call end_tab?");

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* end_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		return mvPythonTranslator::GetPyNone();

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			ThrowPythonException("end_tab must follow a call to add_tab.");

		else if (parentItem->getType() == mvAppItemType::TabItem)
			mvApp::GetApp()->popParent();

		else
			ThrowPythonException("end_tab was called incorrectly and will be ignored");

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* end_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		return mvPythonTranslator::GetPyNone();

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			ThrowPythonException("end_tabbar must follow a call to add_menubar.");

		else if (parentItem->getType() == mvAppItemType::TabBar)
			mvApp::GetApp()->popParent();

		else
		{
			ThrowPythonException("add_menubar was called incorrectly. Did you forget to call end_menu?");
			ThrowPythonException("Taking corrective action. Clearing parent stack.");

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

		if (!(*mvApp::GetApp()->getParsers())["add_menu_bar"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_menu"].parse(args, kwargs, __FUNCTION__, &name,
			&tip, &parent, &before))
			return mvPythonTranslator::GetPyNone();

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			ThrowPythonException("add_menu must follow a call to add_menubar.");

		else if (parentItem->getType() == mvAppItemType::MenuBar || parentItem->getType() == mvAppItemType::Menu)
		{
			mvAppItem* item = new mvMenu("", name);
			item->setTip(tip);
			AddItemWithRuntimeChecks(item, parent, before);
			mvApp::GetApp()->pushParent(item);
		}

		else
			ThrowPythonException("add_menu was called incorrectly. Did you forget to call end_menu?");

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* end_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			ThrowPythonException("end_menu must follow a call to add_menu.");

		else if (parentItem->getType() == mvAppItemType::Menu)
			mvApp::GetApp()->popParent();

		else
			ThrowPythonException("end_menu was called incorrectly and will be ignored");

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* end_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			ThrowPythonException("end_menu_bar must follow a call to add_menu_bar.");

		else if (parentItem->getType() == mvAppItemType::MenuBar)
			mvApp::GetApp()->popParent();

		else
		{
			ThrowPythonException("end_menu_bar was called incorrectly. Did you forget to call end_menu?");
			ThrowPythonException("Taking corrective action. Clearing parent stack.");

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

		if (!(*mvApp::GetApp()->getParsers())["add_menu_item"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_spacing"].parse(args, kwargs, __FUNCTION__, &name, &count,
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

		if (!(*mvApp::GetApp()->getParsers())["add_same_line"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_radio_button"].parse(args, kwargs, __FUNCTION__, &name, &items,
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

		if (!(*mvApp::GetApp()->getParsers())["add_group"].parse(args, kwargs, __FUNCTION__, &name,
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
			ThrowPythonException("end_group must follow a call to add_group.");

		else if (parentItem->getType() == mvAppItemType::Group)
			mvApp::GetApp()->popParent();

		else
			ThrowPythonException("end_group was called incorrectly and will be ignored");

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

		if (!(*mvApp::GetApp()->getParsers())["add_child"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
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
			ThrowPythonException("end_window must follow a call to add_window.");

		else if (parentItem->getType() == mvAppItemType::Window)
			mvApp::GetApp()->popParent();

		else
			ThrowPythonException("end_window was called incorrectly and will be ignored");

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_window_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* window;
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_window_pos"].parse(args, kwargs, __FUNCTION__, &window, &x, &y))
			return mvPythonTranslator::GetPyNone();

		auto awindow = mvApp::GetApp()->getWindow(window);

		if (awindow == nullptr)
		{
			ThrowPythonException(window + std::string(" window was not found"));
			return mvPythonTranslator::GetPyNone();
		}

		awindow->setWindowPos(x, y);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_window_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* window;

		if (!(*mvApp::GetApp()->getParsers())["get_window_pos"].parse(args, kwargs, __FUNCTION__, &window))
			return mvPythonTranslator::GetPyNone();

		auto awindow = mvApp::GetApp()->getWindow(window);

		if (awindow == nullptr)
		{
			ThrowPythonException(window + std::string(" window was not found"));
			return mvPythonTranslator::GetPyNone();
		}

		mvVec2 pos = awindow->getWindowPos();

		return mvPythonTranslator::ToPyPair(pos.x, pos.y);
	}

	PyObject* end_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			ThrowPythonException("end_child must follow a call to add_child.");

		else if (parentItem->getType() == mvAppItemType::Child)
			mvApp::GetApp()->popParent();

		else
			ThrowPythonException("end_child was called incorrectly and will be ignored");

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tipparent;
		const char* name;
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tooltip"].parse(args, kwargs, __FUNCTION__, &tipparent,
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
			ThrowPythonException("end_tooltip must follow a call to add_tooltip.");

		else if (parentItem->getType() == mvAppItemType::Tooltip)
			mvApp::GetApp()->popParent();

		else
			ThrowPythonException("end_tooltip was called incorrectly and will be ignored");

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

		if (!(*mvApp::GetApp()->getParsers())["add_popup"].parse(args, kwargs, __FUNCTION__, &popupparent,
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
			ThrowPythonException("end_popup must follow a call to add_group.");

		else if (parentItem->getType() == mvAppItemType::Popup)
			mvApp::GetApp()->popParent();

		else
			ThrowPythonException("end_popup was called incorrectly and will be ignored");

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

		if (!(*mvApp::GetApp()->getParsers())["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name,
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
			ThrowPythonException("end_collapsing_header must follow a call to add_collapsing_header.");

		else if (parentItem->getType() == mvAppItemType::CollapsingHeader)
			mvApp::GetApp()->popParent();

		else
			ThrowPythonException("end_collapsing_header was called incorrectly and will be ignored");

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

		if (!(*mvApp::GetApp()->getParsers())["add_tree_node"].parse(args, kwargs, __FUNCTION__, &name,
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
			ThrowPythonException("end_tree_node must follow a call to add_tree_node.");

		else if (parentItem->getType() == mvAppItemType::TreeNode)
			mvApp::GetApp()->popParent();

		else
			ThrowPythonException("end_tree_node was called incorrectly and will be ignored");

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

		if (!(*mvApp::GetApp()->getParsers())["add_seperator"].parse(args, kwargs, __FUNCTION__, &name, &tip, &parent, &before))
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

		if (!(*mvApp::GetApp()->getParsers())["add_color_edit3"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_color_edit4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
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

		if (!(*mvApp::GetApp()->getParsers())["add_color_picker3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
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

		if (!(*mvApp::GetApp()->getParsers())["add_color_picker4"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["add_checkbox"].parse(args, kwargs, __FUNCTION__, &name,
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

		if (!(*mvApp::GetApp()->getParsers())["set_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["delete_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["delete_column"].parse(args, kwargs, __FUNCTION__, &table, &column))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["add_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["add_column"].parse(args, kwargs, __FUNCTION__, &table, &name, &column))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["insert_row"].parse(args, kwargs, __FUNCTION__, &table, &row_index, &row))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["insert_column"].parse(args, kwargs, __FUNCTION__, &table, &column_index, &name, &column))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["set_table_selection"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setSelection(row, column, value);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* clear_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!(*mvApp::GetApp()->getParsers())["clear_table"].parse(args, kwargs, __FUNCTION__, &table))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["get_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return mvPythonTranslator::GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return Py_BuildValue("s", atable->getTableItem(row, column).c_str());

	}

	PyObject* get_table_selections(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!(*mvApp::GetApp()->getParsers())["get_table_selections"].parse(args, kwargs, __FUNCTION__, &table))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
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

		if (!(*mvApp::GetApp()->getParsers())["run_async_function"].parse(args, kwargs, __FUNCTION__, &name, &data, &return_handler))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->addMTCallback(name, data, return_handler);

		return mvPythonTranslator::GetPyNone();

	}

	PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			ThrowPythonException("Items can not be modified outside of the main thread.");
			return mvPythonTranslator::GetPyNone();
		}

		const char* item;
		int childrenOnly = false;

		if (!(*mvApp::GetApp()->getParsers())["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
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

		if (!(*mvApp::GetApp()->getParsers())["open_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			return mvPythonTranslator::GetPyNone();

		std::string file = OpenFile(mvPythonTranslator::ToVectPairString(extensions));

		return mvPythonTranslator::ToPyString(file.c_str());
	}

	PyObject* save_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* extensions;

		if (!(*mvApp::GetApp()->getParsers())["save_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			return mvPythonTranslator::GetPyNone();

		std::string file = SaveFile(mvPythonTranslator::ToVectPairString(extensions));

		return mvPythonTranslator::ToPyString(SaveFile(mvPythonTranslator::ToVectPairString(extensions)).c_str());
	}

	PyObject* move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			ThrowPythonException("Items can not be modified outside of the main thread.");
			return mvPythonTranslator::GetPyNone();
		}

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->moveItemUp(item);

		return mvPythonTranslator::GetPyNone();

	}

	PyObject* move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			ThrowPythonException("Items can not be modified outside of the main thread.");
			return mvPythonTranslator::GetPyNone();
		}

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->moveItemDown(item);

		return mvPythonTranslator::GetPyNone();

	}

	PyObject* get_style_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!(*mvApp::GetApp()->getParsers())["get_style_item"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto values = mvApp::GetApp()->getStyleItem(item);

		return mvPythonTranslator::ToPyPair(values.first, values.second);
	}

	PyObject* get_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!(*mvApp::GetApp()->getParsers())["get_theme_item"].parse(args, kwargs, __FUNCTION__, &item))
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

		if (!(*mvApp::GetApp()->getParsers())["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyString(appitem->getCallback());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_height"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyInt(appitem->getHeight());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_width"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyInt(appitem->getWidth());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_popup"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyString(appitem->getPopup());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_tip"].parse(args, kwargs, __FUNCTION__, &item))
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

		if (!(*mvApp::GetApp()->getParsers())["set_threadpool_timeout"].parse(args, kwargs, __FUNCTION__, &time))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->setThreadPoolTimeout(time);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		int threads;

		if (!(*mvApp::GetApp()->getParsers())["set_thread_count"].parse(args, kwargs, __FUNCTION__, &threads))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->setThreadCount(threads);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* add_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		PyObject* data;

		if (!(*mvApp::GetApp()->getParsers())["add_data"].parse(args, kwargs, __FUNCTION__, &name, &data))
			return mvPythonTranslator::GetPyNone();

		mvDataStorage::AddData(name, data);
		Py_XINCREF(data);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_data"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		auto result = mvDataStorage::GetData(name);

		if (result)
			return result;

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* delete_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["delete_data"].parse(args, kwargs, __FUNCTION__, &name))
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

		if (!(*mvApp::GetApp()->getParsers())["add_item_color_style"].parse(args, kwargs, __FUNCTION__, &item, &style, &color))
			return mvPythonTranslator::GetPyNone();

		auto mcolor = mvPythonTranslator::ToColor(color);

		mvApp::GetApp()->addItemColorStyle(item, style, mcolor);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_hovered"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemHovered());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_active"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemActive());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_focused"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemFocused());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_clicked"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemClicked());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_visible"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemVisible());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_edited"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemEdited());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_activated"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemActivated());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_deactivated"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemDeactivated());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_deactivated_after_edit"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemDeactivatedAfterEdit());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_toggled_open"].parse(args, kwargs, __FUNCTION__, &item))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return mvPythonTranslator::ToPyBool(appitem->isItemToogledOpen());

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_min"].parse(args, kwargs, __FUNCTION__, &item))
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

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_max"].parse(args, kwargs, __FUNCTION__, &item))
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

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_size"].parse(args, kwargs, __FUNCTION__, &item))
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

		if (!(*mvApp::GetApp()->getParsers())["change_style_item"].parse(args, kwargs, __FUNCTION__, &item, &x, &y))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->changeStyleItem(item, x, y);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* change_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		int r, g, b, a;

		if (!(*mvApp::GetApp()->getParsers())["change_theme_item"].parse(args, kwargs, __FUNCTION__, &item, &r, &g, &b, &a))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->changeThemeItem(item, { r, g, b, a });



		return mvPythonTranslator::GetPyNone();
	}

	PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_value"].parse(args, kwargs, __FUNCTION__, &name))
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

		if (!(*mvApp::GetApp()->getParsers())["set_value"].parse(args, kwargs, __FUNCTION__, &name, &value))
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

		if (!(*mvApp::GetApp()->getParsers())["show_item"].parse(args, kwargs, __FUNCTION__, &name))
			return mvPythonTranslator::GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->show();

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* hide_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["hide_item"].parse(args, kwargs, __FUNCTION__, &name))
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

		if (!(*mvApp::GetApp()->getParsers())["set_render_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return mvPythonTranslator::GetPyNone();

		if (std::string(handler) == "MainWindow")
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
					ThrowPythonException("Render callback can only be set for window/child items");
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_resize_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
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
					ThrowPythonException("Resize callback can only be set for window/child items");
			}
		}

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["set_item_callback"].parse(args, kwargs, __FUNCTION__, &item, &callback))
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

		if (!(*mvApp::GetApp()->getParsers())["set_item_popup"].parse(args, kwargs, __FUNCTION__, &item, &popup))
			return mvPythonTranslator::GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setPopup(popup);

		return mvPythonTranslator::GetPyNone();
	}

	PyObject* set_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tip, * item;

		if (!(*mvApp::GetApp()->getParsers())["set_item_tip"].parse(args, kwargs, __FUNCTION__, &item, &tip))
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

		if (!(*mvApp::GetApp()->getParsers())["set_item_width"].parse(args, kwargs, __FUNCTION__, &item, &width))
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

		if (!(*mvApp::GetApp()->getParsers())["set_item_height"].parse(args, kwargs, __FUNCTION__, &item, &height))
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

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_size"].parse(args, kwargs, __FUNCTION__, &width, &height))
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

		if (!(*mvApp::GetApp()->getParsers())["set_theme"].parse(args, kwargs, __FUNCTION__, &theme))
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
		const char* file;

		if (!(*mvApp::GetApp()->getParsers())["show_source"].parse(args, kwargs, __FUNCTION__, &file))
			return mvPythonTranslator::GetPyNone();

		mvApp::GetApp()->showStandardWindow("source");
		auto window = static_cast<mvSourceWindow*>(mvApp::GetApp()->getStandardWindow("source"));
		window->setFile(file);
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

	std::vector<std::pair<std::string, long>> GetModuleConstants()
	{
		std::vector<std::pair<std::string, long>> ModuleConstants =
		{
			//-----------------------------------------------------------------------------
			// Plot Flags
			//-----------------------------------------------------------------------------
			{"mvPlotFlags_MousePos"   , 1 << 0},  // the mouse position, in plot coordinates, will be displayed in the bottom-right
			{"mvPlotFlags_Legend"     , 1 << 1},  // a legend will be displayed in the top-left
			{"mvPlotFlags_Highlight"  , 1 << 2},  // plot items will be highlighted when their legend entry is hovered
			{"mvPlotFlags_BoxSelect"  , 1 << 3},  // the user will be able to box-select with right-mouse
			{"mvPlotFlags_Query"      , 1 << 4},  // the user will be able to draw query rects with middle-mouse
			{"mvPlotFlags_ContextMenu", 1 << 5},  // the user will be able to open a context menu with double-right click
			{"mvPlotFlags_Crosshairs" , 1 << 6},  // the default mouse cursor will be replaced with a crosshair when hovered
			{"mvPlotFlags_CullData"   , 1 << 7},  // plot data outside the plot area will be culled from rendering
			{"mvPlotFlags_AntiAliased", 1 << 8},  // lines and fills will be anti-aliased (not recommended)
			{"mvPlotFlags_NoChild"    , 1 << 9},  // a child window region will not be used to capture mouse scroll (can boost performance for single ImGui window applications)
			{"mvPlotFlags_YAxis2"     , 1 << 10}, // enable a 2nd y axis
			{"mvPlotFlags_YAxis3"     , 1 << 11}, // enable a 3rd y axis
			{"mvPlotFlags_Default"    ,     175},

			//-----------------------------------------------------------------------------
			// Axis Flags
			//-----------------------------------------------------------------------------
			{"mvPlotAxisFlags_GridLines" , 1 << 0}, // grid lines will be displayed
			{"mvPlotAxisFlags_TickMarks" , 1 << 1}, // tick marks will be displayed for each grid line
			{"mvPlotAxisFlags_TickLabels", 1 << 2}, // text labels will be displayed for each grid line
			{"mvPlotAxisFlags_Invert"    , 1 << 3}, // the axis will be inverted
			{"mvPlotAxisFlags_LockMin"   , 1 << 4}, // the axis minimum value will be locked when panning/zooming
			{"mvPlotAxisFlags_LockMax"   , 1 << 5}, // the axis maximum value will be locked when panning/zooming
			{"mvPlotAxisFlags_Adaptive"  , 1 << 6}, // grid divisions will adapt to the current pixel size the axis
			{"mvPlotAxisFlags_LogScale"  , 1 << 7}, // a logartithmic (base 10) axis scale will be used
			{"mvPlotAxisFlags_Scientific", 1 << 8}, // scientific notation will be used for tick labels if displayed (WIP, not very good yet)
			{"mvPlotAxisFlags_Default"   ,     71},
			{"mvPlotAxisFlags_Auxiliary" ,     70},

			//-----------------------------------------------------------------------------
			// Plot Colors
			//-----------------------------------------------------------------------------
			{"mvPlotCol_Line"         , 0}, // plot line/outline color (defaults to a rotating color set)
			{"mvPlotCol_Fill"         , 1}, // plot fill color for bars (defaults to the current line color)
			{"mvPlotCol_MarkerOutline", 2}, // marker outline color (defaults to the current line color)
			{"mvPlotCol_MarkerFill"   , 3}, // marker fill color (defaults to the current line color)
			{"mvPlotCol_ErrorBar"     , 4}, // error bar color (defaults to ImGuiCol_Text)
			{"mvPlotCol_FrameBg"      , 5}, // plot frame background color (defaults to ImGuiCol_FrameBg)
			{"mvPlotCol_PlotBg"       , 6}, // plot area background color (defaults to ImGuiCol_WindowBg)
			{"mvPlotCol_PlotBorder"   , 7}, // plot area border color (defaults to ImGuiCol_Text)
			{"mvPlotCol_XAxis"        , 8}, // x-axis grid/label color (defaults to 25% ImGuiCol_Text)
			{"mvPlotCol_YAxis"        , 9}, // y-axis grid/label color (defaults to 25% ImGuiCol_Text)
			{"mvPlotCol_YAxis2"       ,10}, // 2nd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
			{"mvPlotCol_YAxis3"       ,11}, // 3rd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
			{"mvPlotCol_Selection"    ,12}, // box-selection color (defaults to yellow)
			{"mvPlotCol_Query"        ,13}, // box-query color (defaults to green)
			{"mvPlotCol_COUNT"        ,14},

			//-----------------------------------------------------------------------------
			// Plot Marker Specifications
			//-----------------------------------------------------------------------------
			{"mvPlotMarker_None"    , 1 << 0},  // no marker
			{"mvPlotMarker_Circle"  , 1 << 1},  // a circle marker will be rendered at each point
			{"mvPlotMarker_Square"  , 1 << 2},  // a square maker will be rendered at each point
			{"mvPlotMarker_Diamond" , 1 << 3},  // a diamond marker will be rendered at each point
			{"mvPlotMarker_Up"      , 1 << 4},  // an upward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Down"    , 1 << 5},  // an downward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Left"    , 1 << 6},  // an leftward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Right"   , 1 << 7},  // an rightward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Cross"   , 1 << 8},  // a cross marker will be rendered at each point (not filled)
			{"mvPlotMarker_Plus"    , 1 << 9},  // a plus marker will be rendered at each point (not filled)
			{"mvPlotMarker_Asterisk", 1 << 10}, // a asterisk marker will be rendered at each point (not filled)

			//-----------------------------------------------------------------------------
			// Built-in ImPlot Color maps
			//-----------------------------------------------------------------------------
			{"mvPlotColormap_Default",  0}, // ImPlot default colormap         (n=10)
			{"mvPlotColormap_Dark"   ,  1}, // a.k.a. matplotlib "Set1"        (n=9)
			{"mvPlotColormap_Pastel" ,  2}, // a.k.a. matplotlib "Pastel1"     (n=9)
			{"mvPlotColormap_Paired" ,  3}, // a.k.a. matplotlib "Paired"      (n=12)
			{"mvPlotColormap_Viridis",  4}, // a.k.a. matplotlib "viridis"     (n=11)
			{"mvPlotColormap_Plasma" ,  5}, // a.k.a. matplotlib "plasma"      (n=11)
			{"mvPlotColormap_Hot"    ,  6}, // a.k.a. matplotlib/MATLAB "hot"  (n=11)
			{"mvPlotColormap_Cool"   ,  7}, // a.k.a. matplotlib/MATLAB "cool" (n=11)
			{"mvPlotColormap_Pink"   ,  8}, // a.k.a. matplotlib/MATLAB "pink" (n=11)
			{"mvPlotColormap_Jet"    ,  9}, // a.k.a. MATLAB "jet"             (n=11)

			//-----------------------------------------------------------------------------
			// Theme Color IDs
			//-----------------------------------------------------------------------------
			{"mvGuiCol_Text"                 , 0},
			{"mvGuiCol_TextDisabled"         , 1},
			{"mvGuiCol_WindowBg"             , 2}, // Background of normal windows
			{"mvGuiCol_ChildBg"              , 3}, // Background of child windows
			{"mvGuiCol_PopupBg"              , 4}, // Background of popups, menus, tooltips windows
			{"mvGuiCol_Border"               , 5},
			{"mvGuiCol_BorderShadow"         , 6},
			{"mvGuiCol_FrameBg"              , 7},
			{"mvGuiCol_FrameBgHovered"       , 8},
			{"mvGuiCol_FrameBgActive"        , 9},
			{"mvGuiCol_TitleBg"              , 10},
			{"mvGuiCol_TitleBgActive"        , 11},
			{"mvGuiCol_TitleBgCollapsed"     , 12},
			{"mvGuiCol_MenuBarBg"            , 13},
			{"mvGuiCol_ScrollbarBg"          , 14},
			{"mvGuiCol_ScrollbarGrab"        , 15},
			{"mvGuiCol_ScrollbarGrabHovered" , 16},
			{"mvGuiCol_ScrollbarGrabActive"  , 17},
			{"mvGuiCol_CheckMark"            , 18},
			{"mvGuiCol_SliderGrab"           , 19},
			{"mvGuiCol_SliderGrabActive"     , 20},
			{"mvGuiCol_Button"               , 21},
			{"mvGuiCol_ButtonHovered"        , 22},
			{"mvGuiCol_ButtonActive"         , 23},
			{"mvGuiCol_Header"               , 24}, // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
			{"mvGuiCol_HeaderHovered"        , 25},
			{"mvGuiCol_HeaderActive"         , 26},
			{"mvGuiCol_Separator"            , 27},
			{"mvGuiCol_SeparatorHovered"     , 28},
			{"mvGuiCol_SeparatorActive"      , 29},
			{"mvGuiCol_ResizeGrip"           , 30},
			{"mvGuiCol_ResizeGripHovered"    , 31},
			{"mvGuiCol_ResizeGripActive"     , 32},
			{"mvGuiCol_Tab"                  , 33},
			{"mvGuiCol_TabHovered"           , 34},
			{"mvGuiCol_TabActive"            , 35},
			{"mvGuiCol_TabUnfocused"         , 36},
			{"mvGuiCol_TabUnfocusedActive"   , 37},
			{"mvGuiCol_PlotLines"            , 38},
			{"mvGuiCol_PlotLinesHovered"     , 39},
			{"mvGuiCol_PlotHistogram"        , 40},
			{"mvGuiCol_PlotHistogramHovered" , 41},
			{"mvGuiCol_TextSelectedBg"       , 42},
			{"mvGuiCol_DragDropTarget"       , 43},
			{"mvGuiCol_NavHighlight"         , 44}, // Gamepad/keyboard: current highlighted item
			{"mvGuiCol_NavWindowingHighlight", 45}, // Highlight window when using CTRL+TAB
			{"mvGuiCol_NavWindowingDimBg"    , 46}, // Darken/colorize entire screen behind the CTRL+TAB window list, when active 
			{"mvGuiCol_ModalWindowDimBg"     , 47}, // Darken/colorize entire screen behind a modal window, when one is active

			//-----------------------------------------------------------------------------
			// Style IDs
			//-----------------------------------------------------------------------------
			// Enum name --------------------- // Member in mvGuiStyle structure (see GuiStyle for descriptions)
			{ "mvGuiStyleVar_Alpha",               0}, // float     Alpha
			{ "mvGuiStyleVar_WindowPadding",       1}, // mvVec2    WindowPadding
			{ "mvGuiStyleVar_WindowRounding",      2}, // float     WindowRounding
			{ "mvGuiStyleVar_WindowBorderSize",    3}, // float     WindowBorderSize
			{ "mvGuiStyleVar_WindowMinSize",       4}, // mvVec2    WindowMinSize
			{ "mvGuiStyleVar_WindowTitleAlign",    5}, // mvVec2    WindowTitleAlign
			{ "mvGuiStyleVar_ChildRounding",       6}, // float     ChildRounding
			{ "mvGuiStyleVar_ChildBorderSize",     7}, // float     ChildBorderSize
			{ "mvGuiStyleVar_PopupRounding",       8}, // float     PopupRounding
			{ "mvGuiStyleVar_PopupBorderSize",     9}, // float     PopupBorderSize
			{ "mvGuiStyleVar_FramePadding",        10}, // mvVec2    FramePadding
			{ "mvGuiStyleVar_FrameRounding",       11}, // float     FrameRounding
			{ "mvGuiStyleVar_FrameBorderSize",     12}, // float     FrameBorderSize
			{ "mvGuiStyleVar_ItemSpacing",         13}, // mvVec2    ItemSpacing
			{ "mvGuiStyleVar_ItemInnerSpacing",    14}, // mvVec2    ItemInnerSpacing
			{ "mvGuiStyleVar_IndentSpacing",       15}, // float     IndentSpacing
			{ "mvGuiStyleVar_ScrollbarSize",       16}, // float     ScrollbarSize
			{ "mvGuiStyleVar_ScrollbarRounding",   17}, // float     ScrollbarRounding
			{ "mvGuiStyleVar_GrabMinSize",         18}, // float     GrabMinSize
			{ "mvGuiStyleVar_GrabRounding",        19}, // float     GrabRounding
			{ "mvGuiStyleVar_TabRounding",         20}, // float     TabRounding
			{ "mvGuiStyleVar_ButtonTextAlign",     21}, // mvVec2    ButtonTextAlign
			{ "mvGuiStyleVar_SelectableTextAlign", 22}, // mvVec2    SelectableTextAlign

			{ "ImGuiStyleVar_TouchExtraPadding", 23}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_TabBorderSize", 24}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_DisplaySafeAreaPadding", 25}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_AntiAliasedLines", 26}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_AntiAliasedFill", 27}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_CurveTessellationTol", 28}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_CircleSegmentMaxError", 29}, // mvVec2    SelectableTextAlign

			//-----------------------------------------------------------------------------
			// Key Codes
			//-----------------------------------------------------------------------------
			{ "mvKey_Back"			, 0x08 },
			{ "mvKey_Tab"			, 0x09 },
			{ "mvKey_Clear"			, 0x0C },
			{ "mvKey_Return"		, 0x0D },
			{ "mvKey_Shift"			, 0x10 },
			{ "mvKey_Control"		, 0x11 },
			{ "mvKey_Alt"			, 0x12 },
			{ "mvKey_Pause"			, 0x13 },
			{ "mvKey_Capital"		, 0x14 },
			{ "mvKey_Escape"		, 0x1B },
			{ "mvKey_Spacebar"		, 0x20 },
			{ "mvKey_Prior"			, 0x21 },
			{ "mvKey_Next"			, 0x22 },
			{ "mvKey_End"			, 0x23 },
			{ "mvKey_Home"			, 0x24 },
			{ "mvKey_Left"			, 0x25 },
			{ "mvKey_Up"			, 0x26 },
			{ "mvKey_Right"			, 0x27 },
			{ "mvKey_Down"			, 0x28 },
			{ "mvKey_Select"		, 0x29 },
			{ "mvKey_Print"			, 0x2A },
			{ "mvKey_Execute"		, 0x2B },
			{ "mvKey_PrintScreen"	, 0x2C },
			{ "mvKey_Insert"		, 0x2D },
			{ "mvKey_Delete"		, 0x2E },
			{ "mvKey_Help"			, 0x2F },
			{ "mvKey_0"			, 0x30 },
			{ "mvKey_1"			, 0x31 },
			{ "mvKey_2"			, 0x32 },
			{ "mvKey_3"			, 0x33 },
			{ "mvKey_4"			, 0x34 },
			{ "mvKey_5"			, 0x35 },
			{ "mvKey_6"			, 0x36 },
			{ "mvKey_7"			, 0x37 },
			{ "mvKey_8"			, 0x38 },
			{ "mvKey_9"			, 0x39 },
			{ "mvKey_A"			, 0x41 },
			{ "mvKey_B"			, 0x42 },
			{ "mvKey_C"			, 0x43 },
			{ "mvKey_D"			, 0x44 },
			{ "mvKey_E"			, 0x45 },
			{ "mvKey_F"			, 0x46 },
			{ "mvKey_G"			, 0x47 },
			{ "mvKey_H"			, 0x48 },
			{ "mvKey_I"			, 0x49 },
			{ "mvKey_J"			, 0x4A },
			{ "mvKey_K"			, 0x4B },
			{ "mvKey_L"			, 0x4C },
			{ "mvKey_M"			, 0x4D },
			{ "mvKey_N"			, 0x4E },
			{ "mvKey_O"			, 0x4F },
			{ "mvKey_P"			, 0x50 },
			{ "mvKey_Q"			, 0x51 },
			{ "mvKey_R"			, 0x52 },
			{ "mvKey_S"			, 0x53 },
			{ "mvKey_T"			, 0x54 },
			{ "mvKey_U"			, 0x55 },
			{ "mvKey_V"			, 0x56 },
			{ "mvKey_W"			, 0x57 },
			{ "mvKey_X"			, 0x58 },
			{ "mvKey_Y"			, 0x59 },
			{ "mvKey_Z"			, 0x5A },
			{ "mvKey_LWin"		, 0x5B },
			{ "mvKey_RWin"		, 0x5C },
			{ "mvKey_Apps"		, 0x5D },
			{ "mvKey_Sleep"		, 0x5F },
			{ "mvKey_NumPad0"   , 0x60 },
			{ "mvKey_NumPad1"   , 0x61 },
			{ "mvKey_NumPad2"   , 0x62 },
			{ "mvKey_NumPad3"   , 0x63 },
			{ "mvKey_NumPad4"   , 0x64 },
			{ "mvKey_NumPad5"   , 0x65 },
			{ "mvKey_NumPad6"	, 0x66 },
			{ "mvKey_NumPad7"	, 0x67 },
			{ "mvKey_NumPad8"	, 0x68 },
			{ "mvKey_NumPad9"	, 0x69 },
			{ "mvKey_Multiply"	, 0x6A },
			{ "mvKey_Add"		, 0x6B },
			{ "mvKey_Separator"	, 0x6C },
			{ "mvKey_Subtract"	, 0x6D },
			{ "mvKey_Decimal"	, 0x6E },
			{ "mvKey_Divide"	, 0x6F },
			{ "mvKey_F1"		, 0x70 },
			{ "mvKey_F2"		, 0x71 },
			{ "mvKey_F3"		, 0x72 },
			{ "mvKey_F4"		, 0x73 },
			{ "mvKey_F5"		, 0x74 },
			{ "mvKey_F6"		, 0x75 },
			{ "mvKey_F7"		, 0x76 },
			{ "mvKey_F8"		, 0x77 },
			{ "mvKey_F9"		, 0x78 },
			{ "mvKey_F10"		, 0x79 },
			{ "mvKey_F11"		, 0x7A },
			{ "mvKey_F12"		, 0x7B },
			{ "mvKey_F13"		, 0x7C },
			{ "mvKey_F14"		, 0x7D },
			{ "mvKey_F15"		, 0x7E },
			{ "mvKey_F16"		, 0x7F },
			{ "mvKey_F17"		, 0x80 },
			{ "mvKey_F18"		, 0x81 },
			{ "mvKey_F19"		, 0x82 },
			{ "mvKey_F20"		, 0x83 },
			{ "mvKey_F21"		, 0x84 },
			{ "mvKey_F22"		, 0x85 },
			{ "mvKey_F23"		, 0x86 },
			{ "mvKey_F24"		, 0x87 },
			{ "mvKey_NumLock"				, 0x90 },
			{ "mvKey_ScrollLock"			, 0x91 },
			{ "mvKey_LShift"				, 0xA0 },
			{ "mvKey_RShift"				, 0xA1 },
			{ "mvKey_LControl"				, 0xA2 },
			{ "mvKey_RControl"				, 0xA3 },
			{ "mvKey_LMenu"					, 0xA4 },
			{ "mvKey_RMenu"					, 0xA5 },
			{ "mvKey_Browser_Back"			, 0xA6 },
			{ "mvKey_Browser_Forward"		, 0xA7 },
			{ "mvKey_Browser_Refresh"		, 0xA8 },
			{ "mvKey_Browser_Stop"			, 0xA9 },
			{ "mvKey_Browser_Search"		, 0xAA },
			{ "mvKey_Browser_Favorites"		, 0xAB },
			{ "mvKey_Browser_Home"			, 0xAC },
			{ "mvKey_Volume_Mute"			, 0xAD },
			{ "mvKey_Volume_Down"			, 0xAE },
			{ "mvKey_Volume_Up"				, 0xAF },
			{ "mvKey_Media_Next_Track"		, 0xB0 },
			{ "mvKey_Media_Prev_Track"		, 0xB1 },
			{ "mvKey_Media_Stop"			, 0xB2 },
			{ "mvKey_Media_Play_Pause"		, 0xB3 },
			{ "mvKey_Launch_Mail"			, 0xB4 },
			{ "mvKey_Launch_Media_Select"	, 0xB5 },
			{ "mvKey_Launch_App1"			, 0xB6 },
			{ "mvKey_Launch_App2"			, 0xB7 },
			{ "mvKey_Colon"					, 0xBA },
			{ "mvKey_Plus"					, 0xBB },
			{ "mvKey_Comma"					, 0xBC },
			{ "mvKey_Minus"					, 0xBD },
			{ "mvKey_Period"				, 0xBE },
			{ "mvKey_Slash"					, 0xBF },
			{ "mvKey_Tilde"					, 0xC0 },
			{ "mvKey_Open_Brace"			, 0xDB },
			{ "mvKey_Backslash"				, 0xDC },
			{ "mvKey_Close_Brace"			, 0xDD },
			{ "mvKey_Quote"					, 0xDE },



			//-----------------------------------------------------------------------------
			// Mouse Codes
			//-----------------------------------------------------------------------------
			{ "mvMouseButton_Left"  , 0 },
			{ "mvMouseButton_Right" , 1 },
			{ "mvMouseButton_Middle", 2 },
			{ "mvMouseButton_X1", 3 },
			{ "mvMouseButton_X2", 4 },

			//-----------------------------------------------------------------------------
			// Cardinal directions
			//-----------------------------------------------------------------------------
			{ "mvDir_None" ,-1 },
			{ "mvDir_Left" , 0 },
			{ "mvDir_Right", 1 },
			{ "mvDir_Up"   , 2 },
			{ "mvDir_Down" , 3 },

			//-----------------------------------------------------------------------------
			// Log Levels
			//-----------------------------------------------------------------------------
			{ "mvTRACE"   , 0 },
			{ "mvDEBUG"   , 1 },
			{ "mvINFO"    , 2 },
			{ "mvWARNING" , 3 },
			{ "mvERROR"   , 4 },
			{ "mvOFF"     , 5 }
		};

		return ModuleConstants;
	}

	static PyMethodDef dearpyguimethods[]
	{
		ADD_PYTHON_FUNCTION(start_dearpygui)
		ADD_PYTHON_FUNCTION(start_dearpygui_editor)
		ADD_PYTHON_FUNCTION(start_dearpygui_docs)
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
		{
NULL, NULL, 0, NULL
}
	};

	static PyModuleDef dearpyguiModule = {
		PyModuleDef_HEAD_INIT, "dearpygui", NULL, -1, dearpyguimethods,
		NULL, NULL, NULL, NULL
	};

	PyMODINIT_FUNC PyInit_dearpygui(void)
	{
		PyObject* m;

		m = PyModule_Create(&dearpyguiModule);
		if (m == NULL)
			return NULL;

		auto constants = GetModuleConstants();

		for (auto& item : constants)
			PyModule_AddIntConstant(m, item.first.c_str(), item.second);

		auto MarvelError = PyErr_NewException("dearpygui.error", NULL, NULL);
		Py_XINCREF(MarvelError);
		if (PyModule_AddObject(m, "error", MarvelError) < 0) {
			Py_XDECREF(MarvelError);
			Py_CLEAR(MarvelError);
			Py_DECREF(m);
			return NULL;
		}

		return m;
	}

	void start_dearpygui_error()
	{
		PyErr_Print();

		// create window
		mvWindow* window = new mvWindowClass(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), false, true);
		window->show();
		window->run();
		delete window;
		delete mvApp::GetApp();

	}
}