#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>

namespace Marvel {

	class Application
	{

	public:

		Application(const char* name, int argc, char* argv[]);
		~Application();

		void showConsole();
		void hideConsole();
		int  parseCommandLine();
		void handlePaths();
		int  initializePython();
		int  run();
		int  runEditorMode();
		int  runErrorMode();
		int  runRegularMode();
		void logInformation();
		void importModule();

	public:

		int         argc;
		char**      argv;
		wchar_t*    program;
		bool        errorMode = false;
		bool        regularMode = false;
		PyObject*   pModule = nullptr;
		PyObject*   m = nullptr;
		bool        ranFromVS = false;
		std::string addedPath;

		// options
		std::string AppName = "App";
		std::string PathName = "";

		// flags
		bool logger = false;
		bool metrics = false;
		bool source = false;
		bool documentation = false;
		bool editorMode = false;

	};

}