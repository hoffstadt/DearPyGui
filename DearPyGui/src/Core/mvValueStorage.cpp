#include "mvValueStorage.h"
#include "mvApp.h"
#include "mvAppLog.h"

namespace Marvel {

	std::map<std::string, mvValueStorage::ValueTypes> mvValueStorage::s_typeStorage;
	std::map<std::string, int> mvValueStorage::s_refStorage;
	std::set<std::string> mvValueStorage::s_itemStorage;
	std::map<std::string, int> mvValueStorage::s_ints;
	std::map<std::string, std::array<int, 2>> mvValueStorage::s_int2s;
	std::map<std::string, std::array<int, 3>> mvValueStorage::s_int3s;
	std::map<std::string, std::array<int, 4>> mvValueStorage::s_int4s;
	std::map<std::string, float> mvValueStorage::s_floats;
	std::map<std::string, std::array<float, 2>> mvValueStorage::s_float2s;
	std::map<std::string, std::array<float, 3>> mvValueStorage::s_float3s;
	std::map<std::string, std::array<float, 4>> mvValueStorage::s_float4s;
	std::map<std::string, bool> mvValueStorage::s_bools;
	std::map<std::string, std::string> mvValueStorage::s_strings;


	int* mvValueStorage::AddIntValue(const std::string& name, int value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Int)
		{
			IncrementRef(name);
			return GetIntValue(name);
		}

		// wrong type
		else if (HasValue(name))
			return nullptr;

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Int;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_ints[name] = value;
		return &s_ints[name];
	}

	int* mvValueStorage::AddInt2Value(const std::string& name, const std::array<int, 2>& value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Int2)
		{
			IncrementRef(name);
			return GetInt2Value(name);
		}

		// wrong type
		else if (HasValue(name))
			return nullptr;

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Int2;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_int2s[name] = value;
		return s_int2s[name].data();
	}

	int* mvValueStorage::AddInt3Value(const std::string& name, const std::array<int, 3>& value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Int3)
		{
			IncrementRef(name);
			return GetInt3Value(name);
		}

		// wrong type
		else if (HasValue(name))
			return nullptr;

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Int3;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_int3s[name] = value;
		return s_int3s[name].data();
	}

	int* mvValueStorage::AddInt4Value(const std::string& name, const std::array<int, 4>& value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Int4)
		{
			IncrementRef(name);
			return GetInt4Value(name);
		}

		// wrong type
		else if (HasValue(name))
			return nullptr;

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Int4;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_int4s[name] = value;
		return s_int4s[name].data();
	}

	float* mvValueStorage::AddFloatValue(const std::string& name, float value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Float)
		{
			IncrementRef(name);
			return GetFloatValue(name);
		}

		// wrong type
		else if (HasValue(name))
			return nullptr;

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Float;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_floats[name] = value;
		return &s_floats[name];
	}

	float* mvValueStorage::AddFloat2Value(const std::string& name, const std::array<float, 2>& value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Float2)
		{
			IncrementRef(name);
			return GetFloat2Value(name);
		}

		// wrong type
		else if (HasValue(name))
			return nullptr;

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Float2;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_float2s[name] = value;
		return s_float2s[name].data();
	}

	float* mvValueStorage::AddFloat3Value(const std::string& name, const std::array<float, 3>& value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Float3)
		{
			IncrementRef(name);
			return GetFloat3Value(name);
		}

		// wrong type
		else if (HasValue(name))
			return nullptr;

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Float3;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_float3s[name] = value;
		return s_float3s[name].data();
	}

	float* mvValueStorage::AddFloat4Value(const std::string& name, const std::array<float, 4>& value)
	{
		// value exists and is proper type
		if (HasValue(name) && GetType(name) == ValueTypes::Float4)
		{
			IncrementRef(name);
			return GetFloat4Value(name);
		}

		// wrong type
		else if (HasValue(name))
			return nullptr;

		// doesn't have value
		s_typeStorage[name] = ValueTypes::Float4;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_float4s[name] = value;
		return s_float4s[name].data();
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
			return nullptr;

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
			return nullptr;

		// doesn't have value
		s_typeStorage[name] = ValueTypes::String;
		s_refStorage[name] = 1;
		s_itemStorage.insert(name);
		s_strings[name] = value;
		return &s_strings[name];
	}

	int* mvValueStorage::GetIntValue(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Int)
		{
			IncrementRef(name);
			return &s_ints[name];
		}
		return nullptr;
	}

	int* mvValueStorage::GetInt2Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Int2)
		{
			IncrementRef(name);
			return s_int2s[name].data();
		}
		return nullptr;
	}

	int* mvValueStorage::GetInt3Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Int3)
		{
			IncrementRef(name);
			return s_int3s[name].data();
		}
		return nullptr;
	}

	int* mvValueStorage::GetInt4Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Int4)
		{
			IncrementRef(name);
			return s_int4s[name].data();
		}
		return nullptr;
	}

	float* mvValueStorage::GetFloatValue(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Float)
		{
			IncrementRef(name);
			return &s_floats[name];
		}
		return nullptr;
	}

	float* mvValueStorage::GetFloat2Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Float2)
		{
			IncrementRef(name);
			return s_float2s[name].data();
		}
		return nullptr;
	}

	float* mvValueStorage::GetFloat3Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Float3)
		{
			IncrementRef(name);
			return s_float3s[name].data();
		}
		return nullptr;
	}

	float* mvValueStorage::GetFloat4Value(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Float4)
		{
			IncrementRef(name);
			return s_float4s[name].data();
		}
		return nullptr;
	}

	bool* mvValueStorage::GetBoolValue(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::Bool)
		{
			IncrementRef(name);
			return &s_bools[name];
		}
		return nullptr;
	}

	std::string* mvValueStorage::GetStringValue(const std::string& name)
	{
		if (HasValue(name) && GetType(name) == ValueTypes::String)
		{
			IncrementRef(name);
			return &s_strings[name];
		}
		return nullptr;
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
			s_refStorage[name];
			if (s_refStorage[name] == 0)
				DeleteValue(name);
		}
	}

	void mvValueStorage::DeleteValue(const std::string& name)
	{
		if (HasValue(name))
		{
			if(s_typeStorage[name] == ValueTypes::Int)
				s_ints.erase(name);
			s_typeStorage.erase(name);
			s_refStorage.erase(name);
			s_itemStorage.erase(name);
		}
	}

}
