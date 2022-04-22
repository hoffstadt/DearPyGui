#pragma once

#include <mutex>
#include <vector>
#include "mvThreadPool.h"
#include "mvContext.h"

mv_internal PyObject* SanitizeCallback(PyObject* callback)
{
	if (callback == Py_None)
		return nullptr;

	return callback;
}

struct mvCallbackJob
{
	mvUUID      sender    = 0;
	PyObject*   callback  = nullptr;
	PyObject*   app_data  = nullptr;
	PyObject*   user_data = nullptr;
	std::string sender_str;
};

struct mvCallbackRegistry
{
	const i32 maxNumberOfCalls = 50;

	std::vector<mvCallbackJob> jobs;
	mvQueue<mvFunctionWrapper> tasks;
	mvQueue<mvFunctionWrapper> calls;
	std::atomic<b8>            running = false;
	std::atomic<i32>           callCount = 0;

	// callbacks
	PyObject* resizeCallback          = nullptr;
	PyObject* onCloseCallback         = nullptr;
	PyObject* resizeCallbackUserData  = nullptr;
	PyObject* onCloseCallbackUserData = nullptr;

	i32 highestFrame = 0;
	std::unordered_map<i32, PyObject*> frameCallbacks;
	std::unordered_map<i32, PyObject*> frameCallbacksUserData;
};

void mvFrameCallback(i32 frame);
void mvRunTasks();
void mvRunCallback(PyObject* callback, mvUUID sender, PyObject* app_data, PyObject* user_data, bool decrementAppData = true);
void mvRunCallback(PyObject* callback, const std::string& sender, PyObject* app_data, PyObject* user_data);
void mvAddCallback(PyObject* callback, mvUUID sender, PyObject* app_data, PyObject* user_data, bool decrementAppData = true);
void mvAddCallback(PyObject* callback, const std::string& sender, PyObject* app_data, PyObject* user_data);
bool mvRunCallbacks();

template<typename F, typename ...Args>
std::future<typename std::invoke_result<F, Args...>::type> mvSubmitTask(F f)
{

	typedef typename std::invoke_result<F, Args...>::type result_type;
	std::packaged_task<result_type()> task(std::move(f));
	std::future<result_type> res(task.get_future());

	if (GContext->started)
		GContext->callbackRegistry->tasks.push(std::move(task));
	else
		task();

	return res;
}

template<typename F, typename ...Args>
std::future<typename std::invoke_result<F, Args...>::type> mvSubmitCallback(F f)
{

	if (GContext->callbackRegistry->callCount > GContext->callbackRegistry->maxNumberOfCalls)
	{
		return {};
	}

	GContext->callbackRegistry->callCount++;

	typedef typename std::invoke_result<F, Args...>::type result_type;
	std::packaged_task<result_type()> task(std::move(f));
	std::future<result_type> res(task.get_future());

	GContext->callbackRegistry->calls.push(std::move(task));

	return res;
}