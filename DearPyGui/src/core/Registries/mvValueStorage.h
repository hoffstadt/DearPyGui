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

#include <array>
#include <implot.h>
#include <implot_internal.h>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
namespace Marvel
{

//-----------------------------------------------------------------------------
// mvValueStorage
//-----------------------------------------------------------------------------

	class mvValueStorage
	{

		friend class mvDebugWindow;

	public:
		enum class ValueTypes
		{
			None = 0,
			Int,
			Int2,
			Int3,
			Int4,
			Float,
			Float2,
			Float3,
			Float4,
			FloatVect,
			Bool,
			String,
			Time,
			Color,
			TypeCount,
		};

		static std::string vtToString(ValueTypes vt)
		{
			switch (vt)
			{
			case ValueTypes::None:
				return "None";
			case ValueTypes::Int:
				return "Int";
			case ValueTypes::Int2:
				return "Int2";
			case ValueTypes::Int3:
				return "Int3";
			case ValueTypes::Int4:
				return "Int4";
			case ValueTypes::Float:
				return "Float";
			case ValueTypes::Float2:
				return "Float2";
			case ValueTypes::Float3:
				return "Float3";
			case ValueTypes::Float4:
				return "Float4";
			case ValueTypes::FloatVect:
				return "FloatVect";
			case ValueTypes::Bool:
				return "Bool";
			case ValueTypes::String:
				return "String";
			case ValueTypes::Time:
				return "Time";
			case ValueTypes::Color:
				return "Color";
			default:
				return "None";
			}

		}

		// python interfacing
		static PyObject* GetPyValue(const std::string& name);
		static bool SetPyValue(const std::string& name, PyObject* value);
		static void AddPyValue(const std::string& name, PyObject* value);

		static bool HasValue(const std::string& name);
		template<typename T>
		static T* AddValue(const std::string& name, const T& value = {})
		{
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_value_store < T >.find(name) == s_value_store < T >.end()) {
				s_value_store < T >.emplace(name, value);
				s_refStorage[name] = 1;
				s_itemStorage.insert(name);
				SetType(name, value);

//				std::cout << "# AddValue # stored value for item \t[ " << name.c_str()
//						  << " ]\t of type " << vtToString(GetType(name)).c_str() << "\n";
				return &s_value_store<T>[name];
			} else {
//				std::cout << "# AddValue # returned existing reference to \t[ " << name.c_str()
//						  << " ]\t of type " << vtToString(GetType(name)).c_str() << "\n";
				IncrementRef(name);
				return &s_value_store < T >.at(name);
			}
		}

		template<typename T> static T* GetValue(const std::string& name)
		{
			std::lock_guard<std::mutex> lock(s_mutex);
			auto result = s_value_store < T >.find(name);
			if (result == s_value_store < T >.end()) {
				return &s_value_store < T >.at("common");
			} else {
				return &s_value_store<T>[name];
			}
		}

		template<typename T>
		static bool UpdateValue(const std::string& name, const T& value)
		{
			std::lock_guard<std::mutex> lock(s_mutex);
			auto result = s_value_store < T >.find(name);
			if (result == s_value_store < T >.end()) {
				return false;
			} else {
				s_value_store < T >.emplace(name, value);
				return true;
			}
		}


		static ValueTypes GetType(const std::string& name);

		template<typename T> static void DeleteValue(const std::string& name)
		{
			std::lock_guard<std::mutex> lock(s_mutex);

			if (s_value_store < T >.find(name) != s_value_store < T >.end()) {
//				std::cout << "-- DeleteValue # item \t[ " << name.c_str() << " ]\n";
				s_value_store < T >.erase(name);
				s_refStorage.erase(name);
				s_itemStorage.erase(name);
			}
		}
		template<typename T> static T* GetDefaultValue()
		{
			std::lock_guard<std::mutex> lock(s_mutex);
			return &s_value_store < T >.at("common");
		}

		static void IncrementRef(const std::string& name);
		static void DecrementRef(const std::string& name);

		template<typename T> static void SetType(const std::string& name, T)
		{
		};
		static void SetType(const std::string& name, const int)
		{
			s_typeStorage[name] = ValueTypes::Int;
		};
		static void SetType(const std::string& name, const std::array<int, 2>&)
		{
			s_typeStorage[name] = ValueTypes::Int2;
		};
		static void SetType(const std::string& name, const std::array<int, 3>&)
		{
			s_typeStorage[name] = ValueTypes::Int3;
		};
		static void SetType(const std::string& name, const std::array<int, 4>&)
		{
			s_typeStorage[name] = ValueTypes::Int4;
		};
		static void SetType(const std::string& name, const float)
		{
			s_typeStorage[name] = ValueTypes::Float;
		};
		static void SetType(const std::string& name, const std::array<float, 2>&)
		{
			s_typeStorage[name] = ValueTypes::Float2;
		};
		static void SetType(const std::string& name, const std::array<float, 3>&)
		{
			s_typeStorage[name] = ValueTypes::Float3;
		};
		static void SetType(const std::string& name, const std::array<float, 4>&)
		{
			s_typeStorage[name] = ValueTypes::Float4;
		};
		static void SetType(const std::string& name, const bool)
		{
			s_typeStorage[name] = ValueTypes::Bool;
		};
		static void SetType(const std::string& name, const std::string&)
		{
			s_typeStorage[name] = ValueTypes::String;
		};
		static void SetType(const std::string& name, const std::vector<float>&)
		{
			s_typeStorage[name] = ValueTypes::FloatVect;
		};
		static void SetType(const std::string& name, const tm&)
		{
			s_typeStorage[name] = ValueTypes::Time;
		};
		static void SetType(const std::string& name, const ImPlotTime&)
		{
			s_typeStorage[name] = ValueTypes::Time;
		};

		static std::mutex& GetMutex()
		{
			return s_mutex;
		}

	private:
		mvValueStorage() = default;

		static std::mutex s_mutex;

		template<typename T> static std::unordered_map<std::string, T> s_value_store;

		template<> static std::unordered_map<std::string, int> s_value_store<int>;
		template<>
		static std::unordered_map<std::string, std::array<int, 2>>
			s_value_store<std::array<int, 2>>;
		template<>
		static std::unordered_map<std::string, std::array<int, 3>>
			s_value_store<std::array<int, 3>>;
		template<>
		static std::unordered_map<std::string, std::array<int, 4>>
			s_value_store<std::array<int, 4>>;
		template<>
		static std::unordered_map<std::string, float> s_value_store<float>;
		template<>
		static std::unordered_map<std::string, std::array<float, 2>>
			s_value_store<std::array<float, 2>>;
		template<>
		static std::unordered_map<std::string, std::array<float, 3>>
			s_value_store<std::array<float, 3>>;
		template<>
		static std::unordered_map<std::string, std::array<float, 4>>
			s_value_store<std::array<float, 4>>;
		template<> static std::unordered_map<std::string, bool> s_value_store<bool>;
		template<>
		static std::unordered_map<std::string, std::string>
			s_value_store<std::string>;
		template<>
		static std::unordered_map<std::string, std::vector<float>>
			s_value_store<std::vector<float>>;
		template<> static std::unordered_map<std::string, tm> s_value_store<tm>;
		template<>
		static std::unordered_map<std::string, ImPlotTime> s_value_store<ImPlotTime>;

		static std::unordered_map<std::string, ValueTypes>
			s_typeStorage; // keeps track of value mapping
		static std::unordered_map<std::string, int>
			s_refStorage;                           // keeps track of reference count
		static std::set<std::string> s_itemStorage; // keeps track of registered items
	};

} // namespace Marvel