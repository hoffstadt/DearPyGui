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
		std::string AppName = "Demo";
		std::string PathName = "";
		std::string LibraryPath = "";

		// flags
		bool documentation = false; // starts application with the documentation window shown
		bool editorMode    = false; // starts application in editor mode

	};

}