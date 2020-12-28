#include "mvSliderInterface.h"

namespace Marvel {

	void AddSliderWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_slider_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value", "", "0.0"},
			{mvPythonDataType::Float, "min_value", "", "0.0"},
			{mvPythonDataType::Float, "max_value", "", "100.0"},
			{mvPythonDataType::String, "format", "", "'%.3f'"},
			{mvPythonDataType::Bool, "vertical", "sets orientation to vertical", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","Height of a vertical slider", "0"},
			{mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
			{mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds slider for a single float value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0)"},
			{mvPythonDataType::Float, "min_value", "", "0.0"},
			{mvPythonDataType::Float, "max_value", "", "100.0"},
			{mvPythonDataType::String, "format", "", "'%.3f'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
			{mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds slider for a 2 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0)"},
			{mvPythonDataType::Float, "min_value", "", "0.0"},
			{mvPythonDataType::Float, "max_value", "", "100.0"},
			{mvPythonDataType::String, "format", "", "'%.3f'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
			{mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds slider for a 3 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0, 0.0)"},
			{mvPythonDataType::Float, "min_value", "", "0.0"},
			{mvPythonDataType::Float, "max_value", "", "100.0"},
			{mvPythonDataType::String, "format", "", "'%.3f'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
			{mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds slider for a 4 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value", "", "0"},
			{mvPythonDataType::Integer, "min_value", "", "0"},
			{mvPythonDataType::Integer, "max_value", "", "100"},
			{mvPythonDataType::String, "format", "", "'%d'"},
			{mvPythonDataType::Bool, "vertical", "sets orientation to vertical", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","Height of a vertical slider", "0"},
			{mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
			{mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds slider for a single int value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value", "", "(0, 0)"},
			{mvPythonDataType::Integer, "min_value", "", "0"},
			{mvPythonDataType::Integer, "max_value", "", "100"},
			{mvPythonDataType::String, "format", "", "'%d'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
			{mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds slider for a 2 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value", "", "(0, 0, 0)"},
			{mvPythonDataType::Integer, "min_value", "", "0"},
			{mvPythonDataType::Integer, "max_value", "", "100"},
			{mvPythonDataType::String, "format", "", "'%d'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
			{mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds slider for a 3 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_slider_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value", "", "(0, 0, 0, 0)"},
			{mvPythonDataType::Integer, "min_value", "", "0"},
			{mvPythonDataType::Integer, "max_value", "", "100"},
			{mvPythonDataType::String, "format", "", "'%d'"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
			{mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds slider for a 4 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
	}

	PyObject* add_slider_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		int vertical = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int enabled = true;
		int width = 0;
		int height = 0;
		int no_input = false;
		int clamped = false;
		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &vertical, &callback, &callback_data, &tip, &parent, &before,
			&source, &enabled, &width, &height, &no_input, &clamped, &label, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvSliderFloat>(name, default_value, source);
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

	PyObject* add_slider_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int enabled = true;
		int width = 0;
		int no_input = false;
		int clamped = false;
		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width,
			&no_input, &clamped, &label, &show))
			return ToPyBool(false);


		auto vec = ToFloatVect(default_value);

		auto item = CreateRef<mvSliderFloat2>(name, vec.data(), source);
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

	PyObject* add_slider_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int enabled = true;
		int width = 0;
		int no_input = false;
		int clamped = false;
		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled, &width,
			&no_input, &clamped, &label, &show))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);

		auto item = CreateRef<mvSliderFloat3>(name, vec.data(), source);
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

	PyObject* add_slider_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int enabled = true;
		int width = 0;
		int no_input = false;
		int clamped = false;
		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled,
			&width, &no_input, &clamped, &label, &show))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);
		auto item = CreateRef<mvSliderFloat4>(name, vec.data(), source);
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

	PyObject* add_slider_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		int vertical = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int enabled = true;
		int width = 0;
		int height = 0;
		int no_input = false;
		int clamped = false;
		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &vertical, &callback, &callback_data, &tip, &parent, &before, &source, &enabled,
			&width, &height, &no_input, &clamped, &label, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvSliderInt>(name, default_value, source);
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

	PyObject* add_slider_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int enabled = true;
		int width = 0;
		int no_input = false;
		int clamped = false;
		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled,
			&width, &no_input, &clamped, &label, &show))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);
		auto item = CreateRef<mvSliderInt2>(name, vec.data(), source);
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

	PyObject* add_slider_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int enabled = true;
		int width = 0;
		int no_input = false;
		int clamped = false;
		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled,
			&width, &no_input, &clamped, &label, &show))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);
		auto item = CreateRef<mvSliderInt3>(name, vec.data(), source);
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

	PyObject* add_slider_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int enabled = true;
		int width = 0;
		int no_input = false;
		int clamped = false;
		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_slider_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &source, &enabled,
			&width, &no_input, &clamped, &label, &show))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);
		auto item = CreateRef<mvSliderInt4>(name, vec.data(), source);
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
