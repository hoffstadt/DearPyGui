#include "mvCallbackRegistry.h"
#include "mvProfiler.h"
#include "mvContext.h"
#include <chrono>
#include <iostream>
#include "mvItemRegistry.h"
#include "mvAppItemCommons.h"
#include "mvGlobalIntepreterLock.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

void mvRunTasks()
{

	while (!GContext->callbackRegistry->tasks.empty())
	{
		mvFunctionWrapper t;
		GContext->callbackRegistry->tasks.wait_and_pop(t);
		t();
	}
}

void mvFrameCallback(i32 frame)
{

	if (frame > GContext->callbackRegistry->highestFrame)
		return;

	if (GContext->callbackRegistry->frameCallbacks.count(frame) == 0)
		return;

	mvAddCallback(GContext->callbackRegistry->frameCallbacks[frame], frame, nullptr,
		GContext->callbackRegistry->frameCallbacksUserData[frame]);
}

bool mvRunCallbacks()
{
	GContext->callbackRegistry->running = true;

	mvGlobalIntepreterLock gil;

	while (GContext->callbackRegistry->running)
	{
		mvFunctionWrapper t2;
		Py_BEGIN_ALLOW_THREADS;
		GContext->callbackRegistry->calls.wait_and_pop(t2);
		Py_END_ALLOW_THREADS;
		t2();
		GContext->callbackRegistry->callCount--;
	}

	return true;
}

void mvAddCallback(PyObject* callable, mvUUID sender, PyObject* app_data, PyObject* user_data, bool decrementAppData)
{

	if (GContext->callbackRegistry->callCount > GContext->callbackRegistry->maxNumberOfCalls)
	{
		if (app_data != nullptr)
			Py_XDECREF(app_data);
		if (user_data != nullptr)
			Py_XDECREF(user_data);
		assert(false);
		return;
	}

	if (GContext->IO.manualCallbacks)
	{
		if (callable != nullptr)
			Py_XINCREF(callable);
		if (app_data != nullptr)
			Py_XINCREF(app_data);
		if (user_data != nullptr)
			Py_XINCREF(user_data);
		GContext->callbackRegistry->jobs.push_back({ sender, callable, app_data, user_data });
		return;
	}

	mvSubmitCallback([=]() {
		mvRunCallback(callable, sender, app_data, user_data, decrementAppData);
		});
}

void mvAddCallback(PyObject* callable, const std::string& sender, PyObject* app_data, PyObject* user_data)
{

	if (GContext->callbackRegistry->callCount > GContext->callbackRegistry->maxNumberOfCalls)
	{

		if (app_data != nullptr)
			Py_XDECREF(app_data);
		if (user_data != nullptr)
			Py_XDECREF(user_data);
		assert(false);
		return;
	}

	if (GContext->IO.manualCallbacks)
	{
		if (callable != nullptr)
			Py_XINCREF(callable);
		if (app_data != nullptr)
			Py_XINCREF(app_data);
		if (user_data != nullptr)
			Py_XINCREF(user_data);
		GContext->callbackRegistry->jobs.push_back({ 0, callable, app_data, user_data, sender });
		return;
	}

	mvSubmitCallback([=]() {
		mvRunCallback(callable, sender, app_data, user_data);
		});
}

void mvRunCallback(PyObject* callable, const std::string& sender, PyObject* app_data, PyObject* user_data)
{

	if (callable == nullptr)
	{
		//if (data != nullptr)
		//	Py_XDECREF(data);
		return;
	}

	if (!PyCallable_Check(callable))
	{
		if (app_data != nullptr)
			Py_XDECREF(app_data);
		if (user_data != nullptr)
			Py_XDECREF(user_data);
		mvThrowPythonError(mvErrorCode::mvNone, "Callable not callable.");
		PyErr_Print();
		return;
	}

	if (app_data == nullptr)
	{
		app_data = Py_None;
		Py_XINCREF(app_data);
	}
	Py_XINCREF(app_data);

	if (user_data == nullptr)
	{
		user_data = Py_None;
		Py_XINCREF(user_data);
	}
	Py_XINCREF(user_data);

	//PyErr_Clear();
	if (PyErr_Occurred())
		PyErr_Print();

	if (PyErr_Occurred())
		PyErr_Print();

	PyObject* fc = PyObject_GetAttrString(callable, "__code__");
	if (fc) {
		PyObject* ac = PyObject_GetAttrString(fc, "co_argcount");
		if (ac) {
			i32 count = PyLong_AsLong(ac);

			if (PyMethod_Check(callable))
				count--;

			if (count > 3)
			{
				mvPyObject pArgs(PyTuple_New(count));
				PyTuple_SetItem(pArgs, 0, ToPyString(sender));
				PyTuple_SetItem(pArgs, 1, app_data); // steals data, so don't deref
				PyTuple_SetItem(pArgs, 2, user_data); // steals data, so don't deref

				for (int i = 3; i < count; i++)
					PyTuple_SetItem(pArgs, i, GetPyNone());

				mvPyObject result(PyObject_CallObject(callable, pArgs));

				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();

			}
			else if (count == 3)
			{
				mvPyObject pArgs(PyTuple_New(3));
				PyTuple_SetItem(pArgs, 0, ToPyString(sender));
				PyTuple_SetItem(pArgs, 1, app_data); // steals data, so don't deref
				PyTuple_SetItem(pArgs, 2, user_data); // steals data, so don't deref

				mvPyObject result(PyObject_CallObject(callable, pArgs));

				pArgs.delRef();
				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();

			}
			else if (count == 2)
			{
				mvPyObject pArgs(PyTuple_New(2));
				PyTuple_SetItem(pArgs, 0, ToPyString(sender));
				PyTuple_SetItem(pArgs, 1, app_data); // steals data, so don't deref

				mvPyObject result(PyObject_CallObject(callable, pArgs));

				pArgs.delRef();
				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();

			}
			else if (count == 1)
			{
				mvPyObject pArgs(PyTuple_New(1));
				PyTuple_SetItem(pArgs, 0, ToPyString(sender));

				mvPyObject result(PyObject_CallObject(callable, pArgs));

				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();
			}
			else
			{
				mvPyObject result(PyObject_CallObject(callable, nullptr));

				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();


			}
			Py_DECREF(ac);
		}
		Py_DECREF(fc);
	}

}

void mvRunCallback(PyObject* callable, mvUUID sender, PyObject* app_data, PyObject* user_data, bool decrementAppData)
{

	if (callable == nullptr)
	{
		//if (data != nullptr)
		//	Py_XDECREF(data);
		return;
	}

	if (!PyCallable_Check(callable))
	{
		if (app_data != nullptr)
			Py_XDECREF(app_data);
		if (user_data != nullptr)
			Py_XDECREF(user_data);
		mvThrowPythonError(mvErrorCode::mvNone, "Callable not callable.");
		PyErr_Print();
		return;
	}

	if (app_data == nullptr)
	{
		app_data = Py_None;
		Py_XINCREF(app_data);
	}
	if(decrementAppData)
		Py_XINCREF(app_data);

	if (user_data == nullptr)
	{
		user_data = Py_None;
		Py_XINCREF(user_data);
	}
	Py_XINCREF(user_data);

	//PyErr_Clear();
	if (PyErr_Occurred())
		PyErr_Print();

	if (PyErr_Occurred())
		PyErr_Print();

	PyObject* fc = PyObject_GetAttrString(callable, "__code__");
	if (fc) {
		PyObject* ac = PyObject_GetAttrString(fc, "co_argcount");
		if (ac) {
			i32 count = PyLong_AsLong(ac);

			if (PyMethod_Check(callable))
				count--;

			if (count > 3)
			{
				mvPyObject pArgs(PyTuple_New(count));
				PyTuple_SetItem(pArgs, 0, ToPyUUID(sender));
				PyTuple_SetItem(pArgs, 1, app_data); // steals data, so don't deref
				PyTuple_SetItem(pArgs, 2, user_data); // steals data, so don't deref
					
				for (int i = 3; i < count; i++)
					PyTuple_SetItem(pArgs, i, GetPyNone());

				mvPyObject result(PyObject_CallObject(callable, pArgs));

				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();

			}
			else if (count == 3)
			{
				mvPyObject pArgs(PyTuple_New(3));
				PyTuple_SetItem(pArgs, 0, ToPyUUID(sender));
				PyTuple_SetItem(pArgs, 1, app_data); // steals data, so don't deref
				PyTuple_SetItem(pArgs, 2, user_data); // steals data, so don't deref

				mvPyObject result(PyObject_CallObject(callable, pArgs));

				pArgs.delRef();
				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();

			}
			else if (count == 2)
			{
				mvPyObject pArgs(PyTuple_New(2));
				PyTuple_SetItem(pArgs, 0, ToPyUUID(sender));
				PyTuple_SetItem(pArgs, 1, app_data); // steals data, so don't deref

				mvPyObject result(PyObject_CallObject(callable, pArgs));

				pArgs.delRef();
				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();

			}
			else if(count == 1)
			{
				mvPyObject pArgs(PyTuple_New(1));
				PyTuple_SetItem(pArgs, 0, ToPyUUID(sender));

				mvPyObject result(PyObject_CallObject(callable, pArgs));

				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();
			}
			else
			{
				mvPyObject result(PyObject_CallObject(callable, nullptr));

				// check if call succeeded
				if (!result.isOk())
					PyErr_Print();


			}
			Py_DECREF(ac);
		}
		Py_DECREF(fc);
	}

}