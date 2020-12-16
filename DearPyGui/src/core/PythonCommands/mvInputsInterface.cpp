#include "mvInputsInterface.h"

namespace Marvel {

	void AddInputWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_input_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value", "", "0"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Integer, "step", "", "1"},
			{mvPythonDataType::Integer, "step_fast", "", "100"},
			{mvPythonDataType::Bool, "readonly", "", "False"},
		}, "Adds input for integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value", "", "(0, 0)"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "readonly", "", "False"},
		}, "Adds input for 2 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value", "", "(0, 0, 0)"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "readonly", "", "False"},
		}, "Adds input for 3 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value", "", "(0, 0, 0, 0)"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "readonly", "", "False"},
		}, "Adds input for 4 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value", "", "0.0"},
			{mvPythonDataType::String, "format", "", "'%.3f'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Float, "step", "", "0.1"},
			{mvPythonDataType::Float, "step_fast", "", "1.0"},
			{mvPythonDataType::Bool, "readonly", "", "False"},
		}, "Adds input for float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0)"},
			{mvPythonDataType::String, "format", "", "'%.3f'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "readonly", "", "False"},
		}, "Adds input for 2 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0)"},
			{mvPythonDataType::String, "format", "", "'%.3f'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "readonly", "", "False"},
		}, "Adds input for 3 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0, 0.0)"},
			{mvPythonDataType::String, "format", "", "'%.3f'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "readonly", "", "False"},
		}, "Adds input for 4 float values.", "None", "Adding Widgets") });
	}

	PyObject* add_input_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;
		int step = 1;
		int step_fast = 100;
		int readonly = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width, &on_enter,
			&label, &popup, &show, &step, &step_fast, &readonly))
			return ToPyBool(false);

		auto item = CreateRef<mvInputInt>(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;
		int readonly = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int2"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width, &on_enter,
			&label, &popup, &show, &readonly))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);

		auto item = CreateRef<mvInputInt2>(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;
		int readonly = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int3"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width, &on_enter,
			&label, &popup, &show, &readonly))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);

		auto item = CreateRef<mvInputInt3>(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;
		int readonly = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int4"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width, &on_enter,
			&label, &popup, &show, &readonly))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);
		auto item = CreateRef<mvInputInt4>(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;
		float step = 0.1f;
		float step_fast = 1.0f;
		int readonly = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width, &on_enter,
			&label, &popup, &show, &step, &step_fast, &readonly))
			return ToPyBool(false);

		auto item = CreateRef<mvInputFloat>(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;
		int readonly = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float2"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width, &on_enter,
			&label, &popup, &show, &readonly))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);
		auto item = CreateRef<mvInputFloat2>(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;
		int readonly = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float3"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width, &on_enter,
			&label, &popup, &show, &readonly))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);

		auto item = CreateRef<mvInputFloat3>(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_input_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int enabled = true;
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;
		int readonly = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float4"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width, &on_enter,
			&label, &popup, &show, &readonly))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);

		auto item = CreateRef<mvInputFloat4>(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before));
	}

}
