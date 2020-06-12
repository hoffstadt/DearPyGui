#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/mvPythonTranslator.h"
#include "mvLoggerInterface.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = BuildTranslations();

	PyObject* ShowLogger(PyObject* self, PyObject* args)
	{
		mvApp::GetApp()->showLogger();
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* SetLogLevel(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int level;

		if(!Translators["SetLogLevel"].parse(args, kwargs,__FUNCTION__, &level))
			return Py_None;

		mvApp::GetApp()->setLogLevel(level);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* Log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;
		const char* level = "TRACE";

		if(!Translators["Log"].parse(args, kwargs,__FUNCTION__, &message, &level))
			return Py_None;

		mvApp::GetApp()->Log(std::string(message), std::string(level));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* LogDebug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["LogDebug"].parse(args, kwargs,__FUNCTION__, &message))
			return Py_None;

		mvApp::GetApp()->LogDebug(std::string(message));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* LogInfo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["LogInfo"].parse(args, kwargs,__FUNCTION__, &message))
			return Py_None;

		mvApp::GetApp()->LogInfo(std::string(message));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* LogWarning(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["LogWarning"].parse(args, kwargs,__FUNCTION__, &message))
			return Py_None;

		mvApp::GetApp()->LogWarning(std::string(message));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* LogError(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;

		if (!Translators["LogError"].parse(args, kwargs,__FUNCTION__, &message))
			return Py_None;

		mvApp::GetApp()->LogError(std::string(message));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* ClearLog(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvApp::GetApp()->ClearLog();

		Py_INCREF(Py_None);

		return Py_None;
	}

	void CreateLoggerInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(ClearLog, "Not Documented");
		pyModule.addMethod(ShowLogger, "Not Documented");

		pyModule.addMethodD(SetLogLevel);
		pyModule.addMethodD(Log);
		pyModule.addMethodD(LogDebug);
		pyModule.addMethodD(LogInfo);
		pyModule.addMethodD(LogWarning);
		pyModule.addMethodD(LogError);
		
		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}