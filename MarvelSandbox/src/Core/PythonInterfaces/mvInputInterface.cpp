#include "Core/mvPythonModule.h"
#include "Core/mvPythonTranslator.h"
#include "Core/mvApp.h"

namespace Marvel {

	PyObject* getMousePos(PyObject* self, PyObject* args)
	{
		mvMousePos pos = mvApp::GetApp()->getMousePosition();
		PyObject* pvalue = Py_BuildValue("(ff)", pos.x, pos.y);
		return pvalue;
	}

	PyObject* isMouseButtonPressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::Integer, "button")
			});

		if (!pl.parse(__FUNCTION__, &button))
			return Py_None;

		bool pressed = mvApp::GetApp()->isMouseButtonPressed(button);

		PyObject* pvalue = Py_BuildValue("i", pressed);

		return pvalue;
	}

	PyObject* isKeyPressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::Integer, "key")
			});

		if (!pl.parse(__FUNCTION__, &key))
			return Py_None;

		bool pressed = mvApp::GetApp()->isKeyPressed(key);

		PyObject* pvalue = Py_BuildValue("i", pressed);

		return pvalue;
	}

	void CreateInputInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(getMousePos, "Not Documented");
		pyModule.addMethod(isMouseButtonPressed, "Not Documented");
		pyModule.addMethod(isKeyPressed, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}