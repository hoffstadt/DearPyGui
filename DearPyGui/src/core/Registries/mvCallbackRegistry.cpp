#include "mvCallbackRegistry.h"
#include "mvPyObject.h"
#include "mvPythonTranslator.h"
#include "mvPythonExceptions.h"
#include "mvProfiler.h"
#include "mvGlobalIntepreterLock.h"
#include "mvApp.h"

namespace Marvel {

	mvCallbackRegistry::mvCallbackRegistry()
	{
		mvEventBus::Subscribe(this, mvEVT_FRAME);
		mvEventBus::Subscribe(this, mvEVT_PRE_RENDER);
		mvEventBus::Subscribe(this, mvEVT_END_FRAME);
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_INPUT);
	}

	bool mvCallbackRegistry::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvCallbackRegistry::onFrame),    mvEVT_FRAME);
		dispatcher.dispatch(BIND_EVENT_METH(mvCallbackRegistry::onEndFrame), mvEVT_END_FRAME);
		dispatcher.dispatch(BIND_EVENT_METH(mvCallbackRegistry::onRender),   mvEVT_PRE_RENDER);
		dispatcher.dispatch(BIND_EVENT_METH(mvCallbackRegistry::onInputs), 0, mvEVT_CATEGORY_INPUT);

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

		runCallback(m_renderCallback, "Main Application");

		return false;
	}

	bool mvCallbackRegistry::onInputs(mvEvent& event)
	{
		const char* active = mvApp::GetApp()->getItemRegistry().getActiveWindow().c_str();

		switch (event.type)
		{
		case mvEVT_KEY_PRESS:
			runCallback(m_acceleratorCallback, active, ToPyInt(GetEInt(event, "KEY")));
			runCallback(m_keyPressCallback, active, ToPyInt(GetEInt(event, "KEY")));
			break;

		case mvEVT_KEY_DOWN:
			runCallback(m_keyDownCallback, active, ToPyMPair(GetEInt(event, "KEY"), GetEFloat(event, "DURATION")));
			break;

		case mvEVT_KEY_RELEASE:
			runCallback(m_keyReleaseCallback, active, ToPyInt(GetEInt(event, "KEY")));
			break;

		case mvEVT_MOUSE_WHEEL:
			runCallback(m_mouseWheelCallback, active, ToPyInt(GetEFloat(event, "DELTA")));
			break;

		case mvEVT_MOUSE_DRAG:
			runCallback(m_mouseDragCallback, active, 
				ToPyMTrip(GetEInt(event, "BUTTON"), GetEFloat(event, "X"), GetEFloat(event, "Y")));
			break;

		case mvEVT_MOUSE_CLICK:
			runCallback(m_mouseClickCallback, active, ToPyInt(GetEInt(event, "BUTTON")));
			break;

		case mvEVT_MOUSE_DOWN:
			runCallback(m_mouseDownCallback, active, ToPyMPair(GetEInt(event, "BUTTON"), GetEFloat(event, "DURATION")));
			break;

		case mvEVT_MOUSE_DBL_CLK:
			runCallback(m_mouseDoubleClickCallback, active, ToPyInt(GetEInt(event, "BUTTON")));
			break;

		case mvEVT_MOUSE_RELEASE:
			runCallback(m_mouseReleaseCallback, active, ToPyInt(GetEInt(event, "BUTTON")));
			break;

		case mvEVT_MOUSE_MOVE:
			runCallback(m_mouseMoveCallback, active, ToPyPair(GetEFloat(event, "X"), GetEFloat(event, "Y")));
			break;

		default:
			return false;
		}

		return true;
	}

	void mvCallbackRegistry::runCallbacks()
	{

		for (auto& item : m_callbacks)
			runCallback(item.callback, item.sender, item.data);

		m_callbacks.clear();

	}

	void mvCallbackRegistry::addCallback(PyObject* callable, const std::string& sender, PyObject* data)
	{
		m_callbacks.push_back({ sender, callable, data });
	}

	void mvCallbackRegistry::runCallback(PyObject* callable, const std::string& sender, PyObject* data)
	{

		if (callable == nullptr)
		{
			if (data != nullptr)
				Py_XDECREF(data);
			return;
		}

		 

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

		PyObject* intermediateResult = nullptr;
		if (PyCallable_Check(data))
		{
			intermediateResult = PyObject_CallObject(data, nullptr);
			// check if call succeeded
			if (intermediateResult == nullptr)
			{
				PyErr_Print();
				ThrowPythonException("Callable data failed");
				intermediateResult = data;
			}

			// check if error occurred
			if (PyErr_Occurred())
				PyErr_Print();
		}
		else
			intermediateResult = data;

		PyErr_Clear();

		mvPyObject pArgs(PyTuple_New(2));
		PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));
		PyTuple_SetItem(pArgs, 1, intermediateResult); // steals data, so don't deref

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