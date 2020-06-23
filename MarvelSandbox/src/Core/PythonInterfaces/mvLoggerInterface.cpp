#include "Core/PythonUtilities/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvAppLog.h"
#include "Core/PythonUtilities/mvPythonTranslator.h"
#include "mvLoggerInterface.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = BuildTranslations();

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

	void CreateLoggerInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(ClearLog, "Not Documented");
		pyModule.addMethod(showLogger, "Not Documented");

		pyModule.addMethodD(SetLogLevel);
		pyModule.addMethodD(Log);
		pyModule.addMethodD(LogDebug);
		pyModule.addMethodD(LogInfo);
		pyModule.addMethodD(LogWarning);
		pyModule.addMethodD(LogError);
		
		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}