#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvAppLog.h"
#include "mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbDraw");

	std::map<std::string, mvPythonTranslator> BuildDrawingInterface() {

		std::map<std::string, mvPythonTranslator> translators = {

			{"add_drawing", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"}
			}, false, "Adds a drawing widget.")},

			{"set_drawing_size", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"}
			}, false, "Sets the size of a drawing widget.")},

			{"get_drawing_size", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
			}, false, "Returns the size of a drawing widget.", "(float, float)")},

			{"draw_image", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::String, "file"},
				{mvPythonDataType::FloatList, "pmin"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "pmax"},
				{mvPythonDataType::FloatList, "uv_min"},
				{mvPythonDataType::FloatList, "uv_max"},
				{mvPythonDataType::IntList, "color"}
			}, false, "Draws an image on a drawing.")},

			{"draw_line", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Integer, "thickness"},
			}, false, "Draws a line on a drawing.")},

			{"draw_triangle", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::FloatList, "p3"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Draws a triangle on a drawing.")},

			{"draw_rectangle", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "pmin"},
				{mvPythonDataType::FloatList, "pmax"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "rounding"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Draws a rectangle on a drawing.")},

			{"draw_quad", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::FloatList, "p3"},
				{mvPythonDataType::FloatList, "p4"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Draws a quad on a drawing.")},

			{"draw_text", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "pos"},
				{mvPythonDataType::String, "text"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Integer, "size"}
			}, false, "Draws text on a drawing.")},

			{"draw_circle", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "center"},
				{mvPythonDataType::Float, "radius"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "segments"},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::FloatList, "fill"}
			}, false, "Draws a circle on a drawing.")},

			{"draw_polyline", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "points"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "closed"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Draws lines on a drawing.")},

			{"draw_polygon", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "points"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Draws a polygon on a drawing.")},

			{"draw_bezier_curve", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::FloatList, "p3"},
				{mvPythonDataType::FloatList, "p4"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::Integer, "segments"}
			}, false, "Draws a bezier curve on a drawing.")},

			{"clear_drawing", mvPythonTranslator({
				{mvPythonDataType::String, "draw"}
			}, false, "Clears a drawing.")},

		};

		return translators;
	}

	PyObject* add_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width;
		int height;

		if (!Translators["add_drawing"].parse(args, kwargs,__FUNCTION__, &name, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDrawing("", name, width, height);
		mvApp::GetApp()->addItem(item);

		Py_RETURN_NONE;
	}

	PyObject* set_drawing_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width;
		int height;

		if (!Translators["set_drawing_size"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
			Py_RETURN_NONE;

		auto drawing = mvApp::GetApp()->getItem(name);

		if (drawing)
		{
			drawing->setWidth(width);
			drawing->setHeight(height);
		}

		Py_RETURN_NONE;
	}

	PyObject* get_drawing_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!Translators["get_drawing_size"].parse(args, kwargs, __FUNCTION__, &name))
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

		if (!Translators["draw_image"].parse(args, kwargs, __FUNCTION__, &drawing, &file,
			&pmin, &pmax, &uv_min, &uv_max, &color))
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
		dwg->drawImage(file, mpmin, mpmax, muv_min, muv_max, mcolor);

		Py_RETURN_NONE;
	}

	PyObject* draw_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		int thickness;
		PyObject* p1, *p2;
		PyObject* color;

		if (!Translators["draw_line"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &color, &thickness))
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
		dwg->drawLine(mp1, mp2, mcolor, thickness);

		Py_RETURN_NONE;
	}

	PyObject* draw_triangle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, *p3;
		PyObject* color;
		PyObject* fill = nullptr;

		if (!Translators["draw_triangle"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &color, &fill, &thickness))
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
		dwg->drawTriangle(mp1, mp2, mp3, mcolor, mfill, thickness);

		Py_RETURN_NONE;
	}

	PyObject* draw_rectangle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f, rounding = 0.0f;
		PyObject* pmin, * pmax;
		PyObject* color;
		PyObject* fill = nullptr;

		if (!Translators["draw_rectangle"].parse(args, kwargs,__FUNCTION__, &drawing, &pmin, &pmax, &color, &fill, &rounding, &thickness))
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
		dwg->drawRectangle(mpmin, mpmax, mcolor, mfill, rounding, thickness);

		Py_RETURN_NONE;
	}

	PyObject* draw_quad(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, *p3, *p4;
		PyObject* color;
		PyObject* fill = nullptr;

		if (!Translators["draw_quad"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &fill, &thickness))
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
		dwg->drawQuad(mp1, mp2, mp3, mp4, mcolor, mfill, thickness);

		Py_RETURN_NONE;
	}

	PyObject* draw_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* text;
		PyObject* pos;
		int size = 10;
		PyObject* color = nullptr;

		if (!Translators["draw_text"].parse(args, kwargs,__FUNCTION__, &drawing, &pos, &text, &color, &size))
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
		dwg->drawText(mpos, text, mcolor, size);

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

		if (!Translators["draw_circle"].parse(args, kwargs,__FUNCTION__, &drawing, &center, &radius, &color, &segments, &thickness, &fill))
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
		dwg->drawCircle(mcenter, radius, mcolor, segments, thickness, mfill);

		Py_RETURN_NONE;
	}

	PyObject* draw_polyline(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* points;
		PyObject* color;
		int closed = false;
		float thickness = 1.0f;

		if (!Translators["draw_polyline"].parse(args, kwargs,__FUNCTION__, &drawing, &points, &color, &closed, &thickness))
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
		dwg->drawPolyline(mpoints, mcolor, closed, thickness);

		Py_RETURN_NONE;
	}

	PyObject* draw_polygon(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* points;
		PyObject* color;
		PyObject* fill = nullptr;
		float thickness = 1.0f;

		if (!Translators["draw_polygon"].parse(args, kwargs,__FUNCTION__, &drawing, &points, &color, &fill, &thickness))
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
		dwg->drawPolygon(mpoints, mcolor, mfill, thickness);

		Py_RETURN_NONE;
	}

	PyObject* draw_bezier_curve(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, * p3, * p4;
		PyObject* color;
		int segments = 0;

		if (!Translators["draw_bezier_curve"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &thickness, &segments))
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
		dwg->drawBezierCurve(mp1, mp2, mp3, mp4, mcolor, thickness, segments);

		Py_RETURN_NONE;
	}

	PyObject* clear_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		if (!Translators["clear_drawing"].parse(args, kwargs,__FUNCTION__, &drawing))
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