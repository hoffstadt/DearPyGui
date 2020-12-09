#include "mvValueStorage.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvValueStorage* mvValueStorage::s_instance = nullptr;

	mvValueStorage* mvValueStorage::GetValueStorage()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvValueStorage();
		return s_instance;
	}

	mvValueStorage::mvValueStorage()
	{
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_VALUES);

		s_refStorage =
		{
			{"common_int", 1},
			{"common_int2", 1},
			{"common_int3", 1},
			{"common_int4", 1},
			{"common_float", 1},
			{"common_float2", 1},
			{"common_float3", 1},
			{"common_float4", 1},
			{"common_bool", 1},
			{"common_string", 1},
			{"common_floatvec", 1},
			{"common_time", 1},
			{"common_imtime", 1},
		};

		s_itemStorage =
		{
			"common_int",
			"common_int2",
			"common_int3",
			"common_int4",
			"common_float",
			"common_float2",
			"common_float3",
			"common_float4",
			"common_bool",
			"common_string",
			"common_floatvec",
			"common_time",
			"common_imtime"
		};
		s_ints = { {"common_int", 0} };
		s_int2s = { {"common_int2", {0, 0}} };
		s_int3s = { {"common_int3", {0, 0, 0}} };
		s_int4s = { {"common_int4", {0, 0, 0, 0}} };
		s_floats = { {"common_float", 0.0f} };
		s_float2s = { {"common_float2", {0.0f, 0.0f}} };
		s_float3s = { {"common_float3", {0.0f, 0.0f, 0.0f}} };
		s_float4s = { {"common_float4", {0.0f, 0.0f}} };
		s_bools = { {"common_bool", true} };
		s_strings = { {"common_string", ""} };
		s_floatvects = { {"common_floatvec", {0.0f, 0.0f}} };
		s_times = { {"common_time", {}} };
		s_imtimes = { {"common_imtime", ImPlotTime()} };
	}

	bool mvValueStorage::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvValueStorage::onPythonSetEvent), mvEVT_PY_SET_VALUE);

		return event.handled;
	}

	bool mvValueStorage::onPythonSetEvent(mvEvent& event)
	{
		
		SetPyValue(GetEString(event, "NAME"), GetEPtr<PyObject*>(event, "VALUE"));

		Py_XDECREF(GetEPtr<PyObject*>(event, "VALUE"));

		return true;
	}

	PyObject* mvValueStorage::GetPyValue(const std::string& name)
	{

		if (!HasValue(name))
			return GetPyNone();

		switch (GetType(name))
		{

		case mvValueStorage::ValueTypes::Color:
		{
			float* fcolor = GetFloat4Value(name);
			mvColor color = { (int)(fcolor[0]*255), (int)(fcolor[1] * 255), (int)(fcolor[2] * 255), (int)(fcolor[3] * 255), true };
			return ToPyColor(color);
		}

		case mvValueStorage::ValueTypes::Int:
			return ToPyInt(*GetIntValue(name));

		case mvValueStorage::ValueTypes::Int2:
			return ToPyIntList(GetInt2Value(name), 2);

		case mvValueStorage::ValueTypes::Int3:
			return ToPyIntList(GetInt3Value(name), 3);

		case mvValueStorage::ValueTypes::Int4:
			return ToPyIntList(GetInt4Value(name), 4);

		case mvValueStorage::ValueTypes::Float:
			return ToPyFloat(*GetFloatValue(name));

		case mvValueStorage::ValueTypes::Float2:
			return ToPyFloatList(GetFloat2Value(name), 2);

		case mvValueStorage::ValueTypes::Float3:
			return ToPyFloatList(GetFloat3Value(name), 3);

		case mvValueStorage::ValueTypes::Float4:
			return ToPyFloatList(GetFloat4Value(name), 4);

		case mvValueStorage::ValueTypes::String:
			return ToPyString(*GetStringValue(name));

		case mvValueStorage::ValueTypes::Bool:
			return ToPyBool(*GetBoolValue(name));

		case mvValueStorage::ValueTypes::FloatVect:
			return ToPyList(*GetFloatVectorValue(name));

		case mvValueStorage::ValueTypes::Time:
			return ToPyTime(*GetTimeValue(name));

		default:
			return GetPyNone();
		}
	}

	bool mvValueStorage::SetPyValue(const std::string& name, PyObject* value)
	{
		
		if (!HasValue(name))
			return false;

		switch (GetType(name))
		{

		case mvValueStorage::ValueTypes::Int:
			*GetIntValue(name) = ToInt(value);
			return true;

		case mvValueStorage::ValueTypes::Int2:
		{
			std::vector<int> temp = ToIntVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 1)
					break;
				GetInt2Value(name)[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Int3:
		{
			std::vector<int> temp = ToIntVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 2)
					break;
				GetInt3Value(name)[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Int4:
		{
			std::vector<int> temp = ToIntVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 3)
					break;
				GetInt4Value(name)[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Float:
			*GetFloatValue(name) = ToFloat(value);
			return true;

		case mvValueStorage::ValueTypes::Float2:
		{
			std::vector<float> temp = ToFloatVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 1)
					break;
				GetFloat2Value(name)[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Float3:
		{
			std::vector<float> temp = ToFloatVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 2)
					break;
				GetFloat3Value(name)[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Float4:
		{
			std::vector<float> temp = ToFloatVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 3)
					break;
				GetFloat4Value(name)[i] = temp[i];
			}
			return true;
		}

		case mvValueStorage::ValueTypes::Color:
		{
			std::vector<float> temp = ToFloatVect(value);
			for (size_t i = 0; i < temp.size(); i++)
			{
				if (i > 3)
					break;
				GetFloat4Value(name)[i] = temp[i]/255.0f;
			}
			return true;
		}

		case mvValueStorage::ValueTypes::String:
			*GetStringValue(name) = ToString(value);
			return true;

		case mvValueStorage::ValueTypes::Bool:
			*GetBoolValue(name) = ToBool(value);
			return true;

		case mvValueStorage::ValueTypes::FloatVect:
			*GetFloatVectorValue(name) = ToFloatVect(value);
			return true;

		case mvValueStorage::ValueTypes::Time:
			*GetTimeValue(name) = ToTime(value);
			ImPlot::GetGmtTime(*GetImTimeValue(name), GetTimeValue(name));
			return true;

		default:
			return false;
		}
	}

	void mvValueStorage::AddPyValue(const std::string& name, PyObject* value)
	{

		mvGlobalIntepreterLock gil;
		
		// bool
		if (PyBool_Check(value))
			AddBoolValue(name, PyLong_AsLong(value));

		// int
		else if (PyLong_Check(value))
			AddIntValue(name, PyLong_AsLong(value));

		// float
		else if (PyFloat_Check(value))
			AddFloatValue(name, (float)PyFloat_AsDouble(value));

		// bool
		else if (PyBool_Check(value))
			AddBoolValue(name, PyLong_AsLong(value));

		// string
		else if (PyUnicode_Check(value))
			AddStringValue(name, _PyUnicode_AsString(value));

		// list
		else if (PyList_Check(value))
		{

			if (PyList_Size(value) == 0)
				return;
			else if (PyList_Size(value) == 1)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					AddIntValue(name, PyLong_AsLong(item));
				else if (PyFloat_Check(item))
					AddFloatValue(name, (float)PyFloat_AsDouble(item));
				else return;
			}

			else if (PyList_Size(value) == 2)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					AddInt2Value(name, { ToInt(PyList_GetItem(value, 0)), ToInt(PyList_GetItem(value, 1)) });
				else if (PyFloat_Check(item))
					AddFloat2Value(name, { ToFloat(PyList_GetItem(value, 0)), ToFloat(PyList_GetItem(value, 1)) });
				else return;
			}

			else if (PyList_Size(value) == 3)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					AddInt3Value(name, { ToInt(PyList_GetItem(value, 0)), ToInt(PyList_GetItem(value, 1)), ToInt(PyList_GetItem(value, 2)) });
				else if (PyFloat_Check(item))
					AddFloat3Value(name, { ToFloat(PyList_GetItem(value, 0)), ToFloat(PyList_GetItem(value, 1)), ToFloat(PyList_GetItem(value, 2)) });
				else return;
			}

			else if (PyList_Size(value) == 4)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					AddInt4Value(name, { ToInt(PyList_GetItem(value, 0)), ToInt(PyList_GetItem(value, 1)), ToInt(PyList_GetItem(value, 2)), ToInt(PyList_GetItem(value, 3)) });
				else if (PyFloat_Check(item))
					AddFloat4Value(name, { ToFloat(PyList_GetItem(value, 0)), ToFloat(PyList_GetItem(value, 1)), ToFloat(PyList_GetItem(value, 2)), ToFloat(PyList_GetItem(value, 3)) });
				else return;
			}

			else
					AddFloatVectorValue(name, ToFloatVect(value));
		}

		// tuple (color)
		else if (PyTuple_Check(value))
		{

			if (PyTuple_Size(value) == 0)
				return;
			else if (PyTuple_Size(value) == 1)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					AddColorValue(name, { (float)PyFloat_AsDouble(item), 0.0f, 0.0f, 255.0f });
				else return;
			}

			else if (PyTuple_Size(value) == 2)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					AddColorValue(name, { ToFloat(PyTuple_GetItem(value, 0)), ToFloat(PyTuple_GetItem(value, 1)), 0.0f, 255.0f});
				else return;
			}

			else if (PyTuple_Size(value) == 3)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					AddColorValue(name, { ToFloat(PyTuple_GetItem(value, 0)), ToFloat(PyTuple_GetItem(value, 1)), ToFloat(PyTuple_GetItem(value, 1)), 255.0f });
				else return;
			}

			else if (PyTuple_Size(value) == 4)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					AddColorValue(name, { ToFloat(PyTuple_GetItem(value, 0)), ToFloat(PyTuple_GetItem(value, 1)), ToFloat(PyTuple_GetItem(value, 2)), ToFloat(PyTuple_GetItem(value, 3)) });
				else return;
			}

		}

	}

	int* mvValueStorage::AddIntValue(const std::string& name, int value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			IncrementRef(name);
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Int:
				return GetIntValue(name);

			case mvValueStorage::ValueTypes::Int2:
				return GetInt2Value(name);

			case mvValueStorage::ValueTypes::Int3:
				return GetInt3Value(name);

			case mvValueStorage::ValueTypes::Int4:
				return GetInt4Value(name);

			default: // incompatible type
				return &s_ints["common_int"];
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Int;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_ints[name] = value;
		return &s_ints[name];
	}

	int* mvValueStorage::AddInt2Value(const std::string& name, const std::array<int, 2>& value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			IncrementRef(name);
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Int2:
				return GetInt2Value(name);

			case mvValueStorage::ValueTypes::Int3:
				return GetInt3Value(name);

			case mvValueStorage::ValueTypes::Int4:
				return GetInt4Value(name);

			default: // incompatible type
				return s_int2s["common_int2"].data();
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Int2;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_int2s[name] = value;
		return s_int2s[name].data();
	}

	int* mvValueStorage::AddInt3Value(const std::string& name, const std::array<int, 3>& value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			IncrementRef(name);
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Int3:
				return GetInt3Value(name);

			case mvValueStorage::ValueTypes::Int4:
				return GetInt4Value(name);

			default: // incompatible type
				return s_int3s["common_int3"].data();
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Int3;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_int3s[name] = value;
		return s_int3s[name].data();
	}

	int* mvValueStorage::AddInt4Value(const std::string& name, const std::array<int, 4>& value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			IncrementRef(name);
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Int4:
				return GetInt4Value(name);

			default: // incompatible type
				return s_int4s["common_int4"].data();
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Int4;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_int4s[name] = value;
		return s_int4s[name].data();
	}

	float* mvValueStorage::AddFloatValue(const std::string& name, float value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			IncrementRef(name);
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Float:
				return GetFloatValue(name);

			case mvValueStorage::ValueTypes::Float2:
				return GetFloat2Value(name);

			case mvValueStorage::ValueTypes::Float3:
				return GetFloat3Value(name);

			case mvValueStorage::ValueTypes::Float4:
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::FloatVect:
				if (GetFloatVectorValue(name)->size() > 0)
					return GetFloatVectorValue(name)->data();

			default: // incompatible type
				return &s_floats["common_float"];
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Float;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_floats[name] = value;
		return &s_floats[name];
	}

	float* mvValueStorage::AddFloat2Value(const std::string& name, const std::array<float, 2>& value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			IncrementRef(name);
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Float2:
				return GetFloat2Value(name);

			case mvValueStorage::ValueTypes::Float3:
				return GetFloat3Value(name);

			case mvValueStorage::ValueTypes::Float4:
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::FloatVect:
				if (GetFloatVectorValue(name)->size() > 1)
					return GetFloatVectorValue(name)->data();

			default: // incompatible type
				return s_float2s["common_float2"].data();
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Float2;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_float2s[name] = value;
		return s_float2s[name].data();
	}

	float* mvValueStorage::AddFloat3Value(const std::string& name, const std::array<float, 3>& value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Float3:
				return GetFloat3Value(name);

			case mvValueStorage::ValueTypes::Float4:
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::FloatVect:
				if (GetFloatVectorValue(name)->size() > 2)
					return GetFloatVectorValue(name)->data();

			default: // incompatible type
				return s_float3s["common_float3"].data();
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Float3;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_float3s[name] = value;
		return s_float3s[name].data();
	}

	float* mvValueStorage::AddFloat4Value(const std::string& name, const std::array<float, 4>& value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			IncrementRef(name);
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Float4:
				
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::FloatVect:
				if (GetFloatVectorValue(name)->size() > 3)
					return GetFloatVectorValue(name)->data();

			default: // incompatible type
				return s_float4s["common_float4"].data();
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Float4;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_float4s[name] = value;
		return s_float4s[name].data();
	}

	float* mvValueStorage::AddColorValue(const std::string& name, const std::array<float, 4>& value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			IncrementRef(name);
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Color:
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::Float4:
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::FloatVect:
				if (GetFloatVectorValue(name)->size() > 3)
					return GetFloatVectorValue(name)->data();

			default: // incompatible type
				return s_float4s["common_float4"].data();
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Color;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_float4s[name] = value;
		return s_float4s[name].data();
	}

	std::vector<float>* mvValueStorage::AddFloatVectorValue(const std::string& name, const std::vector<float>& value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Float4)
		{
			IncrementRef(name);
			return GetFloatVectorValue(name);
		}

		// wrong type
		else if (HasValue(name))
		{
			IncrementRef(name);
			return &s_floatvects["common_float4"];
		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::FloatVect;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_floatvects[name] = value;
		return &s_floatvects[name];
	}

	bool* mvValueStorage::AddBoolValue(const std::string& name, bool value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Bool)
		{
			IncrementRef(name);
			return GetBoolValue(name);
		}

		// wrong type
		else if (HasValue(name))
		{
			IncrementRef(name);
			return &s_bools["common_bool"];
		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Bool;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_bools[name] = value;
		return &s_bools[name];
	}

	std::string* mvValueStorage::AddStringValue(const std::string& name, const std::string& value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::String)
		{
			IncrementRef(name);
			return GetStringValue(name);
		}

		// wrong type
		else if (HasValue(name))
		{
			IncrementRef(name);
			return &s_strings["common_string"];
		}
			

		// doesn't have value
		s_typeStorage[name] = ValueTypes::String;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_strings[name] = value;
		return &s_strings[name];
	}

	tm* mvValueStorage::AddTimeValue(const std::string& name, const tm& value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Time)
		{
			IncrementRef(name);
			return GetTimeValue(name);
		}

		// wrong type
		else if (HasValue(name))
		{
			IncrementRef(name);
			return &s_times["common_time"];
		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Time;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_times[name] = value;
		s_imtimes[name] = ImPlot::MkGmtTime(&s_times[name]);
		return &s_times[name];
	}

	int* mvValueStorage::GetIntValue(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name))
		{
			switch (GetType(name))
			{
			case ValueTypes::Int: return &s_ints[name];
			case ValueTypes::Int2: return s_int2s[name].data();
			case ValueTypes::Int3: return s_int3s[name].data();
			case ValueTypes::Int4: return s_int4s[name].data();
			default: return nullptr; // should not get here; silence compiler warning
			}
		}
		return &s_ints["common"];
	}

	int* mvValueStorage::GetInt2Value(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name))
		{
			switch (GetType(name))
			{
			case ValueTypes::Int2: return s_int2s[name].data();
			case ValueTypes::Int3: return s_int3s[name].data();
			case ValueTypes::Int4: return s_int4s[name].data();
			default: return nullptr; // should not get here; silence compiler warning
			}
		}
		return s_int2s["common"].data();
	}

	int* mvValueStorage::GetInt3Value(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name))
		{
			switch (GetType(name))
			{
			case ValueTypes::Int3: return s_int3s[name].data();
			case ValueTypes::Int4: return s_int4s[name].data();
			default: return nullptr; // should not get here; silence compiler warning
			}
		}
		return s_int3s["common"].data();
	}

	int* mvValueStorage::GetInt4Value(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name) && GetType(name) == ValueTypes::Int4)
			return s_int4s[name].data();
		return s_int4s["common"].data();
	}

	float* mvValueStorage::GetFloatValue(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name))
		{
			switch (GetType(name))
			{
			case ValueTypes::Float: return &s_floats[name];
			case ValueTypes::Float2: return s_float2s[name].data();
			case ValueTypes::Float3: return s_float3s[name].data();
			case ValueTypes::Float4: return s_float4s[name].data();
			case ValueTypes::FloatVect: return s_floatvects[name].data();
			default: return nullptr; // should not get here; silence compiler warning
			}
		}
		return &s_floats["common"];
	}

	float* mvValueStorage::GetFloat2Value(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name))
		{
			switch (GetType(name))
			{
			case ValueTypes::Float2: return s_float2s[name].data();
			case ValueTypes::Float3: return s_float3s[name].data();
			case ValueTypes::Float4: return s_float4s[name].data();
			case ValueTypes::FloatVect: return s_floatvects[name].data();
			default: return nullptr; // should not get here; silence compiler warning
			}
		}
		return s_float2s["common"].data();
	}

	float* mvValueStorage::GetFloat3Value(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name))
		{
			switch (GetType(name))
			{
			case ValueTypes::Float3: return s_float3s[name].data();
			case ValueTypes::Float4: return s_float4s[name].data();
			case ValueTypes::FloatVect: return s_floatvects[name].data();
			default: return nullptr; // should not get here; silence compiler warning
			}
		}
		return s_float3s["common"].data();
	}

	float* mvValueStorage::GetFloat4Value(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name))
		{
			switch (GetType(name))
			{
			case ValueTypes::Color: return s_float4s[name].data();
			case ValueTypes::Float4: return s_float4s[name].data();
			case ValueTypes::FloatVect: return s_floatvects[name].data();
			default: return nullptr; // should not get here; silence compiler warning
			}
		}
		return s_float4s["common"].data();
	}

	std::vector<float>* mvValueStorage::GetFloatVectorValue(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name) && GetType(name) == ValueTypes::FloatVect)
			return &s_floatvects[name];
		return &s_floatvects["common"];
	}

	bool* mvValueStorage::GetBoolValue(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name) && GetType(name) == ValueTypes::Bool)
			return &s_bools[name];
		return &s_bools["common"];
	}

	std::string* mvValueStorage::GetStringValue(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name) && GetType(name) == ValueTypes::String)
			return &s_strings[name];
		return &s_strings["common"];
	}

	tm* mvValueStorage::GetTimeValue(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name) && GetType(name) == ValueTypes::Time)
			return &s_times[name];
		return &s_times["common"];
	}

	ImPlotTime* mvValueStorage::GetImTimeValue(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name) && GetType(name) == ValueTypes::Time)
			return &s_imtimes[name];
		return &s_imtimes["common"];
	}

	bool mvValueStorage::HasValue(const std::string& name)
	{
		return s_itemStorage.count(name) == 1;
	}

	mvValueStorage::ValueTypes mvValueStorage::GetType(const std::string& name)
	{
		if (HasValue(name))
			return s_typeStorage[name];
		return mvValueStorage::ValueTypes::None;
	}

	void mvValueStorage::IncrementRef(const std::string& name)
	{
		if (HasValue(name))
			s_refStorage[name]++;
	}

	void mvValueStorage::DecrementRef(const std::string& name)
	{
		if (HasValue(name))
		{
			s_refStorage[name]--;
			if (s_refStorage[name] == 0)
				DeleteValue(name);
		}
	}

	void mvValueStorage::DeleteValue(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (HasValue(name))
		{
			switch (s_typeStorage[name])
			{
			case ValueTypes::Int: s_ints.erase(name); break;
			case ValueTypes::Int2: s_int2s.erase(name); break;
			case ValueTypes::Int3: s_int3s.erase(name); break;
			case ValueTypes::Int4: s_int4s.erase(name); break;
			case ValueTypes::Float: s_floats.erase(name); break;
			case ValueTypes::Float2: s_float2s.erase(name); break;
			case ValueTypes::Float3: s_float3s.erase(name); break;
			case ValueTypes::Float4: s_float4s.erase(name); break;
			case ValueTypes::Color: s_float4s.erase(name); break;
			case ValueTypes::String: s_strings.erase(name); break;
			case ValueTypes::Bool: s_bools.erase(name); break;
			case ValueTypes::FloatVect: s_floatvects.erase(name); break;
			case ValueTypes::Time: s_times.erase(name); s_imtimes.erase(name); break;
			default: break; // should not get here; silence compiler warning
			}

			s_typeStorage.erase(name);
			s_refStorage.erase(name);
			s_itemStorage.erase(name);

		}
	}

}
