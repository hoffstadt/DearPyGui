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
			parser.addArg<mvPyDataType::UUID>("handler", mvArgType::KEYWORD_ARG, "''");
			parser.finalize();
			parsers->insert({ "set_resize_callback", parser });
		}

	}

	mvCallbackRegistry::mvCallbackRegistry()
	{
		mvEventBus::Subscribe(this, mvEVT_FRAME);
		mvEventBus::Subscribe(this, mvEVT_PRE_RENDER);
		mvEventBus::Subscribe(this, mvEVT_END_FRAME);
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

		return event.handled;
	}

	bool mvCallbackRegistry::onFrame(mvEvent& event)
	{

		switch (GetEInt(event, "FRAME"))
		{
		case 3:
			addCallback(m_onStartCallback, 0, nullptr, nullptr);
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

		runCallback(m_onCloseCallback, 0, nullptr, nullptr);
		return true;
	}

	void mvCallbackRegistry::addCallback(PyObject* callable, mvUUID sender, PyObject* app_data, PyObject* user_data)
	{

		if (m_callCount > s_MaxNumberOfCalls)
		{
			if (app_data != nullptr)
				Py_XDECREF(app_data);
			if (user_data != nullptr)
				Py_XDECREF(user_data);
			assert(false);
			return;
		}

		submitCallback([=]() {
			runCallback(callable, sender, app_data, user_data);
			});
	}

	void mvCallbackRegistry::runCallback(PyObject* callable, mvUUID sender, PyObject* app_data, PyObject* user_data)
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
			mvThrowPythonError(1000, "Callable not callable.");
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

		PyObject* intermediateResult = nullptr;
		if (PyCallable_Check(user_data))
		{
			intermediateResult = PyObject_CallObject(user_data, nullptr);
			// check if call succeeded
			if (intermediateResult == nullptr)
			{
				PyErr_Print();
				intermediateResult = user_data;
			}

		}
		else
			intermediateResult = user_data;

		//PyErr_Clear();

		PyObject* fc = PyObject_GetAttrString(callable, "__code__");
		if (fc) {
			PyObject* ac = PyObject_GetAttrString(fc, "co_argcount");
			if (ac) {
				int count = PyLong_AsLong(ac);

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

	PyObject* mvCallbackRegistry::set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		mvUUID handler = 0;

		if (!(mvApp::GetApp()->getParsers())["set_resize_callback"].parse(args, kwargs, __FUNCTION__,
			&callback, &handler))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);


		auto fut = mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{

				if (handler == 0)
				{
					mvApp::GetApp()->getCallbackRegistry().setResizeCallback(callback);
					return std::string("");
				}

				mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(handler);

				if (item)
				{
					if (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_ROOT))
					{
						auto windowtype = static_cast<mvWindowAppItem*>(item);
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