#include "mvCallbackRegistry.h"
#include "mvProfiler.h"
#include "mvContext.h"
#include <chrono>
#include <iostream>
#include <utility>
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvAppItemCommons.h"
#include "mvPyUtils.h"


//-----------------------------------------------------------------------------
// mvCallbackPoint
//-----------------------------------------------------------------------------

void mvCallbackPoint::run(mvUUID sender, PyObject *appData)
{
	auto cwd = this->cwd.copy();
	if (appData)
		cwd.appData = mvPyObjectStrict(appData);

	mvSubmitRunCallbackJob({cwd.copy(), sender});
}

void mvCallbackPoint::run_blocking(mvUUID sender, PyObject *appData)
{
	auto cwd = this->cwd.copy();
	if (appData)
		cwd.appData = mvPyObjectStrict(appData);
	auto job = mvCallbackJob(std::move(cwd), sender);
	mvRunCallbackJob(std::move(job));
}

PyObject* mvCallbackPoint::set_from_python(PyObject* self, PyObject* args, PyObject* kwargs)
{
	PyObject* callback;
	PyObject* user_data = nullptr;
	
	if (!Parse((GetParsers())[this->pythonName], args, kwargs, this->pythonName, &callback, &user_data))
		return GetPyNone();

	// callback and user_data are borrowed references
	// so when the callback is made, it bumps the refcounts
	auto wrapper_ptr = std::make_shared<mvCallbackWithData>(SanitizeCallback(callback), nullptr, user_data, MV_CALLBACK_BORROW_ALL);

	mvSubmitCallback([=, wrapper_ptr = std::move(wrapper_ptr)]() mutable
		{
			this->cwd = std::move(*wrapper_ptr.get());
		});

	return GetPyNone();
}

//-----------------------------------------------------------------------------
// mvCallbackJob
//-----------------------------------------------------------------------------

mvCallbackJob::mvCallbackJob(mvCallbackWithData&& cwd, mvUUID sender, std::string sender_str, bool valid)
	: cwd(std::move(cwd)), sender(sender), valid(valid)
{
	if (sender == 0) {
		this->sender_str = sender_str;
	}
}

mvCallbackJob::mvCallbackJob(PyObject* callback, mvUUID sender, std::string sender_str, PyObject* appData, PyObject* userData, mvCallbackRefcountFlags flags)
	: mvCallbackJob(mvCallbackWithData(callback, appData, userData, flags), sender, sender_str, true)
{
}

mvCallbackJob::mvCallbackJob(mvAppItem& item, PyObject* appData, mvCallbackRefcountFlags flags)
	: mvCallbackJob(item.getCallback(false), item, appData, flags)
{
}

mvCallbackJob::mvCallbackJob(PyObject* callback, mvAppItem& item, PyObject* appData, mvCallbackRefcountFlags flags)
	: cwd(callback, appData, item.config.user_data, flags)
{
	if (item.config.alias.empty()) {
		sender = item.uuid;
	}
	else {
		sender = 0;
		sender_str = item.config.alias;
	}
}

PyObject* mvCallbackJob::to_python_tuple(mvCallbackJob&& job)
{
	if (!job.is_valid()) {
		mvThrowPythonError(mvErrorCode::mvNone, "Tried to run an invalidated mvCallbackJob (manual callbacks).");
		PyErr_Print();
		return nullptr;
	}

	job.cwd.nullToNone();

	PyObject* job_py = PyTuple_New(4);
	auto cwd = std::move(job.cwd);
	PyTuple_SetItem(job_py, 0, cwd.callback.steal());

	if (job.sender == 0)
		PyTuple_SetItem(job_py, 1, ToPyString(job.sender_str));
	else
		PyTuple_SetItem(job_py, 1, ToPyUUID(job.sender));

	PyTuple_SetItem(job_py, 2, cwd.appData.steal());
	PyTuple_SetItem(job_py, 3, cwd.userData.steal());
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

	mvAddCallbackJob({GContext->callbackRegistry->frameCallbacks[frame], static_cast<mvUUID>(frame), nullptr,
		GContext->callbackRegistry->frameCallbacksUserData[frame]});
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

void mvAddCallbackJob(mvCallbackJob&& job, bool allowManualManagement)
{
	if (GContext->callbackRegistry->callCount > GContext->callbackRegistry->maxNumberOfCalls)
	{
		assert(false);
		return;
	}

	if (allowManualManagement && GContext->IO.manualCallbacks) {
		GContext->callbackRegistry->jobs.push_back(std::move(job));
	}
	else {
		mvSubmitRunCallbackJob(std::move(job));
	}
}

void mvRunCallbackJob(mvCallbackJob&& job)
{
	if (!job.is_valid()) {
		mvThrowPythonError(mvErrorCode::mvNone, "Tried to run an invalidated mvCallbackJob.");
		PyErr_Print();
		return;
	}

	job.cwd.nullToNone();
	if (*job.cwd.callback == Py_None)
		return;

	if (!PyCallable_Check(*job.cwd.callback))
	{
		mvThrowPythonError(mvErrorCode::mvNone, "Callable not callable.");
		PyErr_Print();
		return;
	}

	if (PyErr_Occurred())
		PyErr_Print();

	// Is this called twice because PyErr_Print() could error?
	if (PyErr_Occurred())
		PyErr_Print();

	auto fc = mvPyObjectStrict(PyObject_GetAttrString(*job.cwd.callback, "__code__"), false);
	if (fc) {
		auto ac = mvPyObjectStrict(PyObject_GetAttrString(*fc, "co_argcount"), false);
		if (ac) {
			i32 count = PyLong_AsLong(*ac);

			if (PyMethod_Check(*job.cwd.callback))
				count--;

			mvPyObjectStrict pArgs(PyTuple_New(count), false);

			if (count >= 1) {
				PyObject* sender = nullptr;
				if (job.sender == 0) {
					sender = ToPyString(job.sender_str);
				}
				else {
					sender = ToPyUUID(job.sender);
				}
				PyTuple_SetItem(*pArgs, 0, sender);
			}

			if (count >= 2)
				PyTuple_SetItem(*pArgs, 1, job.cwd.appData.steal());

			if (count >= 3)
				PyTuple_SetItem(*pArgs, 2, job.cwd.userData.steal());

			if (count > 3) {
				for (int i = 3; i < count; i++)
					PyTuple_SetItem(*pArgs, i, GetPyNone());
			}
			
			mvPyObject result(PyObject_CallObject(*job.cwd.callback, *pArgs));

			// check if call succeeded
			if (!result.isOk())
				PyErr_Print();
		}
	}
}
