#include "mvInputInterface.h"
#include "Core/mvApp.h"

namespace Marvel {

	PyObject* getMousePos(PyObject* self, PyObject* args)
	{
		mvMousePos pos = mvApp::GetApp()->getMousePosition();
		PyObject* pvalue = Py_BuildValue("(ff)", pos.x, pos.y);
		return pvalue;
	}

	void CreateInputInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(getMousePos, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}