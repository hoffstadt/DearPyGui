#include "mvCallbackRegistry.h"
#include "PythonUtilities/mvPyObject.h"
#include "PythonUtilities/mvPythonTranslator.h"
#include "PythonUtilities/mvPythonExceptions.h"
#include "core/mvThreadPool.h"
#include "core/mvProfiler.h"

namespace Marvel {

	mvCallbackRegistry* mvCallbackRegistry::s_instance = nullptr;

	mvCallbackRegistry* mvCallbackRegistry::GetCallbackRegistry()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvCallbackRegistry();
		return s_instance;
	}

	mvCallbackRegistry::mvCallbackRegistry()
	{
		mvEventBus::Subscribe(this, SID("SPECIFIC_FRAME"));
		mvEventBus::Subscribe(this, SID("PRE_RENDER"));
		mvEventBus::Subscribe(this, SID("PRE_RENDER"));
		mvEventBus::Subscribe(this, 0, SID("INPUT_EVENTS"));
	}

	bool mvCallbackRegistry::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvCallbackRegistry::onFrame), SID("SPECIFIC_FRAME"));
		dispatcher.dispatch(BIND_EVENT_METH(mvCallbackRegistry::onEndFrame), SID("END_FRAME"));
		dispatcher.dispatch(BIND_EVENT_METH(mvCallbackRegistry::onRender), SID("PRE_RENDER"));
		dispatcher.dispatch(BIND_EVENT_METH(mvCallbackRegistry::onInputs), 0, SID("INPUT_EVENTS"));

		return event.handled;
	}

	bool mvCallbackRegistry::onFrame(mvEvent& event)
	{

		switch (GetEInt(event, "FRAME"))
		{
		case 3:
			runCallback(m_onStartCallback, "Main Application");
			break;

		default:
			break;
		}

		return false;
	}

	bool mvCallbackRegistry::onEndFrame(mvEvent& event)
	{
		MV_PROFILE_FUNCTION()

		runCallbacks();

		return false;
	}

	bool mvCallbackRegistry::onRender(mvEvent& event)
	{
		MV_PROFILE_FUNCTION()

		runAsyncCallbackReturns();
		runCallback(m_renderCallback, "Main Application");

		return false;
	}

	bool mvCallbackRegistry::onInputs(mvEvent& event)
	{
		const char* active = "new system";

		switch (event.type)
		{
		case SID("KEY_PRESS"):
			runCallback(m_acceleratorCallback, active, ToPyInt(GetEInt(event, "KEY")));
			runCallback(m_keyPressCallback, active, ToPyInt(GetEInt(event, "KEY")));
			break;

		case SID("KEY_DOWN"):
			runCallback(m_keyDownCallback, active, ToPyMPair(GetEInt(event, "KEY"), GetEFloat(event, "DURATION")));
			break;

		case SID("KEY_RELEASE"):
			runCallback(m_keyReleaseCallback, active, ToPyInt(GetEInt(event, "KEY")));
			break;

		case SID("MOUSE_WHEEL"):
			runCallback(m_mouseWheelCallback, active, ToPyInt(GetEFloat(event, "DELTA")));
			break;

		case SID("MOUSE_DRAG"):
			runCallback(m_mouseDragCallback, active, 
				ToPyMTrip(GetEInt(event, "BUTTON"), GetEFloat(event, "X"), GetEFloat(event, "Y")));
			break;

		case SID("MOUSE_CLICK"):
			runCallback(m_mouseClickCallback, active, ToPyInt(GetEInt(event, "BUTTON")));
			break;

		case SID("MOUSE_DOWN"):
			runCallback(m_mouseDownCallback, active, ToPyMPair(GetEInt(event, "BUTTON"), GetEFloat(event, "DURATION")));
			break;

		case SID("MOUSE_DOUBLE_CLICK"):
			runCallback(m_mouseDoubleClickCallback, active, ToPyInt(GetEInt(event, "BUTTON")));
			break;

		case SID("MOUSE_RELEASE"):
			runCallback(m_mouseReleaseCallback, active, ToPyInt(GetEInt(event, "BUTTON")));
			break;

		case SID("MOUSE_MOVE"):
			runCallback(m_mouseMoveCallback, active, ToPyPair(GetEFloat(event, "X"), GetEFloat(event, "Y")));
			break;

		default:
			return false;
		}

		return true;
	}

	void mvCallbackRegistry::runAsyncCallbackReturns()
	{
		// check if any asyncronous functions have returned
		// and are requesting to send data back to main thread
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_asyncReturns.empty())
		{
			while (!m_asyncReturns.empty())
			{
				auto& asyncreturn = m_asyncReturns.front();
				runReturnCallback(asyncreturn.name, "Asyncrounous Callback", asyncreturn.data);
				m_asyncReturns.pop();
			}
		}
	}

	void mvCallbackRegistry::runCallbacks()
	{
		while (!m_callbacks.empty())
		{
			NewCallback callback = m_callbacks.front();
			runCallback(callback.callback, callback.sender, callback.data);
			m_callbacks.pop();
		}
	}

	void mvCallbackRegistry::addCallback(PyObject* callable, const std::string& sender, PyObject* data)
	{
		m_callbacks.push({ sender, callable, data });
	}

	void mvCallbackRegistry::addMTCallback(PyObject* callback, PyObject* data, PyObject* returnname)
	{
		Py_XINCREF(data);
		//std::lock_guard<std::mutex> lock(m_mutex);
		m_asyncCallbacks.push_back({ callback, data, returnname });
	}

	void mvCallbackRegistry::runAsyncCallbacks(mvThreadPool* threadpool)
	{
		// submit to thread pool
		for (auto& callback : m_asyncCallbacks)
			threadpool->submit(std::bind(&mvCallbackRegistry::runAsyncCallback, mvCallbackRegistry::GetCallbackRegistry(),
				callback.name, callback.data, callback.returnname));

		m_asyncCallbacks.clear();
	}

	void mvCallbackRegistry::runAsyncCallback(PyObject* callback, PyObject* data, PyObject* returnname)
	{
		mvGlobalIntepreterLock gil;

		if (callback == nullptr)
		{
			Py_XDECREF(data);
			return;
		}

		// check if handler is callable
		if (PyCallable_Check(callback))
		{
			PyErr_Clear();

			//PyObject* pArgs = PyTuple_New(2);
			mvPyObject pArgs(PyTuple_New(2));
			PyTuple_SetItem(pArgs, 0, PyUnicode_FromString("Async"));
			PyTuple_SetItem(pArgs, 1, data); // steals data, so don't deref

			mvPyObject result(PyObject_CallObject(callback, pArgs));

			// check if call succeeded
			if (!result.isOk())
			{
				PyErr_Print();
				ThrowPythonException("Callback failed");
			}

			if (returnname)
			{
				result.addRef();
				std::lock_guard<std::mutex> lock(m_mutex);
				m_asyncReturns.push({ returnname, result });
			}

			// check if error occurred
			if (PyErr_Occurred())
				PyErr_Print();

		}

		else
			ThrowPythonException("Callback not callable");
	}

	void mvCallbackRegistry::runReturnCallback(PyObject* callback, const std::string& sender, PyObject* data)
	{
		mvGlobalIntepreterLock gil;

		if (callback == nullptr)
		{
			if (data != nullptr)
				Py_XDECREF(data);
			return;
		}

		if (data == nullptr)
		{
			data = Py_None;
			Py_XINCREF(data);
		}

		runCallback(callback, sender, data);
	}

	void mvCallbackRegistry::runCallback(PyObject* callable, const std::string& sender, PyObject* data)
	{

		if (callable == nullptr)
		{
			if (data != nullptr)
				Py_XDECREF(data);
			return;
		}

		mvGlobalIntepreterLock gil;

		if (!PyCallable_Check(callable))
		{
			if (data != nullptr)
				Py_XDECREF(data);
			ThrowPythonException("Callable not callable.");
			return;
		}

		if (data == nullptr)
		{
			data = Py_None;
			Py_XINCREF(data);
		}

		Py_XINCREF(data);

		PyErr_Clear();

		mvPyObject pArgs(PyTuple_New(2));
		PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));
		PyTuple_SetItem(pArgs, 1, data); // steals data, so don't deref

		mvPyObject result(PyObject_CallObject(callable, pArgs));

		// check if call succeeded
		if (!result.isOk())
		{
			PyErr_Print();
			ThrowPythonException("Callable failed");
		}

		// check if error occurred
		if (PyErr_Occurred())
			PyErr_Print();

	}

}