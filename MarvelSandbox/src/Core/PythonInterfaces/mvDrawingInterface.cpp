#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"

namespace Marvel {

	PyObject* addDrawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width;
		int height;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Integer, "width"),
			mvPythonDataElement(mvPythonDataType::Integer, "height")
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &name, &width, &height))
			return Py_None;

		mvAppItem* item = new mvDrawing("", name, width, height);
		mvApp::GetApp()->addItem(item);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* drawLine(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		int thickness;
		PyObject* p1, *p2;
		PyObject* color;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p1"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p2"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Integer, "thickness")
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &color, &thickness))
			return Py_None;

		mvVec2 mp1 = pl.getVec2(p1);
		mvVec2 mp2 = pl.getVec2(p2);
		mvColor mcolor = pl.getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawLine(mp1, mp2, mcolor, thickness);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* drawTriangle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, *p3;
		PyObject* color;
		PyObject* fill = nullptr;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p1"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p2"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p3"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::FloatList, "fill"),
			mvPythonDataElement(mvPythonDataType::Float, "thickness")
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &color, &fill, &thickness))
			return Py_None;


		mvVec2 mp1 = pl.getVec2(p1);
		mvVec2 mp2 = pl.getVec2(p2);
		mvVec2 mp3 = pl.getVec2(p3);
		mvColor mcolor = pl.getColor(color);
		mvColor mfill = pl.getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawTriangle(mp1, mp2, mp3, mcolor, mfill, thickness);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* drawRectangle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f, rounding = 0.0f;
		PyObject* pmin, * pmax;
		PyObject* color;
		PyObject* fill = nullptr;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::FloatList, "pmin"),
			mvPythonDataElement(mvPythonDataType::FloatList, "pmax"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::FloatList, "fill"),
			mvPythonDataElement(mvPythonDataType::Float, "rounding"),
			mvPythonDataElement(mvPythonDataType::Float, "thickness")
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing, &pmin, &pmax, &color, &fill, &rounding, &thickness))
			return Py_None;


		mvVec2 mpmax = pl.getVec2(pmax);
		mvVec2 mpmin = pl.getVec2(pmin);
		mvColor mcolor = pl.getColor(color);
		mvColor mfill = pl.getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawRectangle(mpmin, mpmax, mcolor, mfill, rounding, thickness);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* drawQuad(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, *p3, *p4;
		PyObject* color;
		PyObject* fill = nullptr;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p1"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p2"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p3"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p4"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::FloatList, "fill"),
			mvPythonDataElement(mvPythonDataType::Float, "thickness")
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &fill, &thickness))
			return Py_None;


		mvVec2 mp1 = pl.getVec2(p1);
		mvVec2 mp2 = pl.getVec2(p2);
		mvVec2 mp3 = pl.getVec2(p3);
		mvVec2 mp4 = pl.getVec2(p4);
		mvColor mcolor = pl.getColor(color);
		mvColor mfill = pl.getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawQuad(mp1, mp2, mp3, mp4, mcolor, mfill, thickness);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* drawText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* text;
		PyObject* pos;
		int size = 10;
		PyObject* color = nullptr;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::FloatList, "pos"),
			mvPythonDataElement(mvPythonDataType::String, "text"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Integer, "size")
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing, &pos, &text, &color, &size))
			return Py_None;

		mvVec2 mpos = pl.getVec2(pos);
		mvColor mcolor = pl.getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawText(mpos, text, mcolor, size);

		Py_INCREF(Py_None);

		return Py_None;
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

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::FloatList, "center"),
			mvPythonDataElement(mvPythonDataType::Float, "radius"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "segments"),
			mvPythonDataElement(mvPythonDataType::Float, "thickness"),
			mvPythonDataElement(mvPythonDataType::FloatList, "fill")
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing, &center, &radius, &color, &segments, &thickness, &fill))
			return Py_None;

		mvVec2 mcenter = pl.getVec2(center);
		mvColor mcolor = pl.getColor(color);
		mvColor mfill = pl.getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawCircle(mcenter, radius, mcolor, segments, thickness, mfill);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* drawPolyline(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* points;
		PyObject* color;
		int closed = false;
		float thickness = 1.0f;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::FloatList, "points"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Integer, "closed"),
			mvPythonDataElement(mvPythonDataType::Float, "thickness"),
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing, &points, &color, &closed, &thickness))
			return Py_None;

		auto mpoints = pl.getVectVec2(points);
		mvColor mcolor = pl.getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawPolyline(mpoints, mcolor, closed, thickness);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* drawPolygon(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* points;
		PyObject* color;
		PyObject* fill = nullptr;
		float thickness = 1.0f;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::FloatList, "points"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::FloatList, "fill"),
			mvPythonDataElement(mvPythonDataType::Float, "thickness"),
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing, &points, &color, &fill, &thickness))
			return Py_None;


		auto mpoints = pl.getVectVec2(points);
		mvColor mcolor = pl.getColor(color);
		mvColor mfill = pl.getColor(fill);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawPolygon(mpoints, mcolor, mfill, thickness);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* drawBezierCurve(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, * p3, * p4;
		PyObject* color;
		int segments = 0;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p1"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p2"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p3"),
			mvPythonDataElement(mvPythonDataType::FloatList, "p4"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Optional, ""),
			mvPythonDataElement(mvPythonDataType::Float, "thickness"),
			mvPythonDataElement(mvPythonDataType::Integer, "segments")
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &thickness, &segments))
			return Py_None;


		mvVec2 mp1 = pl.getVec2(p1);
		mvVec2 mp2 = pl.getVec2(p2);
		mvVec2 mp3 = pl.getVec2(p3);
		mvVec2 mp4 = pl.getVec2(p4);
		mvColor mcolor = pl.getColor(color);

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->drawBezierCurve(mp1, mp2, mp3, mp4, mcolor, thickness, segments);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* clearDrawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		auto pl = mvPythonTranslator( {
			mvPythonDataElement(mvPythonDataType::String, "drawing")
			});

		if (!pl.parse(args, kwargs,__FUNCTION__, &drawing))
			return Py_None;

		mvAppItem* item = mvApp::GetApp()->getItem(drawing);
		mvDrawing* dwg = static_cast<mvDrawing*>(item);
		dwg->clear();

		Py_INCREF(Py_None);

		return Py_None;
	}

	void CreateDrawingInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(addDrawing, "Not Documented");
		pyModule.addMethod(drawLine, "Not Documented");
		pyModule.addMethod(drawTriangle, "Not Documented");
		pyModule.addMethod(drawRectangle, "Not Documented");
		pyModule.addMethod(drawQuad, "Not Documented");
		pyModule.addMethod(drawText, "Not Documented");
		pyModule.addMethod(drawCircle, "Not Documented");
		pyModule.addMethod(drawPolyline, "Not Documented");
		pyModule.addMethod(drawPolygon, "Not Documented");
		pyModule.addMethod(drawBezierCurve, "Not Documented");
		pyModule.addMethod(clearDrawing, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}