#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "mvPythonParser.h"
#include "mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonParser> Parsers = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbDraw");

	std::map<std::string, mvPythonParser>& BuildDrawingInterface() {

		std::map<std::string, mvPythonParser>* parsers = new std::map< std::string, mvPythonParser>{

			{"add_drawing", mvPythonParser({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
				{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
				{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
				{mvPythonDataType::Integer, "width",""},
				{mvPythonDataType::Integer, "height",""},
			}, "Adds a drawing widget.")},

			{"set_drawing_size", mvPythonParser({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"}
			}, "Sets the size of a drawing widget.")},

			{"set_drawing_origin", mvPythonParser({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Float, "x"},
				{mvPythonDataType::Float, "y"}
			}, "Sets the drawing origin (default is 0,0).")},

			{"set_drawing_scale", mvPythonParser({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Float, "x"},
				{mvPythonDataType::Float, "y"}
			}, "Sets the drawing scale (default is (1,1)).")},

			{"get_drawing_origin", mvPythonParser({
				{mvPythonDataType::String, "name"},
			}, "Returns the drawing origin.", "(float, float)")},

			{"get_drawing_scale", mvPythonParser({
				{mvPythonDataType::String, "name"},
			}, "Returns the drawing scale.", "(float, float)")},

			{"get_drawing_size", mvPythonParser({
				{mvPythonDataType::String, "name"},
			}, "Returns the size of a drawing widget.", "(float, float)")},

			{"draw_image", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::String, "file"},
				{mvPythonDataType::FloatList, "pmin"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "pmax"},
				{mvPythonDataType::FloatList, "uv_min"},
				{mvPythonDataType::FloatList, "uv_max"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::String, "tag"},
			}, "Draws an image on a drawing.")},

			{"draw_line", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Integer, "thickness"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::String, "tag"},
			}, "Draws a line on a drawing.")},

			{"draw_arrow", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Integer, "thickness"},
				{mvPythonDataType::Integer, "size"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::String, "tag"},
			}, "Draws an arrow on a drawing.")},

			{"draw_triangle", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::FloatList, "p3"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::String, "tag"},
			}, "Draws a triangle on a drawing.")},

			{"draw_rectangle", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "pmin"},
				{mvPythonDataType::FloatList, "pmax"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "rounding"},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::String, "tag"},
			}, "Draws a rectangle on a drawing.")},

			{"draw_quad", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::FloatList, "p3"},
				{mvPythonDataType::FloatList, "p4"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::String, "tag"},
			}, "Draws a quad on a drawing.")},

			{"draw_text", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "pos"},
				{mvPythonDataType::String, "text"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Integer, "size"},
				{mvPythonDataType::String, "tag"},
			}, "Draws text on a drawing.")},

			{"draw_circle", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "center"},
				{mvPythonDataType::Float, "radius"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "segments"},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::String, "tag"},
			}, "Draws a circle on a drawing.")},

			{"draw_polyline", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "points"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "closed"},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::String, "tag"},
			}, "Draws lines on a drawing.")},

			{"draw_polygon", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "points"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::String, "tag"},
			}, "Draws a polygon on a drawing.")},

			{"draw_bezier_curve", mvPythonParser({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::FloatList, "p3"},
				{mvPythonDataType::FloatList, "p4"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::Integer, "segments"},
				{mvPythonDataType::String, "tag"},
			}, "Draws a bezier curve on a drawing.")},

			{"clear_drawing", mvPythonParser({
				{mvPythonDataType::String, "draw"}
			}, "Clears a drawing.")},

		};

		return *parsers;
	}

	PyObject* add_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_drawing"].parse(args, kwargs,__FUNCTION__, &name, &tip, &parent, &before, &width, &height))
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
		PyObject* p1, *p2;
		PyObject* color;
		const char* tag = "";

		if (!Parsers["draw_line"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &color, &thickness, &tag))
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
		PyObject* p1, * p2, *p3;
		PyObject* color;
		PyObject* fill = nullptr;
		const char* tag = "";

		if (!Parsers["draw_triangle"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &color, &fill, &thickness, &tag))
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

		if (!Parsers["draw_rectangle"].parse(args, kwargs,__FUNCTION__, &drawing, &pmin, &pmax, &color, &fill, &rounding, &thickness, &tag))
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
		PyObject* p1, * p2, *p3, *p4;
		PyObject* color;
		PyObject* fill = nullptr;
		const char* tag = "";

		if (!Parsers["draw_quad"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &fill, &thickness, &tag))
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

		if (!Parsers["draw_text"].parse(args, kwargs,__FUNCTION__, &drawing, &pos, &text, &color, &size, &tag))
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

		if (!Parsers["draw_circle"].parse(args, kwargs,__FUNCTION__, &drawing, &center, &radius, &color, &segments, &thickness, &fill, &tag))
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

		if (!Parsers["draw_polyline"].parse(args, kwargs,__FUNCTION__, &drawing, &points, &color, &closed, &thickness, &tag))
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

		if (!Parsers["draw_polygon"].parse(args, kwargs,__FUNCTION__, &drawing, &points, &color, &fill, &thickness, &tag))
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

		if (!Parsers["draw_bezier_curve"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &thickness, &segments, &tag))
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

		if (!Parsers["clear_drawing"].parse(args, kwargs,__FUNCTION__, &drawing))
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

	mvPythonModule* CreateDrawingInterface()
	{
		auto pyModule = new mvPythonModule("sbDraw", {});

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

		return pyModule;
	}

}