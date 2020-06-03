#include "Core/mvPythonInterface.h"
#include "Core/mvWindow.h"
#include "Platform/Windows/mvWindowsWindow.h"
#include <iostream>

using namespace Marvel;

int main(int argc, char* argv[])
{
	const wchar_t* path;
	const char* module_name;

	mvWindow* window = new mvWindowsWindow();
	window->show();

	// get path
	if(argc < 2)
		path = L"python38.zip;../../../../MarvelSandbox";
	else if(argc == 2)
		path = L"python38.zip;";
	else
	{
		// Convert to a wchar_t*
		size_t origsize = strlen(argv[1]) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, argv[1], _TRUNCATE);
		std::wstring wpath = std::wstring(L"python38.zip;") + std::wstring(wcstring);
		path = wpath.c_str();
	}
		
	// add our custom module
	PyImport_AppendInittab("sandbox", &PyInit_emb);

	// set path and start the intpreter
	Py_SetPath(path);
	Py_Initialize();

	PyObject* catchingModule = PyImport_ImportModule("OutputCatching");

	// get module name
	if (argc < 2)
		module_name = "App";
	else if (argc == 2)
		module_name = argv[1];
	else
		module_name = argv[2];

	// get module
	PyObject* pModule = PyImport_ImportModule(module_name); // new reference

	// check module (if it exists)
	if (pModule == nullptr)
	{
		Py_Finalize();
		return 0;
	}

	// check if error occurred
	PyErr_Print();
	if (!PyErr_Occurred())
	{
		// returns the dictionary object representing the module namespace
		PyObject* pDict = PyModule_GetDict(pModule); // borrowed reference

		mvApp::GetApp()->setModuleDict(pDict);

		window->run();

		Py_XDECREF(pDict);
		Py_XDECREF(pModule);

	}
	else
	{
		PyObject* catcher = PyObject_GetAttrString(catchingModule, "catchOutErr");
		PyObject* output = PyObject_GetAttrString(catcher, "value");
		std::string results = PyUnicode_AsUTF8(output);
		std::cout << results << std::endl;
	}

	// shutdown the intpreter
	Py_Finalize();

}