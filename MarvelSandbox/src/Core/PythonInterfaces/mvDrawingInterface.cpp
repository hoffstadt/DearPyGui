#include "Core/PythonUtilities/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvAppLog.h"
#include "Core/PythonUtilities/mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaces.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("DrawingInterface");

	std::map<std::string, mvPythonTranslator> BuildDrawingInterface() {

		std::map<std::string, mvPythonTranslator> translators = {

						{"addDrawing", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"}
			}, false, "Needs documentation")},

			{"drawImage", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::String, "file"},
				{mvPythonDataType::FloatList, "pmin"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "pmax"},
				{mvPythonDataType::FloatList, "uv_min"},
				{mvPythonDataType::FloatList, "uv_max"},
				{mvPythonDataType::IntList, "color"}
			}, false, "Needs documentation")},

			{"drawLine", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Integer, "thickness"},
			}, false, "Needs documentation")},

			{"drawTriangle", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::FloatList, "p3"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Needs documentation")},

			{"drawRectangle", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "pmin"},
				{mvPythonDataType::FloatList, "pmax"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "rounding"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Needs documentation")},

			{"drawQuad", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::FloatList, "p3"},
				{mvPythonDataType::FloatList, "p4"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Needs documentation")},

			{"drawText", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "pos"},
				{mvPythonDataType::String, "text"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Integer, "size"}
			}, false, "Needs documentation")},

			{"drawCircle", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "center"},
				{mvPythonDataType::Float, "radius"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "segments"},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::FloatList, "fill"}
			}, false, "Needs documentation")},

			{"drawPolyline", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "points"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "closed"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Needs documentation")},

			{"drawPolygon", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "points"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "fill"},
				{mvPythonDataType::Float, "thickness"}
			}, false, "Needs documentation")},

			{"drawBezierCurve", mvPythonTranslator({
				{mvPythonDataType::String, "drawing"},
				{mvPythonDataType::FloatList, "p1"},
				{mvPythonDataType::FloatList, "p2"},
				{mvPythonDataType::FloatList, "p3"},
				{mvPythonDataType::FloatList, "p4"},
				{mvPythonDataType::IntList, "color"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "thickness"},
				{mvPythonDataType::Integer, "segments"}
			}, false, "Needs documentation")},

			{"clearDrawing", mvPythonTranslator({
				{mvPythonDataType::String, "draw"}
			}, false, "Needs documentation")},

		};

		return translators;
	}

	PyObject* addDrawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width;
		int height;

		if (!Translators["addDrawing"].parse(args, kwargs,__FUNCTION__, &name, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDrawing("", name, width, height);
		mvApp::GetApp()->addItem(item);

		Py_RETURN_NONE;
	}

	PyObject* drawImage(PyObject* self, PyObject* args, PyObject* kwargs)
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

		if (!Translators["drawImage"].parse(args, kwargs, __FUNCTION__, &drawing, &file,
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

	PyObject* drawLine(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		int thickness;
		PyObject* p1, *p2;
		PyObject* color;

		if (!Translators["drawLine"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &color, &thickness))
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

	PyObject* drawTriangle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, *p3;
		PyObject* color;
		PyObject* fill = nullptr;

		if (!Translators["drawTriangle"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &color, &fill, &thickness))
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

	PyObject* drawRectangle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f, rounding = 0.0f;
		PyObject* pmin, * pmax;
		PyObject* color;
		PyObject* fill = nullptr;

		if (!Translators["drawRectangle"].parse(args, kwargs,__FUNCTION__, &drawing, &pmin, &pmax, &color, &fill, &rounding, &thickness))
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

	PyObject* drawQuad(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, *p3, *p4;
		PyObject* color;
		PyObject* fill = nullptr;

		if (!Translators["drawQuad"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &fill, &thickness))
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

	PyObject* drawText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* text;
		PyObject* pos;
		int size = 10;
		PyObject* color = nullptr;

		if (!Translators["drawText"].parse(args, kwargs,__FUNCTION__, &drawing, &pos, &text, &color, &size))
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

	PyObject* drawCircle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* center;
		float radius;
		PyObject* color;
		int segments = 12;
		float thickness = 1.0f;
		PyObject* fill = nullptr;

		if (!Translators["drawCircle"].parse(args, kwargs,__FUNCTION__, &drawing, &center, &radius, &color, &segments, &thickness, &fill))
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

	PyObject* drawPolyline(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* points;
		PyObject* color;
		int closed = false;
		float thickness = 1.0f;

		if (!Translators["drawPolyline"].parse(args, kwargs,__FUNCTION__, &drawing, &points, &color, &closed, &thickness))
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

	PyObject* drawPolygon(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* points;
		PyObject* color;
		PyObject* fill = nullptr;
		float thickness = 1.0f;

		if (!Translators["drawPolygon"].parse(args, kwargs,__FUNCTION__, &drawing, &points, &color, &fill, &thickness))
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

	PyObject* drawBezierCurve(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, * p3, * p4;
		PyObject* color;
		int segments = 0;

		if (!Translators["drawBezierCurve"].parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &thickness, &segments))
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

	PyObject* clearDrawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		if (!Translators["clearDrawing"].parse(args, kwargs,__FUNCTION__, &drawing))
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

	void CreateDrawingInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethodD(addDrawing);
		pyModule.addMethodD(drawImage);
		pyModule.addMethodD(drawLine);
		pyModule.addMethodD(drawTriangle);
		pyModule.addMethodD(drawRectangle);
		pyModule.addMethodD(drawQuad);
		pyModule.addMethodD(drawText);
		pyModule.addMethodD(drawCircle);
		pyModule.addMethodD(drawPolyline);
		pyModule.addMethodD(drawPolygon);
		pyModule.addMethodD(drawBezierCurve);
		pyModule.addMethodD(clearDrawing);

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}

}