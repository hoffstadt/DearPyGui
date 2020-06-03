#include "Core/mvPythonInterface.h"
#include "Core/mvWindow.h"
#include "Platform/Windows/mvWindowsWindow.h"
#include <iostream>

using namespace Marvel;

int main(int argc, char* argv[])
{
	mvWindow* window = new mvWindowsWindow();

	window->show();
	if(argc < 2)
		Py_SetPath(L"python38.zip;../../../../MarvelSandbox");
	else if(argc == 2)
		Py_SetPath(L"python38.zip;");
	else
	{
		// Convert to a wchar_t*
		size_t origsize = strlen(argv[1]) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, argv[1], _TRUNCATE);
		//wcscat_s(wcstring, L" (wchar_t *)");
		std::wstring path = std::wstring(L"python38.zip;") + std::wstring(wcstring);
		std::wcout << path << std::endl;
		Py_SetPath(path.c_str());
	}
		

	PyImport_AppendInittab("sandbox", &PyInit_emb);

	// start the intpreter
	Py_Initialize();

	// import the module (python file) looking in the path
	PyObject* pModule;
	if (argc < 2)
		pModule = PyImport_ImportModule("App"); // new reference
	else if (argc == 2)
		pModule = PyImport_ImportModule(argv[1]); // new reference
	else
		pModule = PyImport_ImportModule(argv[2]); // new reference

	// returns the dictionary object representing the module namespace
	PyObject* pDict = PyModule_GetDict(pModule); // borrowed reference

	mvApp::GetApp()->setModuleDict(pDict);

	window->run();

	Py_XDECREF(pDict);
	Py_XDECREF(pModule);

	// shutdown the intpreter
	Py_Finalize();
}