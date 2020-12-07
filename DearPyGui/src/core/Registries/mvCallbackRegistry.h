#pragma once
#include <queue>
#include <string>
#include <mutex>
#include "core/mvEvents.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	class mvThreadPool;

	class mvCallbackRegistry : public mvEventHandler
	{
		struct NewCallback
		{
			std::string sender;
			PyObject* callback;   // name of function to run
			PyObject* data;       // any data need by the function
		};

		struct AsyncronousCallback
		{
			PyObject* name;       // name of function to run
			PyObject* data;       // any data need by the function
			PyObject* returnname; // optional return function
		};

	public:

		static mvCallbackRegistry* GetCallbackRegistry();

		bool onEvent   (mvEvent& event) override;
		bool onEndFrame(mvEvent& event);

		void runReturnCallback(PyObject* callback, const std::string& sender, PyObject* data);
        void runCallback      (PyObject* callback, const std::string& sender, PyObject* data = nullptr);
        void runAsyncCallback (PyObject* callback, PyObject* data, PyObject* returnname);
        void addMTCallback    (PyObject* name, PyObject* data, PyObject* returnname = nullptr);
        void addCallback      (PyObject* callback, const std::string& sender, PyObject* data);

		void runAsyncCallbacks(mvThreadPool* threadpool);
		void runAsyncCallbackReturns();
		void runCallbacks();

		bool hasAsyncCallbacks() { return !m_asyncCallbacks.empty(); }
	
	private:

		mvCallbackRegistry();

		static mvCallbackRegistry*       s_instance;
		mutable std::mutex               m_mutex;

		// new callback system
		std::queue<NewCallback>          m_callbacks;

		// concurrency
		std::queue<AsyncronousCallback>  m_asyncReturns;
		std::vector<AsyncronousCallback> m_asyncCallbacks;

	};

}