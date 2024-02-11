#include "mvCallbackRegistry.h"
#include "mvProfiler.h"
#include "mvContext.h"
#include <chrono>
#include <iostream>
#include <utility>
#include "mvItemRegistry.h"
#include "mvAppItemCommons.h"
#include "mvPyUtils.h"


//-----------------------------------------------------------------------------
// mvCallbackWrapper
//-----------------------------------------------------------------------------

void mvCallbackWrapper::run(mvUUID sender, PyObject *appData)
{
	if (!callback) {
		return;
	}
	// Bump refs in case this object gets deleted before the callback's run!
	Py_XINCREF(callback);
	Py_XINCREF(userData);
	Py_XINCREF(appData);

	mvSubmitCallback([=]() {
		mvRunCallback(callback, sender, appData, userData);

		Py_XDECREF(callback);
		Py_XDECREF(userData);
		Py_XDECREF(appData);
		});
}

void mvCallbackWrapper::run_blocking(mvUUID sender, PyObject *appData)
{
	if (!callback) {
		return;
	}

	mvRunCallback(callback, sender, appData, userData);
}

//-----------------------------------------------------------------------------
// mvCallbackPythonSlot
//-----------------------------------------------------------------------------

PyObject* mvCallbackPythonSlot::set_from_python(PyObject* self, PyObject* args, PyObject* kwargs)
{
	PyObject* callback;
	PyObject* user_data = nullptr;
	
	if (!Parse((GetParsers())[this->pythonName], args, kwargs, this->pythonName, &callback, &user_data))
		return GetPyNone();

	auto wrapper_ptr = std::make_shared<mvCallbackWrapper>(SanitizeCallback(callback), user_data);

	mvSubmitCallback([=]() mutable
		{
			this->callbackWrapper = std::move(*wrapper_ptr.get());
		});

	return GetPyNone();
}

//-----------------------------------------------------------------------------
// mvCallbackJob
//-----------------------------------------------------------------------------

mvCallbackJob::mvCallbackJob(PyObject* callback, PyObject* app_data, PyObject* user_data)
	: callback(callback), app_data(app_data), user_data(user_data)
{
	if (!this->callback)
		this->callback = Py_None;
	if (!this->app_data)
		this->app_data = Py_None;
	if (!this->user_data)
		this->user_data = Py_None;

	Py_XINCREF(this->callback);
	Py_XINCREF(this->app_data);
	Py_XINCREF(this->user_data);
	this->valid = true;
}

mvCallbackJob::mvCallbackJob(PyObject* callback, mvUUID sender, PyObject* app_data, PyObject* user_data)
	: mvCallbackJob(callback, app_data, user_data)
{
	this->sender = sender;
}

mvCallbackJob::mvCallbackJob(PyObject* callback, std::string sender, PyObject* app_data, PyObject* user_data)
	: mvCallbackJob(callback, app_data, user_data)
{
	this->sender = 0;
	this->sender_str = sender;
}

mvCallbackJob::~mvCallbackJob()
{
	Py_XDECREF(callback);
	Py_XDECREF(app_data);
	Py_XDECREF(user_data);
}

PyObject* mvCallbackJob::to_python_tuple(mvCallbackJob&& job)
{
	if (!job.is_valid())
		mvThrowPythonError(mvErrorCode::mvNone, "Trying to convert invalid callback job to PyObject.");
		PyErr_Print();
		return nullptr;

	PyObject* job_py = PyTuple_New(4);
	PyTuple_SetItem(job_py, 0, job.callback);

	if (job.sender == 0)
		PyTuple_SetItem(job_py, 1, ToPyString(job.sender_str));
	else
		PyTuple_SetItem(job_py, 1, ToPyUUID(job.sender));

	PyTuple_SetItem(job_py, 2, job.app_data);
	PyTuple_SetItem(job_py, 3, job.user_data);

	job.callback = nullptr;
	job.app_data = nullptr;
	job.user_data = nullptr;
	job.valid = false;

	return job_py;
}

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------

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

	mvAddCallback(GContext->callbackRegistry->frameCallbacks[frame], static_cast<mvUUID>(frame), nullptr,
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

template <typename SENDER>
PyObject* SenderToPyObject(const SENDER& sender);

template <>
PyObject* SenderToPyObject<mvUUID>(const mvUUID& sender)
{
	return ToPyUUID(sender);
}

template <>
PyObject* SenderToPyObject<std::string>(const std::string& sender)
{
	return ToPyString(sender);
}

template <typename SENDER>
static void _mvAddCallback(PyObject* callable, SENDER sender, PyObject* app_data, PyObject* user_data)
{

	if (GContext->callbackRegistry->callCount > GContext->callbackRegistry->maxNumberOfCalls)
	{
		Py_XDECREF(app_data);
		Py_XDECREF(user_data);
		assert(false);
		return;
	}

	if (GContext->IO.manualCallbacks)
	{
		Py_XINCREF(callable);
		Py_XINCREF(app_data);
		Py_XINCREF(user_data);
		GContext->callbackRegistry->jobs.emplace_back(callable, sender, app_data, user_data);
	}
	else {
		mvSubmitCallback([=]() {
			mvRunCallback(callable, sender, app_data, user_data);
			});
	}
}

void mvAddCallback(PyObject* callable, mvUUID sender, PyObject* app_data, PyObject* user_data)
{
	_mvAddCallback(callable, sender, app_data, user_data);
}

void mvAddCallback(PyObject* callable, const std::string& sender, PyObject* app_data, PyObject* user_data)
{
	_mvAddCallback(callable, sender, app_data, user_data);
}

template <typename SENDER>
static void _mvRunCallback(PyObject* callable, SENDER sender, PyObject* app_data, PyObject* user_data)
{

	if (callable == nullptr)
	{
		return;
	}

	if (!PyCallable_Check(callable))
	{
		Py_XDECREF(app_data);
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

	if (PyErr_Occurred())
		PyErr_Print();

	// Is this called twice in case the last PyErr_Print() errored?
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
				PyTuple_SetItem(pArgs, 0, SenderToPyObject(sender));
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
				PyTuple_SetItem(pArgs, 0, SenderToPyObject(sender));
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
				PyTuple_SetItem(pArgs, 0, SenderToPyObject(sender));
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
				PyTuple_SetItem(pArgs, 0, SenderToPyObject(sender));

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

void mvRunCallback(PyObject* callable, mvUUID sender, PyObject* app_data, PyObject* user_data)
{
	_mvRunCallback(callable, sender, app_data, user_data);
}

void mvRunCallback(PyObject* callable, const std::string& sender, PyObject* app_data, PyObject* user_data)
{
	_mvRunCallback(callable, sender, app_data, user_data);
}
