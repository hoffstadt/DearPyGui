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

		void showConsole     ();
		void hideConsole     ();
		int  parseCommandLine();
		void handlePaths     ();
		int  initializePython();
		int  run             ();
		int  runEditorMode   ();
		int  runErrorMode    ();
		int  runRegularMode  ();
		void logInformation  ();
		void importModule    ();

	public:

		int         argc;
		char**      argv;
		wchar_t*    program;
		bool        errorMode   = false;
		bool        regularMode = false;
		PyObject*   pModule     = nullptr;
		PyObject*   m           = nullptr;
		bool        ranFromVS   = false;
		std::string addedPath;

		// options
		std::string AppName = "App";
		std::string PathName = "";

		// flags
		bool logger        = false; // starts application with the logger shown
		bool metrics       = false; // starts application with the metrics window shown
		bool source        = false; // starts application with the source window shown
		bool documentation = false; // starts application with the documentation window shown
		bool editorMode    = false; // starts application in editor mode

	};

}