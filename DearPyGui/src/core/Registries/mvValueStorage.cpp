#include "mvValueStorage.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvAppItem.h"

namespace Marvel {

	mvValueStorage::mvValueStorage()
	{

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
		s_ints = { {"common_int", std::make_shared<int>(0)} };
		s_int2s = { {"common_int2", std::make_shared<std::array<int, 2>>(std::array{0, 0})} };
		s_int3s = { {"common_int3", std::make_shared<std::array<int, 3>>(std::array{0, 0, 0})} };
		s_int4s = { {"common_int4", std::make_shared<std::array<int, 4>>(std::array{0, 0, 0, 0})} };
		s_floats = { {"common_float", std::make_shared<float>(0.0f)} };
		s_float2s = { {"common_float2", std::make_shared<std::array<float, 2>>(std::array{0.0f, 0.0f})} };
		s_float3s = { {"common_float3", std::make_shared<std::array<float, 3>>(std::array{0.0f, 0.0f, 0.0f})} };
		s_float4s = { {"common_float4", std::make_shared<std::array<float, 4>>(std::array{0.0f, 0.0f, 0.0f, 0.0f})} };
		s_bools = { {"common_bool", std::make_shared<bool>(true)} };
		s_strings = { {"common_string", std::make_shared<std::string>("")} };
		s_floatvects = { {"common_floatvec", std::make_shared<std::vector<float>>(std::vector{0.0f, 0.0f})} };
		s_times = { {"common_time", {}} };
		s_imtimes = { {"common_imtime", std::make_shared<ImPlotTime>()} };
	}

	bool mvValueStorage::HasValue(const std::string& name)
	{
		return s_itemStorage.count(name) == 1;
	}

	bool mvValueStorage::HasValueExternal(const std::string& name)
	{
		return s_itemExternalStorage.count(name) == 1;
	}

	void mvValueStorage::RegisterExternalValue(const std::string& name, StorageValueTypes type, mvAppItem* item)
	{
		s_itemExternalStorage.insert(name);
		s_typeStorage[name] = type;
		s_externalReferences[name] = item;

	}

	StorageValueTypes mvValueStorage::GetType(const std::string& name)
	{
		if (HasValue(name) || HasValueExternal(name))
			return s_typeStorage[name];
		return StorageValueTypes::None;
	}

	void mvValueStorage::DeleteValue(const std::string& name)
	{

		s_ints.erase(name); 
		s_int2s.erase(name);
		s_int3s.erase(name);
		s_int4s.erase(name);
		s_floats.erase(name); 
		s_float2s.erase(name);
		s_float3s.erase(name);
		s_float4s.erase(name);
		s_float4s.erase(name);
		s_strings.erase(name);
		s_bools.erase(name);
		s_floatvects.erase(name);
		s_times.erase(name); 
		s_imtimes.erase(name);

		s_typeStorage.erase(name);
		s_itemStorage.erase(name);
		s_itemExternalStorage.erase(name);

	}


#ifndef MV_CPP

	PyObject* mvValueStorage::GetPyValue(const std::string& name)
	{

		if (!HasValue(name) && !HasValueExternal(name))
			return GetPyNone();

		switch (GetType(name))
		{

		case StorageValueTypes::Color:
		{
			mvRef<std::array<float, 4>> result;
			get_value(name, result);
			mvColor color = { (int)(result->data()[0] * 255), (int)(result->data()[1] * 255), (int)(result->data()[2] * 255), (int)(result->data()[3] * 255), true };
			return ToPyColor(color);
		}

		case StorageValueTypes::Int:
		{
			mvRef<int> result;
			get_value(name, result);
			return ToPyInt(*result);
		}

		case StorageValueTypes::Int2:
		{
			mvRef<std::array<int, 2>> result;
			get_value(name, result);
			return ToPyIntList(result->data(), 2);
		}

		case StorageValueTypes::Int3:
		{
			mvRef<std::array<int, 3>> result;
			get_value(name, result);
			return ToPyIntList(result->data(), 3);
		}

		case StorageValueTypes::Int4:
		{
			mvRef<std::array<int, 4>> result;
			get_value(name, result);
			return ToPyIntList(result->data(), 4);
		}

		case StorageValueTypes::Float:
		{
			mvRef<float> result;
			get_value(name, result);
			return ToPyFloat(*result);
		}

		case StorageValueTypes::Float2:
		{
			mvRef<std::array<float, 2>> result;
			get_value(name, result);
			return ToPyFloatList(result->data(), 2);
		}

		case StorageValueTypes::Float3:
		{
			mvRef<std::array<float, 3>> result;
			get_value(name, result);
			return ToPyFloatList(result->data(), 3);
		}

		case StorageValueTypes::Float4:
		{
			mvRef<std::array<float, 4>> result;
			get_value(name, result);
			return ToPyFloatList(result->data(), 4);
		}

		case StorageValueTypes::String:
		{
			mvRef<std::string> result;
			get_value(name, result);
			return ToPyString(*result);
		}

		case StorageValueTypes::Bool:
		{
			mvRef<bool> result;
			get_value(name, result);
			return ToPyBool(*result);
		}

		case StorageValueTypes::FloatVect:
		{
			mvRef<std::vector<float>> result;
			get_value(name, result);
			return ToPyList(*result);
		}

		case StorageValueTypes::Time:
		{
			mvRef<tm> result;
			get_value(name, result);
			return ToPyTime(*result);
		}

		default:
			return GetPyNone();
		}
	}

	bool mvValueStorage::SetPyValue(const std::string& name, PyObject* value)
	{

		if (!HasValue(name) && !HasValueExternal(name))
			return false;

		switch (GetType(name))
		{

		case StorageValueTypes::Int:
			set_value(name, ToInt(value));
			return true;

		case StorageValueTypes::Int2:
		{
			std::vector<int> temp = ToIntVect(value);
			std::array<int, 2> temp_array;
			for (int i = 0; i < temp_array.size(); i++)
				temp_array[i] = temp[i];
			set_value(name, temp_array);
			return true;
		}

		case StorageValueTypes::Int3:
		{
			std::vector<int> temp = ToIntVect(value);
			std::array<int, 3> temp_array;
			for (int i = 0; i < temp_array.size(); i++)
				temp_array[i] = temp[i];
			set_value(name, temp_array);
			return true;
		}

		case StorageValueTypes::Int4:
		{
			std::vector<int> temp = ToIntVect(value);
			std::array<int, 4> temp_array;
			for (int i = 0; i < temp_array.size(); i++)
				temp_array[i] = temp[i];
			set_value(name, temp_array);
			return true;
		}

		case StorageValueTypes::Float:
			set_value(name, ToFloat(value));
			return true;

		case StorageValueTypes::Float2:
		{
			std::vector<float> temp = ToFloatVect(value);
			std::array<float, 2> temp_array;
			for (int i = 0; i < temp_array.size(); i++)
				temp_array[i] = temp[i];
			set_value(name, temp_array);
			return true;
		}

		case StorageValueTypes::Float3:
		{
			std::vector<float> temp = ToFloatVect(value);
			std::array<float, 3> temp_array;
			for (int i = 0; i < temp_array.size(); i++)
				temp_array[i] = temp[i];
			set_value(name, temp_array);
			return true;
		}

		case StorageValueTypes::Float4:
		{
			std::vector<float> temp = ToFloatVect(value);
			std::array<float, 4> temp_array;
			for (int i = 0; i < temp_array.size(); i++)
				temp_array[i] = temp[i];
			set_value(name, temp_array);
			return true;
		}

		case StorageValueTypes::Color:
		{
			std::vector<float> temp = ToFloatVect(value);
			std::array<float, 4> temp_array;
			for (int i = 0; i < temp_array.size(); i++)
				temp_array[i] = temp[i];
			set_value(name, temp_array);
			return true;
		}

		case StorageValueTypes::String:
			set_value(name, ToString(value));
			return true;

		case StorageValueTypes::Bool:
			set_value(name, ToBool(value));
			return true;

		case StorageValueTypes::FloatVect:
			set_value(name, ToFloatVect(value));
			return true;

		case StorageValueTypes::Time:
		{
			set_value(name, ToTime(value));
			std::shared_ptr<tm> time;
			get_value(name, time);
			std::shared_ptr<ImPlotTime> imtime;
			get_value(name, imtime);
			ImPlot::GetGmtTime(*imtime, time.get());
			return true;
		}


		default:
			return false;
		}
	}

	void mvValueStorage::AddPyValue(const std::string& name, PyObject* value)
	{

		// bool
		if (PyBool_Check(value))
			add_value(name, (bool)PyLong_AsLong(value));

		// int
		else if (PyLong_Check(value))
			add_value(name, (int)PyLong_AsLong(value));

		// float
		else if (PyFloat_Check(value))
			add_value(name, (float)PyFloat_AsDouble(value));

		// bool
		else if (PyBool_Check(value))
			add_value(name, (bool)PyLong_AsLong(value));

		// string
		else if (PyUnicode_Check(value))
			add_value(name, std::string(_PyUnicode_AsString(value)));

		// list
		else if (PyList_Check(value))
		{

			if (PyList_Size(value) == 0)
				return;
			else if (PyList_Size(value) == 1)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					add_value(name, (int)PyLong_AsLong(item));
				else if (PyFloat_Check(item))
					add_value(name, (float)PyFloat_AsDouble(item));
				else return;
			}

			else if (PyList_Size(value) == 2)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					add_value(name, std::array{ ToInt(PyList_GetItem(value, 0)), ToInt(PyList_GetItem(value, 1)) });
				else if (PyFloat_Check(item))
					add_value(name, std::array{ ToFloat(PyList_GetItem(value, 0)), ToFloat(PyList_GetItem(value, 1)) });
				else return;
			}

			else if (PyList_Size(value) == 3)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					add_value(name, std::array{ ToInt(PyList_GetItem(value, 0)), ToInt(PyList_GetItem(value, 1)), ToInt(PyList_GetItem(value, 2)) });
				else if (PyFloat_Check(item))
					add_value(name, std::array{ ToFloat(PyList_GetItem(value, 0)), ToFloat(PyList_GetItem(value, 1)), ToFloat(PyList_GetItem(value, 2)) });
				else return;
			}

			else if (PyList_Size(value) == 4)
			{
				PyObject* item = PyList_GetItem(value, 0);
				if (PyLong_Check(item))
					add_value(name, std::array{ ToInt(PyList_GetItem(value, 0)), ToInt(PyList_GetItem(value, 1)), ToInt(PyList_GetItem(value, 2)), ToInt(PyList_GetItem(value, 3)) });
				else if (PyFloat_Check(item))
					add_value(name, std::array{ ToFloat(PyList_GetItem(value, 0)), ToFloat(PyList_GetItem(value, 1)), ToFloat(PyList_GetItem(value, 2)), ToFloat(PyList_GetItem(value, 3)) });
				else return;
			}

			else
				add_value(name, ToFloatVect(value));
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
					add_value(name, std::array{ (float)PyFloat_AsDouble(item), 0.0f, 0.0f, 255.0f });
				else return;
			}

			else if (PyTuple_Size(value) == 2)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					add_value(name, std::array{ ToFloat(PyTuple_GetItem(value, 0)), ToFloat(PyTuple_GetItem(value, 1)), 0.0f, 255.0f });
				else return;
			}

			else if (PyTuple_Size(value) == 3)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					add_value(name, std::array{ ToFloat(PyTuple_GetItem(value, 0)), ToFloat(PyTuple_GetItem(value, 1)), ToFloat(PyTuple_GetItem(value, 1)), 255.0f });
				else return;
			}

			else if (PyTuple_Size(value) == 4)
			{
				PyObject* item = PyTuple_GetItem(value, 0);
				if (PyNumber_Check(item))
					add_value(name, std::array{ ToFloat(PyTuple_GetItem(value, 0)), ToFloat(PyTuple_GetItem(value, 1)), ToFloat(PyTuple_GetItem(value, 2)), ToFloat(PyTuple_GetItem(value, 3)) });
				else return;
			}

		}

	}

#endif // !MV_CPP


}
