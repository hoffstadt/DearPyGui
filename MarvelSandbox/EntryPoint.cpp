#include "Core/mvPythonInterface.h"
#include "Core/mvWindow.h"
#include "Platform/Windows/mvWindowsWindow.h"
#include <iostream>

using namespace Marvel;

int main()
{
	mvWindow* window = new mvWindowsWindow();

	window->show();

	Py_SetPath(L"python38.zip;../../../../MarvelSandbox");
	PyImport_AppendInittab("sandbox", &PyInit_emb);

	// start the intpreter
	Py_Initialize();

	// import the module (python file) looking in the path
	PyObject* pModule = PyImport_ImportModule("App"); // new reference

	// returns the dictionary object representing the module namespace
	PyObject* pDict = PyModule_GetDict(pModule); // borrowed reference

	mvApp::GetApp()->setModuleDict(pDict);

	window->run();

	Py_XDECREF(pDict);
	Py_XDECREF(pModule);

	// shutdown the intpreter
	Py_Finalize();
}