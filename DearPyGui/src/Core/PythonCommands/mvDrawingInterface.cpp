#include "mvDrawingInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddDrawingCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_drawing", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
		}, "Adds a drawing widget.", "None", "Drawing") });

		parsers->insert({ "delete_drawing_item", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "tag"},
		}, "Deletes a drawing item.", "None", "Drawing") });

		parsers->insert({ "draw_image", mvPythonParser({
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
		}, ("Draws an image on a drawing. p_min and p_max represent the upper-left and lower-right corners of the rectangle."
			"uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using (0,0)->(1,1) texture"
			"coordinates will generally display the entire texture."), "None", "Drawing") });

		parsers->insert({ "draw_line", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "thickness"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tag"},
		}, "Draws a line on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_arrow", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "thickness"},
			{mvPythonDataType::Integer, "size"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tag"},
		}, "Draws an arrow on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_triangle", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::FloatList, "p3"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a triangle on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_rectangle", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "pmin"},
			{mvPythonDataType::FloatList, "pmax"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::Float, "rounding"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a rectangle on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_quad", mvPythonParser({
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
		}, "Draws a quad on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_text", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "pos"},
			{mvPythonDataType::String, "text"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "size"},
			{mvPythonDataType::String, "tag"},
		}, "Draws text on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_circle", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "center"},
			{mvPythonDataType::Float, "radius"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "segments"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a circle on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_polyline", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "points"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "closed"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::String, "tag"},
		}, "Draws lines on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_polygon", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "points"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "fill"},
			{mvPythonDataType::Float, "thickness"},
			{mvPythonDataType::String, "tag"},
		}, "Draws a polygon on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_bezier_curve", mvPythonParser({
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
		}, "Draws a bezier curve on a drawing.", "None", "Drawing") });

		parsers->insert({ "clear_drawing", mvPythonParser({
			{mvPythonDataType::String, "draw"}
		}, "Clears a drawing.", "None", "Drawing") });
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
			return ToPyBool(false);

		mvAppItem* item = new mvDrawing(name, width, height);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);

		if (!item)
			return ToPyBool(false);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* delete_drawing_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(*mvApp::GetApp()->getParsers())["delete_drawing_item"].parse(args, kwargs, __FUNCTION__, &drawing, &tag))
			return GetPyNone();

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			ThrowPythonException("Drawing does not exist");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}

		dwg->deleteCommand(tag);

		return GetPyNone();
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
			return GetPyNone();

		mvVec2 mpmin = ToVec2(pmin);
		mvVec2 mpmax = ToVec2(pmax);
		mvVec2 muv_min = ToVec2(uv_min);
		mvVec2 muv_max = ToVec2(uv_max);
		mvColor mcolor = ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawImage(file, mpmin, mpmax, muv_min, muv_max, mcolor, tag);

		return GetPyNone();
	}

	PyObject* draw_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		int thickness;
		PyObject* p1, * p2;
		PyObject* color;
		const char* tag = "";

		if (!(*mvApp::GetApp()->getParsers())["draw_line"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &color, &thickness, &tag))
			return GetPyNone();

		mvVec2 mp1 = ToVec2(p1);
		mvVec2 mp2 = ToVec2(p2);
		mvColor mcolor = ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawLine(mp1, mp2, mcolor, thickness, tag);

		return GetPyNone();
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
			return GetPyNone();

		mvVec2 mp1 = ToVec2(p1);
		mvVec2 mp2 = ToVec2(p2);
		mvColor mcolor = ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawArrow(mp1, mp2, mcolor, thickness, size, tag);

		return GetPyNone();
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
			return GetPyNone();


		mvVec2 mp1 = ToVec2(p1);
		mvVec2 mp2 = ToVec2(p2);
		mvVec2 mp3 = ToVec2(p3);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawTriangle(mp1, mp2, mp3, mcolor, mfill, thickness, tag);

		return GetPyNone();
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
			return GetPyNone();


		mvVec2 mpmax = ToVec2(pmax);
		mvVec2 mpmin = ToVec2(pmin);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawRectangle(mpmin, mpmax, mcolor, mfill, rounding, thickness, tag);

		return GetPyNone();
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
			return GetPyNone();


		mvVec2 mp1 = ToVec2(p1);
		mvVec2 mp2 = ToVec2(p2);
		mvVec2 mp3 = ToVec2(p3);
		mvVec2 mp4 = ToVec2(p4);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawQuad(mp1, mp2, mp3, mp4, mcolor, mfill, thickness, tag);

		return GetPyNone();
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
			return GetPyNone();

		mvVec2 mpos = ToVec2(pos);
		mvColor mcolor = ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawText(mpos, text, mcolor, size, tag);

		return GetPyNone();
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
			return GetPyNone();

		mvVec2 mcenter = ToVec2(center);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawCircle(mcenter, radius, mcolor, segments, thickness, mfill, tag);

		return GetPyNone();
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
			return GetPyNone();

		auto mpoints = ToVectVec2(points);
		mvColor mcolor = ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawPolyline(mpoints, mcolor, closed, thickness, tag);

		return GetPyNone();
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
			return GetPyNone();

		auto mpoints = ToVectVec2(points);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawPolygon(mpoints, mcolor, mfill, thickness, tag);

		return GetPyNone();
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
			return GetPyNone();

		mvVec2 mp1 = ToVec2(p1);
		mvVec2 mp2 = ToVec2(p2);
		mvVec2 mp3 = ToVec2(p3);
		mvVec2 mp4 = ToVec2(p4);
		mvColor mcolor = ToColor(color);

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->drawBezierCurve(mp1, mp2, mp3, mp4, mcolor, thickness, segments, tag);

		return GetPyNone();
	}

	PyObject* clear_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		if (!(*mvApp::GetApp()->getParsers())["clear_drawing"].parse(args, kwargs, __FUNCTION__, &drawing))
			return GetPyNone();

		auto item = mvApp::GetApp()->getItem(drawing);

		if (item == nullptr)
		{
			std::string message = drawing;
			ThrowPythonException(message + " drawing does not exist.");
			return GetPyNone();
		}

		mvDrawing* dwg;
		if (item->getType() == mvAppItemType::Drawing)
			dwg = static_cast<mvDrawing*>(item);
		else
		{
			ThrowPythonException(std::string(drawing) + " is not a drawing.");
			return GetPyNone();
		}
		dwg->clear();

		return GetPyNone();
	}

}