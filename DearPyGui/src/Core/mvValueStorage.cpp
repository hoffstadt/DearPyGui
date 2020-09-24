#include "mvValueStorage.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvPythonTranslator.h"

namespace Marvel {

	PyObject* mvValueStorage::GetPyValue(const std::string& name)
	{
		if (!HasValue(name))
			return GetPyNone();

		switch (GetType(name))
		{

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
		
		// int
		if (PyLong_Check(value))
			AddIntValue(name, PyLong_AsLong(value));

		// float
		else if (PyFloat_Check(value))
			AddFloatValue(name, PyFloat_AsDouble(value));

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
					AddFloatValue(name, PyFloat_AsDouble(item));
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

	}

	int* mvValueStorage::AddIntValue(const std::string& name, int value)
	{
		// value exists and is compatible type
		if (HasValue(name))
		{
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Int:
				IncrementRef(name);
				return GetIntValue(name);

			case mvValueStorage::ValueTypes::Int2:
				IncrementRef(name);
				return GetInt2Value(name);

			case mvValueStorage::ValueTypes::Int3:
				IncrementRef(name);
				return GetInt3Value(name);

			case mvValueStorage::ValueTypes::Int4:
				IncrementRef(name);
				return GetInt4Value(name);

			default: // incompatible type
				return &s_ints["common"];
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
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Int2:
				IncrementRef(name);
				return GetInt2Value(name);

			case mvValueStorage::ValueTypes::Int3:
				IncrementRef(name);
				return GetInt3Value(name);

			case mvValueStorage::ValueTypes::Int4:
				IncrementRef(name);
				return GetInt4Value(name);

			default: // incompatible type
				return s_int2s["common"].data();
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
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Int3:
				IncrementRef(name);
				return GetInt3Value(name);

			case mvValueStorage::ValueTypes::Int4:
				IncrementRef(name);
				return GetInt4Value(name);

			default: // incompatible type
				return s_int3s["common"].data();
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
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Int4:
				IncrementRef(name);
				return GetInt4Value(name);

			default: // incompatible type
				return s_int4s["common"].data();
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
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Float:
				IncrementRef(name);
				return GetFloatValue(name);

			case mvValueStorage::ValueTypes::Float2:
				IncrementRef(name);
				return GetFloat2Value(name);

			case mvValueStorage::ValueTypes::Float3:
				IncrementRef(name);
				return GetFloat3Value(name);

			case mvValueStorage::ValueTypes::Float4:
				IncrementRef(name);
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::FloatVect:
				if (GetFloatVectorValue(name)->size() > 0)
				{
					IncrementRef(name);
					return GetFloatVectorValue(name)->data();
				}

			default: // incompatible type
				return &s_floats["common"];
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
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Float2:
				IncrementRef(name);
				return GetFloat2Value(name);

			case mvValueStorage::ValueTypes::Float3:
				IncrementRef(name);
				return GetFloat3Value(name);

			case mvValueStorage::ValueTypes::Float4:
				IncrementRef(name);
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::FloatVect:
				if (GetFloatVectorValue(name)->size() > 1)
				{
					IncrementRef(name);
					return GetFloatVectorValue(name)->data();
				}

			default: // incompatible type
				return s_float2s["common"].data();
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
				IncrementRef(name);
				return GetFloat3Value(name);

			case mvValueStorage::ValueTypes::Float4:
				IncrementRef(name);
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::FloatVect:
				if (GetFloatVectorValue(name)->size() > 2)
				{
					IncrementRef(name);
					return GetFloatVectorValue(name)->data();
				}

			default: // incompatible type
				return s_float3s["common"].data();
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
			switch (GetType(name))
			{

			case mvValueStorage::ValueTypes::Float4:
				IncrementRef(name);
				return GetFloat4Value(name);

			case mvValueStorage::ValueTypes::FloatVect:
				if (GetFloatVectorValue(name)->size() > 3)
				{
					IncrementRef(name);
					return GetFloatVectorValue(name)->data();
				}

			default: // incompatible type
				return s_float4s["common"].data();
			}

		}

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Float4;
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
			return &s_floatvects["common"];

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
			return &s_bools["common"];

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
			return &s_strings["common"];

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
			return &s_times["common"];

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
		if (HasValue(name) && GetType(name) == ValueTypes::Int)
			return &s_ints[name];
		return &s_ints["common"];
	}

	int* mvValueStorage::GetInt2Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Int2)
			return s_int2s[name].data();
		return s_int2s["common"].data();
	}

	int* mvValueStorage::GetInt3Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Int3)
			return s_int3s[name].data();
		return s_int3s["common"].data();
	}

	int* mvValueStorage::GetInt4Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Int4)
			return s_int4s[name].data();
		s_int4s["common"].data();
	}

	float* mvValueStorage::GetFloatValue(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Float)
			return &s_floats[name];
		return &s_floats["common"];
	}

	float* mvValueStorage::GetFloat2Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Float2)
			return s_float2s[name].data();
		return s_float2s["common"].data();
	}

	float* mvValueStorage::GetFloat3Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Float3)
			return s_float3s[name].data();
		return s_float3s["common"].data();
	}

	float* mvValueStorage::GetFloat4Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Float4)
			return s_float4s[name].data();
		return s_float4s["common"].data();
	}

	std::vector<float>* mvValueStorage::GetFloatVectorValue(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::FloatVect)
			return &s_floatvects[name];
		return &s_floatvects["common"];
	}

	bool* mvValueStorage::GetBoolValue(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Bool)
			return &s_bools[name];
		return &s_bools["common"];
	}

	std::string* mvValueStorage::GetStringValue(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::String)
			return &s_strings[name];
		return &s_strings["common"];
	}

	tm* mvValueStorage::GetTimeValue(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Time)
			return &s_times[name];
		return &s_times["common"];
	}

	ImPlotTime* mvValueStorage::GetImTimeValue(const std::string& name)
	{
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
			case ValueTypes::String: s_strings.erase(name); break;
			case ValueTypes::Bool: s_bools.erase(name); break;
			case ValueTypes::FloatVect: s_floatvects.erase(name); break;
			case ValueTypes::Time: s_times.erase(name); s_imtimes.erase(name); break;
			}

			s_typeStorage.erase(name);
			s_refStorage.erase(name);
			s_itemStorage.erase(name);

		}
	}

	std::map<std::string, mvValueStorage::ValueTypes> mvValueStorage::s_typeStorage;
	std::map<std::string, int> mvValueStorage::s_refStorage = { {"common",  1} };
	std::set<std::string> mvValueStorage::s_itemStorage = { "common" };
	std::map<std::string, int> mvValueStorage::s_ints = { {"common", 0} };
	std::map<std::string, std::array<int, 2>> mvValueStorage::s_int2s = { {"common", {0, 0}} };
	std::map<std::string, std::array<int, 3>> mvValueStorage::s_int3s = { {"common", {0, 0, 0}} };
	std::map<std::string, std::array<int, 4>> mvValueStorage::s_int4s = { {"common", {0, 0, 0, 0}} };
	std::map<std::string, float> mvValueStorage::s_floats = { {"common", 0.0f} };
	std::map<std::string, std::array<float, 2>> mvValueStorage::s_float2s = { {"common", {0.0f, 0.0f}} };
	std::map<std::string, std::array<float, 3>> mvValueStorage::s_float3s = { {"common", {0.0f, 0.0f, 0.0f}} };
	std::map<std::string, std::array<float, 4>> mvValueStorage::s_float4s = { {"common", {0.0f, 0.0f}} };
	std::map<std::string, bool> mvValueStorage::s_bools = { {"common", true} };
	std::map<std::string, std::string> mvValueStorage::s_strings = { {"common", ""} };
	std::map<std::string, std::vector<float>> mvValueStorage::s_floatvects = { {"common", {0.0f, 0.0f}} };
	std::map<std::string, tm> mvValueStorage::s_times = { {"common", {}} };
	std::map<std::string, ImPlotTime> mvValueStorage::s_imtimes = { {"common", ImPlotTime()} };
}
