#include "mvDragInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddDragWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_drag_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a single float value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 2 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 3 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 4 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a single int value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 2 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 3 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

		parsers->insert({ "add_drag_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "on_enter", "Only runs callback on enter"},
		}, "Adds drag for a 4 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
	}

	PyObject* add_drag_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		std::vector<float> defaults;
		defaults.push_back(default_value);
		defaults.push_back(0.0f);
		defaults.push_back(0.0f);
		defaults.push_back(0.0f);

		mvAppItem* item = new mvDragFloat<mvAppItemType::DragFloat, 1, ImGui::DragFloat>(name, defaults.data());
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);
		mvAppItem* item = new mvDragFloat<mvAppItemType::DragFloat2, 2, ImGui::DragFloat2>(name, vec.data());
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);
		mvAppItem* item = new mvDragFloat<mvAppItemType::DragFloat3, 3, ImGui::DragFloat3>(name, vec.data());
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 100.0f;
		const char* format = "%.3f";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		auto vec = ToFloatVect(default_value);

		mvAppItem* item = new mvDragFloat<mvAppItemType::DragFloat4, 4, ImGui::DragFloat4>(name, vec.data());
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		std::vector<int> defaults;
		defaults.push_back(default_value);
		defaults.push_back(0.0f);
		defaults.push_back(0.0f);
		defaults.push_back(0.0f);

		mvAppItem* item = new mvDragInt<mvAppItemType::DragInt, 1, ImGui::DragInt>(name, defaults.data());
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvDragInt<mvAppItemType::DragInt2, 2, ImGui::DragInt2>(name, vec.data());
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvDragInt<mvAppItemType::DragInt3, 3, ImGui::DragInt3>(name, vec.data());
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_drag_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int on_enter = false;

		if (!(*mvApp::GetApp()->getParsers())["add_drag_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &callback_data, &tip, &parent, &before, &data_source, &width, &on_enter))
			return ToPyBool(false);

		auto vec = ToIntVect(default_value);
		mvAppItem* item = new mvDragInt<mvAppItemType::DragInt4, 4, ImGui::DragInt4>(name, vec.data());
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		item->setDataSource(data_source);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

}
