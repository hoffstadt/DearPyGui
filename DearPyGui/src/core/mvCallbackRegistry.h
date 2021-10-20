#pragma once

#include <mutex>
#include "mvThreadPool.h"
#include "mvContext.h"

namespace Marvel {

	mv_internal PyObject* SanitizeCallback(PyObject* callback)
	{
		if (callback == Py_None)
			return nullptr;

		return callback;
	}

	struct mvCallbackRegistry
	{
		const i32 maxNumberOfCalls = 50;

		mvQueue<mvFunctionWrapper> tasks;
		mvQueue<mvFunctionWrapper> calls;
		std::atomic<b8>            running = false;
		std::atomic<i32>           callCount = 0;

		// callbacks
		PyObject* resizeCallback = nullptr;
		PyObject* onCloseCallback = nullptr;

		i32 highestFrame = 0;
		std::unordered_map<i32, PyObject*> frameCallbacks;
	};

	void InsertParser_mvCallbackRegistry(std::map<std::string, mvPythonParser>* parsers);

	void mvFrameCallback(i32 frame);
	void mvRunTasks();
	void mvRunCallback(PyObject* callback, mvUUID sender, PyObject* app_data, PyObject* user_data);
	void mvRunCallback(PyObject* callback, const std::string& sender, PyObject* app_data, PyObject* user_data);
	void mvAddCallback(PyObject* callback, mvUUID sender, PyObject* app_data, PyObject* user_data);
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

	MV_CREATE_FREE_COMMAND(set_frame_callback);
	MV_CREATE_FREE_COMMAND(set_exit_callback);
	MV_CREATE_FREE_COMMAND(set_viewport_resize_callback);

	MV_START_FREE_COMMANDS(mvCallbackRegCommands)
		MV_ADD_COMMAND(set_frame_callback);
		MV_ADD_COMMAND(set_exit_callback);
		MV_ADD_COMMAND(set_viewport_resize_callback);
	MV_END_COMMANDS

}
