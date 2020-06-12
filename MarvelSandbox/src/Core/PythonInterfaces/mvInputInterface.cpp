#include "Core/mvPythonModule.h"
#include "Core/mvPythonTranslator.h"
#include "Core/mvApp.h"
#include "mvInputInterface.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = BuildTranslations();


	PyObject* getMousePos(PyObject* self, PyObject* args)
	{
		mvMousePos pos = mvApp::GetApp()->getMousePosition();
		PyObject* pvalue = Py_BuildValue("(ff)", pos.x, pos.y);
		return pvalue;
	}

	PyObject* isMouseButtonPressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Translators["isMouseButtonPressed"].parse(args, kwargs,__FUNCTION__, &button))
			Py_RETURN_NONE;

		bool pressed = mvApp::GetApp()->isMouseButtonPressed(button);

		PyObject* pvalue = Py_BuildValue("i", pressed);

		return pvalue;
	}

	PyObject* isKeyPressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Translators["isMouseButtonPressed"].parse(args, kwargs,__FUNCTION__, &key))
			Py_RETURN_NONE;

		bool pressed = mvApp::GetApp()->isKeyPressed(key);

		PyObject* pvalue = Py_BuildValue("i", pressed);

		return pvalue;
	}

	void CreateInputInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(getMousePos, "Not Documented");
		pyModule.addMethodD(isMouseButtonPressed);
		pyModule.addMethodD(isKeyPressed);

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}