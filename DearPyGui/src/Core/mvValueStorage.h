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
#include <map>
#include <set>
#include <array>
#include <vector>
#include <implot.h>
#include <implot_internal.h>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvValueStorage
	//-----------------------------------------------------------------------------
	class mvValueStorage
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

		// python interfacing
		static PyObject*           GetPyValue          (const std::string& name);
		static bool                SetPyValue          (const std::string& name, PyObject* value);
		static void                AddPyValue          (const std::string& name, PyObject* value);

		// adders
		static int*                AddIntValue         (const std::string& name, int value = 0);
		static int*                AddInt2Value        (const std::string& name, const std::array<int, 2>& value = {});
		static int*                AddInt3Value        (const std::string& name, const std::array<int, 3>& value = {});
		static int*                AddInt4Value        (const std::string& name, const std::array<int, 4>& value = {});
		static float*              AddFloatValue       (const std::string& name, float value = 0.0f);
		static float*              AddFloat2Value      (const std::string& name, const std::array<float, 2>& value = {});
		static float*              AddFloat3Value      (const std::string& name, const std::array<float, 3>& value = {});
		static float*              AddFloat4Value      (const std::string& name, const std::array<float, 4>& value = {});
		static float*              AddColorValue       (const std::string& name, const std::array<float, 4>& value = {});
		static std::vector<float>* AddFloatVectorValue (const std::string& name, const std::vector<float>& value = {});
		static bool*               AddBoolValue        (const std::string& name, bool value = false);
		static std::string*        AddStringValue      (const std::string& name, const std::string& value = "");
		static tm*                 AddTimeValue        (const std::string& name, const tm& value = {});

		// getters
		static int*                GetIntValue         (const std::string& name);
		static int*                GetInt2Value        (const std::string& name);
		static int*                GetInt3Value        (const std::string& name);
		static int*                GetInt4Value        (const std::string& name);
		static float*              GetFloatValue       (const std::string& name);
		static float*              GetFloat2Value      (const std::string& name);
		static float*              GetFloat3Value      (const std::string& name);
		static float*              GetFloat4Value      (const std::string& name);
		static std::vector<float>* GetFloatVectorValue (const std::string& name);
		static bool*               GetBoolValue        (const std::string& name);
		static std::string*        GetStringValue      (const std::string& name);
		static tm*                 GetTimeValue        (const std::string& name);
		static ImPlotTime*         GetImTimeValue      (const std::string& name);

		static bool                HasValue            (const std::string& name);
		static ValueTypes          GetType             (const std::string& name);
		static void                IncrementRef        (const std::string& name);
		static void                DecrementRef        (const std::string& name);
		static void                DeleteValue         (const std::string& name);

		

	private:

		mvValueStorage() = default;

		static std::map<std::string, ValueTypes>           s_typeStorage;  // keeps track of value mapping
		static std::map<std::string, int>                  s_refStorage;   // keeps track of reference count
		static std::set<std::string>                       s_itemStorage;  // keeps track of registered items

		// ints
		static std::map<std::string, int>                  s_ints; 
		static std::map<std::string, std::array<int, 2>>   s_int2s;
		static std::map<std::string, std::array<int, 3>>   s_int3s;
		static std::map<std::string, std::array<int, 4>>   s_int4s;

		// floats
		static std::map<std::string, float>                s_floats;  
		static std::map<std::string, std::array<float, 2>> s_float2s; 
		static std::map<std::string, std::array<float, 3>> s_float3s; 
		static std::map<std::string, std::array<float, 4>> s_float4s; 
		static std::map<std::string, std::vector<float>>   s_floatvects;

		// other
		static std::map<std::string, bool>                 s_bools;  
		static std::map<std::string, std::string>          s_strings;
		
		// time
		static std::map<std::string, tm>                   s_times;
		static std::map<std::string, ImPlotTime>           s_imtimes; 

	};

}