#include "mvInputInterface.h"
#include "Core/mvApp.h"

namespace Marvel {

	PyObject* addPlot(PyObject* self, PyObject* args)
	{
		const char* parent, * name;
		int width, height;

		PyArg_ParseTuple(args, "ssii", &parent, &name, &width, &height);
		mvApp::GetApp()->addPlot(std::string(parent), std::string(name), width, height);
		Py_INCREF(Py_None);
		return Py_None;
	}

	void CreatePlotInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(addPlot, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}