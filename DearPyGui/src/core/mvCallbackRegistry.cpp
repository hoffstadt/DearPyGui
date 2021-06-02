#include "mvCallbackRegistry.h"
#include "mvProfiler.h"
#include "mvApp.h"
#include <chrono>
#include <iostream>
#include "mvItemRegistry.h"
#include "mvInput.h"
#include "mvAppItemCommons.h"
#include "mvGlobalIntepreterLock.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvCallbackRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback");
			parser.finalize();
			parsers->insert({ "set_start_callback", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback");
			parser.finalize();
			parsers->insert({ "set_exit_callback", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback");
			parser.addArg<mvPyDataType::String>("handler", mvArgType::KEYWORD_ARG, "''");
			parser.finalize();
			parsers->insert({ "set_resize_callback", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback");
			parser.finalize();
			parsers->insert({ "set_mouse_release_callback", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback");
			parser.finalize();
			parsers->insert({ "set_mouse_down_callback", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback");
			parser.addArg<mvPyDataType::Float>("threshold");
			parser.finalize();
			parsers->insert({ "set_mouse_drag_callback", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback");
			parser.finalize();
			parsers->insert({ "set_mouse_wheel_callback", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback");
			parser.finalize();
			parsers->insert({ "set_mouse_double_click_callback", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Callable>("callback");
			parser.finalize();
			parsers->insert({ "set_mouse_click_callback", parser });
		}

	}

	mvCallbackRegistry::mvCallbackRegistry()
	{
		mvEventBus::Subscribe(this, mvEVT_FRAME);
		mvEventBus::Subscribe(this, mvEVT_PRE_RENDER);
		mvEventBus::Subscribe(this, mvEVT_END_FRAME);
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_INPUT);
	}

	void mvCallbackRegistry::runTasks()
	{

		while (!m_tasks.empty())
		{
			mvFunctionWrapper t;
			m_tasks.wait_and_pop(t);
			t();
		}
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
			addCallback(m_onStartCallback, "Main Application", nullptr);
			break;

		default:
			break;
		}

		return false;
	}

	bool mvCallbackRegistry::onEndFrame(mvEvent& event)
	{
		MV_PROFILE_SCOPE("End Frame Tasks")

		runTasks();
		
		return false;
	}

	bool mvCallbackRegistry::onRender(mvEvent& event)
	{
		MV_PROFILE_SCOPE("Begin Frame Tasks")

		runTasks();
	
		return false;
	}

	bool mvCallbackRegistry::onInputs(mvEvent& event)
	{
		std::string active = mvApp::GetApp()->getItemRegistry().getActiveWindow();

		switch (event.type)
		{

		case mvEVT_MOUSE_WHEEL:
			if (m_mouseWheelCallback)
				submitCallback([=]() mutable
				{
					runCallback(m_mouseWheelCallback, active, ToPyInt((int)GetEFloat(event, "DELTA")));
				});
			break;

		case mvEVT_MOUSE_DRAG:
			if (m_mouseDragCallback)
				submitCallback([=]() mutable
				{
					runCallback(m_mouseDragCallback, active,
						ToPyMTrip(GetEInt(event, "BUTTON"), GetEFloat(event, "X"), GetEFloat(event, "Y")));
				});
			break;

		case mvEVT_MOUSE_CLICK:
			if (m_mouseClickCallback)
				submitCallback([=]() mutable
				{
					runCallback(m_mouseClickCallback, active, ToPyInt(GetEInt(event, "BUTTON")));
				});
			break;

		case mvEVT_MOUSE_DOWN:
			if (m_mouseDownCallback)
				submitCallback([=]() mutable
				{
					runCallback(m_mouseDownCallback, active, ToPyMPair(GetEInt(event, "BUTTON"), GetEFloat(event, "DURATION")));
				});
			break;

		case mvEVT_MOUSE_DBL_CLK:
			if (m_mouseDoubleClickCallback)
				submitCallback([=]() mutable
				{
					runCallback(m_mouseDoubleClickCallback, active, ToPyInt(GetEInt(event, "BUTTON")));
				});
			break;

		case mvEVT_MOUSE_RELEASE:
			if (m_mouseReleaseCallback)
				submitCallback([=]() mutable
				{
					runCallback(m_mouseReleaseCallback, active, ToPyInt(GetEInt(event, "BUTTON")));
				});
			break;

		default:
			return false;
		}

		return true;
	}

	bool mvCallbackRegistry::runCallbacks()
	{
		m_running = true;

		mvGlobalIntepreterLock gil;

		while (m_running)
		{
			mvFunctionWrapper t2;
			Py_BEGIN_ALLOW_THREADS;
			m_calls.wait_and_pop(t2);
			Py_END_ALLOW_THREADS;
			t2();
			m_callCount--;
		}

		runCallback(m_onCloseCallback, "Main Application", nullptr);
		return true;
	}

	void mvCallbackRegistry::addCallback(PyObject* callable, const std::string& sender, PyObject* data)
	{

		if (m_callCount > s_MaxNumberOfCalls)
		{
			if (data != nullptr)
				Py_XDECREF(data);
			assert(false);
			return;
		}

		submitCallback([=]() {
			runCallback(callable, sender, data);
			});
	}

	void mvCallbackRegistry::runCallback(PyObject* callable, const std::string& sender, PyObject* data)
	{

		if (callable == nullptr)
		{
			//if (data != nullptr)
			//	Py_XDECREF(data);
			return;
		}

		if (!PyCallable_Check(callable))
		{
			if (data != nullptr)
				Py_XDECREF(data);
			mvThrowPythonError(1000, "Callable not callable.");
			PyErr_Print();
			return;
		}

		if (data == nullptr)
		{
			data = Py_None;
			Py_XINCREF(data);
		}

		Py_XINCREF(data);

		//PyErr_Clear();

		PyObject* intermediateResult = nullptr;
		if (PyCallable_Check(data))
		{
			intermediateResult = PyObject_CallObject(data, nullptr);
			// check if call succeeded
			if (intermediateResult == nullptr)
			{
				PyErr_Print();
				intermediateResult = data;
			}

		}
		else
			intermediateResult = data;

		//PyErr_Clear();

		PyObject* fc = PyObject_GetAttrString(callable, "__code__");
		if (fc) {
			PyObject* ac = PyObject_GetAttrString(fc, "co_argcount");
			if (ac) {
				int count = PyLong_AsLong(ac);

				if (PyMethod_Check(callable))
					count--;

				if (count > 2)
				{
					mvPyObject pArgs(PyTuple_New(count));
					PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));
					PyTuple_SetItem(pArgs, 1, intermediateResult); // steals data, so don't deref
					
					for (int i = 2; i < count; i++)
						PyTuple_SetItem(pArgs, i, GetPyNone());

					mvPyObject result(PyObject_CallObject(callable, pArgs));

					// check if call succeeded
					if (!result.isOk())
						PyErr_Print();

				}
				else if (count == 2)
				{
					mvPyObject pArgs(PyTuple_New(2));
					PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));
					PyTuple_SetItem(pArgs, 1, intermediateResult); // steals data, so don't deref

					mvPyObject result(PyObject_CallObject(callable, pArgs));

					pArgs.delRef();
					// check if call succeeded
					if (!result.isOk())
						PyErr_Print();

				}
				else if(count == 1)
				{
					mvPyObject pArgs(PyTuple_New(1));
					PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));

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

	PyObject* mvCallbackRegistry::set_start_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_start_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setOnStartCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* mvCallbackRegistry::set_exit_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_exit_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setOnCloseCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* mvCallbackRegistry::set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseDownCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* mvCallbackRegistry::set_mouse_drag_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		float threshold;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_drag_callback"].parse(args, kwargs, __FUNCTION__,
			&callback, &threshold))
			return GetPyNone();

		mvInput::setMouseDragThreshold(threshold);

		if (callback)
			Py_XINCREF(callback);

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseDragCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* mvCallbackRegistry::set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_double_click_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseDoubleClickCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* mvCallbackRegistry::set_mouse_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_click_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseClickCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* mvCallbackRegistry::set_mouse_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_release_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseReleaseCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* mvCallbackRegistry::set_mouse_wheel_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_wheel_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseWheelCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* mvCallbackRegistry::set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* handler = "";

		if (!(mvApp::GetApp()->getParsers())["set_resize_callback"].parse(args, kwargs, __FUNCTION__,
			&callback, &handler))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);


		auto fut = mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{

				if (std::string(handler).empty())
				{
					mvApp::GetApp()->getCallbackRegistry().setResizeCallback(callback);
					return std::string("");
				}

				mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(handler);

				if (item)
				{
					if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_ROOT))
					{
						auto windowtype = static_cast<mvWindowAppItem*>(item.get());
						windowtype->setResizeCallback(callback);
					}
					else
						return std::string("Resize callback can only be set for window items");
				}
				return std::string("");
			});

		auto message = fut.get();

		if (!message.empty())
			mvThrowPythonError(1000, message);

		return GetPyNone();
	}
}