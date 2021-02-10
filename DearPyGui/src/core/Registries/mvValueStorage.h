#pragma once

//-----------------------------------------------------------------------------
// mvValueStorage
//
//     - This class acts as a manager for value storage by the users. The 
//       value storage system serves 2 purposes:
//
//         * Allows some widgets to share the same underlying data.
//         * Allows the user to store data for any other purpose where
//           the data can be retrieved at a later time.
//     
//-----------------------------------------------------------------------------

#include <string>
#include <unordered_map>
#include <set>
#include <mutex>
#include <array>
#include <vector>
#include "implot.h"
#include "implot_internal.h"
#include "mvPython.h"
#include "mvAppItemDescription.h"
#include "mvTypeBases.h"

// forward declarations
struct ImPlotTime;

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvValueStorage
	//-----------------------------------------------------------------------------
	class mvValueStorage
	{

		friend class mvDebugWindow;

	public:

		mvValueStorage();
		void RegisterExternalValue(const std::string& name, StorageValueTypes type, mvAppItem* item);
		void DeleteValue(const std::string& name);

#ifndef MV_CPP
		// python interfacing
		PyObject* GetPyValue(const std::string& name);
		bool                SetPyValue          (const std::string& name, PyObject* value);
		void                AddPyValue          (const std::string& name, PyObject* value);
#endif


		bool                HasValue            (const std::string& name);
		bool                HasValueExternal    (const std::string& name);
		StorageValueTypes   GetType             (const std::string& name);
		
		template <typename T>
		void set_value(const std::string& name, T value)
		{
			if (!HasValue(name) && !HasValueExternal(name))
				return;
			std::shared_ptr<T> ptr = get_internal_value<T>(name);
			*ptr = value;

		}

		template <typename T>
		void get_value(const std::string& name, std::shared_ptr<T>& value)
		{
			if (!HasValue(name) && !HasValueExternal(name))
				return;
			value = get_internal_value<T>(name);
		}

		template <typename T>
		std::shared_ptr<T> add_value(const std::string& name, T value)
		{
			if (!HasValue(name))
				s_itemStorage.insert(name);
			return add_internal_value<T>(name, value);
		}

	private:

		template <typename T>
		std::shared_ptr<T> get_internal_value(const std::string& name)
		{
			return {};
		}

		template <typename T>
		std::shared_ptr<T> add_internal_value(const std::string& name, T value)
		{
			return {};
		}

	private:

		std::unordered_map<std::string, StorageValueTypes>    s_typeStorage;  // keeps track of value mapping
		std::set<std::string>                                 s_itemStorage;  // keeps track of registered items
		std::set<std::string>                                 s_itemExternalStorage;  // keeps track of registered items
		std::unordered_map<std::string, mvAppItem*>           s_externalReferences;

		// ints
		std::unordered_map<std::string, std::shared_ptr<int>>                  s_ints; 
		std::unordered_map<std::string, std::shared_ptr<std::array<int, 2>>>   s_int2s;
		std::unordered_map<std::string, std::shared_ptr<std::array<int, 3>>>   s_int3s;
		std::unordered_map<std::string, std::shared_ptr<std::array<int, 4>>>   s_int4s;

		// floats
		std::unordered_map<std::string, std::shared_ptr<float>>                s_floats;
		std::unordered_map<std::string, std::shared_ptr<std::array<float, 2>>> s_float2s;
		std::unordered_map<std::string, std::shared_ptr<std::array<float, 3>>> s_float3s;
		std::unordered_map<std::string, std::shared_ptr<std::array<float, 4>>> s_float4s;
		std::unordered_map<std::string, std::shared_ptr<std::vector<float>>>   s_floatvects;

		// other
		std::unordered_map<std::string, std::shared_ptr<bool>>                 s_bools;
		std::unordered_map<std::string, std::shared_ptr<std::string>>          s_strings;
		
		// time
		std::unordered_map<std::string, std::shared_ptr<tm>>                   s_times;
		std::unordered_map<std::string, std::shared_ptr<ImPlotTime>>           s_imtimes;

	};

	template<>
	inline std::shared_ptr<int> mvValueStorage::add_internal_value(const std::string& name, int value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Int)
				return s_ints["common_int"];
			return s_ints[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Int;
			s_ints[name] = std::make_shared<int>(value);
			return s_ints[name];
		}
	}

	template<>
	inline std::shared_ptr<std::array<int, 2>> mvValueStorage::add_internal_value(const std::string& name, std::array<int, 2> value)
	{

		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Int2)
				return s_int2s["common_int2"];
			return s_int2s[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Int2;
			s_int2s[name] = std::make_shared<std::array<int, 2>>(value);
			return s_int2s[name];
		}
	}

	template<>
	inline std::shared_ptr<std::array<int, 3>> mvValueStorage::add_internal_value(const std::string& name, std::array<int, 3> value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Int3)
				return s_int3s["common_int3"];
			return s_int3s[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Int3;
			s_int3s[name] = std::make_shared<std::array<int, 3>>(value);
			return s_int3s[name];
		}
	}

	template<>
	inline std::shared_ptr<std::array<int, 4>> mvValueStorage::add_internal_value(const std::string& name, std::array<int, 4> value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Int4)
				return s_int4s["common_int4"];
			return s_int4s[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Int4;
			s_int4s[name] = std::make_shared<std::array<int, 4>>(value);
			return s_int4s[name];
		}
	}

	template<>
	inline std::shared_ptr<float> mvValueStorage::add_internal_value(const std::string& name, float value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Float)
				return s_floats["common_float"];
			return s_floats[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Float;
			s_floats[name] = std::make_shared<float>(value);
			return s_floats[name];
		}
	}

	template<>
	inline std::shared_ptr<std::array<float, 2>> mvValueStorage::add_internal_value(const std::string& name, std::array<float, 2> value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Float2)
				return s_float2s["common_float2"];
			return s_float2s[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Float2;
			s_float2s[name] = std::make_shared<std::array<float, 2>>(value);
			return s_float2s[name];
		}
	}

	template<>
	inline std::shared_ptr<std::array<float, 3>> mvValueStorage::add_internal_value(const std::string& name, std::array<float, 3> value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Float3)
				return s_float3s["common_float3"];
			return s_float3s[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Float3;
			s_float3s[name] = std::make_shared<std::array<float, 3>>(value);
			return s_float3s[name];
		}
	}

	template<>
	inline std::shared_ptr<std::array<float, 4>> mvValueStorage::add_internal_value(const std::string& name, std::array<float, 4> value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Float4)
				return s_float4s["common_float4"];
			return s_float4s[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Float4;
			s_float4s[name] = std::make_shared<std::array<float, 4>>(value);
			return s_float4s[name];
		}
	}

	template<>
	inline std::shared_ptr<std::vector<float>> mvValueStorage::add_internal_value(const std::string& name, std::vector<float> value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::FloatVect)
				return s_floatvects["common_int"];
			
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::FloatVect;
			s_floatvects[name] = std::make_shared<std::vector<float>>(value);
		}

		return s_floatvects[name];
	}

	template<>
	inline std::shared_ptr<bool> mvValueStorage::add_internal_value(const std::string& name, bool value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Bool)
				return s_bools["common_bool"];
			return s_bools[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Bool;
			s_bools[name] = std::make_shared<bool>(value);
			return s_bools[name];
		}
	}

	template<>
	inline std::shared_ptr<std::string> mvValueStorage::add_internal_value(const std::string& name, std::string value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::String)
				return s_strings["common_string"];
			return s_strings[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::String;
			s_strings[name] = std::make_shared<std::string>(value);
			return s_strings[name];
		}
	}

	template<>
	inline std::shared_ptr<tm> mvValueStorage::add_internal_value(const std::string& name, tm value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Time)
				return s_times["common_time"];
			return s_times[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Time;
			s_times[name] = std::make_shared<tm>(value);
			return s_times[name];
		}
	}

	template<>
	inline std::shared_ptr<ImPlotTime> mvValueStorage::add_internal_value(const std::string& name, ImPlotTime value)
	{
		if (HasValue(name))
		{
			if (GetType(name) != StorageValueTypes::Time)
				return s_imtimes["common_imtime"];
			return s_imtimes[name];
		}
		else
		{
			s_typeStorage[name] = StorageValueTypes::Time;
			s_imtimes[name] = std::make_shared<ImPlotTime>(value);
			return s_imtimes[name];
		}
	}

	//-----------------------------------------------------------------------------
	// Template Specializations
	//-----------------------------------------------------------------------------
	template<>
	inline std::shared_ptr<int> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvIntPtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_ints[name];
	}

	template<>
	inline std::shared_ptr<std::array<int, 2>> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvInt2PtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_int2s[name];
	}

	template<>
	inline std::shared_ptr<std::array<int, 3>> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvInt3PtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_int3s[name];
	}

	template<>
	inline std::shared_ptr<std::array<int, 4>> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvInt4PtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_int4s[name];
	}

	template<>
	inline std::shared_ptr<float> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvFloatPtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_floats[name];
	}

	template<>
	inline std::shared_ptr<std::array<float, 2>> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvFloat2PtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_float2s[name];
	}

	template<>
	inline std::shared_ptr<std::array<float, 3>> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvFloat3PtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_float3s[name];
	}

	template<>
	inline std::shared_ptr<std::array<float, 4>> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvFloat4PtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_float4s[name];
	}

	template<>
	inline std::shared_ptr<std::vector<float>> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvFloatVectPtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_floatvects[name];
	}

	template<>
	inline std::shared_ptr<bool> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			mvBoolPtrBase* item = (mvBoolPtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_bools[name];
	}

	template<>
	inline std::shared_ptr<std::string> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvStringPtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_strings[name];
	}

	template<>
	inline std::shared_ptr<tm> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvTimePtrBase*)s_externalReferences[name];
			return item->getValue();
		}
		return s_times[name];
	}

	template<>
	inline std::shared_ptr<ImPlotTime> mvValueStorage::get_internal_value(const std::string& name)
	{
		if (HasValueExternal(name))
		{
			auto item = (mvTimePtrBase*)s_externalReferences[name];
			return item->getValue2();
		}
		return s_imtimes[name];
	}

}