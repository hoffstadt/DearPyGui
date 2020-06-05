#include "Core/mvPythonModule.h"
#include "Core/mvApp.h"

namespace Marvel {

	PyObject* TurnOffLogger(PyObject* self, PyObject* args)
	{

		mvApp::GetApp()->turnOffLogger();

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* TurnOnLogger(PyObject* self, PyObject* args)
	{

		mvApp::GetApp()->turnOnLogger();

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* SetLogLevel(PyObject* self, PyObject* args)
	{
		int level;

		PyArg_ParseTuple(args, "i", &level);

		mvApp::GetApp()->setLogLevel(level);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* Log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* message;
		const char* level = "TRACE";
		static const char* keywords[] = { "message", "level", NULL };

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|$s", const_cast<char**>(keywords), &message, &level))
			__debugbreak();

		mvApp::GetApp()->Log(std::string(message), std::string(level));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* LogDebug(PyObject* self, PyObject* args)
	{
		const char* message;

		PyArg_ParseTuple(args, "s", &message);

		mvApp::GetApp()->LogDebug(std::string(message));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* LogInfo(PyObject* self, PyObject* args)
	{
		const char* message;

		PyArg_ParseTuple(args, "s", &message);

		mvApp::GetApp()->LogInfo(std::string(message));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* LogWarning(PyObject* self, PyObject* args)
	{
		const char* message;

		PyArg_ParseTuple(args, "s", &message);

		mvApp::GetApp()->LogWarning(std::string(message));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* LogError(PyObject* self, PyObject* args)
	{
		const char* message;

		PyArg_ParseTuple(args, "s", &message);

		mvApp::GetApp()->LogError(std::string(message));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* ClearLog(PyObject* self, PyObject* args)
	{

		mvApp::GetApp()->ClearLog();

		Py_INCREF(Py_None);

		return Py_None;
	}

	void CreateLoggerInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		pyModule.addMethod(TurnOffLogger, "Not Documented");
		pyModule.addMethod(TurnOnLogger, "Not Documented");
		pyModule.addMethod(SetLogLevel, "Not Documented");
		pyModule.addMethod(Log, "Not Documented");
		pyModule.addMethod(LogDebug, "Not Documented");
		pyModule.addMethod(LogInfo, "Not Documented");
		pyModule.addMethod(LogWarning, "Not Documented");
		pyModule.addMethod(LogError, "Not Documented");
		pyModule.addMethod(ClearLog, "Not Documented");

		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}
}