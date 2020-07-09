#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "mvPythonTranslator.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbLog");

	std::map<std::string, mvPythonTranslator>& BuildLoggingInterface() {

		std::map<std::string, mvPythonTranslator>* translators = new std::map< std::string, mvPythonTranslator>{

			{"get_log_level", mvPythonTranslator({
			}, "Returns the log level.", "int")},

			{"clear_log", mvPythonTranslator({
			}, "Clears the logger.")},

			{"show_logger", mvPythonTranslator({
			}, "Shows the logging window.")},

			{"set_log_level", mvPythonTranslator({
				{mvPythonDataType::Integer, "level"}
			}, "Sets the log level.")},

			{"log", mvPythonTranslator({
				{mvPythonDataType::String, "message"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::String, "level"}
			}, "Logs a trace level log.")},

			{"log_debug", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, "Logs a debug level log.")},

			{"log_info", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, "Logs a info level log.")},

			{"log_warning", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, "Logs a warning level log.")},

			{"log_error", mvPythonTranslator({
				{mvPythonDataType::String, "message"}
			}, "Logs a error level log.")}

		};

		return *translators;
	}

	PyObject* show_logger(PyObject* self, PyObject* args)
	{
		mvApp::GetApp()->showStandardWindow("logger");
		Py_INCREF(Py_None);
		Py_RETURN_NONE;
	}

	

	PyObject* get_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("i", mvAppLog::getLogger()->getLogLevel());
	}

	PyObject* set_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int level;
		if(!Translators["set_log_level"].parse(args, kwargs,__FUNCTION__, &level))
			Py_RETURN_NONE;
		mvAppLog::getLogger()->setLogLevel(level);
		Py_RETURN_NONE;
	}

	PyObject* log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;
		const char* level = "TRACE";
		if(!Translators["log"].parse(args, kwargs,__FUNCTION__, &message, &level))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->Log(std::string(message), std::string(level));
		Py_RETURN_NONE;
	}

	PyObject* log_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["log_debug"].parse(args, kwargs,__FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogDebug(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* log_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["log_info"].parse(args, kwargs,__FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogInfo(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* log_warning(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["log_warning"].parse(args, kwargs,__FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogWarning(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* log_error(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["log_error"].parse(args, kwargs,__FUNCTION__, &message))
			Py_RETURN_NONE;

		mvAppLog::getLogger()->LogError(std::string(message));
		Py_RETURN_NONE;
	}

	PyObject* clear_log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvAppLog::getLogger()->ClearLog();
		Py_RETURN_NONE;
	}

	mvPythonModule* CreateLoggerInterface()
	{

		auto pyModule = new mvPythonModule("sbLog", {});

		pyModule->addMethodD(get_log_level);
		pyModule->addMethodD(clear_log);
		pyModule->addMethodD(show_logger);
		pyModule->addMethodD(set_log_level);
		pyModule->addMethodD(log);
		pyModule->addMethodD(log_debug);
		pyModule->addMethodD(log_info);
		pyModule->addMethodD(log_warning);
		pyModule->addMethodD(log_error);

		return pyModule;
	}
}