#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvPythonTranslator.h"

namespace Marvel {

	PyObject* addDrawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;
		const char* name;
		int width;
		int height;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Integer, "width"),
			mvPythonDataElement(mvPythonDataType::Integer, "height")
			});

		if (!pl.parse(__FUNCTION__, &parent, &name, &width, &height))
			return Py_None;

		mvApp::GetApp()->addDrawing(parent, name, width, height);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* drawLine(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		int x1, y1, x2, y2, thickness;
		PyObject* color;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "drawing"),
			mvPythonDataElement(mvPythonDataType::Integer, "x1"),
			mvPythonDataElement(mvPythonDataType::Integer, "y1"),
			mvPythonDataElement(mvPythonDataType::Integer, "x2"),
			mvPythonDataElement(mvPythonDataType::Integer, "y2"),
			mvPythonDataElement(mvPythonDataType::FloatList, "color"),
			mvPythonDataElement(mvPythonDataType::Integer, "thickness")
			});

		if (!pl.parse(__FUNCTION__, &drawing, &x1, &y1, &x2, &y2, &color, &thickness))
			return Py_None;


		auto acolor = pl.getFloatVec(color);

		mvApp::GetApp()->drawLine(drawing, x1, y1, x2, y2, {acolor[0], acolor[1], acolor[2], acolor[3], true}, thickness);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* clearDrawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "drawing")
			});

		if (!pl.parse(__FUNCTION__, &drawing))
			return Py_None;



		mvApp::GetApp()->clearDrawing(drawing);

		Py_INCREF(Py_None);

		return Py_None;
	}

	void CreateDrawingInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(addDrawing, "Not Documented");
		pyModule.addMethod(drawLine, "Not Documented");
		pyModule.addMethod(clearDrawing, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}