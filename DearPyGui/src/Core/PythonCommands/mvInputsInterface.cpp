#include "mvInputsInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddInputWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_input_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "default_value"},
			{mvPythonDataType::String, "hint"},
			{mvPythonDataType::Bool, "multiline"},
			{mvPythonDataType::Bool, "no_spaces", "Filter out spaces, tabs"},
			{mvPythonDataType::Bool, "uppercase"},
			{mvPythonDataType::Bool, "decimal", "Allow 0123456789.+-*/"},
			{mvPythonDataType::Bool, "hexadecimal", "Allow 0123456789ABCDEFabcdef"},
			{mvPythonDataType::Bool, "readonly"},
			{mvPythonDataType::Bool, "password", "Password mode, display all characters as '*'"},
			{mvPythonDataType::Bool, "scientific", "Allow 0123456789.+-*/eE (Scientific notation input)"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds input for text values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds input for integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds input for 2 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds input for 3 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds input for 4 integer values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::Float, "step"},
			{mvPythonDataType::Float, "step_fast"},
		}, "Adds input for float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds input for 2 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds input for 3 float values.", "None", "Adding Widgets") });

		parsers->insert({ "add_input_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds input for 4 float values.", "None", "Adding Widgets") });
	}

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		const char* hint = "";
		int multiline = 0;
		int no_spaces = false;
		int uppercase = false;
		int decimal = false;
		int hexadecimal = false;
		int readonly = false;
		int password = false;
		int scientific = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = true;

		int flags = 0;

		if (!(*mvApp::GetApp()->getParsers())["add_input_text"].parse(args, kwargs, __FUNCTION__, 
			&name, &default_value, &hint, &multiline, &no_spaces,
			&uppercase, &decimal, &hexadecimal, &readonly, &password, &scientific, &callback, 
			&callback_data, &tip, &parent, &before, &source, &width, &height, &on_enter,
			&label, &popup, &show))
			return ToPyBool(false);


		mvAppItem* item = new mvInputText(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &width, &on_enter,
			&label, &popup, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvInputInt(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int2"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &width, &on_enter,
			&label, &popup, &show))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);

		mvAppItem* item = new mvInputInt2(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int3"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &width, &on_enter,
			&label, &popup, &show))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);

		mvAppItem* item = new mvInputInt3(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_int4"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &width, &on_enter,
			&label, &popup, &show))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvInputInt4(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;
		float step = 0.1f;
		float step_fast = 1.0f;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &width, &on_enter,
			&label, &popup, &show, &step, &step_fast))
			return ToPyBool(false);

		mvAppItem* item = new mvInputFloat(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float2"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &width, &on_enter,
			&label, &popup, &show))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);
		mvAppItem* item = new mvInputFloat2(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float3"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &width, &on_enter,
			&label, &popup, &show))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);

		mvAppItem* item = new mvInputFloat3(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		int on_enter = false;
		const char* label = "";
		const char* popup = "";
		int show = false;

		if (!(*mvApp::GetApp()->getParsers())["add_input_float4"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &width, &on_enter,
			&label, &popup, &show))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);

		mvAppItem* item = new mvInputFloat4(name, vec.data(), source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

}
