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
#include "mvEvents.h"
#include "implot.h"
#include "implot_internal.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

// forward declarations
struct ImPlotTime;

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvValueStorage
	//-----------------------------------------------------------------------------
	class mvValueStorage : public mvEventHandler
	{

		friend class mvDebugWindow;

		enum class ValueTypes 
		{
			None = 0,
			Int, Int2, Int3, Int4,
			Float, Float2, Float3, Float4, FloatVect,
			Bool,
			String,
			Time, Color
		};

	public:

		mvValueStorage();
		~mvValueStorage();

		bool onEvent(mvEvent& event) override;
		bool onPythonSetEvent(mvEvent& event);

		// python interfacing
		PyObject*           GetPyValue          (const std::string& name);
		bool                SetPyValue          (const std::string& name, PyObject* value);
		void                AddPyValue          (const std::string& name, PyObject* value);

		// adders
		int*                AddIntValue         (const std::string& name, int value = 0);
		int*                AddInt2Value        (const std::string& name, const std::array<int, 2>& value = {});
		int*                AddInt3Value        (const std::string& name, const std::array<int, 3>& value = {});
		int*                AddInt4Value        (const std::string& name, const std::array<int, 4>& value = {});
		float*              AddFloatValue       (const std::string& name, float value = 0.0f);
		float*              AddFloat2Value      (const std::string& name, const std::array<float, 2>& value = {});
		float*              AddFloat3Value      (const std::string& name, const std::array<float, 3>& value = {});
		float*              AddFloat4Value      (const std::string& name, const std::array<float, 4>& value = {});
		float*              AddColorValue       (const std::string& name, const std::array<float, 4>& value = {});
		std::vector<float>* AddFloatVectorValue (const std::string& name, const std::vector<float>& value = {});
		bool*               AddBoolValue        (const std::string& name, bool value = false);
		std::string*        AddStringValue      (const std::string& name, const std::string& value = "");
		tm*                 AddTimeValue        (const std::string& name, const tm& value = {});

		// getters
		int*                GetIntValue         (const std::string& name);
		int*                GetInt2Value        (const std::string& name);
		int*                GetInt3Value        (const std::string& name);
		int*                GetInt4Value        (const std::string& name);
		float*              GetFloatValue       (const std::string& name);
		float*              GetFloat2Value      (const std::string& name);
		float*              GetFloat3Value      (const std::string& name);
		float*              GetFloat4Value      (const std::string& name);
		std::vector<float>* GetFloatVectorValue (const std::string& name);
		bool*               GetBoolValue        (const std::string& name);
		std::string*        GetStringValue      (const std::string& name);
		tm*                 GetTimeValue        (const std::string& name);
		ImPlotTime*         GetImTimeValue      (const std::string& name);

		bool                HasValue            (const std::string& name);
		ValueTypes          GetType             (const std::string& name);
		void                IncrementRef        (const std::string& name);
		void                DecrementRef        (const std::string& name);
		void                DeleteValue         (const std::string& name);

		std::mutex& GetMutex() { return s_mutex; }

	private:

		std::mutex                                            s_mutex;

		std::unordered_map<std::string, ValueTypes>           s_typeStorage;  // keeps track of value mapping
		std::unordered_map<std::string, int>                  s_refStorage;   // keeps track of reference count
		std::set<std::string>                                 s_itemStorage;  // keeps track of registered items

		// ints
		std::unordered_map<std::string, int>                  s_ints; 
		std::unordered_map<std::string, std::array<int, 2>>   s_int2s;
		std::unordered_map<std::string, std::array<int, 3>>   s_int3s;
		std::unordered_map<std::string, std::array<int, 4>>   s_int4s;

		// floats
		std::unordered_map<std::string, float>                s_floats;  
		std::unordered_map<std::string, std::array<float, 2>> s_float2s; 
		std::unordered_map<std::string, std::array<float, 3>> s_float3s; 
		std::unordered_map<std::string, std::array<float, 4>> s_float4s; 
		std::unordered_map<std::string, std::vector<float>>   s_floatvects;

		// other
		std::unordered_map<std::string, bool>                 s_bools;  
		std::unordered_map<std::string, std::string>          s_strings;
		
		// time
		std::unordered_map<std::string, tm>                   s_times;
		std::unordered_map<std::string, ImPlotTime>           s_imtimes; 

	};

}