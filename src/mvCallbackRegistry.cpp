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
		cwd.appData = std::make_shared<mvPyObjectStrict>(appData);
	mvSubmitRunCallbackJob({std::move(cwd), sender});
}

void mvCallbackPoint::run_blocking(mvUUID sender, PyObject *appData)
{
	auto cwd = this->cwd.copy();
	if (appData)
		cwd.appData = std::make_shared<mvPyObjectStrict>(appData);
	mvRunCallbackJob({std::move(cwd), sender});
}

PyObject* mvCallbackPoint::set_from_python(PyObject* self, PyObject* args, PyObject* kwargs)
{
	PyObject* callback;
	PyObject* user_data = nullptr;
	
	if (!Parse((GetParsers())[this->pythonName], args, kwargs, this->pythonName, &callback, &user_data))
		return GetPyNone();

	// callback and user_data are borrowed references
	// so when the callback is made, it bumps the refcounts
	auto wrapper_ptr = std::make_shared<mvCallbackWithData>(
		std::make_shared<mvPyObjectStrict>(SanitizeCallback(callback)),
		std::make_shared<mvPyObjectStrict>(),
		std::make_shared<mvPyObjectStrict>(user_data));

	mvSubmitCallback([=, wrapper_ptr = std::move(wrapper_ptr)]() mutable
		{
			this->cwd = std::move(*wrapper_ptr.get());
		});

	return GetPyNone();
}

//-----------------------------------------------------------------------------
// mvCallbackJob
//-----------------------------------------------------------------------------

mvCallbackJob::mvCallbackJob(mvCallbackWithData&& cwd, mvUUID sender, std::string sender_str, std::function<PyObject*()> makeAppData, bool valid)
	: cwd(std::move(cwd)), sender(sender), makeAppData(makeAppData), valid(valid)
{
	if (sender == 0) {
		this->sender_str = sender_str;
	}
}

mvCallbackJob::mvCallbackJob(mvPyObjectStrictPtr callback, mvUUID sender, std::string sender_str, mvAppDataVariant appData, mvPyObjectStrictPtr userData) :
	mvCallbackJob(
		mvCallbackWithData(
			std::move(callback),
			appData.index() == 0 ? std::move(std::get<0>(appData)) : nullptr,
			std::move(userData)),
		sender,
		sender_str,
		appData.index() == 1 ? std::move(std::get<1>(appData)) : nullptr)
{
}

mvCallbackJob::mvCallbackJob(mvAppItem& item, mvAppDataVariant appData) :
	mvCallbackJob(item.getCallback(false), item, std::move(appData))
{
}
mvCallbackJob::mvCallbackJob(mvPyObjectStrict* callback, mvAppItem& item, mvAppDataVariant appData) :
	mvCallbackJob({item.shared_from_this(), std::move(callback)}, item, std::move(appData))
{
}

mvCallbackJob::mvCallbackJob(mvPyObjectStrictPtr callback, mvAppItem& item, mvAppDataVariant appData) :
	cwd(
		std::move(callback),
		appData.index() == 0 ? std::move(std::get<0>(appData)) : nullptr,
		{item.shared_from_this(), &item.config.user_data}),
	makeAppData(appData.index() == 1 ? std::move(std::get<1>(appData)) : nullptr)
{
	if (item.config.alias.empty()) {
		sender = item.uuid;
	}
	else {
		sender = 0;
		sender_str = item.config.alias;
	}
}

void mvCallbackJob::prepare()
{
	if (makeAppData) {
		cwd.appData = std::make_shared<mvPyObjectStrict>(makeAppData(), false);
	}
	cwd.nullToNone();
}

PyObject* mvCallbackJob::to_python_tuple(mvCallbackJob&& job)
{
	if (!job.is_valid()) {
		mvThrowPythonError(mvErrorCode::mvNone, "Tried to run an invalidated mvCallbackJob (manual callbacks).");
		PyErr_Print();
		return nullptr;
	}

	job.prepare();

	auto callback = job.cwd.callback->copy();
	auto appData = job.cwd.appData->copy();
	auto userData = job.cwd.userData->copy();

	PyObject* job_py = PyTuple_New(4);
	PyTuple_SetItem(job_py, 0, callback.steal());

	if (job.sender == 0)
		PyTuple_SetItem(job_py, 1, ToPyString(job.sender_str));
	else
		PyTuple_SetItem(job_py, 1, ToPyUUID(job.sender));

	PyTuple_SetItem(job_py, 2, appData.steal());
	PyTuple_SetItem(job_py, 3, userData.steal());
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
	if (job.no_callback()) {
		return;
	}

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

	if (job.no_callback()) {
		return;
	}

	job.prepare();
	auto callback = job.cwd.callback->copy();
	auto appData = job.cwd.appData->copy();
	auto userData = job.cwd.userData->copy();

	if (!PyCallable_Check(*callback))
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

	auto fc = mvPyObjectStrict(PyObject_GetAttrString(*callback, "__code__"), false);
	if (fc) {
		auto ac = mvPyObjectStrict(PyObject_GetAttrString(*fc, "co_argcount"), false);
		if (ac) {
			i32 count = PyLong_AsLong(*ac);

			if (PyMethod_Check(*callback))
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
				PyTuple_SetItem(*pArgs, 1, appData.steal());

			if (count >= 3)
				PyTuple_SetItem(*pArgs, 2, userData.steal());

			if (count > 3) {
				for (int i = 3; i < count; i++)
					PyTuple_SetItem(*pArgs, i, GetPyNone());
			}
			
			mvPyObject result(PyObject_CallObject(callback.steal(), *pArgs));

			// check if call succeeded
			if (!result.isOk())
				PyErr_Print();
		}
	}
}
