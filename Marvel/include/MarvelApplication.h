#pragma once

//-----------------------------------------------------------------------------
// Application
//
//     - This class represents the actual overall application with the
//       following responsibilities:
//
//         * Command line parsing
//         * Initializing/Finalizing Python
//         * Importing the user's module
//         * Importing marvel modules
//         * Setting overall application mode (i.e. editor, documentation, etc.)
//     
//-----------------------------------------------------------------------------

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
		int  runDocMode      ();
		int  runRegularMode  ();
		void logInformation  ();
		void importModule    ();

		// logging
		static void setLogLevel(unsigned level);
		static void Log(const std::string& text, const std::string& level = "TRACE");
		static void LogDebug(const std::string& text);
		static void LogInfo(const std::string& text);
		static void LogWarning(const std::string& text);
		static void LogError(const std::string& text);
		static void ClearLog();
		static const char* getArgv0() { return s_argv0; }

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

		static const char* s_argv0;

		// options
		std::string AppName = "";
		std::string PathName = "";
		std::string LibraryPath = "";
		std::string theme = "Dark";

		// flags
		bool documentation = false; // starts application with the documentation window shown
		bool editorMode    = false; // starts application in editor mode

	};

}