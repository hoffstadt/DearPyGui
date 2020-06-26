#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvAppLog.h"
#include "mvPythonTranslator.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbLog");

	std::map<std::string, mvPythonTranslator> BuildLoggingInterface() {

		std::map<std::string, mvPythonTranslator> translators = {

			{"ClearLog", mvPythonTranslator({
			}, false, "Needs documentation")},

			{"showLogger", mvPythonTranslator({
			}, false, "Needs documentation")},

			{"SetLogLevel", mvPythonTranslator({
				{mvPythonDataType::Integer, "level"}
			}, false, "Needs documentation")},

			{"Log", mvPythonTranslator({
				{mvPythonDataType::String, "message"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::String, "level"}
			}, false, "Needs documentation")},

			{"LogDebug", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, false, "Needs documentation")},

			{"LogInfo", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, false, "Needs documentation")},

			{"LogWarning", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, false, "Needs documentation")},

			{"LogError", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, false, "Needs documentation")}

		};

		return translators;
	}

	PyObject* showLogger(PyObject* self, PyObject* args)
	{
		mvApp::GetApp()->showLogger();
		Py_INCREF(Py_None);
		Py_RETURN_NONE;
	}

	PyObject* SetLogLevel(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int level;
		if(!Translators["SetLogLevel"].parse(args, kwargs,__FUNCTION__, &level))
			Py_RETURN_NONE;
		mvAppLog::getLogger()->setLogLevel(level);
		Py_RETURN_NONE;
	}

	PyObject* Log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;
		const char* level = "TRACE";
		if(!Translators["Log"].parse(args, kwargs,__FUNCTION__, &message, &level))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->Log(std::string(message), std::string(level));
		Py_RETURN_NONE;
	}

	PyObject* LogDebug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["LogDebug"].parse(args, kwargs,__FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogDebug(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* LogInfo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["LogInfo"].parse(args, kwargs,__FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogInfo(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* LogWarning(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["LogWarning"].parse(args, kwargs,__FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogWarning(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* LogError(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["LogError"].parse(args, kwargs,__FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogError(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* ClearLog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppLog::getLogger()->ClearLog();
		Py_RETURN_NONE;
	}

	mvPythonModule* CreateLoggerInterface()
	{

		auto pyModule = new mvPythonModule("sbLog", {});

		pyModule->addMethodD(ClearLog);
		pyModule->addMethodD(showLogger);
		pyModule->addMethodD(SetLogLevel);
		pyModule->addMethodD(Log);
		pyModule->addMethodD(LogDebug);
		pyModule->addMethodD(LogInfo);
		pyModule->addMethodD(LogWarning);
		pyModule->addMethodD(LogError);

		return pyModule;
	}
}