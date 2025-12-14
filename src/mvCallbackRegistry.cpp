#include "mvCallbackRegistry.h"
#include "mvProfiler.h"
#include "mvContext.h"
#include <chrono>
#include "mvItemRegistry.h"
#include "mvAppItemCommons.h"
#include "mvPyUtils.h"

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
	auto callbackRegistry = GContext->callbackRegistry;   // for brevity

	if (frame > callbackRegistry->highestFrame)
		return;

	if (callbackRegistry->frameCallbacks.count(frame) == 0)
		return;

	// We have to use `std::unordered_map::at()` instead of indexing it with `[]`:
	// `mvPyObject` has no default constructor, whereas `operator[]` can insert
	// a new value into the map and therefore requires a default constructor.

	auto callback = std::make_shared<mvPyObject>(std::move(callbackRegistry->frameCallbacks.at(frame)));
	auto user_data = std::make_shared<mvPyObject>(std::move(callbackRegistry->frameCallbacksUserData.at(frame)));

	// Since mvPyObject objects remaining in the maps are now "empty", it's safe to
	// delete them right here even though we don't own the GIL.
	callbackRegistry->frameCallbacks.erase(frame);
	callbackRegistry->frameCallbacksUserData.erase(frame);

	mvAddOwnerlessCallback(callback, user_data, (mvUUID)frame, "", []() -> PyObject* { return nullptr; });
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

void mvAddCallback(const std::weak_ptr<void>& owner,
                   PyObject* callback,
                   const std::shared_ptr<mvPyObject>& user_data,
                   mvUUID sender,
                   const std::string& alias)
{
    mvAddCallback(owner, callback, user_data, sender, alias, []() -> PyObject* { return nullptr; });
}

void mvAddOwnerlessCallback(const std::shared_ptr<mvPyObject>& callback,
                            const std::shared_ptr<mvPyObject>& user_data,
                            mvUUID sender,
                            const std::string& alias)
{
	mvAddOwnerlessCallback(callback, user_data, sender, alias, []() -> PyObject* { return nullptr; });
}

void mvRunCallback(PyObject* callback, PyObject* user_data, mvUUID sender, const std::string& sender_alias, PyObject* app_data)
{

	if (callback == nullptr)
		return;

	if (!PyCallable_Check(callback))
	{
		mvThrowPythonError(mvErrorCode::mvNone, "Callable not callable.");
		PyErr_Print();
		return;
	}

	//PyErr_Clear();
	if (PyErr_Occurred())
		PyErr_Print();

	PyObject* fc = PyObject_GetAttrString(callback, "__code__");
	if (fc) {
		PyObject* ac = PyObject_GetAttrString(fc, "co_argcount");
		if (ac) {
			i32 count = PyLong_AsLong(ac);

			if (PyMethod_Check(callback))
				count--;

			mvPyObject pArgs(count > 0 ? PyTuple_New(count) : nullptr);

			if (count > 0)
			{
				PyTuple_SetItem(pArgs, 0, sender_alias.empty()?
						ToPyUUID(sender) :
						ToPyString(sender_alias));

				if (count > 1)
				{
					if (app_data == nullptr)
						app_data = Py_None;
					// Need an owned ref here: PyTuple_SetItem takes ownership;
					// this also handles Py_None correctly (need to incref it).
					Py_INCREF(app_data);
					PyTuple_SetItem(pArgs, 1, app_data);

					if (count > 2)
					{
						if (user_data == nullptr)
							user_data = Py_None;
						// Need an owned ref here: PyTuple_SetItem takes ownership;
						// this also handles Py_None correctly (need to incref it).
						Py_INCREF(user_data);
						PyTuple_SetItem(pArgs, 2, user_data);

						// If the callback takes more parms, just pass None in there
						for (int i = 3; i < count; i++)
							PyTuple_SetItem(pArgs, i, GetPyNone());
					}
				}
			}

			// perform the actual call
			mvPyObject result(PyObject_CallObject(callback, pArgs));

			// check if call succeeded
			if (!result.isOk())
				PyErr_Print();

			Py_DECREF(ac);
		}
		Py_DECREF(fc);
	}

}
