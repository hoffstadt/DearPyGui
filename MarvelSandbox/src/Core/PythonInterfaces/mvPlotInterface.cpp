#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvPythonTranslator.h"

namespace Marvel {

	PyObject* addPlot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, * name;
		int width, height;

		auto pl = mvPythonTranslator(args, kwargs, {
			mvPythonDataElement(mvPythonDataType::String, "parent"),
			mvPythonDataElement(mvPythonDataType::String, "name"),
			mvPythonDataElement(mvPythonDataType::Integer, "width"),
			mvPythonDataElement(mvPythonDataType::Integer, "height")
			});

		pl.parse(__FUNCTION__, &parent, &name, &width, &height);

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